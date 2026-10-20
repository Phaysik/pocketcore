/*! @file battleValidation.cpp
	@brief Contains the function definitions for battle validation functions
	@date 10/20/2026
	@since 0.12.51
	@version 0.12.51
	@author Matthew Moore
*/

#include "Validation/Battle/battleValidation.h"

#include <expected>
#include <span>

#include "Battle/battleAction.h"
#include "Battle/battleHelpers.h"
#include "Battle/battleState.h"
#include "Battle/battleValidation.h"
#include "Core/attributeMacros.h"
#include "Move/moveID.h"
#include "Pokemon/pokemon.h"
#include "Registry/moveRegistry.h"
#include "Validation/Battle/battleError.h"

namespace PocketCore::Validation::Battle
{
	using PocketCore::Battle::activeSlots;
	using PocketCore::Battle::BattleAction;
	using PocketCore::Battle::BattlePhase;
	using PocketCore::Battle::BattleResult;
	using PocketCore::Battle::BattleSlot;
	using PocketCore::Battle::BattleState;
	using PocketCore::Battle::BattleTarget;
	using PocketCore::Battle::getResult;
	using PocketCore::Battle::isHealthy;
	using PocketCore::Battle::MoveAction;
	using PocketCore::Battle::party;
	using PocketCore::Battle::SwitchAction;
	using PocketCore::Move::MoveID;
	using PocketCore::Pokemon::Pokemon;
	using PocketCore::Registry::Move::MoveRegistry;
	using PocketCore::Validation::Battle::BattleEngineError;

	ATTR_NODISCARD ATTR_PURE std::expected<void, BattleEngineError> validateSwitchAction(const BattleState &state,
																						 const SwitchAction &action)
	{
		// Switching requires an initialized battle and an undecided result.
		if (!state.mBattleStarted)
		{
			return std::unexpected{BattleEngineError::BattleNotStarted};
		}

		if (getResult(state) != BattleResult::InProgress)
		{
			return std::unexpected{BattleEngineError::BattleFinished};
		}

		// Verify the outgoing battlefield slot befor indexing the active side.
		const std::span<const BattleSlot> slots{activeSlots(state, action.mSide)};

		if (action.mActiveSlotIndex >= slots.size())
		{
			return std::unexpected{BattleEngineError::InvalidActiveSlot};
		}

		// Verify the incoming party index independently because party and active-slot indexes are unrelated.
		const std::span<const Pokemon *const> trainerParty{party(state, action.mSide)};

		if (action.mPartyIndex >= trainerParty.size())
		{
			return std::unexpected{BattleEngineError::InvalidPartyIndex};
		}

		const Pokemon *incomingPokemon{trainerParty.at(action.mPartyIndex)};

		// Fainted reserves cannot be switched into battle.
		if (!isHealthy(incomingPokemon))
		{
			return std::unexpected{BattleEngineError::PokemonFainted};
		}

		// Prevent one Pokemon from occupying two active slots or switching into its current slot.
		if (isActive(state, action.mSide, incomingPokemon))
		{
			return std::unexpected{BattleEngineError::PokemonAlreadyActive};
		}

		return {};
	}

	ATTR_NODISCARD std::expected<void, BattleEngineError> validateMoveAction(const BattleState &state, const MoveAction &action,
																			 const BattlePhase phase, const MoveRegistry *moveRegistry)
	{
		// Move actions are meaningful only after a battle has successfully started.
		if (!state.mBattleStarted)
		{
			return std::unexpected{BattleEngineError::BattleNotStarted};
		}

		// Forced replacements must be resolved before either trainer can submit another move.
		if (phase == BattlePhase::AwaitingReplacements)
		{
			return std::unexpected{BattleEngineError::ReplacementRequired};
		}

		// No action may mutate a battle whose result has already been decided.
		if (getResult(state) != BattleResult::InProgress)
		{
			return std::unexpected{BattleEngineError::BattleFinished};
		}

		// Resolve the acting slot on the action's side and reject out-of-range indexes before access.
		const std::span<const BattleSlot> slots{activeSlots(state, action.mSide)};

		if (action.mUserSlotIndex >= slots.size())
		{
			return std::unexpected{BattleEngineError::InvalidActiveSlot};
		}

		const BattleSlot &userSlot{slots.at(action.mUserSlotIndex)};

		// A fainted active Pokemon remains in its slot until replacement, but it cannot act.
		if (!isHealthy(userSlot))
		{
			return std::unexpected{BattleEngineError::PokemonFainted};
		}

		// The selected move slot must exist in the Pokemon's fixed move array.
		const Pokemon *userPokemon{userSlot.mPokemon};

		if (action.mMoveSlotIndex >= userPokemon->getMoveIDsArray().size())
		{
			return std::unexpected{BattleEngineError::InvalidMoveSlot};
		}

		// A move is usable only when its ID is assigned and resolves to registered metadata.
		const MoveID moveID{userPokemon->getMoveID(action.mMoveSlotIndex)};

		if (moveRegistry->getMoveMetadata(moveID) == nullptr)
		{
			return std::unexpected{BattleEngineError::MoveNotFound};
		}

		// PP is checked during validation and consumed later only when the move actually executes.
		if (userPokemon->getCurrentPP(action.mMoveSlotIndex) == 0U)
		{
			return std::unexpected{BattleEngineError::NoPP};
		}

		// Resolve targets now so malformed or ambiguous selections fail before turn state is mutated.
		const std::expected<std::vector<BattleTarget>, BattleEngineError> targets{getMoveTargets(state, action, moveRegistry)};

		if (!targets.has_value())
		{
			return std::unexpected{targets.error()};
		}

		return {};
	}

	ATTR_NODISCARD std::expected<void, BattleEngineError> getValidationResult(const BattleState &state, const BattleAction &action,
																			  const BattlePhase phase, const MoveRegistry *moveRegistry)
	{
		return std::visit(
			[&state, phase, &moveRegistry](const auto &selectedAction) {
				using Action = std::decay_t<decltype(selectedAction)>;
				if constexpr (std::is_same_v<Action, MoveAction>)
				{
					return validateMoveAction(state, selectedAction, phase, moveRegistry);
				}
				else
				{
					return validateSwitchAction(state, selectedAction);
				}
			},
			action);
	}
} // namespace PocketCore::Validation::Battle
