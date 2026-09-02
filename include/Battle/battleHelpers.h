/*! @file battleHelpers.h
	@brief Declares helper functions for battle actions.
	@date 09/02/2026
	@since 0.9.14
	@version 0.12.17
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

	/*! @brief Returns the side opposing the supplied battle side.
		@param[in] side The battle side to invert.
		@return @ref Side::B for @ref Side::A; otherwise @ref Side::A.
		@note Supports compile-time and runtime evaluation.
		@since 0.9.14
		@version 0.10.9
	*/
	ATTR_NODISCARD constexpr Side getOppositeSide(const Side side) noexcept
	{
		return side == Side::A ? Side::B : Side::A;
	}

	/*! @brief Returns the deterministic ordering key for a battle side.
		@param[in] side The side whose ordering key is requested.
		@return Zero for @ref Side::A and one for every other side value.
		@note Supports compile-time and runtime evaluation.
		@since 0.9.14
		@version 0.10.9
	*/
	ATTR_NODISCARD constexpr std::size_t getSideOrder(const Side side) noexcept
	{
		return side == Side::A ? 0U : 1U;
	}

	/*! @brief Checks whether a party contains a null Pokemon pointer.
		@param[in] party The non-owning party pointer span to inspect.
		@return True when at least one element is nullptr; otherwise false.
		@note Time complexity is O(n), where n is @p party.size(). Supports compile-time and runtime evaluation.
		@since 0.10.3
		@version 0.10.9
	*/
	ATTR_NODISCARD constexpr bool anyPartyPokemonNull(const std::span<Pokemon *const> &party)
	{
		return std::ranges::any_of(party, [](const Pokemon *pokemon) { return pokemon == nullptr; });
	}

	/*! @brief Selects the mutable active-slot collection for a side.
		@param[in,out] state The battle state containing both active-slot collections.
		@param[in] side The side to select.
		@return The mutable side-A collection for @ref Side::A; otherwise the mutable side-B collection. The reference remains valid while
	   @p state exists.
		@since 0.9.14
		@version 0.10.9
	*/
	ATTR_NODISCARD ATTR_CONST std::vector<BattleSlot> &activeSlots(BattleState &state, const Side side);

	/*! @overload const std::vector<BattleSlot> &activeSlots(const BattleState &state, Side side)
		@brief Selects the read-only active-slot collection for a side.
		@param[in] state The battle state containing both active-slot collections.
		@param[in] side The side to select.
		@return The read-only side-A collection for @ref Side::A; otherwise the read-only side-B collection. The reference remains valid
	   while @p state exists.
		@since 0.9.14
		@version 0.10.9
	*/
	ATTR_NODISCARD ATTR_CONST const std::vector<BattleSlot> &activeSlots(const BattleState &state, const Side side);

	/*! @brief Finds a mutable active slot from effect-context coordinates.
		@param[in,out] state The battle state containing the requested slot.
		@param[in] side The side containing the slot.
		@param[in] slotIndex The zero-based active-slot index.
		@return A non-owning pointer to the slot, or nullptr when @p slotIndex is outside the selected side. A non-null pointer remains
	   valid until the selected slot vector reallocates.
		@since 0.9.16
		@version 0.10.9
	*/
	ATTR_NODISCARD ATTR_PURE BattleSlot *contextSlot(BattleState &state, const Side side, const ub slotIndex) noexcept;

	/*! @brief Selects the mutable party collection for a side.
		@param[in,out] state The battle state containing both parties.
		@param[in] side The side to select.
		@return The mutable side-A party for @ref Side::A; otherwise the mutable side-B party. The reference remains valid while @p state
	   exists.
		@since 0.9.14
		@version 0.10.9
	*/
	ATTR_NODISCARD ATTR_CONST std::vector<Pokemon *> &party(BattleState &state, const Side side);

	/*! @overload const std::vector<Pokemon *> &party(const BattleState &state, Side side)
		@brief Selects the read-only party collection for a side.
		@param[in] state The battle state containing both parties.
		@param[in] side The side to select.
		@return The read-only side-A party for @ref Side::A; otherwise the read-only side-B party. The reference remains valid while @p
	   state exists.
		@since 0.9.14
		@version 0.10.9
	*/
	ATTR_NODISCARD ATTR_CONST const std::vector<Pokemon *> &party(const BattleState &state, const Side side);

	/*! @brief Checks whether an active slot contains a non-fainted Pokemon.
		@param[in] slot The active slot to inspect.
		@return True when the slot has a non-null Pokemon that has not fainted; otherwise false.
		@since 0.9.14
		@version 0.10.9
	*/
	ATTR_NODISCARD ATTR_PURE bool isHealthy(const BattleSlot &slot) noexcept;

	/*! @overload bool isHealthy(const Pokemon *pokemon)
		@brief Checks whether a Pokemon pointer identifies a non-fainted Pokemon.
		@param[in] pokemon The non-owning pointer to inspect; may be nullptr.
		@return True when @p pokemon is non-null and has not fainted; otherwise false.
		@since 0.9.14
		@version 0.10.9
	*/
	ATTR_NODISCARD ATTR_PURE bool isHealthy(const Pokemon *pokemon) noexcept;

	/*! @brief Checks whether a Pokemon occupies an active slot on a side.
		@param[in] state The battle state to inspect.
		@param[in] side The side whose active slots are searched.
		@param[in] pokemon The non-owning Pokemon identity to find; may be nullptr.
		@return True when any selected active slot stores @p pokemon; otherwise false.
		@note Time complexity is O(a), where a is the number of active slots on @p side.
		@since 0.9.14
		@version 0.10.9
	*/
	ATTR_NODISCARD ATTR_PURE bool isActive(const BattleState &state, const Side side, const Pokemon *pokemon) noexcept;

	/*! @brief Checks whether two battlefield positions are within one position of each other.
		@param[in] source The source slot supplying the first position.
		@param[in] target The target slot supplying the second position.
		@return True when the absolute position difference is zero or one; otherwise false.
		@since 0.9.14
		@version 0.10.9
	*/
	ATTR_NODISCARD ATTR_PURE bool isAdjacent(const BattleSlot &source, const BattleSlot &target) noexcept;

	/*! @brief Checks whether a target identifies a healthy occupied active slot.
		@param[in] state The battle state to inspect.
		@param[in] target The side and zero-based active-slot index to resolve.
		@return True when the slot index exists and contains a non-fainted Pokemon; otherwise false.
		@since 0.9.14
		@version 0.10.9
	*/
	ATTR_NODISCARD ATTR_PURE bool targetExists(const BattleState &state, const BattleTarget target) noexcept;

	/*! @brief Checks whether a party has at least one healthy Pokemon.
		@param[in] state The battle state containing the party.
		@param[in] side The party side to inspect.
		@return True when at least one party pointer is non-null and identifies a non-fainted Pokemon; otherwise false.
		@note Time complexity is O(n), where n is the selected party size.
		@since 0.9.14
		@version 0.10.9
	*/
	ATTR_NODISCARD ATTR_PURE bool sideHasHealthyPokemon(const BattleState &state, const Side side) noexcept;

	/*! @brief Calculates the speed used to order an active slot's move.
		@details Multiplies the occupant's base speed by the cached speed-stage multiplier and the slot's damage-formula speed modifier.
		@param[in] slot The active slot whose effective speed is requested.
		@return The calculated effective speed, or 0.0 when the slot has no Pokemon.
		@since 0.9.16
		@version 0.10.9
	*/
	ATTR_NODISCARD ATTR_PURE double getEffectiveSpeed(const BattleSlot &slot) noexcept;

	/*! @brief Creates the initial effect context for one move target and hit attempt.
		@details Copies move identity, type, power, accuracy, range, damage category, user coordinates, target coordinates, and hit-attempt
	   index into a new context.
		@param[in] action The move action identifying the user and selected move slot.
		@param[in] moveMeta The resolved metadata for the selected move.
		@param[in] target The active slot receiving the move.
		@param[in] hitAttemptIndex The hit-attempt index stored in the context.
		@return A new move-sourced effect context initialized from the supplied values.
		@since 0.9.16
		@version 0.10.9
	*/
	ATTR_NODISCARD ATTR_PURE EffectContext makeMoveContext(const MoveAction &action, const MoveMeta &moveMeta, const BattleTarget target,
														   const ub hitAttemptIndex);

	/*! @brief Applies recoil from a completed damage context to the move user.
		@details Floors damage multiplied by the recoil ratio, clamps positive recoil to the unsigned health range, and saturates the user's
	   health at zero. No recoil is applied for a missing user, zero damage, or a non-finite or non-positive ratio.
		@param[in,out] state The battle state containing the move user whose health may change.
		@param[in] context The completed effect context supplying user coordinates, dealt damage, and recoil ratio.
		@since 0.9.16
		@version 0.12.14
	*/
	void applyRecoil(BattleState &state, const EffectContext &context);

	/*! @brief Resolves the number of hit attempts selected by a move's hit-count policy.
		@details Returns a fixed policy's count directly. For weighted policies, ignores non-finite and non-positive weights and samples one
	   positive outcome using @ref Random.
		@param[in] moveMeta The move metadata containing the hit-count policy.
		@return The fixed or randomly selected hit count, or zero when a weighted policy has no valid positive total weight.
		@note Weighted resolution advances the shared random-number generator state.
		@since 0.9.16
		@version 0.10.9
	*/
	ATTR_NODISCARD ub resolveHitCount(const MoveMeta &moveMeta);

	/*! @brief Checks for duplicate Pokemon identities within or across two parties.
		@param[in] partyA The first non-owning party pointer span.
		@param[in] partyB The second non-owning party pointer span.
		@return True when any pointer value occurs more than once in the combined parties; otherwise false.
		@note Time complexity is O(n log n) and auxiliary space is O(n), where n is the combined party size.
		@since 0.10.3
		@version 0.10.9
	*/
	ATTR_NODISCARD bool hasDuplicatePokemonPointers(const std::span<Pokemon *const> &partyA, const std::span<Pokemon *const> &partyB);

	/*! @brief Counts healthy Pokemon in a party.
		@param[in] party The non-owning party pointer span to inspect.
		@return The number of non-null, non-fainted Pokemon pointers.
		@note Time complexity is O(n), where n is @p party.size().
		@since 0.10.3
		@version 0.10.9
	*/
	ATTR_NODISCARD ATTR_PURE sl healthyPokemonInParty(const std::span<Pokemon *const> &party);

	/*! @brief Appends the first healthy party members to active slots in party order.
		@details Assigns contiguous positions from the current slot count and stops when @p activePokemonPerSide slots are populated.
	   Fainted and null party entries are skipped.
		@pre @p activePokemonPerSide > 0 and @p slots.size() < @p activePokemonPerSide.
		@param[in] party The non-owning party pointer span searched in declaration order.
		@param[in,out] slots The active-slot collection receiving selected Pokemon.
		@param[in] activePokemonPerSide The total number of active slots to populate.
		@since 0.10.3
		@version 0.10.9
	*/
	void assignActiveSlots(const std::span<Pokemon *const> party, std::vector<BattleSlot> &slots, const ub activePokemonPerSide);

	/*! @brief Checks whether a candidate is a healthy target within the requested formation range.
		@pre @p source identifies an existing active slot when @p rangeID is not @ref BattleRangeID::Unrestricted.
		@param[in] state The battle state containing the source and candidate slots.
		@param[in] source The active slot from which range is measured.
		@param[in] candidate The candidate active slot to validate.
		@param[in] rangeID The formation-range rule to apply.
		@return False when @p candidate does not exist; otherwise true for unrestricted range or when source and candidate positions are
	   adjacent.
		@since 0.10.3
		@version 0.10.9
	*/
	ATTR_NODISCARD bool canTarget(const BattleState &state, const BattleTarget &source, const BattleTarget &candidate,
								  const BattleRangeID rangeID);

	/*! @brief Appends every eligible target on one side in active-slot order.
		@pre @p source identifies an existing active slot when @p rangeID is not @ref BattleRangeID::Unrestricted.
		@param[in,out] targets The target collection to append to; existing entries are preserved.
		@param[in] state The battle state containing candidate slots.
		@param[in] source The active slot from which range is measured.
		@param[in] rangeID The formation-range rule applied to each candidate.
		@param[in] side The side whose active slots are considered.
		@since 0.10.3
		@version 0.10.9
	*/
	void appendSide(std::vector<BattleTarget> &targets, const BattleState &state, const BattleTarget &source, const BattleRangeID rangeID,
					const Side side);

	/*! @brief Derives the current battle result from party health.
		@param[in] state The battle state whose parties are inspected.
		@return @ref BattleResult::NotStarted before startup, @ref BattleResult::InProgress while both parties have a healthy Pokemon, the
	   surviving side's win result, or @ref BattleResult::Draw when neither side has one.
		@note Time complexity is O(a + b), where a and b are the party sizes.
		@since 0.10.3
		@version 0.10.9
	*/
	ATTR_NODISCARD ATTR_PURE BattleResult getResult(const BattleState &state) noexcept;

	/*! @brief Checks whether a side has a healthy Pokemon that is not currently active.
		@param[in] state The battle state used to identify active Pokemon.
		@param[in] side The side whose active slots are checked.
		@param[in] trainerParty The non-owning party pointers searched for a reserve.
		@return True when @p trainerParty contains a healthy Pokemon not active on @p side; otherwise false.
		@note Time complexity is O(p * a), where p is the party size and a is the active-slot count.
		@since 0.10.3
		@version 0.10.10
	*/
	ATTR_NODISCARD ATTR_PURE bool hasReserve(const BattleState &state, const Side side, const std::span<Pokemon *const> &trainerParty);

	/*! @brief Resolves the active targets selected by a move action.
		@details Resolves the acting slot and its registered move metadata, then delegates selector and range behavior to @ref
	   resolveTargets.
		@pre @p moveRegistry is non-null and @p action contains an active-slot index valid for its side.
		@param[in] state The state of the battle.
		@param[in] action The move action whose metadata supplies the target selector.
		@param[in] moveRegistry The non-owning move registry used to resolve metadata; must not be nullptr.
		@return The selected active slots in deterministic side and slot order, or @ref BattleEngineError::InvalidActiveSlot, @ref
	   BattleEngineError::MoveNotFound, or a target-resolution error.
		@since 0.10.4
		@version 0.12.17
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
		@since 0.10.3
		@version 0.10.4
	*/
	ATTR_NODISCARD std::expected<std::vector<BattleTarget>, BattleEngineError> resolveTargets(
		const BattleState &state, const Side sourceSide, const ub sourceSlotIndex, const BattleTargetID targetID,
		const BattleRangeID rangeID, const std::optional<BattleTarget> &selectedTarget);

	/*! @brief Validates whether a Pokemon can enter the selected active slot.
		@details Checks the battle result, active and party indexes, incoming Pokemon health, and whether the incoming Pokemon is already
	   active without mutating battle state.
		@param[in] state The state of the battle.
		@param[in] action The switch action to validate.
		@return Void when the switch is valid, or the first applicable @ref BattleEngineError.
		@since 0.10.4
		@version 0.10.9
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
		@version 0.12.17
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
		@version 0.10.9
	*/
	ATTR_NODISCARD std::expected<void, BattleEngineError> getValidationResult(const BattleState &state, const BattleAction &action,
																			  const BattlePhase phase, const MoveRegistry *moveRegistry);

	/*! @brief Returns the active slot that owns a battle action.
		@param[in] action The move or switch action whose actor is requested.
		@return The move user's side and slot index for a @ref MoveAction, or the replaced side and active-slot index for a @ref
	   SwitchAction.
		@since 0.10.7
		@version 0.10.9
	*/
	ATTR_NODISCARD ATTR_PURE BattleTarget getBattleTarget(const BattleAction &action);

	/*! @brief Randomizes exact ties and orders move actions by priority and effective speed.
		@details Shuffles first, then performs a stable descending sort by registered move priority and effective speed. Missing metadata is
	   ordered after present metadata; exact ties retain their randomized order.
		@pre @p moveRegistry is non-null, and each action identifies an existing occupied slot and valid move index.
		@param[in] state The battle state supplying active occupants and effective-speed modifiers.
		@param[in,out] moves The move actions to shuffle and sort in execution order.
		@param[in] moveRegistry The non-owning registry used to resolve move priorities; must not be nullptr.
		@note Advances the shared random-number generator state. Sorting performs O(m log m) comparisons for m actions.
		@since 0.10.7
		@version 0.10.9
	*/
	void handleMovePrioritization(const BattleState &state, std::vector<MoveAction> &moves, const MoveRegistry *moveRegistry);
} // namespace PocketCore::Battle

#endif
