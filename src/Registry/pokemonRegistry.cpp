/*! @file pokemonRegistry.cpp
	@brief Contains the pokemon registry implementation
	@date 10/13/2026
	@since 0.12.28
	@version 0.12.51
	@author Matthew Moore
*/

#include "Registry/pokemonRegistry.h"

#include <array>
#include <cstddef>
#include <expected>
#include <span>
#include <type_traits>
#include <variant>

#include "Ability/abilityID.h"
#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Item/itemID.h"
#include "Learnset/learnsetMeta.h"
#include "Location/locationID.h"
#include "Move/moveID.h"
#include "Nature/natureID.h"
#include "Pokemon/pokemon.h"
#include "Pokemon/pokemonEncounter.h"
#include "Pokemon/pokemonHelpers.h"
#include "Pokemon/pokemonID.h"
#include "Pokemon/pokemonMeta.h"
#include "Pokemon/pokemonStats.h"
#include "Utility/random.h"
#include "Validation/Ability/abilityError.h"
#include "Validation/Item/itemError.h"
#include "Validation/Move/moveError.h"
#include "Validation/Nature/natureError.h"
#include "Validation/Pokemon/pokemonError.h"
#include "Validation/Registry/registryError.h"

namespace PocketCore::Registry::Pokemon
{
	using PocketCore::Ability::AbilityID;
	using PocketCore::Configuration::MAX_ABILITIES_PER_POKEMON;
	using PocketCore::Configuration::MAX_ITEMS_PER_POKEMON;
	using PocketCore::Configuration::MAX_MOVES_PER_POKEMON;
	using PocketCore::Configuration::MAX_NATURES_PER_POKEMON;
	using PocketCore::Core::us;
	using PocketCore::Item::ItemID;
	using PocketCore::Learnset::LearnsetMeta;
	using PocketCore::Location::LocationID;
	using PocketCore::Move::MoveID;
	using PocketCore::Nature::NatureID;
	using PocketCore::Pokemon::getValidPokemonAbilities;
	using PocketCore::Pokemon::getValidPokemonEVs;
	using PocketCore::Pokemon::getValidPokemonItems;
	using PocketCore::Pokemon::getValidPokemonIVs;
	using PocketCore::Pokemon::getValidPokemonMoves;
	using PocketCore::Pokemon::getValidPokemonNatures;
	using PocketCore::Pokemon::Pokemon;
	using PocketCore::Pokemon::POKEMON_STAT_COUNT;
	using PocketCore::Pokemon::PokemonEncounter;
	using PocketCore::Pokemon::PokemonID;
	using PocketCore::Pokemon::PokemonMeta;
	using PocketCore::Pokemon::SetLevelEncounter;
	using PocketCore::Utility::Random;
	using PocketCore::Validation::Ability::AbilityError;
	using PocketCore::Validation::Item::ItemError;
	using PocketCore::Validation::Move::MoveError;
	using PocketCore::Validation::Nature::NatureError;
	using PocketCore::Validation::Pokemon::PokemonError;
	using PocketCore::Validation::Registry::RegistryError;

	ATTR_NODISCARD std::expected<Pokemon, PokemonInstantiationError> PokemonRegistry::instantiate(
		const PokemonID pokemonID, const std::size_t encounterIndex, PokemonInstantiationDependencies *dependencyRegistries,
		const std::span<const us> ivs, const std::span<const us> evs, const std::span<const NatureID> natureIDs,
		const std::span<const AbilityID> abilityIDs, const std::span<const ItemID> itemIDs, const std::span<const MoveID> moveIDs) const
	{
		if (dependencyRegistries == nullptr || dependencyRegistries->mAbilityRegistry == nullptr
			|| dependencyRegistries->mItemRegistry == nullptr || dependencyRegistries->mMoveRegistry == nullptr
			|| dependencyRegistries->mNatureRegistry == nullptr || dependencyRegistries->mLearnsetRegistry == nullptr
			|| dependencyRegistries->mLocationRegistry == nullptr)
		{
			return std::unexpected{RegistryError::MissingRegistry};
		}

		const PokemonMeta *pokemonMeta{getPokemonMetadata(pokemonID)};

		if (pokemonMeta == nullptr)
		{
			return std::unexpected{RegistryError::PokemonNotFound};
		}

		const LearnsetMeta *learnsetMeta{dependencyRegistries->mLearnsetRegistry->getLearnsetMetadata(pokemonMeta->mLearnsetID)};

		if (learnsetMeta == nullptr)
		{
			return std::unexpected{RegistryError::LearnsetNotFound};
		}

		if (encounterIndex > pokemonMeta->mEncounters.size())
		{
			return std::unexpected{RegistryError::LocationNotFound};
		}

		const PokemonEncounter encounterData{pokemonMeta->mEncounters.at(encounterIndex)};

		const us pokemonEncounterLevel{
			std::visit(
				[](const auto &policy) {
					using Policy = std::decay_t<decltype(policy)>;

					if constexpr (std::is_same_v<Policy, SetLevelEncounter>)
					{
						return policy.mLevel;
					}
					else
					{
						return Random::get(policy.mMinLevel, policy.mMaxLevel);
					}
				},
				encounterData.mLevel),
		};

		std::array<NatureID, MAX_NATURES_PER_POKEMON> validNatureIDs{};
		std::array<AbilityID, MAX_ABILITIES_PER_POKEMON> validAbilityIDs{};
		std::array<ItemID, MAX_ITEMS_PER_POKEMON> validItemIDs{};
		std::array<MoveID, MAX_MOVES_PER_POKEMON> validMoveIDs{};
		std::array<us, POKEMON_STAT_COUNT> validIVs{};
		std::array<us, POKEMON_STAT_COUNT> validEVs{};

		if (const std::expected<void, NatureError> error{
				getValidPokemonNatures(natureIDs, validNatureIDs, *dependencyRegistries->mNatureRegistry),
			};
			!error.has_value())
		{
			return std::unexpected{error.error()};
		}

		if (const std::expected<void, AbilityError> error{
				getValidPokemonAbilities(abilityIDs, validAbilityIDs, *dependencyRegistries->mAbilityRegistry),
			};
			!error.has_value())
		{
			return std::unexpected{error.error()};
		}

		if (const std::expected<void, ItemError> error{getValidPokemonItems(itemIDs, validItemIDs, *dependencyRegistries->mItemRegistry)};
			!error.has_value())
		{
			return std::unexpected{error.error()};
		}

		if (const std::expected<void, MoveError> error{
				getValidPokemonMoves(moveIDs, validMoveIDs, *dependencyRegistries->mMoveRegistry, *learnsetMeta, encounterData,
									 pokemonEncounterLevel),
			};
			!error.has_value())
		{
			return std::unexpected{error.error()};
		}

		if (const std::expected<void, PokemonError> error{getValidPokemonIVs(ivs, validIVs)}; !error.has_value())
		{
			return std::unexpected{error.error()};
		}

		if (const std::expected<void, PokemonError> error{getValidPokemonEVs(evs, validEVs)}; !error.has_value())
		{
			return std::unexpected{error.error()};
		}

		return Pokemon{
			pokemonMeta->mPokemonID,
			pokemonMeta->mLearnsetID,
			pokemonMeta->mName,
			validMoveIDs,
			{},
			{},
			pokemonMeta->mBaseStats,
			pokemonEncounterLevel,
			validAbilityIDs,
			validItemIDs,
			pokemonMeta->mTypeIDs,
			validNatureIDs,
			*dependencyRegistries->mNatureRegistry,
			validIVs,
			validEVs,
		};
	}
} // namespace PocketCore::Registry::Pokemon
