#include "Battle/battleHelpers.h"

#include <algorithm>
#include <cmath>
#include <expected>
#include <limits>
#include <type_traits>
#include <variant>
#include <vector>

#include "Battle/battleAction.h"
#include "Battle/battleState.h"
#include "Battle/battleTargetsAndTriggers.h"
#include "Battle/battleValidation.h"
#include "Configuration/cache.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Effect/effectContext.h"
#include "Effect/effectSourceAndSuppresion.h"
#include "Move/moveHitPolicy.h"
#include "Move/moveID.h"
#include "Move/moveMeta.h"
#include "Pokemon/pokemon.h"
#include "Registry/moveRegistry.h"
#include "Utility/random.h"

namespace PocketCore::Battle
{
	using PocketCore::Battle::BattleSlot;
	using PocketCore::Battle::BattleTarget;
	using PocketCore::Configuration::CACHE_STAT_STAGE_MULTIPLIERS;
	using PocketCore::Configuration::statStageCacheIndex;
	using PocketCore::Core::sb;
	using PocketCore::Core::sl;
	using PocketCore::Core::ub;
	using PocketCore::Effect::EffectSource;
	using PocketCore::Effect::Side;
	using PocketCore::Move::FixedHitCount;
	using PocketCore::Move::MoveMeta;
	using PocketCore::Move::WeightedHitCountOutcome;
	using PocketCore::Pokemon::Pokemon;
	using PocketCore::Registry::Move::MoveRegistry;
	using PocketCore::Utility::Random;

#if defined(ATTR_GCC) && !defined(ATTR_CLANG)
	// GCC suggests returns_nonnull for references even though the attribute accepts only pointer returns.
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wsuggest-attribute=returns_nonnull"
#endif

	ATTR_NODISCARD ATTR_CONST std::vector<BattleSlot> &activeSlots(BattleState &state, const Side side)
	{
		return side == Side::A ? state.mSideA : state.mSideB;
	}

	ATTR_NODISCARD ATTR_CONST const std::vector<BattleSlot> &activeSlots(const BattleState &state, const Side side)
	{
		return side == Side::A ? state.mSideA : state.mSideB;
	}

	ATTR_NODISCARD ATTR_PURE BattleSlot *contextSlot(BattleState &state, const Side side, const ub slotIndex) noexcept
	{
		std::vector<BattleSlot> &slots{activeSlots(state, side)};
		return slotIndex < slots.size() ? &slots.at(slotIndex) : nullptr;
	}

	ATTR_NODISCARD ATTR_CONST std::vector<Pokemon *> &party(BattleState &state, const Side side)
	{
		return side == Side::A ? state.mPartyA : state.mPartyB;
	}

	ATTR_NODISCARD ATTR_CONST const std::vector<Pokemon *> &party(const BattleState &state, const Side side)
	{
		return side == Side::A ? state.mPartyA : state.mPartyB;
	}

#if defined(ATTR_GCC) && !defined(ATTR_CLANG)
	#pragma GCC diagnostic pop
#endif

	ATTR_NODISCARD ATTR_PURE bool isHealthy(const BattleSlot &slot) noexcept
	{
		return slot.mPokemon != nullptr && !slot.mPokemon->isFainted();
	}

	ATTR_NODISCARD ATTR_PURE bool isHealthy(const Pokemon *pokemon) noexcept
	{
		return pokemon != nullptr && !pokemon->isFainted();
	}

	ATTR_NODISCARD ATTR_PURE bool isActive(const BattleState &state, const Side side, const Pokemon *pokemon) noexcept
	{
		const std::vector<BattleSlot> &slots{activeSlots(state, side)};
		return std::ranges::any_of(slots, [pokemon](const BattleSlot &slot) { return slot.mPokemon == pokemon; });
	}

	ATTR_NODISCARD ATTR_PURE bool isAdjacent(const BattleSlot &source, const BattleSlot &target) noexcept
	{
		const sb positionDifference{static_cast<sb>(std::abs(static_cast<sb>(source.mPosition) - static_cast<sb>(target.mPosition)))};
		return positionDifference <= 1;
	}

