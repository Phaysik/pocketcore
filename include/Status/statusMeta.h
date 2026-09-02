/*! @file statusMeta.h
	@brief Defines the metadata stored for built-in and user-defined statuses.
	@date 09/02/2026
	@since 0.7.0
	@version 0.12.16
	@author Matthew Moore
*/

#ifndef INCLUDE_STATUS_STATUSMETA_H
#define INCLUDE_STATUS_STATUSMETA_H

#include <string_view>
#include <vector>

#include "Interaction/interaction.h"

#include "statusID.h"

namespace PocketCore::Status
{
	using PocketCore::Interaction::Interaction;

	/*! @struct StatusMeta Status/statusMeta.h
		@brief Stores one status's stable ID, display name, and owned trigger definitions.
		@details The trigger vector owns its elements and their effect vectors. The display name is a non-owning view whose backing storage
	   must remain valid while this metadata is registered.
		@date 09/02/2026
		@since 0.7.0
		@version 0.12.16
		@author Matthew Moore
	*/
	struct StatusMeta
	{
		public:
			/*! @brief The statuses this one interacts with and the corresponding actions. */
			std::vector<Interaction<StatusID>> mStatusInteractions{};

			/*! @brief The case-sensitive display name stored as a non-owning view. */
			std::string_view mName{};

			/*! @brief The stable built-in or user-assigned identifier. */
			StatusID mStatusID{};
	};
} // namespace PocketCore::Status

#endif
