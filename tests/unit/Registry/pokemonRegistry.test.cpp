/*! @file pokemonRegistry.test.cpp
	@brief C++ file for running tests for the PokemonRegistry.
	@date 10/28/2026
	@since 0.12.20
	@version 0.12.53
	@author Matthew Moore
*/

#include "Registry/pokemonRegistry.h"

#include <expected>
#include <memory>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <utility>

#include "Ability/builtInAbilityID.h"
#include "Configuration/pokemonRegistryConfiguration.h"
#include "Core/typedefs.h"
#include "EggGroup/builtInEggGroupID.h"
#include "Learnset/builtInLearnsetID.h"
#include "Pokemon/builtInPokemonID.h"
#include "Pokemon/constants.h"
#include "Pokemon/pokemonID.h"
#include "Pokemon/pokemonMeta.h"
#include "Type/builtInTypeID.h"
#include "Validation/Registry/registryError.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Ability::BuiltinAbilityID;
using PocketCore::Ability::toAbilityID;
using PocketCore::Configuration::PokemonRegistryConfiguration;
using PocketCore::Core::ub;
using PocketCore::EggGroup::BuiltinEggGroupID;
using PocketCore::EggGroup::toEggGroupID;
using PocketCore::Learnset::BuiltinLearnsetID;
using PocketCore::Learnset::toLearnsetID;
using PocketCore::Pokemon::BLASTOISE_BASE_STATS;
using PocketCore::Pokemon::BuiltinPokemonID;
using PocketCore::Pokemon::BULBASAUR_BASE_STATS;
using PocketCore::Pokemon::CHARIZARD_BASE_STATS;
using PocketCore::Pokemon::CHARMANDER_BASE_STATS;
using PocketCore::Pokemon::CHARMELEON_BASE_STATS;
using PocketCore::Pokemon::DITTO_BASE_STATS;
using PocketCore::Pokemon::IVYSAUR_BASE_STATS;
using PocketCore::Pokemon::NO_POKEMON_ID;
using PocketCore::Pokemon::POKEMON_NAME_BLASTOISE;
using PocketCore::Pokemon::POKEMON_NAME_BULBASAUR;
using PocketCore::Pokemon::POKEMON_NAME_CHARIZARD;
using PocketCore::Pokemon::POKEMON_NAME_CHARMANDER;
using PocketCore::Pokemon::POKEMON_NAME_CHARMELEON;
using PocketCore::Pokemon::POKEMON_NAME_DITTO;
using PocketCore::Pokemon::POKEMON_NAME_IVYSAUR;
using PocketCore::Pokemon::POKEMON_NAME_NONE;
using PocketCore::Pokemon::POKEMON_NAME_SQUIRTLE;
using PocketCore::Pokemon::POKEMON_NAME_VENUSAUR;
using PocketCore::Pokemon::POKEMON_NAME_WARTORTLE;
using PocketCore::Pokemon::PokemonGender;
using PocketCore::Pokemon::PokemonID;
using PocketCore::Pokemon::PokemonMeta;
using PocketCore::Pokemon::SQUIRTLE_BASE_STATS;
using PocketCore::Pokemon::toPokemonID;
using PocketCore::Pokemon::VENUSAUR_BASE_STATS;
using PocketCore::Pokemon::WARTORTLE_BASE_STATS;
using PocketCore::Registry::Pokemon::PokemonRegistry;
using PocketCore::Type::BuiltinTypeID;
using PocketCore::Type::toTypeID;
using PocketCore::Validation::Registry::RegistryError;

template <typename Registry>
concept PubliclyStructurallyMutable = requires(Registry &registry) { registry.setAmountRegistered(0); };

