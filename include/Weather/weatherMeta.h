/*! @file weatherMeta.h
	@brief Defines the metadata stored for built-in and user-defined weathers.
	@date 09/02/2026
	@since 0.8.0
	@version 0.12.16
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
		@date 09/02/2026
		@since 0.8.0
		@version 0.12.16
		@author Matthew Moore
	*/
	struct WeatherMeta
	{
		public:
			/*! @brief The weathers this one interacts with and the corresponding actions. */
			std::vector<Interaction<WeatherID>> mWeatherInteractions{};

			/*! @brief The case-sensitive display name stored as a non-owning view. */
			std::string_view mName{};

			/*! @brief The stable built-in or user-assigned identifier. */
			WeatherID mWeatherID{};
	};
} // namespace PocketCore::Weather

#endif
