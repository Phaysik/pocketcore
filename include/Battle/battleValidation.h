/*! @file battleValidation.h
	@brief Defines validation methods for battling Pokemon and their actions.
	@date 07/27/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_BATTLE_BATTLE_VALIDATION_H
#define INCLUDE_BATTLE_BATTLE_VALIDATION_H

#include "Core/typedefs.h"

#include "battleAction.h"

namespace PocketCore::Battle
{
	using PocketCore::Core::ub;

	enum class BattleEngineError : ub
	{
		BattleAlreadyStarted,
		BattleNotStarted,
		BattleFinished,
		InvalidParty,
		InvalidActiveSlot,
		InvalidPartyIndex,
		InvalidMoveSlot,
		InvalidTarget,
		DuplicateAction,
		ReplacementRequired,
		PokemonFainted,
		PokemonAlreadyActive,
		MoveNotFound,
		AbilityNotFound,
		ItemNotFound,
		NoPP,
		MissingRegistry,
	};

	/*! @enum BattleResult
		@showenumvalues
		@brief Identifies whether a battle is ongoing or which trainer has won.
		@date 08/05/2026
		@version x.x.x
		@since x.x.x
		@author Matthew Moore
	*/
	enum class BattleResult : ub
	{
		NotStarted,
		InProgress,
		SideAWon,
		SideBWon,
		Draw,
	};

	/*! @enum BattlePhase
		@showenumvalues
		@brief Identifies the input or terminal phase of the battle state machine.
		@date 08/05/2026
		@version x.x.x
		@since x.x.x
		@author Matthew Moore
	*/
	enum class BattlePhase : ub
	{
		NotStarted,
		AwaitingActions,
		AwaitingReplacements,
		Finished,
	};
} // namespace PocketCore::Battle

#endif
