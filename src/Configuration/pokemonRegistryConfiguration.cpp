/*! @file pokemonRegistryConfiguration.cpp
	@brief Defines validated user customization operations for the pokemon registry.
	@date 08/30/2026
	@since 0.12.0
	@version 0.12.12
	@author Matthew Moore
*/

#include "Configuration/pokemonRegistryConfiguration.h"

#include <expected>
#include <span>
#include <string_view>

#include "Core/attributeMacros.h"
#include "Pokemon/pokemonID.h"
#include "Pokemon/pokemonMeta.h"
#include "Registry/registryError.h"

namespace PocketCore::Configuration
{
	using PocketCore::Pokemon::PokemonID;
	using PocketCore::Pokemon::PokemonMeta;
	using PocketCore::Registry::RegistryErrorInfo;

	ATTR_NODISCARD std::expected<PokemonID, RegistryErrorInfo> PokemonRegistryConfiguration::addPokemon(const PokemonMeta &pokemonMeta)
	{
		return addMetadata(pokemonMeta);
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> PokemonRegistryConfiguration::addPokemons(
		const std::span<const PokemonMeta> &pokemonMetas)
	{
		return addMetadataBatch(pokemonMetas, [](const PokemonMeta &definition) { return PokemonMeta{definition}; });
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> PokemonRegistryConfiguration::renamePokemon(const std::string_view &oldName,
																									  const std::string_view &newName)
	{
		return renameMetadata(oldName, newName);
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> PokemonRegistryConfiguration::updatePokemon(const std::string_view &pokemonName,
																									  const PokemonMeta &pokemonMeta)
	{
		return mutateMetadata(pokemonName, "updatePokemon", [&pokemonMeta](PokemonMeta &metadata) { metadata = pokemonMeta; });
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> PokemonRegistryConfiguration::updatePokemon(const PokemonID pokemonID,
																									  const PokemonMeta &pokemonMeta)
	{
		return mutateMetadata(pokemonID, "updatePokemon", [&pokemonMeta](PokemonMeta &metadata) { metadata = pokemonMeta; });
	}

	ATTR_NODISCARD std::expected<PokemonID, RegistryErrorInfo> PokemonRegistryConfiguration::removePokemon(
		const std::string_view &pokemonName)
	{
		return removeMetadata(pokemonName);
	}

	ATTR_NODISCARD std::expected<PokemonID, RegistryErrorInfo> PokemonRegistryConfiguration::removePokemon(const PokemonID pokemonID)
	{
		return removeMetadata(pokemonID);
	}
} // namespace PocketCore::Configuration
