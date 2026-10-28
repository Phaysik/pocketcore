/*! @file setRainHandler.test.cpp
	@brief C++ file for running tests for the SetRainHandler.
	@date 10/27/2026
	@since 0.12.43
	@version 0.12.53
	@author Matthew Moore
*/

#include "EffectHandler/setRainHandler.h"

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
using PocketCore::Effect::applySetRain;
using PocketCore::Effect::EffectContext;
using PocketCore::Effect::SetRainHandler;
using PocketCore::Registry::RegistryProvider;
using PocketCore::Testing::getDefaultInitializedRegistryProvider;
using PocketCore::Weather::BuiltinWeatherID;
using PocketCore::Weather::toWeatherID;
using PocketCore::Weather::WeatherID;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

SCENARIO("SetRainHandler")
{
	SetRainHandler setRainHandler{};
	RegistryProvider provider{getDefaultInitializedRegistryProvider()};
	BattleRandomSources randomSources{};

	WeatherID rainID{toWeatherID(BuiltinWeatherID::Rain)};
	WeatherID hailID{toWeatherID(BuiltinWeatherID::Hail)};

	BattleState handlerState{};
	BattleState freeFunctionState{};
	EffectContext handlerContext{};
	EffectContext freeFunctionContext{};

	WHEN("applied to empty active weathers")
	{
		setRainHandler.apply(handlerState, handlerContext, provider, randomSources);
		applySetRain(freeFunctionState, freeFunctionContext, provider, randomSources);

		WHEN("rain weather is applied")
		{
			CHECK((freeFunctionState.mWeatherIDs == handlerState.mWeatherIDs));
			CHECK(std::ranges::contains(freeFunctionState.mWeatherIDs, rainID));
		}
	}

	GIVEN("rain weather is already active")
	{
		handlerState.mWeatherIDs = {rainID};
		freeFunctionState.mWeatherIDs = {rainID};

		setRainHandler.apply(handlerState, handlerContext, provider, randomSources);
		applySetRain(freeFunctionState, freeFunctionContext, provider, randomSources);

		THEN("both leave rain weather active exactly once")
		{
			CHECK((freeFunctionState.mWeatherIDs == handlerState.mWeatherIDs));
			CHECK((std::ranges::count(freeFunctionState.mWeatherIDs, rainID) == 1));
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

		setRainHandler.apply(handlerState, handlerContext, provider, randomSources);
		applySetRain(freeFunctionState, freeFunctionContext, provider, randomSources);

		THEN("both replace the existing weather identically")
		{
			CHECK((freeFunctionState.mWeatherIDs == handlerState.mWeatherIDs));
			CHECK(std::ranges::contains(freeFunctionState.mWeatherIDs, rainID));
			CHECK_FALSE(std::ranges::contains(freeFunctionState.mWeatherIDs, hailID));
		}
	}

	WHEN("the ruleset permits no active weathers")
	{
		handlerState.mRuleset.mMaxWeathers = 0;
		freeFunctionState.mRuleset.mMaxWeathers = 0;

		setRainHandler.apply(handlerState, handlerContext, provider, randomSources);
		applySetRain(freeFunctionState, freeFunctionContext, provider, randomSources);

		THEN("neither activates a weather")
		{
			CHECK((freeFunctionState.mWeatherIDs == handlerState.mWeatherIDs));
			CHECK_FALSE(std::ranges::contains(freeFunctionState.mWeatherIDs, rainID));
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
