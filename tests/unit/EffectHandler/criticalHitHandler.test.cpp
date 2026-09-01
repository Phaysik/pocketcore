/*! @file criticalHitHandler.test.cpp
	@brief C++ file for running tests for the CriticalHitHandler.
	@date 09/01/2026
	@since 0.8.7
	@version 0.12.14
	@author Matthew Moore
*/

#include "EffectHandler/criticalHitHandler.h"

#include "Battle/battleState.h"
#include "Core/typedefs.h"
#include "Effect/effectContext.h"
#include "EffectHandler/criticalHitHandler.testHelper.h"
#include "Registry/registryProvider.h"
#include "Utility/random.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Battle::BattleState;
using PocketCore::Core::ub;
using PocketCore::Effect::CriticalHitHandler;
using PocketCore::Effect::EffectContext;
using PocketCore::Registry::RegistryProvider;
using PocketCore::Testing::hasCriticalMultiplier;
using PocketCore::Utility::Random;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("CriticalHitHandler")
{
	CriticalHitHandler criticalHitHandler{};
	BattleState battleState{};
	RegistryProvider provider{};
	EffectContext context{};

	GIVEN("a seed that deterministically produces a critical roll")
	{
		WHEN("critical hit handling is applied")
		{
			Random::setSeed(9);
			criticalHitHandler.apply(battleState, context, provider);

			THEN("critical state and multiplier are set")
			{
				CHECK(context.mDamage.mIsCritical);
				CHECK(hasCriticalMultiplier(context));
			}
		}
	}

	GIVEN("a seed that deterministically won't produces a critical roll")
	{
		WHEN("critical hit handling is applied")
		{
			Random::setSeed(2);
			criticalHitHandler.apply(battleState, context, provider);

			THEN("critical state and multiplier are set")
			{
				CHECK_FALSE(context.mDamage.mIsCritical);
				CHECK_FALSE(hasCriticalMultiplier(context));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
