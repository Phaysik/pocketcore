/*! @file volatileStatus.h
	@brief Defines the strongly typed wrapper distinguishing battle-slot-owned volatile statuses from Pokemon-owned non-volatile statuses.
	@date 09/28/2026
	@since 0.12.45
	@version 0.12.45
	@author Matthew Moore
*/

#ifndef INCLUDE_STATUS_VOLATILESTATUS_H
#define INCLUDE_STATUS_VOLATILESTATUS_H

#include "statusID.h"

namespace PocketCore::Status
{
	/*! @struct VolatileStatus Status/volatileStatus.h
		@brief Wraps a @ref StatusID as a distinct type owned by a battle slot rather than a Pokemon.
		@details A volatile status lives on a @ref PocketCore::Battle::BattleSlot and clears when the slot's occupant changes, whereas a
	   non-volatile status is stored directly as a @ref StatusID on a Pokemon. Because this wrapper neither implicitly converts to nor from
	   @ref StatusID, the two categories are distinct types and cannot be silently assigned to one another; a volatile identifier must be
	   wrapped explicitly through brace initialization and read back through the @ref mStatusID member.
		@date 09/28/2026
		@since 0.12.45
		@version 0.12.45
		@author Matthew Moore
	*/
	struct VolatileStatus
	{
		public:
			/*! @brief Compares two VolatileStatus instances for equivalent metadata.
				@details Compares all fields exactly.
				@param[in] other The VolatileStatus instance to compare.
				@return True when both instances contain equivalent metadata; otherwise false.
				@since 0.12.45
				@version 0.12.45
			*/
			ATTR_NODISCARD constexpr bool operator==(const VolatileStatus &other) const = default;

			// NOLINTBEGIN(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)

			/*! @brief The wrapped registry status identifier.
				@details Defaults to @ref NO_STATUS_ID to represent an unoccupied volatile-status slot.
			*/
			StatusID mStatusID{NO_STATUS_ID};

			// NOLINTEND(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)
	};
} // namespace PocketCore::Status

#endif
