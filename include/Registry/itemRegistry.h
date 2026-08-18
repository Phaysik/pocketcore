/*! @file itemRegistry.h
	@brief Provides fixed-capacity storage and lookup for built-in and user-defined items.
	@date 07/27/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_REGISTRY_ITEM_REGISTRY_H
#define INCLUDE_REGISTRY_ITEM_REGISTRY_H

#include <optional>
#include <span>
#include <string_view>

#include "Battle/battleTargetsAndTriggers.h"
#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Effect/builtInEffectID.h"
#include "Item/builtInItemID.h"
#include "Item/constants.h"
#include "Item/itemID.h"
#include "Item/itemMeta.h"
#include "Registry/fixedMetadataRegistry.h"

namespace PocketCore::Registry::Item
{
	using PocketCore::Battle::BattleEventID;
	using PocketCore::Battle::BattleTargetID;
	using PocketCore::Configuration::MAX_ITEMS;
	using PocketCore::Core::us;
	using PocketCore::Effect::BuiltinEffectID;
	using PocketCore::Item::BuiltinItemID;
	using PocketCore::Item::ItemEffectTrigger;
	using PocketCore::Item::ItemID;
	using PocketCore::Item::ItemMeta;
	using PocketCore::Item::toItemID;
	using PocketCore::Registry::FixedMetadataRegistry;

	/*! @class ItemRegistry Registry/itemRegistry.h
		@brief Stores built-in and user-defined item metadata in fixed-capacity storage.
		@details Built-in items are registered during construction with IDs derived from @ref BuiltinItemID. Configuration code may
	   append, replace, or remove entries through the low-level mutators while battle-time callers use allocation-free lookup operations.
		@note Lookup operations are O(n), where n is bounded by @ref MAX_ITEMS.
		@date 07/27/2026
		@version x.x.x
		@since x.x.x
		@author Matthew Moore
	*/
	class ItemRegistry : private FixedMetadataRegistry<ItemMeta, ItemID, MAX_ITEMS, &ItemMeta::mItemID>
	{
		private:
			using Base = FixedMetadataRegistry<ItemMeta, ItemID, MAX_ITEMS, &ItemMeta::mItemID>;

		public:
			// LCOV_EXCL_START - If the built in additions fail, the program wouldn't work anyway
			/*! @brief Constructs a registry populated with every @ref BuiltinItemID. */
			ATTR_NOINLINE explicit constexpr ItemRegistry() : Base{static_cast<us>(toItemID(BuiltinItemID::ChestoBerry).getValue() + 1U)}
			{
				addBuiltin({.mTriggers = {}, .mName = PocketCore::Item::ITEM_NAME_NONE, .mItemID = toItemID(BuiltinItemID::None)});
				addBuiltin({
					.mTriggers = {{.mEffects = {BuiltinEffectID::StatusRemove}, .mTrigger = BattleEventID::TurnEnd}},
					.mName = PocketCore::Item::ITEM_NAME_CHERI_BERRY,
					.mItemID = toItemID(BuiltinItemID::CheriBerry),
					.mTargetID = BattleTargetID::Self,
					.mIsConsumable = true,
				});
				addBuiltin({
					.mTriggers = {{.mEffects = {BuiltinEffectID::StatusRemove}, .mTrigger = BattleEventID::TurnEnd}},
					.mName = PocketCore::Item::ITEM_NAME_CHESTO_BERRY,
					.mItemID = toItemID(BuiltinItemID::ChestoBerry),
					.mTargetID = BattleTargetID::Self,
					.mIsConsumable = true,
				});
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

			/*! @brief Looks up item metadata by stable ID.
				@param[in] itemID The stable item identifier.
				@return A non-owning pointer to metadata if registered, or nullptr otherwise. The pointer remains valid until replacement or
			   registry destruction.
			*/
			ATTR_NODISCARD constexpr const ItemMeta *getItemMetadata(const ItemID itemID) const
			{
				return getMetadata(itemID);
			}

			/*! @brief Looks up an item ID by display name.
				@param[in] name The case-sensitive display name.
				@return The stable ID if registered, or std::nullopt otherwise.
			*/
			ATTR_NODISCARD constexpr const std::optional<ItemID> getItemID(const std::string_view &name) const
			{
				return getID(name);
			}

			/*! @brief Looks up an item display name by stable ID.
				@param[in] itemID The stable item identifier.
				@return The display name if registered, or std::nullopt otherwise.
			*/
			ATTR_NODISCARD constexpr const std::optional<std::string_view> getItemName(const ItemID itemID) const
			{
				return getName(itemID);
			}

			/*! @brief Returns all currently registered item definitions.
				@return A read-only span that remains valid until the registry is mutated or destroyed.
			*/
			ATTR_NODISCARD constexpr const std::span<const ItemMeta> getRegisteredItems() const noexcept
			{
				return getRegisteredEntries();
			}

			/*! @brief Returns the next stable ID assigned to a custom item.
				@return The underlying numeric value of the next item ID.
			*/
			ATTR_NODISCARD constexpr us getNextItemID() const noexcept
			{
				return getNextID();
			}

			/*! @brief Finds an internal array index by stable item ID.
				@param[in] itemID The stable item identifier.
				@return The internal index if registered, or std::nullopt otherwise.
			*/
			ATTR_NODISCARD constexpr const std::optional<us> findIndexByItemID(const ItemID itemID) const
			{
				return findIndexByID(itemID);
			}

			/*! @brief Checks whether an item name is registered.
				@param[in] name The case-sensitive display name.
				@return True if the name is registered, otherwise false.
			*/
			ATTR_NODISCARD constexpr bool hasItem(const std::string_view &name) const
			{
				return hasEntry(name);
			}

			/*! @brief Checks whether an item ID is registered.
				@param[in] itemID The stable item identifier.
				@return True if the ID is registered, otherwise false.
			*/
			ATTR_NODISCARD constexpr bool hasItem(const ItemID itemID) const
			{
				return hasEntry(itemID);
			}

			/*! @brief Sets the next custom item ID counter.
				@param[in] nextID The next underlying ID value.
			*/
			constexpr void setNextItemID(const us nextID) noexcept
			{
				setNextID(nextID);
			}

			/*! @brief Increments the next custom item ID counter. */
			constexpr void incrementNextItemID() noexcept
			{
				incrementNextID();
			}
	};
} // namespace PocketCore::Registry::Item

#endif
