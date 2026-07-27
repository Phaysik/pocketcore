/*! @file constants.h
	@brief Contains constexpr assert message strings for the type registry.
	@date 04/17/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_REGISTRY_CONSTANTS_H
#define INCLUDE_REGISTRY_CONSTANTS_H

#include <string_view>

namespace PocketCore::Ability
{
	constexpr std::string_view ABILITY_NAME_NONE{"None"};
	constexpr std::string_view ABILITY_NAME_DRIZZLE{"Drizzle"};
	constexpr std::string_view ABILITY_NAME_STENCH{"Stench"};
} // namespace PocketCore::Ability

#endif
