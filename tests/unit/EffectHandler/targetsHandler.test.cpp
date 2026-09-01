/*! @file targetsHandler.test.cpp
	@brief C++ file for running tests for the TargetsHandler.
	@date 09/01/2026
	@since 0.8.7
	@version 0.12.14
	@author Matthew Moore
*/

#include "EffectHandler/targetsHandler.h"

#include "Battle/battleState.h"
#include "Battle/battleTargetsAndTriggers.h"
#include "Effect/effectContext.h"
#include "Effect/effectContext.testHelper.h"
#include "EffectHandler/targetsHandler.testHelper.h"
#include "Move/builtInMoveID.h"
#include "Move/moveID.h"
#include "Registry/moveRegistry.h"
#include "Registry/moveRegistry.testHelper.h"
#include "Registry/registryProvider.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Battle::BattleState;
using PocketCore::Battle::BattleTargetID;
using PocketCore::Effect::EffectContext;
using PocketCore::Effect::TargetsHandler;
using PocketCore::Move::BuiltinMoveID;
using PocketCore::Move::MoveID;
using PocketCore::Move::toMoveID;
using PocketCore::Registry::Move::MoveRegistry;
using PocketCore::Registry::RegistryProvider;
using PocketCore::Testing::hasTargetsMultiplier;
using PocketCore::Testing::makeEffectContext;
using PocketCore::Testing::registerMove;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

SCENARIO("TargetsHandler")
{
	TargetsHandler targetsHandler{};
	BattleState battleState{};
	MoveRegistry moveRegistry{};
	RegistryProvider provider{.moveRegistry = &moveRegistry};

	GIVEN("a move that targets a single opponent")
	{
		EffectContext context{makeEffectContext({.mMoveID = toMoveID(BuiltinMoveID::Pound)})};

		WHEN("applying target handling")
		{
			targetsHandler.apply(battleState, context, provider);

			THEN("no targets spread multiplier is added")
			{
				CHECK_FALSE(hasTargetsMultiplier(context));
			}
		}
	}

	GIVEN("a move that targets only self")
	{
		EffectContext context{makeEffectContext({.mMoveID = registerMove(moveRegistry, {.mTargetID = BattleTargetID::Self})})};

		WHEN("applying target handling")
		{
			targetsHandler.apply(battleState, context, provider);

			THEN("no targets spread multiplier is added")
			{
				CHECK_FALSE(hasTargetsMultiplier(context));
			}
		}
	}

	GIVEN("a move that can hit multiple targets")
	{
		EffectContext context{makeEffectContext({.mMoveID = registerMove(moveRegistry, {.mTargetID = BattleTargetID::AllOpponents})})};

		WHEN("applying target handling")
		{
			targetsHandler.apply(battleState, context, provider);

			THEN("the targets spread multiplier is added")
			{
				CHECK(hasTargetsMultiplier(context));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
