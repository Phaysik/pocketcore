/*! @file constants.h
	@brief Contains constexpr assert message strings for the learnset registry.
	@date 10/08/2026
	@since 0.12.24
	@version 0.12.50
	@author Matthew Moore
*/

#ifndef INCLUDE_LEARNSET_CONSTANTS_H
#define INCLUDE_LEARNSET_CONSTANTS_H

#include <string_view>

namespace PocketCore::Learnset
{
	inline constexpr std::string_view LEARNSET_NAME_NONE{"None"};
	inline constexpr std::string_view LEARNSET_NAME_BULBASAUR{"Bulbasaur Learnset"};
	inline constexpr std::string_view LEARNSET_NAME_IVYSAUR{"Ivysaur Learnset"};
	inline constexpr std::string_view LEARNSET_NAME_VENUSAUR{"Venusaur Learnset"};
	inline constexpr std::string_view LEARNSET_NAME_CHARMANDER{"Charmander Learnset"};
	inline constexpr std::string_view LEARNSET_NAME_CHARMELEON{"Charmeleon Learnset"};
	inline constexpr std::string_view LEARNSET_NAME_CHARIZARD{"Charizard Learnset"};
	inline constexpr std::string_view LEARNSET_NAME_SQUIRTLE{"Squirtle Learnset"};
	inline constexpr std::string_view LEARNSET_NAME_WARTORTLE{"Wartortle Learnset"};
	inline constexpr std::string_view LEARNSET_NAME_BLASTOISE{"Blastoise Learnset"};
} // namespace PocketCore::Learnset

#endif
