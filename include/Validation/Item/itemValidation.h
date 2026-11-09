/*! @file itemValidation.h
	@brief Defines validation functions that help with Item.
	@date 11/09/2026
	@since 0.12.51
	@version 0.12.54
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
#include "Registry/itemRegistry.h"

#include "itemError.h"

namespace PocketCore::Validation::Item
{
	using PocketCore::Configuration::MAX_ITEMS_PER_POKEMON;
	using PocketCore::Core::us;
	using PocketCore::Item::ItemID;
	using PocketCore::Registry::Item::ItemRegistry;

	/*! @brief Validates the metadata for the given item IDs.
		@param[in] itemIDs The array of item IDs to validate.
		@param[in] itemRegistry The registry to validate item metadata.
		@return A @ref ItemError if any item IDs are invalid, or std::nullopt if all are valid.
		@since 0.12.26
		@version 0.12.54
	 */
	ATTR_NODISCARD constexpr std::expected<void, ItemError> validateItemMetadata(const std::span<const ItemID> itemIDs,
																				 const ItemRegistry &itemRegistry)
	{
		if (itemIDs.size() > MAX_ITEMS_PER_POKEMON)
		{
			return std::unexpected{ItemError::TOO_MANY_ITEMS};
		}

		if (std::ranges::any_of(itemIDs, [&itemRegistry](const ItemID itemID) { return itemRegistry.getItemMetadata(itemID) == nullptr; }))
		{
			return std::unexpected{ItemError::MISSING_ITEM_METADATA};
		}

		return {};
	}

} // namespace PocketCore::Validation::Item

#endif
