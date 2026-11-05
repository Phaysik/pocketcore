/*! @file populationBombHandler.test.cpp
	@brief C++ file for running tests for the PopulationBombHandler.
	@date 11/03/2026
	@since 0.12.53
	@version 0.12.53
	@author Matthew Moore
*/

#include "EffectHandler/populationBombHandler.h"

#include "Battle/battleRandomSources.h"
#include "Battle/battleState.h"
#include "Configuration/constants.h"
#include "Effect/effectContext.h"
#include "EffectHandler/populationBombHandler.testHelper.h"
#include "Registry/registryProvider.h"
#include "Registry/registryProvider.testHelper.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Battle::BattleRandomSources;
using PocketCore::Battle::BattleState;
using PocketCore::Configuration::POPULATION_BOMB_CONSECUTIVE_HIT_MULTIPLIER_VALUE;
using PocketCore::Configuration::POPULATION_BOMB_FIRST_HIT_MULTIPLIER_VALUE;
using PocketCore::Effect::applyPopulationBomb;
using PocketCore::Effect::EffectContext;
using PocketCore::Effect::PopulationBombHandler;
using PocketCore::Registry::RegistryProvider;
using PocketCore::Testing::getDefaultInitializedRegistryProvider;
using PocketCore::Testing::hasPopulationBombMultiplier;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("PopulationBombHandler")
{
	PopulationBombHandler populationBombHandler{};
	BattleState handlerState{};
	BattleState freeFunctionState{};
	RegistryProvider provider{getDefaultInitializedRegistryProvider()};
	EffectContext handlerContext{};
	EffectContext freeFunctionContext{};
	BattleRandomSources handlerRandomSources{};
	BattleRandomSources freeFunctionRandomSources{};

	GIVEN("a context where the hit attempt index is 0")
	{
		handlerContext.mHitAttemptIndex = 0;
		freeFunctionContext.mHitAttemptIndex = 0;

		WHEN("population bomb hit handling is applied")
		{
			populationBombHandler.apply(handlerState, handlerContext, provider, handlerRandomSources);
			applyPopulationBomb(freeFunctionState, freeFunctionContext, provider, freeFunctionRandomSources);

			THEN("no multiplier value is set")
			{
				CHECK_FALSE(hasPopulationBombMultiplier(handlerContext, POPULATION_BOMB_FIRST_HIT_MULTIPLIER_VALUE));
				CHECK_FALSE(hasPopulationBombMultiplier(freeFunctionContext, POPULATION_BOMB_FIRST_HIT_MULTIPLIER_VALUE));

				CHECK_FALSE(hasPopulationBombMultiplier(handlerContext, POPULATION_BOMB_CONSECUTIVE_HIT_MULTIPLIER_VALUE));
				CHECK_FALSE(hasPopulationBombMultiplier(freeFunctionContext, POPULATION_BOMB_CONSECUTIVE_HIT_MULTIPLIER_VALUE));
			}
		}
	}

	GIVEN("a context where the hit attempt index is 1")
	{
		handlerContext.mHitAttemptIndex = 1;
		freeFunctionContext.mHitAttemptIndex = 1;

		WHEN("population bomb hit handling is applied")
		{
			populationBombHandler.apply(handlerState, handlerContext, provider, handlerRandomSources);
			applyPopulationBomb(freeFunctionState, freeFunctionContext, provider, freeFunctionRandomSources);

			THEN("the first hit multiplier value is set")
			{
				CHECK(hasPopulationBombMultiplier(handlerContext, POPULATION_BOMB_FIRST_HIT_MULTIPLIER_VALUE));
				CHECK(hasPopulationBombMultiplier(freeFunctionContext, POPULATION_BOMB_FIRST_HIT_MULTIPLIER_VALUE));
			}
		}
	}

	GIVEN("a context where the hit attempt index is greater than 1")
	{
		handlerContext.mHitAttemptIndex = 2;
		freeFunctionContext.mHitAttemptIndex = 2;

		WHEN("population bomb hit handling is applied")
		{
			populationBombHandler.apply(handlerState, handlerContext, provider, handlerRandomSources);
			applyPopulationBomb(freeFunctionState, freeFunctionContext, provider, freeFunctionRandomSources);

			THEN("the consecutive hit multiplier value is set")
			{
				CHECK(hasPopulationBombMultiplier(handlerContext, POPULATION_BOMB_CONSECUTIVE_HIT_MULTIPLIER_VALUE));
				CHECK(hasPopulationBombMultiplier(freeFunctionContext, POPULATION_BOMB_CONSECUTIVE_HIT_MULTIPLIER_VALUE));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
