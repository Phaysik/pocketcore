/*! @file constants.h
	@brief Contains constexpr assert message strings for the type registry.
	@date 09/02/2026
	@since 0.3.0
	@version 0.12.17
	@author Matthew Moore
*/

#ifndef INCLUDE_ABILITY_CONSTANTS_H
#define INCLUDE_ABILITY_CONSTANTS_H

#include <string_view>

namespace PocketCore::Ability
{
	inline constexpr std::string_view ABILITY_NAME_NONE{"None"};
	inline constexpr std::string_view ABILITY_NAME_DRIZZLE{"Drizzle"};
	inline constexpr std::string_view ABILITY_NAME_STENCH{"Stench"};

	inline constexpr std::string_view ABILITY_NAME_GUTS{"Guts"};
	inline constexpr std::string_view ABILITY_NAME_AIR_LOCK{"Air Lock"};
} // namespace PocketCore::Ability

#endif
