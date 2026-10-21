/*! @file battleValidation.h
	@brief Defines validation methods for battling Pokemon and their actions.
	@date 10/20/2026
	@since 0.10.2
	@version 0.12.51
	@author Matthew Moore
*/

#ifndef INCLUDE_BATTLE_BATTLE_VALIDATION_H
#define INCLUDE_BATTLE_BATTLE_VALIDATION_H

#include "Core/typedefs.h"

#include "battleAction.h"

namespace PocketCore::Battle
{
	using PocketCore::Core::ub;

	/*! @enum BattleResult
		@showenumvalues
		@brief Identifies whether a battle is ongoing or which trainer has won.
		@date 08/28/2026
		@since 0.10.2
		@version 0.12.9
		@author Matthew Moore
	*/
	enum class BattleResult : ub
	{
		/*! @brief Indicates the battle has not been started. */
		NotStarted,
		/*! @brief Indicates the battle is still being fought. */
		InProgress,
		/*! @brief Indicates side A won the battle. */
		SideAWon,
		/*! @brief Indicates side B won the battle. */
		SideBWon,
		/*! @brief Indicates the battle ended without a winner. */
		Draw,
	};

	/*! @enum BattlePhase
		@showenumvalues
		@brief Identifies the input or terminal phase of the battle state machine.
		@date 08/28/2026
		@since 0.10.2
		@version 0.12.9
		@author Matthew Moore
	*/
	enum class BattlePhase : ub
	{
		/*! @brief Indicates the battle has not been started. */
		NotStarted,
		/*! @brief Indicates the engine is waiting for each side to submit its actions. */
		AwaitingActions,
		/*! @brief Indicates the engine is waiting for fainted Pokemon to be replaced. */
		AwaitingReplacements,
		/*! @brief Indicates the battle has reached a terminal state and accepts no further input. */
		Finished,
	};
} // namespace PocketCore::Battle

#endif
