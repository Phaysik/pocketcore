/*! @file typeID.h
	@brief Defines the open identifier used for built-in and user-defined Pokemon types.
	@date 07/27/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_TYPES_TYPE_ID_H
#define INCLUDE_TYPES_TYPE_ID_H

#include <limits>

#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "ID/idInterface.h"
#include "Types/types.h"

namespace PocketCore::Types
{
	using PocketCore::Core::ub;

	namespace Detail
	{
		/*! @brief Distinguishes type identifiers from all other stable identifier domains. */
		struct TypeIDTag;
	} // namespace Detail

	/*! @typedef TypeID
		@brief A strongly typed stable identifier for any registered Pokemon type.
		@details Values are assigned by the type registry. The type is open and represents both built-in @ref Types values and user-defined
	   types without extending the built-in enum. Its tag prevents comparison or conversion with identifiers from other registry domains.
	*/
	using TypeID = PocketCore::ID::IDInterface<Detail::TypeIDTag, std::numeric_limits<ub>::max()>;

	/*! @brief The identifier used for an unassigned Pokemon type slot. */
	inline constexpr TypeID NO_TYPE_ID{};

	/*! @brief Converts a built-in type catalog value to its stable registry identifier.
		@param[in] builtinType The built-in type to convert.
		@return The corresponding open type identifier.
	*/
	ATTR_NODISCARD constexpr TypeID toTypeID(const Types builtinType) noexcept
	{
		return TypeID{static_cast<ub>(builtinType)};
	}
} // namespace PocketCore::Types

#endif
