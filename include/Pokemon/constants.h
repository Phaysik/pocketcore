/*! @file constants.h
	@brief Contains constexpr assert message strings for the pokemon registry.
	@date 09/11/2026
	@since 0.11.6
	@version 0.12.22
	@author Matthew Moore
*/

#ifndef INCLUDE_POKEMON_CONSTANTS_H
#define INCLUDE_POKEMON_CONSTANTS_H

#include <string_view>

#include "Pokemon/pokemonMeta.h"

namespace PocketCore::Pokemon
{
	using PocketCore::Pokemon::BasePokemonStats;

	inline constexpr std::string_view POKEMON_NAME_NONE{"None"};
	inline constexpr std::string_view POKEMON_NAME_BULBASAUR{"Bulbasaur"};
	inline constexpr BasePokemonStats BULBASAUR_BASE_STATS{
		.mBaseMaxHealth = 45,
		.mBaseAttack = 49,
		.mBaseDefense = 49,
		.mBaseSpAttack = 65,
		.mBaseSpDefense = 65,
		.mBaseSpeed = 45,
	};

	inline constexpr std::string_view POKEMON_NAME_IVYSAUR{"Ivysaur"};
	inline constexpr BasePokemonStats IVYSAUR_BASE_STATS{
		.mBaseMaxHealth = 60,
		.mBaseAttack = 62,
		.mBaseDefense = 63,
		.mBaseSpAttack = 80,
		.mBaseSpDefense = 80,
		.mBaseSpeed = 60,
	};
	
	inline constexpr std::string_view POKEMON_NAME_VENUSAUR{"Venusaur"};
	inline constexpr BasePokemonStats VENUSAUR_BASE_STATS{
		.mBaseMaxHealth = 80,
		.mBaseAttack = 82,
		.mBaseDefense = 83,
		.mBaseSpAttack = 100,
		.mBaseSpDefense = 100,
		.mBaseSpeed = 80,
	};
	
	inline constexpr std::string_view POKEMON_NAME_CHARMANDER{"Charmander"};
	inline constexpr BasePokemonStats CHARMANDER_BASE_STATS{
		.mBaseMaxHealth = 39,
		.mBaseAttack = 52,
		.mBaseDefense = 43,
		.mBaseSpAttack = 60,
		.mBaseSpDefense = 50,
		.mBaseSpeed = 65,
	};
	
	inline constexpr std::string_view POKEMON_NAME_CHARMELEON{"Charmeleon"};
	inline constexpr BasePokemonStats CHARMELEON_BASE_STATS{
		.mBaseMaxHealth = 58,
		.mBaseAttack = 64,
		.mBaseDefense = 58,
		.mBaseSpAttack = 80,
		.mBaseSpDefense = 65,
		.mBaseSpeed = 80,
	};
	
	inline constexpr std::string_view POKEMON_NAME_CHARIZARD{"Charizard"};
	inline constexpr BasePokemonStats CHARIZARD_BASE_STATS{
		.mBaseMaxHealth = 78,
		.mBaseAttack = 84,
		.mBaseDefense = 78,
		.mBaseSpAttack = 109,
		.mBaseSpDefense = 85,
		.mBaseSpeed = 100,
	};
	
	inline constexpr std::string_view POKEMON_NAME_SQUIRTLE{"Squirtle"};
	inline constexpr BasePokemonStats SQUIRTLE_BASE_STATS{
		.mBaseMaxHealth = 44,
		.mBaseAttack = 48,
		.mBaseDefense = 65,
		.mBaseSpAttack = 50,
		.mBaseSpDefense = 64,
		.mBaseSpeed = 43,
	};
	
	inline constexpr std::string_view POKEMON_NAME_WARTORTLE{"Wartortle"};
	inline constexpr BasePokemonStats WARTORTLE_BASE_STATS{
		.mBaseMaxHealth = 59,
		.mBaseAttack = 63,
		.mBaseDefense = 80,
		.mBaseSpAttack = 65,
		.mBaseSpDefense = 80,
		.mBaseSpeed = 58,
	};
	
	inline constexpr std::string_view POKEMON_NAME_BLASTOISE{"Blastoise"};
	inline constexpr BasePokemonStats BLASTOISE_BASE_STATS{
		.mBaseMaxHealth = 79,
		.mBaseAttack = 83,
		.mBaseDefense = 100,
		.mBaseSpAttack = 85,
		.mBaseSpDefense = 105,
		.mBaseSpeed = 78,
	};
} // namespace PocketCore::Pokemon

#endif
