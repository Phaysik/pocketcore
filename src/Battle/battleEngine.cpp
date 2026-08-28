/*! @file battleEngine.cpp
	@brief Defines battle orchestration for fights between two Pokemon trainers.
	@date 08/21/2026
	@since 0.9.16
	@version 0.11.0
	@author Matthew Moore
*/

#include "Battle/battleEngine.h"

#include <algorithm>
#include <array>
#include <cstddef>
#include <expected>
#include <optional>
#include <span>
#include <utility>
#include <variant>
#include <vector>

#include "Ability/abilityID.h"
#include "Ability/abilityMeta.h"
#include "Battle/battleAction.h"
#include "Battle/battleHelpers.h"
#include "Battle/battleState.h"
#include "Battle/battleTargetsAndTriggers.h"
#include "Battle/battleValidation.h"
#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Effect/effectContext.h"
#include "Effect/effectID.h"
#include "Effect/effectMeta.h"
#include "Effect/effectSourceAndSuppresion.h"
#include "Effect/effectTrigger.h"
#include "Item/itemID.h"
#include "Item/itemMeta.h"
#include "Move/moveHitPolicy.h"
#include "Move/moveID.h"
#include "Move/moveMeta.h"
#include "Nature/natureID.h"
#include "Nature/natureMeta.h"
#include "Pokemon/pokemon.h"
#include "Status/statusID.h"
#include "Weather/weatherID.h"

namespace PocketCore::Battle
{
	using PocketCore::Ability::AbilityID;
	using PocketCore::Ability::AbilityMeta;
	using PocketCore::Ability::NO_ABILITY_ID;
	using PocketCore::Configuration::MAX_ABILITIES_PER_POKEMON;
	using PocketCore::Configuration::MAX_ITEMS_PER_POKEMON;
	using PocketCore::Configuration::MAX_NATURES_PER_POKEMON;
	using PocketCore::Configuration::MAX_STATUSES_PER_POKEMON;
	using PocketCore::Core::ub;
	using PocketCore::Effect::EffectID;
	using PocketCore::Effect::EffectMeta;
	using PocketCore::Effect::EffectSource;
	using PocketCore::Effect::EffectTrigger;
	using PocketCore::Item::ItemID;
	using PocketCore::Item::ItemMeta;
	using PocketCore::Item::NO_ITEM_ID;
	using PocketCore::Move::FixedHitCount;
	using PocketCore::Move::MoveID;
	using PocketCore::Move::MoveMeta;
	using PocketCore::Nature::NatureID;
	using PocketCore::Nature::NatureMeta;
	using PocketCore::Nature::NO_NATURE_ID;
	using PocketCore::Pokemon::Pokemon;
	using PocketCore::Status::StatusID;
	using PocketCore::Weather::WeatherID;

	ATTR_NODISCARD std::expected<void, BattleEngineError> BattleEngine::startBattle(const std::span<Pokemon *const> &partyA,
																					const std::span<Pokemon *const> &partyB,
																					const ub activePokemonPerSide)
	{
		// Starting twice would discard live battle state, so reject the request before validating new input
		if (mState.mBattleStarted)
		{
			return std::unexpected{BattleEngineError::BattleAlreadyStarted};
		}

		// Every metadata registry used during setup and trigger execution must be available.
		if (mProvider == nullptr || mEffectRegistry == nullptr || mProvider->abilityRegistry == nullptr
			|| mProvider->moveRegistry == nullptr || mProvider->itemRegistry == nullptr || mProvider->typeRegistry == nullptr
			|| mProvider->statusRegistry == nullptr || mProvider->weatherRegistry == nullptr || mProvider->terrainRegistry == nullptr
			|| mProvider->multiplierRegistry == nullptr)
		{
			return std::unexpected{BattleEngineError::MissingRegistry};
		}

		// Both parties must contain enough valid pointers to populate the requested active slots.
		if (activePokemonPerSide == 0U || partyA.size() < activePokemonPerSide || partyB.size() < activePokemonPerSide
			|| anyPartyPokemonNull(partyA) || anyPartyPokemonNull(partyB))
		{
			return std::unexpected{BattleEngineError::InvalidParty};
		}

		// Sorting one combined pointer list makes duplicate Pokemon pointers detectable both within and across parties.
		if (hasDuplicatePokemonPointers(partyA, partyB))
		{
			return std::unexpected{BattleEngineError::InvalidParty};
		}

		// Count usable party members independently from party size because fainted Pokemon cannot start active.
		if (healthyPokemonInParty(partyA) < activePokemonPerSide || healthyPokemonInParty(partyB) < activePokemonPerSide)
		{
			return std::unexpected{BattleEngineError::InvalidParty};
		}

		// Build locally so allocation failures cannot leave partially initialized engine state.
		BattleState newState{};
		newState.mPartyA.assign(partyA.begin(), partyA.end());
		newState.mPartyB.assign(partyB.begin(), partyB.end());
		newState.mSideA.reserve(activePokemonPerSide);
		newState.mSideB.reserve(activePokemonPerSide);

		// Walk the party in its declared order so the first eligible Pokemon fill the battlefield from slot zero onward.
		assignActiveSlots(partyA, newState.mSideA, activePokemonPerSide);
		assignActiveSlots(partyB, newState.mSideB, activePokemonPerSide);

		newState.mBattleStarted = true;

		// Commit the fully prepared state and begin accepting normal turn actions
		mState = std::move(newState);
		mPhase = BattlePhase::AwaitingActions;

		for (const Side side : std::array{Side::A, Side::B})
		{
			const std::vector<BattleSlot> &slots{activeSlots(mState, side)};

			for (std::size_t slotIndex{0}; slotIndex < slots.size(); ++slotIndex)
			{
				const BattleTarget owner{.mSide = side, .mSlotIndex = static_cast<ub>(slotIndex)};

				triggerSlot(owner, BattleEventID::BattleStart);
				triggerSlot(owner, BattleEventID::SwitchIn);
			}
		}

		// Startup effects may have caused faints, which can immediately require replacements or finish the battle.
		processFaints();

		return {};
	}

