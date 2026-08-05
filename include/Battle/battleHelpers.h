/*! @file battleHelpers.h
	@brief Declares helper functions for battle actions.
	@date 08/05/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_BATTLE_BATTLE_HELPERS_H
#define INCLUDE_BATTLE_BATTLE_HELPERS_H

#include <cstddef>
#include <vector>

#include "Battle/battleEngine_copy.h"
#include "Battle/battleState.h"
#include "Core/attributeMacros.h"
#include "Effect/effectContext.h"

namespace PocketCore::Battle
{
	using PocketCore::Battle::BattleSlot;
	using PocketCore::Battle::BattleTarget;
	using PocketCore::Effect::Side;

	ATTR_NODISCARD constexpr Side getOppositeSide(const Side side) noexcept
	{
		return side == Side::A ? Side::B : Side::A;
	}

	ATTR_NODISCARD constexpr std::size_t getSideOrder(const Side side) noexcept
	{
		return side == Side::A ? 0U : 1U;
	}

	ATTR_NODISCARD std::vector<BattleSlot> &activeSlots(BattleState &state, const Side side);

	ATTR_NODISCARD const std::vector<BattleSlot> &activeSlots(const BattleState &state, const Side side);

	ATTR_NODISCARD std::vector<Pokemon *> &party(BattleState &state, const Side side);

	ATTR_NODISCARD const std::vector<Pokemon *> &party(const BattleState &state, const Side side);

	ATTR_NODISCARD bool isHealthy(const BattleSlot &slot) noexcept;

	ATTR_NODISCARD bool isHealthy(const Pokemon *pokemon) noexcept;

	ATTR_NODISCARD bool isActive(const BattleState &state, const Side side, const Pokemon *pokemon) noexcept;

	ATTR_NODISCARD bool isAdjacent(const BattleSlot &source, const BattleSlot &target) noexcept;

	ATTR_NODISCARD bool targetExists(const BattleState &state, const BattleTarget target) noexcept;

	ATTR_NODISCARD bool sideHasHealthyPokemon(const BattleState &state, const Side side) noexcept;
} // namespace PocketCore::Battle

#endif
