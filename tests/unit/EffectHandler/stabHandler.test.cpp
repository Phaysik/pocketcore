/*! @file stabHandler.test.cpp
	@brief C++ file for running tests for the StabHandler.
	@date 10/27/2026
	@since 0.8.7
	@version 0.12.53
	@author Matthew Moore
*/

#include "EffectHandler/stabHandler.h"

#include "Battle/battleRandomSources.h"
#include "Battle/battleState.h"
#include "Battle/battleState.testHelper.h"
#include "Core/typedefs.h"
#include "Effect/effectContext.h"
#include "Effect/effectContext.testHelper.h"
#include "EffectHandler/stabHandler.testHelper.h"
#include "Pokemon/pokemon.h"
#include "Pokemon/pokemon.testHelper.h"
#include "Registry/registryProvider.h"
#include "Registry/registryProvider.testHelper.h"
#include "Type/builtInTypeID.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Battle::BattleRandomSources;
using PocketCore::Battle::BattleState;
using PocketCore::Core::ub;
using PocketCore::Effect::applySTAB;
using PocketCore::Effect::EffectContext;
using PocketCore::Effect::Side;
using PocketCore::Effect::StabHandler;
using PocketCore::Pokemon::Pokemon;
using PocketCore::Registry::RegistryProvider;
using PocketCore::Testing::getDefaultInitializedRegistryProvider;
using PocketCore::Testing::hasStabMultiplier;
using PocketCore::Testing::makeBattleState;
using PocketCore::Testing::makeEffectContext;
using PocketCore::Testing::makePokemon;
using PocketCore::Type::BuiltinTypeID;
using PocketCore::Type::toTypeID;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("StabHandler")
{
	Pokemon userPokemon{makePokemon({.mMetadata = {.mTypeIDs = {toTypeID(BuiltinTypeID::Fire)}}})};
	BattleState handlerState{makeBattleState({.mSideA = {{.mPokemon = &userPokemon}}})};
	BattleState freeFunctionState{makeBattleState({.mSideA = {{.mPokemon = &userPokemon}}})};
	StabHandler stabHandler{};
	RegistryProvider provider{getDefaultInitializedRegistryProvider()};
	EffectContext handlerContext{makeEffectContext({.mMoveTypeID = toTypeID(BuiltinTypeID::Fire), .mUserIndex = 0, .mUserSide = Side::A})};
	EffectContext freeFunctionContext{
		makeEffectContext({.mMoveTypeID = toTypeID(BuiltinTypeID::Fire), .mUserIndex = 0, .mUserSide = Side::A}),
	};
	BattleRandomSources randomSources{};

	GIVEN("a user Pokemon with a matching move type")
	{

		WHEN("applying STAB handling")
		{
			stabHandler.apply(handlerState, handlerContext, provider, randomSources);
			applySTAB(freeFunctionState, freeFunctionContext, provider, randomSources);

			THEN("the STAB multiplier is recorded")
			{
				CHECK(hasStabMultiplier(handlerContext));
				CHECK(hasStabMultiplier(freeFunctionContext));
			}
		}
	}

	GIVEN("a user Pokemon without a matching move type")
	{
		userPokemon.setTypeID(0, toTypeID(BuiltinTypeID::Water));

		WHEN("applying STAB handling")
		{
			stabHandler.apply(handlerState, handlerContext, provider, randomSources);
			applySTAB(freeFunctionState, freeFunctionContext, provider, randomSources);

			THEN("the STAB multiplier is not recorded")
			{
				CHECK_FALSE(hasStabMultiplier(handlerContext));
				CHECK_FALSE(hasStabMultiplier(freeFunctionContext));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