	ATTR_NODISCARD ATTR_PURE bool targetExists(const BattleState &state, const BattleTarget target) noexcept
	{
		const std::vector<BattleSlot> &slots{activeSlots(state, target.mSide)};
		return target.mSlotIndex < slots.size() && isHealthy(slots.at(target.mSlotIndex));
	}

	ATTR_NODISCARD ATTR_PURE bool sideHasHealthyPokemon(const BattleState &state, const Side side) noexcept
	{
		const std::vector<Pokemon *> &sideParty{party(state, side)};
		return std::ranges::any_of(sideParty, [](const Pokemon *pokemon) { return isHealthy(pokemon); });
	}

	ATTR_NODISCARD ATTR_PURE double getEffectiveSpeed(const BattleSlot &slot) noexcept
	{
		const Pokemon *slotPokemon{slot.mPokemon};

		if (slotPokemon == nullptr)
		{
			return 0.0;
		}

		const double stageMultiplier{CACHE_STAT_STAGE_MULTIPLIERS.at(statStageCacheIndex(slot.mStatStages.mSpeed))};

		return slotPokemon->getSpeed() * stageMultiplier * slot.mDamageFormulaModifiers.mSpeedModifier;
	}

	ATTR_NODISCARD EffectContext makeMoveContext(const MoveAction &action, const MoveMeta &moveMeta, const BattleTarget target,
												 const ub hitAttemptIndex)
	{
		EffectContext context{};

		context.mMoveID = moveMeta.mMoveID;
		context.mMoveTypeID = moveMeta.mTypeID;
		context.mMoveBasePower = moveMeta.mPower;
		context.mMoveAccuracy = moveMeta.mAccuracy;
		context.mRangeOverride = moveMeta.mRangeID;
		context.mHitAttemptIndex = hitAttemptIndex;
		context.mSourceType = EffectSource::Move;
		context.mUserSide = action.mSide;
		context.mTargetSide = target.mSide;
		context.mUserIndex = action.mUserSlotIndex;
		context.mTargetIndex = target.mSlotIndex;
		context.mIsSpecial = moveMeta.mSpecial;

		return context;
	}

	void applyRecoil(BattleState &state, const EffectContext &context)
	{
		const BattleSlot *userSlot{contextSlot(state, context.mUserSide, context.mUserIndex)};

		if (userSlot == nullptr || userSlot->mPokemon == nullptr || context.mDamage.mDamage == 0U
			|| !std::isfinite(context.mDamage.mRecoilRatio) || context.mDamage.mRecoilRatio <= 0.0F)
		{
			return;
		}

		Pokemon *userPokemon{userSlot->mPokemon};

		const double recoilValue{
			std::floor(static_cast<double>(context.mDamage.mDamage) * static_cast<double>(context.mDamage.mRecoilRatio)),
		};

		const us recoilDamage{static_cast<us>(std::clamp(recoilValue, 1.0, static_cast<double>(std::numeric_limits<us>::max())))};
		const us currentHealth{userPokemon->getHealth()};

		userPokemon->setHealth(recoilDamage >= currentHealth ? static_cast<us>(0) : static_cast<us>(currentHealth - recoilDamage));
	}

	ATTR_NODISCARD ub resolveHitCount(const MoveMeta &moveMeta)
	{
		return std::visit(
			[](const auto &policy) {
				using Policy = std::decay_t<decltype(policy)>;

				if constexpr (std::is_same_v<Policy, FixedHitCount>)
				{
					return policy.mHitCount;
				}
				else
				{
					double totalWeight{0.0};

					std::ranges::for_each(policy.mOutcomes, [&totalWeight](const WeightedHitCountOutcome &outcome) {
						if (std::isfinite(outcome.mWeight) && outcome.mWeight > 0.0)
						{
							totalWeight += outcome.mWeight;
						}
					});

					if (!std::isfinite(totalWeight) || totalWeight <= 0.0)
					{
						return static_cast<ub>(0);
					}

					const double selectedWeight{Random::get(0.0, totalWeight)};
					double cumulativeWeight{0.0};

					const auto selectedOutcome{
						std::ranges::find_if(policy.mOutcomes,
											 [&cumulativeWeight, selectedWeight](const WeightedHitCountOutcome &outcome) {
												 if (std::isfinite(outcome.mWeight) && outcome.mWeight > 0.0)
												 {
													 cumulativeWeight += outcome.mWeight;

													 if (selectedWeight < cumulativeWeight)
													 {
														 return true;
													 }
												 }

												 return false;
											 }),
					};

					if (selectedOutcome != policy.mOutcomes.end())
					{
						return selectedOutcome->mHitCount;
					}

					return policy.mOutcomes.back().mHitCount;
				}
			},
			moveMeta.mHitCountPolicy);
	}

