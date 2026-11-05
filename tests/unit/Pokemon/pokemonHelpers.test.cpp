/*! @file pokemonHelpers.test.cpp
	@brief C++ file for running tests for the Pokemon helper functions.
	@date 11/05/2026
	@since 0.12.28
	@version 0.12.54
	@author Matthew Moore
*/

#include "Pokemon/pokemonHelpers.h"

#include <algorithm>
#include <array>
#include <cstddef>
#include <expected>
#include <memory>
#include <sstream>
#include <vector>

#include "Ability/abilityID.h"
#include "Ability/abilityMeta.h"
#include "Ability/builtInAbilityID.h"
#include "Battle/battleRandomSources.h"
#include "Configuration/abilityRegistryConfiguration.h"
#include "Configuration/constants.h"
#include "Configuration/itemRegistryConfiguration.h"
#include "Configuration/natureRegistryConfiguration.h"
#include "Core/typedefs.h"
#include "EggGroup/builtInEggGroupID.h"
#include "Item/builtInItemID.h"
#include "Item/itemID.h"
#include "Learnset/learnsetMeta.h"
#include "Move/builtInMoveID.h"
#include "Move/moveID.h"
#include "Nature/builtInNatureID.h"
#include "Nature/natureID.h"
#include "Nature/natureMeta.h"
#include "Pokemon/pokemon.h"
#include "Pokemon/pokemon.testHelper.h"
#include "Pokemon/pokemonEncounter.h"
#include "Pokemon/pokemonInstantiationContext.h"
#include "Pokemon/pokemonMeta.h"
#include "Pokemon/pokemonStats.h"
#include "Registry/abilityRegistry.h"
#include "Registry/itemRegistry.h"
#include "Registry/moveRegistry.h"
#include "Registry/natureRegistry.h"
#include "Registry/registryProvider.h"
#include "Registry/registryProvider.testHelper.h"
#include "Status/builtInStatusID.h"
#include "Type/builtInTypeID.h"
#include "Utility/Random/callableRandomSource.h"
#include "Utility/Random/random.testHelper.h"
#include "Validation/Ability/abilityError.h"
#include "Validation/Item/itemError.h"
#include "Validation/Move/moveError.h"
#include "Validation/Nature/natureError.h"
#include "Validation/Pokemon/pokemonError.h"
#include "Validation/Registry/registryError.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Ability::AbilityClassification;
using PocketCore::Ability::AbilityID;
using PocketCore::Ability::BuiltinAbilityID;
using PocketCore::Ability::NO_ABILITY_ID;
using PocketCore::Ability::toAbilityID;
using PocketCore::Battle::BattleRandomSources;
using PocketCore::Configuration::AbilityRegistryConfiguration;
using PocketCore::Configuration::ItemRegistryConfiguration;
using PocketCore::Configuration::MAX_ABILITIES_PER_POKEMON;
using PocketCore::Configuration::MAX_ABILITY_POOL_PER_POKEMON;
using PocketCore::Configuration::MAX_ITEMS_PER_POKEMON;
using PocketCore::Configuration::MAX_MOVES_PER_POKEMON;
using PocketCore::Configuration::MAX_NATURES_PER_POKEMON;
using PocketCore::Configuration::NatureRegistryConfiguration;
using PocketCore::Core::ub;
using PocketCore::Core::us;
using PocketCore::EggGroup::BuiltinEggGroupID;
using PocketCore::EggGroup::toEggGroupID;
using PocketCore::Item::BuiltinItemID;
using PocketCore::Item::ItemID;
using PocketCore::Item::NO_ITEM_ID;
using PocketCore::Item::toItemID;
using PocketCore::Learnset::AcquisitionMethod;
using PocketCore::Learnset::LearnsetMeta;
using PocketCore::Learnset::MoveLearnEntry;
using PocketCore::Move::BuiltinMoveID;
using PocketCore::Move::MoveID;
using PocketCore::Move::NO_MOVE_ID;
using PocketCore::Move::toMoveID;
using PocketCore::Nature::BuiltinNatureID;
using PocketCore::Nature::NatureID;
using PocketCore::Nature::NatureMeta;
using PocketCore::Nature::NO_NATURE_ID;
using PocketCore::Nature::toNatureID;
using PocketCore::Pokemon::fillWithValidMoves;
using PocketCore::Pokemon::getAbilitiesFromAbilityPool;
using PocketCore::Pokemon::getEggAbilities;
using PocketCore::Pokemon::getItemsFromItemPool;
using PocketCore::Pokemon::getValidPokemonAbilities;
using PocketCore::Pokemon::getValidPokemonEVs;
using PocketCore::Pokemon::getValidPokemonItems;
using PocketCore::Pokemon::getValidPokemonIVs;
using PocketCore::Pokemon::getValidPokemonMoves;
using PocketCore::Pokemon::getValidPokemonNatures;
using PocketCore::Pokemon::Pokemon;
using PocketCore::Pokemon::POKEMON_STAT_COUNT;
using PocketCore::Pokemon::PokemonAbilityEntry;
using PocketCore::Pokemon::PokemonBreedingContext;
using PocketCore::Pokemon::PokemonEncounter;
using PocketCore::Pokemon::PokemonGender;
using PocketCore::Pokemon::PokemonItemEntry;
using PocketCore::Pokemon::PokemonOrigin;
using PocketCore::Registry::Ability::AbilityRegistry;
using PocketCore::Registry::Item::ItemRegistry;
using PocketCore::Registry::Move::MoveRegistry;
using PocketCore::Registry::Nature::NatureRegistry;
using PocketCore::Registry::RegistryProvider;
using PocketCore::Status::BuiltinStatusID;
using PocketCore::Status::toStatusID;
using PocketCore::Testing::FixedRandom;
using PocketCore::Testing::getDefaultInitializedRegistryProvider;
using PocketCore::Testing::getNullAbilityRegistryProvider;
using PocketCore::Testing::getNullItemRegistryProvider;
using PocketCore::Testing::getNullMoveRegistryProvider;
using PocketCore::Testing::getNullNatureRegistryProvider;
using PocketCore::Testing::getNullStatusRegistryProvider;
using PocketCore::Testing::getNullTypeRegistryProvider;
using PocketCore::Testing::makePokemon;
using PocketCore::Testing::SequenceRandom;
using PocketCore::Type::BuiltinTypeID;
using PocketCore::Type::toTypeID;
using PocketCore::Utility::Random::CallableRandomSource;
using PocketCore::Validation::Ability::AbilityError;
using PocketCore::Validation::Item::ItemError;
using PocketCore::Validation::Move::MoveError;
using PocketCore::Validation::Nature::NatureError;
using PocketCore::Validation::Pokemon::PokemonError;
using PocketCore::Validation::Registry::RegistryError;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("Pokemon free function")
{
	RegistryProvider provider{getDefaultInitializedRegistryProvider()};
	BattleRandomSources randomSources{};

	GIVEN("getValidPokemonNatures")
	{
		std::array<NatureID, MAX_NATURES_PER_POKEMON> validNatureIDs{};
		std::unique_ptr<NatureRegistryConfiguration> configRegistryStorage{std::make_unique<NatureRegistryConfiguration>()};
		const NatureRegistry &natureRegistry{configRegistryStorage->getRuntimeRegistry()};

		GIVEN("no passed in nature IDs")
		{
			WHEN("theres no eligble natures in the registry")
			{
				configRegistryStorage->removeAllNatures();
				std::expected<void, NatureError> result{getValidPokemonNatures({}, validNatureIDs, natureRegistry, randomSources)};

				THEN("it returns NOT_ENOUGH_REGISTERED_NATURES")
				{
					REQUIRE(!result.has_value());
					CHECK((result.error() == NatureError::NOT_ENOUGH_REGISTERED_NATURES));
				}
			}

			WHEN("selecting a nature from builtins")
			{
				CallableRandomSource<std::size_t, SequenceRandom<std::size_t>> indexSource{SequenceRandom<std::size_t>{1, 5}};
				randomSources.mIndex = indexSource;

				std::expected<void, NatureError> result{getValidPokemonNatures({}, validNatureIDs, natureRegistry, randomSources)};

				THEN("it returns Naughty Nature")
				{
					REQUIRE(result.has_value());
					CHECK((validNatureIDs.at(0) == toNatureID(BuiltinNatureID::Bold)));
				}
			}

			WHEN("selecting a custom nature")
			{
				std::expected<NatureID, RegistryError> addedResult{configRegistryStorage->addNature({.mName = "TestNature"})};

				REQUIRE(addedResult.has_value());

				NatureID addedID{addedResult.value()};
				const NatureMeta *metadata{natureRegistry.getNatureMetadata(addedID)};

				REQUIRE(metadata != nullptr);

				// Eligible natures are sorted by ID and exclude NO_NATURE_ID (0), so ID N maps to index N - 1.
				const std::size_t nextID{static_cast<std::size_t>(metadata->mNatureID.getValue()) - 1};

				CallableRandomSource<std::size_t, SequenceRandom<std::size_t>> indexSource{SequenceRandom<std::size_t>{1, nextID}};

				randomSources.mIndex = indexSource;

				std::expected<void, NatureError> result{getValidPokemonNatures({}, validNatureIDs, natureRegistry, randomSources)};

				THEN("it returns Naughty Nature")
				{
					REQUIRE(result.has_value());
					CHECK((validNatureIDs.at(0) == addedID));

					CHECK((metadata->mName == "TestNature"));
				}
			}
		}

		GIVEN("a span of nature IDs")
		{
			WHEN("calling with valid nature IDs")
			{
				std::array<NatureID, MAX_NATURES_PER_POKEMON> natureIDs{toNatureID(BuiltinNatureID::Bashful)};

				std::expected<void, NatureError> result{getValidPokemonNatures(natureIDs, validNatureIDs, natureRegistry, randomSources)};

				THEN("it successfully validates and puts the data in the validNatureIDs")
				{
					REQUIRE(result.has_value());
					CHECK(std::ranges::equal(natureIDs, validNatureIDs));
				}
			}

			WHEN("calling with duplicate nature replacement")
			{
				GIVEN("the default replacement does not have valid move metadata")
				{
					std::array<NatureID, MAX_NATURES_PER_POKEMON> natureIDs{toNatureID(BuiltinNatureID::Bashful)};

					std::expected<void, NatureError> result{
						getValidPokemonNatures(natureIDs, validNatureIDs, natureRegistry, randomSources, NatureID{200}),
					};

					THEN("it rejects and returns an error")
					{
						REQUIRE_FALSE(result.has_value());
						CHECK((result.error() == NatureError::DUPLICATE_REPLACEMENT_MISSING_METADATA));
					}
				}
			}

			WHEN("calling with NO_NATURE_ID")
			{
				std::array<NatureID, MAX_NATURES_PER_POKEMON> natureIDs{NO_NATURE_ID};

				std::expected<void, NatureError> result{getValidPokemonNatures(natureIDs, validNatureIDs, natureRegistry, randomSources)};

				THEN("it successfully validates and puts the data in the validNatureIDs")
				{
					REQUIRE(result.has_value());
					CHECK(std::ranges::equal(natureIDs, validNatureIDs));
				}
			}

			WHEN("calling with invalid nature IDs")
			{
				std::array<NatureID, MAX_NATURES_PER_POKEMON> natureIDs{NatureID{200}};

				std::expected<void, NatureError> result{getValidPokemonNatures(natureIDs, validNatureIDs, natureRegistry, randomSources)};

				THEN("it successfully invalidates and returns an error")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error() == NatureError::MISSING_NATURE_METADATA));
					CHECK((validNatureIDs.at(0) == NO_NATURE_ID));
				}
			}
		}
	}

	GIVEN("getValidPokemonAbilities")
	{
		std::array<PokemonAbilityEntry, MAX_ABILITIES_PER_POKEMON> validAbilityEntries{};
		std::unique_ptr<AbilityRegistryConfiguration> configRegistryStorage{std::make_unique<AbilityRegistryConfiguration>()};
		const AbilityRegistry &abilityRegistry{configRegistryStorage->getRuntimeRegistry()};
		Pokemon firstParent{
			makePokemon({.mMetadata = {.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Fairy)}}, .mGender = PokemonGender::Male}),
		};
		Pokemon secondParent{
			makePokemon({
			    .mMetadata = {.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Fairy)}},
				.mAbilityEntries = {{
    				{
    					.mAbilityID = toAbilityID(BuiltinAbilityID::CloudNine),
    					.mClassification = AbilityClassification::Standard,
    				},
    			},},
				.mGender = PokemonGender::Female,
			}),
		};
		PokemonBreedingContext breedingContext{.mFirstParent = &firstParent, .mSecondParent = &secondParent};
		PokemonEncounter encounterData{};

		GIVEN("no passed in ability IDs")
		{
			std::vector<PokemonAbilityEntry> abilityPool{
				{.mAbilityID = toAbilityID(BuiltinAbilityID::Blaze), .mClassification = AbilityClassification::Standard},
				{.mAbilityID = toAbilityID(BuiltinAbilityID::Guts), .mClassification = AbilityClassification::Standard},
				{.mAbilityID = toAbilityID(BuiltinAbilityID::SolarPower), .mClassification = AbilityClassification::Hidden},
				{.mAbilityID = toAbilityID(BuiltinAbilityID::Guts), .mClassification = AbilityClassification::Hidden},
			};

			WHEN("theres no eligble abilities in the registry")
			{
				configRegistryStorage->removeAllAbilities();

				std::expected<void, AbilityError> result{
					getValidPokemonAbilities({}, validAbilityEntries, abilityPool, abilityRegistry, randomSources, breedingContext,
											 encounterData),
				};

				THEN("it returns NOT_ENOUGH_REGISTERED_ABILITIES")
				{
					REQUIRE(!result.has_value());
					CHECK((result.error() == AbilityError::NOT_ENOUGH_REGISTERED_ABILITIES));
				}
			}

			WHEN("there's an invalid ability ID in the ability pool")
			{
				abilityPool.push_back({.mAbilityID = AbilityID{200}});

				std::expected<void, AbilityError> result{
					getValidPokemonAbilities({}, validAbilityEntries, abilityPool, abilityRegistry, randomSources, breedingContext,
											 encounterData),
				};

				THEN("it returns MISSING_ABILITY_METADATA")
				{
					REQUIRE(!result.has_value());
					CHECK((result.error() == AbilityError::MISSING_ABILITY_METADATA));
				}
			}

			WHEN("there's valid ability IDs in the ability pool")
			{
				CallableRandomSource<std::size_t, SequenceRandom<std::size_t>> indexSource{SequenceRandom<std::size_t>{1, 1}};
				randomSources.mIndex = indexSource;

				std::expected<void, AbilityError> result{
					getValidPokemonAbilities({}, validAbilityEntries, abilityPool, abilityRegistry, randomSources, breedingContext,
											 encounterData),
				};

				PokemonAbilityEntry expected{
					.mAbilityID = toAbilityID(BuiltinAbilityID::Guts),
					.mClassification = AbilityClassification::Standard,
				};

				THEN("it adds the ability IDs to the array")
				{
					REQUIRE(result.has_value());

					CHECK((validAbilityEntries.at(0) == expected));
				}
			}

			GIVEN("the encounter is an egg")
			{
				encounterData.mOrigin = PokemonOrigin::Egg;

				WHEN("calling with invalid breeding context")
				{
					Pokemon newSecondPokemon{makePokemon({.mGender = PokemonGender::Male})};
					breedingContext.mSecondParent = &newSecondPokemon;

					std::expected<void, AbilityError> result{
						getValidPokemonAbilities({}, validAbilityEntries, abilityPool, abilityRegistry, randomSources, breedingContext,
												 encounterData),
					};

					THEN("it returns an error")
					{
						REQUIRE_FALSE(result.has_value());
						CHECK((result.error() == AbilityError::SAME_SEX_PARENTS_CANT_REPRODUCE));
					}
				}

				WHEN("calling with valid breeding context")
				{
					CallableRandomSource<std::size_t, SequenceRandom<std::size_t>> indexSource{SequenceRandom<std::size_t>{1, 1}};
					CallableRandomSource<ub, FixedRandom<ub>> unsignedSource{FixedRandom<ub>{80}};

					randomSources.mIndex = indexSource;
					randomSources.mUnsigned = unsignedSource;

					std::expected<void, AbilityError> result{
						getValidPokemonAbilities({}, validAbilityEntries, abilityPool, abilityRegistry, randomSources, breedingContext,
												 encounterData),
					};

					PokemonAbilityEntry expected{
						.mAbilityID = toAbilityID(BuiltinAbilityID::CloudNine),
						.mClassification = AbilityClassification::Standard,
					};

					THEN("the ability pool ability is overwritten by the parent's ability")
					{
						REQUIRE(result.has_value());
						CHECK((validAbilityEntries.at(0) == expected));
					}
				}
			}
		}

		GIVEN("a span of ability IDs")
		{
			WHEN("calling with valid ability IDs")
			{
				std::array<PokemonAbilityEntry, MAX_ABILITIES_PER_POKEMON> abilityEntries{
					{{.mAbilityID = toAbilityID(BuiltinAbilityID::CloudNine)}},
				};
				std::array<PokemonAbilityEntry, MAX_ABILITY_POOL_PER_POKEMON> abilityPool{};

				std::expected<void, AbilityError> result{
					getValidPokemonAbilities(abilityEntries, validAbilityEntries, abilityPool, abilityRegistry, randomSources,
											 breedingContext, encounterData),
				};

				THEN("it successfully validates and puts the data in the validabilityEntries")
				{
					REQUIRE(result.has_value());
					CHECK(std::ranges::equal(abilityEntries, validAbilityEntries));
				}
			}

			WHEN("calling with duplicate ability replacement")
			{
				GIVEN("the default replacement does not have valid move metadata")
				{
					std::array<PokemonAbilityEntry, MAX_ABILITIES_PER_POKEMON> abilityEntries{
						{{.mAbilityID = toAbilityID(BuiltinAbilityID::CloudNine)}},
					};
					std::array<PokemonAbilityEntry, MAX_ABILITY_POOL_PER_POKEMON> abilityPool{};

					std::expected<void, AbilityError> result{
						getValidPokemonAbilities(abilityEntries, validAbilityEntries, abilityPool, abilityRegistry, randomSources,
												 breedingContext, encounterData, {.mAbilityID = AbilityID{200}}),
					};

					THEN("it rejects and returns an error")
					{
						REQUIRE_FALSE(result.has_value());
						CHECK((result.error() == AbilityError::DUPLICATE_REPLACEMENT_MISSING_METADATA));
					}
				}
			}

			WHEN("calling with NO_ABILITY_ID")
			{
				std::array<PokemonAbilityEntry, MAX_ABILITIES_PER_POKEMON> abilityEntries{{{.mAbilityID = NO_ABILITY_ID}}};
				std::array<PokemonAbilityEntry, MAX_ABILITY_POOL_PER_POKEMON> abilityPool{};

				std::expected<void, AbilityError> result{
					getValidPokemonAbilities(abilityEntries, validAbilityEntries, abilityPool, abilityRegistry, randomSources,
											 breedingContext, encounterData),
				};

				THEN("it successfully validates and puts the data in the validabilityEntries")
				{
					REQUIRE(result.has_value());
					CHECK(std::ranges::equal(abilityEntries, validAbilityEntries));
				}
			}

			WHEN("calling with invalid ability IDs")
			{
				std::array<PokemonAbilityEntry, MAX_ABILITIES_PER_POKEMON> abilityEntries{{{.mAbilityID = AbilityID{200}}}};
				std::array<PokemonAbilityEntry, MAX_ABILITY_POOL_PER_POKEMON> abilityPool{};

				std::expected<void, AbilityError> result{
					getValidPokemonAbilities(abilityEntries, validAbilityEntries, abilityPool, abilityRegistry, randomSources,
											 breedingContext, encounterData),
				};

				THEN("it successfully invalidates and returns an error")
				{
					PokemonAbilityEntry expected{.mAbilityID = NO_ABILITY_ID};

					REQUIRE_FALSE(result.has_value());
					CHECK((result.error() == AbilityError::MISSING_ABILITY_METADATA));
					CHECK((validAbilityEntries.at(0) == expected));
				}
			}
		}
	}

	GIVEN("getAbilitiesFromAbilityPool")
	{
		std::array<PokemonAbilityEntry, MAX_ABILITIES_PER_POKEMON> validAbilityEntries{};
		std::vector<PokemonAbilityEntry> abilityPool{
			{.mAbilityID = NO_ABILITY_ID},
			{.mAbilityID = toAbilityID(BuiltinAbilityID::Blaze), .mClassification = AbilityClassification::Standard},
			{.mAbilityID = toAbilityID(BuiltinAbilityID::Guts), .mClassification = AbilityClassification::Standard},
			{.mAbilityID = toAbilityID(BuiltinAbilityID::SolarPower), .mClassification = AbilityClassification::Hidden},
			{.mAbilityID = toAbilityID(BuiltinAbilityID::Guts), .mClassification = AbilityClassification::Hidden},
		};
		std::unique_ptr<AbilityRegistryConfiguration> configRegistryStorage{std::make_unique<AbilityRegistryConfiguration>()};
		const AbilityRegistry &abilityRegistry{configRegistryStorage->getRuntimeRegistry()};

		WHEN("calling with no selectable entries")
		{
			std::expected<void, AbilityError> result{
				getAbilitiesFromAbilityPool(validAbilityEntries, {}, abilityRegistry, randomSources),
			};

			THEN("it returns EMPTY_ABILITY_POOL")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error() == AbilityError::EMPTY_ABILITY_POOL));
			}
		}

		WHEN("calling with an invalid ability ID")
		{
			abilityPool.push_back({.mAbilityID = AbilityID{200}});

			std::expected<void, AbilityError> result{
				getAbilitiesFromAbilityPool(validAbilityEntries, abilityPool, abilityRegistry, randomSources),
			};

			THEN("it returns MISSING_ABILITY_METADATA")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error() == AbilityError::MISSING_ABILITY_METADATA));
			}
		}

		WHEN("calling with an valid ability IDs")
		{
			CallableRandomSource<std::size_t, SequenceRandom<std::size_t>> indexSource{SequenceRandom<std::size_t>{1, 1}};
			randomSources.mIndex = indexSource;

			std::expected<void, AbilityError> result{
				getAbilitiesFromAbilityPool(validAbilityEntries, abilityPool, abilityRegistry, randomSources),
			};

			PokemonAbilityEntry expected{
				.mAbilityID = toAbilityID(BuiltinAbilityID::Guts),
				.mClassification = AbilityClassification::Standard,
			};

			THEN("it adds the ability IDs to the array")
			{
				REQUIRE(result.has_value());

				CHECK((validAbilityEntries.at(0) == expected));
			}
		}
	}

	GIVEN("getEggAbilities")
	{
		std::array<PokemonAbilityEntry, MAX_ABILITIES_PER_POKEMON> validAbilityEntries{};
		Pokemon firstParent{
			makePokemon({.mMetadata = {.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Fairy)}}, .mGender = PokemonGender::Male}),
		};
		Pokemon secondParent{
			makePokemon({
			    .mMetadata = {.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Fairy)}},
				.mAbilityEntries = {{
    				{
    					.mAbilityID = toAbilityID(BuiltinAbilityID::Blaze),
    					.mClassification = AbilityClassification::Standard,
    				},
    			},},
				.mGender = PokemonGender::Female,
			}),
		};
		PokemonBreedingContext breedingContext{.mFirstParent = &firstParent, .mSecondParent = &secondParent};

		WHEN("calling when the first parent is null")
		{
			breedingContext.mFirstParent = nullptr;

			std::expected<void, AbilityError> result{getEggAbilities(validAbilityEntries, breedingContext, randomSources)};

			THEN("it returns INVALID_BREEDING_CONTEXT")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error() == AbilityError::INVALID_BREEDING_CONTEXT));
			}
		}

		WHEN("calling when the second parent is null")
		{
			breedingContext.mSecondParent = nullptr;

			std::expected<void, AbilityError> result{getEggAbilities(validAbilityEntries, breedingContext, randomSources)};

			THEN("it returns INVALID_BREEDING_CONTEXT")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error() == AbilityError::INVALID_BREEDING_CONTEXT));
			}
		}

		WHEN("calling when both parents are male")
		{
			secondParent.setGender(PokemonGender::Male);

			std::expected<void, AbilityError> result{getEggAbilities(validAbilityEntries, breedingContext, randomSources)};

			THEN("it returns SAME_SEX_PARENTS_CANT_REPRODUCE")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error() == AbilityError::SAME_SEX_PARENTS_CANT_REPRODUCE));
			}
		}

		WHEN("calling when both parents are female")
		{
			firstParent.setGender(PokemonGender::Female);

			std::expected<void, AbilityError> result{getEggAbilities(validAbilityEntries, breedingContext, randomSources)};

			THEN("it returns SAME_SEX_PARENTS_CANT_REPRODUCE")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error() == AbilityError::SAME_SEX_PARENTS_CANT_REPRODUCE));
			}
		}

		WHEN("calling when the parents's egg groups are not compatible")
		{
			Pokemon newSecondPokemon{
				makePokemon(
					{.mMetadata = {.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::HumanLike)}}, .mGender = PokemonGender::Female}),
			};
			breedingContext.mSecondParent = &newSecondPokemon;

			std::expected<void, AbilityError> result{getEggAbilities(validAbilityEntries, breedingContext, randomSources)};

			THEN("it returns NO_COMMON_EGG_GROUP")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error() == AbilityError::NO_COMMON_EGG_GROUP));
			}
		}

		GIVEN("calling when inheriting from the second pokemon")
		{
			WHEN("the second parent is female with a standard ability")
			{
				Pokemon newSecondPokemon{
					makePokemon({
						.mMetadata = {.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Fairy)}},
						.mAbilityEntries = {{
							{
								.mAbilityID = toAbilityID(BuiltinAbilityID::Blaze),
								.mClassification = AbilityClassification::Standard,
							},
						},},
						.mGender = PokemonGender::Female,
					}),
				};
				breedingContext.mSecondParent = &newSecondPokemon;

				WHEN("the chance fails")
				{
					CallableRandomSource<ub, FixedRandom<ub>> unsignedSource{FixedRandom<ub>{81}};
					randomSources.mUnsigned = unsignedSource;

					std::expected<void, AbilityError> result{getEggAbilities(validAbilityEntries, breedingContext, randomSources)};

					THEN("the ability is not added")
					{
						REQUIRE(result.has_value());
						CHECK(std::ranges::all_of(validAbilityEntries,
												  [](const PokemonAbilityEntry &entry) { return entry.mAbilityID == NO_ABILITY_ID; }));
					}
				}

				WHEN("the array already holds it")
				{
					validAbilityEntries.at(0) = {
						.mAbilityID = toAbilityID(BuiltinAbilityID::Blaze),
						.mClassification = AbilityClassification::Standard,
					};
					CallableRandomSource<ub, FixedRandom<ub>> unsignedSource{FixedRandom<ub>{80}};
					randomSources.mUnsigned = unsignedSource;

					std::expected<void, AbilityError> result{getEggAbilities(validAbilityEntries, breedingContext, randomSources)};

					PokemonAbilityEntry expected{
						.mAbilityID = toAbilityID(BuiltinAbilityID::Blaze),
						.mClassification = AbilityClassification::Standard,
					};

					THEN("the ability is not added and the original is retained")
					{
						REQUIRE(result.has_value());
						CHECK((validAbilityEntries.at(0) == expected));
					}
				}

				WHEN("the chance passes")
				{
					CallableRandomSource<ub, FixedRandom<ub>> unsignedSource{FixedRandom<ub>{80}};
					randomSources.mUnsigned = unsignedSource;

					std::expected<void, AbilityError> result{getEggAbilities(validAbilityEntries, breedingContext, randomSources)};

					PokemonAbilityEntry expected{
						.mAbilityID = toAbilityID(BuiltinAbilityID::Blaze),
						.mClassification = AbilityClassification::Standard,
					};

					THEN("the ability is added")
					{
						REQUIRE(result.has_value());
						CHECK((validAbilityEntries.at(0) == expected));
					}
				}
			}

			WHEN("the second parent is male with a standard ability and the first parent is a ditto")
			{
				Pokemon newSecondPokemon{
					makePokemon({
						.mMetadata = {.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Fairy)}},
						.mAbilityEntries = {{
							{
								.mAbilityID = toAbilityID(BuiltinAbilityID::Blaze),
								.mClassification = AbilityClassification::Standard,
							},
						},},
						.mGender = PokemonGender::Male,
					}),
				};
				Pokemon newFirstPokemon{
					makePokemon({
						.mMetadata = {.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Ditto)}},
						.mGender = PokemonGender::Genderless,
					}),
				};
				breedingContext.mFirstParent = &newFirstPokemon;
				breedingContext.mSecondParent = &newSecondPokemon;

				WHEN("the chance fails")
				{
					CallableRandomSource<ub, FixedRandom<ub>> unsignedSource{FixedRandom<ub>{81}};
					randomSources.mUnsigned = unsignedSource;

					std::expected<void, AbilityError> result{getEggAbilities(validAbilityEntries, breedingContext, randomSources)};

					THEN("the ability is not added")
					{
						REQUIRE(result.has_value());
						CHECK(std::ranges::all_of(validAbilityEntries,
												  [](const PokemonAbilityEntry &entry) { return entry.mAbilityID == NO_ABILITY_ID; }));
					}
				}

				WHEN("the array already holds it")
				{
					validAbilityEntries.at(0) = {
						.mAbilityID = toAbilityID(BuiltinAbilityID::Blaze),
						.mClassification = AbilityClassification::Standard,
					};
					CallableRandomSource<ub, FixedRandom<ub>> unsignedSource{FixedRandom<ub>{80}};
					randomSources.mUnsigned = unsignedSource;

					std::expected<void, AbilityError> result{getEggAbilities(validAbilityEntries, breedingContext, randomSources)};

					PokemonAbilityEntry expected{
						.mAbilityID = toAbilityID(BuiltinAbilityID::Blaze),
						.mClassification = AbilityClassification::Standard,
					};

					THEN("the ability is not added and the original is retained")
					{
						REQUIRE(result.has_value());
						CHECK((validAbilityEntries.at(0) == expected));
					}
				}

				WHEN("the chance passes")
				{
					CallableRandomSource<ub, FixedRandom<ub>> unsignedSource{FixedRandom<ub>{80}};
					randomSources.mUnsigned = unsignedSource;

					std::expected<void, AbilityError> result{getEggAbilities(validAbilityEntries, breedingContext, randomSources)};

					PokemonAbilityEntry expected{
						.mAbilityID = toAbilityID(BuiltinAbilityID::Blaze),
						.mClassification = AbilityClassification::Standard,
					};

					THEN("the ability is added")
					{
						REQUIRE(result.has_value());
						CHECK((validAbilityEntries.at(0) == expected));
					}
				}
			}
		}

		GIVEN("calling when inheriting from the first pokemon")
		{
			WHEN("the second parent is male with a hidden ability and the first parent is a ditto")
			{
				Pokemon newSecondPokemon{
					makePokemon({
						.mMetadata = {.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Fairy)}},
						.mGender = PokemonGender::Male,
					}),
				};
				Pokemon newFirstPokemon{
					makePokemon({
						.mMetadata = {.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Fairy)}},
						.mAbilityEntries = {{
							{
								.mAbilityID = toAbilityID(BuiltinAbilityID::Blaze),
								.mClassification = AbilityClassification::Hidden,
							},
						},},
						.mGender = PokemonGender::Female,
					}),
				};
				breedingContext.mFirstParent = &newFirstPokemon;
				breedingContext.mSecondParent = &newSecondPokemon;

				WHEN("the chance fails")
				{
					CallableRandomSource<ub, FixedRandom<ub>> unsignedSource{FixedRandom<ub>{61}};
					randomSources.mUnsigned = unsignedSource;

					std::expected<void, AbilityError> result{getEggAbilities(validAbilityEntries, breedingContext, randomSources)};

					THEN("the ability is not added")
					{
						REQUIRE(result.has_value());
						CHECK(std::ranges::all_of(validAbilityEntries,
												  [](const PokemonAbilityEntry &entry) { return entry.mAbilityID == NO_ABILITY_ID; }));
					}
				}

				WHEN("the array already holds it")
				{
					validAbilityEntries.at(0) = {
						.mAbilityID = toAbilityID(BuiltinAbilityID::Blaze),
						.mClassification = AbilityClassification::Hidden,
					};
					CallableRandomSource<ub, FixedRandom<ub>> unsignedSource{FixedRandom<ub>{60}};
					randomSources.mUnsigned = unsignedSource;

					std::expected<void, AbilityError> result{getEggAbilities(validAbilityEntries, breedingContext, randomSources)};

					PokemonAbilityEntry expected{
						.mAbilityID = toAbilityID(BuiltinAbilityID::Blaze),
						.mClassification = AbilityClassification::Hidden,
					};

					THEN("the ability is not added and the original is retained")
					{
						REQUIRE(result.has_value());
						CHECK((validAbilityEntries.at(0) == expected));
					}
				}

				WHEN("the chance passes")
				{
					CallableRandomSource<ub, FixedRandom<ub>> unsignedSource{FixedRandom<ub>{60}};
					randomSources.mUnsigned = unsignedSource;

					std::expected<void, AbilityError> result{getEggAbilities(validAbilityEntries, breedingContext, randomSources)};

					PokemonAbilityEntry expected{
						.mAbilityID = toAbilityID(BuiltinAbilityID::Blaze),
						.mClassification = AbilityClassification::Hidden,
					};

					THEN("the ability is added")
					{
						REQUIRE(result.has_value());
						CHECK((validAbilityEntries.at(0) == expected));
					}
				}
			}
		}
	}

	GIVEN("getValidPokemonItems")
	{
		std::array<ItemID, MAX_ITEMS_PER_POKEMON> validItemIDs{};
		std::unique_ptr<ItemRegistryConfiguration> configRegistryStorage{std::make_unique<ItemRegistryConfiguration>()};
		const ItemRegistry &itemRegistry{configRegistryStorage->getRuntimeRegistry()};
		std::vector<PokemonItemEntry> itemPool{
			{.mItemID = toItemID(BuiltinItemID::AbilityCapsule), .mChance = 0.1},
			{.mItemID = toItemID(BuiltinItemID::CheriBerry), .mChance = 0.5},
			{.mItemID = toItemID(BuiltinItemID::ChestoBerry), .mChance = 0.7},
		};

		GIVEN("no passed in item IDs")
		{
			WHEN("theres no eligble items in the registry")
			{
				configRegistryStorage->removeAllItems();

				std::expected<void, ItemError> result{getValidPokemonItems({}, validItemIDs, itemPool, itemRegistry, randomSources)};

				THEN("it returns NOT_ENOUGH_REGISTERED_ITEMS")
				{
					REQUIRE(!result.has_value());
					CHECK((result.error() == ItemError::NOT_ENOUGH_REGISTERED_ITEMS));
				}
			}

			WHEN("there's an invalid item ID in the item pool")
			{
				itemPool.push_back({.mItemID = ItemID{200}});

				std::expected<void, ItemError> result{getValidPokemonItems({}, validItemIDs, itemPool, itemRegistry, randomSources)};

				THEN("it returns MISSING_ITEM_METADATA")
				{
					REQUIRE(!result.has_value());
					CHECK((result.error() == ItemError::MISSING_ITEM_METADATA));
				}
			}

			WHEN("there's valid item IDs in the item pool")
			{
				CallableRandomSource<double, SequenceRandom<double>> realSource{SequenceRandom<double>{0.3, 0.4, 0.8}};
				CallableRandomSource<std::size_t, FixedRandom<std::size_t>> indexSource{FixedRandom<std::size_t>{0}};

				randomSources.mReal = realSource;
				randomSources.mIndex = indexSource;

				std::expected<void, ItemError> result{getValidPokemonItems({}, validItemIDs, itemPool, itemRegistry, randomSources)};

				THEN("it adds the ability IDs to the array")
				{
					REQUIRE(result.has_value());

					CHECK((validItemIDs.at(0) == toItemID(BuiltinItemID::CheriBerry)));
				}
			}
		}

		GIVEN("a span of item IDs")
		{
			WHEN("calling with valid item IDs")
			{
				std::array<ItemID, MAX_ITEMS_PER_POKEMON> itemIDs{toItemID(BuiltinItemID::CheriBerry)};

				std::expected<void, ItemError> result{getValidPokemonItems(itemIDs, validItemIDs, itemPool, itemRegistry, randomSources)};

				THEN("it successfully validates and puts the data in the validItemIDs")
				{
					REQUIRE(result.has_value());
					CHECK(std::ranges::equal(itemIDs, validItemIDs));
				}
			}

			WHEN("calling with duplicate item replacement")
			{
				GIVEN("the default replacement does not have valid move metadata")
				{
					std::array<ItemID, MAX_ITEMS_PER_POKEMON> itemIDs{toItemID(BuiltinItemID::CheriBerry)};

					std::expected<void, ItemError> result{
						getValidPokemonItems(itemIDs, validItemIDs, itemPool, itemRegistry, randomSources, ItemID{200}),
					};

					THEN("it rejects and returns an error")
					{
						REQUIRE_FALSE(result.has_value());
						CHECK((result.error() == ItemError::DUPLICATE_REPLACEMENT_MISSING_METADATA));
					}
				}
			}

			WHEN("calling with NO_ITEM_ID")
			{
				std::array<ItemID, MAX_ITEMS_PER_POKEMON> itemIDs{NO_ITEM_ID};

				std::expected<void, ItemError> result{getValidPokemonItems(itemIDs, validItemIDs, itemPool, itemRegistry, randomSources)};

				THEN("it successfully validates and puts the data in the validItemIDs")
				{
					REQUIRE(result.has_value());
					CHECK(std::ranges::equal(itemIDs, validItemIDs));
				}
			}

			WHEN("calling with invalid item IDs")
			{
				std::array<ItemID, MAX_ITEMS_PER_POKEMON> itemIDs{ItemID{200}};

				std::expected<void, ItemError> result{getValidPokemonItems(itemIDs, validItemIDs, itemPool, itemRegistry, randomSources)};

				THEN("it successfully invalidates and returns an error")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error() == ItemError::MISSING_ITEM_METADATA));
					CHECK((validItemIDs.at(0) == NO_ITEM_ID));
				}
			}
		}
	}

	GIVEN("getItemsFromItemPool")
	{
		std::array<ItemID, MAX_ITEMS_PER_POKEMON> validItemIDs{};
		std::unique_ptr<ItemRegistryConfiguration> configRegistryStorage{std::make_unique<ItemRegistryConfiguration>()};
		const ItemRegistry &itemRegistry{configRegistryStorage->getRuntimeRegistry()};
		std::vector<PokemonItemEntry> itemPool{
			{.mItemID = NO_ITEM_ID},
			{.mItemID = toItemID(BuiltinItemID::AbilityCapsule), .mChance = 0.1},
			{.mItemID = toItemID(BuiltinItemID::CheriBerry), .mChance = 0.5},
			{.mItemID = toItemID(BuiltinItemID::ChestoBerry), .mChance = 0.7},
			{.mItemID = toItemID(BuiltinItemID::AbilityCapsule), .mChance = 0.9},
		};

		WHEN("calling with no selectable entries")
		{
			std::expected<void, ItemError> result{
				getItemsFromItemPool(validItemIDs, {}, itemRegistry, randomSources),
			};

			THEN("it returns EMPTY_ITEM_POOL")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error() == ItemError::EMPTY_ITEM_POOL));
			}
		}

		WHEN("calling with an invalid ability ID")
		{
			itemPool.push_back({.mItemID = ItemID{200}});

			std::expected<void, ItemError> result{
				getItemsFromItemPool(validItemIDs, itemPool, itemRegistry, randomSources),
			};

			THEN("it returns MISSING_ITEM_METADATA")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error() == ItemError::MISSING_ITEM_METADATA));
			}
		}

		WHEN("calling when all items fail their chance")
		{
			CallableRandomSource<double, SequenceRandom<double>> realSource{SequenceRandom<double>{0.2, 0.7, 0.9}};
			randomSources.mReal = realSource;

			std::expected<void, ItemError> result{
				getItemsFromItemPool(validItemIDs, itemPool, itemRegistry, randomSources),
			};

			THEN("no items from item pool are added")
			{
				REQUIRE(result.has_value());
				CHECK(std::ranges::all_of(validItemIDs, [](const ItemID itemID) { return itemID == NO_ITEM_ID; }));
			}
		}

		WHEN("calling when an item passes")
		{
			CallableRandomSource<double, SequenceRandom<double>> realSource{SequenceRandom<double>{0.2, 0.4, 0.9}};
			CallableRandomSource<std::size_t, FixedRandom<std::size_t>> indexSource{FixedRandom<std::size_t>{0}};

			randomSources.mReal = realSource;
			randomSources.mIndex = indexSource;

			std::expected<void, ItemError> result{
				getItemsFromItemPool(validItemIDs, itemPool, itemRegistry, randomSources),
			};

			THEN("no items from item pool are added")
			{
				REQUIRE(result.has_value());
				CHECK((validItemIDs.at(0) == toItemID(BuiltinItemID::CheriBerry)));
			}
		}
	}

	GIVEN("getValidPokemonMoves")
	{
		std::array<MoveID, MAX_MOVES_PER_POKEMON> validMoveIDs{};
		LearnsetMeta meta{};
		PokemonEncounter encounter{.mOrigin = PokemonOrigin::Egg};
		us pokemonEncounterLevel{10};
		MoveRegistry moveRegistry{*provider.mMoveRegistry};

		WHEN("no passed in move IDs")
		{
			GIVEN("valid metadata")
			{
				std::vector<MoveLearnEntry> entries{
					{.mMoveID = toMoveID(BuiltinMoveID::Facade), .mLevelRequirement = 10},
					{.mMoveID = toMoveID(BuiltinMoveID::Pound), .mLevelRequirement = 2},
					{
						.mMoveID = toMoveID(BuiltinMoveID::KarateChop),
						.mLevelRequirement = 8,
						.mAcquisitionMethod = AcquisitionMethod::Egg,
					},
					{.mMoveID = toMoveID(BuiltinMoveID::Pound), .mLevelRequirement = 1},
					{.mMoveID = toMoveID(BuiltinMoveID::KarateChop), .mLevelRequirement = 6},
				};

				meta.mEntries = entries;

				std::expected<void, MoveError> result{
					getValidPokemonMoves({}, validMoveIDs, moveRegistry, meta, encounter, pokemonEncounterLevel),
				};

				THEN("it properly returns with filled out valid move IDs")
				{
					std::vector<MoveID> expected{
						toMoveID(BuiltinMoveID::KarateChop),
						toMoveID(BuiltinMoveID::Pound),
						toMoveID(BuiltinMoveID::Facade),
						NO_MOVE_ID,
					};

					REQUIRE(result.has_value());
					CHECK(std::ranges::equal(validMoveIDs, expected));
				}
			}

			GIVEN("invalid metadata")
			{
				std::vector<MoveLearnEntry> entries{
					{.mMoveID = toMoveID(BuiltinMoveID::Facade), .mLevelRequirement = 10},
					{.mMoveID = MoveID{200}, .mLevelRequirement = 2},
					{
						.mMoveID = toMoveID(BuiltinMoveID::KarateChop),
						.mLevelRequirement = 8,
						.mAcquisitionMethod = AcquisitionMethod::Egg,
					},
					{.mMoveID = toMoveID(BuiltinMoveID::Pound), .mLevelRequirement = 1},
					{.mMoveID = toMoveID(BuiltinMoveID::KarateChop), .mLevelRequirement = 6},
				};

				meta.mEntries = entries;

				std::expected<void, MoveError> result{
					getValidPokemonMoves({}, validMoveIDs, moveRegistry, meta, encounter, pokemonEncounterLevel),
				};

				THEN("it leaves the move ID array empty and returns an error")
				{
					std::vector<MoveID> expected{
						NO_MOVE_ID,
						NO_MOVE_ID,
						NO_MOVE_ID,
						NO_MOVE_ID,
					};

					REQUIRE_FALSE(result.has_value());
					CHECK((result.error() == MoveError::MISSING_MOVE_METADATA));
					CHECK(std::ranges::equal(validMoveIDs, expected));
				}
			}
		}

		GIVEN("a span of move IDs")
		{
			WHEN("calling with valid move IDs")
			{
				std::array<MoveID, MAX_MOVES_PER_POKEMON> moveIDs{
					toMoveID(BuiltinMoveID::HydroSteam),
					toMoveID(BuiltinMoveID::KarateChop),
					toMoveID(BuiltinMoveID::Facade),
					toMoveID(BuiltinMoveID::Pound),
				};

				std::expected<void, MoveError> result{
					getValidPokemonMoves(moveIDs, validMoveIDs, moveRegistry, meta, encounter, pokemonEncounterLevel),
				};

				THEN("it successfully validates and puts the data in the validMoveIDs")
				{
					REQUIRE(result.has_value());
					CHECK(std::ranges::equal(moveIDs, validMoveIDs));
				}
			}

			WHEN("calling with duplicate move IDs")
			{
				GIVEN("the default replacement does not have valid move metadata")
				{
					std::array<MoveID, MAX_MOVES_PER_POKEMON> moveIDs{
						toMoveID(BuiltinMoveID::HydroSteam),
						toMoveID(BuiltinMoveID::KarateChop),
						toMoveID(BuiltinMoveID::KarateChop),
						toMoveID(BuiltinMoveID::Pound),
					};

					std::expected<void, MoveError> result{
						getValidPokemonMoves(moveIDs, validMoveIDs, moveRegistry, meta, encounter, pokemonEncounterLevel, MoveID{200}),
					};

					THEN("it rejects and returns an error")
					{
						REQUIRE_FALSE(result.has_value());
						CHECK((result.error() == MoveError::DUPLICATE_REPLACEMENT_MISSING_METADATA));
					}
				}

				GIVEN("the default replacement has valid move metadata")
				{
					std::array<MoveID, MAX_MOVES_PER_POKEMON> moveIDs{
						toMoveID(BuiltinMoveID::HydroSteam),
						toMoveID(BuiltinMoveID::KarateChop),
						toMoveID(BuiltinMoveID::KarateChop),
						toMoveID(BuiltinMoveID::Pound),
					};

					std::expected<void, MoveError> result{
						getValidPokemonMoves(moveIDs, validMoveIDs, moveRegistry, meta, encounter, pokemonEncounterLevel),
					};

					THEN("it successfully validates, replaces the duplicate with NO_MOVE_ID, and sorts it to the back while preserving "
						 "the "
						 "other values in their order")
					{
						std::array<MoveID, MAX_MOVES_PER_POKEMON> expected{
							toMoveID(BuiltinMoveID::HydroSteam),
							toMoveID(BuiltinMoveID::KarateChop),
							toMoveID(BuiltinMoveID::Pound),
							NO_MOVE_ID,
						};

						REQUIRE(result.has_value());
						CHECK(std::ranges::equal(validMoveIDs, expected));
					}
				}
			}

			WHEN("calling with NO_MOVE_ID")
			{
				std::array<MoveID, MAX_MOVES_PER_POKEMON> moveIDs{NO_MOVE_ID};

				std::expected<void, MoveError> result{
					getValidPokemonMoves(moveIDs, validMoveIDs, moveRegistry, meta, encounter, pokemonEncounterLevel),
				};

				THEN("it successfully validates and puts the data in the validMoveIDs")
				{
					REQUIRE(result.has_value());
					CHECK(std::ranges::equal(moveIDs, validMoveIDs));
				}
			}

			WHEN("calling with invalid move IDs")
			{
				std::array<MoveID, MAX_MOVES_PER_POKEMON> moveIDs{MoveID{200}};

				std::expected<void, MoveError> result{
					getValidPokemonMoves(moveIDs, validMoveIDs, moveRegistry, meta, encounter, pokemonEncounterLevel),
				};

				THEN("it successfully invalidates and returns an error")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error() == MoveError::MISSING_MOVE_METADATA));
					CHECK((validMoveIDs.at(0) == NO_MOVE_ID));
				}
			}
		}
	}

	GIVEN("fillWithValidMoves")
	{
		std::array<MoveID, MAX_MOVES_PER_POKEMON> moveIDs{};
		LearnsetMeta meta{};
		PokemonEncounter encounter{};
		us pokemonEncounterLevel{0};
		MoveRegistry moveRegistry{*provider.mMoveRegistry};

		GIVEN("a wild Pokemon")
		{
			encounter.mOrigin = PokemonOrigin::Wild;

			GIVEN("four possible moves")
			{
				GIVEN("with/without duplicates")
				{
					GIVEN("a wild Pokemon with four possible non-duplicate moves")
					{
						std::vector<MoveLearnEntry> entries{
							{.mMoveID = toMoveID(BuiltinMoveID::Facade), .mLevelRequirement = 10},
							{.mMoveID = toMoveID(BuiltinMoveID::HydroSteam), .mLevelRequirement = 2},
							{.mMoveID = toMoveID(BuiltinMoveID::KarateChop), .mLevelRequirement = 8},
							{.mMoveID = toMoveID(BuiltinMoveID::Pound), .mLevelRequirement = 1},
						};

						meta.mEntries = entries;

						WHEN("the encounter has a full set of valid level up moves")
						{
							pokemonEncounterLevel = 10;

							std::expected<void, MoveError> result{
								fillWithValidMoves(moveIDs, moveRegistry, meta, encounter, pokemonEncounterLevel),
							};

							std::vector<MoveID> expected{
								toMoveID(BuiltinMoveID::Pound),
								toMoveID(BuiltinMoveID::HydroSteam),
								toMoveID(BuiltinMoveID::KarateChop),
								toMoveID(BuiltinMoveID::Facade),
							};

							THEN("all the moves are assigned to the moveIDs")
							{
								REQUIRE(result.has_value());
								CHECK(std::ranges::equal(moveIDs, expected));
							}
						}

						WHEN("the encounter has only two valid level up moves")
						{
							pokemonEncounterLevel = 3;

							std::expected<void, MoveError> result{
								fillWithValidMoves(moveIDs, moveRegistry, meta, encounter, pokemonEncounterLevel),
							};

							std::vector<MoveID> expected{
								toMoveID(BuiltinMoveID::Pound),
								toMoveID(BuiltinMoveID::HydroSteam),
								NO_MOVE_ID,
								NO_MOVE_ID,
							};

							THEN("only the moves that meet the level requirement are assigned to the moveIDs")
							{
								REQUIRE(result.has_value());
								CHECK(std::ranges::equal(moveIDs, expected));
							}
						}
					}

					GIVEN("a wild Pokemon with four possible moves that includes duplicates")
					{
						std::vector<MoveLearnEntry> entries{
							{.mMoveID = toMoveID(BuiltinMoveID::Facade), .mLevelRequirement = 10},
							{.mMoveID = toMoveID(BuiltinMoveID::Pound), .mLevelRequirement = 2},
							{.mMoveID = toMoveID(BuiltinMoveID::KarateChop), .mLevelRequirement = 8},
							{.mMoveID = toMoveID(BuiltinMoveID::Pound), .mLevelRequirement = 1},
						};

						meta.mEntries = entries;

						WHEN("the encounter has a full set of valid level up moves")
						{
							pokemonEncounterLevel = 10;

							std::expected<void, MoveError> result{
								fillWithValidMoves(moveIDs, moveRegistry, meta, encounter, pokemonEncounterLevel),
							};

							std::vector<MoveID> expected{
								toMoveID(BuiltinMoveID::Pound),
								toMoveID(BuiltinMoveID::KarateChop),
								toMoveID(BuiltinMoveID::Facade),
								NO_MOVE_ID,
							};

							THEN("all unique moves are assigned to moveIDs and duplicate moves are set to NO_MOVE_ID and moved to the "
								 "end")
							{
								REQUIRE(result.has_value());
								CHECK(std::ranges::equal(moveIDs, expected));
							}
						}

						WHEN("the encounter has only two valid level up moves")
						{
							pokemonEncounterLevel = 3;

							std::expected<void, MoveError> result{
								fillWithValidMoves(moveIDs, moveRegistry, meta, encounter, pokemonEncounterLevel),
							};

							std::vector<MoveID> expected{
								toMoveID(BuiltinMoveID::Pound),
								NO_MOVE_ID,
								NO_MOVE_ID,
								NO_MOVE_ID,
							};

							THEN("the unique move is added and the duplicate is set to NO_MOVE_ID and moved to the end")
							{
								REQUIRE(result.has_value());
								CHECK(std::ranges::equal(moveIDs, expected));
							}
						}
					}
				}

				GIVEN("with/without egg moves")
				{
					GIVEN("a wild Pokemon with four non-duplicate moves that have egg requirements")
					{
						std::vector<MoveLearnEntry> entries{
							{.mMoveID = toMoveID(BuiltinMoveID::Facade), .mLevelRequirement = 10},
							{
								.mMoveID = toMoveID(BuiltinMoveID::HydroSteam),
								.mLevelRequirement = 2,
								.mAcquisitionMethod = AcquisitionMethod::Egg,
							},
							{.mMoveID = toMoveID(BuiltinMoveID::KarateChop), .mLevelRequirement = 8},
							{.mMoveID = toMoveID(BuiltinMoveID::Pound), .mLevelRequirement = 1},
						};

						meta.mEntries = entries;

						WHEN("the encounter has a full set of valid level up moves")
						{
							pokemonEncounterLevel = 10;

							std::expected<void, MoveError> result{
								fillWithValidMoves(moveIDs, moveRegistry, meta, encounter, pokemonEncounterLevel),
							};

							std::vector<MoveID> expected{
								toMoveID(BuiltinMoveID::Pound),
								toMoveID(BuiltinMoveID::KarateChop),
								toMoveID(BuiltinMoveID::Facade),
								NO_MOVE_ID,
							};

							THEN("all the level up only moves are assigned to the moveIDs and the egg move is ignored")
							{
								REQUIRE(result.has_value());
								CHECK(std::ranges::equal(moveIDs, expected));
							}
						}

						WHEN("the encounter has only two valid level up moves")
						{
							pokemonEncounterLevel = 3;

							std::expected<void, MoveError> result{
								fillWithValidMoves(moveIDs, moveRegistry, meta, encounter, pokemonEncounterLevel),
							};

							std::vector<MoveID> expected{
								toMoveID(BuiltinMoveID::Pound),
								NO_MOVE_ID,
								NO_MOVE_ID,
								NO_MOVE_ID,
							};

							THEN("the level up move is assigned to the moveIDs and the egg move is ignored")
							{
								REQUIRE(result.has_value());
								CHECK(std::ranges::equal(moveIDs, expected));
							}
						}
					}

					GIVEN("a wild Pokemon with four possible duplicate moves that may have egg requirements")
					{
						std::vector<MoveLearnEntry> entries{
							{.mMoveID = toMoveID(BuiltinMoveID::Facade), .mLevelRequirement = 10},
							{.mMoveID = toMoveID(BuiltinMoveID::Pound), .mLevelRequirement = 2},
							{
								.mMoveID = toMoveID(BuiltinMoveID::KarateChop),
								.mLevelRequirement = 8,
								.mAcquisitionMethod = AcquisitionMethod::Egg,
							},
							{.mMoveID = toMoveID(BuiltinMoveID::Pound), .mLevelRequirement = 1},
						};

						meta.mEntries = entries;

						WHEN("the encounter has a full set of valid level up moves")
						{
							pokemonEncounterLevel = 10;

							std::expected<void, MoveError> result{
								fillWithValidMoves(moveIDs, moveRegistry, meta, encounter, pokemonEncounterLevel),
							};

							std::vector<MoveID> expected{
								toMoveID(BuiltinMoveID::Pound),
								toMoveID(BuiltinMoveID::Facade),
								NO_MOVE_ID,
								NO_MOVE_ID,
							};

							THEN("all unique non-egg moves are assigned to moveIDs and duplicate moves are set to NO_MOVE_ID and moved "
								 "to "
								 "the "
								 "end")
							{
								REQUIRE(result.has_value());
								CHECK(std::ranges::equal(moveIDs, expected));
							}
						}
					}
				}
			}

			GIVEN("five possible moves")
			{
				GIVEN("with/without duplicates")
				{
					GIVEN("a wild Pokemon with five possible non-duplicate moves")
					{
						std::vector<MoveLearnEntry> entries{
							{.mMoveID = toMoveID(BuiltinMoveID::Facade), .mLevelRequirement = 10},
							{.mMoveID = toMoveID(BuiltinMoveID::HydroSteam), .mLevelRequirement = 2},
							{.mMoveID = toMoveID(BuiltinMoveID::KarateChop), .mLevelRequirement = 8},
							{.mMoveID = toMoveID(BuiltinMoveID::Pound), .mLevelRequirement = 5},
							{.mMoveID = toMoveID(BuiltinMoveID::None), .mLevelRequirement = 1},
						};

						meta.mEntries = entries;

						WHEN("the encounter has a full set of valid level up moves")
						{
							pokemonEncounterLevel = 10;

							std::expected<void, MoveError> result{
								fillWithValidMoves(moveIDs, moveRegistry, meta, encounter, pokemonEncounterLevel),
							};

							std::vector<MoveID> expected{
								toMoveID(BuiltinMoveID::HydroSteam),
								toMoveID(BuiltinMoveID::Pound),
								toMoveID(BuiltinMoveID::KarateChop),
								toMoveID(BuiltinMoveID::Facade),
							};

							THEN("only the top 4 level requirement moves are assigned to the moveIDs")
							{
								REQUIRE(result.has_value());
								CHECK(std::ranges::equal(moveIDs, expected));
							}
						}
					}

					GIVEN("a wild Pokemon with five possible moves that includes duplicates")
					{
						std::vector<MoveLearnEntry> entries{
							{.mMoveID = toMoveID(BuiltinMoveID::Facade), .mLevelRequirement = 10},
							{.mMoveID = toMoveID(BuiltinMoveID::Pound), .mLevelRequirement = 2},
							{.mMoveID = toMoveID(BuiltinMoveID::KarateChop), .mLevelRequirement = 8},
							{.mMoveID = toMoveID(BuiltinMoveID::Pound), .mLevelRequirement = 2},
							{.mMoveID = toMoveID(BuiltinMoveID::HydroSteam), .mLevelRequirement = 1},
						};

						meta.mEntries = entries;

						WHEN("the encounter has a full set of valid level up moves")
						{
							pokemonEncounterLevel = 10;

							std::expected<void, MoveError> result{
								fillWithValidMoves(moveIDs, moveRegistry, meta, encounter, pokemonEncounterLevel),
							};

							std::vector<MoveID> expected{
								toMoveID(BuiltinMoveID::HydroSteam),
								toMoveID(BuiltinMoveID::Pound),
								toMoveID(BuiltinMoveID::KarateChop),
								toMoveID(BuiltinMoveID::Facade),
							};

							THEN("grabs the top 4 level requirement moves after filtering out the duplicates")
							{
								REQUIRE(result.has_value());
								CHECK(std::ranges::equal(moveIDs, expected));
							}
						}
					}
				}

				GIVEN("with/without egg moves")
				{
					GIVEN("a wild Pokemon with five possible non-duplicate moves that have egg requirements")
					{
						std::vector<MoveLearnEntry> entries{
							{.mMoveID = toMoveID(BuiltinMoveID::Facade), .mLevelRequirement = 10},
							{
								.mMoveID = toMoveID(BuiltinMoveID::HydroSteam),
								.mLevelRequirement = 2,
								.mAcquisitionMethod = AcquisitionMethod::Egg,
							},
							{.mMoveID = toMoveID(BuiltinMoveID::KarateChop), .mLevelRequirement = 8},
							{.mMoveID = toMoveID(BuiltinMoveID::Pound), .mLevelRequirement = 3},
							{.mMoveID = toMoveID(BuiltinMoveID::None), .mLevelRequirement = 5},
						};

						meta.mEntries = entries;

						WHEN("the encounter has a full set of valid level up moves")
						{
							pokemonEncounterLevel = 10;

							std::expected<void, MoveError> result{
								fillWithValidMoves(moveIDs, moveRegistry, meta, encounter, pokemonEncounterLevel),
							};

							std::vector<MoveID> expected{
								toMoveID(BuiltinMoveID::Pound),
								toMoveID(BuiltinMoveID::KarateChop),
								toMoveID(BuiltinMoveID::Facade),
								NO_MOVE_ID,
							};

							THEN("it grabs the top 4 level requirement moves of level up method, and shifts any NO_MOVE_ID entries to "
								 "the "
								 "end")
							{
								REQUIRE(result.has_value());
								CHECK(std::ranges::equal(moveIDs, expected));
							}
						}
					}

					GIVEN("a wild Pokemon with five possible duplicate moves that may have egg requirements")
					{
						std::vector<MoveLearnEntry> entries{
							{.mMoveID = toMoveID(BuiltinMoveID::Facade), .mLevelRequirement = 10},
							{.mMoveID = toMoveID(BuiltinMoveID::Pound), .mLevelRequirement = 2},
							{
								.mMoveID = toMoveID(BuiltinMoveID::KarateChop),
								.mLevelRequirement = 8,
								.mAcquisitionMethod = AcquisitionMethod::Egg,
							},
							{.mMoveID = toMoveID(BuiltinMoveID::Pound), .mLevelRequirement = 1},
							{.mMoveID = toMoveID(BuiltinMoveID::HydroSteam), .mLevelRequirement = 6},
						};

						meta.mEntries = entries;

						WHEN("the encounter has a full set of valid level up moves")
						{
							pokemonEncounterLevel = 10;

							std::expected<void, MoveError> result{
								fillWithValidMoves(moveIDs, moveRegistry, meta, encounter, pokemonEncounterLevel),
							};

							std::vector<MoveID> expected{
								toMoveID(BuiltinMoveID::Pound),
								toMoveID(BuiltinMoveID::HydroSteam),
								toMoveID(BuiltinMoveID::Facade),
								NO_MOVE_ID,
							};

							THEN("all unique non-egg moves are assigned to moveIDs and duplicate moves are set to NO_MOVE_ID and moved "
								 "to "
								 "the end")
							{
								REQUIRE(result.has_value());
								CHECK(std::ranges::equal(moveIDs, expected));
							}
						}
					}
				}
			}

			GIVEN("invalid move metadata")
			{
				std::vector<MoveLearnEntry> entries{
					{.mMoveID = toMoveID(BuiltinMoveID::Facade), .mLevelRequirement = 10},
					{.mMoveID = MoveID{200}, .mLevelRequirement = 2},
					{
						.mMoveID = toMoveID(BuiltinMoveID::KarateChop),
						.mLevelRequirement = 8,
						.mAcquisitionMethod = AcquisitionMethod::Egg,
					},
					{.mMoveID = toMoveID(BuiltinMoveID::Pound), .mLevelRequirement = 1},
					{.mMoveID = toMoveID(BuiltinMoveID::HydroSteam), .mLevelRequirement = 6},
				};

				meta.mEntries = entries;

				WHEN("the encounter has a move that has invalid move metadata")
				{
					pokemonEncounterLevel = 10;

					std::expected<void, MoveError> result{
						fillWithValidMoves(moveIDs, moveRegistry, meta, encounter, pokemonEncounterLevel),
					};

					std::vector<MoveID> expected{
						NO_MOVE_ID,
						NO_MOVE_ID,
						NO_MOVE_ID,
						NO_MOVE_ID,
					};

					THEN("the move IDs are set to NO_MOVE_ID and an error is returned")
					{
						REQUIRE_FALSE(result.has_value());
						CHECK(std::ranges::equal(moveIDs, expected));
						CHECK((result.error() == MoveError::MISSING_MOVE_METADATA));
					}
				}
			}
		}

		GIVEN("an egg Pokemon")
		{
			encounter.mOrigin = PokemonOrigin::Egg;

			GIVEN("four possible moves")
			{
				GIVEN("with/without duplicates")
				{
					GIVEN("a wild Pokemon with four possible non-duplicate moves")
					{
						std::vector<MoveLearnEntry> entries{
							{.mMoveID = toMoveID(BuiltinMoveID::Facade), .mLevelRequirement = 10},
							{.mMoveID = toMoveID(BuiltinMoveID::HydroSteam), .mLevelRequirement = 2},
							{.mMoveID = toMoveID(BuiltinMoveID::KarateChop), .mLevelRequirement = 8},
							{.mMoveID = toMoveID(BuiltinMoveID::Pound), .mLevelRequirement = 1},
						};

						meta.mEntries = entries;

						WHEN("the encounter has a full set of valid level up moves")
						{
							pokemonEncounterLevel = 10;

							std::expected<void, MoveError> result{
								fillWithValidMoves(moveIDs, moveRegistry, meta, encounter, pokemonEncounterLevel),
							};

							std::vector<MoveID> expected{
								toMoveID(BuiltinMoveID::Pound),
								toMoveID(BuiltinMoveID::HydroSteam),
								toMoveID(BuiltinMoveID::KarateChop),
								toMoveID(BuiltinMoveID::Facade),
							};

							THEN("all the moves are assigned to the moveIDs")
							{
								REQUIRE(result.has_value());
								CHECK(std::ranges::equal(moveIDs, expected));
							}
						}

						WHEN("the encounter has only two valid level up moves")
						{
							pokemonEncounterLevel = 3;

							std::expected<void, MoveError> result{
								fillWithValidMoves(moveIDs, moveRegistry, meta, encounter, pokemonEncounterLevel),
							};

							std::vector<MoveID> expected{
								toMoveID(BuiltinMoveID::Pound),
								toMoveID(BuiltinMoveID::HydroSteam),
								NO_MOVE_ID,
								NO_MOVE_ID,
							};

							THEN("only the moves that meet the level requirement are assigned to the moveIDs")
							{
								REQUIRE(result.has_value());

								CHECK(std::ranges::equal(moveIDs, expected));
							}
						}
					}

					GIVEN("a wild Pokemon with four possible moves that includes duplicates")
					{
						std::vector<MoveLearnEntry> entries{
							{.mMoveID = toMoveID(BuiltinMoveID::Facade), .mLevelRequirement = 10},
							{.mMoveID = toMoveID(BuiltinMoveID::Pound), .mLevelRequirement = 2},
							{.mMoveID = toMoveID(BuiltinMoveID::KarateChop), .mLevelRequirement = 8},
							{.mMoveID = toMoveID(BuiltinMoveID::Pound), .mLevelRequirement = 1},
						};

						meta.mEntries = entries;

						WHEN("the encounter has a full set of valid level up moves")
						{
							pokemonEncounterLevel = 10;

							std::expected<void, MoveError> result{
								fillWithValidMoves(moveIDs, moveRegistry, meta, encounter, pokemonEncounterLevel),
							};

							std::vector<MoveID> expected{
								toMoveID(BuiltinMoveID::Pound),
								toMoveID(BuiltinMoveID::KarateChop),
								toMoveID(BuiltinMoveID::Facade),
								NO_MOVE_ID,
							};

							THEN("all unique moves are assigned to moveIDs and duplicate moves are set to NO_MOVE_ID and moved to the "
								 "end")
							{
								REQUIRE(result.has_value());
								CHECK(std::ranges::equal(moveIDs, expected));
							}
						}

						WHEN("the encounter has only two valid level up moves")
						{
							pokemonEncounterLevel = 3;

							std::expected<void, MoveError> result{
								fillWithValidMoves(moveIDs, moveRegistry, meta, encounter, pokemonEncounterLevel),
							};

							std::vector<MoveID> expected{
								toMoveID(BuiltinMoveID::Pound),
								NO_MOVE_ID,
								NO_MOVE_ID,
								NO_MOVE_ID,
							};

							THEN("the unique move is added and the duplicate is set to NO_MOVE_ID and moved to the end")
							{
								REQUIRE(result.has_value());

								CHECK(std::ranges::equal(moveIDs, expected));
							}
						}
					}
				}

				GIVEN("with/without egg moves")
				{
					GIVEN("a wild Pokemon with four non-duplicate moves that have egg requirements")
					{
						std::vector<MoveLearnEntry> entries{
							{.mMoveID = toMoveID(BuiltinMoveID::Facade), .mLevelRequirement = 10},
							{
								.mMoveID = toMoveID(BuiltinMoveID::HydroSteam),
								.mLevelRequirement = 2,
								.mAcquisitionMethod = AcquisitionMethod::Egg,
							},
							{.mMoveID = toMoveID(BuiltinMoveID::KarateChop), .mLevelRequirement = 8},
							{.mMoveID = toMoveID(BuiltinMoveID::Pound), .mLevelRequirement = 1},
						};

						meta.mEntries = entries;

						WHEN("the encounter has a full set of valid level up moves")
						{
							pokemonEncounterLevel = 10;

							std::expected<void, MoveError> result{
								fillWithValidMoves(moveIDs, moveRegistry, meta, encounter, pokemonEncounterLevel),
							};

							std::vector<MoveID> expected{
								toMoveID(BuiltinMoveID::HydroSteam),
								toMoveID(BuiltinMoveID::Pound),
								toMoveID(BuiltinMoveID::KarateChop),
								toMoveID(BuiltinMoveID::Facade),
							};

							THEN("all the level up and egg moves are assigned to the moveIDs")
							{
								REQUIRE(result.has_value());
								CHECK(std::ranges::equal(moveIDs, expected));
							}
						}

						WHEN("the encounter has only two valid level up moves")
						{
							pokemonEncounterLevel = 3;

							std::expected<void, MoveError> result{
								fillWithValidMoves(moveIDs, moveRegistry, meta, encounter, pokemonEncounterLevel),
							};

							std::vector<MoveID> expected{
								toMoveID(BuiltinMoveID::HydroSteam),
								toMoveID(BuiltinMoveID::Pound),
								NO_MOVE_ID,
								NO_MOVE_ID,
							};

							THEN("the level up and egg move is assigned to the moveIDs")
							{
								REQUIRE(result.has_value());
								CHECK(std::ranges::equal(moveIDs, expected));
							}
						}
					}

					GIVEN("a wild Pokemon with four possible duplicate moves that may have egg requirements")
					{
						std::vector<MoveLearnEntry> entries{
							{.mMoveID = toMoveID(BuiltinMoveID::Facade), .mLevelRequirement = 10},
							{.mMoveID = toMoveID(BuiltinMoveID::Pound), .mLevelRequirement = 2},
							{
								.mMoveID = toMoveID(BuiltinMoveID::KarateChop),
								.mLevelRequirement = 8,
								.mAcquisitionMethod = AcquisitionMethod::Egg,
							},
							{.mMoveID = toMoveID(BuiltinMoveID::Pound), .mLevelRequirement = 1},
						};

						meta.mEntries = entries;

						WHEN("the encounter has a full set of valid level up moves")
						{
							pokemonEncounterLevel = 10;

							std::expected<void, MoveError> result{
								fillWithValidMoves(moveIDs, moveRegistry, meta, encounter, pokemonEncounterLevel),
							};

							std::vector<MoveID> expected{
								toMoveID(BuiltinMoveID::KarateChop),
								toMoveID(BuiltinMoveID::Pound),
								toMoveID(BuiltinMoveID::Facade),
								NO_MOVE_ID,
							};

							THEN("all unique level-up and egg moves are assigned to moveIDs and duplicate moves are set to NO_MOVE_ID "
								 "and "
								 "moved to the end")
							{
								REQUIRE(result.has_value());
								CHECK(std::ranges::equal(moveIDs, expected));
							}
						}
					}
				}
			}

			GIVEN("five possible moves")
			{
				GIVEN("with/without duplicates")
				{
					GIVEN("a wild Pokemon with five possible non-duplicate moves")
					{
						std::vector<MoveLearnEntry> entries{
							{.mMoveID = toMoveID(BuiltinMoveID::Facade), .mLevelRequirement = 10},
							{.mMoveID = toMoveID(BuiltinMoveID::HydroSteam), .mLevelRequirement = 2},
							{.mMoveID = toMoveID(BuiltinMoveID::KarateChop), .mLevelRequirement = 8},
							{.mMoveID = toMoveID(BuiltinMoveID::Pound), .mLevelRequirement = 5},
							{.mMoveID = toMoveID(BuiltinMoveID::None), .mLevelRequirement = 1},
						};

						meta.mEntries = entries;

						WHEN("the encounter has a full set of valid level up moves")
						{
							pokemonEncounterLevel = 10;

							std::expected<void, MoveError> result{
								fillWithValidMoves(moveIDs, moveRegistry, meta, encounter, pokemonEncounterLevel),
							};

							std::vector<MoveID> expected{
								toMoveID(BuiltinMoveID::HydroSteam),
								toMoveID(BuiltinMoveID::Pound),
								toMoveID(BuiltinMoveID::KarateChop),
								toMoveID(BuiltinMoveID::Facade),
							};

							THEN("only the top 4 level requirement moves are assigned to the moveIDs")
							{
								REQUIRE(result.has_value());
								CHECK(std::ranges::equal(moveIDs, expected));
							}
						}
					}

					GIVEN("a wild Pokemon with five possible moves that includes duplicates")
					{
						std::vector<MoveLearnEntry> entries{
							{.mMoveID = toMoveID(BuiltinMoveID::Facade), .mLevelRequirement = 10},
							{.mMoveID = toMoveID(BuiltinMoveID::Pound), .mLevelRequirement = 2},
							{.mMoveID = toMoveID(BuiltinMoveID::KarateChop), .mLevelRequirement = 8},
							{.mMoveID = toMoveID(BuiltinMoveID::Pound), .mLevelRequirement = 2},
							{.mMoveID = toMoveID(BuiltinMoveID::HydroSteam), .mLevelRequirement = 1},
						};

						meta.mEntries = entries;

						WHEN("the encounter has a full set of valid level up moves")
						{
							pokemonEncounterLevel = 10;

							std::expected<void, MoveError> result{
								fillWithValidMoves(moveIDs, moveRegistry, meta, encounter, pokemonEncounterLevel),
							};

							std::vector<MoveID> expected{
								toMoveID(BuiltinMoveID::HydroSteam),
								toMoveID(BuiltinMoveID::Pound),
								toMoveID(BuiltinMoveID::KarateChop),
								toMoveID(BuiltinMoveID::Facade),
							};

							THEN("grabs the top 4 level requirement moves after filtering out the duplicates")
							{
								REQUIRE(result.has_value());
								CHECK(std::ranges::equal(moveIDs, expected));
							}
						}
					}
				}

				GIVEN("with/without egg moves")
				{
					GIVEN("a wild Pokemon with five possible non-duplicate moves that have egg requirements")
					{
						std::vector<MoveLearnEntry> entries{
							{.mMoveID = toMoveID(BuiltinMoveID::Facade), .mLevelRequirement = 10},
							{
								.mMoveID = toMoveID(BuiltinMoveID::HydroSteam),
								.mLevelRequirement = 2,
								.mAcquisitionMethod = AcquisitionMethod::Egg,
							},
							{.mMoveID = toMoveID(BuiltinMoveID::KarateChop), .mLevelRequirement = 8},
							{.mMoveID = toMoveID(BuiltinMoveID::Pound), .mLevelRequirement = 3},
							{.mMoveID = toMoveID(BuiltinMoveID::None), .mLevelRequirement = 5},
						};

						meta.mEntries = entries;

						WHEN("the encounter has a full set of valid level up moves")
						{
							pokemonEncounterLevel = 10;

							std::expected<void, MoveError> result{
								fillWithValidMoves(moveIDs, moveRegistry, meta, encounter, pokemonEncounterLevel),
							};

							std::vector<MoveID> expected{
								toMoveID(BuiltinMoveID::HydroSteam),
								toMoveID(BuiltinMoveID::Pound),
								toMoveID(BuiltinMoveID::KarateChop),
								toMoveID(BuiltinMoveID::Facade),
							};

							THEN("it grabs the top 4 level requirement moves of level up and egg methods")
							{
								REQUIRE(result.has_value());
								CHECK(std::ranges::equal(moveIDs, expected));
							}
						}
					}

					GIVEN("a wild Pokemon with five possible duplicate moves that may have egg requirements")
					{
						std::vector<MoveLearnEntry> entries{
							{.mMoveID = toMoveID(BuiltinMoveID::Facade), .mLevelRequirement = 10},
							{.mMoveID = toMoveID(BuiltinMoveID::Pound), .mLevelRequirement = 2},
							{
								.mMoveID = toMoveID(BuiltinMoveID::KarateChop),
								.mLevelRequirement = 8,
								.mAcquisitionMethod = AcquisitionMethod::Egg,
							},
							{.mMoveID = toMoveID(BuiltinMoveID::Pound), .mLevelRequirement = 1},
							{.mMoveID = toMoveID(BuiltinMoveID::KarateChop), .mLevelRequirement = 6},
						};

						meta.mEntries = entries;

						WHEN("the encounter has a full set of valid level up moves")
						{
							pokemonEncounterLevel = 10;

							std::expected<void, MoveError> result{
								fillWithValidMoves(moveIDs, moveRegistry, meta, encounter, pokemonEncounterLevel),
							};

							std::vector<MoveID> expected{
								toMoveID(BuiltinMoveID::KarateChop),
								toMoveID(BuiltinMoveID::Pound),
								toMoveID(BuiltinMoveID::Facade),
								NO_MOVE_ID,
							};

							THEN("all unique level-up and egg moves are assigned to moveIDs and duplicate moves are set to NO_MOVE_ID "
								 "and "
								 "moved to the end")
							{
								REQUIRE(result.has_value());
								CHECK(std::ranges::equal(moveIDs, expected));
							}
						}
					}
				}
			}

			GIVEN("invalid move metadata")
			{
				std::vector<MoveLearnEntry> entries{
					{.mMoveID = toMoveID(BuiltinMoveID::Facade), .mLevelRequirement = 10},
					{.mMoveID = toMoveID(BuiltinMoveID::KarateChop), .mLevelRequirement = 2},
					{
						.mMoveID = MoveID{200},
						.mLevelRequirement = 8,
						.mAcquisitionMethod = AcquisitionMethod::Egg,
					},
					{.mMoveID = toMoveID(BuiltinMoveID::Pound), .mLevelRequirement = 1},
					{.mMoveID = toMoveID(BuiltinMoveID::HydroSteam), .mLevelRequirement = 6},
				};

				meta.mEntries = entries;

				WHEN("the encounter has a move that has invalid move metadata")
				{
					pokemonEncounterLevel = 10;

					std::expected<void, MoveError> result{
						fillWithValidMoves(moveIDs, moveRegistry, meta, encounter, pokemonEncounterLevel),
					};

					std::vector<MoveID> expected{
						NO_MOVE_ID,
						NO_MOVE_ID,
						NO_MOVE_ID,
						NO_MOVE_ID,
					};

					THEN("the move IDs are set to NO_MOVE_ID and an error is returned")
					{
						REQUIRE_FALSE(result.has_value());
						CHECK(std::ranges::equal(moveIDs, expected));
						CHECK((result.error() == MoveError::MISSING_MOVE_METADATA));
					}
				}
			}
		}
	}

	GIVEN("getValidPokemonIVs")
	{
		std::array<us, POKEMON_STAT_COUNT> validPokemonIVs{};
		CallableRandomSource<std::size_t, SequenceRandom<std::size_t>> indexSource{SequenceRandom<std::size_t>{5, 8, 20, 25, 14, 31}};
		randomSources.mIndex = indexSource;

		WHEN("no passed in Pokemon IVs")
		{
			std::expected<void, PokemonError> result{getValidPokemonIVs({}, validPokemonIVs, randomSources)};

			THEN("it passes and initializes random ivs")
			{
				REQUIRE(result.has_value());

				std::array<us, POKEMON_STAT_COUNT> expectedIVs{5, 8, 20, 25, 14, 31};

				CHECK(std::ranges::equal(validPokemonIVs, expectedIVs));
			}
		}

		GIVEN("a span of Pokemon IVs")
		{
			WHEN("calling with valid Pokemon IVs")
			{
				std::array<us, POKEMON_STAT_COUNT> pokemonIVs{25, 2, 15, 30, 14, 24};

				std::expected<void, PokemonError> result{getValidPokemonIVs(pokemonIVs, validPokemonIVs, randomSources)};

				THEN("it successfully validates and puts the data in the validItemIDs")
				{
					REQUIRE(result.has_value());
					CHECK(std::ranges::equal(pokemonIVs, validPokemonIVs));
				}
			}

			WHEN("calling with an invalid Pokemon IV")
			{
				std::array<us, POKEMON_STAT_COUNT> pokemonIVs{25, 2, 15, 30, 14, 32};

				std::expected<void, PokemonError> result{getValidPokemonIVs(pokemonIVs, validPokemonIVs, randomSources)};

				THEN("it successfully invalidates and returns an error")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error() == PokemonError::INVALID_IV_ARRAY));
					CHECK(std::ranges::all_of(validPokemonIVs, [](const us pokemonIV) { return pokemonIV == 0; }));
				}
			}
		}
	}

	GIVEN("getValidPokemonEVs")
	{
		std::array<us, POKEMON_STAT_COUNT> validPokemonEVs{};

		WHEN("no passed in Pokemon EVs")
		{
			std::expected<void, PokemonError> result{getValidPokemonEVs({}, validPokemonEVs)};

			THEN("it passes and initializes all evs to 0")
			{
				CHECK(result.has_value());
				CHECK(std::ranges::all_of(validPokemonEVs, [](const us pokemonEV) { return pokemonEV == 0; }));
			}
		}

		GIVEN("a span of Pokemon EVs")
		{
			WHEN("calling with valid Pokemon EVs")
			{
				std::array<us, POKEMON_STAT_COUNT> pokemonEVs{25, 2, 15, 30, 14, 24};

				std::expected<void, PokemonError> result{getValidPokemonEVs(pokemonEVs, validPokemonEVs)};

				THEN("it successfully validates and puts the data in the validItemIDs")
				{
					REQUIRE(result.has_value());
					CHECK(std::ranges::equal(pokemonEVs, validPokemonEVs));
				}
			}

			WHEN("calling with an single invalid Pokemon EV")
			{
				std::array<us, POKEMON_STAT_COUNT> pokemonEVs{25, 2, 15, 30, 14, 253};

				std::expected<void, PokemonError> result{getValidPokemonEVs(pokemonEVs, validPokemonEVs)};

				THEN("it successfully invalidates and returns an error")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error() == PokemonError::INVALID_EV_ARRAY));
					CHECK(std::ranges::all_of(validPokemonEVs, [](const us pokemonEV) { return pokemonEV == 0; }));
				}
			}

			WHEN("calling with an total sum invalid Pokemon EV")
			{
				std::array<us, POKEMON_STAT_COUNT> pokemonEVs{28, 20, 50, 150, 11, 252};

				std::expected<void, PokemonError> result{getValidPokemonEVs(pokemonEVs, validPokemonEVs)};

				THEN("it successfully invalidates and returns an error")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error() == PokemonError::INVALID_EV_ARRAY));
					CHECK(std::ranges::all_of(validPokemonEVs, [](const us pokemonEV) { return pokemonEV == 0; }));
				}
			}
		}
	}

	GIVEN("printPokemonWithNames")
	{
		GIVEN("valid registries")
		{
			Pokemon pokemon{
    			makePokemon({
    				.mMetadata = {
                        .mName = "TestMon",
        				.mBaseStats = {
        					.mMaxHealth = 150,
        					.mAttack = 101,
        					.mDefense = 102,
        					.mSpAttack = 103,
        					.mSpDefense = 104,
        					.mSpeed = 105,
        				},
        				.mTypeIDs = {toTypeID(BuiltinTypeID::Fire), toTypeID(BuiltinTypeID::Bug)},
                    },
                    .mMoveMetas {
                        {
                            {
                                .mMoveID = toMoveID(BuiltinMoveID::Pound),
                                .mPPMaxAmount = 15,
                                .mPPDefaultAmount = 5,
                            },
                            {
                                .mMoveID = toMoveID(BuiltinMoveID::KarateChop),
                                .mPPMaxAmount = 20,
                                .mPPDefaultAmount = 10,
                           	},
                            {
                                .mMoveID = toMoveID(BuiltinMoveID::Facade),
                                .mPPMaxAmount = 25,
                                .mPPDefaultAmount = 15,
                            },
                            {
                                .mMoveID = toMoveID(BuiltinMoveID::HydroSteam),
                                .mPPMaxAmount = 30,
                                .mPPDefaultAmount = 20,
                            },
                        },
                    },
    				.mStatusIDs
    				= {toStatusID(BuiltinStatusID::Poison), toStatusID(BuiltinStatusID::Toxic), toStatusID(BuiltinStatusID::Paralysis),
    				   toStatusID(BuiltinStatusID::Sleep), toStatusID(BuiltinStatusID::Freeze),},
    				.mAbilityEntries = {{{.mAbilityID = toAbilityID(BuiltinAbilityID::AirLock), .mClassification = AbilityClassification::Standard}}},
    				.mItemIDs = {toItemID(BuiltinItemID::CheriBerry)},
    				.mNatureIDs = {toNatureID(BuiltinNatureID::Hardy)},
    				.mHealth = 150,
    				.mLevel = 50,
    			}),
    		};
			WHEN("the Pokemon is written to a stream")
			{
				std::ostringstream output;
				printPokemonWithNames(output, pokemon, provider);

				THEN("all public state is written with numeric identifiers and PP")
				{
					std::string_view expected{
						"Pokemon {\n"
						"  Name: TestMon\n"
						"  Level: 50\n"
						"  Level Damage Factor: 22\n"
						"  Health: 150/210\n"
						"    IV: 0\n"
						"    EV: 0\n"
						"  Attack: 106\n"
						"    IV: 0\n"
						"    EV: 0\n"
						"  Defense: 107\n"
						"    IV: 0\n"
						"    EV: 0\n"
						"  Special Attack: 108\n"
						"    IV: 0\n"
						"    EV: 0\n"
						"  Special Defense: 109\n"
						"    IV: 0\n"
						"    EV: 0\n"
						"  Speed: 110\n"
						"    IV: 0\n"
						"    EV: 0\n"
						"  Types:\n"
						"    [0]:\n"
						"      ID: 2\n"
						"      Name: Fire\n"
						"    [1]:\n"
						"      ID: 12\n"
						"      Name: Bug\n"
						"  Natures:\n"
						"    [0]:\n"
						"      ID: 1\n"
						"      Name: Hardy\n"
						"  Abilities:\n"
						"    [0]:\n"
						"      ID: 15\n"
						"      Name: Air Lock\n"
						"      Classification: Standard\n"
						"  Items:\n"
						"    [0]:\n"
						"      ID: 2\n"
						"      Name: Cheri Berry\n"
						"  Non-Volatile Statuses:\n"
						"    [0]:\n"
						"      ID: 5\n"
						"      Name: Poison\n"
						"    [1]:\n"
						"      ID: 6\n"
						"      Name: Toxic\n"
						"    [2]:\n"
						"      ID: 1\n"
						"      Name: Paralysis\n"
						"    [3]:\n"
						"      ID: 3\n"
						"      Name: Sleep\n"
						"    [4]:\n"
						"      ID: 4\n"
						"      Name: Freeze\n"
						"  Moves:\n"
						"    [0]:\n"
						"      ID: 1\n"
						"      Name: Pound\n"
						"      PP: 5/15\n"
						"    [1]:\n"
						"      ID: 2\n"
						"      Name: Karate Chop\n"
						"      PP: 10/20\n"
						"    [2]:\n"
						"      ID: 13\n"
						"      Name: Facade\n"
						"      PP: 15/25\n"
						"    [3]:\n"
						"      ID: 14\n"
						"      Name: Hydro Steam\n"
						"      PP: 20/30\n"
						"}",
					};

					CHECK((output.str() == expected));
				}
			}
		}

		GIVEN("invalid type registry")
		{
			Pokemon pokemon{
    			makePokemon({
    				.mMetadata = {
                        .mName = "TestMon",
        				.mBaseStats = {
        					.mMaxHealth = 150,
        					.mAttack = 101,
        					.mDefense = 102,
        					.mSpAttack = 103,
        					.mSpDefense = 104,
        					.mSpeed = 105,
        				},
        				.mTypeIDs = {toTypeID(BuiltinTypeID::Fire), toTypeID(BuiltinTypeID::Bug)},
                    },
                    .mMoveMetas {
                        {
                            {
                                .mMoveID = toMoveID(BuiltinMoveID::Pound),
                                .mPPMaxAmount = 15,
                                .mPPDefaultAmount = 5,
                            },
                            {
                                .mMoveID = toMoveID(BuiltinMoveID::KarateChop),
                                .mPPMaxAmount = 20,
                                .mPPDefaultAmount = 10,
                           	},
                            {
                                .mMoveID = toMoveID(BuiltinMoveID::Facade),
                                .mPPMaxAmount = 25,
                                .mPPDefaultAmount = 15,
                            },
                            {
                                .mMoveID = toMoveID(BuiltinMoveID::HydroSteam),
                                .mPPMaxAmount = 30,
                                .mPPDefaultAmount = 20,
                            },
                        },
                    },
    				.mStatusIDs
    				= {toStatusID(BuiltinStatusID::Poison), toStatusID(BuiltinStatusID::Toxic), toStatusID(BuiltinStatusID::Paralysis),
    				   toStatusID(BuiltinStatusID::Sleep), toStatusID(BuiltinStatusID::Freeze),},
    				.mAbilityEntries = {{{.mAbilityID = toAbilityID(BuiltinAbilityID::AirLock), .mClassification = AbilityClassification::Standard}}},
    				.mItemIDs = {toItemID(BuiltinItemID::CheriBerry)},
    				.mNatureIDs = {toNatureID(BuiltinNatureID::Hardy)},
    				.mHealth = 150,
    				.mLevel = 50,
    			}),
    		};

			RegistryProvider nullProvider{getNullTypeRegistryProvider()};

			WHEN("the Pokemon is written to a stream")
			{
				std::ostringstream output;
				printPokemonWithNames(output, pokemon, nullProvider);

				THEN("all public state is written with numeric identifiers and PP")
				{
					std::string_view expected{
						"Pokemon {\n"
						"  Name: TestMon\n"
						"  Level: 50\n"
						"  Level Damage Factor: 22\n"
						"  Health: 150/210\n"
						"    IV: 0\n"
						"    EV: 0\n"
						"  Attack: 106\n"
						"    IV: 0\n"
						"    EV: 0\n"
						"  Defense: 107\n"
						"    IV: 0\n"
						"    EV: 0\n"
						"  Special Attack: 108\n"
						"    IV: 0\n"
						"    EV: 0\n"
						"  Special Defense: 109\n"
						"    IV: 0\n"
						"    EV: 0\n"
						"  Speed: 110\n"
						"    IV: 0\n"
						"    EV: 0\n"
						"  Types:\n"
						"    [0]:\n"
						"      ID: 2\n"
						"      Name: <unregistered>\n"
						"    [1]:\n"
						"      ID: 12\n"
						"      Name: <unregistered>\n"
						"  Natures:\n"
						"    [0]:\n"
						"      ID: 1\n"
						"      Name: Hardy\n"
						"  Abilities:\n"
						"    [0]:\n"
						"      ID: 15\n"
						"      Name: Air Lock\n"
						"      Classification: Standard\n"
						"  Items:\n"
						"    [0]:\n"
						"      ID: 2\n"
						"      Name: Cheri Berry\n"
						"  Non-Volatile Statuses:\n"
						"    [0]:\n"
						"      ID: 5\n"
						"      Name: Poison\n"
						"    [1]:\n"
						"      ID: 6\n"
						"      Name: Toxic\n"
						"    [2]:\n"
						"      ID: 1\n"
						"      Name: Paralysis\n"
						"    [3]:\n"
						"      ID: 3\n"
						"      Name: Sleep\n"
						"    [4]:\n"
						"      ID: 4\n"
						"      Name: Freeze\n"
						"  Moves:\n"
						"    [0]:\n"
						"      ID: 1\n"
						"      Name: Pound\n"
						"      PP: 5/15\n"
						"    [1]:\n"
						"      ID: 2\n"
						"      Name: Karate Chop\n"
						"      PP: 10/20\n"
						"    [2]:\n"
						"      ID: 13\n"
						"      Name: Facade\n"
						"      PP: 15/25\n"
						"    [3]:\n"
						"      ID: 14\n"
						"      Name: Hydro Steam\n"
						"      PP: 20/30\n"
						"}",
					};

					CHECK((output.str() == expected));
				}
			}
		}

		GIVEN("invalid nautre registry")
		{
			Pokemon pokemon{
    			makePokemon({
    				.mMetadata = {
                        .mName = "TestMon",
        				.mBaseStats = {
        					.mMaxHealth = 150,
        					.mAttack = 101,
        					.mDefense = 102,
        					.mSpAttack = 103,
        					.mSpDefense = 104,
        					.mSpeed = 105,
        				},
        				.mTypeIDs = {toTypeID(BuiltinTypeID::Fire), toTypeID(BuiltinTypeID::Bug)},
                    },
                    .mMoveMetas {
                        {
                            {
                                .mMoveID = toMoveID(BuiltinMoveID::Pound),
                                .mPPMaxAmount = 15,
                                .mPPDefaultAmount = 5,
                            },
                            {
                                .mMoveID = toMoveID(BuiltinMoveID::KarateChop),
                                .mPPMaxAmount = 20,
                                .mPPDefaultAmount = 10,
                           	},
                            {
                                .mMoveID = toMoveID(BuiltinMoveID::Facade),
                                .mPPMaxAmount = 25,
                                .mPPDefaultAmount = 15,
                            },
                            {
                                .mMoveID = toMoveID(BuiltinMoveID::HydroSteam),
                                .mPPMaxAmount = 30,
                                .mPPDefaultAmount = 20,
                            },
                        },
                    },
    				.mStatusIDs
    				= {toStatusID(BuiltinStatusID::Poison), toStatusID(BuiltinStatusID::Toxic), toStatusID(BuiltinStatusID::Paralysis),
    				   toStatusID(BuiltinStatusID::Sleep), toStatusID(BuiltinStatusID::Freeze),},
    				.mAbilityEntries = {{{.mAbilityID = toAbilityID(BuiltinAbilityID::AirLock), .mClassification = AbilityClassification::Hidden}}},
    				.mItemIDs = {toItemID(BuiltinItemID::CheriBerry)},
    				.mNatureIDs = {toNatureID(BuiltinNatureID::Hardy)},
    				.mHealth = 150,
    				.mLevel = 50,
    			}),
    		};

			RegistryProvider nullProvider{getNullNatureRegistryProvider()};

			WHEN("the Pokemon is written to a stream")
			{
				std::ostringstream output;
				printPokemonWithNames(output, pokemon, nullProvider);

				THEN("all public state is written with numeric identifiers and PP")
				{
					std::string_view expected{
						"Pokemon {\n"
						"  Name: TestMon\n"
						"  Level: 50\n"
						"  Level Damage Factor: 22\n"
						"  Health: 150/210\n"
						"    IV: 0\n"
						"    EV: 0\n"
						"  Attack: 106\n"
						"    IV: 0\n"
						"    EV: 0\n"
						"  Defense: 107\n"
						"    IV: 0\n"
						"    EV: 0\n"
						"  Special Attack: 108\n"
						"    IV: 0\n"
						"    EV: 0\n"
						"  Special Defense: 109\n"
						"    IV: 0\n"
						"    EV: 0\n"
						"  Speed: 110\n"
						"    IV: 0\n"
						"    EV: 0\n"
						"  Types:\n"
						"    [0]:\n"
						"      ID: 2\n"
						"      Name: Fire\n"
						"    [1]:\n"
						"      ID: 12\n"
						"      Name: Bug\n"
						"  Natures:\n"
						"    [0]:\n"
						"      ID: 1\n"
						"      Name: <unregistered>\n"
						"  Abilities:\n"
						"    [0]:\n"
						"      ID: 15\n"
						"      Name: Air Lock\n"
						"      Classification: Hidden\n"
						"  Items:\n"
						"    [0]:\n"
						"      ID: 2\n"
						"      Name: Cheri Berry\n"
						"  Non-Volatile Statuses:\n"
						"    [0]:\n"
						"      ID: 5\n"
						"      Name: Poison\n"
						"    [1]:\n"
						"      ID: 6\n"
						"      Name: Toxic\n"
						"    [2]:\n"
						"      ID: 1\n"
						"      Name: Paralysis\n"
						"    [3]:\n"
						"      ID: 3\n"
						"      Name: Sleep\n"
						"    [4]:\n"
						"      ID: 4\n"
						"      Name: Freeze\n"
						"  Moves:\n"
						"    [0]:\n"
						"      ID: 1\n"
						"      Name: Pound\n"
						"      PP: 5/15\n"
						"    [1]:\n"
						"      ID: 2\n"
						"      Name: Karate Chop\n"
						"      PP: 10/20\n"
						"    [2]:\n"
						"      ID: 13\n"
						"      Name: Facade\n"
						"      PP: 15/25\n"
						"    [3]:\n"
						"      ID: 14\n"
						"      Name: Hydro Steam\n"
						"      PP: 20/30\n"
						"}",
					};

					CHECK((output.str() == expected));
				}
			}
		}

		GIVEN("invalid ability registry")
		{
			Pokemon pokemon{
    			makePokemon({
    				.mMetadata = {
                        .mName = "TestMon",
        				.mBaseStats = {
        					.mMaxHealth = 150,
        					.mAttack = 101,
        					.mDefense = 102,
        					.mSpAttack = 103,
        					.mSpDefense = 104,
        					.mSpeed = 105,
        				},
        				.mTypeIDs = {toTypeID(BuiltinTypeID::Fire), toTypeID(BuiltinTypeID::Bug)},
                    },
                    .mMoveMetas {
                        {
                            {
                                .mMoveID = toMoveID(BuiltinMoveID::Pound),
                                .mPPMaxAmount = 15,
                                .mPPDefaultAmount = 5,
                            },
                            {
                                .mMoveID = toMoveID(BuiltinMoveID::KarateChop),
                                .mPPMaxAmount = 20,
                                .mPPDefaultAmount = 10,
                           	},
                            {
                                .mMoveID = toMoveID(BuiltinMoveID::Facade),
                                .mPPMaxAmount = 25,
                                .mPPDefaultAmount = 15,
                            },
                            {
                                .mMoveID = toMoveID(BuiltinMoveID::HydroSteam),
                                .mPPMaxAmount = 30,
                                .mPPDefaultAmount = 20,
                            },
                        },
                    },
    				.mStatusIDs
    				= {toStatusID(BuiltinStatusID::Poison), toStatusID(BuiltinStatusID::Toxic), toStatusID(BuiltinStatusID::Paralysis),
    				   toStatusID(BuiltinStatusID::Sleep), toStatusID(BuiltinStatusID::Freeze),},
    				.mAbilityEntries = {{{.mAbilityID = toAbilityID(BuiltinAbilityID::AirLock), .mClassification = AbilityClassification::Hidden}}},
    				.mItemIDs = {toItemID(BuiltinItemID::CheriBerry)},
    				.mNatureIDs = {toNatureID(BuiltinNatureID::Hardy)},
    				.mHealth = 150,
    				.mLevel = 50,
    			}),
    		};

			RegistryProvider nullProvider{getNullAbilityRegistryProvider()};

			WHEN("the Pokemon is written to a stream")
			{
				std::ostringstream output;
				printPokemonWithNames(output, pokemon, nullProvider);

				THEN("all public state is written with numeric identifiers and PP")
				{
					std::string_view expected{
						"Pokemon {\n"
						"  Name: TestMon\n"
						"  Level: 50\n"
						"  Level Damage Factor: 22\n"
						"  Health: 150/210\n"
						"    IV: 0\n"
						"    EV: 0\n"
						"  Attack: 106\n"
						"    IV: 0\n"
						"    EV: 0\n"
						"  Defense: 107\n"
						"    IV: 0\n"
						"    EV: 0\n"
						"  Special Attack: 108\n"
						"    IV: 0\n"
						"    EV: 0\n"
						"  Special Defense: 109\n"
						"    IV: 0\n"
						"    EV: 0\n"
						"  Speed: 110\n"
						"    IV: 0\n"
						"    EV: 0\n"
						"  Types:\n"
						"    [0]:\n"
						"      ID: 2\n"
						"      Name: Fire\n"
						"    [1]:\n"
						"      ID: 12\n"
						"      Name: Bug\n"
						"  Natures:\n"
						"    [0]:\n"
						"      ID: 1\n"
						"      Name: Hardy\n"
						"  Abilities:\n"
						"    [0]:\n"
						"      ID: 15\n"
						"      Name: <unregistered>\n"
						"      Classification: Hidden\n"
						"  Items:\n"
						"    [0]:\n"
						"      ID: 2\n"
						"      Name: Cheri Berry\n"
						"  Non-Volatile Statuses:\n"
						"    [0]:\n"
						"      ID: 5\n"
						"      Name: Poison\n"
						"    [1]:\n"
						"      ID: 6\n"
						"      Name: Toxic\n"
						"    [2]:\n"
						"      ID: 1\n"
						"      Name: Paralysis\n"
						"    [3]:\n"
						"      ID: 3\n"
						"      Name: Sleep\n"
						"    [4]:\n"
						"      ID: 4\n"
						"      Name: Freeze\n"
						"  Moves:\n"
						"    [0]:\n"
						"      ID: 1\n"
						"      Name: Pound\n"
						"      PP: 5/15\n"
						"    [1]:\n"
						"      ID: 2\n"
						"      Name: Karate Chop\n"
						"      PP: 10/20\n"
						"    [2]:\n"
						"      ID: 13\n"
						"      Name: Facade\n"
						"      PP: 15/25\n"
						"    [3]:\n"
						"      ID: 14\n"
						"      Name: Hydro Steam\n"
						"      PP: 20/30\n"
						"}",
					};

					CHECK((output.str() == expected));
				}
			}
		}

		GIVEN("invalid item registry")
		{
			Pokemon pokemon{
    			makePokemon({
    				.mMetadata = {
                        .mName = "TestMon",
        				.mBaseStats = {
        					.mMaxHealth = 150,
        					.mAttack = 101,
        					.mDefense = 102,
        					.mSpAttack = 103,
        					.mSpDefense = 104,
        					.mSpeed = 105,
        				},
        				.mTypeIDs = {toTypeID(BuiltinTypeID::Fire), toTypeID(BuiltinTypeID::Bug)},
                    },
                    .mMoveMetas {
                        {
                            {
                                .mMoveID = toMoveID(BuiltinMoveID::Pound),
                                .mPPMaxAmount = 15,
                                .mPPDefaultAmount = 5,
                            },
                            {
                                .mMoveID = toMoveID(BuiltinMoveID::KarateChop),
                                .mPPMaxAmount = 20,
                                .mPPDefaultAmount = 10,
                           	},
                            {
                                .mMoveID = toMoveID(BuiltinMoveID::Facade),
                                .mPPMaxAmount = 25,
                                .mPPDefaultAmount = 15,
                            },
                            {
                                .mMoveID = toMoveID(BuiltinMoveID::HydroSteam),
                                .mPPMaxAmount = 30,
                                .mPPDefaultAmount = 20,
                            },
                        },
                    },
    				.mStatusIDs
    				= {toStatusID(BuiltinStatusID::Poison), toStatusID(BuiltinStatusID::Toxic), toStatusID(BuiltinStatusID::Paralysis),
    				   toStatusID(BuiltinStatusID::Sleep), toStatusID(BuiltinStatusID::Freeze),},
    				.mAbilityEntries = {{{.mAbilityID = toAbilityID(BuiltinAbilityID::AirLock), .mClassification = AbilityClassification::Standard}}},
    				.mItemIDs = {toItemID(BuiltinItemID::CheriBerry)},
    				.mNatureIDs = {toNatureID(BuiltinNatureID::Hardy)},
    				.mHealth = 150,
    				.mLevel = 50,
    			}),
    		};

			RegistryProvider nullProvider{getNullItemRegistryProvider()};

			WHEN("the Pokemon is written to a stream")
			{
				std::ostringstream output;
				printPokemonWithNames(output, pokemon, nullProvider);

				THEN("all public state is written with numeric identifiers and PP")
				{
					std::string_view expected{
						"Pokemon {\n"
						"  Name: TestMon\n"
						"  Level: 50\n"
						"  Level Damage Factor: 22\n"
						"  Health: 150/210\n"
						"    IV: 0\n"
						"    EV: 0\n"
						"  Attack: 106\n"
						"    IV: 0\n"
						"    EV: 0\n"
						"  Defense: 107\n"
						"    IV: 0\n"
						"    EV: 0\n"
						"  Special Attack: 108\n"
						"    IV: 0\n"
						"    EV: 0\n"
						"  Special Defense: 109\n"
						"    IV: 0\n"
						"    EV: 0\n"
						"  Speed: 110\n"
						"    IV: 0\n"
						"    EV: 0\n"
						"  Types:\n"
						"    [0]:\n"
						"      ID: 2\n"
						"      Name: Fire\n"
						"    [1]:\n"
						"      ID: 12\n"
						"      Name: Bug\n"
						"  Natures:\n"
						"    [0]:\n"
						"      ID: 1\n"
						"      Name: Hardy\n"
						"  Abilities:\n"
						"    [0]:\n"
						"      ID: 15\n"
						"      Name: Air Lock\n"
						"      Classification: Standard\n"
						"  Items:\n"
						"    [0]:\n"
						"      ID: 2\n"
						"      Name: <unregistered>\n"
						"  Non-Volatile Statuses:\n"
						"    [0]:\n"
						"      ID: 5\n"
						"      Name: Poison\n"
						"    [1]:\n"
						"      ID: 6\n"
						"      Name: Toxic\n"
						"    [2]:\n"
						"      ID: 1\n"
						"      Name: Paralysis\n"
						"    [3]:\n"
						"      ID: 3\n"
						"      Name: Sleep\n"
						"    [4]:\n"
						"      ID: 4\n"
						"      Name: Freeze\n"
						"  Moves:\n"
						"    [0]:\n"
						"      ID: 1\n"
						"      Name: Pound\n"
						"      PP: 5/15\n"
						"    [1]:\n"
						"      ID: 2\n"
						"      Name: Karate Chop\n"
						"      PP: 10/20\n"
						"    [2]:\n"
						"      ID: 13\n"
						"      Name: Facade\n"
						"      PP: 15/25\n"
						"    [3]:\n"
						"      ID: 14\n"
						"      Name: Hydro Steam\n"
						"      PP: 20/30\n"
						"}",
					};

					CHECK((output.str() == expected));
				}
			}
		}

		GIVEN("invalid status registry")
		{
			Pokemon pokemon{
    			makePokemon({
    				.mMetadata = {
                        .mName = "TestMon",
        				.mBaseStats = {
        					.mMaxHealth = 150,
        					.mAttack = 101,
        					.mDefense = 102,
        					.mSpAttack = 103,
        					.mSpDefense = 104,
        					.mSpeed = 105,
        				},
        				.mTypeIDs = {toTypeID(BuiltinTypeID::Fire), toTypeID(BuiltinTypeID::Bug)},
                    },
                    .mMoveMetas {
                        {
                            {
                                .mMoveID = toMoveID(BuiltinMoveID::Pound),
                                .mPPMaxAmount = 15,
                                .mPPDefaultAmount = 5,
                            },
                            {
                                .mMoveID = toMoveID(BuiltinMoveID::KarateChop),
                                .mPPMaxAmount = 20,
                                .mPPDefaultAmount = 10,
                           	},
                            {
                                .mMoveID = toMoveID(BuiltinMoveID::Facade),
                                .mPPMaxAmount = 25,
                                .mPPDefaultAmount = 15,
                            },
                            {
                                .mMoveID = toMoveID(BuiltinMoveID::HydroSteam),
                                .mPPMaxAmount = 30,
                                .mPPDefaultAmount = 20,
                            },
                        },
                    },
    				.mStatusIDs
    				= {toStatusID(BuiltinStatusID::Poison), toStatusID(BuiltinStatusID::Toxic), toStatusID(BuiltinStatusID::Paralysis),
    				   toStatusID(BuiltinStatusID::Sleep), toStatusID(BuiltinStatusID::Freeze),},
    				.mAbilityEntries = {{{.mAbilityID = toAbilityID(BuiltinAbilityID::AirLock), .mClassification = AbilityClassification::Standard}}},
    				.mItemIDs = {toItemID(BuiltinItemID::CheriBerry)},
    				.mNatureIDs = {toNatureID(BuiltinNatureID::Hardy)},
    				.mHealth = 150,
    				.mLevel = 50,
    			}),
    		};

			RegistryProvider nullProvider{getNullStatusRegistryProvider()};

			WHEN("the Pokemon is written to a stream")
			{
				std::ostringstream output;
				printPokemonWithNames(output, pokemon, nullProvider);

				THEN("all public state is written with numeric identifiers and PP")
				{
					std::string_view expected{
						"Pokemon {\n"
						"  Name: TestMon\n"
						"  Level: 50\n"
						"  Level Damage Factor: 22\n"
						"  Health: 150/210\n"
						"    IV: 0\n"
						"    EV: 0\n"
						"  Attack: 106\n"
						"    IV: 0\n"
						"    EV: 0\n"
						"  Defense: 107\n"
						"    IV: 0\n"
						"    EV: 0\n"
						"  Special Attack: 108\n"
						"    IV: 0\n"
						"    EV: 0\n"
						"  Special Defense: 109\n"
						"    IV: 0\n"
						"    EV: 0\n"
						"  Speed: 110\n"
						"    IV: 0\n"
						"    EV: 0\n"
						"  Types:\n"
						"    [0]:\n"
						"      ID: 2\n"
						"      Name: Fire\n"
						"    [1]:\n"
						"      ID: 12\n"
						"      Name: Bug\n"
						"  Natures:\n"
						"    [0]:\n"
						"      ID: 1\n"
						"      Name: Hardy\n"
						"  Abilities:\n"
						"    [0]:\n"
						"      ID: 15\n"
						"      Name: Air Lock\n"
						"      Classification: Standard\n"
						"  Items:\n"
						"    [0]:\n"
						"      ID: 2\n"
						"      Name: Cheri Berry\n"
						"  Non-Volatile Statuses:\n"
						"    [0]:\n"
						"      ID: 5\n"
						"      Name: <unregistered>\n"
						"    [1]:\n"
						"      ID: 6\n"
						"      Name: <unregistered>\n"
						"    [2]:\n"
						"      ID: 1\n"
						"      Name: <unregistered>\n"
						"    [3]:\n"
						"      ID: 3\n"
						"      Name: <unregistered>\n"
						"    [4]:\n"
						"      ID: 4\n"
						"      Name: <unregistered>\n"
						"  Moves:\n"
						"    [0]:\n"
						"      ID: 1\n"
						"      Name: Pound\n"
						"      PP: 5/15\n"
						"    [1]:\n"
						"      ID: 2\n"
						"      Name: Karate Chop\n"
						"      PP: 10/20\n"
						"    [2]:\n"
						"      ID: 13\n"
						"      Name: Facade\n"
						"      PP: 15/25\n"
						"    [3]:\n"
						"      ID: 14\n"
						"      Name: Hydro Steam\n"
						"      PP: 20/30\n"
						"}",
					};

					CHECK((output.str() == expected));
				}
			}
		}

		GIVEN("invalid move registry")
		{
			Pokemon pokemon{
    			makePokemon({
    				.mMetadata = {
                        .mName = "TestMon",
        				.mBaseStats = {
        					.mMaxHealth = 150,
        					.mAttack = 101,
        					.mDefense = 102,
        					.mSpAttack = 103,
        					.mSpDefense = 104,
        					.mSpeed = 105,
        				},
        				.mTypeIDs = {toTypeID(BuiltinTypeID::Fire), toTypeID(BuiltinTypeID::Bug)},
                    },
                    .mMoveMetas {
                        {
                            {
                                .mMoveID = toMoveID(BuiltinMoveID::Pound),
                                .mPPMaxAmount = 15,
                                .mPPDefaultAmount = 5,
                            },
                            {
                                .mMoveID = toMoveID(BuiltinMoveID::KarateChop),
                                .mPPMaxAmount = 20,
                                .mPPDefaultAmount = 10,
                           	},
                            {
                                .mMoveID = toMoveID(BuiltinMoveID::Facade),
                                .mPPMaxAmount = 25,
                                .mPPDefaultAmount = 15,
                            },
                            {
                                .mMoveID = toMoveID(BuiltinMoveID::HydroSteam),
                                .mPPMaxAmount = 30,
                                .mPPDefaultAmount = 20,
                            },
                        },
                    },
    				.mStatusIDs
    				= {toStatusID(BuiltinStatusID::Poison), toStatusID(BuiltinStatusID::Toxic), toStatusID(BuiltinStatusID::Paralysis),
    				   toStatusID(BuiltinStatusID::Sleep), toStatusID(BuiltinStatusID::Freeze),},
    				.mAbilityEntries = {{{.mAbilityID = toAbilityID(BuiltinAbilityID::AirLock), .mClassification = AbilityClassification::Hidden}}},
    				.mItemIDs = {toItemID(BuiltinItemID::CheriBerry)},
    				.mNatureIDs = {toNatureID(BuiltinNatureID::Hardy)},
    				.mHealth = 150,
    				.mLevel = 50,
    			}),
    		};

			RegistryProvider nullProvider{getNullMoveRegistryProvider()};

			WHEN("the Pokemon is written to a stream")
			{
				std::ostringstream output;
				printPokemonWithNames(output, pokemon, nullProvider);

				THEN("all public state is written with numeric identifiers and PP")
				{
					std::string_view expected{
						"Pokemon {\n"
						"  Name: TestMon\n"
						"  Level: 50\n"
						"  Level Damage Factor: 22\n"
						"  Health: 150/210\n"
						"    IV: 0\n"
						"    EV: 0\n"
						"  Attack: 106\n"
						"    IV: 0\n"
						"    EV: 0\n"
						"  Defense: 107\n"
						"    IV: 0\n"
						"    EV: 0\n"
						"  Special Attack: 108\n"
						"    IV: 0\n"
						"    EV: 0\n"
						"  Special Defense: 109\n"
						"    IV: 0\n"
						"    EV: 0\n"
						"  Speed: 110\n"
						"    IV: 0\n"
						"    EV: 0\n"
						"  Types:\n"
						"    [0]:\n"
						"      ID: 2\n"
						"      Name: Fire\n"
						"    [1]:\n"
						"      ID: 12\n"
						"      Name: Bug\n"
						"  Natures:\n"
						"    [0]:\n"
						"      ID: 1\n"
						"      Name: Hardy\n"
						"  Abilities:\n"
						"    [0]:\n"
						"      ID: 15\n"
						"      Name: Air Lock\n"
						"      Classification: Hidden\n"
						"  Items:\n"
						"    [0]:\n"
						"      ID: 2\n"
						"      Name: Cheri Berry\n"
						"  Non-Volatile Statuses:\n"
						"    [0]:\n"
						"      ID: 5\n"
						"      Name: Poison\n"
						"    [1]:\n"
						"      ID: 6\n"
						"      Name: Toxic\n"
						"    [2]:\n"
						"      ID: 1\n"
						"      Name: Paralysis\n"
						"    [3]:\n"
						"      ID: 3\n"
						"      Name: Sleep\n"
						"    [4]:\n"
						"      ID: 4\n"
						"      Name: Freeze\n"
						"  Moves:\n"
						"    [0]:\n"
						"      ID: 1\n"
						"      Name: <unregistered>\n"
						"      PP: 5/15\n"
						"    [1]:\n"
						"      ID: 2\n"
						"      Name: <unregistered>\n"
						"      PP: 10/20\n"
						"    [2]:\n"
						"      ID: 13\n"
						"      Name: <unregistered>\n"
						"      PP: 15/25\n"
						"    [3]:\n"
						"      ID: 14\n"
						"      Name: <unregistered>\n"
						"      PP: 20/30\n"
						"}",
					};

					CHECK((output.str() == expected));
				}
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
