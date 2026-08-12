/*! @file constants.h
	@brief Contains constexpr assert message strings for the type registry.
	@date 04/17/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_EFFECT_CONSTANTS_H
#define INCLUDE_EFFECT_CONSTANTS_H

#include <string_view>

namespace PocketCore::Effect
{
	inline constexpr std::string_view EFFECT_NAME_NONE{"None"};
	inline constexpr std::string_view EFFECT_NAME_CRITICAL_HIT{"Critical Hit"};
	inline constexpr std::string_view EFFECT_NAME_BASE_DAMAGE{"Base Damage"};
	inline constexpr std::string_view EFFECT_NAME_TARGETS{"Targets"};
	inline constexpr std::string_view EFFECT_NAME_WEATHER{"Weather"};
	inline constexpr std::string_view EFFECT_NAME_TERRAIN{"Terrain"};
	inline constexpr std::string_view EFFECT_NAME_POPULATION_BOMB{"Population Bomb"};
	inline constexpr std::string_view EFFECT_NAME_RANDOMIZATION{"Randomization"};
	inline constexpr std::string_view EFFECT_NAME_STAB{"Same Type Attack Bonus"};
	inline constexpr std::string_view EFFECT_NAME_TYPE_EFFECTIVENESS{"Type Effectiveness"};
	inline constexpr std::string_view EFFECT_NAME_BURN_DAMAGE_REDUCTION{"Burn Damage Reduction"};
	inline constexpr std::string_view EFFECT_NAME_FLINCH{"Flinch"};
	inline constexpr std::string_view EFFECT_NAME_RECOIL{"Recoil"};
	inline constexpr std::string_view EFFECT_NAME_STATUS_APPLY{"Status Apply"};
	inline constexpr std::string_view EFFECT_NAME_STATUS_REMOVE{"Status Remove"};
	inline constexpr std::string_view EFFECT_NAME_STATUS_TURN_SKIP{"Status Turn Skip"};
	inline constexpr std::string_view EFFECT_NAME_STATUS_TICK{"Status Tick"};
	inline constexpr std::string_view EFFECT_NAME_ACCURACY_CHECK{"Accuracy Check"};
	inline constexpr std::string_view EFFECT_NAME_SET_SANDSTORM{"Set Sandstorm"};
	inline constexpr std::string_view EFFECT_NAME_SET_SUN{"Set Sun"};
	inline constexpr std::string_view EFFECT_NAME_SET_RAIN{"Set Rain"};
	inline constexpr std::string_view EFFECT_NAME_PSYCHIC_TERRAIN_PRIORITY_BLOCK{"Psychic Terrain Priority Block"};
} // namespace PocketCore::Effect

#endif
