/*! @file itemTargetsAndTriggers.h
	@brief Contains the item targets and triggers
	@date 07/24/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_ITEM_TARGETSANDTRIGGERS_H
#define INCLUDE_ITEM_TARGETSANDTRIGGERS_H

#include <vector>

#include "Core/typedefs.h"
#include "Effect/effectType.h"

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
	};

	enum class ItemTargetID : PocketCore::Core::ub
	{
		Self,
		AllAllies,
		AllOpponents,
	};

	struct ItemEffectTrigger
	{
		public:
			ItemTriggerID mTrigger;
			std::vector<PocketCore::Effect::EffectTypeID> mEffects;
	};
} // namespace PocketCore::Item

#endif