	ATTR_NODISCARD bool hasDuplicatePokemonPointers(const std::span<Pokemon *const> &partyA, const std::span<Pokemon *const> &partyB)
	{
		std::vector<Pokemon *> combinedParties{};
		combinedParties.reserve(partyA.size() + partyB.size());
		combinedParties.insert(combinedParties.end(), partyA.begin(), partyA.end());
		combinedParties.insert(combinedParties.end(), partyB.begin(), partyB.end());
		std::ranges::sort(combinedParties);

		return std::ranges::adjacent_find(combinedParties) != combinedParties.end();
	}

	ATTR_NODISCARD ATTR_PURE sl healthyPokemonInParty(const std::span<Pokemon *const> &party)
	{
		return std::ranges::count_if(party, [](const Pokemon *pokemon) { return isHealthy(pokemon); });
	}

	void assignActiveSlots(const std::span<Pokemon *const> party, std::vector<BattleSlot> &slots, const ub activePokemonPerSide)
	{
		for (Pokemon *pokemon : party)
		{
			// Fainted Pokemon remain in the party for later inspection, but cannot be placed in an active slot.
			if (!isHealthy(pokemon))
			{
				continue;
			}

			// Use the number of slots already filled as the battlefield position; this stays contiguous even when
			// fainted or otherwise ineligible party members were skipped earlier in the party.
			slots.push_back(BattleSlot{.mPokemon = pokemon, .mPosition = static_cast<ub>(slots.size())});

			if (slots.size() == activePokemonPerSide)
			{
				// The caller already confirmed that enough healthy Pokemon exist, so no later party member can be selected.
				return;
			}
		}
	}

	ATTR_NODISCARD bool canTarget(const BattleState &state, const BattleTarget &source, const BattleTarget &candidate,
								  const BattleRangeID rangeID)
	{
		// Centralize occupancy and range checks so every selector applies identical eligibility rules.
		if (!targetExists(state, candidate))
		{
			return false;
		}

		// Unrestricted effects ignore formation adjacency but still require an existing target.
		if (rangeID == BattleRangeID::Unrestricted)
		{
			return true;
		}

		// Restricted effects use each slot's battlefield position to determine reachability.
		const BattleSlot &sourceSlot{activeSlots(state, source.mSide).at(source.mSlotIndex)};
		const BattleSlot &candidateSlot{activeSlots(state, candidate.mSide).at(candidate.mSlotIndex)};

		return isAdjacent(sourceSlot, candidateSlot);
	}

	void appendSide(std::vector<BattleTarget> &targets, const BattleState &state, const BattleTarget &source, const BattleRangeID rangeID,
					const Side side)
	{
		// Append eligible slots in active-slot order to keep multi-target execution deterministic.
		const std::vector<BattleSlot> &slots{activeSlots(state, side)};

		for (std::size_t slotIndex{0}; slotIndex < slots.size(); ++slotIndex)
		{
			const BattleTarget candidate{.mSide = side, .mSlotIndex = static_cast<ub>(slotIndex)};

			if (canTarget(state, source, candidate, rangeID))
			{
				targets.push_back(candidate);
			}
		}
	}

