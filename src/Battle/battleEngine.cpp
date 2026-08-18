/*! @file battleEngine.cpp
	@brief Defines battle orchestration for fights between two Pokemon trainers.
	@date 08/05/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#include "Battle/battleEngine.h"

#include <algorithm>
#include <expected>
#include <optional>
#include <span>
#include <utility>
#include <vector>

#include "Ability/abilityID.h"
#include "Ability/abilityMeta.h"
#include "Battle/battleAction.h"
#include "Battle/battleHelpers.h"
#include "Battle/battleTargetsAndTriggers.h"
#include "Battle/battleValidation.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Effect/builtInEffectID.h"
#include "Effect/effectMeta.h"
#include "Effect/effectSourceAndSuppresion.h"
#include "Item/itemID.h"
#include "Item/itemMeta.h"
#include "Move/moveID.h"
#include "Move/moveMeta.h"
#include "Pokemon/pokemon.h"

namespace PocketCore::Battle
{
	using PocketCore::Ability::AbilityEffectTrigger;
	using PocketCore::Ability::AbilityID;
	using PocketCore::Ability::AbilityMeta;
	using PocketCore::Ability::NO_ABILITY_ID;
	using PocketCore::Core::ub;
	using PocketCore::Effect::BuiltinEffectID;
	using PocketCore::Effect::EffectMeta;
	using PocketCore::Effect::EffectSource;
	using PocketCore::Effect::toEffectID;
	using PocketCore::Item::ItemEffectTrigger;
	using PocketCore::Item::ItemID;
	using PocketCore::Item::ItemMeta;
	using PocketCore::Item::NO_ITEM_ID;
	using PocketCore::Move::MoveEffectTrigger;
	using PocketCore::Move::MoveID;
	using PocketCore::Move::MoveMeta;
	using PocketCore::Pokemon::Pokemon;

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

				triggerSlot(owner, BattleTriggerID::OnBattleStart);
				triggerSlot(owner, BattleTriggerID::OnSwitchIn);
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
		triggerSlot(battleTarget, BattleTriggerID::OnSwitchIn);
		triggerSlot(battleTarget, BattleTriggerID::OnHazardSwitchIn);

		// Switch-in effects may faint Pokemon or satisfy the final outstanding replacement.
		processFaints();

		return {};
	}

	ATTR_NODISCARD ATTR_PURE bool BattleEngine::isSuppressed(const EffectSource source, const BattleTarget &owner,
															 const BattleTriggerID triggerID, const EffectContext &context) const noexcept
	{
		// A trigger is suppressed when any active rule matches its source, trigger, and optional metadata ID.
		return std::ranges::any_of(mActiveSuppressions, [source, &owner, triggerID, &context](const ActiveSuppression &suppression) {
			const SuppressionRule &rule{suppression.mRule};

			// A source never suppresses its own trigger on the slot that established the rule.
			if (suppression.mSource == source && suppression.mOwner == owner)
			{
				return false;
			}

			// Rules for a different source category or trigger cannot affect this dispatch.
			if (rule.mTargetSource != source || rule.mTargetTrigger != triggerID)
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

	void BattleEngine::activateAbilitySuppressions(const AbilityMeta &abilityMeta, const BattleTarget &owner,
												   const BattleTriggerID triggerID)
	{
		// Only suppression rules attached to the trigger currently being dispatched become active.
		std::ranges::for_each(abilityMeta.mTriggers, [this, triggerID, &owner](const AbilityEffectTrigger &trigger) {
			if (trigger.mTrigger == triggerID)
			{
				activateSuppressions(trigger.mSuppressionRules, trigger.mSuppresionRuleCount, EffectSource::Ability, owner);
			}
		});
	}

	void BattleEngine::activateItemSuppressions(const ItemMeta &itemMeta, const BattleTarget &owner, const BattleTriggerID triggerID)
	{
		// Item suppression activation mirrors ability activation to preserve source-specific ownership.
		std::ranges::for_each(itemMeta.mTriggers, [this, triggerID, &owner](const ItemEffectTrigger &trigger) {
			if (trigger.mTrigger == triggerID)
			{
				activateSuppressions(trigger.mSuppressionRules, trigger.mSuppresionRuleCount, EffectSource::Item, owner);
			}
		});
	}

	void BattleEngine::executeEffect(const BuiltinEffectID effect, EffectContext &context)
	{
		// Resolve the built-in identifier to metadata, then invoke its registered runtime function.
		const EffectMeta *effectMeta{mEffectRegistry->getEffectMetadata(toEffectID(effect))};

		if (effectMeta != nullptr && effectMeta->mApply != nullptr)
		{
			effectMeta->mApply(mState, context, *mProvider);
		}
	}

	void BattleEngine::executeEffects(const std::span<const BuiltinEffectID> &effects, EffectContext &context)
	{
		// Effects execute in metadata order and share one mutable context.
		std::ranges::for_each(effects, [this, &context](const BuiltinEffectID effect) {
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
											  const std::span<const BuiltinEffectID> &effects, EffectContext &context)
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
		// TODO implement this function
		if (action.mMoveSlotIndex > 200)
		{
			mState.mBattleStarted = false;
			return;
		}
	}

	void BattleEngine::executeMoveTrigger(const MoveMeta &moveMeta, BattleTriggerID triggerID, EffectContext &context)
	{
		// Save the caller's source because nested trigger dispatch reuses the same context.
		const EffectSource previousSource{context.mSourceType};
		const BattleTarget owner{.mSide = context.mUserSide, .mSlotIndex = context.mUserIndex};
		context.mSourceType = EffectSource::Move;

		// Execute every metadata entry matching this move trigger in declaration order.
		std::ranges::for_each(moveMeta.mTriggers, [this, triggerID, &owner, &context](const MoveEffectTrigger &trigger) {
			if (trigger.mTrigger == triggerID)
			{
				// A trigger's suppression rules become visible before deciding whether its effects are suppressed.
				activateSuppressions(trigger.mSuppressionRules, trigger.mSuppresionRuleCount, EffectSource::Move, owner);

				if (!isSuppressed(EffectSource::Move, owner, triggerID, context))
				{
					executeEffects(trigger.mEffects, context);
				}
			}
		});

		// Restore the outer source so subsequent dispatch observes the context it received.
		context.mSourceType = previousSource;
	}

	void BattleEngine::executeAbilityTrigger(const BattleTarget &owner, const AbilityMeta &abilityMeta, const BattleTriggerID triggerID,
											 EffectContext &context, const bool targetEffects)
	{
		// Stamp source identity into the shared context for suppression matching and effect behavior.
		context.mAbilityID = abilityMeta.mAbilityID;
		context.mSourceType = EffectSource::Ability;

		// Dispatch matching, unsuppressed trigger entries in metadata order.
		for (const AbilityEffectTrigger &trigger : abilityMeta.mTriggers)
		{
			if (trigger.mTrigger != triggerID || isSuppressed(EffectSource::Ability, owner, triggerID, context))
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

	void BattleEngine::executeItemTrigger(const BattleTarget &owner, const ItemMeta &itemMeta, const BattleTriggerID triggerID,
										  EffectContext &context, const bool targetEffects)
	{
		// Stamp source identity into the shared context for suppression matching and effect behavior.
		context.mItemID = itemMeta.mItemID;
		context.mSourceType = EffectSource::Item;

		// Dispatch matching, unsuppressed trigger entries in metadata order.
		for (const ItemEffectTrigger &trigger : itemMeta.mTriggers)
		{
			if (trigger.mTrigger != triggerID || isSuppressed(EffectSource::Item, owner, triggerID, context))
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

	void BattleEngine::executeEndTurnTrigger()
	{
		for (const Side side : std::array{Side::A, Side::B})
		{
			const std::vector<BattleSlot> &slots{activeSlots(mState, side)};

			for (std::size_t slotIndex{0}; slotIndex < slots.size(); ++slotIndex)
			{
				if (isHealthy(slots.at(slotIndex)))
				{
					triggerSlot(BattleTarget{.mSide = side, .mSlotIndex = static_cast<ub>(slotIndex)}, BattleTriggerID::OnTurnEnd);
				}
			}
		}
	}

	void BattleEngine::triggerSlot(const BattleTarget &owner, const BattleTriggerID triggerID,
								   const std::optional<BattleTarget> &eventTarget)
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
		triggerSlotInContext(owner, triggerID, context);
		mActiveSuppressions.clear();
	}

	void BattleEngine::triggerSlotInContext(const BattleTarget &owner, const BattleTriggerID triggerID, EffectContext &context)
	{
		// Ignore stale targets that were removed by an earlier nested effect.
		if (!targetExists(mState, owner))
		{
			return;
		}

		// Resolve the current occupant's ability and item metadata, allowin either source to be absent.
		Pokemon *pokemon{activeSlots(mState, owner.mSide).at(owner.mSlotIndex).mPokemon};
		dispatchSlotSources(owner, pokemon, triggerID, context, SlotTriggerTargeting::ResolveMetadataTargets);
	}

	void BattleEngine::dispatchSlotSources(const BattleTarget &owner, const Pokemon *pokemon, const BattleTriggerID triggerID,
										   EffectContext &context, const SlotTriggerTargeting targeting)
	{
		// Resolve the occupant's ability and item metadata, allowing either source to be absent.
		// Preserve the outer source across nested ability and item trigger dispatch.
		const EffectSource previousSource{context.mSourceType};

		const AbilityID abilityID{pokemon->getAbilityID()};
		const AbilityMeta *abilityMeta{abilityID != NO_ABILITY_ID ? mProvider->abilityRegistry->getAbilityMetadata(abilityID) : nullptr};

		const ItemID itemID{pokemon->getItemID()};
		const ItemMeta *itemMeta{itemID != NO_ITEM_ID ? mProvider->itemRegistry->getItemMetadata(itemID) : nullptr};

		// Activate all relevant suppression rules before executing either source's effects.
		if (abilityMeta != nullptr)
		{
			activateAbilitySuppressions(*abilityMeta, owner, triggerID);
		}

		if (itemMeta != nullptr)
		{
			activateItemSuppressions(*itemMeta, owner, triggerID);
		}

		// Ability effects execute before item effects for the same slot and trigger.
		const bool targetEffects{targeting == SlotTriggerTargeting::ResolveMetadataTargets};

		if (abilityMeta != nullptr)
		{
			executeAbilityTrigger(owner, *abilityMeta, triggerID, context, targetEffects);
		}

		if (itemMeta != nullptr)
		{
			executeItemTrigger(owner, *itemMeta, triggerID, context, targetEffects);
		}

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
		dispatchSlotSources(faintedTarget, faintedPokemon, BattleTriggerID::OnFaint, context, SlotTriggerTargeting::PreserveContextTarget);
	}

} // namespace PocketCore::Battle
