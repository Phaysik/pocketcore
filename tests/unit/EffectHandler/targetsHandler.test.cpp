/*! @file targetsHandler.test.cpp
	@brief C++ file for running tests for the TargetsHandler.
	@date 10/27/2026
	@since 0.8.7
	@version 0.12.53
	@author Matthew Moore
*/

#include "EffectHandler/targetsHandler.h"

#include "Battle/battleRandomSources.h"
#include "Battle/battleState.h"
#include "Battle/battleTargetsAndTriggers.h"
#include "Configuration/moveRegistryConfiguration.h"
#include "Effect/effectContext.h"
#include "Effect/effectContext.testHelper.h"
#include "EffectHandler/targetsHandler.testHelper.h"
#include "Move/builtInMoveID.h"
#include "Move/moveID.h"
#include "Registry/moveRegistry.h"
#include "Registry/moveRegistry.testHelper.h"
#include "Registry/registryProvider.h"
#include "Registry/registryProvider.testHelper.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Battle::BattleRandomSources;
using PocketCore::Battle::BattleState;
using PocketCore::Battle::BattleTargetID;
using PocketCore::Configuration::MoveRegistryConfiguration;
using PocketCore::Effect::applyTargets;
using PocketCore::Effect::EffectContext;
using PocketCore::Effect::TargetsHandler;
using PocketCore::Move::BuiltinMoveID;
using PocketCore::Move::MoveID;
using PocketCore::Move::toMoveID;
using PocketCore::Registry::Move::MoveRegistry;
using PocketCore::Registry::RegistryProvider;
using PocketCore::Testing::getNullMoveRegistryProvider;
using PocketCore::Testing::hasTargetsMultiplier;
using PocketCore::Testing::makeEffectContext;
using PocketCore::Testing::registerMove;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

SCENARIO("TargetsHandler")
{
	TargetsHandler targetsHandler{};
	BattleState handlerState{};
	BattleState freeFunctionState{};
	MoveRegistryConfiguration moveConfiguration{};
	const MoveRegistry &moveRegistry{moveConfiguration.getRuntimeRegistry()};
	RegistryProvider provider{.mMoveRegistry = &moveRegistry};
	BattleRandomSources randomSources{};
	EffectContext handlerContext{makeEffectContext({.mMoveID = toMoveID(BuiltinMoveID::Pound)})};
	EffectContext freeFunctionContext{makeEffectContext({.mMoveID = toMoveID(BuiltinMoveID::Pound)})};

	GIVEN("a null move provider")
	{
		RegistryProvider nullProvider{getNullMoveRegistryProvider()};

		WHEN("applying target handling")
		{
			targetsHandler.apply(handlerState, handlerContext, nullProvider, randomSources);
			applyTargets(freeFunctionState, freeFunctionContext, nullProvider, randomSources);

			THEN("no targets spread multiplier is added")
			{
				CHECK_FALSE(hasTargetsMultiplier(handlerContext));
				CHECK_FALSE(hasTargetsMultiplier(freeFunctionContext));
			}
		}
	}

	GIVEN("an invalid moveID")
	{
		handlerContext.mMoveID = MoveID{200};
		freeFunctionContext.mMoveID = MoveID{200};

		WHEN("applying target handling")
		{
			targetsHandler.apply(handlerState, handlerContext, provider, randomSources);
			applyTargets(freeFunctionState, freeFunctionContext, provider, randomSources);

			THEN("no targets spread multiplier is added")
			{
				CHECK_FALSE(hasTargetsMultiplier(handlerContext));
				CHECK_FALSE(hasTargetsMultiplier(freeFunctionContext));
			}
		}
	}

	GIVEN("a move that targets a single opponent")
	{
		handlerContext.mMoveID = toMoveID(BuiltinMoveID::Pound);
		freeFunctionContext.mMoveID = toMoveID(BuiltinMoveID::Pound);

		WHEN("applying target handling")
		{
			targetsHandler.apply(handlerState, handlerContext, provider, randomSources);
			applyTargets(freeFunctionState, freeFunctionContext, provider, randomSources);

			THEN("no targets spread multiplier is added")
			{
				CHECK_FALSE(hasTargetsMultiplier(handlerContext));
				CHECK_FALSE(hasTargetsMultiplier(freeFunctionContext));
			}
		}
	}

	GIVEN("a move that targets only self")
	{
		MoveID registeredMove{registerMove(moveConfiguration, {.mTargetID = BattleTargetID::Self})};
		handlerContext.mMoveID = registeredMove;
		freeFunctionContext.mMoveID = registeredMove;

		WHEN("applying target handling")
		{
			targetsHandler.apply(handlerState, handlerContext, provider, randomSources);
			applyTargets(freeFunctionState, freeFunctionContext, provider, randomSources);

			THEN("no targets spread multiplier is added")
			{
				CHECK_FALSE(hasTargetsMultiplier(handlerContext));
				CHECK_FALSE(hasTargetsMultiplier(freeFunctionContext));
			}
		}
	}

	GIVEN("a move that can hit multiple targets")
	{
		MoveID registeredMove{registerMove(moveConfiguration, {.mTargetID = BattleTargetID::AllOpponents})};
		handlerContext.mMoveID = registeredMove;
		freeFunctionContext.mMoveID = registeredMove;

		WHEN("applying target handling")
		{
			targetsHandler.apply(handlerState, handlerContext, provider, randomSources);
			applyTargets(freeFunctionState, freeFunctionContext, provider, randomSources);

			THEN("the targets spread multiplier is added")
			{
				CHECK(hasTargetsMultiplier(handlerContext));
				CHECK(hasTargetsMultiplier(freeFunctionContext));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
