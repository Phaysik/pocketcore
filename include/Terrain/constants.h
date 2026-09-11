/*! @file constants.h
	@brief Contains constexpr assert message strings for the terrain registry.
	@date 09/10/2026
	@since 0.8.0
	@version 0.12.20
	@author Matthew Moore
*/

#ifndef INCLUDE_TERRAIN_CONSTANTS_H
#define INCLUDE_TERRAIN_CONSTANTS_H

#include <string_view>

namespace PocketCore::Terrain
{
	constexpr std::string_view TERRAIN_NAME_NONE{"None"};
	constexpr std::string_view TERRAIN_NAME_ELECTRIC{"Electric"};
	constexpr std::string_view TERRAIN_NAME_GRASSY{"Grassy"};
	constexpr std::string_view TERRAIN_NAME_MISTY{"Misty"};
	constexpr std::string_view TERRAIN_NAME_PSYCHIC{"Psychic"};
} // namespace PocketCore::Terrain

#endif
