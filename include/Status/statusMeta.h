/*! @file statusMeta.h
	@brief Defines the metadata stored for built-in and user-defined statuses.
	@date 08/04/2026
	@since 0.7.0
		@version 0.9.11
	@author Matthew Moore
*/

#ifndef INCLUDE_STATUS_STATUSMETA_H
#define INCLUDE_STATUS_STATUSMETA_H

#include <string_view>
#include <vector>

#include "Core/typedefs.h"

#include "statusID.h"

namespace PocketCore::Status
{
	/*! @enum StatusInteractionAction
		@showenumvalues
		@brief Identifies how an incoming status interacts with an existing status.
		@details Each action is evaluated from the incoming status toward the existing status identified by @ref
	   StatusInteraction::mExistingStatusID.
		@note Callers handling this enum should account for every value.
		@date 0/04/2026
		@since 0.9.11
		@version 0.9.11
		@author Matthew Moore
	*/
	enum class StatusInteractionAction : PocketCore::Core::ub
	{
		/*! @brief Allows the incoming and existing statuses to remain active together. */
		Coexist,

		/*! @brief Replaces the existing status with the incoming status. */
		ReplaceCurrent,

		/*! @brief Removes the existing status before the incoming status is applied. */
		RemoveCurrent,

		/*! @brief Prevents the incoming status from being applied while the existing status is active. */
		BlockIncoming,
	};

	/*! @struct StatusInteraction Status/statusMeta.h
		@brief Associates an existing status with the action taken when another status is applied.
		@details Instances are stored by the incoming status and describe its behavior when @ref mExistingStatusID is already active.
		@date 0/04/2026
		@since 0.9.11
		@version 0.9.11
		@author Matthew Moore
	*/
	struct StatusInteraction
	{
		public:
			/*! @brief The stable identifier of the status that is already active. */
			StatusID mExistingStatusID{};

			/*! @brief The action taken between the incoming status and the existing status. */
			StatusInteractionAction mAction{StatusInteractionAction::Coexist};
	};

	/*! @struct StatusMeta Status/statusMeta.h
		@brief Stores one status's stable ID, display name, and owned trigger definitions.
		@details The trigger vector owns its elements and their effect vectors. The display name is a non-owning view whose backing storage
	   must remain valid while this metadata is registered.
		@date 08/04/2026
		@since 0.7.0
		@version 0.9.11
		@author Matthew Moore
	*/
	struct StatusMeta
	{
		public:
			/*! @brief The statuses this one interacts with and the corresponding actions. */
			std::vector<StatusInteraction> mStatusInteractions{};

			/*! @brief The case-sensitive display name stored as a non-owning view. */
			std::string_view mName{};

			/*! @brief The stable built-in or user-assigned identifier. */
			StatusID mStatusID{};
	};
} // namespace PocketCore::Status

#endif
