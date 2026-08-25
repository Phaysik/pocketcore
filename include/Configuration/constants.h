/*! @file constants.h
	@brief Defines configuration constants.
	@date 07/27/2026
	@since 0.1.0
	@version 0.4.1
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

	// MARK: Constexpr statements

	inline constexpr us MAX_TYPES{1'000};
	inline constexpr ub MAX_TYPES_PER_POKEMON{2};

	inline constexpr us MAX_ABILITIES{1'000};
	inline constexpr ub MAX_ABILITIES_PER_POKEMON{1};
	inline constexpr ub MAX_ABILITY_POOL_PER_POKEMON{10};

	inline constexpr us MAX_ITEMS{1'000};
	inline constexpr ub MAX_ITEMS_PER_POKEMON{1};

	inline constexpr us MAX_MOVES{1'000};
	inline constexpr ub MAX_MOVES_PER_POKEMON{4};

	inline constexpr us MAX_STATUSES{1'000};
	inline constexpr us MAX_STATUSES_PER_POKEMON{5};

	inline constexpr us MAX_NATURES{1'000};
	inline constexpr ub MAX_NATURES_PER_POKEMON{1};

	inline constexpr us MAX_POKEMON{10'000};
	inline constexpr us MAX_LEARNSETS{1'000};

	inline constexpr us MAX_WEATHERS{1'000};
	inline constexpr us MAX_TERRAINS{1'000};
	inline constexpr us MAX_MULTIPLIERS{1'000};
	inline constexpr us MAX_EFFECTS{1'000};

	inline constexpr ub MIN_CRITICAL_HIT_VALUE{0};
	inline constexpr ub MAX_CRITICAL_HIT_VALUE{99};
	inline constexpr ub CRITICAL_HIT_PERCENTAGE{5}; // Out of 100 i.e 5 = 5%

	inline constexpr double BASE_MULTIPLIER_VALUE{1.0};
	inline constexpr double CRITICAL_HIT_MULTIPLIER_VALUE{1.5};
	inline constexpr double STAB_HIT_MULTIPLIER_VALUE{1.5};
	inline constexpr double TARGETS_HIT_MULTIPLIER_VALUE{0.75};
	inline constexpr double POPULATION_BOMB_FIRST_HIT_MULTIPLIER_VALUE{1.0};
	inline constexpr double POPULATION_BOMB_CONSECUTIVE_HIT_MULTIPLIER_VALUE{0.5};
	inline constexpr double BURN_MULTIPLIER_VALUE{0.5};

	// These should ideally be within [0, 100] since the randomly grabbed value will be between them and then divided by 100
	// i.e [85, 100] could generate 93 = 0.93
	inline constexpr ub RANDOMIZATION_MULTIPLIER_MIN_VALUE{85};
	inline constexpr ub RANDOMIZATION_MULTIPLIER_MAX_VALUE{100};

	inline constexpr double NOT_VERY_EFFECTIVE_VALUE{0.5};
	inline constexpr double EFFECTIVE_VALUE{1.0};
	inline constexpr double SUPER_EFFECTIVE_VALUE{2.0};
	inline constexpr double NO_EFFECTIVE_VALUE{0.0};

	inline constexpr double WEATHER_NULLIFICATION_MULTIPLIER_VALUE{1.0};
	inline constexpr double WATER_MOVE_IN_RAIN_MULTIPLIER_VALUE{1.5};
	inline constexpr double FIRE_MOVE_IN_RAIN_MULTIPLIER_VALUE{0.5};
	inline constexpr double FIRE_MOVE_IN_HARSH_SUNLIGHT_MULTIPLIER_VALUE{1.5};
	inline constexpr double WATER_MOVE_IN_HARSH_SUNLIGHT_MULTIPLIER_VALUE{0.5};

	inline constexpr double ELECTRIC_BUFF_IN_TERRAIN_BASE_DAMAGE_VALUE{1.3};
	inline constexpr double GRASS_BUFF_IN_TERRAIN_BASE_DAMAGE_VALUE{1.3};
	inline constexpr double PSYCHIC_BUFF_IN_TERRAIN_BASE_DAMAGE_VALUE{1.3};
	inline constexpr double DRAGON_DEBUFF_IN_TERRAIN_BASE_DAMAGE_VALUE{0.5};

	inline constexpr ub MAX_SUPPRESSION_RULES_PER_TRIGGER{2};

	inline constexpr double BASE_STAGE_MULTIPLIER_NUMERATOR{2.0};
	inline constexpr double BASE_STAGE_MULTIPLIER_DENOMINATOR{2.0};

	inline constexpr double BASE_ACCURACY_MULTIPLIER_NUMERATOR{3.0};
	inline constexpr double BASE_ACCURACY_MULTIPLIER_DENOMINATOR{3.0};
	inline constexpr double BASE_EVASION_MULTIPLIER_NUMERATOR{3.0};
	inline constexpr double BASE_EVASION_MULTIPLIER_DENOMINATOR{3.0};

	inline constexpr ub MIN_ACCURACY_HIT_VALUE{1};
	inline constexpr ub MAX_ACCURACY_HIT_VALUE{100};

	inline constexpr ub LEVEL_DAMAGE_FACTOR_NUMERATOR{2};
	inline constexpr ub LEVEL_DAMAGE_FACTOR_DENOMINATOR{5};
	inline constexpr ub LEVEL_DAMAGE_FACTOR_OFFSET{2};

	inline constexpr double FIXED_POINT_MULTIPLIER_NUMERATOR{4'096.0};
	inline constexpr double FIXED_POINT_MULTIPLIER_DENOMINATOR{4'096.0};
	inline constexpr double ROUND_DOWN_HALF_POINT{0.5};
	inline constexpr double ROUND_DOWN_TOLERANCE{1e-9};

	inline constexpr double NATURE_STAT_BASE_MULTIPLIER{1.0};
	inline constexpr double NATURE_STAT_BOOST_MULTIPLIER{1.1};
	inline constexpr double NATURE_STAT_WEAKNESS_MULTIPLIER{0.9};

	inline constexpr sb MAX_STAT_STAGES{6};
	inline constexpr sb MAX_STAGE_CACHE{(MAX_STAT_STAGES * 2) + 1};
} // namespace PocketCore::Configuration

#endif
