/*! @file constants.h
	@brief Defines configuration constants.
	@date 08/12/2026
	@since 0.1.0
	@version 0.10.0
	@author Matthew Moore
*/

#ifndef INCLUDE_CONFIGURATION_CONSTANTS_H
#define INCLUDE_CONFIGURATION_CONSTANTS_H

#include "Core/typedefs.h"

namespace PocketCore::Configuration
{
	using PocketCore::Core::sb;
	using PocketCore::Core::ub;
	using PocketCore::Core::us;

	/*! @brief The maximum number of registered Pokemon types. */
	inline constexpr us MAX_TYPES{1'000};

	/*! @brief The maximum number of types assigned to one Pokemon. */
	inline constexpr ub MAX_TYPES_PER_POKEMON{2};

	/*! @brief The maximum number of registered abilities. */
	inline constexpr us MAX_ABILITIES{1'000};

	/*! @brief The maximum number of abilities assigned to one Pokemon. */
	inline constexpr ub MAX_ABILITIES_PER_POKEMON{1};

	/*! @brief The maximum number of abilities available in one Pokemon's ability pool. */
	inline constexpr ub MAX_ABILITY_POOL_PER_POKEMON{10};

	/*! @brief The maximum number of registered items. */
	inline constexpr us MAX_ITEMS{1'000};

	/*! @brief The maximum number of items assigned to one Pokemon. */
	inline constexpr ub MAX_ITEMS_PER_POKEMON{1};

	/*! @brief The maximum number of registered moves. */
	inline constexpr us MAX_MOVES{1'000};

	/*! @brief The maximum number of moves known by one Pokemon. */
	inline constexpr ub MAX_MOVES_PER_POKEMON{4};

	/*! @brief The maximum number of registered statuses. */
	inline constexpr us MAX_STATUSES{1'000};

	/*! @brief The maximum number of statuses assigned to one Pokemon. */
	inline constexpr us MAX_STATUSES_PER_POKEMON{5};

	/*! @brief The maximum number of registered natures. */
	inline constexpr us MAX_NATURES{1'000};

	/*! @brief The maximum number of natures assigned to one Pokemon. */
	inline constexpr ub MAX_NATURES_PER_POKEMON{1};

	/*! @brief The maximum number of registered Pokemon species. */
	inline constexpr us MAX_POKEMON{10'000};

	/*! @brief The maximum number of registered learnsets. */
	inline constexpr us MAX_LEARNSETS{1'000};

	/*! @brief The maximum number of registered weather conditions. */
	inline constexpr us MAX_WEATHERS{1'000};

	/*! @brief The maximum number of registered terrain conditions. */
	inline constexpr us MAX_TERRAINS{1'000};

	/*! @brief The maximum number of registered damage multipliers. */
	inline constexpr us MAX_MULTIPLIERS{1'000};

	/*! @brief The maximum number of registered effects. */
	inline constexpr us MAX_EFFECTS{1'000};

	/*! @brief The inclusive lower bound of a critical-hit random roll. */
	inline constexpr ub MIN_CRITICAL_HIT_VALUE{0};

	/*! @brief The inclusive upper bound of a critical-hit random roll. */
	inline constexpr ub MAX_CRITICAL_HIT_VALUE{99};

	/*! @brief The critical-hit probability represented as a percentage from 0 to 100. */
	inline constexpr ub CRITICAL_HIT_PERCENTAGE{5};

	/*! @brief The neutral value for a multiplicative damage modifier. */
	inline constexpr double BASE_MULTIPLIER_VALUE{1.0};

	/*! @brief The damage multiplier applied to critical hits. */
	inline constexpr double CRITICAL_HIT_MULTIPLIER_VALUE{1.5};

	/*! @brief The damage multiplier applied when the move shares a type with the user. */
	inline constexpr double STAB_HIT_MULTIPLIER_VALUE{1.5};

	/*! @brief The damage multiplier applied to moves affecting multiple targets. */
	inline constexpr double TARGETS_HIT_MULTIPLIER_VALUE{0.75};

	/*! @brief The multiplier applied to the first hit of Population Bomb. */
	inline constexpr double POPULATION_BOMB_FIRST_HIT_MULTIPLIER_VALUE{1.0};

	/*! @brief The multiplier applied to consecutive hits of Population Bomb. */
	inline constexpr double POPULATION_BOMB_CONSECUTIVE_HIT_MULTIPLIER_VALUE{0.5};

	/*! @brief The damage multiplier applied to a burned Pokemon's applicable physical moves. */
	inline constexpr double BURN_MULTIPLIER_VALUE{0.5};

	/*! @brief The inclusive lower bound of the random damage multiplier percentage. */
	inline constexpr ub RANDOMIZATION_MULTIPLIER_MIN_VALUE{85};

	/*! @brief The inclusive upper bound of the random damage multiplier percentage. */
	inline constexpr ub RANDOMIZATION_MULTIPLIER_MAX_VALUE{100};

	/*! @brief The type-effectiveness multiplier for not very effective moves. */
	inline constexpr double NOT_VERY_EFFECTIVE_VALUE{0.5};

	/*! @brief The neutral type-effectiveness multiplier. */
	inline constexpr double EFFECTIVE_VALUE{1.0};

	/*! @brief The type-effectiveness multiplier for super-effective moves. */
	inline constexpr double SUPER_EFFECTIVE_VALUE{2.0};

	/*! @brief The type-effectiveness multiplier for moves with no effect. */
	inline constexpr double NO_EFFECTIVE_VALUE{0.0};

	/*! @brief The multiplier used when weather nullifies a weather-based effect. */
	inline constexpr double WEATHER_NULLIFICATION_MULTIPLIER_VALUE{1.0};

	/*! @brief The water-move damage multiplier during rain. */
	inline constexpr double WATER_MOVE_IN_RAIN_MULTIPLIER_VALUE{1.5};

	/*! @brief The fire-move damage multiplier during rain. */
	inline constexpr double FIRE_MOVE_IN_RAIN_MULTIPLIER_VALUE{0.5};

	/*! @brief The fire-move damage multiplier during harsh sunlight. */
	inline constexpr double FIRE_MOVE_IN_HARSH_SUNLIGHT_MULTIPLIER_VALUE{1.5};

	/*! @brief The water-move damage multiplier during harsh sunlight. */
	inline constexpr double WATER_MOVE_IN_HARSH_SUNLIGHT_MULTIPLIER_VALUE{0.5};

	/*! @brief The electric-move base-damage multiplier in electric terrain. */
	inline constexpr double ELECTRIC_BUFF_IN_TERRAIN_BASE_DAMAGE_VALUE{1.3};

	/*! @brief The grass-move base-damage multiplier in grassy terrain. */
	inline constexpr double GRASS_BUFF_IN_TERRAIN_BASE_DAMAGE_VALUE{1.3};

	/*! @brief The psychic-move base-damage multiplier in psychic terrain. */
	inline constexpr double PSYCHIC_BUFF_IN_TERRAIN_BASE_DAMAGE_VALUE{1.3};

	/*! @brief The dragon-move base-damage multiplier in misty terrain. */
	inline constexpr double DRAGON_DEBUFF_IN_TERRAIN_BASE_DAMAGE_VALUE{0.5};

	/*! @brief The maximum number of suppression rules associated with one trigger. */
	inline constexpr ub MAX_SUPPRESSION_RULES_PER_TRIGGER{2};

	/*! @brief The numerator used by the non-negative standard stat-stage formula. */
	inline constexpr double BASE_STAGE_MULTIPLIER_NUMERATOR{2.0};

	/*! @brief The denominator used by the negative standard stat-stage formula. */
	inline constexpr double BASE_STAGE_MULTIPLIER_DENOMINATOR{2.0};

	/*! @brief The numerator used by the non-negative accuracy-stage formula. */
	inline constexpr double BASE_ACCURACY_MULTIPLIER_NUMERATOR{3.0};

	/*! @brief The denominator used by the negative accuracy-stage formula. */
	inline constexpr double BASE_ACCURACY_MULTIPLIER_DENOMINATOR{3.0};

	/*! @brief The numerator used by the negative evasion-stage formula. */
	inline constexpr double BASE_EVASION_MULTIPLIER_NUMERATOR{3.0};

	/*! @brief The denominator used by the non-negative evasion-stage formula. */
	inline constexpr double BASE_EVASION_MULTIPLIER_DENOMINATOR{3.0};

	/*! @brief The inclusive lower bound of an accuracy hit roll. */
	inline constexpr ub MIN_ACCURACY_HIT_VALUE{1};

	/*! @brief The inclusive upper bound of an accuracy hit roll. */
	inline constexpr ub MAX_ACCURACY_HIT_VALUE{100};

	/*! @brief The numerator of the level-based damage factor. */
	inline constexpr ub LEVEL_DAMAGE_FACTOR_NUMERATOR{2};

	/*! @brief The denominator of the level-based damage factor. */
	inline constexpr ub LEVEL_DAMAGE_FACTOR_DENOMINATOR{5};

	/*! @brief The offset added to the user's level in the damage factor. */
	inline constexpr ub LEVEL_DAMAGE_FACTOR_OFFSET{2};

	/*! @brief The numerator used to represent fixed-point multipliers. */
	inline constexpr double FIXED_POINT_MULTIPLIER_NUMERATOR{4'096.0};

	/*! @brief The denominator used to represent fixed-point multipliers. */
	inline constexpr double FIXED_POINT_MULTIPLIER_DENOMINATOR{4'096.0};

	/*! @brief The fractional threshold used when rounding damage down. */
	inline constexpr double ROUND_DOWN_HALF_POINT{0.5};

	/*! @brief The tolerance used when comparing values near a rounding boundary. */
	inline constexpr double ROUND_DOWN_TOLERANCE{1e-9};

	/*! @brief The neutral nature multiplier for an unchanged stat. */
	inline constexpr double NATURE_STAT_BASE_MULTIPLIER{1.0};

	/*! @brief The nature multiplier for a boosted stat. */
	inline constexpr double NATURE_STAT_BOOST_MULTIPLIER{1.1};

	/*! @brief The nature multiplier for a weakened stat. */
	inline constexpr double NATURE_STAT_WEAKNESS_MULTIPLIER{0.9};

	/*! @brief The maximum positive or negative stat stage. */
	inline constexpr sb MAX_STAT_STAGES{6};

	/*! @brief The number of entries in each stat-stage multiplier cache. */
	inline constexpr sb MAX_STAGE_CACHE{(MAX_STAT_STAGES * 2) + 1};
} // namespace PocketCore::Configuration

#endif
