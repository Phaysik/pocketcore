/*! @file moveRegistry.h
	@brief Provides fixed-capacity storage and lookup for built-in and user-defined moves.
	@date 09/03/2026
	@since 0.5.3
	@version 0.12.18
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
	using PocketCore::Move::MoveID;
	using PocketCore::Move::MoveMeta;
	using PocketCore::Move::toMoveID;
	using PocketCore::Registry::FixedMetadataRegistry;
	using PocketCore::Type::BuiltinTypeID;
	using PocketCore::Type::toTypeID;

	/*! @class MoveRegistry Registry/moveRegistry.h
		@brief Stores built-in and user-defined move metadata in fixed-capacity storage.
		@details Built-in moves are registered during construction with IDs derived from @ref BuiltinMoveID. Configuration code may
	   append, replace, or remove entries through the low-level mutators while battle-time callers use allocation-free lookup
	   operations.
		@note Lookup operations are O(n), where n is bounded by @ref MAX_MOVES.
		@date 09/03/2026
		@since 0.5.3
		@version 0.12.18
		@author Matthew Moore
	*/
	class MoveRegistry : private FixedMetadataRegistry<MoveMeta, MoveID, MAX_MOVES, &MoveMeta::mMoveID>
	{
		private:
			using Base = FixedMetadataRegistry<MoveMeta, MoveID, MAX_MOVES, &MoveMeta::mMoveID>;

		public:
			// LCOV_EXCL_START - If the built in additions fail, the program wouldn't work anyway
			/*! @brief Constructs a registry populated with every @ref BuiltinMoveID.
				@since 0.5.3
				@version 0.12.17
			 */
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
					.mTypeID = toTypeID(BuiltinTypeID::Normal),
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
					.mTypeID = toTypeID(BuiltinTypeID::Fighting),
					.mPower = 50,
					.mTargetID = BattleTargetID::SingleOpponent,
					.mRangeID = BattleRangeID::Adjacent,
					.mAccuracy = 100,
					.mPriority = 0,
					.mSpecial = false,
				});

				addBuiltin({
					.mName = PocketCore::Move::MOVE_NAME_FACADE,
					.mMoveID = toMoveID(BuiltinMoveID::Facade),
				});

				addBuiltin({
					.mName = PocketCore::Move::MOVE_NAME_HYDRO_STEAM,
					.mMoveID = toMoveID(BuiltinMoveID::HydroSteam),
				});

				// NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
			}

			// LCOV_EXCL_STOP

		protected:
			using Base::addEntry;
			using Base::createCheckpoint;
			using Base::decrementAmountRegistered;
			using Base::eraseEntry;
			using Base::incrementAmountRegistered;
			using Base::restoreCheckpoint;
			using Base::setAmountRegistered;
			using Base::setEntry;

		public:
			using Base::findIndexByID;
			using Base::getAmountRegistered;
			using Base::getEntry;
			using Base::getID;
			using Base::getMetadata;
			using Base::getName;
			using Base::getNextID;
			using Base::getRegisteredEntries;
			using Base::hasEntry;

			/*! @brief Looks up move metadata by stable ID.
				@param[in] moveID The stable move identifier.
				@return A non-owning pointer to metadata if registered, or nullptr otherwise. The pointer remains valid until replacement or
			   registry destruction.
				@since 0.5.3
				@version 0.5.3
			*/
			ATTR_NODISCARD constexpr const MoveMeta *getMoveMetadata(const MoveID moveID) const
			{
				return getMetadata(moveID);
			}

			/*! @brief Looks up an move ID by display name.
				@param[in] name The case-sensitive display name.
				@return The stable ID if registered, or std::nullopt otherwise.
				@since 0.5.3
				@version 0.5.3
			*/
			ATTR_NODISCARD constexpr const std::optional<MoveID> getMoveID(const std::string_view &name) const
			{
				return getID(name);
			}

			/*! @brief Looks up an move display name by stable ID.
				@param[in] moveID The stable move identifier.
				@return The display name if registered, or std::nullopt otherwise.
				@since 0.5.3
				@version 0.5.3
			*/
			ATTR_NODISCARD constexpr const std::optional<std::string_view> getMoveName(const MoveID moveID) const
			{
				return getName(moveID);
			}

			/*! @brief Returns all currently registered move definitions.
				@return A read-only span that remains valid until the registry is mutated or destroyed.
				@since 0.5.3
				@version 0.5.3
			*/
			ATTR_NODISCARD constexpr const std::span<const MoveMeta> getRegisteredMoves() const noexcept
			{
				return getRegisteredEntries();
			}

			/*! @brief Returns the next stable ID assigned to a custom move.
				@return The underlying numeric value of the next move ID.
				@since 0.5.3
				@version 0.5.3
			*/
			ATTR_NODISCARD constexpr us getNextMoveID() const noexcept
			{
				return getNextID();
			}

			/*! @brief Finds an internal array index by stable move ID.
				@param[in] moveID The stable move identifier.
				@return The internal index if registered, or std::nullopt otherwise.
				@since 0.5.3
				@version 0.5.3
			*/
			ATTR_NODISCARD constexpr const std::optional<us> findIndexByMoveID(const MoveID moveID) const
			{
				return findIndexByID(moveID);
			}

			/*! @brief Checks whether an move name is registered.
				@param[in] name The case-sensitive display name.
				@return True if the name is registered, otherwise false.
				@since 0.5.3
				@version 0.5.3
			*/
			ATTR_NODISCARD constexpr bool hasMove(const std::string_view &name) const
			{
				return hasEntry(name);
			}

			/*! @brief Checks whether an move ID is registered.
				@param[in] moveID The stable move identifier.
				@return True if the ID is registered, otherwise false.
				@since 0.5.3
				@version 0.5.3
			*/
			ATTR_NODISCARD constexpr bool hasMove(const MoveID moveID) const
			{
				return hasEntry(moveID);
			}
	};
} // namespace PocketCore::Registry::Move

#endif
