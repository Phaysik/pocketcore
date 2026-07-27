/*! @file moveTargetsAndTriggers.h
	@brief Contains the move targets and triggers
	@date 07/24/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_MOVE_TARGETSANDTRIGGERS_H
#define INCLUDE_MOVE_TARGETSANDTRIGGERS_H

#include <vector>

#include "Core/typedefs.h"
#include "Effect/effectType.h"

namespace PocketCore::Move
{
	enum class MoveTriggerID : PocketCore::Core::ub
	{
		OnUse,
		OnHazardSwitchIn,
	};

	enum class MoveTargetID : PocketCore::Core::ub
	{
		SingleOpponent,
		AllOpponents,
		AllAllies,
		Self,
		AllExceptSelf
	};

	struct MoveEffectTrigger
	{
		public:
			std::vector<PocketCore::Effect::EffectTypeID> mEffects;
			MoveTriggerID mTrigger;
	};
} // namespace PocketCore::Move

#endif
