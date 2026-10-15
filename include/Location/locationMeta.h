/*! @file locationMeta.h
	@brief Defines the metadata stored for built-in and user-defined locations.
	@date 10/09/2026
	@since 0.12.51
	@version 0.12.51
	@author Matthew Moore
*/

#ifndef INCLUDE_LOCATION_LOCATION_META_H
#define INCLUDE_LOCATION_LOCATION_META_H

#include <string>

#include "Core/attributeMacros.h"
#include "Core/typedefs.h"

#include "constants.h"
#include "locationID.h"

namespace PocketCore::Location
{
	using PocketCore::Core::ub;
	using PocketCore::Core::us;

	/*! @struct LocationMeta Location/locationMeta.h
		@brief Stores a location's stable identifier, owned display name, acquisition category, and encounter-level specification.
		@details Owns its name and level specification by value. The identifier references a location without owning registry storage.
			Describes acquisition data rather than an individual Pokemon's current level or recorded history.
		@note Not thread-safe. Callers must synchronize concurrent access involving writes and validate level data before use.
		@date 10/09/2026
		@since 0.12.51
		@version 0.12.51
		@author Matthew Moore
	*/
	struct LocationMeta
	{
		public:
			/*! @brief Compares all location metadata fields for exact equality.
				@param[in] other The non-owning comparison reference, which must remain valid for the duration of the call.
				@return True when names, identifiers, acquisition categories, and level specifications match; otherwise false.
				@note Level specifications match only when their variant alternatives and stored values match.
					Supports compile-time and runtime comparison and marks the result nodiscard to discourage ignoring it.
				@date 10/09/2026
				@since 0.12.51
				@version 0.12.51
				@author Matthew Moore
			*/
			ATTR_NODISCARD constexpr bool operator==(const LocationMeta &other) const = default;

			// NOLINTBEGIN(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)

			/*! @brief The owned, case-sensitive location display name. */
			std::string mName{};

			/*! @brief The stable built-in or user-assigned location identifier.
				@note This value does not own the location registry entry; its default represents @ref NO_LOCATION_ID.
			*/
			LocationID mLocationID{};

			// NOLINTEND(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)
	};
} // namespace PocketCore::Location

#endif
