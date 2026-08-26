/*! @file constants.h
	@brief Contains constexpr assert message strings for the weather registry.
	@date 07/30/2026
	@since 0.8.0
	@version 0.8.0
	@author Matthew Moore
*/

#ifndef INCLUDE_WEATHER_CONSTANTS_H
#define INCLUDE_WEATHER_CONSTANTS_H

#include <string_view>

namespace PocketCore::Weather
{
	constexpr std::string_view WEATHER_NAME_NONE{"None"};
	constexpr std::string_view WEATHER_NAME_HARSH_SUNLIGHT{"Harsh Sunlight"};
	constexpr std::string_view WEATHER_NAME_RAIN{"Rain"};
	constexpr std::string_view WEATHER_NAME_SANDSTORM{"Sandstorm"};
	constexpr std::string_view WEATHER_NAME_HAIL{"Hail"};
	constexpr std::string_view WEATHER_NAME_SNOW{"Snow"};
	constexpr std::string_view WEATHER_NAME_FOG{"Fog"};
	constexpr std::string_view WEATHER_NAME_EXTREMELY_HARSH_SUNLIGHT{"Extremely Harsh Sunlight"};
	constexpr std::string_view WEATHER_NAME_HEAVY_RAIN{"Heavy Rain"};
	constexpr std::string_view WEATHER_NAME_STRONG_WINDS{"Strong Winds"};
	constexpr std::string_view WEATHER_NAME_SHADOWY_AURA{"Shadowy Aura"};
} // namespace PocketCore::Weather

#endif
