/*! @file constants.h
	@brief Contains the function declarations for creating a Detailed file description
	@date 07/27/2026
	@since 0.1.0
	@version 0.4.0
	@author Matthew Moore
*/

#ifndef INCLUDE_TYPES_CONSTANTS_H
#define INCLUDE_TYPES_CONSTANTS_H

#include <array>
#include <string_view>

#include "Core/typedefs.h"

#include "typeEffectiveness.h"

namespace PocketCore::Types
{
	using enum TypeEffectiveness;
	using PocketCore::Core::ub;

	constexpr ub MAX_BUILT_IN_TYPE_COUNT{18}; // The number of built-in types with predefined matchups

	// NOLINTBEGIN(readability-trailing-comma)

	inline constexpr std::array<TypeEffectiveness, MAX_BUILT_IN_TYPE_COUNT> NORMAL_TYPE_MATCHUP{E, E, E, E, E, NVE, E, NE, NVE,
																								E, E, E, E, E, E,	E, E,  E};
	inline constexpr std::array<TypeEffectiveness, MAX_BUILT_IN_TYPE_COUNT> FIGHTING_TYPE_MATCHUP{SE, E, NVE, NVE, E,	SE, NVE, NE, SE,
																								  E,  E, E,	  E,   NVE, SE, E,	 SE, NVE};
	inline constexpr std::array<TypeEffectiveness, MAX_BUILT_IN_TYPE_COUNT> FLYING_TYPE_MATCHUP{E, SE, E,  E,	E, NVE, SE, E, NVE,
																								E, E,  SE, NVE, E, E,	E,	E, E};
	inline constexpr std::array<TypeEffectiveness, MAX_BUILT_IN_TYPE_COUNT> POISON_TYPE_MATCHUP{E, E, E,  NVE, NVE, NVE, E, NVE, NE,
																								E, E, SE, E,   E,	E,	 E, E,	 SE};
	inline constexpr std::array<TypeEffectiveness, MAX_BUILT_IN_TYPE_COUNT> GROUND_TYPE_MATCHUP{E,	E, NE,	SE, E, SE, NVE, E, SE,
																								SE, E, NVE, SE, E, E,  E,	E, E};
	inline constexpr std::array<TypeEffectiveness, MAX_BUILT_IN_TYPE_COUNT> ROCK_TYPE_MATCHUP{E,  NVE, SE, E, NVE, E,  SE, E, NVE,
																							  SE, E,   E,  E, E,   SE, E,  E, E};
	inline constexpr std::array<TypeEffectiveness, MAX_BUILT_IN_TYPE_COUNT> BUG_TYPE_MATCHUP{E,	  NVE, NVE, NVE, E,	 E, E, NVE, NVE,
																							 NVE, E,   SE,	E,	 SE, E, E, SE,	NVE};
	inline constexpr std::array<TypeEffectiveness, MAX_BUILT_IN_TYPE_COUNT> GHOST_TYPE_MATCHUP{NE, E, E, E, E,	E, E, SE,  E,
																							   E,  E, E, E, SE, E, E, NVE, E};
	inline constexpr std::array<TypeEffectiveness, MAX_BUILT_IN_TYPE_COUNT> STEEL_TYPE_MATCHUP{E,	E,	 E, E,	 E, SE, E, E, NVE,
																							   NVE, NVE, E, NVE, E, SE, E, E, SE};
	inline constexpr std::array<TypeEffectiveness, MAX_BUILT_IN_TYPE_COUNT> FIRE_TYPE_MATCHUP{E,   E,	E,	E, E, NVE, SE,	E, SE,
																							  NVE, NVE, SE, E, E, SE,  NVE, E, E};
	inline constexpr std::array<TypeEffectiveness, MAX_BUILT_IN_TYPE_COUNT> WATER_TYPE_MATCHUP{E,  E,	E,	 E, SE, SE, E,	 E, E,
																							   SE, NVE, NVE, E, E,	E,	NVE, E, E};
	inline constexpr std::array<TypeEffectiveness, MAX_BUILT_IN_TYPE_COUNT> GRASS_TYPE_MATCHUP{E,	E,	NVE, NVE, SE, SE, NVE, E, NVE,
																							   NVE, SE, NVE, E,	  E,  E,  NVE, E, E};
	inline constexpr std::array<TypeEffectiveness, MAX_BUILT_IN_TYPE_COUNT> ELECTRIC_TYPE_MATCHUP{E, E,	 SE,  E,   NE, E, E,   E, E,
																								  E, SE, NVE, NVE, E,  E, NVE, E, E};
	inline constexpr std::array<TypeEffectiveness, MAX_BUILT_IN_TYPE_COUNT> PSYCHIC_TYPE_MATCHUP{E, SE, E, SE, E,	E, E, E,  NVE,
																								 E, E,	E, E,  NVE, E, E, NE, E};
	inline constexpr std::array<TypeEffectiveness, MAX_BUILT_IN_TYPE_COUNT> ICE_TYPE_MATCHUP{E,	  E,   SE, E, SE, E,   E,  E, NVE,
																							 NVE, NVE, SE, E, E,  NVE, SE, E, E};
	inline constexpr std::array<TypeEffectiveness, MAX_BUILT_IN_TYPE_COUNT> DRAGON_TYPE_MATCHUP{E, E, E, E, E, E, E,  E, NVE,
																								E, E, E, E, E, E, SE, E, NE};
	inline constexpr std::array<TypeEffectiveness, MAX_BUILT_IN_TYPE_COUNT> DARK_TYPE_MATCHUP{E, NVE, E, E, E,	E, E, SE,  E,
																							  E, E,	  E, E, SE, E, E, NVE, NVE};
	inline constexpr std::array<TypeEffectiveness, MAX_BUILT_IN_TYPE_COUNT> FAIRY_TYPE_MATCHUP{E,	SE, E, NVE, E, E, E,  E,  NVE,
																							   NVE, E,	E, E,	E, E, SE, SE, E};

	// NOLINTEND(readability-trailing-comma)

	inline constexpr std::string_view NORMAL_NAME{"Normal"};
	inline constexpr std::string_view FIGHTING_NAME{"Fighting"};
	inline constexpr std::string_view FLYING_NAME{"Flying"};
	inline constexpr std::string_view POISON_NAME{"Poison"};
	inline constexpr std::string_view GROUND_NAME{"Ground"};
	inline constexpr std::string_view ROCK_NAME{"Rock"};
	inline constexpr std::string_view BUG_NAME{"Bug"};
	inline constexpr std::string_view GHOST_NAME{"Ghost"};
	inline constexpr std::string_view STEEL_NAME{"Steel"};
	inline constexpr std::string_view FIRE_NAME{"Fire"};
	inline constexpr std::string_view WATER_NAME{"Water"};
	inline constexpr std::string_view GRASS_NAME{"Grass"};
	inline constexpr std::string_view ELECTRIC_NAME{"Electric"};
	inline constexpr std::string_view PSYCHIC_NAME{"Psychic"};
	inline constexpr std::string_view ICE_NAME{"Ice"};
	inline constexpr std::string_view DRAGON_NAME{"Dragon"};
	inline constexpr std::string_view DARK_NAME{"Dark"};
	inline constexpr std::string_view FAIRY_NAME{"Fairy"};
	inline constexpr std::string_view STELLAR_NAME{"Stellar"};
} // namespace PocketCore::Types

#endif
