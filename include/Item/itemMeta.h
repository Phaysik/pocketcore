/*! @file itemMeta.h
	@brief Defines the metadata stored for built-in and user-defined items.
	@date 07/24/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_ITEM_ITEMMETA_H
#define INCLUDE_ITEM_ITEMMETA_H

#include <string_view>
#include <vector>

#include "itemID.h"
#include "itemTargetsAndTriggers.h"

namespace PocketCore::Item
{
	/*! @struct ItemMeta Item/itemMeta.h
		@brief Stores one item's stable ID, display name, and owned trigger definitions.
		@details The trigger vector owns its elements and their effect vectors. The display name is a non-owning view whose backing storage
	   must remain valid while this metadata is registered.
		@date 07/27/2026
		@version x.x.x
		@since x.x.x
		@author Matthew Moore
	*/
	struct ItemMeta
	{
		public:
			/*! @brief The stable built-in or user-assigned identifier. */
			ItemID mItemID{};

			/*! @brief The case-sensitive display name stored as a non-owning view. */
			std::string_view mName{};

			/*! @brief The owned trigger and effect definitions for this item. */
			std::vector<ItemEffectTrigger> mTriggers;
	};
} // namespace PocketCore::Item

#endif
