/*! @file constants.h
	@brief Contains constexpr assert message strings for the weather registry.
	@date 09/11/2026
	@since 0.8.0
	@version 0.12.24
	@author Matthew Moore
*/

#ifndef INCLUDE_WEATHER_CONSTANTS_H
#define INCLUDE_WEATHER_CONSTANTS_H

#include <string_view>

namespace PocketCore::Weather
{
	inline constexpr std::string_view WEATHER_NAME_NONE{"None"};
	inline constexpr std::string_view WEATHER_NAME_HARSH_SUNLIGHT{"Harsh Sunlight"};
	inline constexpr std::string_view WEATHER_NAME_RAIN{"Rain"};
	inline constexpr std::string_view WEATHER_NAME_SANDSTORM{"Sandstorm"};
	inline constexpr std::string_view WEATHER_NAME_HAIL{"Hail"};
	inline constexpr std::string_view WEATHER_NAME_SNOW{"Snow"};
	inline constexpr std::string_view WEATHER_NAME_FOG{"Fog"};
	inline constexpr std::string_view WEATHER_NAME_EXTREMELY_HARSH_SUNLIGHT{"Extremely Harsh Sunlight"};
	inline constexpr std::string_view WEATHER_NAME_HEAVY_RAIN{"Heavy Rain"};
	inline constexpr std::string_view WEATHER_NAME_STRONG_WINDS{"Strong Winds"};
	inline constexpr std::string_view WEATHER_NAME_SHADOWY_AURA{"Shadowy Aura"};
} // namespace PocketCore::Weather

#endif
