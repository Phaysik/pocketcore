/*! @file criticalHitHandler.test.cpp
	@brief C++ file for running tests for the CriticalHitHandler.
	@date 10/27/2026
	@since 0.8.7
	@version 0.12.53
	@author Matthew Moore
*/

#include "EffectHandler/criticalHitHandler.h"

#include "Battle/battleRandomSources.h"
#include "Battle/battleState.h"
#include "Core/typedefs.h"
#include "Effect/effectContext.h"
#include "EffectHandler/criticalHitHandler.testHelper.h"
#include "Registry/registryProvider.h"
#include "Registry/registryProvider.testHelper.h"
#include "Utility/Random/callableRandomSource.h"
#include "Utility/Random/random.testHelper.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Battle::BattleRandomSources;
using PocketCore::Battle::BattleState;
using PocketCore::Core::ub;
using PocketCore::Effect::applyCriticalHit;
using PocketCore::Effect::CriticalHitHandler;
using PocketCore::Effect::EffectContext;
using PocketCore::Registry::RegistryProvider;
using PocketCore::Testing::FixedRandom;
using PocketCore::Testing::getDefaultInitializedRegistryProvider;
using PocketCore::Testing::hasCriticalMultiplier;
using PocketCore::Utility::Random::CallableRandomSource;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("CriticalHitHandler")
{
	CriticalHitHandler criticalHitHandler{};
	BattleState handlerState{};
	BattleState freeFunctionState{};
	RegistryProvider provider{getDefaultInitializedRegistryProvider()};
	EffectContext handlerContext{};
	EffectContext freeFunctionContext{};
	BattleRandomSources handlerRandomSources{};
	BattleRandomSources freeFunctionRandomSources{};

	GIVEN("a fixed random value that produces a critical roll")
	{
		WHEN("critical hit handling is applied")
		{
			CallableRandomSource<ub, FixedRandom<ub>> handlerUnsignedSource{FixedRandom<ub>{0}};
			CallableRandomSource<ub, FixedRandom<ub>> freeFunctionUnsignedSource{FixedRandom<ub>{0}};
			handlerRandomSources.mUnsigned = handlerUnsignedSource;
			freeFunctionRandomSources.mUnsigned = freeFunctionUnsignedSource;

			criticalHitHandler.apply(handlerState, handlerContext, provider, handlerRandomSources);
			applyCriticalHit(freeFunctionState, freeFunctionContext, provider, freeFunctionRandomSources);

			THEN("critical state and multiplier are set")
			{
				CHECK(handlerContext.mDamage.mIsCritical);
				CHECK(hasCriticalMultiplier(handlerContext));

				CHECK(freeFunctionContext.mDamage.mIsCritical);
				CHECK(hasCriticalMultiplier(freeFunctionContext));
			}
		}
	}

	GIVEN("a fixed random value that won't produces a critical roll")
	{
		WHEN("critical hit handling is applied")
		{
			CallableRandomSource<ub, FixedRandom<ub>> handlerUnsignedSource{FixedRandom<ub>{99}};
			CallableRandomSource<ub, FixedRandom<ub>> freeFunctionUnsignedSource{FixedRandom<ub>{99}};
			handlerRandomSources.mUnsigned = handlerUnsignedSource;
			freeFunctionRandomSources.mUnsigned = freeFunctionUnsignedSource;

			criticalHitHandler.apply(handlerState, handlerContext, provider, handlerRandomSources);
			applyCriticalHit(freeFunctionState, freeFunctionContext, provider, freeFunctionRandomSources);

			THEN("critical state and multiplier are not set")
			{
				CHECK_FALSE(handlerContext.mDamage.mIsCritical);
				CHECK_FALSE(hasCriticalMultiplier(handlerContext));

				CHECK_FALSE(freeFunctionContext.mDamage.mIsCritical);
				CHECK_FALSE(hasCriticalMultiplier(freeFunctionContext));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
