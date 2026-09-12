/*! @file itemHelpers.h
	@brief Houses free functions that aide in handling items.
	@date 09/11/2026
	@since 0.12.26
	@version 0.12.27
	@author Matthew Moore
*/

#ifndef INCLUDE_ITEM_ITEM_HELPERS_H
#define INCLUDE_ITEM_ITEM_HELPERS_H

#include <utility>

#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Registry/itemRegistry.h"
#include "Utility/random.h"

#include "builtInItemID.h"
#include "itemID.h"

namespace PocketCore::Item
{
	using PocketCore::Core::us;
	using PocketCore::Registry::Item::ItemRegistry;
	using PocketCore::Utility::Random;

	/*! @brief Gets a random item ID from the built-in items.
		@return The random item ID.
		@since 0.12.26
		@version 0.12.26
	*/
	ATTR_NODISCARD constexpr ItemID getRandomItemIDFromBuiltIns()
	{
		return ItemID(Random::get<us>(std::to_underlying(BuiltinItemID::None) + 1, std::to_underlying(BuiltinItemID::FinalItem) - 1));
	}

	/*! @brief Gets a random item ID from the given configuration.
		@param[in] config The configuration to get the random item ID from.
		@return The random item ID.
		@since 0.12.26
		@version 0.12.27
	*/
	ATTR_NODISCARD constexpr ItemID getRandomItemIDFromConfiguration(const ItemRegistry &config)
	{
		return ItemID(Random::get<us>(std::to_underlying(BuiltinItemID::None) + 1, config.getAmountRegistered()));
	}
} // namespace PocketCore::Item

#endif
