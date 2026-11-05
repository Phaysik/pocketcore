/*! @file setSandstormHandler.test.cpp
	@brief C++ file for running tests for the SetSandstormHandler.
	@date 10/27/2026
	@since 0.12.43
	@version 0.12.53
	@author Matthew Moore
*/

#include "EffectHandler/setSandstormHandler.h"

#include <algorithm>

#include "Battle/battleRandomSources.h"
#include "Battle/battleState.h"
#include "Effect/effectContext.h"
#include "Registry/registryProvider.h"
#include "Registry/registryProvider.testHelper.h"
#include "Weather/builtInWeatherID.h"
#include "Weather/weatherID.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Battle::BattleRandomSources;
using PocketCore::Battle::BattleState;
using PocketCore::Effect::applySetSandstorm;
using PocketCore::Effect::EffectContext;
using PocketCore::Effect::SetSandstormHandler;
using PocketCore::Registry::RegistryProvider;
using PocketCore::Testing::getDefaultInitializedRegistryProvider;
using PocketCore::Weather::BuiltinWeatherID;
using PocketCore::Weather::toWeatherID;
using PocketCore::Weather::WeatherID;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

SCENARIO("SetSandstormHandler")
{
	SetSandstormHandler setSandstormHandler{};
	RegistryProvider provider{getDefaultInitializedRegistryProvider()};
	BattleRandomSources randomSources{};

	WeatherID sandstormID{toWeatherID(BuiltinWeatherID::Sandstorm)};
	WeatherID hailID{toWeatherID(BuiltinWeatherID::Hail)};

	BattleState handlerState{};
	BattleState freeFunctionState{};
	EffectContext handlerContext{};
	EffectContext freeFunctionContext{};

	WHEN("applied to empty active weathers")
	{
		setSandstormHandler.apply(handlerState, handlerContext, provider, randomSources);
		applySetSandstorm(freeFunctionState, freeFunctionContext, provider, randomSources);

		WHEN("sandstorm weather is applied")
		{
			CHECK((freeFunctionState.mWeatherIDs == handlerState.mWeatherIDs));
			CHECK(std::ranges::contains(freeFunctionState.mWeatherIDs, sandstormID));
		}
	}

	GIVEN("sandstorm weather is already active")
	{
		handlerState.mWeatherIDs = {sandstormID};
		freeFunctionState.mWeatherIDs = {sandstormID};

		setSandstormHandler.apply(handlerState, handlerContext, provider, randomSources);
		applySetSandstorm(freeFunctionState, freeFunctionContext, provider, randomSources);

		THEN("both leave sandstorm weather active exactly once")
		{
			CHECK((freeFunctionState.mWeatherIDs == handlerState.mWeatherIDs));
			CHECK((std::ranges::count(freeFunctionState.mWeatherIDs, sandstormID) == 1));
		}
	}

	WHEN("a different weather is active and replacement is allowed")
	{
		handlerState.mWeatherIDs = {hailID};
		handlerState.mRuleset.mMaxWeathers = 1;
		handlerState.mRuleset.mReplaceWeatherWhenFull = true;
		freeFunctionState.mWeatherIDs = {hailID};
		freeFunctionState.mRuleset.mMaxWeathers = 1;
		freeFunctionState.mRuleset.mReplaceWeatherWhenFull = true;

		setSandstormHandler.apply(handlerState, handlerContext, provider, randomSources);
		applySetSandstorm(freeFunctionState, freeFunctionContext, provider, randomSources);

		THEN("both replace the existing weather identically")
		{
			CHECK((freeFunctionState.mWeatherIDs == handlerState.mWeatherIDs));
			CHECK(std::ranges::contains(freeFunctionState.mWeatherIDs, sandstormID));
			CHECK_FALSE(std::ranges::contains(freeFunctionState.mWeatherIDs, hailID));
		}
	}

	WHEN("the ruleset permits no active weathers")
	{
		handlerState.mRuleset.mMaxWeathers = 0;
		freeFunctionState.mRuleset.mMaxWeathers = 0;

		setSandstormHandler.apply(handlerState, handlerContext, provider, randomSources);
		applySetSandstorm(freeFunctionState, freeFunctionContext, provider, randomSources);

		THEN("neither activates a weather")
		{
			CHECK((freeFunctionState.mWeatherIDs == handlerState.mWeatherIDs));
			CHECK_FALSE(std::ranges::contains(freeFunctionState.mWeatherIDs, sandstormID));
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
