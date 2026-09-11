/*! @file pokemonRegistry.test.cpp
	@brief C++ file for running tests for the PokemonRegistry.
	@date 09/11/2026
	@since 0.12.20
	@version 0.12.22
	@author Matthew Moore
*/

#include "Registry/pokemonRegistry.h"

#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <utility>

#include "Ability/builtInAbilityID.h"
#include "Core/typedefs.h"
#include "Pokemon/builtInPokemonID.h"
#include "Pokemon/constants.h"
#include "Pokemon/pokemonID.h"
#include "Pokemon/pokemonMeta.h"
#include "Types/builtInTypeID.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Ability::BuiltinAbilityID;
using PocketCore::Ability::toAbilityID;
using PocketCore::Core::ub;
using PocketCore::Pokemon::BLASTOISE_BASE_STATS;
using PocketCore::Pokemon::BuiltinPokemonID;
using PocketCore::Pokemon::BULBASAUR_BASE_STATS;
using PocketCore::Pokemon::CHARIZARD_BASE_STATS;
using PocketCore::Pokemon::CHARMANDER_BASE_STATS;
using PocketCore::Pokemon::CHARMELEON_BASE_STATS;
using PocketCore::Pokemon::IVYSAUR_BASE_STATS;
using PocketCore::Pokemon::NO_POKEMON_ID;
using PocketCore::Pokemon::POKEMON_NAME_BLASTOISE;
using PocketCore::Pokemon::POKEMON_NAME_BULBASAUR;
using PocketCore::Pokemon::POKEMON_NAME_CHARIZARD;
using PocketCore::Pokemon::POKEMON_NAME_CHARMANDER;
using PocketCore::Pokemon::POKEMON_NAME_CHARMELEON;
using PocketCore::Pokemon::POKEMON_NAME_IVYSAUR;
using PocketCore::Pokemon::POKEMON_NAME_NONE;
using PocketCore::Pokemon::POKEMON_NAME_SQUIRTLE;
using PocketCore::Pokemon::POKEMON_NAME_VENUSAUR;
using PocketCore::Pokemon::POKEMON_NAME_WARTORTLE;
using PocketCore::Pokemon::PokemonID;
using PocketCore::Pokemon::PokemonMeta;
using PocketCore::Pokemon::SQUIRTLE_BASE_STATS;
using PocketCore::Pokemon::toPokemonID;
using PocketCore::Pokemon::VENUSAUR_BASE_STATS;
using PocketCore::Pokemon::WARTORTLE_BASE_STATS;
using PocketCore::Registry::Pokemon::PokemonRegistry;
using PocketCore::Type::BuiltinTypeID;
using PocketCore::Type::toTypeID;

template <typename Registry>
concept PubliclyStructurallyMutable = requires(Registry &registry) { registry.setAmountRegistered(0); };

