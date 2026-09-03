/*! @file statusMeta.h
	@brief Defines the metadata stored for built-in and user-defined statuses.
	@date 09/03/2026
	@since 0.7.0
	@version 0.12.19
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
		@date 09/03/2026
		@since 0.7.0
		@version 0.12.19
		@author Matthew Moore
	*/
	struct StatusMeta
	{
		public:
			/*! @brief Compares two StatusMeta instances for equivalent metadata.
				@details Compares all fields exactly.
				@param[in] other The StatusMeta instance to compare.
				@return True when both instances contain equivalent metadata; otherwise false.
				@since 0.12.19
				@version 0.12.19
			*/
			ATTR_NODISCARD constexpr bool operator==(const StatusMeta &other) const noexcept = default;

			// NOLINTBEGIN(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)

			/*! @brief The statuses this one interacts with and the corresponding actions. */
			std::vector<Interaction<StatusID>> mStatusInteractions{};

			/*! @brief The case-sensitive display name stored as a non-owning view. */
			std::string_view mName{};

			/*! @brief The stable built-in or user-assigned identifier. */
			StatusID mStatusID{};

			// NOLINTEND(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)
	};
} // namespace PocketCore::Status

#endif
