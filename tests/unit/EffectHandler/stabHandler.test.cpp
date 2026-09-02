/*! @file stabHandler.test.cpp
	@brief C++ file for running tests for the StabHandler.
	@date 09/02/2026
	@since 0.8.7
	@version 0.12.17
	@author Matthew Moore
*/

#include "EffectHandler/stabHandler.h"

#include "Battle/battleState.h"
#include "Battle/battleState.testHelper.h"
#include "Core/typedefs.h"
#include "Effect/effectContext.h"
#include "Effect/effectContext.testHelper.h"
#include "EffectHandler/stabHandler.testHelper.h"
#include "Pokemon/pokemon.h"
#include "Pokemon/pokemon.testHelper.h"
#include "Registry/registryProvider.h"
#include "Types/builtInTypeID.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Battle::BattleState;
using PocketCore::Core::ub;
using PocketCore::Effect::EffectContext;
using PocketCore::Effect::Side;
using PocketCore::Effect::StabHandler;
using PocketCore::Pokemon::Pokemon;
using PocketCore::Registry::RegistryProvider;
using PocketCore::Testing::hasStabMultiplier;
using PocketCore::Testing::makeBattleState;
using PocketCore::Testing::makeEffectContext;
using PocketCore::Testing::makePokemon;
using PocketCore::Type::BuiltinTypeID;
using PocketCore::Type::toTypeID;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("StabHandler")
{
	StabHandler stabHandler{};
	RegistryProvider provider{};
	EffectContext context{makeEffectContext({.mMoveTypeID = toTypeID(BuiltinTypeID::Fire), .mUserIndex = 0, .mUserSide = Side::A})};

	GIVEN("a user Pokemon with a matching move type")
	{
		Pokemon userPokemon{makePokemon({.mTypesIDs = {toTypeID(BuiltinTypeID::Fire)}})};

		BattleState battleState{makeBattleState({.mSideA = {{.mPokemon = &userPokemon}}})};

		WHEN("applying STAB handling")
		{
			stabHandler.apply(battleState, context, provider);

			THEN("the STAB multiplier is recorded")
			{
				CHECK(hasStabMultiplier(context));
			}
		}
	}

	GIVEN("a user Pokemon without a matching move type")
	{
		Pokemon userPokemon{makePokemon({.mTypesIDs = {toTypeID(BuiltinTypeID::Water)}})};

		BattleState battleState{makeBattleState({.mSideA = {{.mPokemon = &userPokemon}}})};

		WHEN("applying STAB handling")
		{
			stabHandler.apply(battleState, context, provider);

			THEN("the STAB multiplier is not recorded")
			{
				CHECK_FALSE(hasStabMultiplier(context));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
