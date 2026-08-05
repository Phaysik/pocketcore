/*! @file abilityTargetsAndTriggers.h
	@brief Contains the ability targets and triggers
	@date 07/24/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_ABILITY_TARGETSANDTRIGGERS_H
#define INCLUDE_ABILITY_TARGETSANDTRIGGERS_H

#include "Core/typedefs.h"

namespace PocketCore::Ability
{
	using PocketCore::Core::ub;

	enum class AbilityTriggerID : ub
	{
		OnBattleStart,
		OnSwitchIn,
		OnTurnEnd,
		OnDamageCalc,
		OnMoveUse,
		OnSuccessfulHit,
		OnFaint,
		OnWeather,
		OnStatus,
	};

	enum class AbilityTargetID : ub
	{
		Self,
		AllAllies,
		AllOpponents,
		AllExceptSelf,
	};
} // namespace PocketCore::Ability

#endif
