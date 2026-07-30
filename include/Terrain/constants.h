/*! @file constants.h
	@brief Contains constexpr assert message strings for the terrain registry.
	@date 04/17/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_TERRAIN_CONSTANTS_H
#define INCLUDE_TERRAIN_CONSTANTS_H

#include <string_view>

namespace PocketCore::Terrain
{
	constexpr std::string_view TERRAIN_NAME_NONE{"None"};
	constexpr std::string_view TERRAIN_NAME_ELECTRIC{"Electric"};
	constexpr std::string_view TERRAIN_NAME_GRASS{"Grass"};
	constexpr std::string_view TERRAIN_NAME_MISTY{"Misty"};
	constexpr std::string_view TERRAIN_NAME_PSYCHIC{"Psychic"};
} // namespace PocketCore::Terrain

#endif
