/*! @file statusID.h
	@brief Contains the status effects
	@date 07/24/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_STATUS_STATUSID_H
#define INCLUDE_STATUS_STATUSID_H

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
