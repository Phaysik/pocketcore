/*! @file pokemonID.h
	@brief Contains the pokemon status
	@date 07/24/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_POKEMON_POKEMONID_H
#define INCLUDE_POKEMON_POKEMONID_H

#include "ID/idInterface.h"

namespace PocketCore::Pokemon
{
	namespace Detail
	{
		/*! @brief Distinguishes pokemon identifiers from all other stable identifier domains. */
		struct PokemonIDTag;
	} // namespace Detail

	/*! @typedef PokemonID
		@brief A strongly typed stable identifier for any registered pokemon.
		@details Values are assigned by the pokemon registry. Unlike @ref BuiltinPokemonID, this type is open and can represent user-defined
	   abilities without extending an enum. Its tag prevents comparison or conversion with identifiers from other registry domains.
	*/
	using PokemonID = PocketCore::ID::IDInterface<Detail::PokemonIDTag, 0>;

	/*! @brief The stable identifier representing no pokemon. */
	inline constexpr PokemonID NO_POKEMON_ID{};
} // namespace PocketCore::Pokemon

#endif
