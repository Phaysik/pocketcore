/*! @file builtinLocationID.h
	@brief Defines identifiers for locations compiled into PocketCore.
	@date 10/09/2026
	@since 0.12.51
	@version 0.12.51
	@author Matthew Moore
*/

#ifndef INCLUDE_LOCATION_BUILTIN_LOCATION_ID_H
#define INCLUDE_LOCATION_BUILTIN_LOCATION_ID_H

#include <utility>

#include "Core/attributeMacros.h"
#include "Core/typedefs.h"

#include "locationID.h"

namespace PocketCore::Location
{
	using PocketCore::Core::ub;

	/*! @enum BuiltinLocationID
		@showenumvalues
		@brief Names the locations provided by PocketCore itself.
		@details This closed enum is only a catalog of built-in locations. Runtime state and user-facing APIs use the open @ref LocationID
	   type.
		@note All enum values must be handled exhaustively when registering built-in metadata.
		@date 10/09/2026
		@since 0.12.51
		@version 0.12.51
		@author Matthew Moore
	*/
	enum class BuiltinLocationID : ub
	{
		None,
		Route1,
		FinalLocation,
	};

	/*! @brief Converts a built-in catalog value to its stable registry identifier.
		@param[in] builtinLocationID The built-in location to convert.
		@return The corresponding open location identifier.
		@since 0.12.51
		@version 0.12.51
	*/
	ATTR_NODISCARD constexpr LocationID toLocationID(const BuiltinLocationID builtinLocationID) noexcept
	{
		return LocationID{std::to_underlying(builtinLocationID)};
	}
} // namespace PocketCore::Location

#endif
