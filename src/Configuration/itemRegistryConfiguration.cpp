/*! @file itemRegistryConfiguration.cpp
	@brief Defines validated user customization operations for the item registry.
	@date 07/27/2026
	@since 0.5.0
	@version 0.5.0
	@author Matthew Moore
*/

#include "Configuration/itemRegistryConfiguration.h"

#include <expected>
#include <span>
#include <string_view>
#include <vector>

#include "Battle/battleTargetsAndTriggers.h"
#include "Core/attributeMacros.h"
#include "Item/itemID.h"
#include "Item/itemMeta.h"

namespace PocketCore::Configuration
{
	using PocketCore::Battle::BattleTargetID;
	using PocketCore::Item::ItemID;
	using PocketCore::Item::ItemMeta;

	ATTR_NODISCARD std::expected<ItemID, RegistryErrorInfo> ItemRegistryConfiguration::addItem(const ItemMeta &itemMeta)
	{
		return addMetadata(itemMeta);
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> ItemRegistryConfiguration::addItems(const std::span<const ItemMeta> &itemMetas)
	{
		return addMetadataBatch(itemMetas, [](const ItemMeta &definition) { return ItemMeta{definition}; });
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> ItemRegistryConfiguration::setItemTriggers(
		const std::string_view &itemName, const std::span<const ItemEffectTrigger> &triggers)
	{
		return mutateMetadata(itemName, "setItemTriggers",
							  [&triggers](ItemMeta &metadata) { metadata.mTriggers.assign(triggers.begin(), triggers.end()); });
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> ItemRegistryConfiguration::setItemTriggers(
		const ItemID itemID, const std::span<const ItemEffectTrigger> &triggers)
	{
		return mutateMetadata(itemID, "setItemTriggers",
							  [&triggers](ItemMeta &metadata) { metadata.mTriggers.assign(triggers.begin(), triggers.end()); });
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> ItemRegistryConfiguration::setItemTarget(const std::string_view &itemName,
																								   const BattleTargetID target)
	{
		return mutateMetadata(itemName, "setItemTarget", [&target](ItemMeta &metadata) { metadata.mTargetID = target; });
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> ItemRegistryConfiguration::setItemTarget(const ItemID itemID,
																								   const BattleTargetID target)
	{
		return mutateMetadata(itemID, "setItemTarget", [&target](ItemMeta &metadata) { metadata.mTargetID = target; });
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> ItemRegistryConfiguration::renameItem(const std::string_view &oldName,
																								const std::string_view &newName)
	{
		return renameMetadata(oldName, newName);
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> ItemRegistryConfiguration::updateItem(const std::string_view &itemName,
																								const ItemMeta &itemMeta)
	{
		return mutateMetadata(itemName, "updateItem", [&itemMeta](ItemMeta &metadata) { metadata = itemMeta; });
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> ItemRegistryConfiguration::updateItem(const ItemID itemID,
																								const ItemMeta &itemMeta)
	{
		return mutateMetadata(itemID, "updateItem", [&itemMeta](ItemMeta &metadata) { metadata = itemMeta; });
	}

	ATTR_NODISCARD std::expected<ItemID, RegistryErrorInfo> ItemRegistryConfiguration::removeItem(const std::string_view &itemName)
	{
		return removeMetadata(itemName);
	}

	ATTR_NODISCARD std::expected<ItemID, RegistryErrorInfo> ItemRegistryConfiguration::removeItem(const ItemID itemID)
	{
		return removeMetadata(itemID);
	}
} // namespace PocketCore::Configuration
