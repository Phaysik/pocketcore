/*! @file pokemonRegistry.cpp
	@brief Contains the pokemon registry implementation
	@date 11/09/2026
	@since 0.12.28
	@version 0.12.54
	@author Matthew Moore
*/

#include "Registry/pokemonRegistry.h"

#include <array>
#include <cstddef>
#include <expected>
#include <optional>
#include <span>
#include <type_traits>
#include <variant>

#include "Ability/abilityID.h"
#include "Battle/battleRandomSources.h"
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
#include "Pokemon/pokemonInstantiationContext.h"
#include "Pokemon/pokemonMeta.h"
#include "Pokemon/pokemonStats.h"
#include "Validation/Ability/abilityError.h"
#include "Validation/Item/itemError.h"
#include "Validation/Move/moveError.h"
#include "Validation/Nature/natureError.h"
#include "Validation/Pokemon/pokemonError.h"
#include "Validation/Registry/registryError.h"

namespace PocketCore::Registry::Pokemon
{
	using PocketCore::Ability::AbilityID;
	using PocketCore::Battle::BattleRandomSources;
	using PocketCore::Configuration::MAX_ABILITIES_PER_POKEMON;
	using PocketCore::Configuration::MAX_ITEMS_PER_POKEMON;
	using PocketCore::Configuration::MAX_MOVES_PER_POKEMON;
	using PocketCore::Configuration::MAX_NATURES_PER_POKEMON;
	using PocketCore::Core::us;
	using PocketCore::Item::ItemID;
	using PocketCore::Learnset::LearnsetMeta;
	using PocketCore::Location::LocationID;
	using PocketCore::Location::NO_LOCATION_ID;
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
	using PocketCore::Pokemon::PokemonAbilityEntry;
	using PocketCore::Pokemon::PokemonEncounter;
	using PocketCore::Pokemon::PokemonGender;
	using PocketCore::Pokemon::PokemonID;
	using PocketCore::Pokemon::PokemonMeta;
	using PocketCore::Pokemon::SetLevelEncounter;
	using PocketCore::Validation::Ability::AbilityError;
	using PocketCore::Validation::Item::ItemError;
	using PocketCore::Validation::Move::MoveError;
	using PocketCore::Validation::Nature::NatureError;
	using PocketCore::Validation::Pokemon::PokemonError;
	using PocketCore::Validation::Registry::RegistryError;

