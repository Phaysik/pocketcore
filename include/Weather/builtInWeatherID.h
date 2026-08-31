/*! @file builtinWeatherID.h
	@brief Defines identifiers for weathers compiled into PocketCore.
	@date 08/22/2026
	@since 0.8.0
	@version 0.11.6
	@author Matthew Moore
*/

#ifndef INCLUDE_WEATHER_BUILTIN_WEATHER_ID_H
#define INCLUDE_WEATHER_BUILTIN_WEATHER_ID_H

#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Weather/weatherID.h"

namespace PocketCore::Weather
{
	/*! @enum BuiltinWeatherID
		@showenumvalues
		@brief Names the weathers provided by PocketCore itself.
		@details This closed enum is only a catalog of built-in weathers. Runtime state and user-facing APIs use the open @ref WeatherID
	   type.
		@note All enum values must be handled exhaustively when registering built-in metadata.
		@date 08/22/2026
		@since 0.8.0
		@version 0.11.6
		@author Matthew Moore
	*/
	enum class BuiltinWeatherID : PocketCore::Core::ub
	{
		None,
		HarshSunlight,
		Rain,
		Sandstorm,
		Hail,
		Snow,
		Fog,
		ExtremelyHarshSunlight,
		HeavyRain,
		StrongWinds,
		ShadowyAura,
		FinalWeather,
	};

	/*! @brief Converts a built-in catalog value to its stable registry identifier.
		@param[in] builtinWeatherID The built-in weather to convert.
		@return The corresponding open weather identifier.
		@since 0.8.0
		@version 0.8.0
	*/
	ATTR_NODISCARD constexpr WeatherID toWeatherID(const BuiltinWeatherID builtinWeatherID) noexcept
	{
		return WeatherID{static_cast<PocketCore::Core::ub>(builtinWeatherID)};
	}
} // namespace PocketCore::Weather

#endif
