/*! @file battleHelpers.h
	@brief Declares helper functions for battle actions.
	@date 08/05/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_BATTLE_BATTLE_HELPERS_H
#define INCLUDE_BATTLE_BATTLE_HELPERS_H

#include <algorithm>
#include <cstddef>
#include <expected>
#include <span>
#include <vector>

#include "Battle/battleState.h"
#include "Battle/battleValidation.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Effect/effectContext.h"
#include "Move/moveMeta.h"
#include "Registry/moveRegistry.h"

#include "battleAction.h"
#include "battleValidation.h"

namespace PocketCore::Battle
{
	using PocketCore::Battle::BattleSlot;
	using PocketCore::Core::sl;
	using PocketCore::Core::ub;
	using PocketCore::Effect::EffectContext;
	using PocketCore::Effect::Side;
	using PocketCore::Move::MoveMeta;
	using PocketCore::Registry::Move::MoveRegistry;

	ATTR_NODISCARD constexpr Side getOppositeSide(const Side side) noexcept
	{
		return side == Side::A ? Side::B : Side::A;
	}

	ATTR_NODISCARD constexpr std::size_t getSideOrder(const Side side) noexcept
	{
		return side == Side::A ? 0U : 1U;
	}

	ATTR_NODISCARD constexpr bool anyPartyPokemonNull(const std::span<Pokemon *const> &party)
	{
		return std::ranges::any_of(party, [](const Pokemon *pokemon) { return pokemon == nullptr; });
	}

	ATTR_NODISCARD ATTR_CONST std::vector<BattleSlot> &activeSlots(BattleState &state, const Side side);

	ATTR_NODISCARD ATTR_CONST const std::vector<BattleSlot> &activeSlots(const BattleState &state, const Side side);

	ATTR_NODISCARD ATTR_PURE BattleSlot *contextSlot(BattleState &state, const Side side, const ub slotIndex) noexcept;

	ATTR_NODISCARD ATTR_CONST std::vector<Pokemon *> &party(BattleState &state, const Side side);

	ATTR_NODISCARD ATTR_CONST const std::vector<Pokemon *> &party(const BattleState &state, const Side side);

	ATTR_NODISCARD ATTR_PURE bool isHealthy(const BattleSlot &slot) noexcept;

	ATTR_NODISCARD ATTR_PURE bool isHealthy(const Pokemon *pokemon) noexcept;

	ATTR_NODISCARD ATTR_PURE bool isActive(const BattleState &state, const Side side, const Pokemon *pokemon) noexcept;

	ATTR_NODISCARD ATTR_PURE bool isAdjacent(const BattleSlot &source, const BattleSlot &target) noexcept;

	ATTR_NODISCARD ATTR_PURE bool targetExists(const BattleState &state, const BattleTarget target) noexcept;

	ATTR_NODISCARD ATTR_PURE bool sideHasHealthyPokemon(const BattleState &state, const Side side) noexcept;

	ATTR_NODISCARD ATTR_PURE double getEffectiveSpeed(const BattleSlot &slot) noexcept;

	ATTR_NODISCARD ATTR_PURE EffectContext makeMoveContext(const MoveAction &action, const MoveMeta &moveMeta, const BattleTarget target,
														   const ub hitAttemptIndex);

	void applyRecoil(BattleState &state, const EffectContext &context);

	ATTR_NODISCARD ub resolveHitCount(const MoveMeta &moveMeta);

	ATTR_NODISCARD bool hasDuplicatePokemonPointers(const std::span<Pokemon *const> &partyA, const std::span<Pokemon *const> &partyB);

	ATTR_NODISCARD ATTR_PURE sl healthyPokemonInParty(const std::span<Pokemon *const> &party);

	void assignActiveSlots(const std::span<Pokemon *const> party, std::vector<BattleSlot> &slots, const ub activePokemonPerSide);

	ATTR_NODISCARD bool canTarget(const BattleState &state, const BattleTarget &source, const BattleTarget &candidate,
								  const BattleRangeID rangeID);

	void appendSide(std::vector<BattleTarget> &targets, const BattleState &state, const BattleTarget &source, const BattleRangeID rangeID,
					const Side side);

	ATTR_NODISCARD ATTR_PURE BattleResult getResult(const BattleState &state) noexcept;

	ATTR_NODISCARD ATTR_PURE bool hasReserve(const BattleState &state, const Side side, const std::vector<Pokemon *> &trainerParty);

	/*! @brief Resolves the active targets selected by a move action.
		@param[in] state The state of the battle.
		@param[in] action The move action whose metadata supplies the target selector.
		@param[in] moveRegistry The move registry to grab the move metadata from.
		@return The selected active slots in deterministic side and slot order, or a validation error.
	*/
	ATTR_NODISCARD std::expected<std::vector<BattleTarget>, BattleEngineError> getMoveTargets(const BattleState &state,
																							  const MoveAction &action,
																							  const MoveRegistry *moveRegistry);

	/*! @brief Expands a target selector into concrete, eligible active slots.
		@details Applies occupancy and formation-range checks and preserves deterministic side and active-slot ordering for
	   multi-target selectors. A single-opponent selector requires an explicit eligible selection unless exactly one opponent is
	   eligible.
		@param[in] state The state of the battle.
		@param[in] sourceSide The side containing the source slot.
		@param[in] sourceSlotIndex The active-slot index of the source.
		@param[in] targetID The selector that determines which sides and slots are considered.
		@param[in] rangeID The formation range applied between the source and each candidate, or @ref BattleRangeID::Unrestricted.
		@param[in] selectedTarget The optional explicit target used by selectors that require one.
		@return The non-empty list of eligible targets, or @ref BattleEngineError::InvalidActiveSlot or @ref
	   BattleEngineError::InvalidTarget.
	*/
	ATTR_NODISCARD std::expected<std::vector<BattleTarget>, BattleEngineError> resolveTargets(
		const BattleState &state, const Side sourceSide, const ub sourceSlotIndex, const BattleTargetID targetID,
		const BattleRangeID rangeID, const std::optional<BattleTarget> &selectedTarget);

	/*! @brief Validates whether a Pokemon can enter the selected active slot.
		@details Checks the battle result, active and party indexes, incoming Pokemon health, and whether the incoming Pokemon is
	   already active without mutating battle state.
		@param[in] state The state of the battle.
		@param[in] action The switch action to validate.
		@return Void when the switch is valid, or the first applicable @ref BattleEngineError.
	*/
	ATTR_NODISCARD ATTR_PURE std::expected<void, BattleEngineError> validateSwitchAction(const BattleState &state,
																						 const SwitchAction &action);

	/*! @brief Validates whether a move action can be accepted in the current battle state.
		@details Checks the battle phase and result, acting slot, move metadata, remaining PP, and target selection without mutating
	   battle state.
		@param[in] state The state of the battle.
		@param[in] action The move action to validate.
		@param[in] phase The phase of the battle.
		@param[in] moveRegistry The move registry to grab the move metadata from.
		@return Void when the action is valid, or the first applicable @ref BattleEngineError.
	*/
	ATTR_NODISCARD std::expected<void, BattleEngineError> validateMoveAction(const BattleState &state, const MoveAction &action,
																			 const BattlePhase phase, const MoveRegistry *moveRegistry);
} // namespace PocketCore::Battle

#endif
