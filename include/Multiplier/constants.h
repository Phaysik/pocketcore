/*! @file constants.h
	@brief Contains constexpr assert message strings for the multiplier registry.
	@date 09/11/2026
	@since 0.8.1
	@version 0.12.24
	@author Matthew Moore
*/

#ifndef INCLUDE_MULTIPLIER_CONSTANTS_H
#define INCLUDE_MULTIPLIER_CONSTANTS_H

#include <string_view>

namespace PocketCore::Multiplier
{
	inline constexpr std::string_view MULTIPLIER_NAME_NONE{"None"};
	inline constexpr std::string_view MULTIPLIER_NAME_ABILITY{"Ability"};
	inline constexpr std::string_view MULTIPLIER_NAME_ITEM{"Item"};
	inline constexpr std::string_view MULTIPLIER_NAME_TARGETS{"Targets"};
	inline constexpr std::string_view MULTIPLIER_NAME_POPULATION_BOMB{"Population Bomb"};
	inline constexpr std::string_view MULTIPLIER_NAME_WEATHER{"Weather"};
	inline constexpr std::string_view MULTIPLIER_NAME_CRITICAL{"Critical"};
	inline constexpr std::string_view MULTIPLIER_NAME_RANDOMIZATION{"Randomization"};
	inline constexpr std::string_view MULTIPLIER_NAME_STAB{"Stab"};
	inline constexpr std::string_view MULTIPLIER_NAME_TYPE_EFFECTIVENESS{"Type Effectiveness"};
	inline constexpr std::string_view MULTIPLIER_NAME_BURN{"Burn"};
} // namespace PocketCore::Multiplier

#endif
