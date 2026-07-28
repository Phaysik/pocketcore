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

#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Effect/effectType.h"
#include "Move/builtinMoveID.h"
#include "Move/constants.h"
#include "Move/moveID.h"
#include "Move/moveMeta.h"
#include "Move/moveTargetsAndTriggers.h"
#include "Registry/fixedMetadataRegistry.h"

namespace PocketCore::Registry::Move
{
	using PocketCore::Configuration::MAX_MOVES;
	using PocketCore::Core::us;
	using PocketCore::Effect::EffectTypeID;
	using PocketCore::Move::baseAttackEffects;
	using PocketCore::Move::BuiltinMoveID;
	using PocketCore::Move::MoveEffectTrigger;
	using PocketCore::Move::MoveID;
	using PocketCore::Move::MoveMeta;
	using PocketCore::Move::MoveTriggerID;
	using PocketCore::Move::toMoveID;

	/*! @class MoveRegistry Registry/moveRegistry.h
		@brief Stores built-in and user-defined move metadata in fixed-capacity storage.
		@details Built-in moves are registered during construction with IDs derived from @ref BuiltinMoveID. Configuration code may
	   append, replace, or remove entries through the low-level mutators while battle-time callers use allocation-free lookup operations.
		@note Lookup operations are O(n), where n is bounded by @ref MAX_MOVES.
		@date 07/27/2026
		@version x.x.x
		@since x.x.x
		@author Matthew Moore
	*/
	class MoveRegistry : private PocketCore::Registry::FixedMetadataRegistry<MoveMeta, MoveID, MAX_MOVES, &MoveMeta::mMoveID>
	{
		private:
			using Base = PocketCore::Registry::FixedMetadataRegistry<MoveMeta, MoveID, MAX_MOVES, &MoveMeta::mMoveID>;

		public:
			/*! @brief Constructs a registry populated with every @ref BuiltinMoveID. */
			ATTR_NOINLINE explicit constexpr MoveRegistry() : Base{static_cast<us>(toMoveID(BuiltinMoveID::KarateChop).getValue() + 1U)}
			{
				addBuiltin({.mTriggers = {}, .mName = PocketCore::Move::MOVE_NAME_NONE, .mMoveID = toMoveID(BuiltinMoveID::None)});
				addBuiltin({
					.mTriggers = {{.mEffects = {Move::baseAttackEffects()}, .mTrigger = MoveTriggerID::OnUse}},
					.mName = PocketCore::Move::MOVE_NAME_POUND,
					.mMoveID = toMoveID(BuiltinMoveID::Pound),
				});
				addBuiltin({
					.mTriggers = {{.mEffects = {Move::baseAttackEffects()}, .mTrigger = MoveTriggerID::OnUse}},
					.mName = PocketCore::Move::MOVE_NAME_KARATE_CHOP,
					.mMoveID = toMoveID(BuiltinMoveID::KarateChop),
				});
			}

			using Base::decrementAmountRegistered;
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
