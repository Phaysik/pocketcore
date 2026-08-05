/*! @file itemTargetsAndTriggers.h
	@brief Contains the item targets and triggers
	@date 07/24/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_ITEM_TARGETSANDTRIGGERS_H
#define INCLUDE_ITEM_TARGETSANDTRIGGERS_H

#include "Core/typedefs.h"

namespace PocketCore::Item
{
	enum class ItemTriggerID : PocketCore::Core::ub
	{
		OnSwitchIn,
		OnUserDamageCalc,
		OnTargetDamageCalc,
		OnTurnEnd,
		OnFaint,
		OnMoveUse,
		OnMoveUseTarget,
	};

	enum class ItemTargetID : PocketCore::Core::ub
	{
		Self,
		AllAllies,
		AllOpponents,
		SingleOpponent,
	};
} // namespace PocketCore::Item

#endif
