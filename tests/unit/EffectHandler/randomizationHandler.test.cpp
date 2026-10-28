/*! @file randomizationHandler.test.cpp
	@brief C++ file for running tests for the RandomizationHandler.
	@date 10/27/2026
	@since 0.12.51
	@version 0.12.53
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
#include "Registry/registryProvider.testHelper.h"
#include "Utility/Debug/Logging/logging.testHelper.h"
#include "Utility/Random/callableRandomSource.h"
#include "Utility/Random/random.testHelper.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Battle::BattleRandomSources;
using PocketCore::Battle::BattleState;
using PocketCore::Core::ub;
using PocketCore::Effect::applyRandomization;
using PocketCore::Effect::EffectContext;
using PocketCore::Effect::RandomizationHandler;
using PocketCore::Effect::Side;
using PocketCore::Pokemon::Pokemon;
using PocketCore::Registry::RegistryProvider;
using PocketCore::Testing::ensureLoggerInitialized;
using PocketCore::Testing::FixedRandom;
using PocketCore::Testing::getDefaultInitializedRegistryProvider;
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
	BattleState handlerState{makeBattleState({.mSideA = {{{.mPokemon = &sideA}}}, .mSideB = {{{.mPokemon = &sideB}}}})};
	BattleState freeFunctionState{makeBattleState({.mSideA = {{{.mPokemon = &sideA}}}, .mSideB = {{{.mPokemon = &sideB}}}})};
	RegistryProvider provider{getDefaultInitializedRegistryProvider()};
	EffectContext handlerContext{makeEffectContext({.mUserIndex = 0, .mTargetIndex = 0, .mUserSide = Side::A, .mTargetSide = Side::B})};
	EffectContext freeFunctionContext{
		makeEffectContext({.mUserIndex = 0, .mTargetIndex = 0, .mUserSide = Side::A, .mTargetSide = Side::B}),
	};
	BattleRandomSources handlerRandomSources{};
	BattleRandomSources freeFunctionRandomSources{};

	GIVEN("a fixed value that produces 1/5")
	{
		WHEN("randomization handling is applied")
		{
			CallableRandomSource<ub, FixedRandom<ub>> handlerUnsignedSource{FixedRandom<ub>{20}};
			CallableRandomSource<ub, FixedRandom<ub>> freeFunctionUnsignedSource{FixedRandom<ub>{20}};
			handlerRandomSources.mUnsigned = handlerUnsignedSource;
			freeFunctionRandomSources.mUnsigned = freeFunctionUnsignedSource;

			randomizationHandler.apply(handlerState, handlerContext, provider, handlerRandomSources);
			applyRandomization(freeFunctionState, freeFunctionContext, provider, freeFunctionRandomSources);

			THEN("the multiplier is set")
			{
				CHECK(hasRandomizationMultiplier(handlerContext, 0.20));
				CHECK(hasRandomizationMultiplier(freeFunctionContext, 0.20));
			}
		}
	}

	GIVEN("a fixed value that produces 1.0")
	{
		WHEN("randomization handling is applied")
		{
			CallableRandomSource<ub, FixedRandom<ub>> handlerUnsignedSource{FixedRandom<ub>{100}};
			CallableRandomSource<ub, FixedRandom<ub>> freeFunctionUnsignedSource{FixedRandom<ub>{100}};
			handlerRandomSources.mUnsigned = handlerUnsignedSource;
			freeFunctionRandomSources.mUnsigned = freeFunctionUnsignedSource;

			handlerState.mSideA = {{{.mPokemon = nullptr}}};
			handlerState.mSideB = {{{.mPokemon = nullptr}}};
			freeFunctionState.mSideA = {{{.mPokemon = nullptr}}};
			freeFunctionState.mSideB = {{{.mPokemon = nullptr}}};

			handlerContext.mUserSide = Side::B;
			handlerContext.mTargetSide = Side::A;
			freeFunctionContext.mUserSide = Side::B;
			freeFunctionContext.mTargetSide = Side::A;

			randomizationHandler.apply(handlerState, handlerContext, provider, handlerRandomSources);
			applyRandomization(freeFunctionState, freeFunctionContext, provider, freeFunctionRandomSources);

			THEN("the multiplier is set")
			{
				CHECK(hasRandomizationMultiplier(handlerContext));
				CHECK(hasRandomizationMultiplier(freeFunctionContext));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
