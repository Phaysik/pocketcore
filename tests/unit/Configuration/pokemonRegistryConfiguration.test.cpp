/*! @file pokemonRegistryConfiguration.test.cpp
	@brief C++ file for running tests for the PokemonRegistryConfiguration.
	@date 09/10/2026
	@since 0.12.20
	@version 0.12.20
	@author Matthew Moore
*/

#include "Configuration/pokemonRegistryConfiguration.h"

#include <expected>
#include <format>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "Configuration/constants.h"
#include "Core/typedefs.h"
#include "Pokemon/builtInPokemonID.h"
#include "Pokemon/constants.h"
#include "Pokemon/pokemonID.h"
#include "Pokemon/pokemonMeta.h"
#include "Registry/pokemonRegistry.h"
#include "Registry/registryError.h"
#include "Utility/Debug/Logging/logging.testHelper.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Configuration::MAX_POKEMON;
using PocketCore::Configuration::PokemonRegistryConfiguration;
using PocketCore::Configuration::RegistryError;
using PocketCore::Core::ub;
using PocketCore::Core::us;
using PocketCore::Pokemon::BuiltinPokemonID;
using PocketCore::Pokemon::NO_POKEMON_ID;
using PocketCore::Pokemon::POKEMON_NAME_NONE;
using PocketCore::Pokemon::POKEMON_NAME_SQUIRTLE;
using PocketCore::Pokemon::PokemonID;
using PocketCore::Pokemon::PokemonMeta;
using PocketCore::Pokemon::toPokemonID;
using PocketCore::Registry::Pokemon::PokemonRegistry;
using PocketCore::Registry::RegistryErrorInfo;
using PocketCore::Testing::ensureLoggerInitialized;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("PokemonRegistryConfiguration")
{
	ensureLoggerInitialized("pokemon registry configuration test", "pokemonRegistryConfiguration_test.log");

	PokemonRegistryConfiguration config{};
	PokemonRegistry registry{};
	ub finalPokemonUnderlyingValue{std::to_underlying(BuiltinPokemonID::FinalPokemon)};

	GIVEN("getAmountRegistered")
	{
		THEN("the default registry returns the expected amount of built-in pokemons")
		{
			CHECK((config.getAmountRegistered() == finalPokemonUnderlyingValue));
		}
	}

	GIVEN("getRuntimeRegistry")
	{
		THEN("the runtime registry is the same as the default registry")
		{
			CHECK((config.getRuntimeRegistry() == registry));
		}
	}

	GIVEN("getPokemonMetadata")
	{
		THEN("unknown IDs are absent")
		{
			CHECK((config.getPokemonMetadata(PokemonID{200}) == nullptr));
		}

		THEN("the metadata is retrieved when accessed by a valid Pokemon ID")
		{
			PokemonMeta expected{
				.mName = std::string(POKEMON_NAME_NONE),
				.mPokemonID = toPokemonID(BuiltinPokemonID::None),
			};

			CHECK((expected == *config.getPokemonMetadata(NO_POKEMON_ID)));
		}
	}

	GIVEN("getPokemonID")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(config.getPokemonID("Unknown").has_value());
		}

		THEN("the Pokemon ID is retrieved by valid Pokemon name")
		{
			std::optional<PokemonID> pokemonID{config.getPokemonID(POKEMON_NAME_NONE)};

			REQUIRE(pokemonID.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((pokemonID.value() == toPokemonID(BuiltinPokemonID::None)));
		}
	}

	GIVEN("getPokemonName")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(config.getPokemonName(PokemonID{200}).has_value());
		}

		THEN("a registered pokemon name is returned by stable ID")
		{
			std::optional<std::string_view> pokemonName{config.getPokemonName(toPokemonID(BuiltinPokemonID::None))};

			REQUIRE(pokemonName.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((pokemonName.value() == POKEMON_NAME_NONE));
		}
	}

	GIVEN("getRegisteredPokemons")
	{
		THEN("the amount of pokemons returned matches the amount that are built-in")
		{
			CHECK((config.getRegisteredPokemons().size() == finalPokemonUnderlyingValue));
		}
	}

	GIVEN("hasPokemon")
	{
		WHEN("calling the string_view overload")
		{
			THEN("an unknown pokemon name has no entry")
			{
				CHECK_FALSE(config.hasPokemon("Unknown"));
			}

			THEN("a known pokemon name has an entry")
			{
				CHECK(config.hasPokemon(POKEMON_NAME_NONE));
			}
		}

		WHEN("calling the PokemonID overload")
		{
			THEN("an unknown pokemon ID has no entry")
			{
				CHECK_FALSE(config.hasPokemon(PokemonID{200}));
			}

			THEN("a known pokemon ID has an entry")
			{
				CHECK(config.hasPokemon(NO_POKEMON_ID));
			}
		}
	}

	GIVEN("addPokemon")
	{
		WHEN("trying to add an pokemon past the capacity")
		{
			us newPokemonCount{finalPokemonUnderlyingValue};

			for (us i{0}; i < MAX_POKEMON - finalPokemonUnderlyingValue; ++i)
			{
				std::string name{std::format("String_{:04}", i)};
				PokemonMeta definition{.mName = name};
				std::expected<PokemonID, RegistryErrorInfo> result{config.addPokemon(definition)};

				REQUIRE(result.has_value());

				PokemonID assignedID{result.value()};
				CHECK((assignedID.getValue() == newPokemonCount++));
			}

			std::string name{std::format("String_{:04}", MAX_POKEMON + 1)};

			PokemonMeta definition{.mName = name};
			std::expected<PokemonID, RegistryErrorInfo> result{config.addPokemon(definition)};

			THEN("registration reports a max capacity and nothing is added")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::MaxCapacity));
				CHECK((config.getAmountRegistered() == MAX_POKEMON));
			}
		}

		WHEN("an pokemon whose name is already in use is added")
		{
			PokemonMeta definition{.mName = std::string(POKEMON_NAME_SQUIRTLE)};
			std::expected<PokemonID, RegistryErrorInfo> result{config.addPokemon(definition)};

			THEN("registration reports a duplicate pokemon and nothing is added")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::DuplicatePokemon));
				CHECK((config.getAmountRegistered() == finalPokemonUnderlyingValue));
			}
		}

		WHEN("a unique pokemon definition is added")
		{
			PokemonMeta definition{.mName = "TestPokemonName"};

			std::expected<PokemonID, RegistryErrorInfo> result{config.addPokemon(definition)};

			THEN("it receives the first custom stable ID and owns its trigger data")
			{
				REQUIRE(result.has_value());
				PokemonID assignedID{result.value()};
				CHECK((assignedID.getValue() == finalPokemonUnderlyingValue));

				const PokemonMeta *metadata{config.getPokemonMetadata(assignedID)};

				REQUIRE((metadata != nullptr));
				CHECK((metadata->mName == "TestPokemonName"));
			}
		}
	}

	GIVEN("addPokemons")
	{
		WHEN("trying to add an pokemon past the capacity")
		{
			std::vector<PokemonMeta> pokemonMetas;

			for (us i{0}; i < MAX_POKEMON - finalPokemonUnderlyingValue; ++i)
			{
				std::string name{std::format("String_{:04}", i)};
				pokemonMetas.push_back({.mName = name});
			}

			std::string name{std::format("String_{:04}", MAX_POKEMON + 1)};

			pokemonMetas.push_back({.mName = name});
			std::expected<void, RegistryErrorInfo> result{config.addPokemons(pokemonMetas)};

			THEN("registration reports a max capacity and nothing is added")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::MaxCapacity));
				CHECK((config.getAmountRegistered() == finalPokemonUnderlyingValue));
			}
		}

		WHEN("an pokemon whose name is already in use is added")
		{
			std::vector<PokemonMeta> pokemonMetas{};

			for (us i{0}; i < 20; ++i)
			{
				std::string name{std::format("String_{:04}", i)};
				pokemonMetas.push_back({.mName = name});
			}

			pokemonMetas.push_back({.mName = std::string(POKEMON_NAME_SQUIRTLE)});
			std::expected<void, RegistryErrorInfo> result{config.addPokemons(pokemonMetas)};

			THEN("registration reports a duplicate pokemon and the registry is rollback to the checkpoint before the erroneous addition")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::DuplicatePokemon));
				CHECK((config.getAmountRegistered() == finalPokemonUnderlyingValue));
			}
		}

		WHEN("a unique pokemon definition is added")
		{
			std::vector<PokemonMeta> pokemonMetas{};

			pokemonMetas.push_back({.mName = "TestPokemonName"});

			std::expected<void, RegistryErrorInfo> result{config.addPokemons(pokemonMetas)};

			THEN("the registry reports no error and all the pokemon definitions are added")
			{
				REQUIRE(result.has_value());
				CHECK((config.getAmountRegistered() == finalPokemonUnderlyingValue + 1));
			}
		}
	}

	GIVEN("renamePokemon")
	{
		WHEN("calling with an invalid pokemon name")
		{
			std::expected<void, RegistryErrorInfo> result{config.renamePokemon("ThisIsInvalid", "NewName")};

			THEN("the registry reports an error and there is no update to the registry")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::PokemonNotFound));
				CHECK((config.getAmountRegistered() == finalPokemonUnderlyingValue));
			}
		}

		WHEN("trying to rename to a name already in the registry")
		{
			std::expected<void, RegistryErrorInfo> result{
				config.renamePokemon(POKEMON_NAME_NONE, POKEMON_NAME_SQUIRTLE),
			};

			THEN("the registry reports an error and there is no update to the registry")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::DuplicatePokemon));
				CHECK((config.getAmountRegistered() == finalPokemonUnderlyingValue));
			}
		}

		WHEN("updating an existing pokemon definition")
		{
			std::expected<void, RegistryErrorInfo> result{config.renamePokemon(POKEMON_NAME_NONE, "NewName")};

			THEN("the registry reports no error and the name is appropriately updated")
			{
				REQUIRE(result.has_value());
				CHECK((config.getAmountRegistered() == finalPokemonUnderlyingValue));
				CHECK((config.getPokemonMetadata(toPokemonID(BuiltinPokemonID::None))->mName == "NewName"));
			}
		}
	}

	GIVEN("updatePokemon")
	{
		PokemonMeta definition{.mName = "TestPokemonName"};

		WHEN("calling the string_view overload")
		{
			WHEN("calling with an invalid pokemon name")
			{
				std::expected<void, RegistryErrorInfo> result{config.updatePokemon("ThisIsInvalid", definition)};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::PokemonNotFound));
					CHECK((config.getAmountRegistered() == finalPokemonUnderlyingValue));
				}
			}

			WHEN("trying to rename to a name already in the registry")
			{
				std::expected<void, RegistryErrorInfo> result{
					config.updatePokemon(POKEMON_NAME_NONE, {.mName = std::string(POKEMON_NAME_SQUIRTLE)}),
				};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::DuplicatePokemon));
					CHECK((config.getAmountRegistered() == finalPokemonUnderlyingValue));
				}
			}

			WHEN("updating an existing pokemon definition")
			{
				std::expected<void, RegistryErrorInfo> result{config.updatePokemon(POKEMON_NAME_NONE, definition)};

				THEN("the registry reports no error and the target is appropriately updated")
				{
					REQUIRE(result.has_value());
					CHECK((config.getAmountRegistered() == finalPokemonUnderlyingValue));
					CHECK((config.getPokemonMetadata(toPokemonID(BuiltinPokemonID::None))->mName == "TestPokemonName"));
				}
			}
		}

		WHEN("calling the PokemonID overload")
		{
			WHEN("calling with an invalid pokemon name")
			{
				std::expected<void, RegistryErrorInfo> result{config.updatePokemon(PokemonID{200}, definition)};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::PokemonNotFound));
					CHECK((config.getAmountRegistered() == finalPokemonUnderlyingValue));
				}
			}

			WHEN("trying to rename to a name already in the registry")
			{
				std::expected<void, RegistryErrorInfo> result{
					config.updatePokemon(toPokemonID(BuiltinPokemonID::None), {.mName = std::string(POKEMON_NAME_SQUIRTLE)}),
				};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::DuplicatePokemon));
					CHECK((config.getAmountRegistered() == finalPokemonUnderlyingValue));
				}
			}

			WHEN("updating an existing pokemon definition")
			{
				std::expected<void, RegistryErrorInfo> result{config.updatePokemon(toPokemonID(BuiltinPokemonID::None), definition)};

				THEN("the registry reports no error and the target is appropriately updated")
				{
					REQUIRE(result.has_value());
					CHECK((config.getAmountRegistered() == finalPokemonUnderlyingValue));
					CHECK((config.getPokemonMetadata(toPokemonID(BuiltinPokemonID::None))->mName == "TestPokemonName"));
				}
			}
		}
	}

	GIVEN("removePokemon")
	{
		WHEN("calling the string_view overload")
		{
			WHEN("calling with an unknown pokemon name")
			{
				std::expected<PokemonID, RegistryErrorInfo> result{config.removePokemon("Unknown")};

				THEN("the result is an error and the registry is not updated")
				{
					CHECK_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::PokemonNotFound));
					CHECK((config.getAmountRegistered() == finalPokemonUnderlyingValue));
				}
			}

			WHEN("calling with a known pokemon name")
			{
				std::expected<PokemonID, RegistryErrorInfo> result{config.removePokemon(POKEMON_NAME_NONE)};

				THEN("the result is a success and the registry is updated")
				{
					CHECK(result.has_value());
					CHECK((result.value() == toPokemonID(BuiltinPokemonID::None)));
					CHECK((config.getAmountRegistered() == finalPokemonUnderlyingValue - 1));
				}
			}
		}

		WHEN("calling the PokemonID overload")
		{
			WHEN("calling with an unknown pokemon ID")
			{
				std::expected<PokemonID, RegistryErrorInfo> result{config.removePokemon(PokemonID{200})};

				THEN("the result is an error and the registry is not updated")
				{
					CHECK_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::PokemonNotFound));
					CHECK((config.getAmountRegistered() == finalPokemonUnderlyingValue));
				}
			}

			WHEN("calling with a known pokemon ID")
			{
				std::expected<PokemonID, RegistryErrorInfo> result{config.removePokemon(toPokemonID(BuiltinPokemonID::None))};

				THEN("the result is a success and the registry is updated")
				{
					CHECK(result.has_value());
					CHECK((result.value() == toPokemonID(BuiltinPokemonID::None)));
					CHECK((config.getAmountRegistered() == finalPokemonUnderlyingValue - 1));
				}
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
