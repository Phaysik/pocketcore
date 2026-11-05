/*! @file burnDamageHandler.test.cpp
	@brief C++ file for running tests for the BurnDamageHandler.
	@date 10/27/2026
	@since 0.12.53
	@version 0.12.53
	@author Matthew Moore
*/

#include "EffectHandler/burnDamageHandler.h"

#include "Ability/builtInAbilityID.h"
#include "Battle/battleRandomSources.h"
#include "Battle/battleState.h"
#include "Battle/battleState.testHelper.h"
#include "Core/typedefs.h"
#include "Effect/effectContext.h"
#include "Effect/effectContext.testHelper.h"
#include "EffectHandler/burnDamageHandler.testHelper.h"
#include "Move/builtInMoveID.h"
#include "Pokemon/pokemon.h"
#include "Pokemon/pokemon.testHelper.h"
#include "Registry/registryProvider.h"
#include "Registry/registryProvider.testHelper.h"
#include "Status/builtInStatusID.h"
#include "Status/statusID.h"
#include "Utility/Debug/Logging/logging.testHelper.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Ability::BuiltinAbilityID;
using PocketCore::Ability::toAbilityID;
using PocketCore::Battle::BattleRandomSources;
using PocketCore::Battle::BattleState;
using PocketCore::Core::ub;
using PocketCore::Effect::applyBurnDamage;
using PocketCore::Effect::BurnDamageHandler;
using PocketCore::Effect::EffectContext;
using PocketCore::Effect::Side;
using PocketCore::Move::BuiltinMoveID;
using PocketCore::Move::toMoveID;
using PocketCore::Pokemon::Pokemon;
using PocketCore::Registry::RegistryProvider;
using PocketCore::Status::BuiltinStatusID;
using PocketCore::Status::NO_STATUS_ID;
using PocketCore::Status::toStatusID;
using PocketCore::Testing::ensureLoggerInitialized;
using PocketCore::Testing::getDefaultInitializedRegistryProvider;
using PocketCore::Testing::hasBurnDamageMultiplier;
using PocketCore::Testing::makeBattleState;
using PocketCore::Testing::makeEffectContext;
using PocketCore::Testing::makePokemon;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("BurnDamageHandler")
{
	ensureLoggerInitialized("randomization handler test", "burnDamageHandler.log");

	BurnDamageHandler burnDamageHandler{};
	Pokemon sideA{
		makePokemon({
			.mStatusIDs = {toStatusID(BuiltinStatusID::Burn)},
			.mAbilityEntries = {{{.mAbilityID = toAbilityID(BuiltinAbilityID::AirLock)}}},
		}),
	};
	RegistryProvider provider{getDefaultInitializedRegistryProvider()};
	BattleRandomSources randomSources{};

	BattleState handlerState{makeBattleState({.mSideA = {{{.mPokemon = &sideA}, {.mPokemon = nullptr}}}})};
	BattleState freeFunctionState{makeBattleState({.mSideA = {{{.mPokemon = &sideA}, {.mPokemon = nullptr}}}})};
	EffectContext handlerContext{
		makeEffectContext({.mMoveID = toMoveID(BuiltinMoveID::ExtremeSpeed), .mUserIndex = 0, .mUserSide = Side::A, .mIsSpecial = false}),
	};
	EffectContext freeFunctionContext{
		makeEffectContext({.mMoveID = toMoveID(BuiltinMoveID::ExtremeSpeed), .mUserIndex = 0, .mUserSide = Side::A, .mIsSpecial = false}),
	};

	WHEN("calling with a null Pokemon")
	{
		handlerContext.mUserIndex = 1;
		freeFunctionContext.mUserIndex = 1;
		burnDamageHandler.apply(handlerState, handlerContext, provider, randomSources);
		applyBurnDamage(freeFunctionState, freeFunctionContext, provider, randomSources);

		THEN("the free function yields the same result as the handler")
		{
			CHECK_FALSE(hasBurnDamageMultiplier(handlerContext));
			CHECK_FALSE(hasBurnDamageMultiplier(freeFunctionContext));
		}
	}

	WHEN("calling without a burned Pokemon")
	{
		sideA.setStatusID(0, NO_STATUS_ID);
		burnDamageHandler.apply(handlerState, handlerContext, provider, randomSources);
		applyBurnDamage(freeFunctionState, freeFunctionContext, provider, randomSources);

		THEN("the free function yields the same result as the handler")
		{
			CHECK_FALSE(hasBurnDamageMultiplier(handlerContext));
			CHECK_FALSE(hasBurnDamageMultiplier(freeFunctionContext));
		}
	}

	WHEN("calling when the move is a special move")
	{
		handlerContext.mIsSpecial = true;
		freeFunctionContext.mIsSpecial = true;
		burnDamageHandler.apply(handlerState, handlerContext, provider, randomSources);
		applyBurnDamage(freeFunctionState, freeFunctionContext, provider, randomSources);

		THEN("the free function yields the same result as the handler")
		{
			CHECK_FALSE(hasBurnDamageMultiplier(handlerContext));
			CHECK_FALSE(hasBurnDamageMultiplier(freeFunctionContext));
		}
	}

	WHEN("calling when the move is a special move")
	{
		sideA.setAbilityEntry(0, {.mAbilityID = toAbilityID(BuiltinAbilityID::Guts)});
		burnDamageHandler.apply(handlerState, handlerContext, provider, randomSources);
		applyBurnDamage(freeFunctionState, freeFunctionContext, provider, randomSources);

		THEN("the free function yields the same result as the handler")
		{
			CHECK_FALSE(hasBurnDamageMultiplier(handlerContext));
			CHECK_FALSE(hasBurnDamageMultiplier(freeFunctionContext));
		}
	}

	WHEN("calling when the move is Facade")
	{
		handlerContext.mMoveID = toMoveID(BuiltinMoveID::Facade);
		freeFunctionContext.mMoveID = toMoveID(BuiltinMoveID::Facade);
		burnDamageHandler.apply(handlerState, handlerContext, provider, randomSources);
		applyBurnDamage(freeFunctionState, freeFunctionContext, provider, randomSources);

		THEN("the free function yields the same result as the handler")
		{
			CHECK_FALSE(hasBurnDamageMultiplier(handlerContext));
			CHECK_FALSE(hasBurnDamageMultiplier(freeFunctionContext));
		}
	}

	WHEN("calling with valid data")
	{
		burnDamageHandler.apply(handlerState, handlerContext, provider, randomSources);
		applyBurnDamage(freeFunctionState, freeFunctionContext, provider, randomSources);

		THEN("the multiplier is set")
		{
			CHECK(hasBurnDamageMultiplier(handlerContext));
			CHECK(hasBurnDamageMultiplier(freeFunctionContext));
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
