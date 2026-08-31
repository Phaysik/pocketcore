/*! @file builtinItemID.h
	@brief Defines identifiers for items compiled into PocketCore.
	@date 08/22/2026
	@since 0.4.1
	@version 0.11.6
	@author Matthew Moore
*/

#ifndef INCLUDE_ITEM_BUILTIN_ITEM_ID_H
#define INCLUDE_ITEM_BUILTIN_ITEM_ID_H

#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Item/itemID.h"

namespace PocketCore::Item
{
	/*! @enum BuiltinItemID
		@showenumvalues
		@brief Names the items provided by PocketCore itself.
		@details This closed enum is only a catalog of built-in items. Runtime state and user-facing APIs use the open @ref ItemID
	   type.
		@note All enum values must be handled exhaustively when registering built-in metadata.
		@date 08/22/2026
		@since 0.4.1
		@version 0.11.6
		@author Matthew Moore
	*/
	enum class BuiltinItemID : PocketCore::Core::ub
	{
		None,
		CheriBerry,
		ChestoBerry,
		FinalItem,
	};

	/*! @brief Converts a built-in catalog value to its stable registry identifier.
		@param[in] builtinItemID The built-in item to convert.
		@return The corresponding open item identifier.
		@since 0.4.1
		@version 0.4.1
	*/
	ATTR_NODISCARD constexpr ItemID toItemID(const BuiltinItemID builtinItemID) noexcept
	{
		return ItemID{static_cast<PocketCore::Core::ub>(builtinItemID)};
	}
} // namespace PocketCore::Item

#endif
