/*! @file itemID.h
	@brief Contains the item status
	@date 07/24/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_ITEM_ITEMID_H
#define INCLUDE_ITEM_ITEMID_H

#include "Core/typedefs.h"

namespace PocketCore::Item
{
	enum class ItemID : PocketCore::Core::ub
	{
		None,
		CheriBerry,
		ChestoBerry,
	};
} // namespace PocketCore::Item

#endif
