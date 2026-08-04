/*! @file constants.h
	@brief Contains constexpr assert message strings for the multiplier registry.
	@date 04/17/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_MULTIPLIER_CONSTANTS_H
#define INCLUDE_MULTIPLIER_CONSTANTS_H

#include <string_view>

namespace PocketCore::Multiplier
{
	constexpr std::string_view MULTIPLIER_NAME_NONE{"None"};
	constexpr std::string_view MULTIPLIER_NAME_ABILITY{"Ability"};
	constexpr std::string_view MULTIPLIER_NAME_ITEM{"Item"};
	constexpr std::string_view MULTIPLIER_NAME_TARGETS{"Targets"};
	constexpr std::string_view MULTIPLIER_NAME_POPULATION_BOMB{"Population Bomb"};
	constexpr std::string_view MULTIPLIER_NAME_WEATHER{"Weather"};
	constexpr std::string_view MULTIPLIER_NAME_CRITICAL{"Critical"};
	constexpr std::string_view MULTIPLIER_NAME_RANDOMIZATION{"Randomization"};
	constexpr std::string_view MULTIPLIER_NAME_STAB{"Stab"};
	constexpr std::string_view MULTIPLIER_NAME_TYPE_EFFECTIVENESS{"Type Effectiveness"};
	constexpr std::string_view MULTIPLIER_NAME_BURN{"Burn"};
} // namespace PocketCore::Multiplier

#endif
