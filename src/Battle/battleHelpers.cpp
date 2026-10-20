/*! @file battleHelpers.cpp
	@brief Contains the function definitions for battle helper functions
	@date 10/20/2026
	@since 0.9.14
	@version 0.12.51
	@author Matthew Moore
*/

#include "Battle/battleHelpers.h"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <expected>
#include <limits>
#include <optional>
#include <ostream>
#include <ranges>
#include <span>
#include <sstream>
#include <string>
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
#include "Effect/effectSourceAndSuppression.h"
#include "Move/moveMeta.h"
#include "Pokemon/pokemon.h"
#include "Pokemon/pokemonHelpers.h"
#include "Registry/moveRegistry.h"
#include "Registry/registryProvider.h"
#include "Status/statusID.h"
#include "Status/volatileStatus.h"
#include "Validation/Battle/battleError.h"

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
	using PocketCore::Move::MoveMeta;
	using PocketCore::Pokemon::Pokemon;
	using PocketCore::Pokemon::printPokemonWithNames;
	using PocketCore::Registry::Move::MoveRegistry;
	using PocketCore::Registry::RegistryProvider;
	using PocketCore::Status::StatusID;
	using PocketCore::Status::VolatileStatus;
	using PocketCore::Validation::Battle::BattleEngineError;

#if ATTR_ONLY_GCC
	// GCC suggests returns_nonnull for references even though the attribute accepts only pointer returns.
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wsuggest-attribute=returns_nonnull"
#endif

	ATTR_NODISCARD ATTR_CONST std::vector<BattleSlot> &activeSlots(BattleState &state, const Side side) noexcept
	{
		return side == Side::A ? state.mSideA : state.mSideB;
	}

	ATTR_NODISCARD ATTR_CONST const std::span<const BattleSlot> activeSlots(const BattleState &state, const Side side) noexcept
	{
		return side == Side::A ? state.mSideA : state.mSideB;
	}

	ATTR_NODISCARD ATTR_PURE BattleSlot *contextSlot(BattleState &state, const Side side, const ub slotIndex) noexcept
	{
		std::vector<BattleSlot> &slots{activeSlots(state, side)};
		return slotIndex < slots.size() ? &slots.at(slotIndex) : nullptr;
	}

	ATTR_NODISCARD ATTR_CONST std::vector<Pokemon *> &party(BattleState &state, const Side side) noexcept
	{
		return side == Side::A ? state.mPartyA : state.mPartyB;
	}

	ATTR_NODISCARD ATTR_CONST const std::span<const Pokemon *const> party(const BattleState &state, const Side side) noexcept
	{
		return side == Side::A ? state.mPartyA : state.mPartyB;
	}

#if ATTR_ONLY_GCC
	#pragma GCC diagnostic pop
