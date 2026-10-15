/*! @file pokemonHelpers.test.cpp
	@brief C++ file for running tests for the Pokemon helper functions.
	@date 10/14/2026
	@since 0.12.28
	@version 0.12.51
	@author Matthew Moore
*/

#include "Pokemon/pokemonHelpers.h"

#include <algorithm>
#include <array>
#include <expected>
#include <sstream>
#include <vector>

#include "Ability/abilityID.h"
#include "Ability/builtInAbilityID.h"
#include "Configuration/constants.h"
#include "Core/typedefs.h"
#include "Item/builtInItemID.h"
#include "Item/itemID.h"
#include "Learnset/learnsetMeta.h"
#include "Move/builtInMoveID.h"
#include "Move/moveID.h"
#include "Nature/builtInNatureID.h"
#include "Nature/natureID.h"
#include "Pokemon/pokemon.h"
#include "Pokemon/pokemon.testHelper.h"
#include "Pokemon/pokemonEncounter.h"
#include "Pokemon/pokemonStats.h"
#include "Registry/abilityRegistry.h"
#include "Registry/itemRegistry.h"
#include "Registry/moveRegistry.h"
#include "Registry/natureRegistry.h"
#include "Registry/registryProvider.h"
#include "Registry/registryProvider.testHelper.h"
#include "Status/builtInStatusID.h"
#include "Type/builtInTypeID.h"
#include "Validation/Ability/abilityError.h"
#include "Validation/Item/itemError.h"
#include "Validation/Move/moveError.h"
#include "Validation/Nature/natureError.h"
#include "Validation/Pokemon/pokemonError.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Ability::AbilityID;
using PocketCore::Ability::BuiltinAbilityID;
using PocketCore::Ability::NO_ABILITY_ID;
using PocketCore::Ability::toAbilityID;
using PocketCore::Configuration::MAX_ABILITIES_PER_POKEMON;
using PocketCore::Configuration::MAX_ITEMS_PER_POKEMON;
using PocketCore::Configuration::MAX_MOVES_PER_POKEMON;
using PocketCore::Configuration::MAX_NATURES_PER_POKEMON;
using PocketCore::Core::us;
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
using PocketCore::Nature::NO_NATURE_ID;
using PocketCore::Nature::toNatureID;
using PocketCore::Pokemon::fillWithValidMoves;
using PocketCore::Pokemon::getValidPokemonAbilities;
using PocketCore::Pokemon::getValidPokemonEVs;
using PocketCore::Pokemon::getValidPokemonItems;
using PocketCore::Pokemon::getValidPokemonIVs;
using PocketCore::Pokemon::getValidPokemonMoves;
using PocketCore::Pokemon::getValidPokemonNatures;
using PocketCore::Pokemon::Pokemon;
using PocketCore::Pokemon::POKEMON_STAT_COUNT;
using PocketCore::Pokemon::PokemonEncounter;
using PocketCore::Pokemon::PokemonOrigin;
using PocketCore::Registry::Ability::AbilityRegistry;
using PocketCore::Registry::Item::ItemRegistry;
using PocketCore::Registry::Move::MoveRegistry;
using PocketCore::Registry::Nature::NatureRegistry;
using PocketCore::Registry::RegistryProvider;
using PocketCore::Status::BuiltinStatusID;
using PocketCore::Status::toStatusID;
using PocketCore::Testing::getDefaultInitializedRegistryProvider;
using PocketCore::Testing::getNullAbilityRegistryProvider;
using PocketCore::Testing::getNullItemRegistryProvider;
using PocketCore::Testing::getNullMoveRegistryProvider;
using PocketCore::Testing::getNullNatureRegistryProvider;
using PocketCore::Testing::getNullStatusRegistryProvider;
using PocketCore::Testing::getNullTypeRegistryProvider;
using PocketCore::Testing::makePokemon;
using PocketCore::Type::BuiltinTypeID;
using PocketCore::Type::toTypeID;
using PocketCore::Validation::Ability::AbilityError;
using PocketCore::Validation::Item::ItemError;
using PocketCore::Validation::Move::MoveError;
using PocketCore::Validation::Nature::NatureError;
using PocketCore::Validation::Pokemon::PokemonError;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("Pokemon free function")
{
	RegistryProvider provider{getDefaultInitializedRegistryProvider()};

	GIVEN("getValidPokemonNatures")
	{
		std::array<NatureID, MAX_NATURES_PER_POKEMON> validNatureIDs{};
		NatureRegistry natureRegistry{*provider.mNatureRegistry};

		WHEN("no passed in nature IDs")
		{
			std::expected<void, NatureError> result{getValidPokemonNatures({}, validNatureIDs, natureRegistry)};

			THEN("it returns nothing")
			{
				CHECK(result.has_value());
			}
		}

		GIVEN("a span of nature IDs")
		{
			WHEN("calling with valid nature IDs")
			{
				std::array<NatureID, MAX_NATURES_PER_POKEMON> natureIDs{toNatureID(BuiltinNatureID::Bashful)};

				std::expected<void, NatureError> result{getValidPokemonNatures(natureIDs, validNatureIDs, natureRegistry)};

				THEN("it successfully validates and puts the data in the validNatureIDs")
				{
					REQUIRE(result.has_value());
					CHECK(std::ranges::equal(natureIDs, validNatureIDs));
				}
			}

			WHEN("calling with NO_NATURE_ID")
			{
				std::array<NatureID, MAX_NATURES_PER_POKEMON> natureIDs{NO_NATURE_ID};

				std::expected<void, NatureError> result{getValidPokemonNatures(natureIDs, validNatureIDs, natureRegistry)};

				THEN("it successfully validates and puts the data in the validNatureIDs")
				{
					REQUIRE(result.has_value());
					CHECK(std::ranges::equal(natureIDs, validNatureIDs));
				}
			}

			WHEN("calling with invalid nature IDs")
			{
				std::array<NatureID, MAX_NATURES_PER_POKEMON> natureIDs{NatureID{200}};

				std::expected<void, NatureError> result{getValidPokemonNatures(natureIDs, validNatureIDs, natureRegistry)};

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
		std::array<AbilityID, MAX_ABILITIES_PER_POKEMON> validAbilityIDs{};
		AbilityRegistry abilityRegistry{*provider.mAbilityRegistry};

		WHEN("no passed in ability IDs")
		{
			std::expected<void, AbilityError> result{getValidPokemonAbilities({}, validAbilityIDs, abilityRegistry)};

			THEN("it returns nothing")
			{
				CHECK(result.has_value());
			}
		}

		GIVEN("a span of ability IDs")
		{
			WHEN("calling with valid ability IDs")
			{
				std::array<AbilityID, MAX_ABILITIES_PER_POKEMON> abilityIDs{toAbilityID(BuiltinAbilityID::CloudNine)};

				std::expected<void, AbilityError> result{getValidPokemonAbilities(abilityIDs, validAbilityIDs, abilityRegistry)};

				THEN("it successfully validates and puts the data in the validAbilityIDs")
				{
					REQUIRE(result.has_value());
					CHECK(std::ranges::equal(abilityIDs, validAbilityIDs));
				}
			}

			WHEN("calling with NO_ABILITY_ID")
			{
				std::array<AbilityID, MAX_ABILITIES_PER_POKEMON> abilityIDs{NO_ABILITY_ID};

				std::expected<void, AbilityError> result{getValidPokemonAbilities(abilityIDs, validAbilityIDs, abilityRegistry)};

				THEN("it successfully validates and puts the data in the validAbilityIDs")
				{
					REQUIRE(result.has_value());
					CHECK(std::ranges::equal(abilityIDs, validAbilityIDs));
				}
			}

			WHEN("calling with invalid ability IDs")
			{
				std::array<AbilityID, MAX_ABILITIES_PER_POKEMON> abilityIDs{AbilityID{200}};

				std::expected<void, AbilityError> result{getValidPokemonAbilities(abilityIDs, validAbilityIDs, abilityRegistry)};

				THEN("it successfully invalidates and returns an error")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error() == AbilityError::MISSING_ABILITY_METADATA));
					CHECK((validAbilityIDs.at(0) == NO_ABILITY_ID));
				}
			}
		}
	}

	GIVEN("getValidPokemonItems")
	{
		std::array<ItemID, MAX_ITEMS_PER_POKEMON> validItemIDs{};
		ItemRegistry itemRegistry{*provider.mItemRegistry};

		WHEN("no passed in item IDs")
		{
			std::expected<void, ItemError> result{getValidPokemonItems({}, validItemIDs, itemRegistry)};

			THEN("it returns nothing")
			{
				CHECK(result.has_value());
			}
		}

		GIVEN("a span of item IDs")
		{
			WHEN("calling with valid item IDs")
			{
				std::array<ItemID, MAX_ITEMS_PER_POKEMON> itemIDs{toItemID(BuiltinItemID::CheriBerry)};

				std::expected<void, ItemError> result{getValidPokemonItems(itemIDs, validItemIDs, itemRegistry)};

				THEN("it successfully validates and puts the data in the validItemIDs")
				{
					REQUIRE(result.has_value());
					CHECK(std::ranges::equal(itemIDs, validItemIDs));
				}
			}

			WHEN("calling with NO_ITEM_ID")
			{
				std::array<ItemID, MAX_ITEMS_PER_POKEMON> itemIDs{NO_ITEM_ID};

				std::expected<void, ItemError> result{getValidPokemonItems(itemIDs, validItemIDs, itemRegistry)};

				THEN("it successfully validates and puts the data in the validItemIDs")
				{
					REQUIRE(result.has_value());
					CHECK(std::ranges::equal(itemIDs, validItemIDs));
				}
			}

			WHEN("calling with invalid item IDs")
			{
				std::array<ItemID, MAX_ITEMS_PER_POKEMON> itemIDs{ItemID{200}};

				std::expected<void, ItemError> result{getValidPokemonItems(itemIDs, validItemIDs, itemRegistry)};

				THEN("it successfully invalidates and returns an error")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error() == ItemError::MISSING_ITEM_METADATA));
					CHECK((validItemIDs.at(0) == NO_ITEM_ID));
				}
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
				std::array<MoveID, MAX_MOVES_PER_POKEMON> moveIDs{
					toMoveID(BuiltinMoveID::HydroSteam),
					toMoveID(BuiltinMoveID::KarateChop),
					toMoveID(BuiltinMoveID::KarateChop),
					toMoveID(BuiltinMoveID::Pound),
				};

				std::expected<void, MoveError> result{
					getValidPokemonMoves(moveIDs, validMoveIDs, moveRegistry, meta, encounter, pokemonEncounterLevel),
				};

				THEN("it successfully validates, replaces the duplicate with NO_MOVE_ID, and sorts it to the back while preserving the "
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

							THEN("all unique moves are assigned to moveIDs and duplicate moves are set to NO_MOVE_ID and moved to the end")
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

							THEN("all unique non-egg moves are assigned to moveIDs and duplicate moves are set to NO_MOVE_ID and moved to "
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

							THEN("it grabs the top 4 level requirement moves of level up method, and shifts any NO_MOVE_ID entries to the "
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

							THEN("all unique non-egg moves are assigned to moveIDs and duplicate moves are set to NO_MOVE_ID and moved to "
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

							THEN("all unique moves are assigned to moveIDs and duplicate moves are set to NO_MOVE_ID and moved to the end")
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

							THEN("all unique level-up and egg moves are assigned to moveIDs and duplicate moves are set to NO_MOVE_ID and "
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

							THEN("all unique level-up and egg moves are assigned to moveIDs and duplicate moves are set to NO_MOVE_ID and "
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

		WHEN("no passed in Pokemon IVs")
		{
			std::expected<void, PokemonError> result{getValidPokemonIVs({}, validPokemonIVs)};

			THEN("it returns nothing")
			{
				CHECK(result.has_value());
			}
		}

		GIVEN("a span of Pokemon IVs")
		{
			WHEN("calling with valid Pokemon IVs")
			{
				std::array<us, POKEMON_STAT_COUNT> pokemonIVs{25, 2, 15, 30, 14, 24};

				std::expected<void, PokemonError> result{getValidPokemonIVs(pokemonIVs, validPokemonIVs)};

				THEN("it successfully validates and puts the data in the validItemIDs")
				{
					REQUIRE(result.has_value());
					CHECK(std::ranges::equal(pokemonIVs, validPokemonIVs));
				}
			}

			WHEN("calling with an invalid Pokemon IV")
			{
				std::array<us, POKEMON_STAT_COUNT> pokemonIVs{25, 2, 15, 30, 14, 32};

				std::expected<void, PokemonError> result{getValidPokemonIVs(pokemonIVs, validPokemonIVs)};

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

			THEN("it returns nothing")
			{
				CHECK(result.has_value());
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
    				.mName = "TestMon",
    				.mStats = {
    					.mMaxHealth = 150,
    					.mAttack = 101,
    					.mDefense = 102,
    					.mSpAttack = 103,
    					.mSpDefense = 104,
    					.mSpeed = 105,
    				},
    				.mStatusIDs
    				= {toStatusID(BuiltinStatusID::Poison), toStatusID(BuiltinStatusID::Toxic), toStatusID(BuiltinStatusID::Paralysis),
    				   toStatusID(BuiltinStatusID::Sleep), toStatusID(BuiltinStatusID::Freeze),},
    				.mMoveIDs = {toMoveID(BuiltinMoveID::Pound), toMoveID(BuiltinMoveID::KarateChop), toMoveID(BuiltinMoveID::Facade), toMoveID(BuiltinMoveID::HydroSteam)},
    				.mMaxPP = {15, 20, 25, 30},
    				.mCurrentPP = {5, 10, 15, 20},
    				.mTypesIDs = {toTypeID(BuiltinTypeID::Fire), toTypeID(BuiltinTypeID::Bug)},
    				.mAbilityIDs = {toAbilityID(BuiltinAbilityID::AirLock)},
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
						"      ID: 6\n"
						"      Name: Air Lock\n"
						"  Items:\n"
						"    [0]:\n"
						"      ID: 1\n"
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
						"      ID: 3\n"
						"      Name: Facade\n"
						"      PP: 15/25\n"
						"    [3]:\n"
						"      ID: 4\n"
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
    				.mName = "TestMon",
    				.mStats = {
    					.mMaxHealth = 150,
    					.mAttack = 101,
    					.mDefense = 102,
    					.mSpAttack = 103,
    					.mSpDefense = 104,
    					.mSpeed = 105,
    				},
    				.mStatusIDs
    				= {toStatusID(BuiltinStatusID::Poison), toStatusID(BuiltinStatusID::Toxic), toStatusID(BuiltinStatusID::Paralysis),
    				   toStatusID(BuiltinStatusID::Sleep), toStatusID(BuiltinStatusID::Freeze),},
    				.mMoveIDs = {toMoveID(BuiltinMoveID::Pound), toMoveID(BuiltinMoveID::KarateChop), toMoveID(BuiltinMoveID::Facade), toMoveID(BuiltinMoveID::HydroSteam)},
    				.mMaxPP = {15, 20, 25, 30},
    				.mCurrentPP = {5, 10, 15, 20},
    				.mTypesIDs = {toTypeID(BuiltinTypeID::Fire), toTypeID(BuiltinTypeID::Bug)},
    				.mAbilityIDs = {toAbilityID(BuiltinAbilityID::AirLock)},
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
						"      ID: 6\n"
						"      Name: Air Lock\n"
						"  Items:\n"
						"    [0]:\n"
						"      ID: 1\n"
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
						"      ID: 3\n"
						"      Name: Facade\n"
						"      PP: 15/25\n"
						"    [3]:\n"
						"      ID: 4\n"
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
    				.mName = "TestMon",
    				.mStats = {
    					.mMaxHealth = 150,
    					.mAttack = 101,
    					.mDefense = 102,
    					.mSpAttack = 103,
    					.mSpDefense = 104,
    					.mSpeed = 105,
    				},
    				.mStatusIDs
    				= {toStatusID(BuiltinStatusID::Poison), toStatusID(BuiltinStatusID::Toxic), toStatusID(BuiltinStatusID::Paralysis),
    				   toStatusID(BuiltinStatusID::Sleep), toStatusID(BuiltinStatusID::Freeze),},
    				.mMoveIDs = {toMoveID(BuiltinMoveID::Pound), toMoveID(BuiltinMoveID::KarateChop), toMoveID(BuiltinMoveID::Facade), toMoveID(BuiltinMoveID::HydroSteam)},
    				.mMaxPP = {15, 20, 25, 30},
    				.mCurrentPP = {5, 10, 15, 20},
    				.mTypesIDs = {toTypeID(BuiltinTypeID::Fire), toTypeID(BuiltinTypeID::Bug)},
    				.mAbilityIDs = {toAbilityID(BuiltinAbilityID::AirLock)},
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
						"      ID: 6\n"
						"      Name: Air Lock\n"
						"  Items:\n"
						"    [0]:\n"
						"      ID: 1\n"
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
						"      ID: 3\n"
						"      Name: Facade\n"
						"      PP: 15/25\n"
						"    [3]:\n"
						"      ID: 4\n"
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
    				.mName = "TestMon",
    				.mStats = {
    					.mMaxHealth = 150,
    					.mAttack = 101,
    					.mDefense = 102,
    					.mSpAttack = 103,
    					.mSpDefense = 104,
    					.mSpeed = 105,
    				},
    				.mStatusIDs
    				= {toStatusID(BuiltinStatusID::Poison), toStatusID(BuiltinStatusID::Toxic), toStatusID(BuiltinStatusID::Paralysis),
    				   toStatusID(BuiltinStatusID::Sleep), toStatusID(BuiltinStatusID::Freeze),},
    				.mMoveIDs = {toMoveID(BuiltinMoveID::Pound), toMoveID(BuiltinMoveID::KarateChop), toMoveID(BuiltinMoveID::Facade), toMoveID(BuiltinMoveID::HydroSteam)},
    				.mMaxPP = {15, 20, 25, 30},
    				.mCurrentPP = {5, 10, 15, 20},
    				.mTypesIDs = {toTypeID(BuiltinTypeID::Fire), toTypeID(BuiltinTypeID::Bug)},
    				.mAbilityIDs = {toAbilityID(BuiltinAbilityID::AirLock)},
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
						"      ID: 6\n"
						"      Name: <unregistered>\n"
						"  Items:\n"
						"    [0]:\n"
						"      ID: 1\n"
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
						"      ID: 3\n"
						"      Name: Facade\n"
						"      PP: 15/25\n"
						"    [3]:\n"
						"      ID: 4\n"
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
    				.mName = "TestMon",
    				.mStats = {
    					.mMaxHealth = 150,
    					.mAttack = 101,
    					.mDefense = 102,
    					.mSpAttack = 103,
    					.mSpDefense = 104,
    					.mSpeed = 105,
    				},
    				.mStatusIDs
    				= {toStatusID(BuiltinStatusID::Poison), toStatusID(BuiltinStatusID::Toxic), toStatusID(BuiltinStatusID::Paralysis),
    				   toStatusID(BuiltinStatusID::Sleep), toStatusID(BuiltinStatusID::Freeze),},
    				.mMoveIDs = {toMoveID(BuiltinMoveID::Pound), toMoveID(BuiltinMoveID::KarateChop), toMoveID(BuiltinMoveID::Facade), toMoveID(BuiltinMoveID::HydroSteam)},
    				.mMaxPP = {15, 20, 25, 30},
    				.mCurrentPP = {5, 10, 15, 20},
    				.mTypesIDs = {toTypeID(BuiltinTypeID::Fire), toTypeID(BuiltinTypeID::Bug)},
    				.mAbilityIDs = {toAbilityID(BuiltinAbilityID::AirLock)},
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
						"      ID: 6\n"
						"      Name: Air Lock\n"
						"  Items:\n"
						"    [0]:\n"
						"      ID: 1\n"
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
						"      ID: 3\n"
						"      Name: Facade\n"
						"      PP: 15/25\n"
						"    [3]:\n"
						"      ID: 4\n"
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
    				.mName = "TestMon",
    				.mStats = {
    					.mMaxHealth = 150,
    					.mAttack = 101,
    					.mDefense = 102,
    					.mSpAttack = 103,
    					.mSpDefense = 104,
    					.mSpeed = 105,
    				},
    				.mStatusIDs
    				= {toStatusID(BuiltinStatusID::Poison), toStatusID(BuiltinStatusID::Toxic), toStatusID(BuiltinStatusID::Paralysis),
    				   toStatusID(BuiltinStatusID::Sleep), toStatusID(BuiltinStatusID::Freeze),},
    				.mMoveIDs = {toMoveID(BuiltinMoveID::Pound), toMoveID(BuiltinMoveID::KarateChop), toMoveID(BuiltinMoveID::Facade), toMoveID(BuiltinMoveID::HydroSteam)},
    				.mMaxPP = {15, 20, 25, 30},
    				.mCurrentPP = {5, 10, 15, 20},
    				.mTypesIDs = {toTypeID(BuiltinTypeID::Fire), toTypeID(BuiltinTypeID::Bug)},
    				.mAbilityIDs = {toAbilityID(BuiltinAbilityID::AirLock)},
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
						"      ID: 6\n"
						"      Name: Air Lock\n"
						"  Items:\n"
						"    [0]:\n"
						"      ID: 1\n"
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
						"      ID: 3\n"
						"      Name: Facade\n"
						"      PP: 15/25\n"
						"    [3]:\n"
						"      ID: 4\n"
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
    				.mName = "TestMon",
    				.mStats = {
    					.mMaxHealth = 150,
    					.mAttack = 101,
    					.mDefense = 102,
    					.mSpAttack = 103,
    					.mSpDefense = 104,
    					.mSpeed = 105,
    				},
    				.mStatusIDs
    				= {toStatusID(BuiltinStatusID::Poison), toStatusID(BuiltinStatusID::Toxic), toStatusID(BuiltinStatusID::Paralysis),
    				   toStatusID(BuiltinStatusID::Sleep), toStatusID(BuiltinStatusID::Freeze),},
    				.mMoveIDs = {toMoveID(BuiltinMoveID::Pound), toMoveID(BuiltinMoveID::KarateChop), toMoveID(BuiltinMoveID::Facade), toMoveID(BuiltinMoveID::HydroSteam)},
    				.mMaxPP = {15, 20, 25, 30},
    				.mCurrentPP = {5, 10, 15, 20},
    				.mTypesIDs = {toTypeID(BuiltinTypeID::Fire), toTypeID(BuiltinTypeID::Bug)},
    				.mAbilityIDs = {toAbilityID(BuiltinAbilityID::AirLock)},
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
						"      ID: 6\n"
						"      Name: Air Lock\n"
						"  Items:\n"
						"    [0]:\n"
						"      ID: 1\n"
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
						"      ID: 3\n"
						"      Name: <unregistered>\n"
						"      PP: 15/25\n"
						"    [3]:\n"
						"      ID: 4\n"
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
