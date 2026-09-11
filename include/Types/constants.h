/*! @file constants.h
	@brief Contains the function declarations for creating a Detailed file description
	@date 09/10/2026
	@since 0.1.0
	@version 0.12.20
	@author Matthew Moore
*/

#ifndef INCLUDE_TYPES_CONSTANTS_H
#define INCLUDE_TYPES_CONSTANTS_H

#include <array>
#include <string_view>

#include "Configuration/constants.h"

#include "typeEffectiveness.h"

namespace PocketCore::Type
{
	using enum TypeEffectiveness;
	using PocketCore::Configuration::MAX_TYPES;

	inline constexpr std::array<TypeEffectiveness, MAX_TYPES> INVALID_TYPE_CHART_ROW{};

	// NOLINTBEGIN(readability-trailing-comma)

	inline constexpr std::array<TypeEffectiveness, MAX_TYPES> NORMAL_TYPE_MATCHUP{NOT_DEFINED, E, E, E,	  E,  E, E, E,	 E, E,
																				  E,		   E, E, NVE, NE, E, E, NVE, E};
	inline constexpr std::array<TypeEffectiveness, MAX_TYPES> FIRE_TYPE_MATCHUP{NOT_DEFINED, E, NVE, NVE, E, SE,  SE, E,  E, E,
																				E,			 E, SE,	 NVE, E, NVE, E,  SE, E};
	inline constexpr std::array<TypeEffectiveness, MAX_TYPES> WATER_TYPE_MATCHUP{NOT_DEFINED, E, SE, NVE, E, NVE, E, E, E, SE,
																				 E,			  E, E,	 SE,  E, NVE, E, E, E};
	inline constexpr std::array<TypeEffectiveness, MAX_TYPES> ELECTRIC_TYPE_MATCHUP{NOT_DEFINED, E, E, SE, NVE, NVE, E, E, E, NE,
																					SE,			 E, E, E,  E,	NVE, E, E, E};
	inline constexpr std::array<TypeEffectiveness, MAX_TYPES> GRASS_TYPE_MATCHUP{NOT_DEFINED, E, NVE, SE, E, NVE, E, E,	  NVE, SE,
																				 NVE,		  E, NVE, SE, E, NVE, E, NVE, E};
	inline constexpr std::array<TypeEffectiveness, MAX_TYPES> ICE_TYPE_MATCHUP{NOT_DEFINED, E, NVE, NVE, E, SE, NVE, E,	  E, SE,
																			   SE,			E, E,	E,	 E, SE, E,	 NVE, E};
	inline constexpr std::array<TypeEffectiveness, MAX_TYPES> FIGHTING_TYPE_MATCHUP{NOT_DEFINED, SE,  E,   E,  E,  E, SE, E,  NVE, E,
																					NVE,		 NVE, NVE, SE, NE, E, SE, SE, NVE};
	inline constexpr std::array<TypeEffectiveness, MAX_TYPES> POISON_TYPE_MATCHUP{NOT_DEFINED, E, E, E,	  E,   SE, E, E,  NVE, NVE,
																				  E,		   E, E, NVE, NVE, E,  E, NE, SE};
	inline constexpr std::array<TypeEffectiveness, MAX_TYPES> GROUND_TYPE_MATCHUP{NOT_DEFINED, E, SE,  E,  SE, NVE, E, E,  SE, E,
																				  NE,		   E, NVE, SE, E,  E,	E, SE, E};
	inline constexpr std::array<TypeEffectiveness, MAX_TYPES> FLYING_TYPE_MATCHUP{NOT_DEFINED, E, E,  E,   NVE, SE, E, SE,	E, E,
																				  E,		   E, SE, NVE, E,	E,	E, NVE, E};
	inline constexpr std::array<TypeEffectiveness, MAX_TYPES> PSYCHIC_TYPE_MATCHUP{NOT_DEFINED, E,	 E, E, E, E, E,	 SE,  SE, E,
																				   E,			NVE, E, E, E, E, NE, NVE, E};
	inline constexpr std::array<TypeEffectiveness, MAX_TYPES> BUG_TYPE_MATCHUP{NOT_DEFINED, E,	NVE, E, E,	 SE, E,	 NVE, NVE, E,
																			   NVE,			SE, E,	 E, NVE, E,	 SE, NVE, NVE};
	inline constexpr std::array<TypeEffectiveness, MAX_TYPES> ROCK_TYPE_MATCHUP{NOT_DEFINED, E, SE, E, E, E, SE, NVE, E, NVE,
																				SE,			 E, SE, E, E, E, E,	 NVE, E};
	inline constexpr std::array<TypeEffectiveness, MAX_TYPES> GHOST_TYPE_MATCHUP{NOT_DEFINED, NE, E, E, E,	E, E,	E, E, E,
																				 E,			  SE, E, E, SE, E, NVE, E, E};
	inline constexpr std::array<TypeEffectiveness, MAX_TYPES> DRAGON_TYPE_MATCHUP{NOT_DEFINED, E, E, E, E, E,  E, E,   E, E,
																				  E,		   E, E, E, E, SE, E, NVE, NE};
	inline constexpr std::array<TypeEffectiveness, MAX_TYPES> DARK_TYPE_MATCHUP{NOT_DEFINED, E,	 E, E, E,  E, E,   NVE, E,	E,
																				E,			 SE, E, E, SE, E, NVE, E,	NVE};
	inline constexpr std::array<TypeEffectiveness, MAX_TYPES> STEEL_TYPE_MATCHUP{NOT_DEFINED, E, NVE, NVE, NVE, E, SE, E,	E, E,
																				 E,			  E, E,	  SE,  E,	E, E,  NVE, SE};
	inline constexpr std::array<TypeEffectiveness, MAX_TYPES> FAIRY_TYPE_MATCHUP{NOT_DEFINED, E, NVE, E, E, E,	E,	SE,	 NVE, E,
																				 E,			  E, E,	  E, E, SE, SE, NVE, E};