#endif

	ATTR_NODISCARD ATTR_PURE bool isHealthy(const BattleSlot &slot) noexcept
	{
		return slot.mPokemon != nullptr && !slot.mPokemon->isFainted();
	}

	ATTR_NODISCARD ATTR_PURE bool isHealthy(const Pokemon *const pokemon) noexcept
	{
		return pokemon != nullptr && !pokemon->isFainted();
	}

	ATTR_NODISCARD ATTR_PURE bool isActive(const BattleState &state, const Side side, const Pokemon *const pokemon) noexcept
	{
		const std::span<const BattleSlot> slots{activeSlots(state, side)};
		return std::ranges::any_of(slots, [&pokemon](const BattleSlot &slot) { return slot.mPokemon == pokemon; });
	}

	ATTR_NODISCARD ATTR_PURE bool isAdjacent(const BattleSlot &source, const BattleSlot &target) noexcept
	{
		const sb positionDifference{static_cast<sb>(std::abs(static_cast<sb>(source.mPosition) - static_cast<sb>(target.mPosition)))};
		return positionDifference <= 1;
	}

	ATTR_NODISCARD ATTR_PURE bool targetExists(const BattleState &state, const BattleTarget &target) noexcept
	{
		const std::span<const BattleSlot> slots{activeSlots(state, target.mSide)};
		return target.mSlotIndex < slots.size() && isHealthy(slots.at(target.mSlotIndex));
	}

	ATTR_NODISCARD ATTR_PURE bool sideHasHealthyPokemon(const BattleState &state, const Side side) noexcept
	{
		const std::span<const Pokemon *const> sideParty{party(state, side)};
		return std::ranges::any_of(sideParty, [](const Pokemon *const pokemon) { return isHealthy(pokemon); });
	}

	ATTR_NODISCARD ATTR_PURE double getEffectiveSpeed(const BattleSlot &slot) noexcept
	{
		const Pokemon *slotPokemon{slot.mPokemon};

		if (slotPokemon == nullptr)
		{
			return 0.0;
		}

		const double stageMultiplier{CACHE_STAT_STAGE_MULTIPLIERS.at(statStageCacheIndex(slot.mStatStages.mSpeed))};

		return std::max(0.0, std::floor(slotPokemon->getSpeed() * stageMultiplier * slot.mDamageFormulaModifiers.mSpeedModifier));
	}

	ATTR_NODISCARD EffectContext makeMoveContext(const MoveAction &action, const MoveMeta &moveMeta, const BattleTarget &target,
												 const ub hitAttemptIndex) noexcept
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

		if (userSlot == nullptr

			|| userSlot->mPokemon == nullptr

			|| context.mDamage.mDamage == 0U || !std::isfinite(context.mDamage.mRecoilRatio)

			|| context.mDamage.mRecoilRatio <= 0.0)
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

	ATTR_NODISCARD bool hasDuplicatePokemonPointers(const std::span<Pokemon *const> partyA, const std::span<Pokemon *const> partyB)
	{
		std::vector<Pokemon *> combinedParties{};
		combinedParties.reserve(partyA.size() + partyB.size());
		combinedParties.insert(combinedParties.end(), partyA.begin(), partyA.end());
		combinedParties.insert(combinedParties.end(), partyB.begin(), partyB.end());
		std::ranges::sort(combinedParties);

		return std::ranges::adjacent_find(combinedParties) != combinedParties.end();
	}

	ATTR_NODISCARD ATTR_PURE sl amountOfHealthyPokemonInParty(const std::span<const Pokemon *const> party) noexcept
	{
		return std::ranges::count_if(party, [](const Pokemon *pokemon) noexcept { return isHealthy(pokemon); });
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
								  const BattleRangeID rangeID) noexcept
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

		// LCOV_EXCL_BR_START - The branches are from the .at()

		// Restricted effects use each slot's battlefield position to determine reachability.
		const BattleSlot &sourceSlot{activeSlots(state, source.mSide).at(source.mSlotIndex)};
		const BattleSlot &candidateSlot{activeSlots(state, candidate.mSide).at(candidate.mSlotIndex)};

		// LCOV_EXCL_BR_STOP

		return isAdjacent(sourceSlot, candidateSlot);
	}

	void appendEligibleTargets(std::vector<BattleTarget> &targets, const BattleState &state, const BattleTarget &source,
							   const BattleRangeID rangeID, const Side side)
	{
		// Append eligible slots in active-slot order to keep multi-target execution deterministic.
		const std::span<const BattleSlot> slots{activeSlots(state, side)};

		for (const std::size_t slotIndex : std::views::iota(std::size_t{0}, slots.size()))
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

	ATTR_NODISCARD ATTR_PURE bool hasReserve(const BattleState &state, const Side side,
											 const std::span<const Pokemon *const> trainerParty) noexcept
	{
		// A fainted slot requires replacement only when this side has a healthy reserve available.
		return std::ranges::any_of(trainerParty, [side, &state](const Pokemon *pokemon) noexcept {
			return isHealthy(pokemon) && !isActive(state, side, pokemon);
		});
	}

	ATTR_NODISCARD std::expected<std::vector<BattleTarget>, BattleEngineError> resolveTargets(
		const BattleState &state, const BattleTarget &source, const BattleTargetID targetID, const BattleRangeID rangeID,
		const std::optional<BattleTarget> &selectedTarget)
	{
		// The source itself must identify a currently occupied active slot
		if (!targetExists(state, source))
		{
			return std::unexpected{BattleEngineError::InvalidActiveSlot};
		}

		std::vector<BattleTarget> targets{};

		const auto singleTargetHandler{
			[&selectedTarget, &source, &targets, &state, rangeID](const Side targetSide, const bool singleAlly = false) {
				// A supplied selection must identify an eligible target.
				if (selectedTarget.has_value())
				{
					if (selectedTarget->mSide != targetSide || !canTarget(state, source, *selectedTarget, rangeID))
					{
						return true;
					}

					targets.push_back(*selectedTarget);

					return false;
				}

				// Without a selection, implicit targeting is valid only when exactly one target is eligible.
				appendEligibleTargets(targets, state, source, rangeID, targetSide);

				if (singleAlly)
				{
					std::erase(targets, source); // Erase self from target selection since BattleTargetID::Self exists
				}

				return targets.size() != 1U;
			},
		};

		// Expand the move's target selector into concrete active-slot addresses.
		switch (targetID)
		{
			case BattleTargetID::Self:
				// Self always passed targetExists above and does not need a caller-provided selection.
				targets.push_back(source);
				break;
			case BattleTargetID::SingleAlly:
				// A supplied selection must identify an eligible ally.
				if (singleTargetHandler(source.mSide, true))
				{
					return std::unexpected{BattleEngineError::InvalidTarget};
				}

				break;
			case BattleTargetID::SingleOpponent:
				if (singleTargetHandler(getOppositeSide(source.mSide)))
				{
					return std::unexpected{BattleEngineError::InvalidTarget};
				}

				break;
			case BattleTargetID::AllAlliesIncludingSelf:
				appendEligibleTargets(targets, state, source, rangeID, source.mSide);
				break;
			case BattleTargetID::AllAlliesExceptSelf:
				appendEligibleTargets(targets, state, source, rangeID, source.mSide);

				std::erase(targets, source);
				break;
			case BattleTargetID::AllOpponents:
				appendEligibleTargets(targets, state, source, rangeID, getOppositeSide(source.mSide));
				break;
			case BattleTargetID::AllExceptSelf:
				appendEligibleTargets(targets, state, source, rangeID, Side::A);
				appendEligibleTargets(targets, state, source, rangeID, Side::B);

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

	ATTR_NODISCARD std::expected<std::vector<BattleTarget>, BattleEngineError> getMoveTargets(const BattleState &state,
																							  const MoveAction &action,
																							  const MoveRegistry *moveRegistry)
	{
		// Validate only the action fields required to locate the move and its targeting metadata.
		const std::span<const BattleSlot> slots{activeSlots(state, action.mSide)};

		if (!state.mBattleStarted || action.mUserSlotIndex >= slots.size())
		{
			return std::unexpected{BattleEngineError::InvalidActiveSlot};
		}

		const BattleSlot &userSlot{slots.at(action.mUserSlotIndex)};

		if (!isHealthy(userSlot))
		{
			return std::unexpected{BattleEngineError::InvalidActiveSlot};
		}

		// Guard the fixed move array before retrieving the selected move ID.
		if (action.mMoveSlotIndex >= userSlot.mPokemon->getMoveIDsArray().size())
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

		BattleTarget source{.mSide = action.mSide, .mSlotIndex = action.mUserSlotIndex};

		// Delegate selector semantics and range filtering to the common target resolver.
		return resolveTargets(state, source, moveMeta->mTargetID, moveMeta->mRangeID, action.mSelectedTarget);
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

	std::string indentBlock(const std::string &block)
	{
		std::string result{"  "};

		for (const char character : block)
		{
			result += character;

			if (character == '\n')
			{
				result += "  ";
			}
		}

		return result;
	}

#if ATTR_ONLY_GCC
	// GCC suggests returns_nonnull for references even though the attribute accepts only pointer returns.
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wsuggest-attribute=returns_nonnull"
#endif

	std::ostream &printBattleSlotWithNames(std::ostream &outStream, const BattleSlot &battleSlot, const RegistryProvider &registryProvider)
	{
		if (battleSlot.mPokemon == nullptr)
		{
			return outStream;
		}

		const auto printIDAndName = [&outStream]<typename StableID, typename NameLookup>(
										const std::string_view indentation, const StableID stableID, const NameLookup &nameLookup) {
			constexpr std::string_view unregisteredName{"<unregistered>"};
			const std::optional<std::string_view> name{nameLookup(stableID)};

			outStream << indentation << "ID: " << stableID.getValue() << '\n' << indentation << "Name: " << name.value_or(unregisteredName);
		};

		std::ostringstream modifiersStream;
		modifiersStream << battleSlot.mDamageFormulaModifiers;

		std::ostringstream pokemonStream;
		printPokemonWithNames(pokemonStream, *battleSlot.mPokemon, registryProvider);

		std::ostringstream statStagesStream;
		statStagesStream << battleSlot.mStatStages;

		outStream << "Battle Slot {\n" << indentBlock(modifiersStream.str()) << '\n';
		outStream << "  Volatile Statuses:\n";

		for (const auto &[index, statusID] :
			 battleSlot.mVolatileStatuses | std::views::transform([](const VolatileStatus &status) noexcept {
				 return StatusID{status.mStatusID};
			 }) | std::views::enumerate)
		{
			outStream << "    [" << index << "]:\n";

			printIDAndName("      ", statusID, [&registryProvider](const StatusID identifier) {
				return registryProvider.mStatusRegistry != nullptr ? registryProvider.mStatusRegistry->getStatusName(identifier)
																   : std::nullopt;
			});

			outStream << '\n';
		}

		outStream << std::boolalpha << indentBlock(pokemonStream.str()) << '\n'
				  << indentBlock(statStagesStream.str()) << '\n'
				  << "  Choice Locked Move ID: " << battleSlot.mChoiceLockedMove.getValue() << '\n'
				  << "  Position: " << static_cast<us>(battleSlot.mPosition) << '\n'
				  << "  Sleep: " << static_cast<us>(battleSlot.mSleepCounter) << '\n'
				  << "  Toxic: " << static_cast<us>(battleSlot.mToxicCounter) << '\n'
				  << "  Protection: " << static_cast<us>(battleSlot.mProtectionCounter) << '\n'
				  << "  Is Protected: " << battleSlot.mIsProtected << '\n'
				  << "  Is Flinched: " << battleSlot.mIsFlinched << '\n'
				  << "  Is Grounded: " << battleSlot.mIsGrounded << '\n'
				  << "  Faint Processed: " << battleSlot.mFaintProcessed << '\n';

		outStream << '}';

		return outStream;
	}

#if ATTR_ONLY_GCC
	#pragma GCC diagnostic pop
#endif
} // namespace PocketCore::Battle