	ATTR_NODISCARD std::expected<void, BattleEngineError> BattleEngine::executeTurn(const std::span<const BattleAction> &actions)
	{
		// Reject invalid engine phases before allocating work buffers or inspecting actions
		if (!mState.mBattleStarted)
		{
			return std::unexpected{BattleEngineError::BattleNotStarted};
		}

		if (mPhase == BattlePhase::AwaitingReplacements)
		{
			return std::unexpected{BattleEngineError::ReplacementRequired};
		}

		if (getResult(mState) != BattleResult::InProgress)
		{
			return std::unexpected{BattleEngineError::BattleFinished};
		}

		// Track action owners and switch destinations while validating the complete turn atomically
		std::vector<BattleTarget> actors{};
		std::vector<Pokemon *> incomingPokemon{};
		actors.reserve(actions.size());
		incomingPokemon.reserve(actions.size());

		for (const BattleAction &action : actions)
		{
			const std::expected<void, BattleEngineError> validationResult{
				getValidationResult(mState, action, mPhase, mProvider->moveRegistry),
			};

			if (!validationResult.has_value())
			{
				return std::unexpected{validationResult.error()};
			}

			// Normalize both action types into the active slot that owns the action.
			const BattleTarget actor{getBattleTarget(action)};

			// Each active slot may contribute at most one action to a turn.
			if (std::ranges::find(actors, actor) != actors.end())
			{
				return std::unexpected{BattleEngineError::DuplicateAction};
			}

			actors.push_back(actor);

			const auto *switchAction{std::get_if<SwitchAction>(&action)};

			if (switchAction != nullptr)
			{
				Pokemon *incoming{party(mState, switchAction->mSide).at(switchAction->mPartyIndex)};

				if (std::ranges::find(incomingPokemon, incoming) != incomingPokemon.end())
				{
					return std::unexpected{BattleEngineError::DuplicateAction};
				}

				incomingPokemon.push_back(incoming);
			}
		}

		// Separate actions into execution groups because all switches resolve before any moves.
		std::vector<SwitchAction> switches{};
		std::vector<MoveAction> moves{};
		switches.reserve(actions.size());
		moves.reserve(actions.size());

		std::ranges::for_each(actions, [&switches, &moves](const BattleAction &action) {
			const auto *switchAction{std::get_if<SwitchAction>(&action)};

			if (switchAction != nullptr)
			{
				switches.push_back(*switchAction);
			}
			else
			{
				moves.push_back(std::get<MoveAction>(action));
			}
		});

		// Resolve simultaneous switches deterministically by side and then battlefield slot.
		std::ranges::sort(switches, [](const SwitchAction &left, const SwitchAction &right) {
			return std::pair{getSideOrder(left.mSide), left.mActiveSlotIndex}
				 < std::pair{getSideOrder(right.mSide), right.mActiveSlotIndex};
		});

		// Execute validated switches first so incoming Pokemon participate in subsequent move resolution.
		for (const SwitchAction &switchAction : switches)
		{
			const std::expected<void, BattleEngineError> switchResult{switchPokemon(switchAction)};

			if (!switchResult.has_value())
			{
				return std::unexpected{switchResult.error()};
			}
		}

		// Randomize exact move-order ties before applying the priority and speed ordering below.
		handleMovePrioritization(mState, moves, mProvider->moveRegistry);

		// Re-check volatile battle conditions because earlier actions may have fainted or disabled a later actor.
		std::ranges::for_each(moves, [this](const MoveAction &moveAction) {
			const std::vector<BattleSlot> &slots{activeSlots(mState, moveAction.mSide)};
			const BattleSlot userSlot{slots.at(moveAction.mUserSlotIndex)};

			if (moveAction.mUserSlotIndex < slots.size() && isHealthy(userSlot) && !userSlot.mIsFlinched && userSlot.mSleepCounter == 0U)
			{
				executeMove(moveAction);
			}
		});

		// Give each surviving active Pokemon one turn-end trigger in deterministic side-then-slot order.
		executeEndTurnTrigger();

		processFaints();

		return {};
	}

#if defined(ATTR_GCC) && !defined(ATTR_CLANG)
	// GCC suggests returns_nonnull for references even though the attribute accepts only pointer returns.
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wsuggest-attribute=returns_nonnull"
#endif

