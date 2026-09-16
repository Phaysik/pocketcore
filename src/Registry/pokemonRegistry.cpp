/*! @file pokemonRegistry.cpp
	@brief Contains the pokemon registry implementation
	@date 09/12/2026
	@since 0.12.28
	@version 0.12.29
	@author Matthew Moore
*/

#include "Registry/pokemonRegistry.h"

#include <expected>

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

namespace PocketCore::Registry::Pokemon
{
	using PocketCore::Ability::AbilityID;
	using PocketCore::Configuration::MAX_ABILITIES_PER_POKEMON;
	using PocketCore::Configuration::MAX_ITEMS_PER_POKEMON;
	using PocketCore::Configuration::MAX_MOVES_PER_POKEMON;
	using PocketCore::Configuration::MAX_NATURES_PER_POKEMON;
	using PocketCore::Core::us;
	using PocketCore::Item::ItemID;
	using PocketCore::Move::MoveID;
	using PocketCore::Nature::NatureID;
	using PocketCore::Pokemon::Pokemon;
	using PocketCore::Pokemon::POKEMON_STAT_COUNT;
	using PocketCore::Pokemon::PokemonID;
	using PocketCore::Pokemon::PokemonMeta;
	using PocketCore::Registry::RegistryErrorInfo;

	ATTR_NODISCARD std::expected<Pokemon, RegistryErrorInfo> PokemonRegistry::instantiate(
		const PokemonID pokemonID, const PokemonInstantiationDependencies *dependencyRegistries,
		const std::array<us, POKEMON_STAT_COUNT> *ivs, const std::array<us, POKEMON_STAT_COUNT> &evs,
		const std::array<NatureID, MAX_NATURES_PER_POKEMON> *natureIDs, const std::array<AbilityID, MAX_ABILITIES_PER_POKEMON> *abilityIDs,
		const std::array<ItemID, MAX_ITEMS_PER_POKEMON> *itemIDs, const std::array<MoveID, MAX_MOVES_PER_POKEMON> *moveIDs) const
	{
		if (dependencyRegistries == nullptr || dependencyRegistries->abilityRegistry == nullptr
			|| dependencyRegistries->itemRegistry == nullptr || dependencyRegistries->moveRegistry == nullptr
			|| dependencyRegistries->natureRegistry == nullptr)
		{
			return std::unexpected{RegistryErrorInfo{
				RegistryError::MaxCapacity, {}, "PokemonRegistry::instantiate: missing registry dependency"}};
		}

		const PokemonMeta *pokemonMeta{getPokemonMetadata(pokemonID)};

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
} // namespace PocketCore::Registry::Pokemon
