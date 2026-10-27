/*! @file constants.h
	@brief Contains constexpr assert message strings for the type registry.
	@date 10/27/2026
	@since 0.3.0
	@version 0.12.53
	@author Matthew Moore
*/

#ifndef INCLUDE_ABILITY_CONSTANTS_H
#define INCLUDE_ABILITY_CONSTANTS_H

#include <string_view>

namespace PocketCore::Ability
{
	inline constexpr std::string_view ABILITY_CLASSIFICATION_STANDARD{"Standard"};
	inline constexpr std::string_view ABILITY_CLASSIFICATION_HIDDEN{"Hidden"};

	inline constexpr std::string_view ABILITY_NAME_NONE{"None"};
	inline constexpr std::string_view ABILITY_NAME_DRIZZLE{"Drizzle"};
	inline constexpr std::string_view ABILITY_NAME_STENCH{"Stench"};
	inline constexpr std::string_view ABILITY_NAME_SPEED_BOOST{"Speed Boost"};
	inline constexpr std::string_view ABILITY_NAME_BATTLE_ARMOR{"Battle Armor"};
	inline constexpr std::string_view ABILITY_NAME_STURDY{"Sturdy"};

	inline constexpr std::string_view ABILITY_NAME_CLOUD_NINE{"Cloud Nine"};
	inline constexpr std::string_view ABILITY_NAME_LEVITATE{"Levitate"};
	inline constexpr std::string_view ABILITY_NAME_GUTS{"Guts"};
	inline constexpr std::string_view ABILITY_NAME_AIR_LOCK{"Air Lock"};
	inline constexpr std::string_view ABILITY_NAME_ELEVATE{"Elevate"};
} // namespace PocketCore::Ability

#endif