	ATTR_NODISCARD ATTR_CONST const BattleState &BattleEngine::getState() const noexcept
	{
		return mState;
	}

#if defined(ATTR_GCC) && !defined(ATTR_CLANG)
	#pragma GCC diagnostic pop
#endif

	ATTR_NODISCARD std::expected<void, BattleEngineError> BattleEngine::switchPokemon(const SwitchAction &action)
	{
		// During forced replacement, only slots listed as vacant/fainted may be replaced.
		if (mPhase == BattlePhase::AwaitingReplacements
			&& std::ranges::find(mRequiredReplacements, BattleTarget{.mSide = action.mSide, .mSlotIndex = action.mActiveSlotIndex})
				   == mRequiredReplacements.end())
		{
			return std::unexpected{BattleEngineError::ReplacementRequired};
		}

		// Reuse normal switch validation for both voluntary switches and forced replacements.
		const std::expected<void, BattleEngineError> validationResult{validateSwitchAction(mState, action)};

		if (!validationResult.has_value())
		{
			return std::unexpected{validationResult.error()};
		}

		// Preserve the battlefield position while resetting all occupant-specific slot state.
		std::vector<BattleSlot> &slots{activeSlots(mState, action.mSide)};
		BattleSlot &slot{slots.at(action.mActiveSlotIndex)};
		const ub position{slot.mPosition};

		std::vector<Pokemon *> &trainerParty{party(mState, action.mSide)};
		slot = BattleSlot{.mPokemon = trainerParty.at(action.mPartyIndex), .mPosition = position};

		const BattleTarget battleTarget{.mSide = action.mSide, .mSlotIndex = action.mActiveSlotIndex};

		// The new occupant can react immediately through ability and item switch-in triggers.
		triggerSlot(battleTarget, BattleEventID::SwitchIn);

		// Switch-in effects may faint Pokemon or satisfy the final outstanding replacement.
		processFaints();

		return {};
	}

	ATTR_NODISCARD ATTR_PURE bool BattleEngine::isSuppressed(const EffectSource source, const BattleTarget &owner,
															 const BattleEventID eventID, const BattleEventRole role,
															 const EffectContext &context) const noexcept
	{
		// A trigger is suppressed when any active rule matches its source, trigger, and optional metadata ID.
		return std::ranges::any_of(mActiveSuppressions, [source, &owner, eventID, role, &context](const ActiveSuppression &suppression) {
			const SuppressionRule &rule{suppression.mRule};

			// A source never suppresses its own trigger on the slot that established the rule.
			if (suppression.mSource == source && suppression.mOwner == owner)
			{
				return false;
			}

			// Rules for a different source category or trigger cannot affect this dispatch.
			if (rule.mTargetSource != source || rule.mTargetTrigger != eventID
				|| (rule.mTargetRole != BattleEventRole::Any && rule.mTargetRole != role))
			{
				return false;
			}

			// Metadata-bearing sources may be filtered to one specific ability, item, or move ID.
			switch (source)
			{
				case EffectSource::Ability:
					return !rule.mTargetAbilityID.has_value() || rule.mTargetAbilityID == context.mAbilityID;
				case EffectSource::Item:
					return !rule.mTargetItemID.has_value() || rule.mTargetItemID == context.mItemID;
				case EffectSource::Move:
					return !rule.mTargetMoveID.has_value() || rule.mTargetMoveID == context.mMoveID;
				case EffectSource::Nature:
					return !rule.mTargetNatureID.has_value() || rule.mTargetNatureID == context.mNatureID;
				case EffectSource::None:
					ATTR_FALLTHROUGH;
				case EffectSource::Hazard:
					return true;
				default:
					return false;
			}
		});
	}

	void BattleEngine::activateSuppressions(const std::span<const SuppressionRule> &suppressionRules, const ub suppressionRuleCount,
											const EffectSource source, const BattleTarget &owner)
	{
		// Metadata carries a logical count; clamp it to the physical span before slicing.
		const std::size_t activeRuleCount{std::min(static_cast<std::size_t>(suppressionRuleCount), suppressionRules.size())};
		const std::span<const SuppressionRule> activeRules{suppressionRules.first(activeRuleCount)};

		// Record ownership and source alongside each rule so self-suppression can be excluded later.
		std::ranges::for_each(activeRules, [this, &owner, source](const SuppressionRule &rule) {
			mActiveSuppressions.push_back({.mRule = rule, .mOwner = owner, .mSource = source});
		});
	}

	void BattleEngine::activateAbilitySuppressions(const AbilityMeta &abilityMeta, const BattleTarget &owner, const BattleEventID eventID,
												   const BattleEventRole role)
	{
		// Only suppression rules attached to the trigger currently being dispatched become active.
		std::ranges::for_each(abilityMeta.mTriggers, [this, eventID, role, &owner](const EffectTrigger &trigger) {
			if (trigger.mTrigger == eventID && (trigger.mRole == BattleEventRole::Any || trigger.mRole == role))
			{
				activateSuppressions(trigger.mSuppressionRules, trigger.mSuppresionRuleCount, EffectSource::Ability, owner);
			}
		});
	}

