/*! @file criticalHitHandler.test.cpp
	@brief C++ file for running tests for the CriticalHitHandler.
	@date 10/19/2026
	@since 0.8.7
	@version 0.12.51
	@author Matthew Moore
*/

#include "EffectHandler/criticalHitHandler.h"

#include "Battle/battleRandomSources.h"
#include "Battle/battleState.h"
#include "Core/typedefs.h"
#include "Effect/effectContext.h"
#include "EffectHandler/criticalHitHandler.testHelper.h"
#include "Registry/registryProvider.h"
#include "Utility/Random/callableRandomSource.h"
#include "Utility/Random/random.testHelper.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Battle::BattleRandomSources;
using PocketCore::Battle::BattleState;
using PocketCore::Core::ub;
using PocketCore::Effect::CriticalHitHandler;
using PocketCore::Effect::EffectContext;
using PocketCore::Registry::RegistryProvider;
using PocketCore::Testing::FixedRandom;
using PocketCore::Testing::hasCriticalMultiplier;
using PocketCore::Utility::Random::CallableRandomSource;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("CriticalHitHandler")
{
	CriticalHitHandler criticalHitHandler{};
	BattleState battleState{};
	RegistryProvider provider{};
	EffectContext context{};
	BattleRandomSources randomSources{};

	GIVEN("a fixed random value that produces a critical roll")
	{
		WHEN("critical hit handling is applied")
		{
			CallableRandomSource<ub, FixedRandom<ub>> unsignedSource{FixedRandom<ub>{0}};
			randomSources.mUnsigned = unsignedSource;
			criticalHitHandler.apply(battleState, context, provider, randomSources);

			THEN("critical state and multiplier are set")
			{
				CHECK(context.mDamage.mIsCritical);
				CHECK(hasCriticalMultiplier(context));
			}
		}
	}

	GIVEN("a fixed random value that won't produces a critical roll")
	{
		WHEN("critical hit handling is applied")
		{
			CallableRandomSource<ub, FixedRandom<ub>> unsignedSource{FixedRandom<ub>{99}};
			randomSources.mUnsigned = unsignedSource;
			criticalHitHandler.apply(battleState, context, provider, randomSources);

			THEN("critical state and multiplier are not set")
			{
				CHECK_FALSE(context.mDamage.mIsCritical);
				CHECK_FALSE(hasCriticalMultiplier(context));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
