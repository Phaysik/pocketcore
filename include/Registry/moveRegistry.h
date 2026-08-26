/*! @file moveRegistry.h
	@brief Provides fixed-capacity storage and lookup for built-in and user-defined moves.
	@date 07/27/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_REGISTRY_MOVE_REGISTRY_H
#define INCLUDE_REGISTRY_MOVE_REGISTRY_H

#include <optional>
#include <span>
#include <string_view>

#include "Battle/battleTargetsAndTriggers.h"
#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Effect/builtInEffectID.h"
#include "Move/builtInMoveID.h"
#include "Move/constants.h"
#include "Move/moveID.h"
#include "Move/moveMeta.h"
#include "Registry/fixedMetadataRegistry.h"
#include "Types/builtInTypeID.h"

namespace PocketCore::Registry::Move
{
	using PocketCore::Battle::BattleEventID;
	using PocketCore::Battle::BattleEventRole;
	using PocketCore::Battle::BattleRangeID;
	using PocketCore::Battle::BattleTargetID;
	using PocketCore::Configuration::MAX_MOVES;
	using PocketCore::Core::us;
	using PocketCore::Effect::BuiltinEffectID;
	using PocketCore::Effect::toEffectID;
	using PocketCore::Move::baseAttackEffects;
	using PocketCore::Move::BuiltinMoveID;
	using PocketCore::Move::MoveEffectTrigger;
	using PocketCore::Move::MoveID;
	using PocketCore::Move::MoveMeta;
	using PocketCore::Move::toMoveID;
	using PocketCore::Registry::FixedMetadataRegistry;
	using PocketCore::Type::BuiltInTypeID;
	using PocketCore::Type::toTypeID;

	/*! @class MoveRegistry Registry/moveRegistry.h
		@brief Stores built-in and user-defined move metadata in fixed-capacity storage.
		@details Built-in moves are registered during construction with IDs derived from @ref BuiltinMoveID. Configuration code may
	   append, replace, or remove entries through the low-level mutators while battle-time callers use allocation-free lookup
	   operations.
		@note Lookup operations are O(n), where n is bounded by @ref MAX_MOVES.
		@date 07/27/2026
		@version x.x.x
		@since x.x.x
		@author Matthew Moore
	*/
	class MoveRegistry : private FixedMetadataRegistry<MoveMeta, MoveID, MAX_MOVES, &MoveMeta::mMoveID>
	{
		private:
			using Base = FixedMetadataRegistry<MoveMeta, MoveID, MAX_MOVES, &MoveMeta::mMoveID>;

		public:
			// LCOV_EXCL_START - If the built in additions fail, the program wouldn't work anyway
			/*! @brief Constructs a registry populated with every @ref BuiltinMoveID. */
			ATTR_NOINLINE explicit constexpr MoveRegistry() : Base{toMoveID(BuiltinMoveID::FinalMove).getValue()}
			{
				// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

				addBuiltin({.mTriggers = {}, .mName = PocketCore::Move::MOVE_NAME_NONE, .mMoveID = toMoveID(BuiltinMoveID::None)});
				addBuiltin({
					.mTriggers = {{
					    .mEffects = {toEffectID(BuiltinEffectID::AccuracyCheck)},
					    .mTrigger = BattleEventID::BeforeHit,
						.mRole = BattleEventRole::Target,
					},
					{
						.mEffects = {Move::baseAttackEffects().begin(), Move::baseAttackEffects().end()},
						.mTrigger = BattleEventID::Hit,
						.mRole = BattleEventRole::Target,
					},},
					.mName = PocketCore::Move::MOVE_NAME_POUND,
					.mMoveID = toMoveID(BuiltinMoveID::Pound),
					.mTypeID = toTypeID(BuiltInTypeID::Normal),
					.mPower = 40,
					.mTargetID = BattleTargetID::SingleOpponent,
					.mRangeID = BattleRangeID::Adjacent,
					.mAccuracy = 100,
					.mPriority = 0,
					.mSpecial = false,
				});
				addBuiltin({
					.mTriggers = {{
					    .mEffects = {toEffectID(BuiltinEffectID::AccuracyCheck)},
					    .mTrigger = BattleEventID::BeforeHit,
						.mRole = BattleEventRole::Target,
					},
					{
						.mEffects = {Move::baseAttackEffects().begin(), Move::baseAttackEffects().end()},
						.mTrigger = BattleEventID::Hit,
						.mRole = BattleEventRole::Target,
					},},
					.mName = PocketCore::Move::MOVE_NAME_KARATE_CHOP,
					.mMoveID = toMoveID(BuiltinMoveID::KarateChop),
					.mTypeID = toTypeID(BuiltInTypeID::Fighting),
					.mPower = 50,
					.mTargetID = BattleTargetID::SingleOpponent,
					.mRangeID = BattleRangeID::Adjacent,
					.mAccuracy = 100,
					.mPriority = 0,
					.mSpecial = false,
				});

				// NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
			}

			// LCOV_EXCL_STOP

			using Base::decrementAmountRegistered;
			using Base::eraseEntry;
			using Base::findIndexByID;
			using Base::getAmountRegistered;
			using Base::getEntry;
			using Base::getID;
			using Base::getMetadata;
			using Base::getName;
			using Base::getNextID;
			using Base::getRegisteredEntries;
			using Base::hasEntry;
			using Base::incrementAmountRegistered;
			using Base::incrementNextID;
			using Base::setAmountRegistered;
			using Base::setEntry;
			using Base::setNextID;

			/*! @brief Looks up move metadata by stable ID.
				@param[in] moveID The stable move identifier.
				@return A non-owning pointer to metadata if registered, or nullptr otherwise. The pointer remains valid until replacement or
			   registry destruction.
			*/
			ATTR_NODISCARD constexpr const MoveMeta *getMoveMetadata(const MoveID moveID) const
			{
				return getMetadata(moveID);
			}

			/*! @brief Looks up an move ID by display name.
				@param[in] name The case-sensitive display name.
				@return The stable ID if registered, or std::nullopt otherwise.
			*/
			ATTR_NODISCARD constexpr const std::optional<MoveID> getMoveID(const std::string_view &name) const
			{
				return getID(name);
			}

			/*! @brief Looks up an move display name by stable ID.
				@param[in] moveID The stable move identifier.
				@return The display name if registered, or std::nullopt otherwise.
			*/
			ATTR_NODISCARD constexpr const std::optional<std::string_view> getMoveName(const MoveID moveID) const
			{
				return getName(moveID);
			}

			/*! @brief Returns all currently registered move definitions.
				@return A read-only span that remains valid until the registry is mutated or destroyed.
			*/
			ATTR_NODISCARD constexpr const std::span<const MoveMeta> getRegisteredMoves() const noexcept
			{
				return getRegisteredEntries();
			}

			/*! @brief Returns the next stable ID assigned to a custom move.
				@return The underlying numeric value of the next move ID.
			*/
			ATTR_NODISCARD constexpr us getNextMoveID() const noexcept
			{
				return getNextID();
			}

			/*! @brief Finds an internal array index by stable move ID.
				@param[in] moveID The stable move identifier.
				@return The internal index if registered, or std::nullopt otherwise.
			*/
			ATTR_NODISCARD constexpr const std::optional<us> findIndexByMoveID(const MoveID moveID) const
			{
				return findIndexByID(moveID);
			}

			/*! @brief Checks whether an move name is registered.
				@param[in] name The case-sensitive display name.
				@return True if the name is registered, otherwise false.
			*/
			ATTR_NODISCARD constexpr bool hasMove(const std::string_view &name) const
			{
				return hasEntry(name);
			}

			/*! @brief Checks whether an move ID is registered.
				@param[in] moveID The stable move identifier.
				@return True if the ID is registered, otherwise false.
			*/
			ATTR_NODISCARD constexpr bool hasMove(const MoveID moveID) const
			{
				return hasEntry(moveID);
			}

			/*! @brief Sets the next custom move ID counter.
				@param[in] nextID The next underlying ID value.
			*/
			constexpr void setNextMoveID(const us nextID) noexcept
			{
				setNextID(nextID);
			}

			/*! @brief Increments the next custom move ID counter. */
			constexpr void incrementNextMoveID() noexcept
			{
				incrementNextID();
			}
	};
} // namespace PocketCore::Registry::Move

#endif