	void BattleEngine::activateItemSuppressions(const ItemMeta &itemMeta, const BattleTarget &owner, const BattleEventID eventID,
												const BattleEventRole role)
	{
		// Item suppression activation mirrors ability activation to preserve source-specific ownership.
		std::ranges::for_each(itemMeta.mTriggers, [this, eventID, role, &owner](const EffectTrigger &trigger) {
			if (trigger.mTrigger == eventID && (trigger.mRole == BattleEventRole::Any || trigger.mRole == role))
			{
				activateSuppressions(trigger.mSuppressionRules, trigger.mSuppresionRuleCount, EffectSource::Item, owner);
			}
		});
	}

	void BattleEngine::activateNatureSuppressions(const NatureMeta &natureMeta, const BattleTarget &owner, const BattleEventID eventID,
												  const BattleEventRole role)
	{
		// Nature suppression activation mirrors ability activation to preserve source-specific ownership.
		std::ranges::for_each(natureMeta.mTriggers, [this, eventID, role, &owner](const EffectTrigger &trigger) {
			if (trigger.mTrigger == eventID && (trigger.mRole == BattleEventRole::Any || trigger.mRole == role))
			{
				activateSuppressions(trigger.mSuppressionRules, trigger.mSuppresionRuleCount, EffectSource::Nature, owner);
			}
		});
	}

	void BattleEngine::executeEffect(const EffectID effect, EffectContext &context)
	{
		// Resolve the open identifier to metadata, then invoke its registered runtime function.
		const EffectMeta *effectMeta{mEffectRegistry->getEffectMetadata(effect)};

		if (effectMeta == nullptr || effectMeta->mApply == nullptr)
		{
			return;
		}

		const bool mayChangeWeather{effectMeta->mMayChangeWeather};
		const bool mayChangeStatus{effectMeta->mMayChangeStatus};
		const WeatherID previousWeatherID{mState.mWeatherID};

		Pokemon *statusTarget{nullptr};

		if (mayChangeStatus)
		{
			const BattleSlot *targetSlot{contextSlot(mState, context.mTargetSide, context.mTargetIndex)};
			statusTarget = targetSlot != nullptr ? targetSlot->mPokemon : nullptr;
		}

		const std::array<StatusID, MAX_STATUSES_PER_POKEMON> previousStatuses{
			statusTarget != nullptr ? statusTarget->getStatusesArray() : decltype(statusTarget->getStatusesArray()){}};

		effectMeta->mApply(mState, context, *mProvider);

		if (mayChangeWeather && mState.mWeatherID != previousWeatherID)
		{
			std::ranges::for_each(std::array{Side::A, Side::B}, [this, &context](const Side side) {
				const std::vector<BattleSlot> &slots{activeSlots(mState, side)};

				for (std::size_t slotIndex{0}; slotIndex < slots.size(); ++slotIndex)
				{
					if (isHealthy(slots.at(slotIndex)))
					{
						triggerSlotInContext(BattleTarget{.mSide = side, .mSlotIndex = static_cast<ub>(slotIndex)},
											 BattleEventID::WeatherChanged, context, BattleEventRole::Any);
					}
				}
			});
		}

		if (statusTarget != nullptr && statusTarget->getStatusesArray() != previousStatuses)
		{
			const BattleTarget target{.mSide = context.mTargetSide, .mSlotIndex = context.mTargetIndex};
			triggerSlotInContext(target, BattleEventID::StatusChanged, context, BattleEventRole::Target);
		}
	}

	void BattleEngine::executeEffects(const std::span<const EffectID> &effects, EffectContext &context)
	{
		// Effects execute in metadata order and share one mutable context.
		std::ranges::for_each(effects, [this, &context](const EffectID effect) {
			if (!context.mDamage.mShouldContinue)
			{
				return;
			}

			executeEffect(effect, context);
		});

		// Resolve effect-caused faints after completing or cancelling the chain.
		processFaints();
	}

	void BattleEngine::executeTargetedEffects(const BattleTarget &owner, const BattleTargetID targetID,
											  const std::span<const EffectID> &effects, EffectContext &context)
	{
		// Save event coordinates between target iteration temporarily rewrites both user and target fields.
		const Side previousUserSide{context.mUserSide};
		const Side previousTargetSide{context.mTargetSide};
		const ub previousUserIndex{context.mUserIndex};
		const ub previousTargetIndex{context.mTargetIndex};
		const BattleTarget eventTarget{.mSide = previousTargetSide, .mSlotIndex = previousTargetIndex};

		// Reuse an opposin event target as the explicit selection for single-opponent selectors when possible.
		const std::optional<BattleTarget> selectedTarget{
			eventTarget.mSide != owner.mSide && targetExists(mState, eventTarget) ? std::optional{eventTarget} : std::nullopt,
		};

		// Ability and item effects are not constrained by move formation range.
		const auto targets{resolveTargets(mState, owner.mSide, owner.mSlotIndex, targetID, BattleRangeID::Unrestricted, selectedTarget)};

		if (targets.has_value())
		{
			std::ranges::for_each(targets.value(), [this, &effects, &context, owner](const BattleTarget target) {
				context.mUserSide = owner.mSide;
				context.mUserIndex = owner.mSlotIndex;
				context.mTargetSide = target.mSide;
				context.mTargetIndex = target.mSlotIndex;

				executeEffects(effects, context);
			});
		}

		// Restore event coordinate so sibling triggers receive the original context unchanged.
		context.mUserSide = previousUserSide;
		context.mUserIndex = previousUserIndex;
		context.mTargetSide = previousTargetSide;
		context.mTargetIndex = previousTargetIndex;
	}

