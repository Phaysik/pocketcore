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
	using PocketCore::Core::ub;

	enum class MoveTriggerID : ub
	{
		OnUse,
		BeforeHit,
		OnHit,
		AfterHit,
		OnHazardSwitchIn,
	};

	enum class MoveTargetID : ub
	{
		SingleOpponent,
		AllOpponents,
		AllAllies,
		Self,
		AllExceptSelf,
	};

	enum class MoveRangeID : ub
	{
		Unrestricted,
		Adjacent,
	};

	struct MoveEffectTrigger
	{
		public:
			std::vector<PocketCore::Effect::EffectTypeID> mEffects;
			MoveTriggerID mTrigger;
	};
} // namespace PocketCore::Move

#endif
