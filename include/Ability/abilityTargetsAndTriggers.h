/*! @file abilityTargetsAndTriggers.h
	@brief Contains the ability targets and triggers
	@date 07/24/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_ABILITY_TARGETSANDTRIGGERS_H
#define INCLUDE_ABILITY_TARGETSANDTRIGGERS_H

#include <vector>

#include "Core/typedefs.h"
#include "Effect/effectType.h"

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

	struct AbilityEffectTrigger
	{
		public:
			std::vector<PocketCore::Effect::EffectTypeID> mEffects;
			AbilityTriggerID mTrigger;
	};
} // namespace PocketCore::Ability

#endif
