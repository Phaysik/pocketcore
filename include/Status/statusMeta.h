/*! @file statusMeta.h
	@brief Defines the metadata stored for built-in and user-defined statuses.
	@date 09/23/2026
	@since 0.7.0
	@version 0.12.44
	@author Matthew Moore
*/

#ifndef INCLUDE_STATUS_STATUSMETA_H
#define INCLUDE_STATUS_STATUSMETA_H

#include <string>
#include <vector>

#include "Core/typedefs.h"
#include "Interaction/interaction.h"

#include "statusID.h"

namespace PocketCore::Status
{
	using PocketCore::Core::ub;
	using PocketCore::Interaction::Interaction;

	/*! @enum StatusClassification
		@showenumvalues
		@brief Distinguishes statuses that persist across turns from those that clear when a Pokemon leaves battle.
		@details A NonVolatile status (such as a burn or paralysis) remains attached to a Pokemon until explicitly cured, whereas a Volatile
	   status (such as confusion) is transient and typically ends when the Pokemon switches out or the effect resolves.
		@date 09/23/2026
		@since 0.12.44
		@version 0.12.44
		@author Matthew Moore
	*/
	enum class StatusClassification : ub
	{
		/*! @brief A persistent status that stays applied until cured. */
		NonVolatile,
		/*! @brief A transient status that clears when the Pokemon switches out or the effect ends. */
		Volatile,
	};

	/*! @struct StatusMeta Status/statusMeta.h
		@brief Stores one status's stable ID, display name, and owned trigger definitions.
		@details The trigger vector owns its elements and their effect vectors. The display name is a non-owning view whose backing storage
	   must remain valid while this metadata is registered.
		@date 09/23/2026
		@since 0.7.0
		@version 0.12.44
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
				@version 0.12.44
			*/
			ATTR_NODISCARD constexpr bool operator==(const StatusMeta &other) const = default;

			// NOLINTBEGIN(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)

			/*! @brief The case-sensitive display name stored. */
			std::string mName{};

			/*! @brief The statuses this one interacts with and the corresponding actions. */
			std::vector<Interaction<StatusID>> mStatusInteractions{};

			/*! @brief The stable built-in or user-assigned identifier. */
			StatusID mStatusID{};

			/*! @brief What classification the status is. */
			StatusClassification mStatusClassification{StatusClassification::NonVolatile};

			// NOLINTEND(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)
	};
} // namespace PocketCore::Status

#endif
