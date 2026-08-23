/*! @file constants.h
	@brief Contains constexpr assert message strings for the pokemon registry.
	@date 04/17/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_POKEMON_CONSTANTS_H
#define INCLUDE_POKEMON_CONSTANTS_H

#include <string_view>

namespace PocketCore::Pokemon
{
	constexpr std::string_view POKEMON_NAME_NONE{"None"};
	constexpr std::string_view POKEMON_NAME_BULBASAUR{"Bulbasaur"};
	constexpr std::string_view POKEMON_NAME_IVYSAUR{"Ivysaur"};
	constexpr std::string_view POKEMON_NAME_VENUSAUR{"Venusaur"};
	constexpr std::string_view POKEMON_NAME_CHARMANDER{"Charmander"};
	constexpr std::string_view POKEMON_NAME_CHARMELEON{"Charmeleon"};
	constexpr std::string_view POKEMON_NAME_CHARIZARD{"Charizard"};
	constexpr std::string_view POKEMON_NAME_SQUIRTLE{"Squirtle"};
	constexpr std::string_view POKEMON_NAME_WARTORTLE{"Wartortle"};
	constexpr std::string_view POKEMON_NAME_BLASTOISE{"Blastoise"};
} // namespace PocketCore::Pokemon

#endif
