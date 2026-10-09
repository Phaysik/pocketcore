/*! @file weatherID.h
	@brief Defines the open identifier used for built-in and user-defined weathers.
	@date 10/09/2026
	@since 0.8.0
	@version 0.12.51
	@author Matthew Moore
*/

#ifndef INCLUDE_WEATHER_WEATHER_ID_H
#define INCLUDE_WEATHER_WEATHER_ID_H

#include "ID/idInterface.h"

namespace PocketCore::Weather
{
	namespace Detail
	{
		/*! @brief Distinguishes weather identifiers from all other stable identifier domains. */
		struct WeatherIDTag;
	} // namespace Detail

	/*! @typedef WeatherID
		@brief A strongly typed stable identifier for any registered weather.
		@details Values are assigned by the weather registry. Unlike @ref BuiltinWeatherID, this type is open and can represent user-defined
	   abilities without extending an enum. Its tag prevents comparison or conversion with identifiers from other registry domains.
	*/
	using WeatherID = PocketCore::ID::IDInterface<Detail::WeatherIDTag, 0>;

	/*! @brief The stable identifier representing no weather. */
	inline constexpr WeatherID NO_WEATHER_ID{};
} // namespace PocketCore::Weather

#endif