	// NOLINTEND(readability-trailing-comma)

	inline constexpr std::string_view TYPE_NAME_NONE{"None"};
	inline constexpr std::string_view TYPE_NAME_NORMAL{"Normal"};
	inline constexpr std::string_view TYPE_NAME_FIGHTING{"Fighting"};
	inline constexpr std::string_view TYPE_NAME_FLYING{"Flying"};
	inline constexpr std::string_view TYPE_NAME_POISON{"Poison"};
	inline constexpr std::string_view TYPE_NAME_GROUND{"Ground"};
	inline constexpr std::string_view TYPE_NAME_ROCK{"Rock"};
	inline constexpr std::string_view TYPE_NAME_BUG{"Bug"};
	inline constexpr std::string_view TYPE_NAME_GHOST{"Ghost"};
	inline constexpr std::string_view TYPE_NAME_STEEL{"Steel"};
	inline constexpr std::string_view TYPE_NAME_FIRE{"Fire"};
	inline constexpr std::string_view TYPE_NAME_WATER{"Water"};
	inline constexpr std::string_view TYPE_NAME_GRASS{"Grass"};
	inline constexpr std::string_view TYPE_NAME_ELECTRIC{"Electric"};
	inline constexpr std::string_view TYPE_NAME_PSYCHIC{"Psychic"};
	inline constexpr std::string_view TYPE_NAME_ICE{"Ice"};
	inline constexpr std::string_view TYPE_NAME_DRAGON{"Dragon"};
	inline constexpr std::string_view TYPE_NAME_DARK{"Dark"};
	inline constexpr std::string_view TYPE_NAME_FAIRY{"Fairy"};
	inline constexpr std::string_view TYPE_NAME_STELLAR{"Stellar"};
} // namespace PocketCore::Type

#endif
