/*! @file locationID.h
	@brief Defines the open identifier used for built-in and user-defined locations.
	@date 10/09/2026
	@since 0.12.51
	@version 0.12.51
	@author Matthew Moore
*/

#ifndef INCLUDE_LOCATION_LOCATION_ID_H
#define INCLUDE_LOCATION_LOCATION_ID_H

#include "ID/idInterface.h"

namespace PocketCore::Location
{
	namespace Detail
	{
		/*! @brief Distinguishes location identifiers from all other stable identifier domains. */
		struct LocationIDTag;
	} // namespace Detail

	/*! @typedef LocationID
		@brief A strongly typed stable identifier for any registered location.
		@details Values are assigned by the location registry. Unlike @ref BuiltinLocationID, this type is open and can represent
	   user-defined abilities without extending an enum. Its tag prevents comparison or conversion with identifiers from other registry
	   domains.
	*/
	using LocationID = PocketCore::ID::IDInterface<Detail::LocationIDTag, 0>;

	/*! @brief The stable identifier representing no location. */
	inline constexpr LocationID NO_LOCATION_ID{};
} // namespace PocketCore::Location

#endif
