/*! @file itemValidation.h
	@brief Defines validation functions that help with Item.
	@date 10/09/2026
	@since 0.12.51
	@version 0.12.51
	@author Matthew Moore
*/

#ifndef INCLUDE_VALIDATION_ITEM_ITEM_VALIDATION_H
#define INCLUDE_VALIDATION_ITEM_ITEM_VALIDATION_H

#include <algorithm>
#include <expected>
#include <span>

#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Item/itemID.h"
#include "Item/itemMeta.h"
#include "Registry/itemRegistry.h"

#include "itemError.h"

namespace PocketCore::Validation::Item
{
	using PocketCore::Configuration::MAX_ITEMS_PER_POKEMON;
	using PocketCore::Core::us;
	using PocketCore::Item::ItemID;
	using PocketCore::Item::ItemMeta;
	using PocketCore::Registry::Item::ItemRegistry;

	/*! @brief Validates the metadata for the given item IDs.
		@param[in] itemIDs The array of item IDs to validate.
		@param[in] itemRegistry The registry to validate item metadata.
		@return A @ref ItemError if any item IDs are invalid, or std::nullopt if all are valid.
		@since 0.12.26
		@version 0.12.51
	 */
	ATTR_NODISCARD constexpr std::expected<void, ItemError> validateItemMetadata(
		const std::span<const ItemID, MAX_ITEMS_PER_POKEMON> *itemIDs, const ItemRegistry &itemRegistry)
	{
		std::array<const ItemMeta *, MAX_ITEMS_PER_POKEMON> itemMetas{};
		std::ranges::transform(*itemIDs, itemMetas.begin(),
							   [&itemRegistry](const ItemID itemID) { return itemRegistry.getItemMetadata(itemID); });

		if (std::ranges::any_of(itemMetas, [](const ItemMeta *meta) { return meta == nullptr; }))
		{
			return std::unexpected{ItemError::MISSING_ITEM_METADATA};
		}

		return {};
	}

} // namespace PocketCore::Validation::Item

#endif
