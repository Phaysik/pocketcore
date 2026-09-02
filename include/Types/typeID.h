/*! @file typeID.h
	@brief Defines the open identifier used for built-in and user-defined Pokemon types.
	@date 09/02/2026
	@since 0.4.0
	@version 0.12.17
	@author Matthew Moore
*/

#ifndef INCLUDE_TYPES_TYPEID_H
#define INCLUDE_TYPES_TYPEID_H

#include "Core/typedefs.h"
#include "ID/idInterface.h"

namespace PocketCore::Type
{
	using PocketCore::Core::ub;

	namespace Detail
	{
		/*! @brief Distinguishes type identifiers from all other stable identifier domains. */
		struct typeID;
	} // namespace Detail

	/*! @typedef TypeID
		@brief A strongly typed stable identifier for any registered Pokemon type.
		@details Values are assigned by the type registry. The type is open and represents both built-in @ref Types values and user-defined
	   types without extending the built-in enum. Its tag prevents comparison or conversion with identifiers from other registry domains.
	*/
	using TypeID = PocketCore::ID::IDInterface<Detail::typeID, 0>;

	/*! @brief The identifier used for an unassigned Pokemon type slot. */
	inline constexpr TypeID NO_TYPE_ID{};
} // namespace PocketCore::Type

#endif