	void BattleEngine::executeMove(const MoveAction &action)
	{
		// Recove the actor and move metadata from the already validated action
		const BattleSlot &userSlot{activeSlots(mState, action.mSide).at(action.mUserSlotIndex)};
		Pokemon *userPokemon{userSlot.mPokemon};

		const MoveID moveID{userPokemon->getMoveID(action.mMoveSlotIndex)};
		const MoveMeta *moveMeta{mProvider->moveRegistry->getMoveMetadata(moveID)};

		if (moveMeta == nullptr)
		{
			// Metadata may have changed since validation. Fail will close without consuming PP.
			return;
		}

		// Resolve targets again after switches and earlier moves may have changed the battlefield.
		const std::expected<std::vector<BattleTarget>, BattleEngineError> targetsResult{
			getMoveTargets(mState, action, mProvider->moveRegistry),
		};

		if (!targetsResult.has_value())
		{
			return;
		}

		// Suppressions are scoped to this move execution and rebuilt by each relevant event.
		mActiveSuppressions.clear();

		userPokemon->usePP(action.mMoveSlotIndex);
		const ub hitCount{resolveHitCount(*moveMeta)};
		const BattleTarget user{.mSide = action.mSide, .mSlotIndex = action.mUserSlotIndex};
		EffectContext moveContext{makeMoveContext(action, *moveMeta, targetsResult->front(), 0U)};

		// The move user and the move itself receive the user-side move-use event once.
		triggerSlotInContext(user, BattleEventID::MoveUse, moveContext, BattleEventRole::User);
		executeMoveTrigger(*moveMeta, BattleEventID::MoveUse, BattleEventRole::User, moveContext);

		if (!moveContext.mDamage.mShouldContinue)
		{
			mActiveSuppressions.clear();
			return;
		}

		// Keep move-wide suppression rules as the baseline restored before processing each target.
		const std::size_t moveSuppressionCount{mActiveSuppressions.size()};

		for (const BattleTarget target : targetsResult.value())
		{
			mActiveSuppressions.resize(moveSuppressionCount);

			if (!targetExists(mState, target))
			{
				continue;
			}

			EffectContext targetContext{moveContext};
			targetContext.mTargetSide = target.mSide;
			targetContext.mTargetIndex = target.mSlotIndex;

			// The current recipient observes the target side move-use event.
			triggerSlotInContext(target, BattleEventID::MoveUse, targetContext, BattleEventRole::Target);

			std::size_t targetSuppressionCount{mActiveSuppressions.size()};
			const bool runBeforeHitPerAttempt{std::holds_alternative<FixedHitCount>(moveMeta->mHitCountPolicy)};

			if (executeWeightHitCountPolicy(runBeforeHitPerAttempt, targetContext, moveMeta, targetSuppressionCount))
			{
				continue;
			}

			for (ub hitAttempt{0}; hitAttempt < hitCount && targetExists(mState, target) && targetContext.mDamage.mShouldContinue;
				 ++hitAttempt)
			{
				mActiveSuppressions.resize(targetSuppressionCount);
				EffectContext context{targetContext};
				context.mHitAttemptIndex = static_cast<ub>(hitAttempt + 1U);

				if (executeFixedHitCountPolicy(runBeforeHitPerAttempt, context, moveMeta))
				{
					break;
				}

				// Damage modifiers are dispatched once from each participant's perspective.
				triggerSlotInContext(user, BattleEventID::DamageCalculation, context, BattleEventRole::User);
				triggerSlotInContext(target, BattleEventID::DamageCalculation, context, BattleEventRole::Target);

				executeMoveTrigger(*moveMeta, BattleEventID::Hit, BattleEventRole::Target, context);

				executeDamageApplication(context, target);

				applyRecoil(mState, context);
				processFaints();

				if (!context.mDamage.mIsMiss)
				{
					// User-owned effects such as Stench react to a successful hit on the current target.
					triggerSlotInContext(user, BattleEventID::Hit, context, BattleEventRole::User);
					executeMoveTrigger(*moveMeta, BattleEventID::AfterHit, BattleEventRole::Target, context);
				}

				if (!context.mDamage.mShouldContinue)
				{
					break;
				}

				targetContext = context;
				targetContext.mDamage = {};
				targetContext.resetMultipliers();
			}
		}

		mActiveSuppressions.clear();
	}

	ATTR_NODISCARD bool BattleEngine::executeWeightHitCountPolicy(const bool runBeforeHitPerAttempt, EffectContext &targetContext,
																  const MoveMeta *moveMeta, std::size_t &targetSuppressionCount)
	{
		if (runBeforeHitPerAttempt)
		{
			return false;
		}

		targetContext.mHitAttemptIndex = 1U;
		executeMoveTrigger(*moveMeta, BattleEventID::BeforeHit, BattleEventRole::Target, targetContext);

		if (targetContext.mDamage.mIsMiss || !targetContext.mDamage.mShouldContinue)
		{
			return true;
		}

		targetSuppressionCount = mActiveSuppressions.size();

		return false;
	}

