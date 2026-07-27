/*! @file itemRegistry.h
	@brief Provides fixed-capacity storage and lookup for built-in and user-defined items.
	@date 07/27/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_REGISTRY_ITEM_REGISTRY_H
#define INCLUDE_REGISTRY_ITEM_REGISTRY_H

#include <array>
#include <cassert>
#include <functional>
#include <optional>
#include <span>
#include <string_view>
#include <utility>

#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Effect/effectType.h"
#include "Item/builtinItemID.h"
#include "Item/constants.h"
#include "Item/itemID.h"
#include "Item/itemMeta.h"
#include "Item/itemTargetsAndTriggers.h"

namespace PocketCore::Registry::Item
{
	using PocketCore::Configuration::MAX_ITEMS;
	using PocketCore::Core::us;
	using PocketCore::Effect::EffectTypeID;
	using PocketCore::Item::BuiltinItemID;
	using PocketCore::Item::ItemEffectTrigger;
	using PocketCore::Item::ItemID;
	using PocketCore::Item::ItemMeta;
	using PocketCore::Item::ItemTriggerID;
	using PocketCore::Item::toItemID;

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
	class ItemRegistry
	{
		public:
			/*! @brief Constructs a registry populated with every @ref BuiltinItemID. */
			explicit constexpr ItemRegistry() : mNextItemID{static_cast<us>(toItemID(BuiltinItemID::ChestoBerry).getValue() + 1U)}
			{
				addBuiltin({.mItemID = toItemID(BuiltinItemID::None), .mName = PocketCore::Item::ITEM_NAME_NONE, .mTriggers = {}});
				addBuiltin({
					.mItemID = toItemID(BuiltinItemID::CheriBerry),
					.mName = PocketCore::Item::ITEM_NAME_CHERI_BERRY,
					.mTriggers = {{.mTrigger = ItemTriggerID::OnTurnEnd, .mEffects = {EffectTypeID::StatusRemove}}},
				});
				addBuiltin({
					.mItemID = toItemID(BuiltinItemID::ChestoBerry),
					.mName = PocketCore::Item::ITEM_NAME_CHESTO_BERRY,
					.mTriggers = {{.mTrigger = ItemTriggerID::OnTurnEnd, .mEffects = {EffectTypeID::StatusRemove}}},
				});
			}

			/*! @brief Returns the metadata stored at an internal array index.
				@pre @p index < @ref MAX_ITEMS.
				@param[in] index The internal array index.
				@return A const reference to the stored metadata that remains valid until the registry is mutated or destroyed.
			*/
			ATTR_NODISCARD constexpr const ItemMeta &getEntry(const us index) const
			{
				assert(index < mItems.size());
				return mItems.at(index);
			}

			/*! @brief Looks up item metadata by stable ID.
				@param[in] itemID The stable item identifier.
				@return A reference to the metadata if registered, or std::nullopt otherwise. The reference remains valid until mutation or
			   destruction.
			*/
			ATTR_NODISCARD constexpr std::optional<std::reference_wrapper<const ItemMeta>> getItemMetadata(const ItemID itemID) const
			{
				const us index{findEntryIndexByID(itemID)};

				if (index == mAmountRegistered)
				{
					return std::nullopt;
				}

				return std::cref(mItems.at(index));
			}

			/*! @brief Looks up an item ID by display name.
				@param[in] name The case-sensitive display name.
				@return The stable ID if registered, or std::nullopt otherwise.
			*/
			ATTR_NODISCARD constexpr std::optional<ItemID> getItemID(const std::string_view name) const
			{
				const us index{findEntryIndexByName(name)};

				if (index == mAmountRegistered)
				{
					return std::nullopt;
				}

				return mItems.at(index).mItemID;
			}

			/*! @brief Looks up an item display name by stable ID.
				@param[in] itemID The stable item identifier.
				@return The display name if registered, or std::nullopt otherwise.
			*/
			ATTR_NODISCARD constexpr std::optional<std::string_view> getItemName(const ItemID itemID) const
			{
				const auto metadata{getItemMetadata(itemID)};

				if (!metadata.has_value())
				{
					return std::nullopt;
				}

				return metadata->get().mName;
			}

			/*! @brief Returns all currently registered item definitions.
				@return A read-only span that remains valid until the registry is mutated or destroyed.
			*/
			ATTR_NODISCARD constexpr std::span<const ItemMeta> getRegisteredItems() const noexcept
			{
				return {mItems.data(), mAmountRegistered};
			}

			/*! @brief Returns the number of registered items.
				@return The number of valid entries in the registry.
			*/
			ATTR_NODISCARD constexpr us getAmountRegistered() const noexcept
			{
				return mAmountRegistered;
			}

			/*! @brief Returns the next stable ID assigned to a custom item.
				@return The underlying numeric value of the next item ID.
			*/
			ATTR_NODISCARD constexpr us getNextItemID() const noexcept
			{
				return mNextItemID;
			}

			/*! @brief Finds an internal array index by stable item ID.
				@param[in] itemID The stable item identifier.
				@return The internal index if registered, or std::nullopt otherwise.
			*/
			ATTR_NODISCARD constexpr std::optional<us> findIndexByItemID(const ItemID itemID) const
			{
				const us index{findEntryIndexByID(itemID)};

				if (index == mAmountRegistered)
				{
					return std::nullopt;
				}

				return index;
			}

			/*! @brief Checks whether an item name is registered.
				@param[in] name The case-sensitive display name.
				@return True if the name is registered, otherwise false.
			*/
			ATTR_NODISCARD constexpr bool hasItem(const std::string_view name) const
			{
				return findEntryIndexByName(name) != mAmountRegistered;
			}

			/*! @brief Checks whether an item ID is registered.
				@param[in] itemID The stable item identifier.
				@return True if the ID is registered, otherwise false.
			*/
			ATTR_NODISCARD constexpr bool hasItem(const ItemID itemID) const
			{
				return findEntryIndexByID(itemID) != mAmountRegistered;
			}

			/*! @brief Replaces the metadata at an internal array index.
				@pre @p index < @ref MAX_ITEMS.
				@param[in] index The internal array index.
				@param[in] metadata The complete metadata to store.
			*/
			constexpr void setEntry(const us index, const ItemMeta &metadata)
			{
				assert(index < mItems.size());
				mItems.at(index) = metadata;
			}

			/*! @brief Sets the registered entry count.
				@param[in] amount The new number of valid entries.
			*/
			constexpr void setAmountRegistered(const us amount) noexcept
			{
				mAmountRegistered = amount;
			}

			/*! @brief Sets the next custom item ID counter.
				@param[in] nextID The next underlying ID value.
			*/
			constexpr void setNextItemID(const us nextID) noexcept
			{
				mNextItemID = nextID;
			}

			/*! @brief Increments the registered entry count. */
			constexpr void incrementAmountRegistered() noexcept
			{
				++mAmountRegistered;
			}

			/*! @brief Decrements the registered entry count. */
			constexpr void decrementAmountRegistered() noexcept
			{
				--mAmountRegistered;
			}

			/*! @brief Increments the next custom item ID counter. */
			constexpr void incrementNextItemID() noexcept
			{
				++mNextItemID;
			}

		private:
			ATTR_NODISCARD constexpr us findEntryIndexByName(const std::string_view name) const
			{
				for (us index{0}; index < mAmountRegistered; ++index)
				{
					if (mItems.at(index).mName == name)
					{
						return index;
					}
				}

				return mAmountRegistered;
			}

			ATTR_NODISCARD constexpr us findEntryIndexByID(const ItemID itemID) const
			{
				for (us index{0}; index < mAmountRegistered; ++index)
				{
					if (mItems.at(index).mItemID == itemID)
					{
						return index;
					}
				}

				return mAmountRegistered;
			}

			constexpr void addBuiltin(ItemMeta metadata)
			{
				assert(mAmountRegistered < mItems.size());
				mItems.at(mAmountRegistered) = std::move(metadata);
				++mAmountRegistered;
			}

		private:
			std::array<ItemMeta, MAX_ITEMS> mItems{};
			us mAmountRegistered{0};
			us mNextItemID{0};
	};
} // namespace PocketCore::Registry::Item

#endif
