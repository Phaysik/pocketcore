/*! @file weatherMeta.h
	@brief Defines the metadata stored for built-in and user-defined weathers.
	@date 09/03/2026
	@since 0.8.0
	@version 0.12.19
	@author Matthew Moore
*/

#ifndef INCLUDE_WEATHER_WEATHERMETA_H
#define INCLUDE_WEATHER_WEATHERMETA_H

#include <string_view>
#include <vector>

#include "Interaction/interaction.h"

#include "weatherID.h"

namespace PocketCore::Weather
{
	using PocketCore::Interaction::Interaction;

	/*! @struct WeatherMeta Weather/weatherMeta.h
		@brief Stores one weather's stable ID, display name, and owned trigger definitions.
		@details The trigger vector owns its elements and their effect vectors. The display name is a non-owning view whose backing storage
	   must remain valid while this metadata is registered.
		@date 09/03/2026
		@since 0.8.0
		@version 0.12.19
		@author Matthew Moore
	*/
	struct WeatherMeta
	{
		public:
			/*! @brief Compares two WeatherMeta instances for equivalent metadata.
				@details Compares all fields exactly.
				@param[in] other The WeatherMeta instance to compare.
				@return True when both instances contain equivalent metadata; otherwise false.
				@since 0.12.19
				@version 0.12.19
			*/
			ATTR_NODISCARD constexpr bool operator==(const WeatherMeta &other) const noexcept = default;

			// NOLINTBEGIN(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)

			/*! @brief The weathers this one interacts with and the corresponding actions. */
			std::vector<Interaction<WeatherID>> mWeatherInteractions{};

			/*! @brief The case-sensitive display name stored as a non-owning view. */
			std::string_view mName{};

			/*! @brief The stable built-in or user-assigned identifier. */
			WeatherID mWeatherID{};

			// NOLINTEND(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)
	};
} // namespace PocketCore::Weather

#endif
