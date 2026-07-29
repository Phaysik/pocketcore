/*! @file effectHandlerHelpers.h
	@brief Contains the effect handler helpers
	@date 07/24/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_EFFECT_EFFECTHANDLERHELPERS_H
#define INCLUDE_EFFECT_EFFECTHANDLERHELPERS_H

#include <vector>

#include "Battle/battleState.h"
#include "Core/attributeMacros.h"
#include "Effect/effectContext.h"

namespace PocketCore::Effect
{
	using PocketCore::Battle::BattleSlot;
	using PocketCore::Battle::BattleState;

	ATTR_NODISCARD static inline std::vector<BattleSlot> &getTeam(BattleState &state, Side side)
	{
		return (side == Side::A) ? state.mSideA : state.mSideB;
	}

	ATTR_NODISCARD static inline const std::vector<BattleSlot> &getTeamConst(const BattleState &state, Side side)
	{

		return (side == Side::A) ? state.mSideA : state.mSideB;
	}
} // namespace PocketCore::Effect

#endif
