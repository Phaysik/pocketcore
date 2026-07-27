/*! @file itemRegistryConfiguration.cpp
	@brief Defines item-specific operations over the shared metadata configuration core.
	@date 07/27/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#include "Configuration/itemRegistryConfiguration.h"

#include <expected>
#include <span>
#include <string_view>
#include <utility>
#include <vector>

#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Item/itemID.h"
#include "Item/itemMeta.h"

namespace PocketCore::Configuration
{
	using PocketCore::Item::ItemID;
	using PocketCore::Item::ItemMeta;

	ATTR_NODISCARD std::expected<ItemID, RegistryErrorInfo> ItemRegistryConfiguration::addItem(const ItemDefinition &definition)
	{
		std::vector<ItemEffectTrigger> ownedTriggers{definition.triggers.begin(), definition.triggers.end()};
		return addMetadata(ItemMeta{.mName = definition.name, .mTriggers = std::move(ownedTriggers)});
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> ItemRegistryConfiguration::addItems(
		const std::span<const ItemDefinition> definitions)
	{
		return addMetadataBatch(definitions, [](const ItemDefinition &definition) {
			return ItemMeta{
				.mName = definition.name,
				.mTriggers = std::vector<ItemEffectTrigger>{definition.triggers.begin(), definition.triggers.end()},
			};
		});
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> ItemRegistryConfiguration::setItemTriggers(
		const std::string_view itemName, const std::span<const ItemEffectTrigger> triggers)
	{
		return mutateMetadata(itemName, "setItemTriggers",
							  [triggers](ItemMeta &metadata) { metadata.mTriggers.assign(triggers.begin(), triggers.end()); });
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> ItemRegistryConfiguration::setItemTriggers(
		const ItemID itemID, const std::span<const ItemEffectTrigger> triggers)
	{
		return mutateMetadata(itemID, "setItemTriggers",
							  [triggers](ItemMeta &metadata) { metadata.mTriggers.assign(triggers.begin(), triggers.end()); });
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> ItemRegistryConfiguration::renameItem(const std::string_view oldName,
																								const std::string_view newName)
	{
		return renameMetadata(oldName, newName);
	}

	ATTR_NODISCARD std::expected<ItemID, RegistryErrorInfo> ItemRegistryConfiguration::removeItem(const std::string_view itemName)
	{
		return removeMetadata(itemName);
	}

	ATTR_NODISCARD std::expected<ItemID, RegistryErrorInfo> ItemRegistryConfiguration::removeItem(const ItemID itemID)
	{
		return removeMetadata(itemID);
	}
} // namespace PocketCore::Configuration
