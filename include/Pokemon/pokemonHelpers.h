/*! @file pokemonHelpers.h
	@brief Houses free functions that aide in handling pokemon.
	@date 09/12/2026
	@since 0.12.28
	@version 0.12.28
	@author Matthew Moore
*/

#ifndef INCLUDE_POKEMON_POKEMON_HELPERS_H
#define INCLUDE_POKEMON_POKEMON_HELPERS_H

#include "Registry/registryProvider.h"

#include "pokemon.h"

namespace PocketCore::Pokemon
{
	using PocketCore::Registry::RegistryProvider;
	/*! @brief Writes a Pokemon with stable identifier names resolved from runtime registries.
		@details Ability, item, type, status, and move identifiers are printed with their registered names. Missing registry entries are
		   printed as `<unregistered>`.
		@param[in,out] outStream The stream receiving the formatted Pokemon state.
		@param[in] pokemon The Pokemon whose state is printed.
		@param[in] registryProvider The registries used to resolve stable identifier names. Its registry pointers may be nullptr.
		@return The supplied stream after writing the complete representation.
		@since 0.11.2
		@version 0.12.28
	*/
	std::ostream &printPokemonWithNames(std::ostream &outStream, const Pokemon &pokemon, const RegistryProvider &registryProvider);
} // namespace PocketCore::Pokemon

#endif