	ATTR_NODISCARD bool BattleEngine::executeFixedHitCountPolicy(const bool runBeforeHitPerAttempt, EffectContext &context,
																 const MoveMeta *moveMeta)
	{
		if (runBeforeHitPerAttempt)
		{
			executeMoveTrigger(*moveMeta, BattleEventID::BeforeHit, BattleEventRole::Target, context);

			if (context.mDamage.mIsMiss)
			{
				return true;
			}
		}

		return false;
	}

	void BattleEngine::executeDamageApplication(const EffectContext &context, const BattleTarget &target)
	{
		if (context.mDamage.mShouldApplyDamage && context.mDamage.mDamage > 0U)
		{
			Pokemon *targetPokemon{activeSlots(mState, target.mSide).at(target.mSlotIndex).mPokemon};
			const us damage{context.applyMultiplier(context.mDamage.mDamage, *mProvider->multiplierRegistry)};
			const us remainingHealth{
				damage >= targetPokemon->getHealth() ? static_cast<us>(0) : static_cast<us>(targetPokemon->getHealth() - damage),
			};

			targetPokemon->setHealth(remainingHealth);
		}
	}

	void BattleEngine::executeMoveTrigger(const MoveMeta &moveMeta, const BattleEventID eventID, const BattleEventRole role,
										  EffectContext &context)
	{
		// Save the caller's source because nested trigger dispatch reuses the same context.
		const EffectSource previousSource{context.mSourceType};
		const BattleTarget owner{.mSide = context.mUserSide, .mSlotIndex = context.mUserIndex};
		context.mSourceType = EffectSource::Move;

		// Execute every metadata entry matching this move trigger in declaration order.
		std::ranges::for_each(moveMeta.mTriggers, [this, eventID, role, &owner, &context](const EffectTrigger &trigger) {
			if (trigger.mTrigger == eventID && (trigger.mRole == BattleEventRole::Any || trigger.mRole == role))
			{
				// A trigger's suppression rules become visible before deciding whether its effects are suppressed.
				activateSuppressions(trigger.mSuppressionRules, trigger.mSuppresionRuleCount, EffectSource::Move, owner);

				if (!isSuppressed(EffectSource::Move, owner, eventID, role, context))
				{
					executeEffects(trigger.mEffects, context);
				}
			}
		});

		// Restore the outer source so subsequent dispatch observes the context it received.
		context.mSourceType = previousSource;
	}

	void BattleEngine::executeAbilityTrigger(const BattleTarget &owner, const AbilityMeta &abilityMeta, const BattleEventID eventID,
											 const BattleEventRole role, EffectContext &context, const bool targetEffects)
	{
		// Stamp source identity into the shared context for suppression matching and effect behavior.
		context.mAbilityID = abilityMeta.mAbilityID;
		context.mSourceType = EffectSource::Ability;

		// Dispatch matching, unsuppressed trigger entries in metadata order.
		for (const EffectTrigger &trigger : abilityMeta.mTriggers)
		{
			if (trigger.mTrigger != eventID || (trigger.mRole != BattleEventRole::Any && trigger.mRole != role)
				|| isSuppressed(EffectSource::Ability, owner, eventID, role, context))
			{
				continue;
			}

			if (targetEffects)
			{
				// Normal slot triggers resolve the ability's declared target selector.
				executeTargetedEffects(owner, abilityMeta.mTargetID, trigger.mEffects, context);
			}
			else
			{
				// Faint handling preserves its prebuilt event context and executes effects directly.
				executeEffects(trigger.mEffects, context);
			}
		}
	}

	void BattleEngine::executeItemTrigger(const BattleTarget &owner, const ItemMeta &itemMeta, const BattleEventID eventID,
										  const BattleEventRole role, EffectContext &context, const bool targetEffects)
	{
		// Stamp source identity into the shared context for suppression matching and effect behavior.
		context.mItemID = itemMeta.mItemID;
		context.mSourceType = EffectSource::Item;

		// Dispatch matching, unsuppressed trigger entries in metadata order.
		for (const EffectTrigger &trigger : itemMeta.mTriggers)
		{
			if (trigger.mTrigger != eventID || (trigger.mRole != BattleEventRole::Any && trigger.mRole != role)
				|| isSuppressed(EffectSource::Item, owner, eventID, role, context))
			{
				continue;
			}

			if (targetEffects)
			{
				// Normal slot triggers resolve the ability's declared target selector.
				executeTargetedEffects(owner, itemMeta.mTargetID, trigger.mEffects, context);
			}
			else
			{
				// Faint handling preserves its prebuilt event context and executes effects directly.
				executeEffects(trigger.mEffects, context);
			}
		}
	}

