/*! @file pokemonRegistry.test.cpp
	@brief C++ file for running tests for the PokemonRegistry.
	@date 11/23/2026
	@since 0.12.20
	@version 0.12.56
	@author Matthew Moore
*/

#include "Registry/pokemonRegistry.h"

#include <cstddef>
#include <expected>
#include <functional>
#include <memory>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "Ability/abilityID.h"
#include "Ability/abilityMeta.h"
#include "Ability/builtInAbilityID.h"
#include "Battle/battleRandomSources.h"
#include "Configuration/constants.h"
#include "Configuration/pokemonRegistryConfiguration.h"
#include "Core/typedefs.h"
#include "EggGroup/builtInEggGroupID.h"
#include "Item/builtInItemID.h"
#include "Item/itemID.h"
#include "Learnset/builtInLearnsetID.h"
#include "Learnset/learnsetID.h"
#include "Location/builtInLocationID.h"
#include "Move/builtInMoveID.h"
#include "Move/moveID.h"
#include "Nature/builtInNatureID.h"
#include "Nature/natureID.h"
#include "Pokemon/builtInPokemonID.h"
#include "Pokemon/constants.h"
#include "Pokemon/pokemon.h"
#include "Pokemon/pokemon.testHelper.h"
#include "Pokemon/pokemonEncounter.h"
#include "Pokemon/pokemonID.h"
#include "Pokemon/pokemonInstantiationContext.h"
#include "Pokemon/pokemonMeta.h"
#include "Registry/registryProvider.h"
#include "Registry/registryProvider.testHelper.h"
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
using PocketCore::Ability::toAbilityID;
using PocketCore::Battle::BattleRandomSources;
using PocketCore::Configuration::MAX_ABILITIES_PER_POKEMON;
using PocketCore::Configuration::PokemonRegistryConfiguration;
using PocketCore::Core::sb;
using PocketCore::Core::ub;
using PocketCore::Core::us;
using PocketCore::EggGroup::BuiltinEggGroupID;
using PocketCore::EggGroup::toEggGroupID;
using PocketCore::Item::BuiltinItemID;
using PocketCore::Item::ItemID;
using PocketCore::Item::toItemID;
using PocketCore::Learnset::BuiltinLearnsetID;
using PocketCore::Learnset::LearnsetID;
using PocketCore::Learnset::toLearnsetID;
using PocketCore::Location::BuiltinLocationID;
using PocketCore::Location::toLocationID;
using PocketCore::Move::BuiltinMoveID;
using PocketCore::Move::MoveID;
using PocketCore::Move::toMoveID;
using PocketCore::Nature::BuiltinNatureID;
using PocketCore::Nature::NatureID;
using PocketCore::Nature::toNatureID;
using PocketCore::Pokemon::BuiltinPokemonID;
using PocketCore::Pokemon::NO_POKEMON_ID;
using PocketCore::Pokemon::Pokemon;
using PocketCore::Pokemon::POKEMON_NAME_NONE;
using PocketCore::Pokemon::PokemonAbilityEntry;
using PocketCore::Pokemon::PokemonGender;
using PocketCore::Pokemon::PokemonID;
using PocketCore::Pokemon::PokemonMeta;
using PocketCore::Pokemon::PokemonOrigin;
using PocketCore::Pokemon::PokemonSpec;
using PocketCore::Pokemon::SetLevelEncounter;
using PocketCore::Pokemon::toPokemonID;
using PocketCore::Pokemon::VariableLevelEncounter;
using PocketCore::Registry::Pokemon::PokemonInstantiationDependencies;
using PocketCore::Registry::Pokemon::PokemonInstantiationError;
using PocketCore::Registry::Pokemon::PokemonRegistry;
using PocketCore::Registry::RegistryProvider;
using PocketCore::Testing::checkStats;
using PocketCore::Testing::getDefaultInitializedRegistryProvider;
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

template <typename Registry>
concept PubliclyStructurallyMutable = requires(Registry &registry) { registry.setAmountRegistered(0); };

