/*! @file itemRegistryConfiguration.h
	@brief Declares the user-facing facade for configuring item metadata.
	@date 07/27/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_CONFIGURATION_ITEM_REGISTRY_CONFIGURATION_H
#define INCLUDE_CONFIGURATION_ITEM_REGISTRY_CONFIGURATION_H

#include <expected>
#include <functional>
#include <optional>
#include <span>
#include <string_view>

#include "Configuration/constants.h"
#include "Configuration/fixedMetadataRegistryConfiguration.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Item/itemID.h"
#include "Item/itemMeta.h"
#include "Item/itemTargetsAndTriggers.h"
#include "Registry/itemRegistry.h"

namespace PocketCore::Configuration
{
	using PocketCore::Core::us;
	using PocketCore::Item::ItemEffectTrigger;
	using PocketCore::Item::ItemID;
	using PocketCore::Item::ItemMeta;

	namespace Detail
	{
		struct ItemRegistryConfigurationPolicy
		{
				static constexpr std::string_view configurationName{"ItemRegistryConfiguration"};
				static constexpr std::string_view entityName{"item"};
				static constexpr RegistryError duplicateError{RegistryError::DuplicateItem};
				static constexpr RegistryError notFoundError{RegistryError::ItemNotFound};
		};
	} // namespace Detail

	/*! @struct ItemDefinition Configuration/itemRegistryConfiguration.h
		@brief Describes a user-defined item before the registry assigns its stable ID.
		@details The trigger span is copied into registry-owned metadata during registration. The name is non-owning and must remain valid
	   while the item is registered.
		@date 07/27/2026
		@version x.x.x
		@since x.x.x
		@author Matthew Moore
	*/
	struct ItemDefinition
	{
			/*! @brief The unique, case-sensitive display name with storage that outlives the registered item. */
			std::string_view name{};

			/*! @brief The trigger and effect definitions copied during registration. */
			std::span<const ItemEffectTrigger> triggers{};
	};

	/*! @class ItemRegistryConfiguration Configuration/itemRegistryConfiguration.h
		@brief Provides validated user customization over an internal item registry.
		@details Supports lookup, addition, atomic batch addition, trigger replacement, renaming, and removal. Shared lifecycle behavior is
	   implemented by @ref FixedMetadataRegistryConfiguration.
		@date 07/27/2026
		@version x.x.x
		@since x.x.x
		@author Matthew Moore
	*/
	class ItemRegistryConfiguration
		: private FixedMetadataRegistryConfiguration<Registry::Item::ItemRegistry, ItemMeta, ItemID, MAX_ITEMS, &ItemMeta::mItemID,
													 Detail::ItemRegistryConfigurationPolicy>
	{
		private:
			using Base = FixedMetadataRegistryConfiguration<Registry::Item::ItemRegistry, ItemMeta, ItemID, MAX_ITEMS, &ItemMeta::mItemID,
															Detail::ItemRegistryConfigurationPolicy>;

		public:
			/*! @brief Constructs a configuration containing all built-in items. */
			constexpr ItemRegistryConfiguration() = default;

			/*! @brief Looks up complete item metadata by stable ID. */
			ATTR_NODISCARD constexpr std::optional<std::reference_wrapper<const ItemMeta>> getItemMetadata(const ItemID itemID) const
			{
				return getMetadata(itemID);
			}

			/*! @brief Looks up a stable item ID by display name. */
			ATTR_NODISCARD constexpr std::optional<ItemID> getItemID(const std::string_view name) const
			{
				return getID(name);
			}

			/*! @brief Looks up an item display name by stable ID. */
			ATTR_NODISCARD constexpr std::optional<std::string_view> getItemName(const ItemID itemID) const
			{
				return getName(itemID);
			}

			/*! @brief Returns all currently registered item definitions. */
			ATTR_NODISCARD constexpr std::span<const ItemMeta> getRegisteredItems() const noexcept
			{
				return getRegisteredEntries();
			}

			/*! @brief Returns the number of registered built-in and custom items. */
			ATTR_NODISCARD constexpr us getAmountRegistered() const noexcept
			{
				return Base::getAmountRegistered();
			}

			/*! @brief Checks whether an item name is registered. */
			ATTR_NODISCARD constexpr bool hasItem(const std::string_view name) const
			{
				return hasEntry(name);
			}

			/*! @brief Checks whether an item ID is registered. */
			ATTR_NODISCARD constexpr bool hasItem(const ItemID itemID) const
			{
				return hasEntry(itemID);
			}

			/*! @brief Registers one user-defined item and assigns a stable ID.
				@param[in] definition The name and trigger metadata to copy into the registry.
				@return The assigned ID on success, or contextual registry error information.
			*/
			ATTR_NODISCARD std::expected<ItemID, RegistryErrorInfo> addItem(const ItemDefinition &definition);

			/*! @brief Registers multiple items atomically.
				@param[in] definitions The item definitions to register in order.
				@return Void on success, or the first registry error after rollback.
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> addItems(std::span<const ItemDefinition> definitions);

			/*! @brief Replaces all trigger metadata for an item selected by name. */
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> setItemTriggers(std::string_view itemName,
																				  std::span<const ItemEffectTrigger> triggers);

			/*! @brief Replaces all trigger metadata for an item selected by stable ID. */
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> setItemTriggers(ItemID itemID,
																				  std::span<const ItemEffectTrigger> triggers);

			/*! @brief Renames an item without changing its stable ID or trigger metadata. */
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> renameItem(std::string_view oldName, std::string_view newName);

			/*! @brief Removes an item by display name. */
			ATTR_NODISCARD std::expected<ItemID, RegistryErrorInfo> removeItem(std::string_view itemName);

			/*! @brief Removes an item by stable ID. */
			ATTR_NODISCARD std::expected<ItemID, RegistryErrorInfo> removeItem(ItemID itemID);
	};
} // namespace PocketCore::Configuration

#endif
