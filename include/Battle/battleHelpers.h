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

#include "Battle/battleState.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Effect/effectContext.h"
#include "Move/moveMeta.h"

#include "battleAction.h"

namespace PocketCore::Battle
{
	using PocketCore::Battle::BattleSlot;
	using PocketCore::Core::ub;
	using PocketCore::Effect::EffectContext;
	using PocketCore::Effect::Side;
	using PocketCore::Move::MoveMeta;

	ATTR_NODISCARD constexpr Side getOppositeSide(const Side side) noexcept
	{
		return side == Side::A ? Side::B : Side::A;
	}

	ATTR_NODISCARD constexpr std::size_t getSideOrder(const Side side) noexcept
	{
		return side == Side::A ? 0U : 1U;
	}

	ATTR_NODISCARD ATTR_CONST std::vector<BattleSlot> &activeSlots(BattleState &state, const Side side);

	ATTR_NODISCARD ATTR_CONST const std::vector<BattleSlot> &activeSlots(const BattleState &state, const Side side);

	ATTR_NODISCARD ATTR_PURE BattleSlot *contextSlot(BattleState &state, const Side side, const ub slotIndex) noexcept;

	ATTR_NODISCARD ATTR_CONST std::vector<Pokemon *> &party(BattleState &state, const Side side);

	ATTR_NODISCARD ATTR_CONST const std::vector<Pokemon *> &party(const BattleState &state, const Side side);

	ATTR_NODISCARD ATTR_PURE bool isHealthy(const BattleSlot &slot) noexcept;

	ATTR_NODISCARD ATTR_PURE bool isHealthy(const Pokemon *pokemon) noexcept;

	ATTR_NODISCARD ATTR_PURE bool isActive(const BattleState &state, const Side side, const Pokemon *pokemon) noexcept;

	ATTR_NODISCARD ATTR_PURE bool isAdjacent(const BattleSlot &source, const BattleSlot &target) noexcept;

	ATTR_NODISCARD ATTR_PURE bool targetExists(const BattleState &state, const BattleTarget target) noexcept;

	ATTR_NODISCARD ATTR_PURE bool sideHasHealthyPokemon(const BattleState &state, const Side side) noexcept;

	ATTR_NODISCARD ATTR_PURE double getEffectiveSpeed(const BattleSlot &slot) noexcept;

	ATTR_NODISCARD ATTR_PURE EffectContext makeMoveContext(const MoveAction &action, const MoveMeta &moveMeta, const BattleTarget target,
														   const ub hitAttemptIndex);

	void applyRecoil(BattleState &state, const EffectContext &context);

	ATTR_NODISCARD ub resolveHitCount(const MoveMeta &moveMeta);
} // namespace PocketCore::Battle

#endif
