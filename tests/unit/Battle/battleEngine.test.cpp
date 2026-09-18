/*! @file battleEngine.test.cpp
	@brief C++ file for running tests for the BattleEngine.
	@date 09/17/2026
	@since 0.12.37
	@version 0.12.37
	@author Matthew Moore
*/

#include "Battle/battleEngine.h"

#include <array>

#include "Battle/battleValidation.h"
#include "Pokemon/pokemon.h"
#include "Pokemon/pokemon.testHelper.h"
#include "Registry/effectRegistry.h"
#include "Registry/registryProvider.h"
#include "Registry/registryProvider.testHelper.h"
#include "Ruleset/rulesetPolicy.h"
#include "Ruleset/rulesetPresets.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Battle::BattleEngine;
using PocketCore::Battle::BattleEngineError;
using PocketCore::Battle::BattlePhase;
using PocketCore::Pokemon::Pokemon;
using PocketCore::Registry::Effect::EffectRegistry;
using PocketCore::Registry::RegistryProvider;
using PocketCore::Ruleset::GEN9_BASE_GAME;
using PocketCore::Ruleset::RulesetPolicy;
using PocketCore::Testing::getDefaultInitializedRegistryProvider;
using PocketCore::Testing::getNullAbilityRegistryProvider;
using PocketCore::Testing::getNullItemRegistryProvider;
using PocketCore::Testing::getNullMoveRegistryProvider;
using PocketCore::Testing::getNullMultiplierRegistryProvider;
using PocketCore::Testing::getNullNatureRegistryProvider;
using PocketCore::Testing::getNullPokemonRegistryProvider;
using PocketCore::Testing::getNullStatusRegistryProvider;
using PocketCore::Testing::getNullTerrainRegistryProvider;
using PocketCore::Testing::getNullTypeRegistryProvider;
using PocketCore::Testing::getNullWeatherRegistryProvider;
using PocketCore::Testing::makePokemon;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("BattleEngine")
{
	RegistryProvider provider{getDefaultInitializedRegistryProvider()};
	EffectRegistry effectRegistry{};
	BattleEngine engine{provider, effectRegistry};
	RulesetPolicy ruleset{GEN9_BASE_GAME};
	Pokemon partyAOne{makePokemon({.mHealth = 100})};
	Pokemon partyBOne{makePokemon({.mHealth = 100})};
	Pokemon partyATwo{makePokemon({.mHealth = 100})};
	Pokemon partyBTwo{makePokemon({.mHealth = 100})};

	const std::array<Pokemon *const, 2> pokemonA{{&partyAOne, &partyATwo}};
	const std::array<Pokemon *const, 2> pokemonB{{&partyBOne, &partyBTwo}};

	GIVEN("startBattle")
	{
		WHEN("trying to start a battle that has already started")
		{
			std::expected<void, BattleEngineError> result{engine.startBattle(pokemonA, pokemonB, ruleset)};

			REQUIRE(result.has_value());

			THEN("it will return an error")
			{
				result = engine.startBattle(pokemonA, pokemonB, ruleset);

				REQUIRE_FALSE(result.has_value());
				CHECK((result.error() == BattleEngineError::BattleAlreadyStarted));
				CHECK((engine.getPhase() == BattlePhase::AwaitingActions));
			}
		}

		WHEN("trying to start a battle with invalid metadata registries")
		{
			GIVEN("a null registry provider")
			{
				RegistryProvider *nullProvider{nullptr};
				BattleEngine nullEngine{*nullProvider, effectRegistry};
				std::expected<void, BattleEngineError> result{nullEngine.startBattle(pokemonA, pokemonB, ruleset)};

				THEN("it will return an error")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error() == BattleEngineError::MissingRegistry));
					CHECK((engine.getPhase() == BattlePhase::NotStarted));
				}
			}

			GIVEN("a null effect registry")
			{
				EffectRegistry *nullEffect{nullptr};
				BattleEngine nullEngine{{}, *nullEffect};
				std::expected<void, BattleEngineError> result{nullEngine.startBattle(pokemonA, pokemonB, ruleset)};

				THEN("it will return an error")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error() == BattleEngineError::MissingRegistry));
					CHECK((engine.getPhase() == BattlePhase::NotStarted));
				}
			}

			GIVEN("a null ability registry")
			{
				RegistryProvider nullAbilityProvider{getNullAbilityRegistryProvider()};
				BattleEngine nullEngine{nullAbilityProvider, effectRegistry};
				std::expected<void, BattleEngineError> result{nullEngine.startBattle(pokemonA, pokemonB, ruleset)};

				THEN("it will return an error")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error() == BattleEngineError::MissingRegistry));
					CHECK((engine.getPhase() == BattlePhase::NotStarted));
				}
			}

			GIVEN("a null move registry")
			{
				RegistryProvider nullMoveProvider{getNullMoveRegistryProvider()};
				BattleEngine nullEngine{nullMoveProvider, effectRegistry};
				std::expected<void, BattleEngineError> result{nullEngine.startBattle(pokemonA, pokemonB, ruleset)};

				THEN("it will return an error")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error() == BattleEngineError::MissingRegistry));
					CHECK((engine.getPhase() == BattlePhase::NotStarted));
				}
			}

			GIVEN("a null item registry")
			{
				RegistryProvider nullItemProvider{getNullItemRegistryProvider()};
				BattleEngine nullEngine{nullItemProvider, effectRegistry};
				std::expected<void, BattleEngineError> result{nullEngine.startBattle(pokemonA, pokemonB, ruleset)};

				THEN("it will return an error")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error() == BattleEngineError::MissingRegistry));
					CHECK((engine.getPhase() == BattlePhase::NotStarted));
				}
			}

			GIVEN("a null type registry")
			{
				RegistryProvider nullTypeProvider{getNullTypeRegistryProvider()};
				BattleEngine nullEngine{nullTypeProvider, effectRegistry};
				std::expected<void, BattleEngineError> result{nullEngine.startBattle(pokemonA, pokemonB, ruleset)};

				THEN("it will return an error")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error() == BattleEngineError::MissingRegistry));
					CHECK((engine.getPhase() == BattlePhase::NotStarted));
				}
			}

			GIVEN("a null status registry")
			{
				RegistryProvider nullStatusProvider{getNullStatusRegistryProvider()};
				BattleEngine nullEngine{nullStatusProvider, effectRegistry};
				std::expected<void, BattleEngineError> result{nullEngine.startBattle(pokemonA, pokemonB, ruleset)};

				THEN("it will return an error")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error() == BattleEngineError::MissingRegistry));
					CHECK((engine.getPhase() == BattlePhase::NotStarted));
				}
			}

			GIVEN("a null weather registry")
			{
				RegistryProvider nullWeatherProvider{getNullWeatherRegistryProvider()};
				BattleEngine nullEngine{nullWeatherProvider, effectRegistry};
				std::expected<void, BattleEngineError> result{nullEngine.startBattle(pokemonA, pokemonB, ruleset)};

				THEN("it will return an error")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error() == BattleEngineError::MissingRegistry));
					CHECK((engine.getPhase() == BattlePhase::NotStarted));
				}
			}

			GIVEN("a null terrain registry")
			{
				RegistryProvider nullTerrainProvider{getNullTerrainRegistryProvider()};
				BattleEngine nullEngine{nullTerrainProvider, effectRegistry};
				std::expected<void, BattleEngineError> result{nullEngine.startBattle(pokemonA, pokemonB, ruleset)};

				THEN("it will return an error")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error() == BattleEngineError::MissingRegistry));
					CHECK((engine.getPhase() == BattlePhase::NotStarted));
				}
			}

			GIVEN("a null multiplier registry")
			{
				RegistryProvider nullMultiplierProvider{getNullMultiplierRegistryProvider()};
				BattleEngine nullEngine{nullMultiplierProvider, effectRegistry};
				std::expected<void, BattleEngineError> result{nullEngine.startBattle(pokemonA, pokemonB, ruleset)};

				THEN("it will return an error")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error() == BattleEngineError::MissingRegistry));
					CHECK((engine.getPhase() == BattlePhase::NotStarted));
				}
			}

			GIVEN("a null nature registry")
			{
				RegistryProvider nullNatureProvider{getNullNatureRegistryProvider()};
				BattleEngine nullEngine{nullNatureProvider, effectRegistry};
				std::expected<void, BattleEngineError> result{nullEngine.startBattle(pokemonA, pokemonB, ruleset)};

				THEN("it will return an error")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error() == BattleEngineError::MissingRegistry));
					CHECK((engine.getPhase() == BattlePhase::NotStarted));
				}
			}

			GIVEN("a null pokemon registry")
			{
				RegistryProvider nullPokemonProvider{getNullPokemonRegistryProvider()};
				BattleEngine nullEngine{nullPokemonProvider, effectRegistry};
				std::expected<void, BattleEngineError> result{nullEngine.startBattle(pokemonA, pokemonB, ruleset)};

				THEN("it will return an error")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error() == BattleEngineError::MissingRegistry));
					CHECK((engine.getPhase() == BattlePhase::NotStarted));
				}
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
