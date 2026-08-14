#include "Battle/battleHelpers.h"

#include <algorithm>
#include <cmath>
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
#include "Move/moveMeta.h"
#include "Pokemon/pokemon.h"
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
} // namespace PocketCore::Battle