static_assert(!PubliclyStructurallyMutable<PokemonRegistry>);

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("PokemonRegistry")
{
	std::unique_ptr<PokemonRegistryConfiguration> configRegistryStorage{std::make_unique<PokemonRegistryConfiguration>()};
	const PokemonRegistry &registry{configRegistryStorage->getRuntimeRegistry()};
	ub finalPokemonUnderlyingValue{std::to_underlying(BuiltinPokemonID::FinalPokemon)};

	GIVEN("a default constructed pokemon registry")
	{
		THEN("Bulbasaur has the appropriate properties")
		{
			PokemonMeta expected{
			    .mEncounters = {
					{
						{
                		    .mAbilityEntries = std::array<PokemonAbilityEntry, MAX_ABILITIES_PER_POKEMON>{
                    				{{.mAbilityID = toAbilityID(BuiltinAbilityID::Chlorophyll), .mClassification = AbilityClassification::Hidden}},
                    			},
                			.mLevel = SetLevelEncounter{5},
                			.mLocationID = toLocationID(BuiltinLocationID::ProfessorOaksLab),
                			.mOrigin = PokemonOrigin::Starter,
						},
					},
				},
				.mItemPool = {},
				.mGenderPool = {
				    {
						{.mChance = 0.875, .mGender = PokemonGender::Male},
						{.mChance = 0.125, .mGender = PokemonGender::Female},
					},
				},
				.mAbilityPool = {{
					{.mAbilityID = toAbilityID(BuiltinAbilityID::Overgrow), .mClassification = AbilityClassification::Standard},
					{.mAbilityID = toAbilityID(BuiltinAbilityID::Chlorophyll), .mClassification = AbilityClassification::Hidden},
				},},
				.mName = "Bulbasaur",
				.mBaseStats = {
					.mMaxHealth = 45,
					.mAttack = 49,
					.mDefense = 49,
					.mSpAttack = 65,
					.mSpDefense = 65,
					.mSpeed = 45,
				},
				.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Grass), toEggGroupID(BuiltinEggGroupID::Monster)},
				.mTypeIDs = {toTypeID(BuiltinTypeID::Grass), toTypeID(BuiltinTypeID::Poison)},
				.mPokemonID = toPokemonID(BuiltinPokemonID::Bulbasaur),
				.mLearnsetID = toLearnsetID(BuiltinLearnsetID::BulbasaurLearnset),
			};

			const PokemonMeta *actual{registry.getPokemonMetadata(toPokemonID(BuiltinPokemonID::Bulbasaur))};

			CHECK((expected == *actual));
		}

		THEN("Ivysaur has the appropriate properties")
		{
			PokemonMeta expected{
    			.mEncounters = {},
				.mItemPool = {},
				.mGenderPool = {
				    {
						{.mChance = 0.875, .mGender = PokemonGender::Male},
						{.mChance = 0.125, .mGender = PokemonGender::Female},
					},
				},
				.mAbilityPool = {{
					{.mAbilityID = toAbilityID(BuiltinAbilityID::Overgrow), .mClassification = AbilityClassification::Standard},
					{.mAbilityID = toAbilityID(BuiltinAbilityID::Chlorophyll), .mClassification = AbilityClassification::Hidden},
				},},
				.mName = "Ivysaur",
				.mBaseStats = {
					.mMaxHealth = 60,
					.mAttack = 62,
					.mDefense = 63,
					.mSpAttack = 80,
					.mSpDefense = 80,
					.mSpeed = 60,
				},
				.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Grass), toEggGroupID(BuiltinEggGroupID::Monster)},
				.mTypeIDs = {toTypeID(BuiltinTypeID::Grass), toTypeID(BuiltinTypeID::Poison)},
				.mPokemonID = toPokemonID(BuiltinPokemonID::Ivysaur),
				.mLearnsetID = toLearnsetID(BuiltinLearnsetID::IvysaurLearnset),
			};

			const PokemonMeta *actual{registry.getPokemonMetadata(toPokemonID(BuiltinPokemonID::Ivysaur))};

			CHECK((expected == *actual));
		}

		THEN("Venusaur has the appropriate properties")
		{
			PokemonMeta expected{
			    .mEncounters = {},
				.mItemPool = {},
				.mGenderPool = {
				    {
						{.mChance = 0.875, .mGender = PokemonGender::Male},
						{.mChance = 0.125, .mGender = PokemonGender::Female},
					},
				},
				.mAbilityPool = {{
					{.mAbilityID = toAbilityID(BuiltinAbilityID::Overgrow), .mClassification = AbilityClassification::Standard},
					{.mAbilityID = toAbilityID(BuiltinAbilityID::Chlorophyll), .mClassification = AbilityClassification::Hidden},
				},},
				.mName = "Venusaur",
				.mBaseStats = {
					.mMaxHealth = 80,
					.mAttack = 82,
					.mDefense = 83,
					.mSpAttack = 100,
					.mSpDefense = 100,
					.mSpeed = 80,
				},
				.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Grass), toEggGroupID(BuiltinEggGroupID::Monster)},
				.mTypeIDs = {toTypeID(BuiltinTypeID::Grass), toTypeID(BuiltinTypeID::Poison)},
				.mPokemonID = toPokemonID(BuiltinPokemonID::Venusaur),
				.mLearnsetID = toLearnsetID(BuiltinLearnsetID::VenusaurLearnset),
			};

			const PokemonMeta *actual{registry.getPokemonMetadata(toPokemonID(BuiltinPokemonID::Venusaur))};

			CHECK((expected == *actual));
		}

		THEN("Charmander has the appropriate properties")
		{
			PokemonMeta expected{
    			.mEncounters = {
                	{
                		{
                  		    .mAbilityEntries = std::array<PokemonAbilityEntry, MAX_ABILITIES_PER_POKEMON>{
                    				{{.mAbilityID = toAbilityID(BuiltinAbilityID::SolarPower), .mClassification = AbilityClassification::Hidden}},
                     			},
                 			.mLevel = SetLevelEncounter{5},
                 			.mLocationID = toLocationID(BuiltinLocationID::ProfessorOaksLab),
                 			.mOrigin = PokemonOrigin::Starter,
                		},
                	},
                },
				.mItemPool = {},
				.mGenderPool = {
				    {
						{.mChance = 0.875, .mGender = PokemonGender::Male},
						{.mChance = 0.125, .mGender = PokemonGender::Female},
					},
				},
				.mAbilityPool = {{
					{.mAbilityID = toAbilityID(BuiltinAbilityID::Blaze), .mClassification = AbilityClassification::Standard},
					{.mAbilityID = toAbilityID(BuiltinAbilityID::SolarPower), .mClassification = AbilityClassification::Hidden},
				},},
				.mName = "Charmander",
				.mBaseStats = {
					.mMaxHealth = 39,
					.mAttack = 52,
					.mDefense = 43,
					.mSpAttack = 60,
					.mSpDefense = 50,
					.mSpeed = 65,
				},
				.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Dragon), toEggGroupID(BuiltinEggGroupID::Monster)},
				.mTypeIDs = {toTypeID(BuiltinTypeID::Fire)},
				.mPokemonID = toPokemonID(BuiltinPokemonID::Charmander),
				.mLearnsetID = toLearnsetID(BuiltinLearnsetID::CharmanderLearnset),
			};

			const PokemonMeta *actual{registry.getPokemonMetadata(toPokemonID(BuiltinPokemonID::Charmander))};

			CHECK((expected == *actual));
		}

		THEN("Charmeleon has the appropriate properties")
		{
			PokemonMeta expected{
			    .mEncounters = {},
				.mItemPool = {},
				.mGenderPool = {
				    {
						{.mChance = 0.875, .mGender = PokemonGender::Male},
						{.mChance = 0.125, .mGender = PokemonGender::Female},
					},
				},
				.mAbilityPool = {{
					{.mAbilityID = toAbilityID(BuiltinAbilityID::Blaze), .mClassification = AbilityClassification::Standard},
					{.mAbilityID = toAbilityID(BuiltinAbilityID::SolarPower), .mClassification = AbilityClassification::Hidden},
				},},
				.mName = "Charmeleon",
				.mBaseStats = {
					.mMaxHealth = 58,
					.mAttack = 64,
					.mDefense = 58,
					.mSpAttack = 80,
					.mSpDefense = 65,
					.mSpeed = 80,
				},
				.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Dragon), toEggGroupID(BuiltinEggGroupID::Monster)},
				.mTypeIDs = {toTypeID(BuiltinTypeID::Fire)},
				.mPokemonID = toPokemonID(BuiltinPokemonID::Charmeleon),
				.mLearnsetID = toLearnsetID(BuiltinLearnsetID::CharmeleonLearnset),
			};

			const PokemonMeta *actual{registry.getPokemonMetadata(toPokemonID(BuiltinPokemonID::Charmeleon))};

			CHECK((expected == *actual));
		}

		THEN("Charizard has the appropriate properties")
		{
			PokemonMeta expected{
				.mEncounters = {},
				.mItemPool = {},
				.mGenderPool = {
				    {
						{.mChance = 0.875, .mGender = PokemonGender::Male},
						{.mChance = 0.125, .mGender = PokemonGender::Female},
					},
				},
				.mAbilityPool = {{
					{.mAbilityID = toAbilityID(BuiltinAbilityID::Blaze), .mClassification = AbilityClassification::Standard},
					{.mAbilityID = toAbilityID(BuiltinAbilityID::SolarPower), .mClassification = AbilityClassification::Hidden},
				},},
				.mName = "Charizard",
				.mBaseStats = {
					.mMaxHealth = 78,
					.mAttack = 84,
					.mDefense = 78,
					.mSpAttack = 109,
					.mSpDefense = 85,
					.mSpeed = 100,
				},
				.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Dragon), toEggGroupID(BuiltinEggGroupID::Monster)},
				.mTypeIDs = {toTypeID(BuiltinTypeID::Fire), toTypeID(BuiltinTypeID::Flying)},
				.mPokemonID = toPokemonID(BuiltinPokemonID::Charizard),
				.mLearnsetID = toLearnsetID(BuiltinLearnsetID::CharizardLearnset),
			};

			const PokemonMeta *actual{registry.getPokemonMetadata(toPokemonID(BuiltinPokemonID::Charizard))};

			CHECK((expected == *actual));
		}

		THEN("Squirtle has the appropriate properties")
		{
			PokemonMeta expected{
			    .mEncounters = {
                	{
                		{
                  		    .mAbilityEntries = std::array<PokemonAbilityEntry, MAX_ABILITIES_PER_POKEMON>{
                    				{{.mAbilityID = toAbilityID(BuiltinAbilityID::RainDish), .mClassification = AbilityClassification::Hidden}},
                     			},
                 			.mLevel = SetLevelEncounter{5},
                 			.mLocationID = toLocationID(BuiltinLocationID::ProfessorOaksLab),
                 			.mOrigin = PokemonOrigin::Starter,
                		},
                	},
                },
				.mItemPool = {},
				.mGenderPool = {
				    {
						{.mChance = 0.875, .mGender = PokemonGender::Male},
						{.mChance = 0.125, .mGender = PokemonGender::Female},
					},
				},
				.mAbilityPool = {{
					{.mAbilityID = toAbilityID(BuiltinAbilityID::Torrent), .mClassification = AbilityClassification::Standard},
					{.mAbilityID = toAbilityID(BuiltinAbilityID::RainDish), .mClassification = AbilityClassification::Hidden},
				},},
				.mName = "Squirtle",
				.mBaseStats = {
					.mMaxHealth = 44,
					.mAttack = 48,
					.mDefense = 65,
					.mSpAttack = 50,
					.mSpDefense = 64,
					.mSpeed = 43,
				},
				.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Monster), toEggGroupID(BuiltinEggGroupID::Water1)},
				.mTypeIDs = {toTypeID(BuiltinTypeID::Water)},
				.mPokemonID = toPokemonID(BuiltinPokemonID::Squirtle),
				.mLearnsetID = toLearnsetID(BuiltinLearnsetID::SquirtleLearnset),
			};

			const PokemonMeta *actual{registry.getPokemonMetadata(toPokemonID(BuiltinPokemonID::Squirtle))};

			CHECK((expected == *actual));
		}

		THEN("Wartortle has the appropriate properties")
		{
			PokemonMeta expected{
				.mEncounters = {},
				.mItemPool = {},
				.mGenderPool = {
				    {
						{.mChance = 0.875, .mGender = PokemonGender::Male},
						{.mChance = 0.125, .mGender = PokemonGender::Female},
					},
				},
				.mAbilityPool = {{
					{.mAbilityID = toAbilityID(BuiltinAbilityID::Torrent), .mClassification = AbilityClassification::Standard},
					{.mAbilityID = toAbilityID(BuiltinAbilityID::RainDish), .mClassification = AbilityClassification::Hidden},
				},},
				.mName = "Wartortle",
				.mBaseStats = {
					.mMaxHealth = 59,
					.mAttack = 63,
					.mDefense = 80,
					.mSpAttack = 65,
					.mSpDefense = 80,
					.mSpeed = 58,
				},
				.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Monster), toEggGroupID(BuiltinEggGroupID::Water1)},
				.mTypeIDs = {toTypeID(BuiltinTypeID::Water)},
				.mPokemonID = toPokemonID(BuiltinPokemonID::Wartortle),
				.mLearnsetID = toLearnsetID(BuiltinLearnsetID::WartortleLearnset),
			};

			const PokemonMeta *actual{registry.getPokemonMetadata(toPokemonID(BuiltinPokemonID::Wartortle))};

			CHECK((expected == *actual));
		}

		THEN("Blastoise has the appropriate properties")
		{
			PokemonMeta expected{
				.mEncounters = {},
				.mItemPool = {},
				.mGenderPool = {
				    {
						{.mChance = 0.875, .mGender = PokemonGender::Male},
						{.mChance = 0.125, .mGender = PokemonGender::Female},
					},
				},
				.mAbilityPool = {{
					{.mAbilityID = toAbilityID(BuiltinAbilityID::Torrent), .mClassification = AbilityClassification::Standard},
					{.mAbilityID = toAbilityID(BuiltinAbilityID::RainDish), .mClassification = AbilityClassification::Hidden},
				},},
				.mName = "Blastoise",
				.mBaseStats = {
					.mMaxHealth = 79,
					.mAttack = 83,
					.mDefense = 100,
					.mSpAttack = 85,
					.mSpDefense = 105,
					.mSpeed = 78,
				},
				.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Monster), toEggGroupID(BuiltinEggGroupID::Water1)},
				.mTypeIDs = {toTypeID(BuiltinTypeID::Water)},
				.mPokemonID = toPokemonID(BuiltinPokemonID::Blastoise),
				.mLearnsetID = toLearnsetID(BuiltinLearnsetID::BlastoiseLearnset),
			};

			const PokemonMeta *actual{registry.getPokemonMetadata(toPokemonID(BuiltinPokemonID::Blastoise))};

			CHECK((expected == *actual));
		}

		THEN("Ditto has the appropriate properties")
		{
			PokemonMeta expected{
				.mEncounters = {},
				.mItemPool = {},
				.mGenderPool = {
				    {
						{.mChance = 1, .mGender = PokemonGender::Genderless},
					},
				},
				.mAbilityPool = {{
					{.mAbilityID = toAbilityID(BuiltinAbilityID::Limber), .mClassification = AbilityClassification::Standard},
					{.mAbilityID = toAbilityID(BuiltinAbilityID::Imposter), .mClassification = AbilityClassification::Hidden},
				},},
				.mName = "Ditto",
				.mBaseStats = {
					.mMaxHealth = 48,
					.mAttack = 48,
					.mDefense = 48,
					.mSpAttack = 48,
					.mSpDefense = 48,
					.mSpeed = 48,
				},
				.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Ditto)},
				.mTypeIDs = {toTypeID(BuiltinTypeID::Normal)},
				.mPokemonID = toPokemonID(BuiltinPokemonID::Ditto),
				.mLearnsetID = toLearnsetID(BuiltinLearnsetID::DittoLearnset),
			};

			const PokemonMeta *actual{registry.getPokemonMetadata(toPokemonID(BuiltinPokemonID::Ditto))};

			CHECK((expected == *actual));
		}

		THEN("Garchomp has the appropriate properties")
		{
			PokemonMeta expected{
				.mEncounters = {},
				.mItemPool = {},
				.mGenderPool = {
				    {
						{.mChance = 0.5, .mGender = PokemonGender::Male},
						{.mChance = 0.5, .mGender = PokemonGender::Female},
					},
				},
				.mAbilityPool = {{
					{.mAbilityID = toAbilityID(BuiltinAbilityID::SandVeil), .mClassification = AbilityClassification::Standard},
					{.mAbilityID = toAbilityID(BuiltinAbilityID::RoughSkin), .mClassification = AbilityClassification::Hidden},
				},},
				.mName = "Garchomp",
				.mBaseStats = {
					.mMaxHealth = 108,
					.mAttack = 130,
					.mDefense = 95,
					.mSpAttack = 80,
					.mSpDefense = 85,
					.mSpeed = 102,
				},
				.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Dragon), toEggGroupID(BuiltinEggGroupID::Monster)},
				.mTypeIDs = {toTypeID(BuiltinTypeID::Dragon), toTypeID(BuiltinTypeID::Ground)},
				.mPokemonID = toPokemonID(BuiltinPokemonID::Garchomp),
				.mLearnsetID = toLearnsetID(BuiltinLearnsetID::GarchompLearnset),
			};

			const PokemonMeta *actual{registry.getPokemonMetadata(toPokemonID(BuiltinPokemonID::Garchomp))};

			CHECK((expected == *actual));
		}
	}

	GIVEN("getPokemonMetadata")
	{
		THEN("unknown IDs are absent")
		{
			CHECK((registry.getPokemonMetadata(PokemonID{200}) == nullptr));
		}

		THEN("the metadata is retrieved when accessed by a valid Pokemon ID")
		{
			PokemonMeta expected{
				.mName = std::string(POKEMON_NAME_NONE),
				.mPokemonID = toPokemonID(BuiltinPokemonID::None),
			};

			CHECK((expected == *registry.getPokemonMetadata(NO_POKEMON_ID)));
		}
	}

	GIVEN("getPokemonID")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(registry.getPokemonID("Unknown").has_value());
		}

		THEN("the Pokemon ID is retrieved by valid Pokemon name")
		{
			std::optional<PokemonID> pokemonID{registry.getPokemonID(POKEMON_NAME_NONE)};

			REQUIRE(pokemonID.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((pokemonID == toPokemonID(BuiltinPokemonID::None)));
		}
	}

	GIVEN("getPokemonName")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(registry.getPokemonName(PokemonID{200}).has_value());
		}

		THEN("a registered pokemon name is returned by stable ID")
		{
			std::optional<std::string_view> pokemonName{registry.getPokemonName(toPokemonID(BuiltinPokemonID::None))};

			REQUIRE(pokemonName.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((pokemonName == POKEMON_NAME_NONE));
		}
	}

	GIVEN("getAmountRegistered")
	{
		THEN("the registered span contains the exact amount of built-in entries")
		{
			CHECK((registry.getAmountRegistered() == finalPokemonUnderlyingValue));
		}
	}

	GIVEN("getEntry")
	{
		THEN("an invalid internal array index has no metadata")
		{
			CHECK((registry.getEntry(12'000) == nullptr));
		}

		THEN("a valid internal array index has metadata")
		{
			PokemonMeta expected{
				.mName = std::string(POKEMON_NAME_NONE),
				.mPokemonID = toPokemonID(BuiltinPokemonID::None),
			};

			const PokemonMeta *pokemonMeta{registry.getEntry(0)};

			REQUIRE((pokemonMeta != nullptr));
			CHECK((*pokemonMeta == expected));
		}
	}

	GIVEN("getRegisteredPokemons")
	{
		THEN("the amount of pokemons returned matches the amount that are built-in")
		{
			CHECK((registry.getRegisteredPokemons().size() == finalPokemonUnderlyingValue));
		}
	}

	GIVEN("getNextPokemonID")
	{
		THEN("the next available stable Pokemon ID is after all built in pokemon IDs")
		{
			CHECK((registry.getNextPokemonID() == finalPokemonUnderlyingValue));
		}
	}

	GIVEN("findIndexByPokemonID")
	{
		THEN("an unknown stable ID has no internal index")
		{
			std::optional<ub> pokemonIndex{registry.findIndexByPokemonID(PokemonID{200})};
			CHECK_FALSE(pokemonIndex.has_value());
		}

		THEN("the internal array index is retrieved by valid Pokemon ID")
		{
			std::optional<ub> pokemonIndex{registry.findIndexByPokemonID(NO_POKEMON_ID)};

			REQUIRE(pokemonIndex.has_value());
			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((pokemonIndex == 0));
		}
	}

	GIVEN("hasPokemon")
	{
		WHEN("calling the string_view overload")
		{
			THEN("an unknown pokemon name has no entry")
			{
				CHECK_FALSE(registry.hasPokemon("Unknown"));
			}

			THEN("a known pokemon name has an entry")
			{
				CHECK(registry.hasPokemon(POKEMON_NAME_NONE));
			}
		}

		WHEN("calling the PokemonID overload")
		{
			THEN("an unknown pokemon ID has no entry")
			{
				CHECK_FALSE(registry.hasPokemon(PokemonID{200}));
			}

			THEN("a known pokemon ID has an entry")
			{
				CHECK(registry.hasPokemon(NO_POKEMON_ID));
			}
		}
	}

	GIVEN("instantiate")
	{
		RegistryProvider provider{getDefaultInitializedRegistryProvider()};
		PokemonSpec spec{.mLevel = 50, .mPokemonID = toPokemonID(BuiltinPokemonID::Ivysaur)};
		PokemonInstantiationDependencies instantiateDependencies{
			.mAbilityRegistry = provider.mAbilityRegistry,
			.mMoveRegistry = provider.mMoveRegistry,
			.mItemRegistry = provider.mItemRegistry,
			.mNatureRegistry = provider.mNatureRegistry,
			.mLearnsetRegistry = provider.mLearnsetRegistry,
			.mLocationRegistry = provider.mLocationRegistry,
		};
		BattleRandomSources randomSources{};
		std::vector<NatureID> natureIDs{};
		std::vector<PokemonAbilityEntry> abilityEntries{};
		std::vector<ItemID> itemIDs{};
		std::vector<MoveID> moveIDs{};
		std::vector<us> ivs{};
		std::vector<us> evs{};

		GIVEN("calling with invalid dependency registries")
		{
			WHEN("calling with a null ability registry")
			{
				instantiateDependencies.mAbilityRegistry = nullptr;

				std::expected<Pokemon, PokemonInstantiationError> result{
					registry.instantiate(spec, instantiateDependencies, randomSources),
				};

				THEN("it returns MissingRegistry")
				{
					REQUIRE_FALSE(result.has_value());
					REQUIRE(std::holds_alternative<RegistryError>(result.error()));
					CHECK((std::get<RegistryError>(result.error()) == RegistryError::MissingRegistry));
				}
			}

			WHEN("calling with a null item registry")
			{
				instantiateDependencies.mItemRegistry = nullptr;

				std::expected<Pokemon, PokemonInstantiationError> result{
					registry.instantiate(spec, instantiateDependencies, randomSources),
				};

				THEN("it returns MissingRegistry")
				{
					REQUIRE_FALSE(result.has_value());
					REQUIRE(std::holds_alternative<RegistryError>(result.error()));
					CHECK((std::get<RegistryError>(result.error()) == RegistryError::MissingRegistry));
				}
			}

			WHEN("calling with a null move registry")
			{
				instantiateDependencies.mMoveRegistry = nullptr;

				std::expected<Pokemon, PokemonInstantiationError> result{
					registry.instantiate(spec, instantiateDependencies, randomSources),
				};

				THEN("it returns MissingRegistry")
				{
					REQUIRE_FALSE(result.has_value());
					REQUIRE(std::holds_alternative<RegistryError>(result.error()));
					CHECK((std::get<RegistryError>(result.error()) == RegistryError::MissingRegistry));
				}
			}

			WHEN("calling with a null nature registry")
			{
				instantiateDependencies.mNatureRegistry = nullptr;

				std::expected<Pokemon, PokemonInstantiationError> result{
					registry.instantiate(spec, instantiateDependencies, randomSources),
				};

				THEN("it returns MissingRegistry")
				{
					REQUIRE_FALSE(result.has_value());
					REQUIRE(std::holds_alternative<RegistryError>(result.error()));
					CHECK((std::get<RegistryError>(result.error()) == RegistryError::MissingRegistry));
				}
			}

			WHEN("calling with a null learnset registry")
			{
				instantiateDependencies.mLearnsetRegistry = nullptr;

				std::expected<Pokemon, PokemonInstantiationError> result{
					registry.instantiate(spec, instantiateDependencies, randomSources),
				};

				THEN("it returns MissingRegistry")
				{
					REQUIRE_FALSE(result.has_value());
					REQUIRE(std::holds_alternative<RegistryError>(result.error()));
					CHECK((std::get<RegistryError>(result.error()) == RegistryError::MissingRegistry));
				}
			}

			WHEN("calling with a null location registry")
			{
				instantiateDependencies.mLocationRegistry = nullptr;

				std::expected<Pokemon, PokemonInstantiationError> result{
					registry.instantiate(spec, instantiateDependencies, randomSources),
				};

				THEN("it returns MissingRegistry")
				{
					REQUIRE_FALSE(result.has_value());
					REQUIRE(std::holds_alternative<RegistryError>(result.error()));
					CHECK((std::get<RegistryError>(result.error()) == RegistryError::MissingRegistry));
				}
			}
		}

		WHEN("calling an invalid Pokemon ID")
		{
			spec.mPokemonID = PokemonID{2'000};

			std::expected<Pokemon, PokemonInstantiationError> result{
				registry.instantiate(spec, instantiateDependencies, randomSources),
			};

			THEN("it returns PokemonNotFound")
			{
				REQUIRE_FALSE(result.has_value());
				REQUIRE(std::holds_alternative<RegistryError>(result.error()));
				CHECK((std::get<RegistryError>(result.error()) == RegistryError::PokemonNotFound));
			}
		}

		WHEN("calling an invalid Learnset ID")
		{
			PokemonID pokemonID{toPokemonID(BuiltinPokemonID::Bulbasaur)};
			spec.mPokemonID = pokemonID;
			std::expected<void, RegistryError> updateResult{
				configRegistryStorage->updatePokemon(pokemonID, {.mLearnsetID = LearnsetID{200}}),
			};

			REQUIRE(updateResult.has_value());

			std::expected<Pokemon, PokemonInstantiationError> result{
				registry.instantiate(spec, instantiateDependencies, randomSources),
			};

			THEN("it returns LearnsetNotFound")
			{
				REQUIRE_FALSE(result.has_value());
				REQUIRE(std::holds_alternative<RegistryError>(result.error()));
				CHECK((std::get<RegistryError>(result.error()) == RegistryError::LearnsetNotFound));
			}
		}

		GIVEN("calling an invalid Pokemon encounter index")
		{
			WHEN("calling with an encounter index out of bounds")
			{
				spec.mEncounterIndex = 20;

				std::expected<Pokemon, PokemonInstantiationError> result{
					registry.instantiate(spec, instantiateDependencies, randomSources),
				};

				THEN("it returns EncounterNotFound")
				{
					REQUIRE_FALSE(result.has_value());
					REQUIRE(std::holds_alternative<RegistryError>(result.error()));
					CHECK((std::get<RegistryError>(result.error()) == RegistryError::EncounterNotFound));
				}
			}

			WHEN("calling with an encounter index that points to NO_LOCATION_ID")
			{
				spec.mEncounterIndex = 0;

				std::expected<Pokemon, PokemonInstantiationError> result{
					registry.instantiate(spec, instantiateDependencies, randomSources),
				};

				THEN("it returns EncounterNotFound")
				{
					REQUIRE_FALSE(result.has_value());
					REQUIRE(std::holds_alternative<RegistryError>(result.error()));
					CHECK((std::get<RegistryError>(result.error()) == RegistryError::EncounterNotFound));
				}
			}
		}

		WHEN("calling with no encounter data and no level in the Pokemon Spec")
		{
			spec.mLevel = std::nullopt;
			std::expected<Pokemon, PokemonInstantiationError> result{
				registry.instantiate(spec, instantiateDependencies, randomSources),
			};

			THEN("it returns MISSING_LEVEL")
			{
				REQUIRE_FALSE(result.has_value());
				REQUIRE(std::holds_alternative<PokemonError>(result.error()));
				CHECK((std::get<PokemonError>(result.error()) == PokemonError::MISSING_LEVEL));
			}
		}

		WHEN("calling with an invalid nature ID")
		{
			natureIDs = {NatureID{200}};
			spec.mIDContext = {
				.mNatureIDs = natureIDs,
			};

			std::expected<Pokemon, PokemonInstantiationError> result{
				registry.instantiate(spec, instantiateDependencies, randomSources),
			};

			THEN("it returns MISSING_NATURE_METADATA")
			{
				REQUIRE_FALSE(result.has_value());
				REQUIRE(std::holds_alternative<NatureError>(result.error()));
				CHECK((std::get<NatureError>(result.error()) == NatureError::MISSING_NATURE_METADATA));
			}
		}

		WHEN("calling with an invalid ability ID")
		{
			abilityEntries = {{.mAbilityID = AbilityID{200}}};
			spec.mIDContext = {
				.mAbilityEntries = abilityEntries,
			};

			std::expected<Pokemon, PokemonInstantiationError> result{
				registry.instantiate(spec, instantiateDependencies, randomSources),
			};

			THEN("it returns MISSING_ABILITY_METADATA")
			{
				REQUIRE_FALSE(result.has_value());
				REQUIRE(std::holds_alternative<AbilityError>(result.error()));
				CHECK((std::get<AbilityError>(result.error()) == AbilityError::MISSING_ABILITY_METADATA));
			}
		}

		WHEN("calling with an invalid item ID")
		{
			itemIDs = {ItemID{200}};
			spec.mIDContext = {
				.mItemIDs = itemIDs,
			};

			std::expected<Pokemon, PokemonInstantiationError> result{
				registry.instantiate(spec, instantiateDependencies, randomSources),
			};

			THEN("it returns MISSING_ITEM_METADATA")
			{
				REQUIRE_FALSE(result.has_value());
				REQUIRE(std::holds_alternative<ItemError>(result.error()));
				CHECK((std::get<ItemError>(result.error()) == ItemError::MISSING_ITEM_METADATA));
			}
		}

		WHEN("calling with an invalid move ID")
		{
			moveIDs = {MoveID{200}};
			spec.mIDContext = {
				.mMoveIDs = moveIDs,
			};

			std::expected<Pokemon, PokemonInstantiationError> result{
				registry.instantiate(spec, instantiateDependencies, randomSources),
			};

			THEN("it returns MISSING_MOVE_METADATA")
			{
				REQUIRE_FALSE(result.has_value());
				REQUIRE(std::holds_alternative<MoveError>(result.error()));
				CHECK((std::get<MoveError>(result.error()) == MoveError::MISSING_MOVE_METADATA));
			}
		}

		WHEN("calling with an invalid IV")
		{
			ivs = {200};
			spec.mIVEVContext = {
				.mIVs = ivs,
			};

			std::expected<Pokemon, PokemonInstantiationError> result{
				registry.instantiate(spec, instantiateDependencies, randomSources),
			};

			THEN("it returns INVALID_IV_ARRAY")
			{
				REQUIRE_FALSE(result.has_value());
				REQUIRE(std::holds_alternative<PokemonError>(result.error()));
				CHECK((std::get<PokemonError>(result.error()) == PokemonError::INVALID_IV_ARRAY));
			}
		}

		WHEN("calling with an invalid EV")
		{
			evs = {600};
			spec.mIVEVContext = {
				.mEVs = evs,
			};

			std::expected<Pokemon, PokemonInstantiationError> result{
				registry.instantiate(spec, instantiateDependencies, randomSources),
			};

			THEN("it returns INVALID_EV_ARRAY")
			{
				REQUIRE_FALSE(result.has_value());
				REQUIRE(std::holds_alternative<PokemonError>(result.error()));
				CHECK((std::get<PokemonError>(result.error()) == PokemonError::INVALID_EV_ARRAY));
			}
		}

		WHEN("calling with an invalid gender pool")
		{
			PokemonMeta updatedMeta{*registry.getPokemonMetadata(spec.mPokemonID)};
			updatedMeta.mGenderPool = {{{.mChance = -0.5}}};

			std::expected<void, RegistryError> updateResult{
				configRegistryStorage->updatePokemon(spec.mPokemonID, updatedMeta),
			};

			REQUIRE(updateResult.has_value());

			std::expected<Pokemon, PokemonInstantiationError> result{
				registry.instantiate(spec, instantiateDependencies, randomSources),
			};

			THEN("it returns NO_VALID_GENDER_ENTRY_WEIGHTS")
			{
				REQUIRE_FALSE(result.has_value());
				REQUIRE(std::holds_alternative<PokemonError>(result.error()));
				CHECK((std::get<PokemonError>(result.error()) == PokemonError::NO_VALID_GENDER_ENTRY_WEIGHTS));
			}
		}

		WHEN("calling with a spec completely filled out with user defined values")
		{
			PokemonSpec newSpec{};

			ivs = {5, 22, 31, 14, 20, 10};
			evs = {42, 56, 0, 23, 100, 85};
			newSpec.mIVEVContext = {.mIVs = ivs, .mEVs = evs};

			natureIDs = {toNatureID(BuiltinNatureID::Calm)};
			abilityEntries = {{.mAbilityID = toAbilityID(BuiltinAbilityID::CloudNine), .mClassification = AbilityClassification::Hidden}};
			itemIDs = {toItemID(BuiltinItemID::ChestoBerry)};
			moveIDs = {toMoveID(BuiltinMoveID::KarateChop)};

			newSpec.mIDContext = {
				.mNatureIDs = natureIDs,
				.mAbilityEntries = abilityEntries,
				.mItemIDs = itemIDs,
				.mMoveIDs = moveIDs,
			};

			newSpec.mGender = PokemonGender::Genderless;
			newSpec.mBreedingContext = {};
			newSpec.mLevel = 5;
			newSpec.mPokemonID = toPokemonID(BuiltinPokemonID::Bulbasaur);

			SequenceRandom<ub> unsignedSequence{};
			SequenceRandom<sb> signedSequence{};
			SequenceRandom<double> realSequence{};
			SequenceRandom<std::size_t> indexSequence{};

			CallableRandomSource<ub, std::reference_wrapper<SequenceRandom<ub>>> unsignedSource{std::ref(unsignedSequence)};
			CallableRandomSource<sb, std::reference_wrapper<SequenceRandom<sb>>> signedSource{std::ref(signedSequence)};
			CallableRandomSource<double, std::reference_wrapper<SequenceRandom<double>>> realSource{std::ref(realSequence)};
			CallableRandomSource<std::size_t, std::reference_wrapper<SequenceRandom<std::size_t>>> indexSource{std::ref(indexSequence)};

			randomSources.mUnsigned = unsignedSource;
			randomSources.mSigned = signedSource;
			randomSources.mReal = realSource;
			randomSources.mIndex = indexSource;

			std::expected<Pokemon, PokemonInstantiationError> result{registry.instantiate(newSpec, instantiateDependencies, randomSources)};

			THEN("it returns a custom built Pokemon")
			{
				REQUIRE(result.has_value());

				Pokemon expected{
					makePokemon({
						.mMetadata = {
						    .mName = "Bulbasaur",
							.mBaseStats = {
								.mMaxHealth = 45,
								.mAttack = 49,
								.mDefense = 49,
								.mSpAttack = 65,
								.mSpDefense = 65,
								.mSpeed = 45,
							},
							.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Grass), toEggGroupID(BuiltinEggGroupID::Monster)},
							.mTypeIDs = {toTypeID(BuiltinTypeID::Grass), toTypeID(BuiltinTypeID::Poison)},
							.mPokemonID = toPokemonID(BuiltinPokemonID::Bulbasaur),
							.mLearnsetID = toLearnsetID(BuiltinLearnsetID::BulbasaurLearnset),
						},
						.mMoveMetas = {{{.mMoveID = toMoveID(BuiltinMoveID::KarateChop), .mPPMaxAmount = 40, .mPPDefaultAmount = 25}}},
						.mPokemonIVs = {5, 22, 31, 14, 20, 10},
						.mPokemonEVs = {42, 56, 0, 23, 100, 85},
						.mAbilityEntries
						= {{{.mAbilityID = toAbilityID(BuiltinAbilityID::CloudNine), .mClassification = AbilityClassification::Hidden}}},
						.mItemIDs = {toItemID(BuiltinItemID::ChestoBerry)},
						.mNatureIDs = {toNatureID(BuiltinNatureID::Calm)},
						.mHealth = 20,
						.mLevel = 5,
						.mGender = PokemonGender::Genderless,
					}),
				};

				CHECK((result.value() == expected));

				CHECK((unsignedSequence.calls() == 0));
				CHECK((signedSequence.calls() == 0));
				CHECK((realSequence.calls() == 0));
				CHECK((indexSequence.calls() == 0));
			}
		}

		GIVEN("calling with a spec partially filled out with user defined values aside from the level")
		{
			WHEN("calling with a SetLevelEncounter")
			{
				PokemonSpec newSpec{};

				ivs = {5, 22, 31, 14, 20, 10};
				evs = {42, 56, 0, 23, 100, 85};
				newSpec.mIVEVContext = {.mIVs = ivs, .mEVs = evs};

				natureIDs = {toNatureID(BuiltinNatureID::Calm)};
				abilityEntries
					= {{.mAbilityID = toAbilityID(BuiltinAbilityID::CloudNine), .mClassification = AbilityClassification::Hidden}};
				itemIDs = {toItemID(BuiltinItemID::ChestoBerry)};
				moveIDs = {toMoveID(BuiltinMoveID::KarateChop)};

				newSpec.mIDContext = {
					.mNatureIDs = natureIDs,
					.mAbilityEntries = abilityEntries,
					.mItemIDs = itemIDs,
					.mMoveIDs = moveIDs,
				};

				newSpec.mGender = PokemonGender::Genderless;
				newSpec.mBreedingContext = {};
				newSpec.mPokemonID = toPokemonID(BuiltinPokemonID::Bulbasaur);
				newSpec.mEncounterIndex = 0;

				// updatePokemon replaces all metadata, so start from the registered entry to keep its location and learnset.
				PokemonMeta updatedMeta{*registry.getPokemonMetadata(newSpec.mPokemonID)};
				updatedMeta.mEncounters.at(0).mLevel = SetLevelEncounter{20};

				std::expected<void, RegistryError> updateResult{
					configRegistryStorage->updatePokemon(newSpec.mPokemonID, updatedMeta),
				};

				REQUIRE(updateResult.has_value());

				SequenceRandom<ub> unsignedSequence{};
				SequenceRandom<sb> signedSequence{};
				SequenceRandom<double> realSequence{};
				SequenceRandom<std::size_t> indexSequence{};

				CallableRandomSource<ub, std::reference_wrapper<SequenceRandom<ub>>> unsignedSource{std::ref(unsignedSequence)};
				CallableRandomSource<sb, std::reference_wrapper<SequenceRandom<sb>>> signedSource{std::ref(signedSequence)};
				CallableRandomSource<double, std::reference_wrapper<SequenceRandom<double>>> realSource{std::ref(realSequence)};
				CallableRandomSource<std::size_t, std::reference_wrapper<SequenceRandom<std::size_t>>> indexSource{std::ref(indexSequence)};

				randomSources.mUnsigned = unsignedSource;
				randomSources.mSigned = signedSource;
				randomSources.mReal = realSource;
				randomSources.mIndex = indexSource;

				std::expected<Pokemon, PokemonInstantiationError> result{
					registry.instantiate(newSpec, instantiateDependencies, randomSources),
				};

				THEN("it returns a semi-custom built Pokemon")
				{
					REQUIRE(result.has_value());

					Pokemon expected{
    					makePokemon({
    						.mMetadata = {
    						    .mName = "Bulbasaur",
    							.mBaseStats = {
    								.mMaxHealth = 45,
    								.mAttack = 49,
    								.mDefense = 49,
    								.mSpAttack = 65,
    								.mSpDefense = 65,
    								.mSpeed = 45,
    							},
    							.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Grass), toEggGroupID(BuiltinEggGroupID::Monster)},
    							.mTypeIDs = {toTypeID(BuiltinTypeID::Grass), toTypeID(BuiltinTypeID::Poison)},
    							.mPokemonID = toPokemonID(BuiltinPokemonID::Bulbasaur),
    							.mLearnsetID = toLearnsetID(BuiltinLearnsetID::BulbasaurLearnset),
    						},
    						.mMoveMetas = {{{.mMoveID = toMoveID(BuiltinMoveID::KarateChop), .mPPMaxAmount = 40, .mPPDefaultAmount = 25}}},
    						.mPokemonIVs = {5, 22, 31, 14, 20, 10},
    						.mPokemonEVs = {42, 56, 0, 23, 100, 85},
    						.mAbilityEntries
    						= {{{.mAbilityID = toAbilityID(BuiltinAbilityID::CloudNine), .mClassification = AbilityClassification::Hidden}}},
    						.mItemIDs = {toItemID(BuiltinItemID::ChestoBerry)},
    						.mNatureIDs = {toNatureID(BuiltinNatureID::Calm)},
    						.mHealth = 51,
    						.mLevel = 20,
    						.mGender = PokemonGender::Genderless,
    					}),
    				};

					CHECK((result.value() == expected));

					CHECK((unsignedSequence.calls() == 0));
					CHECK((signedSequence.calls() == 0));
					CHECK((realSequence.calls() == 0));
					CHECK((indexSequence.calls() == 0));
				}
			}

			WHEN("calling with a VariableLevelEncounter")
			{
				PokemonSpec newSpec{};

				ivs = {5, 22, 31, 14, 20, 10};
				evs = {42, 56, 0, 23, 100, 85};
				newSpec.mIVEVContext = {.mIVs = ivs, .mEVs = evs};

				natureIDs = {toNatureID(BuiltinNatureID::Calm)};
				abilityEntries
					= {{.mAbilityID = toAbilityID(BuiltinAbilityID::CloudNine), .mClassification = AbilityClassification::Hidden}};
				itemIDs = {toItemID(BuiltinItemID::ChestoBerry)};
				moveIDs = {toMoveID(BuiltinMoveID::KarateChop)};

				newSpec.mIDContext = {
					.mNatureIDs = natureIDs,
					.mAbilityEntries = abilityEntries,
					.mItemIDs = itemIDs,
					.mMoveIDs = moveIDs,
				};

				newSpec.mGender = PokemonGender::Genderless;
				newSpec.mBreedingContext = {};
				newSpec.mPokemonID = toPokemonID(BuiltinPokemonID::Bulbasaur);
				newSpec.mEncounterIndex = 0;

				// updatePokemon replaces all metadata, so start from the registered entry to keep its location and learnset.
				PokemonMeta updatedMeta{*registry.getPokemonMetadata(newSpec.mPokemonID)};
				updatedMeta.mEncounters.at(0).mLevel = VariableLevelEncounter{.mMinLevel = 7, .mMaxLevel = 51};

				std::expected<void, RegistryError> updateResult{
					configRegistryStorage->updatePokemon(newSpec.mPokemonID, updatedMeta),
				};

				REQUIRE(updateResult.has_value());

				SequenceRandom<ub> unsignedSequence{};
				SequenceRandom<sb> signedSequence{};
				SequenceRandom<double> realSequence{};
				SequenceRandom<std::size_t> indexSequence{SequenceRandom<std::size_t>{20}};

				CallableRandomSource<ub, std::reference_wrapper<SequenceRandom<ub>>> unsignedSource{std::ref(unsignedSequence)};
				CallableRandomSource<sb, std::reference_wrapper<SequenceRandom<sb>>> signedSource{std::ref(signedSequence)};
				CallableRandomSource<double, std::reference_wrapper<SequenceRandom<double>>> realSource{std::ref(realSequence)};
				CallableRandomSource<std::size_t, std::reference_wrapper<SequenceRandom<std::size_t>>> indexSource{std::ref(indexSequence)};

				randomSources.mUnsigned = unsignedSource;
				randomSources.mSigned = signedSource;
				randomSources.mReal = realSource;
				randomSources.mIndex = indexSource;

				std::expected<Pokemon, PokemonInstantiationError> result{
					registry.instantiate(newSpec, instantiateDependencies, randomSources),
				};

				THEN("it returns a semi-custom built Pokemon")
				{
					REQUIRE(result.has_value());

					Pokemon expected{
    					makePokemon({
    						.mMetadata = {
    						    .mName = "Bulbasaur",
    							.mBaseStats = {
    								.mMaxHealth = 45,
    								.mAttack = 49,
    								.mDefense = 49,
    								.mSpAttack = 65,
    								.mSpDefense = 65,
    								.mSpeed = 45,
    							},
    							.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Grass), toEggGroupID(BuiltinEggGroupID::Monster)},
    							.mTypeIDs = {toTypeID(BuiltinTypeID::Grass), toTypeID(BuiltinTypeID::Poison)},
    							.mPokemonID = toPokemonID(BuiltinPokemonID::Bulbasaur),
    							.mLearnsetID = toLearnsetID(BuiltinLearnsetID::BulbasaurLearnset),
    						},
    						.mMoveMetas = {{{.mMoveID = toMoveID(BuiltinMoveID::KarateChop), .mPPMaxAmount = 40, .mPPDefaultAmount = 25}}},
    						.mPokemonIVs = {5, 22, 31, 14, 20, 10},
    						.mPokemonEVs = {42, 56, 0, 23, 100, 85},
    						.mAbilityEntries
    						= {{{.mAbilityID = toAbilityID(BuiltinAbilityID::CloudNine), .mClassification = AbilityClassification::Hidden}}},
    						.mItemIDs = {toItemID(BuiltinItemID::ChestoBerry)},
    						.mNatureIDs = {toNatureID(BuiltinNatureID::Calm)},
    						.mHealth = 51,
    						.mLevel = 20,
    						.mGender = PokemonGender::Genderless,
    					}),
    				};

					CHECK((result.value() == expected));

					CHECK((unsignedSequence.calls() == 0));
					CHECK((signedSequence.calls() == 0));
					CHECK((realSequence.calls() == 0));
					CHECK((indexSequence.calls() == 1));
				}
			}
		}

		WHEN("calling with a spec partially filled out with user defined values aside from the nature")
		{
			PokemonSpec newSpec{};

			ivs = {5, 22, 31, 14, 20, 10};
			evs = {42, 56, 0, 23, 100, 85};
			newSpec.mIVEVContext = {.mIVs = ivs, .mEVs = evs};

			abilityEntries = {{.mAbilityID = toAbilityID(BuiltinAbilityID::CloudNine), .mClassification = AbilityClassification::Hidden}};
			itemIDs = {toItemID(BuiltinItemID::ChestoBerry)};
			moveIDs = {toMoveID(BuiltinMoveID::KarateChop)};

			newSpec.mIDContext = {
				.mAbilityEntries = abilityEntries,
				.mItemIDs = itemIDs,
				.mMoveIDs = moveIDs,
			};

			newSpec.mGender = PokemonGender::Genderless;
			newSpec.mBreedingContext = {};
			newSpec.mLevel = 5;
			newSpec.mPokemonID = toPokemonID(BuiltinPokemonID::Bulbasaur);

			SequenceRandom<ub> unsignedSequence{};
			SequenceRandom<sb> signedSequence{};
			SequenceRandom<double> realSequence{};
			SequenceRandom<std::size_t> indexSequence{SequenceRandom<std::size_t>{1, 5}};

			CallableRandomSource<ub, std::reference_wrapper<SequenceRandom<ub>>> unsignedSource{std::ref(unsignedSequence)};
			CallableRandomSource<sb, std::reference_wrapper<SequenceRandom<sb>>> signedSource{std::ref(signedSequence)};
			CallableRandomSource<double, std::reference_wrapper<SequenceRandom<double>>> realSource{std::ref(realSequence)};
			CallableRandomSource<std::size_t, std::reference_wrapper<SequenceRandom<std::size_t>>> indexSource{std::ref(indexSequence)};

			randomSources.mUnsigned = unsignedSource;
			randomSources.mSigned = signedSource;
			randomSources.mReal = realSource;
			randomSources.mIndex = indexSource;

			std::expected<Pokemon, PokemonInstantiationError> result{registry.instantiate(newSpec, instantiateDependencies, randomSources)};

			THEN("it returns a semi-custom built Pokemon")
			{
				REQUIRE(result.has_value());

				Pokemon expected{
					makePokemon({
						.mMetadata = {
						    .mName = "Bulbasaur",
							.mBaseStats = {
								.mMaxHealth = 45,
								.mAttack = 49,
								.mDefense = 49,
								.mSpAttack = 65,
								.mSpDefense = 65,
								.mSpeed = 45,
							},
							.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Grass), toEggGroupID(BuiltinEggGroupID::Monster)},
							.mTypeIDs = {toTypeID(BuiltinTypeID::Grass), toTypeID(BuiltinTypeID::Poison)},
							.mPokemonID = toPokemonID(BuiltinPokemonID::Bulbasaur),
							.mLearnsetID = toLearnsetID(BuiltinLearnsetID::BulbasaurLearnset),
						},
						.mMoveMetas = {{{.mMoveID = toMoveID(BuiltinMoveID::KarateChop), .mPPMaxAmount = 40, .mPPDefaultAmount = 25}}},
						.mPokemonIVs = {5, 22, 31, 14, 20, 10},
						.mPokemonEVs = {42, 56, 0, 23, 100, 85},
						.mAbilityEntries
						= {{{.mAbilityID = toAbilityID(BuiltinAbilityID::CloudNine), .mClassification = AbilityClassification::Hidden}}},
						.mItemIDs = {toItemID(BuiltinItemID::ChestoBerry)},
						.mNatureIDs = {toNatureID(BuiltinNatureID::Bold)},
						.mHealth = 20,
						.mLevel = 5,
						.mGender = PokemonGender::Genderless,
					}),
				};

				CHECK((result.value() == expected));

				CHECK((unsignedSequence.calls() == 0));
				CHECK((signedSequence.calls() == 0));
				CHECK((realSequence.calls() == 0));
				CHECK((indexSequence.calls() == 2));
			}
		}

		WHEN("calling with a spec partially filled out with user defined values aside from the ability")
		{
			PokemonSpec newSpec{};

			ivs = {5, 22, 31, 14, 20, 10};
			evs = {42, 56, 0, 23, 100, 85};
			newSpec.mIVEVContext = {.mIVs = ivs, .mEVs = evs};

			natureIDs = {toNatureID(BuiltinNatureID::Calm)};
			itemIDs = {toItemID(BuiltinItemID::ChestoBerry)};
			moveIDs = {toMoveID(BuiltinMoveID::KarateChop)};

			newSpec.mIDContext = {
				.mNatureIDs = natureIDs,
				.mItemIDs = itemIDs,
				.mMoveIDs = moveIDs,
			};

			newSpec.mGender = PokemonGender::Genderless;
			newSpec.mBreedingContext = {};
			newSpec.mLevel = 5;
			newSpec.mPokemonID = toPokemonID(BuiltinPokemonID::Bulbasaur);

			SequenceRandom<ub> unsignedSequence{};
			SequenceRandom<sb> signedSequence{};
			SequenceRandom<double> realSequence{};
			SequenceRandom<std::size_t> indexSequence{SequenceRandom<std::size_t>{1, 1}};

			CallableRandomSource<ub, std::reference_wrapper<SequenceRandom<ub>>> unsignedSource{std::ref(unsignedSequence)};
			CallableRandomSource<sb, std::reference_wrapper<SequenceRandom<sb>>> signedSource{std::ref(signedSequence)};
			CallableRandomSource<double, std::reference_wrapper<SequenceRandom<double>>> realSource{std::ref(realSequence)};
			CallableRandomSource<std::size_t, std::reference_wrapper<SequenceRandom<std::size_t>>> indexSource{std::ref(indexSequence)};

			randomSources.mUnsigned = unsignedSource;
			randomSources.mSigned = signedSource;
			randomSources.mReal = realSource;
			randomSources.mIndex = indexSource;

			std::expected<Pokemon, PokemonInstantiationError> result{registry.instantiate(newSpec, instantiateDependencies, randomSources)};

			THEN("it returns a semi-custom built Pokemon")
			{
				REQUIRE(result.has_value());

				Pokemon expected{
					makePokemon({
						.mMetadata = {
						    .mName = "Bulbasaur",
							.mBaseStats = {
								.mMaxHealth = 45,
								.mAttack = 49,
								.mDefense = 49,
								.mSpAttack = 65,
								.mSpDefense = 65,
								.mSpeed = 45,
							},
							.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Grass), toEggGroupID(BuiltinEggGroupID::Monster)},
							.mTypeIDs = {toTypeID(BuiltinTypeID::Grass), toTypeID(BuiltinTypeID::Poison)},
							.mPokemonID = toPokemonID(BuiltinPokemonID::Bulbasaur),
							.mLearnsetID = toLearnsetID(BuiltinLearnsetID::BulbasaurLearnset),
						},
						.mMoveMetas = {{{.mMoveID = toMoveID(BuiltinMoveID::KarateChop), .mPPMaxAmount = 40, .mPPDefaultAmount = 25}}},
						.mPokemonIVs = {5, 22, 31, 14, 20, 10},
						.mPokemonEVs = {42, 56, 0, 23, 100, 85},
						.mAbilityEntries
						= {{{.mAbilityID = toAbilityID(BuiltinAbilityID::Chlorophyll), .mClassification = AbilityClassification::Hidden}}},
						.mItemIDs = {toItemID(BuiltinItemID::ChestoBerry)},
						.mNatureIDs = {toNatureID(BuiltinNatureID::Calm)},
						.mHealth = 20,
						.mLevel = 5,
						.mGender = PokemonGender::Genderless,
					}),
				};

				CHECK((result.value() == expected));

				CHECK((unsignedSequence.calls() == 0));
				CHECK((signedSequence.calls() == 0));
				CHECK((realSequence.calls() == 0));
				CHECK((indexSequence.calls() == 2));
			}
		}

		WHEN("calling with a spec partially filled out with user defined values aside from the item")
		{
			PokemonSpec newSpec{};

			ivs = {5, 22, 31, 14, 20, 10};
			evs = {42, 56, 0, 23, 100, 85};
			newSpec.mIVEVContext = {.mIVs = ivs, .mEVs = evs};

			natureIDs = {toNatureID(BuiltinNatureID::Calm)};
			abilityEntries = {{.mAbilityID = toAbilityID(BuiltinAbilityID::CloudNine), .mClassification = AbilityClassification::Hidden}};
			moveIDs = {toMoveID(BuiltinMoveID::KarateChop)};

			newSpec.mIDContext = {
				.mNatureIDs = natureIDs,
				.mAbilityEntries = abilityEntries,
				.mMoveIDs = moveIDs,
			};

			newSpec.mGender = PokemonGender::Genderless;
			newSpec.mBreedingContext = {};
			newSpec.mLevel = 5;
			newSpec.mPokemonID = toPokemonID(BuiltinPokemonID::Bulbasaur);

			// updatePokemon replaces all metadata, so start from the registered entry to keep its location and learnset.
			PokemonMeta updatedMeta{*registry.getPokemonMetadata(newSpec.mPokemonID)};
			updatedMeta.mItemPool.at(0) = {.mItemID = toItemID(BuiltinItemID::CheriBerry), .mChance = 0.10};

			std::expected<void, RegistryError> updateResult{
				configRegistryStorage->updatePokemon(newSpec.mPokemonID, updatedMeta),
			};

			REQUIRE(updateResult.has_value());

			SequenceRandom<ub> unsignedSequence{};
			SequenceRandom<sb> signedSequence{};
			SequenceRandom<double> realSequence{SequenceRandom<double>{0.05}};
			SequenceRandom<std::size_t> indexSequence{SequenceRandom<std::size_t>{0}};

			CallableRandomSource<ub, std::reference_wrapper<SequenceRandom<ub>>> unsignedSource{std::ref(unsignedSequence)};
			CallableRandomSource<sb, std::reference_wrapper<SequenceRandom<sb>>> signedSource{std::ref(signedSequence)};
			CallableRandomSource<double, std::reference_wrapper<SequenceRandom<double>>> realSource{std::ref(realSequence)};
			CallableRandomSource<std::size_t, std::reference_wrapper<SequenceRandom<std::size_t>>> indexSource{std::ref(indexSequence)};

			randomSources.mUnsigned = unsignedSource;
			randomSources.mSigned = signedSource;
			randomSources.mReal = realSource;
			randomSources.mIndex = indexSource;

			std::expected<Pokemon, PokemonInstantiationError> result{registry.instantiate(newSpec, instantiateDependencies, randomSources)};

			THEN("it returns a semi-custom built Pokemon")
			{
				REQUIRE(result.has_value());

				Pokemon expected{
					makePokemon({
						.mMetadata = {
						    .mName = "Bulbasaur",
							.mBaseStats = {
								.mMaxHealth = 45,
								.mAttack = 49,
								.mDefense = 49,
								.mSpAttack = 65,
								.mSpDefense = 65,
								.mSpeed = 45,
							},
							.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Grass), toEggGroupID(BuiltinEggGroupID::Monster)},
							.mTypeIDs = {toTypeID(BuiltinTypeID::Grass), toTypeID(BuiltinTypeID::Poison)},
							.mPokemonID = toPokemonID(BuiltinPokemonID::Bulbasaur),
							.mLearnsetID = toLearnsetID(BuiltinLearnsetID::BulbasaurLearnset),
						},
						.mMoveMetas = {{{.mMoveID = toMoveID(BuiltinMoveID::KarateChop), .mPPMaxAmount = 40, .mPPDefaultAmount = 25}}},
						.mPokemonIVs = {5, 22, 31, 14, 20, 10},
						.mPokemonEVs = {42, 56, 0, 23, 100, 85},
						.mAbilityEntries
						= {{{.mAbilityID = toAbilityID(BuiltinAbilityID::CloudNine), .mClassification = AbilityClassification::Hidden}}},
						.mItemIDs = {toItemID(BuiltinItemID::CheriBerry)},
						.mNatureIDs = {toNatureID(BuiltinNatureID::Calm)},
						.mHealth = 20,
						.mLevel = 5,
						.mGender = PokemonGender::Genderless,
					}),
				};

				CHECK((result.value() == expected));

				CHECK((unsignedSequence.calls() == 0));
				CHECK((signedSequence.calls() == 0));
				CHECK((realSequence.calls() == 1));
				CHECK((indexSequence.calls() == 1));
			}
		}

		WHEN("calling with a spec partially filled out with user defined values aside from the move")
		{
			PokemonSpec newSpec{};

			ivs = {5, 22, 31, 14, 20, 10};
			evs = {42, 56, 0, 23, 100, 85};
			newSpec.mIVEVContext = {.mIVs = ivs, .mEVs = evs};

			natureIDs = {toNatureID(BuiltinNatureID::Calm)};
			abilityEntries = {{.mAbilityID = toAbilityID(BuiltinAbilityID::CloudNine), .mClassification = AbilityClassification::Hidden}};
			itemIDs = {toItemID(BuiltinItemID::ChestoBerry)};

			newSpec.mIDContext = {
				.mNatureIDs = natureIDs,
				.mAbilityEntries = abilityEntries,
				.mItemIDs = itemIDs,
			};

			newSpec.mGender = PokemonGender::Genderless;
			newSpec.mBreedingContext = {};
			newSpec.mLevel = 5;
			newSpec.mPokemonID = toPokemonID(BuiltinPokemonID::Bulbasaur);

			SequenceRandom<ub> unsignedSequence{};
			SequenceRandom<sb> signedSequence{};
			SequenceRandom<double> realSequence{};
			SequenceRandom<std::size_t> indexSequence{};

			CallableRandomSource<ub, std::reference_wrapper<SequenceRandom<ub>>> unsignedSource{std::ref(unsignedSequence)};
			CallableRandomSource<sb, std::reference_wrapper<SequenceRandom<sb>>> signedSource{std::ref(signedSequence)};
			CallableRandomSource<double, std::reference_wrapper<SequenceRandom<double>>> realSource{std::ref(realSequence)};
			CallableRandomSource<std::size_t, std::reference_wrapper<SequenceRandom<std::size_t>>> indexSource{std::ref(indexSequence)};

			randomSources.mUnsigned = unsignedSource;
			randomSources.mSigned = signedSource;
			randomSources.mReal = realSource;
			randomSources.mIndex = indexSource;

			std::expected<Pokemon, PokemonInstantiationError> result{registry.instantiate(newSpec, instantiateDependencies, randomSources)};

			THEN("it returns a semi-custom built Pokemon")
			{
				REQUIRE(result.has_value());

				Pokemon expected{
					makePokemon({
						.mMetadata = {
						    .mName = "Bulbasaur",
							.mBaseStats = {
								.mMaxHealth = 45,
								.mAttack = 49,
								.mDefense = 49,
								.mSpAttack = 65,
								.mSpDefense = 65,
								.mSpeed = 45,
							},
							.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Grass), toEggGroupID(BuiltinEggGroupID::Monster)},
							.mTypeIDs = {toTypeID(BuiltinTypeID::Grass), toTypeID(BuiltinTypeID::Poison)},
							.mPokemonID = toPokemonID(BuiltinPokemonID::Bulbasaur),
							.mLearnsetID = toLearnsetID(BuiltinLearnsetID::BulbasaurLearnset),
						},
						.mMoveMetas = {{
							{.mMoveID = toMoveID(BuiltinMoveID::Tackle), .mPPMaxAmount = 56, .mPPDefaultAmount = 35},
						    {.mMoveID = toMoveID(BuiltinMoveID::Growl), .mPPMaxAmount = 64, .mPPDefaultAmount = 40},
							{.mMoveID = toMoveID(BuiltinMoveID::VineWhip), .mPPMaxAmount = 40, .mPPDefaultAmount = 25},
						},},
						.mPokemonIVs = {5, 22, 31, 14, 20, 10},
						.mPokemonEVs = {42, 56, 0, 23, 100, 85},
						.mAbilityEntries
						= {{{.mAbilityID = toAbilityID(BuiltinAbilityID::CloudNine), .mClassification = AbilityClassification::Hidden}}},
						.mItemIDs = {toItemID(BuiltinItemID::ChestoBerry)},
						.mNatureIDs = {toNatureID(BuiltinNatureID::Calm)},
						.mHealth = 20,
						.mLevel = 5,
						.mGender = PokemonGender::Genderless,
					}),
				};

				CHECK((result.value() == expected));

				CHECK((unsignedSequence.calls() == 0));
				CHECK((signedSequence.calls() == 0));
				CHECK((realSequence.calls() == 0));
				CHECK((indexSequence.calls() == 0));
			}
		}

		WHEN("calling with a spec partially filled out with user defined values aside from the IVs")
		{
			PokemonSpec newSpec{};

			evs = {42, 56, 0, 23, 100, 85};
			newSpec.mIVEVContext = {.mEVs = evs};

			natureIDs = {toNatureID(BuiltinNatureID::Calm)};
			abilityEntries = {{.mAbilityID = toAbilityID(BuiltinAbilityID::CloudNine), .mClassification = AbilityClassification::Hidden}};
			itemIDs = {toItemID(BuiltinItemID::ChestoBerry)};
			moveIDs = {toMoveID(BuiltinMoveID::KarateChop)};

			newSpec.mIDContext = {
				.mNatureIDs = natureIDs,
				.mAbilityEntries = abilityEntries,
				.mItemIDs = itemIDs,
				.mMoveIDs = moveIDs,
			};

			newSpec.mGender = PokemonGender::Genderless;
			newSpec.mBreedingContext = {};
			newSpec.mLevel = 5;
			newSpec.mPokemonID = toPokemonID(BuiltinPokemonID::Bulbasaur);

			SequenceRandom<ub> unsignedSequence{};
			SequenceRandom<sb> signedSequence{};
			SequenceRandom<double> realSequence{};
			SequenceRandom<std::size_t> indexSequence{SequenceRandom<std::size_t>{8, 7, 15, 24, 31, 27}};

			CallableRandomSource<ub, std::reference_wrapper<SequenceRandom<ub>>> unsignedSource{std::ref(unsignedSequence)};
			CallableRandomSource<sb, std::reference_wrapper<SequenceRandom<sb>>> signedSource{std::ref(signedSequence)};
			CallableRandomSource<double, std::reference_wrapper<SequenceRandom<double>>> realSource{std::ref(realSequence)};
			CallableRandomSource<std::size_t, std::reference_wrapper<SequenceRandom<std::size_t>>> indexSource{std::ref(indexSequence)};

			randomSources.mUnsigned = unsignedSource;
			randomSources.mSigned = signedSource;
			randomSources.mReal = realSource;
			randomSources.mIndex = indexSource;

			std::expected<Pokemon, PokemonInstantiationError> result{registry.instantiate(newSpec, instantiateDependencies, randomSources)};

			THEN("it returns a semi-custom built Pokemon")
			{
				REQUIRE(result.has_value());

				Pokemon expected{
					makePokemon({
						.mMetadata = {
						    .mName = "Bulbasaur",
							.mBaseStats = {
								.mMaxHealth = 45,
								.mAttack = 49,
								.mDefense = 49,
								.mSpAttack = 65,
								.mSpDefense = 65,
								.mSpeed = 45,
							},
							.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Grass), toEggGroupID(BuiltinEggGroupID::Monster)},
							.mTypeIDs = {toTypeID(BuiltinTypeID::Grass), toTypeID(BuiltinTypeID::Poison)},
							.mPokemonID = toPokemonID(BuiltinPokemonID::Bulbasaur),
							.mLearnsetID = toLearnsetID(BuiltinLearnsetID::BulbasaurLearnset),
						},
						.mMoveMetas = {{{.mMoveID = toMoveID(BuiltinMoveID::KarateChop), .mPPMaxAmount = 40, .mPPDefaultAmount = 25}}},
						.mPokemonIVs = {8, 7, 15, 24, 31, 27},
						.mPokemonEVs = {42, 56, 0, 23, 100, 85},
						.mAbilityEntries
						= {{{.mAbilityID = toAbilityID(BuiltinAbilityID::CloudNine), .mClassification = AbilityClassification::Hidden}}},
						.mItemIDs = {toItemID(BuiltinItemID::ChestoBerry)},
						.mNatureIDs = {toNatureID(BuiltinNatureID::Calm)},
						.mHealth = 20,
						.mLevel = 5,
						.mGender = PokemonGender::Genderless,
					}),
				};

				CHECK((result.value() == expected));

				CHECK((unsignedSequence.calls() == 0));
				CHECK((signedSequence.calls() == 0));
				CHECK((realSequence.calls() == 0));
				CHECK((indexSequence.calls() == 6));
			}
		}

		WHEN("calling with a spec partially filled out with user defined values aside from the EVs")
		{
			PokemonSpec newSpec{};

			ivs = {5, 22, 31, 14, 20, 10};
			newSpec.mIVEVContext = {.mIVs = ivs};

			natureIDs = {toNatureID(BuiltinNatureID::Calm)};
			abilityEntries = {{.mAbilityID = toAbilityID(BuiltinAbilityID::CloudNine), .mClassification = AbilityClassification::Hidden}};
			itemIDs = {toItemID(BuiltinItemID::ChestoBerry)};
			moveIDs = {toMoveID(BuiltinMoveID::KarateChop)};

			newSpec.mIDContext = {
				.mNatureIDs = natureIDs,
				.mAbilityEntries = abilityEntries,
				.mItemIDs = itemIDs,
				.mMoveIDs = moveIDs,
			};

			newSpec.mGender = PokemonGender::Genderless;
			newSpec.mBreedingContext = {};
			newSpec.mLevel = 5;
			newSpec.mPokemonID = toPokemonID(BuiltinPokemonID::Bulbasaur);

			SequenceRandom<ub> unsignedSequence{};
			SequenceRandom<sb> signedSequence{};
			SequenceRandom<double> realSequence{};
			SequenceRandom<std::size_t> indexSequence{};

			CallableRandomSource<ub, std::reference_wrapper<SequenceRandom<ub>>> unsignedSource{std::ref(unsignedSequence)};
			CallableRandomSource<sb, std::reference_wrapper<SequenceRandom<sb>>> signedSource{std::ref(signedSequence)};
			CallableRandomSource<double, std::reference_wrapper<SequenceRandom<double>>> realSource{std::ref(realSequence)};
			CallableRandomSource<std::size_t, std::reference_wrapper<SequenceRandom<std::size_t>>> indexSource{std::ref(indexSequence)};

			randomSources.mUnsigned = unsignedSource;
			randomSources.mSigned = signedSource;
			randomSources.mReal = realSource;
			randomSources.mIndex = indexSource;

			std::expected<Pokemon, PokemonInstantiationError> result{registry.instantiate(newSpec, instantiateDependencies, randomSources)};

			THEN("it returns a semi-custom built Pokemon")
			{
				REQUIRE(result.has_value());

				Pokemon expected{
					makePokemon({
						.mMetadata = {
						    .mName = "Bulbasaur",
							.mBaseStats = {
								.mMaxHealth = 45,
								.mAttack = 49,
								.mDefense = 49,
								.mSpAttack = 65,
								.mSpDefense = 65,
								.mSpeed = 45,
							},
							.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Grass), toEggGroupID(BuiltinEggGroupID::Monster)},
							.mTypeIDs = {toTypeID(BuiltinTypeID::Grass), toTypeID(BuiltinTypeID::Poison)},
							.mPokemonID = toPokemonID(BuiltinPokemonID::Bulbasaur),
							.mLearnsetID = toLearnsetID(BuiltinLearnsetID::BulbasaurLearnset),
						},
						.mMoveMetas = {{{.mMoveID = toMoveID(BuiltinMoveID::KarateChop), .mPPMaxAmount = 40, .mPPDefaultAmount = 25}}},
						.mPokemonIVs = {5, 22, 31, 14, 20, 10},
						.mPokemonEVs = {0, 0, 0, 0, 0, 0},
						.mAbilityEntries
						= {{{.mAbilityID = toAbilityID(BuiltinAbilityID::CloudNine), .mClassification = AbilityClassification::Hidden}}},
						.mItemIDs = {toItemID(BuiltinItemID::ChestoBerry)},
						.mNatureIDs = {toNatureID(BuiltinNatureID::Calm)},
						.mHealth = 20,
						.mLevel = 5,
						.mGender = PokemonGender::Genderless,
					}),
				};

				CHECK((result.value() == expected));

				CHECK((unsignedSequence.calls() == 0));
				CHECK((signedSequence.calls() == 0));
				CHECK((realSequence.calls() == 0));
				CHECK((indexSequence.calls() == 0));
			}
		}

		WHEN("calling with a spec partially filled out with user defined values aside from the gender")
		{
			PokemonSpec newSpec{};

			ivs = {5, 22, 31, 14, 20, 10};
			evs = {42, 56, 0, 23, 100, 85};
			newSpec.mIVEVContext = {.mIVs = ivs, .mEVs = evs};

			natureIDs = {toNatureID(BuiltinNatureID::Calm)};
			abilityEntries = {{.mAbilityID = toAbilityID(BuiltinAbilityID::CloudNine), .mClassification = AbilityClassification::Hidden}};
			itemIDs = {toItemID(BuiltinItemID::ChestoBerry)};
			moveIDs = {toMoveID(BuiltinMoveID::KarateChop)};

			newSpec.mIDContext = {
				.mNatureIDs = natureIDs,
				.mAbilityEntries = abilityEntries,
				.mItemIDs = itemIDs,
				.mMoveIDs = moveIDs,
			};

			newSpec.mBreedingContext = {};
			newSpec.mLevel = 5;
			newSpec.mPokemonID = toPokemonID(BuiltinPokemonID::Bulbasaur);

			SequenceRandom<ub> unsignedSequence{};
			SequenceRandom<sb> signedSequence{};
			SequenceRandom<double> realSequence{SequenceRandom<double>{0.9}};
			SequenceRandom<std::size_t> indexSequence{};

			CallableRandomSource<ub, std::reference_wrapper<SequenceRandom<ub>>> unsignedSource{std::ref(unsignedSequence)};
			CallableRandomSource<sb, std::reference_wrapper<SequenceRandom<sb>>> signedSource{std::ref(signedSequence)};
			CallableRandomSource<double, std::reference_wrapper<SequenceRandom<double>>> realSource{std::ref(realSequence)};
			CallableRandomSource<std::size_t, std::reference_wrapper<SequenceRandom<std::size_t>>> indexSource{std::ref(indexSequence)};

			randomSources.mUnsigned = unsignedSource;
			randomSources.mSigned = signedSource;
			randomSources.mReal = realSource;
			randomSources.mIndex = indexSource;

			std::expected<Pokemon, PokemonInstantiationError> result{registry.instantiate(newSpec, instantiateDependencies, randomSources)};

			THEN("it returns a semi-custom built Pokemon")
			{
				REQUIRE(result.has_value());

				Pokemon expected{
					makePokemon({
						.mMetadata = {
						    .mName = "Bulbasaur",
							.mBaseStats = {
								.mMaxHealth = 45,
								.mAttack = 49,
								.mDefense = 49,
								.mSpAttack = 65,
								.mSpDefense = 65,
								.mSpeed = 45,
							},
							.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Grass), toEggGroupID(BuiltinEggGroupID::Monster)},
							.mTypeIDs = {toTypeID(BuiltinTypeID::Grass), toTypeID(BuiltinTypeID::Poison)},
							.mPokemonID = toPokemonID(BuiltinPokemonID::Bulbasaur),
							.mLearnsetID = toLearnsetID(BuiltinLearnsetID::BulbasaurLearnset),
						},
						.mMoveMetas = {{{.mMoveID = toMoveID(BuiltinMoveID::KarateChop), .mPPMaxAmount = 40, .mPPDefaultAmount = 25}}},
						.mPokemonIVs = {5, 22, 31, 14, 20, 10},
						.mPokemonEVs = {42, 56, 0, 23, 100, 85},
						.mAbilityEntries
						= {{{.mAbilityID = toAbilityID(BuiltinAbilityID::CloudNine), .mClassification = AbilityClassification::Hidden}}},
						.mItemIDs = {toItemID(BuiltinItemID::ChestoBerry)},
						.mNatureIDs = {toNatureID(BuiltinNatureID::Calm)},
						.mHealth = 20,
						.mLevel = 5,
						.mGender = PokemonGender::Female,
					}),
				};

				CHECK((result.value() == expected));

				CHECK((unsignedSequence.calls() == 0));
				CHECK((signedSequence.calls() == 0));
				CHECK((realSequence.calls() == 1));
				CHECK((indexSequence.calls() == 0));
			}
		}

		WHEN("calling with a spec will randomly create all values")
		{
			PokemonSpec newSpec{};
			newSpec.mBreedingContext = {};
			newSpec.mPokemonID = toPokemonID(BuiltinPokemonID::Bulbasaur);
			newSpec.mEncounterIndex = 0;

			// updatePokemon replaces all metadata, so start from the registered entry to keep its location and learnset.
			PokemonMeta updatedMeta{*registry.getPokemonMetadata(newSpec.mPokemonID)};
			updatedMeta.mItemPool.at(0) = {.mItemID = toItemID(BuiltinItemID::CheriBerry), .mChance = 0.10};
			updatedMeta.mEncounters.at(0).mLevel = SetLevelEncounter{20};

			std::expected<void, RegistryError> updateResult{
				configRegistryStorage->updatePokemon(newSpec.mPokemonID, updatedMeta),
			};

			REQUIRE(updateResult.has_value());

			SequenceRandom<ub> unsignedSequence{};
			SequenceRandom<sb> signedSequence{};
			SequenceRandom<double> realSequence{SequenceRandom<double>{0.05, 0.9}};
			SequenceRandom<std::size_t> indexSequence{SequenceRandom<std::size_t>{1, 5, 0, 8, 7, 15, 24, 31, 27}};

			CallableRandomSource<ub, std::reference_wrapper<SequenceRandom<ub>>> unsignedSource{std::ref(unsignedSequence)};
			CallableRandomSource<sb, std::reference_wrapper<SequenceRandom<sb>>> signedSource{std::ref(signedSequence)};
			CallableRandomSource<double, std::reference_wrapper<SequenceRandom<double>>> realSource{std::ref(realSequence)};
			CallableRandomSource<std::size_t, std::reference_wrapper<SequenceRandom<std::size_t>>> indexSource{std::ref(indexSequence)};

			randomSources.mUnsigned = unsignedSource;
			randomSources.mSigned = signedSource;
			randomSources.mReal = realSource;
			randomSources.mIndex = indexSource;

			std::expected<Pokemon, PokemonInstantiationError> result{registry.instantiate(newSpec, instantiateDependencies, randomSources)};

			THEN("it returns a randomly built Pokemon")
			{
				REQUIRE(result.has_value());

				Pokemon expected{
					makePokemon({
						.mMetadata = {
						    .mName = "Bulbasaur",
							.mBaseStats = {
								.mMaxHealth = 45,
								.mAttack = 49,
								.mDefense = 49,
								.mSpAttack = 65,
								.mSpDefense = 65,
								.mSpeed = 45,
							},
							.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Grass), toEggGroupID(BuiltinEggGroupID::Monster)},
							.mTypeIDs = {toTypeID(BuiltinTypeID::Grass), toTypeID(BuiltinTypeID::Poison)},
							.mPokemonID = toPokemonID(BuiltinPokemonID::Bulbasaur),
							.mLearnsetID = toLearnsetID(BuiltinLearnsetID::BulbasaurLearnset),
						},
						.mMoveMetas = {{
							{.mMoveID = toMoveID(BuiltinMoveID::Tackle), .mPPMaxAmount = 56, .mPPDefaultAmount = 35},
						    {.mMoveID = toMoveID(BuiltinMoveID::Growl), .mPPMaxAmount = 64, .mPPDefaultAmount = 40},
							{.mMoveID = toMoveID(BuiltinMoveID::VineWhip), .mPPMaxAmount = 40, .mPPDefaultAmount = 25},
						},},
						.mPokemonIVs = {8, 7, 15, 24, 31, 27},
						.mPokemonEVs = {0, 0, 0, 0, 0, 0},
						.mAbilityEntries
						= {{{.mAbilityID = toAbilityID(BuiltinAbilityID::Chlorophyll), .mClassification = AbilityClassification::Hidden}}},
						.mItemIDs = {toItemID(BuiltinItemID::CheriBerry)},
						.mNatureIDs = {toNatureID(BuiltinNatureID::Bold)},
						.mHealth = 51,
						.mLevel = 20,
						.mGender = PokemonGender::Female,
					}),
				};

				CHECK((result.value() == expected));

				CHECK((unsignedSequence.calls() == 0));
				CHECK((signedSequence.calls() == 0));
				CHECK((realSequence.calls() == 2));
				CHECK((indexSequence.calls() == 9));
			}
		}

		WHEN("calling with a known example from Bulbapedia")
		{
			PokemonSpec newSpec{};

			ivs = {24, 12, 30, 16, 23, 5};
			evs = {74, 190, 91, 48, 84, 23};
			newSpec.mIVEVContext = {.mIVs = ivs, .mEVs = evs};

			natureIDs = {toNatureID(BuiltinNatureID::Adamant)};
			abilityEntries = {{.mAbilityID = toAbilityID(BuiltinAbilityID::SandVeil), .mClassification = AbilityClassification::Standard}};
			itemIDs = {toItemID(BuiltinItemID::ChestoBerry)};
			moveIDs = {};

			newSpec.mIDContext = {
				.mNatureIDs = natureIDs,
				.mAbilityEntries = abilityEntries,
				.mItemIDs = itemIDs,
			};

			newSpec.mGender = PokemonGender::Male;
			newSpec.mBreedingContext = {};
			newSpec.mLevel = 78;
			newSpec.mPokemonID = toPokemonID(BuiltinPokemonID::Garchomp);

			SequenceRandom<ub> unsignedSequence{};
			SequenceRandom<sb> signedSequence{};
			SequenceRandom<double> realSequence{};
			SequenceRandom<std::size_t> indexSequence{};

			CallableRandomSource<ub, std::reference_wrapper<SequenceRandom<ub>>> unsignedSource{std::ref(unsignedSequence)};
			CallableRandomSource<sb, std::reference_wrapper<SequenceRandom<sb>>> signedSource{std::ref(signedSequence)};
			CallableRandomSource<double, std::reference_wrapper<SequenceRandom<double>>> realSource{std::ref(realSequence)};
			CallableRandomSource<std::size_t, std::reference_wrapper<SequenceRandom<std::size_t>>> indexSource{std::ref(indexSequence)};

			randomSources.mUnsigned = unsignedSource;
			randomSources.mSigned = signedSource;
			randomSources.mReal = realSource;
			randomSources.mIndex = indexSource;

			std::expected<Pokemon, PokemonInstantiationError> result{registry.instantiate(newSpec, instantiateDependencies, randomSources)};

			THEN("it returns a custom built Pokemon")
			{
				REQUIRE(result.has_value());

				Pokemon expected{
    				makePokemon({
    					.mMetadata = {
    					    .mName = "Garchomp",
    						.mBaseStats = {
       							.mMaxHealth = 108,
                        		.mAttack = 130,
                        		.mDefense = 95,
                        		.mSpAttack = 80,
                        		.mSpDefense = 85,
                        		.mSpeed = 102,
    						},
    						.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Dragon), toEggGroupID(BuiltinEggGroupID::Monster)},
    						.mTypeIDs = {toTypeID(BuiltinTypeID::Dragon), toTypeID(BuiltinTypeID::Ground)},
    						.mPokemonID = toPokemonID(BuiltinPokemonID::Garchomp),
    						.mLearnsetID = toLearnsetID(BuiltinLearnsetID::GarchompLearnset),
    					},
    					.mMoveMetas = {
    					    {
    					        {.mMoveID = toMoveID(BuiltinMoveID::Tackle), .mPPMaxAmount = 56, .mPPDefaultAmount = 35},
    	                        {.mMoveID = toMoveID(BuiltinMoveID::SandTomb), .mPPMaxAmount = 24, .mPPDefaultAmount = 15},
    					    },
    					},
    					.mPokemonIVs = {24, 12, 30, 16, 23, 5},
    					.mPokemonEVs = {74, 190, 91, 48, 84, 23},
    					.mAbilityEntries
    					= {{{.mAbilityID = toAbilityID(BuiltinAbilityID::SandVeil), .mClassification = AbilityClassification::Standard}}},
    					.mItemIDs = {toItemID(BuiltinItemID::ChestoBerry)},
    					.mNatureIDs = {toNatureID(BuiltinNatureID::Adamant)},
    					.mHealth = 289,
    					.mLevel = 78,
    					.mGender = PokemonGender::Male,
    				}),
    			};

				CHECK((result.value() == expected));
				CHECK(checkStats(result.value(), 289, 289, 278, 193, 135, 171, 171));

				CHECK((unsignedSequence.calls() == 0));
				CHECK((signedSequence.calls() == 0));
				CHECK((realSequence.calls() == 0));
				CHECK((indexSequence.calls() == 0));
			}
		}
	}

	WHEN("operator==")
	{
		GIVEN("two default constructed registries")
		{
			std::unique_ptr<PokemonRegistry> other{std::make_unique<PokemonRegistry>()};

			THEN("they are equal")
			{
				CHECK((registry == *other));
			}
		}

		GIVEN("for an entry added in one registry")
		{
			std::unique_ptr<PokemonRegistryConfiguration> other{std::make_unique<PokemonRegistryConfiguration>()};
			std::expected<PokemonID, RegistryError> result{other->addPokemon({.mName = "test"})};

			THEN("they are not equal")
			{
				REQUIRE(result.has_value());
				CHECK((registry != other->getRuntimeRegistry()));
			}
		}

		GIVEN("for an entry removed in one registry")
		{
			std::unique_ptr<PokemonRegistryConfiguration> other{std::make_unique<PokemonRegistryConfiguration>()};
			std::expected<PokemonID, RegistryError> result{other->removePokemon(toPokemonID(BuiltinPokemonID::None))};

			THEN("they are not equal")
			{
				REQUIRE(result.has_value());
				CHECK((registry != other->getRuntimeRegistry()));
			}
		}

		GIVEN("for an entry modified in one registry")
		{
			std::unique_ptr<PokemonRegistryConfiguration> other{std::make_unique<PokemonRegistryConfiguration>()};
			std::expected<void, RegistryError> result{
				other->updatePokemon(toPokemonID(BuiltinPokemonID::None), {.mName = "test"}),
			};

			THEN("they are not equal")
			{
				REQUIRE(result.has_value());
				CHECK((registry != other->getRuntimeRegistry()));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
