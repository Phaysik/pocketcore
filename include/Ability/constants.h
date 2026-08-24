/*! @file constants.h
	@brief Contains constexpr assert message strings for the type registry.
	@date 07/26/2026
	@since 0.3.0
	@version 0.3.0
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
} // namespace PocketCore::Ability

#endif
