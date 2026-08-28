/*! @file battleValidation.h
	@brief Defines validation methods for battling Pokemon and their actions.
	@date 08/13/2026
	@since 0.10.2
	@version 0.10.2
	@author Matthew Moore
*/

#ifndef INCLUDE_BATTLE_BATTLE_VALIDATION_H
#define INCLUDE_BATTLE_BATTLE_VALIDATION_H

#include "Core/typedefs.h"

#include "battleAction.h"

namespace PocketCore::Battle
{
	using PocketCore::Core::ub;

	/*! @enum BattleEngineError
		@showenumvalues
		@brief Identifies why a battle engine request was rejected.
		@details The engine reports these values through `std::expected` when validating battle startup, submitted actions, targets, and
			replacements. A rejected request leaves the battle state unchanged.
		@note All enum values must be handled exhaustively when reporting engine failures.
		@date 08/13/2026
		@since 0.10.2
		@version 0.10.2
		@author Matthew Moore
	*/
	enum class BattleEngineError : ub
	{
		/*! @brief Indicates the battle has already been started. */
		BattleAlreadyStarted,
		/*! @brief Indicates the request requires a battle that has been started. */
		BattleNotStarted,
		/*! @brief Indicates the battle has already finished. */
		BattleFinished,
		/*! @brief Indicates a party is empty or otherwise unusable. */
		InvalidParty,
		/*! @brief Indicates the referenced active battle slot does not exist. */
		InvalidActiveSlot,
		/*! @brief Indicates the referenced party index does not exist. */
		InvalidPartyIndex,
		/*! @brief Indicates the referenced move slot does not exist. */
		InvalidMoveSlot,
		/*! @brief Indicates the selected target is not legal for the action. */
		InvalidTarget,
		/*! @brief Indicates an action was already submitted for the slot. */
		DuplicateAction,
		/*! @brief Indicates a fainted Pokemon must be replaced before continuing. */
		ReplacementRequired,
		/*! @brief Indicates the referenced Pokemon has fainted. */
		PokemonFainted,
		/*! @brief Indicates the referenced Pokemon already occupies an active slot. */
		PokemonAlreadyActive,
		/*! @brief Indicates the referenced move is not registered. */
		MoveNotFound,
		/*! @brief Indicates the referenced ability is not registered. */
		AbilityNotFound,
		/*! @brief Indicates the referenced item is not registered. */
		ItemNotFound,
		/*! @brief Indicates the selected move has no remaining power points. */
		NoPP,
		/*! @brief Indicates a registry required by the request is unavailable. */
		MissingRegistry,
	};

	/*! @enum BattleResult
		@showenumvalues
		@brief Identifies whether a battle is ongoing or which trainer has won.
		@date 08/12/2026
		@since 0.10.2
		@version x.x.x
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
		@date 08/12/2026
		@since 0.10.2
		@version 0.10.2
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
