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
#include <optional>
#include <span>
#include <string_view>

#include "Battle/battleTargetsAndTriggers.h"
#include "Configuration/constants.h"
#include "Configuration/fixedMetadataRegistryConfiguration.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Item/itemID.h"
#include "Item/itemMeta.h"
#include "Registry/itemRegistry.h"

namespace PocketCore::Configuration
{
	using PocketCore::Battle::BattleTargetID;
	using PocketCore::Core::us;
	using PocketCore::Item::ItemEffectTrigger;
	using PocketCore::Item::ItemID;
	using PocketCore::Item::ItemMeta;
	using PocketCore::Registry::Item::ItemRegistry;

	namespace Detail
	{
		struct ItemRegistryConfigurationPolicy
		{
			public:
				static constexpr std::string_view configurationName{"ItemRegistryConfiguration"};
				static constexpr std::string_view entityName{"item"};
				static constexpr RegistryError duplicateError{RegistryError::DuplicateItem};
				static constexpr RegistryError notFoundError{RegistryError::ItemNotFound};
		};
	} // namespace Detail

	/*! @class ItemRegistryConfiguration Configuration/itemRegistryConfiguration.h
		@brief Provides validated user customization over an internal item registry.
		@details Supports lookup, addition, batch addition, trigger replacement, renaming, and removal. Custom IDs are assigned
	   monotonically and are not reused after removal. Batch additions provide all-or-nothing semantics.
		@date 07/27/2026
		@version x.x.x
		@since x.x.x
		@author Matthew Moore
	*/
	class ItemRegistryConfiguration
		: private FixedMetadataRegistryConfiguration<ItemRegistry, ItemMeta, ItemID, MAX_ITEMS, &ItemMeta::mItemID,
													 Detail::ItemRegistryConfigurationPolicy>
	{
		private:
			using Base = FixedMetadataRegistryConfiguration<ItemRegistry, ItemMeta, ItemID, MAX_ITEMS, &ItemMeta::mItemID,
															Detail::ItemRegistryConfigurationPolicy>;

		public:
			/*! @brief Constructs a configuration containing all built-in items. */
			constexpr ItemRegistryConfiguration() = default;

			/*! @brief Returns read-only access to the configured runtime item registry.
				@return A reference that remains valid for the lifetime of this configuration.
			*/
			ATTR_NODISCARD constexpr const ItemRegistry &getRuntimeRegistry() const noexcept
			{
				return Base::getRegistry();
			}

			/*! @brief Looks up complete metadata by stable item ID.
				@param[in] itemID The built-in or custom stable identifier.
				@return A non-owning pointer to metadata if registered, or nullptr otherwise. The pointer remains valid until replacement or
			   configuration destruction.
			*/
			ATTR_NODISCARD constexpr const ItemMeta *getItemMetadata(const ItemID itemID) const
			{
				return getMetadata(itemID);
			}

			/*! @brief Looks up a stable item ID by display name.
				@param[in] name The case-sensitive display name.
				@return The stable ID if registered, or std::nullopt otherwise.
			*/
			ATTR_NODISCARD constexpr const std::optional<ItemID> getItemID(const std::string_view &name) const
			{
				return getID(name);
			}

			/*! @brief Looks up a display name by stable item ID.
				@param[in] itemID The built-in or custom stable identifier.
				@return The display name if registered, or std::nullopt otherwise.
			*/
			ATTR_NODISCARD constexpr const std::optional<std::string_view> getItemName(const ItemID itemID) const
			{
				return getName(itemID);
			}

			/*! @brief Returns all currently registered item definitions.
				@return A read-only span that remains valid until mutation or destruction.
			*/
			ATTR_NODISCARD constexpr const std::span<const ItemMeta> getRegisteredItems() const noexcept
			{
				return getRegisteredEntries();
			}

			/*! @brief Returns the number of registered built-in and custom items.
				@return The current registry entry count.
			*/
			ATTR_NODISCARD constexpr us getAmountRegistered() const noexcept
			{
				return Base::getAmountRegistered();
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
				@param[in] itemID The built-in or custom stable identifier.
				@return True if the ID is registered, otherwise false.
			*/
			ATTR_NODISCARD constexpr bool hasItem(const ItemID itemID) const
			{
				return hasEntry(itemID);
			}

			/*! @brief Registers one user-defined item and assigns a stable ID.
				@param[in] itemMeta The name and trigger metadata to copy into the registry.
				@return The assigned ID on success, or @ref RegistryErrorInfo on duplicate name or exhausted capacity.
			*/
			ATTR_NODISCARD std::expected<ItemID, RegistryErrorInfo> addItem(const ItemMeta &itemMeta);

			/*! @brief Registers multiple items atomically.
				@details Restores the complete prior registry state if any definition fails validation.
				@param[in] itemMetas The item definitions to register in order.
				@return Void on success, or the first @ref RegistryErrorInfo on failure.
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> addItems(const std::span<const ItemMeta> &itemMetas);

			/*! @brief Replaces all trigger metadata for an item selected by name.
				@param[in] itemName The registered display name.
				@param[in] triggers The trigger definitions to copy into the registry.
				@return Void on success, or @ref RegistryErrorInfo if the item is not registered.
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> setItemTriggers(const std::string_view &itemName,
																				  const std::span<const ItemEffectTrigger> &triggers);

			/*! @overload std::expected<void, RegistryErrorInfo> setItemTriggers(ItemID, std::span<const ItemEffectTrigger>)
				@brief Replaces all trigger metadata for an item selected by stable ID.
				@param[in] itemID The built-in or custom stable identifier.
				@param[in] triggers The trigger definitions to copy into the registry.
				@return Void on success, or @ref RegistryErrorInfo if the item is not registered.
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> setItemTriggers(const ItemID itemID,
																				  const std::span<const ItemEffectTrigger> &triggers);

			/*! @brief Replaces all trigger metadata for an item selected by name.
				@param[in] itemName The registered display name.
				@param[in] target The target to copy into the registry.
				@return Void on success, or @ref RegistryErrorInfo if the item is not registered.
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> setItemTarget(const std::string_view &itemName,
																				const BattleTargetID target);

			/*! @overload std::expected<void, RegistryErrorInfo> setItemTarget(ItemID, const BattleTargetID target)
				@brief Replaces all trigger metadata for an item selected by stable ID.
				@param[in] itemID The built-in or custom stable identifier.
				@param[in] target The target to copy into the registry.
				@return Void on success, or @ref RegistryErrorInfo if the item is not registered.
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> setItemTarget(const ItemID itemID, const BattleTargetID target);

			/*! @brief Renames an item without changing its other metadata.
				@details @p newName is stored as a non-owning view and its backing storage must remain valid while registered.
				@param[in] oldName The currently registered display name.
				@param[in] newName The unique replacement display name.
				@return Void on success, or @ref RegistryErrorInfo if the source is absent or target name is already registered.
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> renameItem(const std::string_view &oldName,
																			 const std::string_view &newName);

			/*! @brief Replaces all item metadata for an item selected by stable ID.
				@param[in] itemName The registered display name.
				@param[in] itemMeta The metadata to copy into the registry.
				@return Void on success, or @ref RegistryErrorInfo if the item is not registered.
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> updateItem(const std::string_view &itemName, const ItemMeta &itemMeta);

			/*! @overload std::expected<void, RegistryErrorInfo> updateItem(ItemID, const ItemMeta &itemMeta)
				@brief Replaces all item metadata for an item selected by stable ID.
				@param[in] itemID The built-in or custom stable identifier.
				@param[in] itemMeta The metadata to copy into the registry.
				@return Void on success, or @ref RegistryErrorInfo if the item is not registered.
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> updateItem(const ItemID itemID, const ItemMeta &itemMeta);

			/*! @brief Removes an item by display name.
				@param[in] itemName The registered display name.
				@return The removed stable ID on success, or @ref RegistryErrorInfo if no matching item exists.
			*/
			ATTR_NODISCARD std::expected<ItemID, RegistryErrorInfo> removeItem(const std::string_view &itemName);

			/*! @overload std::expected<ItemID, RegistryErrorInfo> removeItem(ItemID)
				@brief Removes an item by stable ID.
				@param[in] itemID The built-in or custom stable identifier.
				@return The removed stable ID on success, or @ref RegistryErrorInfo if no matching item exists.
			*/
			ATTR_NODISCARD std::expected<ItemID, RegistryErrorInfo> removeItem(const ItemID itemID);
	};
} // namespace PocketCore::Configuration

#endif
