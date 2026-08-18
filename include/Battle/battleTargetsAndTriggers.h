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

	/*! @enum BattleEventID
		@brief Identifies an event that can trigger a battle effect.
		@date 08/05/2026
		@version x.x.x
		@since x.x.x
		@author Matthew Moore
	*/
	enum class BattleEventID : ub
	{
		BattleStart,       /*!< Fires once for each initially active Pokemon when a battle begins. */
		SwitchIn,          /*!< Fires after a Pokemon enters an active battle slot. */
		TurnEnd,           /*!< Fires for active Pokemon after all executable actions in a turn resolve. */
		MoveUse,           /*!< Fires when an active Pokemon begins using a move. */
		BeforeHit,         /*!< Fires before a move attempts to hit its target. */
		DamageCalculation, /*!< Fires while calculating damage for a move hit. */
		Hit,               /*!< Fires when a move hit resolves against its target. */
		AfterHit,          /*!< Fires after a move hit and its associated effects resolve. */
		Faint,             /*!< Fires once when an active Pokemon faints. */
		WeatherChanged,    /*!< Fires when the active weather changes. */
		StatusChanged,     /*!< Fires when a Pokemon's status changes. */
	};

	/*! @enum BattleEventRole
		@brief Identifies a participant's role in a battle event.
		@date 08/18/2026
		@version x.x.x
		@since x.x.x
		@author Matthew Moore
	*/
	enum class BattleEventRole : ub
	{
		Any,    /*!< Matches an event without restricting it to a participant role. */
		User,   /*!< Matches the Pokemon that initiated the event. */
		Target, /*!< Matches a Pokemon targeted by the event. */
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
		Self,            /*!< Targets only the Pokemon that owns or uses the effect. */
		AllAllies,      /*!< Targets every active Pokemon on the source's side, including the source. */
		AllOpponents,   /*!< Targets every eligible active Pokemon on the opposing side. */
		SingleOpponent, /*!< Targets one explicitly selected or unambiguous eligible opponent. */
		AllExceptSelf,  /*!< Targets every eligible active Pokemon except the source. */
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
		Unrestricted, /*!< Allows eligible targets at any battlefield position. */
		Adjacent,     /*!< Restricts eligible targets to the source's position or a neighboring position. */
	};
} // namespace PocketCore::Battle

#endif
