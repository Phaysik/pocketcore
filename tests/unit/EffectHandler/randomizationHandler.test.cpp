/*! @file randomizationHandler.test.cpp
	@brief C++ file for running tests for the RandomizationHandler.
	@date 10/19/2026
	@since 0.12.51
	@version 0.12.51
	@author Matthew Moore
*/

#include "EffectHandler/randomizationHandler.h"

#include "Battle/battleRandomSources.h"
#include "Battle/battleState.h"
#include "Battle/battleState.testHelper.h"
#include "Core/typedefs.h"
#include "Effect/effectContext.h"
#include "Effect/effectContext.testHelper.h"
#include "EffectHandler/randomizationHandler.testHelper.h"
#include "Pokemon/pokemon.h"
#include "Pokemon/pokemon.testHelper.h"
#include "Registry/registryProvider.h"
#include "Utility/Debug/Logging/logging.testHelper.h"
#include "Utility/Random/callableRandomSource.h"
#include "Utility/Random/random.testHelper.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Battle::BattleRandomSources;
using PocketCore::Battle::BattleState;
using PocketCore::Core::ub;
using PocketCore::Effect::EffectContext;
using PocketCore::Effect::RandomizationHandler;
using PocketCore::Effect::Side;
using PocketCore::Pokemon::Pokemon;
using PocketCore::Registry::RegistryProvider;
using PocketCore::Testing::ensureLoggerInitialized;
using PocketCore::Testing::FixedRandom;
using PocketCore::Testing::hasRandomizationMultiplier;
using PocketCore::Testing::makeBattleState;
using PocketCore::Testing::makeEffectContext;
using PocketCore::Testing::makePokemon;
using PocketCore::Utility::Random::CallableRandomSource;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("RandomizationHandler")
{
	ensureLoggerInitialized("randomization handler test", "randomizationHandler.log");

	RandomizationHandler randomizationHandler{};
	Pokemon sideA{makePokemon({})};
	Pokemon sideB{makePokemon({})};
	BattleState battleState{makeBattleState({.mSideA = {{{.mPokemon = &sideA}}}, .mSideB = {{{.mPokemon = &sideB}}}})};
	RegistryProvider provider{};
	EffectContext context{makeEffectContext({.mUserIndex = 0, .mTargetIndex = 0, .mUserSide = Side::A, .mTargetSide = Side::B})};
	BattleRandomSources randomSources{};

	GIVEN("a fixed value that produces 1/5")
	{
		WHEN("randomization handling is applied")
		{
			CallableRandomSource<ub, FixedRandom<ub>> unsignedSource{FixedRandom<ub>{20}};
			randomSources.mUnsigned = unsignedSource;
			randomizationHandler.apply(battleState, context, provider, randomSources);

			THEN("the multiplier is set")
			{
				CHECK(hasRandomizationMultiplier(context, 0.20));
			}
		}
	}

	GIVEN("a fixed value that produces 1.0")
	{
		WHEN("randomization handling is applied")
		{
			CallableRandomSource<ub, FixedRandom<ub>> unsignedSource{FixedRandom<ub>{100}};
			randomSources.mUnsigned = unsignedSource;
			battleState.mSideA = {{{.mPokemon = nullptr}}};
			battleState.mSideB = {{{.mPokemon = nullptr}}};
			context.mUserSide = Side::B;
			context.mTargetSide = Side::A;
			randomizationHandler.apply(battleState, context, provider, randomSources);

			THEN("the multiplier is set")
			{
				CHECK(hasRandomizationMultiplier(context));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