	ATTR_NODISCARD ATTR_PURE BattleResult getResult(const BattleState &state) noexcept
	{
		// Result derivation in stateless: inspect the current parties on every equery.
		if (!state.mBattleStarted)
		{
			return BattleResult::NotStarted;
		}

		// A side remains in the battle while any party member is healthy, active, or reserve.
		const bool sideAHealthy{sideHasHealthyPokemon(state, Side::A)};
		const bool sideBHealthy{sideHasHealthyPokemon(state, Side::B)};

		if (sideAHealthy && sideBHealthy)
		{
			return BattleResult::InProgress;
		}

		// Exactly one surviving side wins; simultaneous exhaustion is a draw.
		if (sideAHealthy)
		{
			return BattleResult::SideAWon;
		}

		if (sideBHealthy)
		{
			return BattleResult::SideBWon;
		}

		return BattleResult::Draw;
	}

	ATTR_NODISCARD ATTR_PURE bool hasReserve(const BattleState &state, const Side side, const std::vector<Pokemon *> &trainerParty)
	{
		// A fainted slot requires replacement only when this side has a healthy reserve available.
		return std::ranges::any_of(
			trainerParty, [side, &state](const Pokemon *pokemon) { return isHealthy(pokemon) && !isActive(state, side, pokemon); });
	}

	ATTR_NODISCARD std::expected<std::vector<BattleTarget>, BattleEngineError> getMoveTargets(const BattleState &state,
																							  const MoveAction &action,
																							  const MoveRegistry *moveRegistry)
	{
		// Validate only the action fields required to locate the move and its targeting metadata.
		const std::vector<BattleSlot> &slots{activeSlots(state, action.mSide)};
		const BattleSlot &userSlot{slots.at(action.mUserSlotIndex)};

		if (!state.mBattleStarted || action.mUserSlotIndex >= slots.size() || !isHealthy(userSlot))
		{
			return std::unexpected{BattleEngineError::InvalidActiveSlot};
		}

		// Guard the fixed move array before retrieving the selected move ID.
		if (action.mMoveSlotIndex >= userSlot.mPokemon->getMovesArray().size())
		{
			return std::unexpected{BattleEngineError::InvalidMoveSlot};
		}

		// Missing metadata makes the move unusable even when a move ID occupoed the selected slot.
		const MoveID moveID{userSlot.mPokemon->getMoveID(action.mMoveSlotIndex)};
		const MoveMeta *moveMeta{moveRegistry->getMoveMetadata(moveID)};

		if (moveMeta == nullptr)
		{
			return std::unexpected{BattleEngineError::MoveNotFound};
		}

		// Delegate selector semantics and range filtering to the common target resolver.
		return resolveTargets(state, action.mSide, action.mUserSlotIndex, moveMeta->mTargetID, moveMeta->mRangeID, action.mSelectedTarget);
	}

	ATTR_NODISCARD std::expected<std::vector<BattleTarget>, BattleEngineError> resolveTargets(
		const BattleState &state, const Side sourceSide, const ub sourceSlotIndex, const BattleTargetID targetID,
		const BattleRangeID rangeID, const std::optional<BattleTarget> &selectedTarget)
	{
		// The source itself must identify a currently occupied active slot
		const BattleTarget source{.mSide = sourceSide, .mSlotIndex = sourceSlotIndex};
		if (!targetExists(state, source))
		{
			return std::unexpected{BattleEngineError::InvalidActiveSlot};
		}

		std::vector<BattleTarget> targets{};

		// Expand the move's target selector into concrete active-slot addresses.

		switch (targetID)
		{
			case BattleTargetID::Self:
				// Self always passed targetExists above and does not need a caller-provided selection.
				targets.push_back(source);
				break;
			case BattleTargetID::SingleOpponent:
				// A supplied selection must identify an eligible opponent.
				if (selectedTarget.has_value())
				{
					if (selectedTarget->mSide != getOppositeSide(sourceSide) || !canTarget(state, source, *selectedTarget, rangeID))
					{
						return std::unexpected{BattleEngineError::InvalidTarget};
					}

					targets.push_back(*selectedTarget);
					break;
				}

				// Without a selection, implicit targeting is valid only when exactly one opponent is eligible.
				appendSide(targets, state, source, rangeID, getOppositeSide(sourceSide));

				if (targets.size() != 1U)
				{
					return std::unexpected{BattleEngineError::InvalidTarget};
				}

				break;
			case BattleTargetID::AllAllies:
				// Ally-wide effects include the source.
				appendSide(targets, state, source, rangeID, sourceSide);
				break;
			case BattleTargetID::AllOpponents:
				appendSide(targets, state, source, rangeID, getOppositeSide(sourceSide));
				break;
			case BattleTargetID::AllExceptSelf:
				appendSide(targets, state, source, rangeID, Side::A);
				appendSide(targets, state, source, rangeID, Side::B);

				std::erase(targets, source);
				break;
			default:
				return std::unexpected{BattleEngineError::InvalidTarget};
		}

		// Selectors that produce no live, in-range slots are invalid rather than successful no-ops.
		if (targets.empty())
		{
			return std::unexpected{BattleEngineError::InvalidTarget};
		}

		return targets;
	}