	void BattleEngine::executeNatureTrigger(const BattleTarget &owner, const NatureMeta &natureMeta, const BattleEventID eventID,
											const BattleEventRole role, EffectContext &context, const bool targetEffects)
	{
		// Stamp source identity into the shared context for suppression matching and effect behavior.
		context.mNatureID = natureMeta.mNatureID;
		context.mSourceType = EffectSource::Nature;

		// Dispatch matching, unsuppressed trigger entries in metadata order.
		for (const EffectTrigger &trigger : natureMeta.mTriggers)
		{
			if (trigger.mTrigger != eventID || (trigger.mRole != BattleEventRole::Any && trigger.mRole != role)
				|| isSuppressed(EffectSource::Nature, owner, eventID, role, context))
			{
				continue;
			}

			if (targetEffects)
			{
				// Normal slot triggers resolve the ability's declared target selector.
				executeTargetedEffects(owner, natureMeta.mTargetID, trigger.mEffects, context);
			}
			else
			{
				// Faint handling preserves its prebuilt event context and executes effects directly.
				executeEffects(trigger.mEffects, context);
			}
		}
	}

	void BattleEngine::executeEndTurnTrigger()
	{
		for (const Side side : std::array{Side::A, Side::B})
		{
			const std::vector<BattleSlot> &slots{activeSlots(mState, side)};

			for (std::size_t slotIndex{0}; slotIndex < slots.size(); ++slotIndex)
			{
				if (isHealthy(slots.at(slotIndex)))
				{
					triggerSlot(BattleTarget{.mSide = side, .mSlotIndex = static_cast<ub>(slotIndex)}, BattleEventID::TurnEnd);
				}
			}
		}
	}

	void BattleEngine::triggerSlot(const BattleTarget &owner, const BattleEventID eventID, const std::optional<BattleTarget> &eventTarget,
								   const BattleEventRole role)
	{
		// Standalone slot triggers being with a fresh suppression scope
		mActiveSuppressions.clear();

		if (!targetExists(mState, owner))
		{
			return;
		}

		// Build a minimal context addressed from the owner to the optional event target.
		EffectContext context{};
		context.mUserSide = owner.mSide;
		context.mUserIndex = owner.mSlotIndex;
		context.mTargetSide = eventTarget.has_value() ? eventTarget->mSide : owner.mSide;
		context.mTargetIndex = eventTarget.has_value() ? eventTarget->mSlotIndex : owner.mSlotIndex;

		// Dispatch ability and item hooks, then end the standalone suppression scope.
		triggerSlotInContext(owner, eventID, context, role);
		mActiveSuppressions.clear();
	}

	void BattleEngine::triggerSlotInContext(const BattleTarget &owner, const BattleEventID eventID, EffectContext &context,
											const BattleEventRole role)
	{
		// Ignore stale targets that were removed by an earlier nested effect.
		if (!targetExists(mState, owner))
		{
			return;
		}

		// Resolve the current occupant's ability and item metadata, allowin either source to be absent.
		Pokemon *pokemon{activeSlots(mState, owner.mSide).at(owner.mSlotIndex).mPokemon};
		dispatchSlotSources(owner, pokemon, eventID, role, context, SlotTriggerTargeting::ResolveMetadataTargets);
	}

	void BattleEngine::dispatchSlotSources(const BattleTarget &owner, const Pokemon *pokemon, const BattleEventID eventID,
										   const BattleEventRole role, EffectContext &context, const SlotTriggerTargeting targeting)
	{
		// Resolve the occupant's ability and item metadata, allowing either source to be absent.
		// Preserve the outer source across nested ability and item trigger dispatch.
		const EffectSource previousSource{context.mSourceType};

		const std::array<AbilityID, MAX_ABILITIES_PER_POKEMON> &abilityIDs{pokemon->getAbilitiesArray()};
		std::array<const AbilityMeta *, MAX_ABILITIES_PER_POKEMON> abilityMetas{};
		std::ranges::transform(abilityIDs, abilityMetas.begin(), [this](const AbilityID abilityID) {
			return abilityID != NO_ABILITY_ID ? mProvider->abilityRegistry->getAbilityMetadata(abilityID) : nullptr;
		});

		const std::array<ItemID, MAX_ITEMS_PER_POKEMON> itemIDs{pokemon->getItemsArray()};
		std::array<const ItemMeta *, MAX_ITEMS_PER_POKEMON> itemMetas{};
		std::ranges::transform(itemIDs, itemMetas.begin(), [this](const ItemID itemID) {
			return itemID != NO_ITEM_ID ? mProvider->itemRegistry->getItemMetadata(itemID) : nullptr;
		});

		const std::array<NatureID, MAX_NATURES_PER_POKEMON> natureIDs{pokemon->getNatureIDsArray()};
		std::array<const NatureMeta *, MAX_NATURES_PER_POKEMON> natureMetas{};
		std::ranges::transform(natureIDs, natureMetas.begin(), [this](const NatureID natureID) {
			return natureID != NO_NATURE_ID ? mProvider->natureRegistry->getNatureMetadata(natureID) : nullptr;
		});

		// Activate all relevant suppression rules before executing either source's effects.
		std::ranges::for_each(abilityMetas, [this, &owner, eventID, role](const AbilityMeta *abilityMeta) {
			if (abilityMeta != nullptr)
			{
				activateAbilitySuppressions(*abilityMeta, owner, eventID, role);
			}
		});

		std::ranges::for_each(itemMetas, [this, &owner, eventID, role](const ItemMeta *itemMeta) {
			if (itemMeta != nullptr)
			{
				activateItemSuppressions(*itemMeta, owner, eventID, role);
			}
		});

		std::ranges::for_each(natureMetas, [this, &owner, eventID, role](const NatureMeta *natureMeta) {
			if (natureMeta != nullptr)
			{
				activateNatureSuppressions(*natureMeta, owner, eventID, role);
			}
		});

		// Ability effects execute before item effects for the same slot and trigger.
		const bool targetEffects{targeting == SlotTriggerTargeting::ResolveMetadataTargets};

		std::ranges::for_each(abilityMetas, [this, &owner, eventID, role, &context, targetEffects](const AbilityMeta *abilityMeta) {
			if (abilityMeta != nullptr)
			{
				executeAbilityTrigger(owner, *abilityMeta, eventID, role, context, targetEffects);
			}
		});

		std::ranges::for_each(itemMetas, [this, &owner, eventID, role, &context, targetEffects](const ItemMeta *itemMeta) {
			if (itemMeta != nullptr)
			{
				executeItemTrigger(owner, *itemMeta, eventID, role, context, targetEffects);
			}
		});

		std::ranges::for_each(natureMetas, [this, &owner, eventID, role, &context, targetEffects](const NatureMeta *natureMeta) {
			if (natureMeta != nullptr)
			{
				executeNatureTrigger(owner, *natureMeta, eventID, role, context, targetEffects);
			}
		});

		// Return source ownership to the caller's trigger context.
		context.mSourceType = previousSource;
	}

