/*! @file battleValidation.h
	@brief Defines validation functions that help with battles.
	@date 10/20/2026
	@since 0.12.51
	@version 0.12.51
	@author Matthew Moore
*/

#ifndef INCLUDE_VALIDATION_BATTLE_BATTLE_VALIDATION_H
#define INCLUDE_VALIDATION_BATTLE_BATTLE_VALIDATION_H

#include <expected>

#include "Battle/battleAction.h"
#include "Battle/battleState.h"
#include "Battle/battleValidation.h"
#include "Core/attributeMacros.h"
#include "Registry/moveRegistry.h"

#include "battleError.h"

namespace PocketCore::Validation::Battle
{
	using PocketCore::Battle::BattleAction;
	using PocketCore::Battle::BattlePhase;
	using PocketCore::Battle::BattleState;
	using PocketCore::Battle::MoveAction;
	using PocketCore::Battle::SwitchAction;
	using PocketCore::Registry::Move::MoveRegistry;

	/*! @brief Validates whether a Pokemon can enter the selected active slot.
		@details Checks the battle result, active and party indexes, incoming Pokemon health, and whether the incoming Pokemon is already
	   active without mutating battle state.
		@param[in] state The state of the battle.
		@param[in] action The switch action to validate.
		@return Void when the switch is valid, or the first applicable @ref BattleEngineError.
		@since 0.10.4
		@version 0.12.51
	*/
	ATTR_NODISCARD ATTR_PURE std::expected<void, BattleEngineError> validateSwitchAction(const BattleState &state,
																						 const SwitchAction &action);

	/*! @brief Validates whether a move action can be accepted in the current battle state.
		@details Checks the battle phase and result, acting slot, move metadata, remaining PP, and target selection without mutating battle
	   state.
		@pre @p moveRegistry is non-null.
		@param[in] state The state of the battle.
		@param[in] action The move action to validate.
		@param[in] phase The phase of the battle.
		@param[in] moveRegistry The move registry to grab the move metadata from.
		@return Void when the action is valid, or the first applicable @ref BattleEngineError.
		@since 0.10.4
		@version 0.12.51
	*/
	ATTR_NODISCARD std::expected<void, BattleEngineError> validateMoveAction(const BattleState &state, const MoveAction &action,
																			 const BattlePhase phase, const MoveRegistry *moveRegistry);

	/*! @brief Validates the concrete move or switch action stored in a battle-action variant.
		@details Dispatches to @ref validateMoveAction for move actions and @ref validateSwitchAction for switch actions without mutating
	   battle state.
		@pre @p moveRegistry is non-null when @p action contains a @ref MoveAction.
		@param[in] state The current battle state.
		@param[in] action The variant action to validate.
		@param[in] phase The current battle input phase.
		@param[in] moveRegistry The non-owning move registry used for move validation.
		@return Void when the concrete action is valid, or its first applicable @ref BattleEngineError.
		@since 0.10.7
		@version 0.12.51
	*/
	ATTR_NODISCARD std::expected<void, BattleEngineError> getValidationResult(const BattleState &state, const BattleAction &action,
																			  const BattlePhase phase, const MoveRegistry *moveRegistry);
} // namespace PocketCore::Validation::Battle

#endif