	ATTR_NODISCARD std::expected<Pokemon, PokemonInstantiationError> PokemonRegistry::instantiate(
		const PokemonSpec &pokemonSpec, const PokemonInstantiationDependencies &dependencyRegistries,
		const BattleRandomSources &randomSources) const
	{
		if (dependencyRegistries.mAbilityRegistry == nullptr || dependencyRegistries.mItemRegistry == nullptr
			|| dependencyRegistries.mMoveRegistry == nullptr || dependencyRegistries.mNatureRegistry == nullptr
			|| dependencyRegistries.mLearnsetRegistry == nullptr || dependencyRegistries.mLocationRegistry == nullptr)
		{
			return std::unexpected{RegistryError::MissingRegistry};
		}

		const PokemonMeta *pokemonMeta{getPokemonMetadata(pokemonSpec.mPokemonID)};

		if (pokemonMeta == nullptr)
		{
			return std::unexpected{RegistryError::PokemonNotFound};
		}

		const LearnsetMeta *learnsetMeta{dependencyRegistries.mLearnsetRegistry->getLearnsetMetadata(pokemonMeta->mLearnsetID)};

		if (learnsetMeta == nullptr)
		{
			return std::unexpected{RegistryError::LearnsetNotFound};
		}

		PokemonEncounter encounterData{};

		if (pokemonSpec.mEncounterIndex.has_value())
		{
			const std::size_t index{*pokemonSpec.mEncounterIndex};

			if (index >= pokemonMeta->mEncounters.size() || pokemonMeta->mEncounters.at(index).mLocationID == NO_LOCATION_ID)
			{
				return std::unexpected{RegistryError::EncounterNotFound};
			}

			encounterData = pokemonMeta->mEncounters.at(index);
		}
		else if (!pokemonSpec.mLevel.has_value())
		{
			return std::unexpected{PokemonError::MISSING_LEVEL};
		}

		const us pokemonEncounterLevel{
			pokemonSpec.mLevel.has_value() ? pokemonSpec.mLevel.value()
										   : std::visit(
												 [&randomSources](const auto &policy) {
													 using Policy = std::decay_t<decltype(policy)>;

													 if constexpr (std::is_same_v<Policy, SetLevelEncounter>)
													 {
														 return policy.mLevel;
													 }
													 else
													 {
														 const auto randomLevelValue = [&randomSources](us minimum, us maximum) -> us {
															 return static_cast<us>(randomSources.mIndex.get()(minimum, maximum));
														 };

														 return randomLevelValue(policy.mMinLevel, policy.mMaxLevel);
													 }
												 },
												 encounterData.mLevel),
		};

		std::array<NatureID, MAX_NATURES_PER_POKEMON> validNatureIDs{};
		std::array<PokemonAbilityEntry, MAX_ABILITIES_PER_POKEMON> validAbilityEntries{};
		std::array<ItemID, MAX_ITEMS_PER_POKEMON> validItemIDs{};
		std::array<MoveID, MAX_MOVES_PER_POKEMON> validMoveIDs{};
		std::array<us, POKEMON_STAT_COUNT> validIVs{};
		std::array<us, POKEMON_STAT_COUNT> validEVs{};

		if (const std::expected<void, NatureError> error{
				getValidPokemonNatures(pokemonSpec.mIDContext.natureIDs, validNatureIDs, *dependencyRegistries.mNatureRegistry,
									   randomSources, encounterData),
			};
			!error.has_value())
		{
			return std::unexpected{error.error()};
		}

		if (const std::expected<void, AbilityError> error{
				getValidPokemonAbilities(pokemonSpec.mIDContext.abilityEntries, validAbilityEntries, pokemonMeta->mAbilityPool,
										 *dependencyRegistries.mAbilityRegistry, randomSources, pokemonSpec.mBreedingContext,
										 encounterData),
			};
			!error.has_value())
		{
			return std::unexpected{error.error()};
		}

		if (const std::expected<void, ItemError> error{
				getValidPokemonItems(pokemonSpec.mIDContext.itemIDs, validItemIDs, pokemonMeta->mItemPool,
									 *dependencyRegistries.mItemRegistry, randomSources, encounterData),
			};
			!error.has_value())
		{
			return std::unexpected{error.error()};
		}

		if (const std::expected<void, MoveError> error{
				getValidPokemonMoves(pokemonSpec.mIDContext.moveIDs, validMoveIDs, *dependencyRegistries.mMoveRegistry, *learnsetMeta,
									 encounterData, pokemonEncounterLevel),
			};
			!error.has_value())
		{
			return std::unexpected{error.error()};
		}

		if (const std::expected<void, PokemonError> error{
				getValidPokemonIVs(pokemonSpec.mIVEVContext.ivs, validIVs, randomSources, encounterData),
			};
			!error.has_value())
		{
			return std::unexpected{error.error()};
		}

		if (const std::expected<void, PokemonError> error{getValidPokemonEVs(pokemonSpec.mIVEVContext.evs, validEVs)}; !error.has_value())
		{
			return std::unexpected{error.error()};
		}

		std::array<MoveMeta, MAX_MOVES_PER_POKEMON> moveMetas;

		for (const std::size_t index : std::views::iota(std::size_t{0}, MAX_MOVES_PER_POKEMON))
		{
			const MoveID currentMove{validMoveIDs.at(index)};
			const MoveMeta *meta{dependencyRegistries.mMoveRegistry->getMoveMetadata(currentMove)};
			moveMetas.at(index) = {.mMoveID = currentMove, .mPPMaxAmount = meta->mPPMaxAmount, .mPPDefaultAmount = meta->mPPDefaultAmount};
		};

		const auto getRandomGender{
			[&pokemonMeta, &randomSources] {
				const std::size_t genderIndex{randomSources.mIndex.get()(std::size_t{0}, pokemonMeta->mGenderPool.size() - 1)};

				return pokemonMeta->mGenderPool.at(genderIndex);
			},
		};

		const PokemonGender gender{
			pokemonSpec.mGender.has_value() ? pokemonSpec.mGender.value() : getRandomGender(),
		};

		return Pokemon{
			*pokemonMeta,
			moveMetas,
			pokemonEncounterLevel,
			validAbilityEntries,
			validItemIDs,
			validNatureIDs,
			*dependencyRegistries.mNatureRegistry,
			validIVs,
			validEVs,
			gender,
		};
	}
} // namespace PocketCore::Registry::Pokemon
