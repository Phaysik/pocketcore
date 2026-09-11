/*! @file constants.h
	@brief Contains constexpr assert message strings for the pokemon registry.
	@date 09/11/2026
	@since 0.11.6
	@version 0.12.23
	@author Matthew Moore
*/

#ifndef INCLUDE_POKEMON_CONSTANTS_H
#define INCLUDE_POKEMON_CONSTANTS_H

#include <string_view>

#include "Pokemon/pokemonMeta.h"

namespace PocketCore::Pokemon
{
	inline constexpr std::string_view POKEMON_NAME_NONE{"None"};
	inline constexpr std::string_view POKEMON_NAME_BULBASAUR{"Bulbasaur"};
	inline constexpr PokemonStats BULBASAUR_BASE_STATS{
		.mMaxHealth = 45,
		.mAttack = 49,
		.mDefense = 49,
		.mSpAttack = 65,
		.mSpDefense = 65,
		.mSpeed = 45,
	};

	inline constexpr std::string_view POKEMON_NAME_IVYSAUR{"Ivysaur"};
	inline constexpr PokemonStats IVYSAUR_BASE_STATS{
		.mMaxHealth = 60,
		.mAttack = 62,
		.mDefense = 63,
		.mSpAttack = 80,
		.mSpDefense = 80,
		.mSpeed = 60,
	};

	inline constexpr std::string_view POKEMON_NAME_VENUSAUR{"Venusaur"};
	inline constexpr PokemonStats VENUSAUR_BASE_STATS{
		.mMaxHealth = 80,
		.mAttack = 82,
		.mDefense = 83,
		.mSpAttack = 100,
		.mSpDefense = 100,
		.mSpeed = 80,
	};

	inline constexpr std::string_view POKEMON_NAME_CHARMANDER{"Charmander"};
	inline constexpr PokemonStats CHARMANDER_BASE_STATS{
		.mMaxHealth = 39,
		.mAttack = 52,
		.mDefense = 43,
		.mSpAttack = 60,
		.mSpDefense = 50,
		.mSpeed = 65,
	};

	inline constexpr std::string_view POKEMON_NAME_CHARMELEON{"Charmeleon"};
	inline constexpr PokemonStats CHARMELEON_BASE_STATS{
		.mMaxHealth = 58,
		.mAttack = 64,
		.mDefense = 58,
		.mSpAttack = 80,
		.mSpDefense = 65,
		.mSpeed = 80,
	};

	inline constexpr std::string_view POKEMON_NAME_CHARIZARD{"Charizard"};
	inline constexpr PokemonStats CHARIZARD_BASE_STATS{
		.mMaxHealth = 78,
		.mAttack = 84,
		.mDefense = 78,
		.mSpAttack = 109,
		.mSpDefense = 85,
		.mSpeed = 100,
	};

	inline constexpr std::string_view POKEMON_NAME_SQUIRTLE{"Squirtle"};
	inline constexpr PokemonStats SQUIRTLE_BASE_STATS{
		.mMaxHealth = 44,
		.mAttack = 48,
		.mDefense = 65,
		.mSpAttack = 50,
		.mSpDefense = 64,
		.mSpeed = 43,
	};

	inline constexpr std::string_view POKEMON_NAME_WARTORTLE{"Wartortle"};
	inline constexpr PokemonStats WARTORTLE_BASE_STATS{
		.mMaxHealth = 59,
		.mAttack = 63,
		.mDefense = 80,
		.mSpAttack = 65,
		.mSpDefense = 80,
		.mSpeed = 58,
	};

	inline constexpr std::string_view POKEMON_NAME_BLASTOISE{"Blastoise"};
	inline constexpr PokemonStats BLASTOISE_BASE_STATS{
		.mMaxHealth = 79,
		.mAttack = 83,
		.mDefense = 100,
		.mSpAttack = 85,
		.mSpDefense = 105,
		.mSpeed = 78,
	};
} // namespace PocketCore::Pokemon

#endif