	ATTR_NODISCARD ATTR_PURE std::expected<void, BattleEngineError> validateSwitchAction(const BattleState &state,
																						 const SwitchAction &action)
	{
		// Switching requires an initialized battle and an undecided result.
		if (!state.mBattleStarted)
		{
			return std::unexpected{BattleEngineError::BattleNotStarted};
		}

		if (getResult(state) != BattleResult::InProgress)
		{
			return std::unexpected{BattleEngineError::BattleFinished};
		}

		// Verify the outgoing battlefield slot befor indexing the active side.
		const std::vector<BattleSlot> &slots{activeSlots(state, action.mSide)};

		if (action.mActiveSlotIndex >= slots.size())
		{
			return std::unexpected{BattleEngineError::InvalidActiveSlot};
		}

		// Verify the incoming party index independently because party and active-slot indexes are unrelated.
		const std::vector<Pokemon *> &trainerParty{party(state, action.mSide)};

		if (action.mPartyIndex >= trainerParty.size())
		{
			return std::unexpected{BattleEngineError::InvalidPartyIndex};
		}

		Pokemon *incomingPokemon{trainerParty.at(action.mPartyIndex)};

		// Fainted reserves cannot be switched into battle.
		if (!isHealthy(incomingPokemon))
		{
			return std::unexpected{BattleEngineError::PokemonFainted};
		}

		// Prevent one Pokemon from occupying two active slots or switching into its current slot.
		if (isActive(state, action.mSide, incomingPokemon))
		{
			return std::unexpected{BattleEngineError::PokemonAlreadyActive};
		}

		return {};
	}

	ATTR_NODISCARD std::expected<void, BattleEngineError> validateMoveAction(const BattleState &state, const MoveAction &action,
																			 const BattlePhase phase, const MoveRegistry *moveRegistry)
	{
		// Move actions are meaningful only after a battle has successfully started.
		if (!state.mBattleStarted)
		{
			return std::unexpected{BattleEngineError::BattleNotStarted};
		}

		// Forced replacements must be resolved before either trainer can submit another move.
		if (phase == BattlePhase::AwaitingReplacements)
		{
			return std::unexpected{BattleEngineError::ReplacementRequired};
		}

		// No action may mutate a battle whose result has already been decided.
		if (getResult(state) != BattleResult::InProgress)
		{
			return std::unexpected{BattleEngineError::BattleFinished};
		}

		// Resolve the acting slot on the action's side and reject out-of-range indexes before access.
		const std::vector<BattleSlot> &slots{activeSlots(state, action.mSide)};

		if (action.mUserSlotIndex >= slots.size())
		{
			return std::unexpected{BattleEngineError::InvalidActiveSlot};
		}

		const BattleSlot &userSlot{slots.at(action.mUserSlotIndex)};

		// A fainted active Pokemon remains in its slot until replacement, but it cannot act.
		if (!isHealthy(userSlot))
		{
			return std::unexpected{BattleEngineError::PokemonFainted};
		}

		// The selected move slot must exist in the Pokemon's fixed move array.
		const Pokemon *userPokemon{userSlot.mPokemon};

		if (action.mMoveSlotIndex >= userPokemon->getMovesArray().size())
		{
			return std::unexpected{BattleEngineError::InvalidMoveSlot};
		}

		// A move is usable only when its ID is assigned and resolves to registered metadata.
		const MoveID moveID{userPokemon->getMoveID(action.mMoveSlotIndex)};

		if (moveID == Move::NO_MOVE_ID && moveRegistry->getMoveMetadata(moveID) == nullptr)
		{
			return std::unexpected{BattleEngineError::MoveNotFound};
		}

		// PP is checked during validation and consumed later only when the move actually executes.
		if (userPokemon->getCurrentPP(action.mMoveSlotIndex) == 0U)
		{
			return std::unexpected{BattleEngineError::NoPP};
		}

		// Resolve targets now so malformed or ambiguous selections fail before turn state is mutated.
		const std::expected<std::vector<BattleTarget>, BattleEngineError> targets{getMoveTargets(state, action, moveRegistry)};

		if (!targets.has_value())
		{
			return std::unexpected{targets.error()};
		}

		return {};
	}

