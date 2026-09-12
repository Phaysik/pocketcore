/*! @file pokemonRegistryConfiguration.cpp
	@brief Defines validated user customization operations for the pokemon registry.
	@date 09/11/2026
	@since 0.12.0
	@version 0.12.24
	@author Matthew Moore
*/

#include "Configuration/pokemonRegistryConfiguration.h"

#include <algorithm>
#include <array>
#include <expected>
#include <span>
#include <string_view>

#include "Ability/abilityID.h"
#include "Ability/abilityMeta.h"
#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Item/itemID.h"
#include "Item/itemMeta.h"
#include "Move/moveID.h"
#include "Move/moveMeta.h"
#include "Nature/natureID.h"
#include "Nature/natureMeta.h"
#include "Pokemon/pokemon.h"
#include "Pokemon/pokemonID.h"
#include "Pokemon/pokemonMeta.h"
#include "Registry/registryError.h"

namespace PocketCore::Configuration
{
	using PocketCore::Ability::AbilityID;
	using PocketCore::Ability::AbilityMeta;
	using PocketCore::Core::us;
	using PocketCore::Item::ItemID;
	using PocketCore::Item::ItemMeta;
	using PocketCore::Move::MoveID;
	using PocketCore::Move::MoveMeta;
	using PocketCore::Nature::NatureID;
	using PocketCore::Nature::NatureMeta;
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

		const auto missingMetadata
			= [](const auto *metas) { return std::ranges::any_of(*metas, [](const auto *meta) { return meta == nullptr; }); };

		std::array<const NatureMeta *, MAX_NATURES_PER_POKEMON> natureMetas{};
		std::ranges::transform(*natureIDs, natureMetas.begin(), [&dependencyRegistries](const NatureID natureID) {
			return dependencyRegistries->natureRegistry->getNatureMetadata(natureID);
		});

		if (missingMetadata(&natureMetas))
		{
			return std::unexpected{
				RegistryErrorInfo{RegistryError::NatureNotFound, {}, "PokemonRegistryConfiguration::instantiate: missing nature metadata"}};
		}

		std::array<const AbilityMeta *, MAX_ABILITIES_PER_POKEMON> abilityMetas{};
		std::ranges::transform(*abilityIDs, abilityMetas.begin(), [&dependencyRegistries](const AbilityID abilityID) {
			return dependencyRegistries->abilityRegistry->getAbilityMetadata(abilityID);
		});

		if (missingMetadata(&abilityMetas))
		{
			return std::unexpected{RegistryErrorInfo{
				RegistryError::AbilityNotFound, {}, "PokemonRegistryConfiguration::instantiate: missing ability metadata"}};
		}

		std::array<const ItemMeta *, MAX_ITEMS_PER_POKEMON> itemMetas{};
		std::ranges::transform(*itemIDs, itemMetas.begin(), [&dependencyRegistries](const ItemID itemID) {
			return dependencyRegistries->itemRegistry->getItemMetadata(itemID);
		});

		if (missingMetadata(&itemMetas))
		{
			return std::unexpected{
				RegistryErrorInfo{RegistryError::ItemNotFound, {}, "PokemonRegistryConfiguration::instantiate: missing item metadata"}};
		}

		std::array<const MoveMeta *, MAX_MOVES_PER_POKEMON> moveMetas{};
		std::ranges::transform(*moveIDs, moveMetas.begin(), [&dependencyRegistries](const MoveID moveID) {
			return dependencyRegistries->moveRegistry->getMoveMetadata(moveID);
		});

		if (missingMetadata(&moveMetas))
		{
			return std::unexpected{
				RegistryErrorInfo{RegistryError::MoveNotFound, {}, "PokemonRegistryConfiguration::instantiate: missing move metadata"}};
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
