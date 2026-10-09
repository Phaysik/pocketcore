/*! @file itemID.h
	@brief Defines the open identifier used for built-in and user-defined items.
	@date 10/09/2026
	@since 0.3.0
	@version 0.12.51
	@author Matthew Moore
*/

#ifndef INCLUDE_ITEM_ITEM_ID_H
#define INCLUDE_ITEM_ITEM_ID_H

#include "ID/idInterface.h"

namespace PocketCore::Item
{
	namespace Detail
	{
		/*! @brief Distinguishes item identifiers from all other stable identifier domains. */
		struct ItemIDTag;
	} // namespace Detail

	/*! @typedef ItemID
		@brief A strongly typed stable identifier for any registered item.
		@details Values are assigned by the item registry. Unlike @ref BuiltinItemID, this type is open and can represent user-defined
	   abilities without extending an enum. Its tag prevents comparison or conversion with identifiers from other registry domains.
	*/
	using ItemID = PocketCore::ID::IDInterface<Detail::ItemIDTag, 0>;

	/*! @brief The stable identifier representing no item. */
	inline constexpr ItemID NO_ITEM_ID{};
} // namespace PocketCore::Item

#endif
