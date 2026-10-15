/*! @file pokemonRegistryConfiguration.cpp
	@brief Defines validated user customization operations for the pokemon registry.
	@date 10/13/2026
	@since 0.12.0
	@version 0.12.51
	@author Matthew Moore
*/

#include "Configuration/pokemonRegistryConfiguration.h"

#include <expected>
#include <span>
#include <string_view>

#include "Core/attributeMacros.h"
#include "Pokemon/pokemonID.h"
#include "Pokemon/pokemonMeta.h"
#include "Validation/Registry/registryError.h"

namespace PocketCore::Configuration
{
	using PocketCore::Pokemon::PokemonID;
	using PocketCore::Pokemon::PokemonMeta;
	using PocketCore::Validation::Registry::RegistryError;

	ATTR_NODISCARD std::expected<PokemonID, RegistryError> PokemonRegistryConfiguration::addPokemon(const PokemonMeta &pokemonMeta)
	{
		return addMetadata(pokemonMeta);
	}

	ATTR_NODISCARD std::expected<void, RegistryError> PokemonRegistryConfiguration::addPokemons(
		const std::span<const PokemonMeta> pokemonMetas)
	{
		return addMetadataBatch(pokemonMetas, [](const PokemonMeta &definition) { return PokemonMeta{definition}; });
	}

	ATTR_NODISCARD std::expected<void, RegistryError> PokemonRegistryConfiguration::renamePokemon(const std::string_view oldName,
																								  const std::string_view newName)
	{
		return renameMetadata(oldName, newName);
	}

	ATTR_NODISCARD std::expected<void, RegistryError> PokemonRegistryConfiguration::updatePokemon(const std::string_view pokemonName,
																								  const PokemonMeta &pokemonMeta)
	{
		return mutateMetadata(pokemonName, "updatePokemon", [&pokemonMeta](PokemonMeta &metadata) { metadata = pokemonMeta; });
	}

	ATTR_NODISCARD std::expected<void, RegistryError> PokemonRegistryConfiguration::updatePokemon(const PokemonID pokemonID,
																								  const PokemonMeta &pokemonMeta)
	{
		return mutateMetadata(pokemonID, "updatePokemon", [&pokemonMeta](PokemonMeta &metadata) { metadata = pokemonMeta; });
	}

	ATTR_NODISCARD std::expected<PokemonID, RegistryError> PokemonRegistryConfiguration::removePokemon(const std::string_view pokemonName)
	{
		return removeMetadata(pokemonName);
	}

	ATTR_NODISCARD std::expected<PokemonID, RegistryError> PokemonRegistryConfiguration::removePokemon(const PokemonID pokemonID)
	{
		return removeMetadata(pokemonID);
	}
} // namespace PocketCore::Configuration
