/*! @file battleEngine.test.cpp
	@brief C++ file for running tests for the BattleEngine.
	@date 10/26/2026
	@since 0.12.37
	@version 0.12.53
	@author Matthew Moore
*/

#include "Battle/battleEngine.h"

#include <array>
#include <cstddef>
#include <expected>

#include "Battle/battleAction.h"
#include "Battle/battleRandomSources.h"
#include "Battle/battleValidation.h"
#include "Configuration/moveRegistryConfiguration.h"
#include "Core/typedefs.h"
#include "Effect/effectContext.h"
#include "Move/builtInMoveID.h"
#include "Move/moveID.h"
#include "Pokemon/pokemon.h"
#include "Pokemon/pokemon.testHelper.h"
#include "Registry/effectRegistry.h"
#include "Registry/registryProvider.h"
#include "Registry/registryProvider.testHelper.h"
#include "Ruleset/rulesetPolicy.h"
#include "Ruleset/rulesetPresets.h"
#include "Utility/Random/callableRandomSource.h"
#include "Utility/Random/defaultRandomSource.h"
#include "Validation/Battle/battleError.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Battle::BattleAction;
using PocketCore::Battle::BattleEngine;
using PocketCore::Battle::BattlePhase;
using PocketCore::Battle::BattleRandomSources;
using PocketCore::Battle::BattleTarget;
using PocketCore::Battle::MoveAction;
using PocketCore::Configuration::MoveRegistryConfiguration;
using PocketCore::Core::sb;
using PocketCore::Core::ub;
using PocketCore::Effect::Side;
using PocketCore::Move::BuiltinMoveID;
using PocketCore::Move::MoveID;
using PocketCore::Move::toMoveID;
using PocketCore::Pokemon::Pokemon;
using PocketCore::Registry::Effect::EffectRegistry;
using PocketCore::Registry::RegistryProvider;
using PocketCore::Ruleset::GEN9_BASE_GAME;
using PocketCore::Ruleset::RulesetPolicy;
using PocketCore::Testing::getDefaultInitializedRegistryProvider;
using PocketCore::Testing::getNullAbilityRegistryProvider;
using PocketCore::Testing::getNullEggGroupRegistryProvider;
using PocketCore::Testing::getNullItemRegistryProvider;
using PocketCore::Testing::getNullLearnsetRegistryProvider;
using PocketCore::Testing::getNullLocationRegistryProvider;
using PocketCore::Testing::getNullMoveRegistryProvider;
using PocketCore::Testing::getNullMultiplierRegistryProvider;
using PocketCore::Testing::getNullNatureRegistryProvider;
using PocketCore::Testing::getNullPokemonRegistryProvider;
using PocketCore::Testing::getNullStatusRegistryProvider;
using PocketCore::Testing::getNullTerrainRegistryProvider;
using PocketCore::Testing::getNullTypeRegistryProvider;
using PocketCore::Testing::getNullWeatherRegistryProvider;
using PocketCore::Testing::makePokemon;
using PocketCore::Utility::Random::CallableRandomSource;
using PocketCore::Utility::Random::getDefaultRandomSource;
using PocketCore::Validation::Battle::BattleEngineError;

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
					CHECK((nullEngine.getPhase() == BattlePhase::NotStarted));
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
					CHECK((nullEngine.getPhase() == BattlePhase::NotStarted));
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
					CHECK((nullEngine.getPhase() == BattlePhase::NotStarted));
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
					CHECK((nullEngine.getPhase() == BattlePhase::NotStarted));
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
					CHECK((nullEngine.getPhase() == BattlePhase::NotStarted));
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
					CHECK((nullEngine.getPhase() == BattlePhase::NotStarted));
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
					CHECK((nullEngine.getPhase() == BattlePhase::NotStarted));
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
					CHECK((nullEngine.getPhase() == BattlePhase::NotStarted));
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
					CHECK((nullEngine.getPhase() == BattlePhase::NotStarted));
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
					CHECK((nullEngine.getPhase() == BattlePhase::NotStarted));
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
					CHECK((nullEngine.getPhase() == BattlePhase::NotStarted));
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
					CHECK((nullEngine.getPhase() == BattlePhase::NotStarted));
				}
			}

			GIVEN("a null learnset registry")
			{
				RegistryProvider nullLearnsetProvider{getNullLearnsetRegistryProvider()};
				BattleEngine nullEngine{nullLearnsetProvider, effectRegistry};
				std::expected<void, BattleEngineError> result{nullEngine.startBattle(pokemonA, pokemonB, ruleset)};

				THEN("it will return an error")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error() == BattleEngineError::MissingRegistry));
					CHECK((nullEngine.getPhase() == BattlePhase::NotStarted));
				}
			}

			GIVEN("a null location registry")
			{
				RegistryProvider nullLocationProvider{getNullLocationRegistryProvider()};
				BattleEngine nullEngine{nullLocationProvider, effectRegistry};
				std::expected<void, BattleEngineError> result{nullEngine.startBattle(pokemonA, pokemonB, ruleset)};

				THEN("it will return an error")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error() == BattleEngineError::MissingRegistry));
					CHECK((nullEngine.getPhase() == BattlePhase::NotStarted));
				}
			}

			GIVEN("a null egg group registry")
			{
				RegistryProvider nullEggGroupProvider{getNullEggGroupRegistryProvider()};
				BattleEngine nullEngine{nullEggGroupProvider, effectRegistry};
				std::expected<void, BattleEngineError> result{nullEngine.startBattle(pokemonA, pokemonB, ruleset)};

				THEN("it will return an error")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error() == BattleEngineError::MissingRegistry));
					CHECK((nullEngine.getPhase() == BattlePhase::NotStarted));
				}
			}
		}

		WHEN("calling with invalid party sizes")
		{
			GIVEN("no active pokemon allowed per side")
			{
				std::expected<void, BattleEngineError> result{engine.startBattle(pokemonA, pokemonB, ruleset, 0)};

				THEN("an error is returned")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error() == BattleEngineError::InvalidParty));
					CHECK((engine.getPhase() == BattlePhase::NotStarted));
				}
			}

			GIVEN("party A's size is less than the required amount per side")
			{
				std::array<Pokemon *const, 1> newPartyA{&partyAOne};
				std::expected<void, BattleEngineError> result{engine.startBattle(newPartyA, pokemonB, ruleset, 2)};

				THEN("an error is returned")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error() == BattleEngineError::InvalidParty));
					CHECK((engine.getPhase() == BattlePhase::NotStarted));
				}
			}

			GIVEN("party B's size is less than the required amount per side")
			{
				std::array<Pokemon *const, 1> newPartyB{&partyBOne};
				std::expected<void, BattleEngineError> result{engine.startBattle(pokemonA, newPartyB, ruleset, 2)};

				THEN("an error is returned")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error() == BattleEngineError::InvalidParty));
					CHECK((engine.getPhase() == BattlePhase::NotStarted));
				}
			}

			GIVEN("party A has a nullptr")
			{
				std::array<Pokemon *const, 2> newPartyA{&partyAOne, nullptr};
				std::expected<void, BattleEngineError> result{engine.startBattle(newPartyA, pokemonB, ruleset, 2)};

				THEN("an error is returned")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error() == BattleEngineError::InvalidParty));
					CHECK((engine.getPhase() == BattlePhase::NotStarted));
				}
			}

			GIVEN("party B has a nullptr")
			{
				std::array<Pokemon *const, 2> newPartyB{&partyBOne, nullptr};
				std::expected<void, BattleEngineError> result{engine.startBattle(pokemonA, newPartyB, ruleset, 2)};

				THEN("an error is returned")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error() == BattleEngineError::InvalidParty));
					CHECK((engine.getPhase() == BattlePhase::NotStarted));
				}
			}
		}

		WHEN("calling with duplicate pokemon pointers")
		{
			GIVEN("party A has a duplicate pointer from party B")
			{
				std::array<Pokemon *const, 2> newPartyA{&partyAOne, &partyBOne};
				std::expected<void, BattleEngineError> result{engine.startBattle(newPartyA, pokemonB, ruleset, 2)};

				THEN("an error is returned")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error() == BattleEngineError::InvalidParty));
					CHECK((engine.getPhase() == BattlePhase::NotStarted));
				}
			}

			GIVEN("party B has a duplicate pointer from party A")
			{
				std::array<Pokemon *const, 2> newPartyB{&partyBOne, &partyAOne};
				std::expected<void, BattleEngineError> result{engine.startBattle(pokemonA, newPartyB, ruleset, 2)};

				THEN("an error is returned")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error() == BattleEngineError::InvalidParty));
					CHECK((engine.getPhase() == BattlePhase::NotStarted));
				}
			}
		}

		WHEN("calling with fainted pokemon")
		{
			GIVEN("party A has less healthy pokemon than the battle amount")
			{
				Pokemon faintedPartyA{makePokemon({.mHealth = 0})};
				std::array<Pokemon *const, 2> newPartyA{&partyAOne, &faintedPartyA};
				std::expected<void, BattleEngineError> result{engine.startBattle(newPartyA, pokemonB, ruleset, 2)};

				THEN("an error is returned")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error() == BattleEngineError::InvalidParty));
					CHECK((engine.getPhase() == BattlePhase::NotStarted));
				}
			}

			GIVEN("party B has less healthy pokemon than the battle amount")
			{
				Pokemon faintedPartyB{makePokemon({.mHealth = 0})};
				std::array<Pokemon *const, 2> newPartyB{&partyBOne, &faintedPartyB};
				std::expected<void, BattleEngineError> result{engine.startBattle(pokemonA, newPartyB, ruleset, 2)};

				THEN("an error is returned")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error() == BattleEngineError::InvalidParty));
					CHECK((engine.getPhase() == BattlePhase::NotStarted));
				}
			}
		}

		WHEN("calling with an invalid ruleset")
		{
			RulesetPolicy invalidRuleset{.mMaxWeathers = 10};
			std::expected<void, BattleEngineError> result{engine.startBattle(pokemonA, pokemonB, invalidRuleset)};

			THEN("an error is returned")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error() == BattleEngineError::InvalidRuleset));
				CHECK((engine.getPhase() == BattlePhase::NotStarted));
			}
		}

		WHEN("calling with more active pokemon per side than allowed")
		{
			RulesetPolicy invalidRuleset{.mMaxSideSize = 1};
			std::expected<void, BattleEngineError> result{engine.startBattle(pokemonA, pokemonB, invalidRuleset, 2)};

			THEN("an error is returned")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error() == BattleEngineError::InvalidParty));
				CHECK((engine.getPhase() == BattlePhase::NotStarted));
			}
		}
	}

	GIVEN("get/set BattleRandomSources")
	{
		WHEN("calling getRandomSources")
		{
			const BattleRandomSources &result{engine.getRandomSources()};

			THEN("the sources match")
			{
				CHECK((&result.mUnsigned.get() == &getDefaultRandomSource<ub>()));
				CHECK((&result.mSigned.get() == &getDefaultRandomSource<sb>()));
				CHECK((&result.mReal.get() == &getDefaultRandomSource<double>()));
				CHECK((&result.mIndex.get() == &getDefaultRandomSource<std::size_t>()));
			}
		}

		// WHEN("calling setRandomSources")
		// {
		// 	auto minimumRandom = []<typename Value>(Value minimum, Value) { return minimum; };
		// 	CallableRandomSource<ub, decltype(minimumRandom)> unsignedSource{minimumRandom};
		// 	CallableRandomSource<sb, decltype(minimumRandom)> signedSource{minimumRandom};
		// 	CallableRandomSource<double, decltype(minimumRandom)> realSource{minimumRandom};
		// 	CallableRandomSource<std::size_t, decltype(minimumRandom)> indexSource{minimumRandom};
		// 	const BattleRandomSources sources{
		// 		.mUnsigned = unsignedSource,
		// 		.mSigned = signedSource,
		// 		.mReal = realSource,
		// 		.mIndex = indexSource,
		// 	};

		// 	THEN("an idle engine accepts every source and can restore the defaults")
		// 	{
		// 		REQUIRE(engine.setRandomSources(sources).has_value());

		// 		const BattleRandomSources &installed{engine.getRandomSources()};
		// 		CHECK((&installed.mUnsigned.get() == &unsignedSource));
		// 		CHECK((&installed.mSigned.get() == &signedSource));
		// 		CHECK((&installed.mReal.get() == &realSource));
		// 		CHECK((&installed.mIndex.get() == &indexSource));

		// 		REQUIRE(engine.setRandomSources({}).has_value());
		// 		CHECK((&installed.mUnsigned.get() == &getDefaultRandomSource<ub>()));
		// 		CHECK((&installed.mSigned.get() == &getDefaultRandomSource<sb>()));
		// 		CHECK((&installed.mReal.get() == &getDefaultRandomSource<double>()));
		// 		CHECK((&installed.mIndex.get() == &getDefaultRandomSource<std::size_t>()));
		// 	}

		// 	THEN("an engine remains configurable after validation returns an error")
		// 	{
		// 		const std::expected<void, BattleEngineError> startupResult{engine.startBattle(pokemonA, pokemonB, ruleset, 0)};

		// 		REQUIRE_FALSE(startupResult.has_value());
		// 		REQUIRE((startupResult.error() == BattleEngineError::InvalidParty));
		// 		REQUIRE(engine.setRandomSources(sources).has_value());
		// 		CHECK((&engine.getRandomSources().mUnsigned.get() == &unsignedSource));

		// 		const std::array<BattleAction, 0> actions{};
		// 		const std::expected<void, BattleEngineError> turnResult{engine.executeTurn(actions)};

		// 		REQUIRE_FALSE(turnResult.has_value());
		// 		REQUIRE((turnResult.error() == BattleEngineError::BattleNotStarted));
		// 		REQUIRE(engine.setRandomSources({}).has_value());
		// 		CHECK((&engine.getRandomSources().mUnsigned.get() == &getDefaultRandomSource<ub>()));
		// 	}

		// 	THEN("an executing engine rejects replacement without changing its sources and accepts it after the turn")
		// 	{
		// 		const MoveID moveID{toMoveID(BuiltinMoveID::Pound)};
		// 		MoveRegistryConfiguration moveConfiguration{};
		// 		REQUIRE(moveConfiguration.setMoveTriggers(moveID, {}).has_value());
		// 		provider.mMoveRegistry = &moveConfiguration.getRuntimeRegistry();

		// 		Pokemon attacker{
		// 			makePokemon({
		// 				.mStats = {.mMaxHealth = 100, .mAttack = 100, .mDefense = 100, .mSpAttack = 100, .mSpDefense = 100, .mSpeed = 100},
		// 				.mMoveIDs = {moveID},
		// 				.mMaxPP = {35},
		// 				.mCurrentPP = {35},
		// 				.mHealth = 100,
		// 			}),
		// 		};
		// 		Pokemon defender{
		// 			makePokemon({
		// 				.mStats = {.mMaxHealth = 100, .mAttack = 100, .mDefense = 100, .mSpAttack = 100, .mSpDefense = 100, .mSpeed = 100},
		// 				.mMoveIDs = {moveID},
		// 				.mMaxPP = {35},
		// 				.mCurrentPP = {35},
		// 				.mHealth = 100,
		// 			}),
		// 		};
		// 		const std::array<Pokemon *, 1> attackerParty{&attacker};
		// 		const std::array<Pokemon *, 1> defenderParty{&defender};
		// 		std::size_t callbackCount{0};

		// 		auto reentrantRandom = [&](std::size_t, std::size_t maximum) -> std::size_t {
		// 			++callbackCount;
		// 			const BattleRandomSources before{engine.getRandomSources()};
		// 			const std::expected<void, BattleEngineError> result{engine.setRandomSources(sources)};

		// 			REQUIRE_FALSE(result.has_value());
		// 			CHECK((result.error() == BattleEngineError::EngineBusy));
		// 			const BattleRandomSources &after{engine.getRandomSources()};
		// 			CHECK((&after.mUnsigned.get() == &before.mUnsigned.get()));
		// 			CHECK((&after.mSigned.get() == &before.mSigned.get()));
		// 			CHECK((&after.mReal.get() == &before.mReal.get()));
		// 			CHECK((&after.mIndex.get() == &before.mIndex.get()));
		// 			return maximum;
		// 		};
		// 		CallableRandomSource<std::size_t, decltype(reentrantRandom)> reentrantSource{reentrantRandom};
		// 		BattleRandomSources activeSources{sources};
		// 		activeSources.mIndex = reentrantSource;
		// 		REQUIRE(engine.setRandomSources(activeSources).has_value());
		// 		REQUIRE(engine.startBattle(attackerParty, defenderParty, ruleset).has_value());
		// 		REQUIRE(engine.setRandomSources(activeSources).has_value());

		// 		const std::array<BattleAction, 2> actions{
		// 			MoveAction{.mSelectedTarget = BattleTarget{.mSide = Side::B}, .mSide = Side::A},
		// 			MoveAction{.mSelectedTarget = BattleTarget{.mSide = Side::A}, .mSide = Side::B},
		// 		};
		// 		REQUIRE(engine.executeTurn(actions).has_value());
		// 		REQUIRE((callbackCount == 1));
		// 		REQUIRE(engine.setRandomSources(sources).has_value());
		// 		CHECK((&engine.getRandomSources().mIndex.get() == &indexSource));
		// 	}
		// }
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