static_assert(!PubliclyStructurallyMutable<PokemonRegistry>);

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("PokemonRegistry")
{
	std::unique_ptr<PokemonRegistry> registryStorage{std::make_unique<PokemonRegistry>()};
	PokemonRegistry &registry{*registryStorage};
	ub finalPokemonUnderlyingValue{std::to_underlying(BuiltinPokemonID::FinalPokemon)};

	GIVEN("a default constructed pokemon registry")
	{
		THEN("Bulbasaur has the appropriate properties")
		{
			PokemonMeta expected{
				.mName = std::string(POKEMON_NAME_BULBASAUR),
				.mAbilityPool = {{{.mAbilityID = toAbilityID(BuiltinAbilityID::None)}}},
				.mBaseStats = BULBASAUR_BASE_STATS,
				.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Grass), toEggGroupID(BuiltinEggGroupID::Monster)},
				.mTypeIDs = {toTypeID(BuiltinTypeID::Grass), toTypeID(BuiltinTypeID::Poison)},
				.mGenderPool = {PokemonGender::Male, PokemonGender::Female},
				.mPokemonID = toPokemonID(BuiltinPokemonID::Bulbasaur),
				.mLearnsetID = toLearnsetID(BuiltinLearnsetID::BulbasaurLearnset),
			};

			const PokemonMeta *actual{registry.getPokemonMetadata(toPokemonID(BuiltinPokemonID::Bulbasaur))};

			CHECK((expected == *actual));
		}

		THEN("Ivysaur has the appropriate properties")
		{
			PokemonMeta expected{
				.mName = std::string(POKEMON_NAME_IVYSAUR),
				.mAbilityPool = {{{.mAbilityID = toAbilityID(BuiltinAbilityID::None)}}},
				.mBaseStats = IVYSAUR_BASE_STATS,
				.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Grass), toEggGroupID(BuiltinEggGroupID::Monster)},
				.mTypeIDs = {toTypeID(BuiltinTypeID::Grass), toTypeID(BuiltinTypeID::Poison)},
				.mGenderPool = {PokemonGender::Male, PokemonGender::Female},
				.mPokemonID = toPokemonID(BuiltinPokemonID::Ivysaur),
				.mLearnsetID = toLearnsetID(BuiltinLearnsetID::IvysaurLearnset),
			};

			const PokemonMeta *actual{registry.getPokemonMetadata(toPokemonID(BuiltinPokemonID::Ivysaur))};

			CHECK((expected == *actual));
		}

		THEN("Venusaur has the appropriate properties")
		{
			PokemonMeta expected{
				.mName = std::string(POKEMON_NAME_VENUSAUR),
				.mAbilityPool = {{{.mAbilityID = toAbilityID(BuiltinAbilityID::None)}}},
				.mBaseStats = VENUSAUR_BASE_STATS,
				.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Grass), toEggGroupID(BuiltinEggGroupID::Monster)},
				.mTypeIDs = {toTypeID(BuiltinTypeID::Grass), toTypeID(BuiltinTypeID::Poison)},
				.mGenderPool = {PokemonGender::Male, PokemonGender::Female},
				.mPokemonID = toPokemonID(BuiltinPokemonID::Venusaur),
				.mLearnsetID = toLearnsetID(BuiltinLearnsetID::VenusaurLearnset),
			};

			const PokemonMeta *actual{registry.getPokemonMetadata(toPokemonID(BuiltinPokemonID::Venusaur))};

			CHECK((expected == *actual));
		}

		THEN("Charmander has the appropriate properties")
		{
			PokemonMeta expected{
				.mName = std::string(POKEMON_NAME_CHARMANDER),
				.mAbilityPool = {{{.mAbilityID = toAbilityID(BuiltinAbilityID::None)}}},
				.mBaseStats = CHARMANDER_BASE_STATS,
				.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Dragon), toEggGroupID(BuiltinEggGroupID::Monster)},
				.mTypeIDs = {toTypeID(BuiltinTypeID::Fire)},
				.mGenderPool = {PokemonGender::Male, PokemonGender::Female},
				.mPokemonID = toPokemonID(BuiltinPokemonID::Charmander),
				.mLearnsetID = toLearnsetID(BuiltinLearnsetID::CharmanderLearnset),
			};

			const PokemonMeta *actual{registry.getPokemonMetadata(toPokemonID(BuiltinPokemonID::Charmander))};

			CHECK((expected == *actual));
		}

		THEN("Charmeleon has the appropriate properties")
		{
			PokemonMeta expected{
				.mName = std::string(POKEMON_NAME_CHARMELEON),
				.mAbilityPool = {{{.mAbilityID = toAbilityID(BuiltinAbilityID::None)}}},
				.mBaseStats = CHARMELEON_BASE_STATS,
				.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Dragon), toEggGroupID(BuiltinEggGroupID::Monster)},
				.mTypeIDs = {toTypeID(BuiltinTypeID::Fire)},
				.mGenderPool = {PokemonGender::Male, PokemonGender::Female},
				.mPokemonID = toPokemonID(BuiltinPokemonID::Charmeleon),
				.mLearnsetID = toLearnsetID(BuiltinLearnsetID::CharmeleonLearnset),
			};

			const PokemonMeta *actual{registry.getPokemonMetadata(toPokemonID(BuiltinPokemonID::Charmeleon))};

			CHECK((expected == *actual));
		}

		THEN("Charizard has the appropriate properties")
		{
			PokemonMeta expected{
				.mName = std::string(POKEMON_NAME_CHARIZARD),
				.mAbilityPool = {{{.mAbilityID = toAbilityID(BuiltinAbilityID::None)}}},
				.mBaseStats = CHARIZARD_BASE_STATS,
				.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Dragon), toEggGroupID(BuiltinEggGroupID::Monster)},
				.mTypeIDs = {toTypeID(BuiltinTypeID::Fire), toTypeID(BuiltinTypeID::Flying)},
				.mGenderPool = {PokemonGender::Male, PokemonGender::Female},
				.mPokemonID = toPokemonID(BuiltinPokemonID::Charizard),
				.mLearnsetID = toLearnsetID(BuiltinLearnsetID::CharizardLearnset),
			};

			const PokemonMeta *actual{registry.getPokemonMetadata(toPokemonID(BuiltinPokemonID::Charizard))};

			CHECK((expected == *actual));
		}

		THEN("Squirtle has the appropriate properties")
		{
			PokemonMeta expected{
				.mName = std::string(POKEMON_NAME_SQUIRTLE),
				.mAbilityPool = {{{.mAbilityID = toAbilityID(BuiltinAbilityID::None)}}},
				.mBaseStats = SQUIRTLE_BASE_STATS,
				.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Monster), toEggGroupID(BuiltinEggGroupID::Water1)},
				.mTypeIDs = {toTypeID(BuiltinTypeID::Water)},
				.mGenderPool = {PokemonGender::Male, PokemonGender::Female},
				.mPokemonID = toPokemonID(BuiltinPokemonID::Squirtle),
				.mLearnsetID = toLearnsetID(BuiltinLearnsetID::SquirtleLearnset),
			};

			const PokemonMeta *actual{registry.getPokemonMetadata(toPokemonID(BuiltinPokemonID::Squirtle))};

			CHECK((expected == *actual));
		}

		THEN("Wartortle has the appropriate properties")
		{
			PokemonMeta expected{
				.mName = std::string(POKEMON_NAME_WARTORTLE),
				.mAbilityPool = {{{.mAbilityID = toAbilityID(BuiltinAbilityID::None)}}},
				.mBaseStats = WARTORTLE_BASE_STATS,
				.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Monster), toEggGroupID(BuiltinEggGroupID::Water1)},
				.mTypeIDs = {toTypeID(BuiltinTypeID::Water)},
				.mGenderPool = {PokemonGender::Male, PokemonGender::Female},
				.mPokemonID = toPokemonID(BuiltinPokemonID::Wartortle),
				.mLearnsetID = toLearnsetID(BuiltinLearnsetID::WartortleLearnset),
			};

			const PokemonMeta *actual{registry.getPokemonMetadata(toPokemonID(BuiltinPokemonID::Wartortle))};

			CHECK((expected == *actual));
		}

		THEN("Blastoise has the appropriate properties")
		{
			PokemonMeta expected{
				.mName = std::string(POKEMON_NAME_BLASTOISE),
				.mAbilityPool = {{{.mAbilityID = toAbilityID(BuiltinAbilityID::None)}}},
				.mBaseStats = BLASTOISE_BASE_STATS,
				.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Monster), toEggGroupID(BuiltinEggGroupID::Water1)},
				.mTypeIDs = {toTypeID(BuiltinTypeID::Water)},
				.mGenderPool = {PokemonGender::Male, PokemonGender::Female},
				.mPokemonID = toPokemonID(BuiltinPokemonID::Blastoise),
				.mLearnsetID = toLearnsetID(BuiltinLearnsetID::BlastoiseLearnset),
			};

			const PokemonMeta *actual{registry.getPokemonMetadata(toPokemonID(BuiltinPokemonID::Blastoise))};

			CHECK((expected == *actual));
		}

		THEN("Ditto has the appropriate properties")
		{
			PokemonMeta expected{
				.mName = std::string(POKEMON_NAME_DITTO),
				.mAbilityPool = {{{.mAbilityID = toAbilityID(BuiltinAbilityID::None)}}},
				.mBaseStats = DITTO_BASE_STATS,
				.mEggGroupIDs = {toEggGroupID(BuiltinEggGroupID::Ditto)},
				.mTypeIDs = {toTypeID(BuiltinTypeID::Normal)},
				.mGenderPool = {PokemonGender::Genderless},
				.mPokemonID = toPokemonID(BuiltinPokemonID::Ditto),
				.mLearnsetID = toLearnsetID(BuiltinLearnsetID::DittoLearnset),
			};

			const PokemonMeta *actual{registry.getPokemonMetadata(toPokemonID(BuiltinPokemonID::Ditto))};

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