	void BattleEngine::processFaints()
	{
		// Scan every active slot in deterministic order for newly fainted occupants.
		for (const Side side : std::array{Side::A, Side::B})
		{
			std::vector<BattleSlot> &slots{activeSlots(mState, side)};

			for (std::size_t slotIndex{0}; slotIndex < slots.size(); ++slotIndex)
			{
				BattleSlot &slot{slots.at(slotIndex)};

				// Ignore empty, healthy, and previously processed slots to fire OnFaint exactly once per occupant.
				if (slot.mPokemon == nullptr || !slot.mPokemon->isFainted() || slot.mFaintProcessed)
				{
					continue;
				}

				// Mark before dispatch so recursive effect processing cannot trigger the same faint twice.
				slot.mFaintProcessed = true;
				triggerFaint(BattleTarget{.mSide = side, .mSlotIndex = static_cast<ub>(slotIndex)});
			}
		}

		// Newly processed faints may require replacements or determine the battle result.
		refreshBattlePhase();
	}

	void BattleEngine::refreshBattlePhase()
	{
		// Recompute replacement requirements from current state instead of incrementally maintaining them.
		mRequiredReplacements.clear();

		if (!mState.mBattleStarted)
		{
			// An unstarted battle accepts neither turn actions nor replacements.
			mPhase = BattlePhase::NotStarted;
			return;
		}

		if (getResult(mState) != BattleResult::InProgress)
		{
			// A decided battle remains finished even if an active slot is fainted.
			mPhase = BattlePhase::Finished;
			return;
		}

		for (const Side side : std::array{Side::A, Side::B})
		{
			const std::vector<BattleSlot> &slots{activeSlots(mState, side)};
			const std::vector<Pokemon *> &trainerParty{party(mState, side)};

			if (!hasReserve(mState, side, trainerParty))
			{
				// No legal switch exists for this side, so do not request impossible replacement input.
				return;
			}

			// Record every unhealthy active slot that can be filled from the reserve party.
			for (std::size_t slotIndex{0}; slotIndex < slots.size(); ++slotIndex)
			{
				if (!isHealthy(slots.at(slotIndex)))
				{
					mRequiredReplacements.push_back({.mSide = side, .mSlotIndex = static_cast<ub>(slotIndex)});
				}
			}
		}

		// Normal actions resume only after all required replacements slots have been filled.
		mPhase = mRequiredReplacements.empty() ? BattlePhase::AwaitingActions : BattlePhase::AwaitingReplacements;
	}

	void BattleEngine::triggerFaint(const BattleTarget faintedTarget)
	{
		// The side may be valid while the slot index is stale, so guard it before access.
		if (faintedTarget.mSlotIndex >= activeSlots(mState, faintedTarget.mSide).size())
		{
			return;
		}

		// A faint event initially addresses the fainted Pokemon as both user and target.
		EffectContext context{};
		context.mUserSide = faintedTarget.mSide;
		context.mTargetSide = faintedTarget.mSide;
		context.mUserIndex = faintedTarget.mSlotIndex;
		context.mTargetIndex = faintedTarget.mSlotIndex;

		Pokemon *faintedPokemon{activeSlots(mState, faintedTarget.mSide).at(faintedTarget.mSlotIndex).mPokemon};

		// Empty slots have no metadata sources to notify.

		if (faintedPokemon == nullptr)
		{
			return;
		}

		// Faint effects preserve the event coordinates instead of resolving metadata target selectors.
		dispatchSlotSources(faintedTarget, faintedPokemon, BattleEventID::Faint, BattleEventRole::Any, context,
							SlotTriggerTargeting::PreserveContextTarget);
	}

} // namespace PocketCore::Battle
