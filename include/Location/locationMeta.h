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
#include <variant>

#include "Core/attributeMacros.h"
#include "Core/typedefs.h"

#include "constants.h"
#include "locationID.h"

namespace PocketCore::Location
{
	using PocketCore::Core::ub;
	using PocketCore::Core::us;

	/*! @enum PokemonOrigin
		@showenumvalues
		@brief Identifies the acquisition category described by location metadata.
		@details Distinguishes wild encounters, egg hatching, and acquisition through a trade; it does not record an individual's history.
		@note All enum values must be handled exhaustively when processing Pokemon origins.
		@date 10/09/2026
		@since 0.12.51
		@version 0.12.51
		@author Matthew Moore
	*/
	enum class PokemonOrigin : ub
	{
		Wild,  /*! @brief Describes acquisition through a wild encounter. */
		Egg,   /*! @brief Describes acquisition through egg hatching. */
		Trade, /*! @brief Describes acquisition through a trade. */
	};

	/*! @struct SetLevelEncounter Location/locationMeta.h
		@brief Stores the single level assigned by a fixed-level encounter.
		@note Valid encounter data requires a nonzero level permitted by the applicable ruleset; this data carrier does not validate it.
		@date 10/09/2026
		@since 0.12.51
		@version 0.12.51
		@author Matthew Moore
	*/
	struct SetLevelEncounter
	{
		public:
			/*! @brief Compares the fixed encounter levels for equality.
				@param[in] other The non-owning comparison reference, which must remain valid for the duration of the call.
				@return True when both instances specify the same level; otherwise false.
				@note Supports compile-time and runtime comparison, is no-throw, and marks the result nodiscard to discourage ignoring it.
				@date 10/09/2026
				@since 0.12.51
				@version 0.12.51
				@author Matthew Moore
			*/
			ATTR_NODISCARD constexpr bool operator==(const SetLevelEncounter &other) const noexcept = default;

			// NOLINTBEGIN(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)

			/*! @brief The level assigned when the Pokemon is acquired, not its later current level. */
			us mLevel{MIN_ENCOUNTER_LEVEL};

			// NOLINTEND(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)
	};

	/*! @struct VariableLevelEncounter Location/locationMeta.h
		@brief Stores the inclusive level bounds permitted by a variable-level encounter.
		@details Consumers select one acquisition level within the bounds; this type does not specify a selection distribution.
		@note Valid encounter data requires nonzero, ruleset-permitted bounds with @ref mMinLevel no greater than @ref mMaxLevel.
			Callers are responsible for validation; this data carrier does not enforce the bounds.
		@date 10/09/2026
		@since 0.12.51
		@version 0.12.51
		@author Matthew Moore
	*/
	struct VariableLevelEncounter
	{
		public:
			/*! @brief Compares both inclusive encounter-level bounds for equality.
				@param[in] other The non-owning comparison reference, which must remain valid for the duration of the call.
				@return True when both minimum and maximum levels match; otherwise false.
				@note Supports compile-time and runtime comparison, is no-throw, and marks the result nodiscard to discourage ignoring it.
				@date 10/09/2026
				@since 0.12.51
				@version 0.12.51
				@author Matthew Moore
			*/
			ATTR_NODISCARD constexpr bool operator==(const VariableLevelEncounter &other) const noexcept = default;

			// NOLINTBEGIN(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)

			/*! @brief The inclusive minimum acquisition level; must be nonzero and no greater than @ref mMaxLevel. */
			us mMinLevel{MIN_ENCOUNTER_LEVEL};

			/*! @brief The inclusive maximum acquisition level; must be permitted by the ruleset and no less than @ref mMinLevel. */
			us mMaxLevel{};

			// NOLINTEND(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)
	};

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

			/*! @brief The fixed acquisition level or inclusive acquisition-level range.
				@details Stores either @ref SetLevelEncounter or @ref VariableLevelEncounter by value.
				@note Defaults to @ref SetLevelEncounter.
			*/
			std::variant<SetLevelEncounter, VariableLevelEncounter> mLevel{};

			/*! @brief The stable built-in or user-assigned location identifier.
				@note This value does not own the location registry entry; its default represents @ref NO_LOCATION_ID.
			*/
			LocationID mLocationID{};

			/*! @brief The acquisition category represented by this metadata, rather than an individual's transfer history.
				@note Defaults to @ref PokemonOrigin::Wild.
			*/
			PokemonOrigin mOrigin{};

			// NOLINTEND(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)
	};
} // namespace PocketCore::Location

#endif
