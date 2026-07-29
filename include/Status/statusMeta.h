/*! @file statusMeta.h
	@brief Defines the metadata stored for built-in and user-defined statuses.
	@date 07/24/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_STATUS_STATUSMETA_H
#define INCLUDE_STATUS_STATUSMETA_H

#include <string_view>

#include "statusID.h"

namespace PocketCore::Status
{
	/*! @struct StatusMeta Status/statusMeta.h
		@brief Stores one status's stable ID, display name, and owned trigger definitions.
		@details The trigger vector owns its elements and their effect vectors. The display name is a non-owning view whose backing storage
	   must remain valid while this metadata is registered.
		@date 07/27/2026
		@version x.x.x
		@since x.x.x
		@author Matthew Moore
	*/
	struct StatusMeta
	{
		public:
			/*! @brief The case-sensitive display name stored as a non-owning view. */
			std::string_view mName{};

			/*! @brief The stable built-in or user-assigned identifier. */
			StatusID mStatusID{};
	};
} // namespace PocketCore::Status

#endif
