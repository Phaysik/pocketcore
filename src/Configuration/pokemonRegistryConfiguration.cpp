/*! @file pokemonRegistryConfiguration.cpp
	@brief Defines validated user customization operations for the pokemon registry.
	@date 09/11/2026
	@since 0.12.0
	@version 0.12.26
	@author Matthew Moore
*/

#include "Configuration/pokemonRegistryConfiguration.h"

#include <array>
#include <expected>
#include <span>
#include <string_view>

#include "Ability/abilityID.h"
#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Item/itemID.h"
#include "Move/moveID.h"
#include "Nature/natureID.h"
#include "Pokemon/pokemon.h"
#include "Pokemon/pokemonID.h"
#include "Pokemon/pokemonMeta.h"
#include "Registry/registryError.h"

namespace PocketCore::Configuration
{
	using PocketCore::Ability::AbilityID;
	using PocketCore::Core::us;
	using PocketCore::Item::ItemID;
	using PocketCore::Move::MoveID;
	using PocketCore::Nature::NatureID;
	using PocketCore::Pokemon::Pokemon;
	using PocketCore::Pokemon::POKEMON_STAT_COUNT;
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

	ATTR_NODISCARD std::expected<Pokemon, RegistryErrorInfo> PokemonRegistryConfiguration::instantiate(
		const PokemonID pokemonID, const PokemonInstantiationDependencies *dependencyRegistries,
		const std::array<us, POKEMON_STAT_COUNT> *ivs, const std::array<us, POKEMON_STAT_COUNT> &evs,
		const std::array<NatureID, MAX_NATURES_PER_POKEMON> *natureIDs, const std::array<AbilityID, MAX_ABILITIES_PER_POKEMON> *abilityIDs,
		const std::array<ItemID, MAX_ITEMS_PER_POKEMON> *itemIDs, const std::array<MoveID, MAX_MOVES_PER_POKEMON> *moveIDs) const
	{
		if (dependencyRegistries->abilityRegistry == nullptr || dependencyRegistries->itemRegistry == nullptr
			|| dependencyRegistries->moveRegistry == nullptr || dependencyRegistries->natureRegistry == nullptr)
		{
			return std::unexpected{RegistryErrorInfo{
				RegistryError::MaxCapacity, {}, "PokemonRegistryConfiguration::instantiate: missing registry dependency"}};
		}

		const PokemonMeta *pokemonMeta{getPokemonMetadata(pokemonID)};

		if (pokemonMeta == nullptr)
		{
			return std::unexpected{RegistryErrorInfo{
				RegistryError::PokemonNotFound, {}, "PokemonRegistryConfiguration::instantiate: missing pokemon metadata"}};
		}

		if (const std::expected<void, RegistryErrorInfo> error{validateNatureMetadata(natureIDs, *dependencyRegistries)})
		{
			return std::unexpected{error.error()};
		}

		if (const std::expected<void, RegistryErrorInfo> error{validateAbilityMetadata(abilityIDs, *dependencyRegistries)})
		{
			return std::unexpected{error.error()};
		}

		if (const std::expected<void, RegistryErrorInfo> error{validateItemMetadata(itemIDs, *dependencyRegistries)})
		{
			return std::unexpected{error.error()};
		}

		if (const std::expected<void, RegistryErrorInfo> error{validateMoveMetadata(moveIDs, *dependencyRegistries)})
		{
			return std::unexpected{error.error()};
		}

		return Pokemon{
			pokemonID,
			pokemonMeta->mName,
			*moveIDs,
			{},
			{},
			pokemonMeta->mBaseStats,
			pokemonMeta->mLevel,
			*abilityIDs,
			*itemIDs,
			pokemonMeta->mTypeIDs,
			*natureIDs,
			{},
			*ivs,
			evs,
		};
	}
} // namespace PocketCore::Configuration
