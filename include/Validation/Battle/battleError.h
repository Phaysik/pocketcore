/*! @file battleError.h
	@brief Contains the error codes for the Battle validation results.
	@date 10/20/2026
	@since 0.12.51
	@version 0.12.51
	@author Matthew Moore
*/

#ifndef INCLUDE_VALIDATION_BATTLE_BATTLE_ERROR_H
#define INCLUDE_VALIDATION_BATTLE_BATTLE_ERROR_H

#include "Core/typedefs.h"

namespace PocketCore::Validation::Battle
{
	using PocketCore::Core::ub;

	/*! @enum BattleEngineError
		@showenumvalues
		@brief Identifies why a battle engine request was rejected.
		@details The engine reports these values through `std::expected` when validating battle startup, submitted actions, targets, and
			replacements. A rejected request leaves the battle state unchanged.
		@note All enum values must be handled exhaustively when reporting engine failures.
		@date 10/19/2026
		@since 0.10.2
		@version 0.12.51
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
		/*! @brief Indicates the selected ruleset is invalid or unsupported. */
		InvalidRuleset,
		/*! @brief Indicates the engine is busy processing. */
		EngineBusy,
	};
} // namespace PocketCore::Validation::Battle

#endif
