/*! @file builtinPokemonID.h
	@brief Defines identifiers for pokemons compiled into PocketCore.
	@date 08/31/2026
	@since 0.11.6
	@version 0.12.13
	@author Matthew Moore
*/

#ifndef INCLUDE_POKEMON_BUILTIN_POKEMON_ID_H
#define INCLUDE_POKEMON_BUILTIN_POKEMON_ID_H

#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Pokemon/pokemonID.h"

namespace PocketCore::Pokemon
{
	using PocketCore::Core::ub;

	/*! @enum BuiltinPokemonID
		@showenumvalues
		@brief Names the pokemons provided by PocketCore itself.
		@details This closed enum is only a catalog of built-in pokemons. Runtime state and user-facing APIs use the open @ref PokemonID
	   type.
		@note All enum values must be handled exhaustively when registering built-in metadata.
		@date 08/31/2026
		@since 0.11.6
		@version 0.12.13
		@author Matthew Moore
	*/
	enum class BuiltinPokemonID : ub
	{
		None,
		Bulbasaur,
		Ivysaur,
		Venusaur,
		Charmander,
		Charmeleon,
		Charizard,
		Squirtle,
		Wartortle,
		Blastoise,
		FinalPokemon,
	};

	/*! @brief Converts a built-in catalog value to its stable registry identifier.
		@param[in] builtinPokemonID The built-in pokemon to convert.
		@return The corresponding open pokemon identifier.
		@since 0.11.6
		@version 0.12.13
	*/
	ATTR_NODISCARD constexpr PokemonID toPokemonID(const BuiltinPokemonID builtinPokemonID) noexcept
	{
		return PokemonID{static_cast<ub>(builtinPokemonID)};
	}
} // namespace PocketCore::Pokemon

#endif
