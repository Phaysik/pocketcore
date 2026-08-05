/*! @file battleTargetsAndTriggers.h
	@brief Defines the shared battle targets, triggers, and ranges.
	@date 08/05/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_BATTLE_BATTLETARGETSANDTRIGGERS_H
#define INCLUDE_BATTLE_BATTLETARGETSANDTRIGGERS_H

#include "Core/typedefs.h"

namespace PocketCore::Battle
{
	using PocketCore::Core::ub;

	/*! @enum BattleTriggerID
		@brief Identifies an event that can trigger a battle effect.
		@date 08/05/2026
		@version x.x.x
		@since x.x.x
		@author Matthew Moore
	*/
	enum class BattleTriggerID : ub
	{
		OnBattleStart,
		OnSwitchIn,
		OnTurnEnd,
		OnDamageCalc,
		OnUserDamageCalc,
		OnTargetDamageCalc,
		OnMoveUse,
		OnMoveUseTarget,
		OnSuccessfulHit,
		OnFaint,
		OnWeather,
		OnStatus,
		OnUse,
		BeforeHit,
		OnHit,
		AfterHit,
		OnHazardSwitchIn,
	};

	/*! @enum BattleTargetID
		@brief Identifies the battlers affected by a battle effect.
		@date 08/05/2026
		@version x.x.x
		@since x.x.x
		@author Matthew Moore
	*/
	enum class BattleTargetID : ub
	{
		Self,
		AllAllies,
		AllOpponents,
		SingleOpponent,
		AllExceptSelf,
	};

	/*! @enum BattleRangeID
		@brief Identifies the positional range allowed for a battle effect.
		@date 08/05/2026
		@version x.x.x
		@since x.x.x
		@author Matthew Moore
	*/
	enum class BattleRangeID : ub
	{
		Unrestricted,
		Adjacent,
	};
} // namespace PocketCore::Battle

#endif