	ATTR_NODISCARD std::expected<void, BattleEngineError> getValidationResult(const BattleState &state, const BattleAction &action,
																			  const BattlePhase phase, const MoveRegistry *moveRegistry)
	{
		return std::visit(
			[&state, phase, &moveRegistry](const auto &selectedAction) {
				using Action = std::decay_t<decltype(selectedAction)>;
				if constexpr (std::is_same_v<Action, MoveAction>)
				{
					return validateMoveAction(state, selectedAction, phase, moveRegistry);
				}
				else
				{
					return validateSwitchAction(state, selectedAction);
				}
			},
			action);
	}

	ATTR_NODISCARD ATTR_PURE BattleTarget getBattleTarget(const BattleAction &action)
	{
		return std::visit(
			[](const auto &selectedAction) {
				using Action = std::decay_t<decltype(selectedAction)>;
				if constexpr (std::is_same_v<Action, MoveAction>)
				{
					return BattleTarget{.mSide = selectedAction.mSide, .mSlotIndex = selectedAction.mUserSlotIndex};
				}
				else
				{
					return BattleTarget{.mSide = selectedAction.mSide, .mSlotIndex = selectedAction.mActiveSlotIndex};
				}
			},
			action);
	}

	void handleMovePrioritization(const BattleState &state, std::vector<MoveAction> &moves, const MoveRegistry *moveRegistry)
	{
		std::shuffle(moves.begin(), moves.end(), Random::getTwister());
		std::ranges::stable_sort(moves, [&state, &moveRegistry](const MoveAction &left, const MoveAction &right) {
			const BattleSlot &leftSlot{activeSlots(state, left.mSide).at(left.mUserSlotIndex)};
			const BattleSlot &rightSlot{activeSlots(state, right.mSide).at(right.mUserSlotIndex)};

			const MoveMeta *leftMove{moveRegistry->getMoveMetadata(leftSlot.mPokemon->getMoveID(left.mMoveSlotIndex))};
			const MoveMeta *rightMove{moveRegistry->getMoveMetadata(rightSlot.mPokemon->getMoveID(right.mMoveSlotIndex))};

			// Validated engine actions always have metadata; keep direct helper calls null-safe and ordered consistently.
			if (leftMove == nullptr || rightMove == nullptr)
			{
				return leftMove != nullptr;
			}

			if (leftMove->mPriority != rightMove->mPriority)
			{
				return leftMove->mPriority > rightMove->mPriority;
			}

			// Compare as doubles without narrowing the effective speed calculation.
			const double leftSpeed{getEffectiveSpeed(leftSlot)};
			const double rightSpeed{getEffectiveSpeed(rightSlot)};

			if (leftSpeed < rightSpeed || leftSpeed > rightSpeed)
			{
				return leftSpeed > rightSpeed;
			}

			return false;
		});
	}
} // namespace PocketCore::Battle
