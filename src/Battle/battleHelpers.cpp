#include "Battle/battleHelpers.h"

#include <algorithm>
#include <vector>

#include "Battle/battleEngine_copy.h"
#include "Battle/battleState.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Effect/effectContext.h"
#include "Pokemon/pokemon.h"

namespace PocketCore::Battle
{
	using PocketCore::Battle::BattleSlot;
	using PocketCore::Battle::BattleTarget;
	using PocketCore::Core::sb;
	using PocketCore::Effect::Side;
	using PocketCore::Pokemon::Pokemon;

	ATTR_NODISCARD std::vector<BattleSlot> &activeSlots(BattleState &state, const Side side)
	{
		return side == Side::A ? state.mSideA : state.mSideB;
	}

	ATTR_NODISCARD const std::vector<BattleSlot> &activeSlots(const BattleState &state, const Side side)
	{
		return side == Side::A ? state.mSideA : state.mSideB;
	}

	ATTR_NODISCARD std::vector<Pokemon *> &party(BattleState &state, const Side side)
	{
		return side == Side::A ? state.mPartyA : state.mPartyB;
	}

	ATTR_NODISCARD const std::vector<Pokemon *> &party(const BattleState &state, const Side side)
	{
		return side == Side::A ? state.mPartyA : state.mPartyB;
	}

	ATTR_NODISCARD bool isHealthy(const BattleSlot &slot) noexcept
	{
		return slot.mPokemon != nullptr && !slot.mPokemon->isFainted();
	}

	ATTR_NODISCARD bool isHealthy(const Pokemon *pokemon) noexcept
	{
		return pokemon != nullptr && !pokemon->isFainted();
	}

	ATTR_NODISCARD bool isActive(const BattleState &state, const Side side, const Pokemon *pokemon) noexcept
	{
		const std::vector<BattleSlot> &slots{activeSlots(state, side)};
		return std::ranges::any_of(slots, [pokemon](const BattleSlot &slot) { return slot.mPokemon == pokemon; });
	}

	ATTR_NODISCARD bool isAdjacent(const BattleSlot &source, const BattleSlot &target) noexcept
	{
		const sb positionDifference{static_cast<sb>(std::abs(static_cast<sb>(source.mPosition) - static_cast<sb>(target.mPosition)))};
		return positionDifference <= 1;
	}

	ATTR_NODISCARD bool targetExists(const BattleState &state, const BattleTarget target) noexcept
	{
		const std::vector<BattleSlot> &slots{activeSlots(state, target.mSide)};
		return target.mSlotIndex < slots.size() && isHealthy(slots.at(target.mSlotIndex));
	}

	ATTR_NODISCARD bool sideHasHealthyPokemon(const BattleState &state, const Side side) noexcept
	{
		const std::vector<Pokemon *> &sideParty{party(state, side)};
		return std::ranges::any_of(sideParty, [](const Pokemon *pokemon) { return isHealthy(pokemon); });
	}
} // namespace PocketCore::Battle
