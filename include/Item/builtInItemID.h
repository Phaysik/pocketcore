/*! @file builtinItemID.h
	@brief Defines identifiers for items compiled into PocketCore.
	@date 07/27/2026
	@version x.x.x
	@since x.x.x
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
		@date 07/27/2026
		@version x.x.x
		@since x.x.x
		@author Matthew Moore
	*/
	enum class BuiltinItemID : PocketCore::Core::ub
	{
		None,
		CheriBerry,
		ChestoBerry,

		AirBalloon,
	};

	/*! @brief Converts a built-in catalog value to its stable registry identifier.
		@param[in] builtinItemID The built-in item to convert.
		@return The corresponding open item identifier.
	*/
	ATTR_NODISCARD constexpr ItemID toItemID(const BuiltinItemID builtinItemID) noexcept
	{
		return ItemID{static_cast<PocketCore::Core::ub>(builtinItemID)};
	}
} // namespace PocketCore::Item

#endif