static_assert(!PubliclyStructurallyMutable<PokemonRegistry>);

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("PokemonRegistry")
{
	PokemonRegistry registry{};
	ub finalPokemonUnderlyingValue{std::to_underlying(BuiltinPokemonID::FinalPokemon)};

	GIVEN("a default constructed pokemon registry")
	{
		THEN("Bulbasaur has the appropriate properties")
		{
			PokemonMeta expected{
				.mName = std::string(POKEMON_NAME_BULBASAUR),
				.mAbilityPool = {toAbilityID(BuiltinAbilityID::None)},
				.mBaseStats = BULBASAUR_BASE_STATS,
				.mTypeIDs = {toTypeID(BuiltinTypeID::Grass), toTypeID(BuiltinTypeID::Poison)},
				.mPokemonID = toPokemonID(BuiltinPokemonID::Bulbasaur),
				.mAbilityPoolCount = 0,
			};

			const PokemonMeta *actual{registry.getPokemonMetadata(toPokemonID(BuiltinPokemonID::Bulbasaur))};

			CHECK((expected == *actual));
		}

		THEN("Ivysaur has the appropriate properties")
		{
			PokemonMeta expected{
				.mName = std::string(POKEMON_NAME_IVYSAUR),
				.mAbilityPool = {toAbilityID(BuiltinAbilityID::None)},
				.mBaseStats = IVYSAUR_BASE_STATS,
				.mTypeIDs = {toTypeID(BuiltinTypeID::Grass), toTypeID(BuiltinTypeID::Poison)},
				.mPokemonID = toPokemonID(BuiltinPokemonID::Ivysaur),
				.mAbilityPoolCount = 0,
			};

			const PokemonMeta *actual{registry.getPokemonMetadata(toPokemonID(BuiltinPokemonID::Ivysaur))};

			CHECK((expected == *actual));
		}

		THEN("Venusaur has the appropriate properties")
		{
			PokemonMeta expected{
				.mName = std::string(POKEMON_NAME_VENUSAUR),
				.mAbilityPool = {toAbilityID(BuiltinAbilityID::None)},
				.mBaseStats = VENUSAUR_BASE_STATS,
				.mTypeIDs = {toTypeID(BuiltinTypeID::Grass), toTypeID(BuiltinTypeID::Poison)},
				.mPokemonID = toPokemonID(BuiltinPokemonID::Venusaur),
				.mAbilityPoolCount = 0,
			};

			const PokemonMeta *actual{registry.getPokemonMetadata(toPokemonID(BuiltinPokemonID::Venusaur))};

			CHECK((expected == *actual));
		}

		THEN("Charmander has the appropriate properties")
		{
			PokemonMeta expected{
				.mName = std::string(POKEMON_NAME_CHARMANDER),
				.mAbilityPool = {toAbilityID(BuiltinAbilityID::None)},
				.mBaseStats = CHARMANDER_BASE_STATS,
				.mTypeIDs = {toTypeID(BuiltinTypeID::Fire)},
				.mPokemonID = toPokemonID(BuiltinPokemonID::Charmander),
				.mAbilityPoolCount = 0,
			};

			const PokemonMeta *actual{registry.getPokemonMetadata(toPokemonID(BuiltinPokemonID::Charmander))};

			CHECK((expected == *actual));
		}

		THEN("Charmeleon has the appropriate properties")
		{
			PokemonMeta expected{
				.mName = std::string(POKEMON_NAME_CHARMELEON),
				.mAbilityPool = {toAbilityID(BuiltinAbilityID::None)},
				.mBaseStats = CHARMELEON_BASE_STATS,
				.mTypeIDs = {toTypeID(BuiltinTypeID::Fire)},
				.mPokemonID = toPokemonID(BuiltinPokemonID::Charmeleon),
				.mAbilityPoolCount = 0,
			};

			const PokemonMeta *actual{registry.getPokemonMetadata(toPokemonID(BuiltinPokemonID::Charmeleon))};

			CHECK((expected == *actual));
		}

		THEN("Charizard has the appropriate properties")
		{
			PokemonMeta expected{
				.mName = std::string(POKEMON_NAME_CHARIZARD),
				.mAbilityPool = {toAbilityID(BuiltinAbilityID::None)},
				.mBaseStats = CHARIZARD_BASE_STATS,
				.mTypeIDs = {toTypeID(BuiltinTypeID::Fire), toTypeID(BuiltinTypeID::Flying)},
				.mPokemonID = toPokemonID(BuiltinPokemonID::Charizard),
				.mAbilityPoolCount = 0,
			};

			const PokemonMeta *actual{registry.getPokemonMetadata(toPokemonID(BuiltinPokemonID::Charizard))};

			CHECK((expected == *actual));
		}

		THEN("Squirtle has the appropriate properties")
		{
			PokemonMeta expected{
				.mName = std::string(POKEMON_NAME_SQUIRTLE),
				.mAbilityPool = {toAbilityID(BuiltinAbilityID::None)},
				.mBaseStats = SQUIRTLE_BASE_STATS,
				.mTypeIDs = {toTypeID(BuiltinTypeID::Water)},
				.mPokemonID = toPokemonID(BuiltinPokemonID::Squirtle),
				.mAbilityPoolCount = 0,
			};

			const PokemonMeta *actual{registry.getPokemonMetadata(toPokemonID(BuiltinPokemonID::Squirtle))};

			CHECK((expected == *actual));
		}

		THEN("Wartortle has the appropriate properties")
		{
			PokemonMeta expected{
				.mName = std::string(POKEMON_NAME_WARTORTLE),
				.mAbilityPool = {toAbilityID(BuiltinAbilityID::None)},
				.mBaseStats = WARTORTLE_BASE_STATS,
				.mTypeIDs = {toTypeID(BuiltinTypeID::Water)},
				.mPokemonID = toPokemonID(BuiltinPokemonID::Wartortle),
				.mAbilityPoolCount = 0,
			};

			const PokemonMeta *actual{registry.getPokemonMetadata(toPokemonID(BuiltinPokemonID::Wartortle))};

			CHECK((expected == *actual));
		}

		THEN("Blastoise has the appropriate properties")
		{
			PokemonMeta expected{
				.mName = std::string(POKEMON_NAME_BLASTOISE),
				.mAbilityPool = {toAbilityID(BuiltinAbilityID::None)},
				.mBaseStats = BLASTOISE_BASE_STATS,
				.mTypeIDs = {toTypeID(BuiltinTypeID::Water)},
				.mPokemonID = toPokemonID(BuiltinPokemonID::Blastoise),
				.mAbilityPoolCount = 0,
			};

			const PokemonMeta *actual{registry.getPokemonMetadata(toPokemonID(BuiltinPokemonID::Blastoise))};

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
			CHECK((pokemonID.value() == toPokemonID(BuiltinPokemonID::None)));
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
			CHECK((pokemonName.value() == POKEMON_NAME_NONE));
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
			CHECK((pokemonIndex.value() == 0));
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
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
