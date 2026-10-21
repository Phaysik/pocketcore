/*! @file statusID.h
	@brief Defines the open identifier used for built-in and user-defined statuses.
	@brief Contains the status effects
	@date 10/09/2026
	@since 0.3.0
	@version 0.12.51
	@author Matthew Moore
*/

#ifndef INCLUDE_STATUS_STATUS_ID_H
#define INCLUDE_STATUS_STATUS_ID_H

#include "ID/idInterface.h"

namespace PocketCore::Status
{
	namespace Detail
	{
		/*! @brief Distinguishes status identifiers from all other stable identifier domains. */
		struct StatusIDTag;
	} // namespace Detail

	/*! @typedef StatusID
		@brief A strongly typed stable identifier for any registered status.
		@details Values are assigned by the status registry. Unlike @ref BuiltinStatusID, this type is open and can represent user-defined
	   statuses without extending an enum. Its tag prevents comparison or conversion with identifiers from other registry domains.
	*/
	using StatusID = PocketCore::ID::IDInterface<Detail::StatusIDTag, 0>;

	/*! @brief The stable identifier representing no status. */
	inline constexpr StatusID NO_STATUS_ID{};
} // namespace PocketCore::Status

#endif
