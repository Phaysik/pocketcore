/*! @file setRainHandler.test.cpp
	@brief C++ file for running tests for the SetRainHandler.
	@date 09/23/2026
	@since 0.12.43
	@version 0.12.43
	@author Matthew Moore
*/

#include "EffectHandler/setRainHandler.h"

#include <algorithm>

#include "Battle/battleState.h"
#include "Effect/effectContext.h"
#include "Registry/registryProvider.h"
#include "Registry/registryProvider.testHelper.h"
#include "Weather/builtInWeatherID.h"
#include "Weather/weatherID.h"

#include <catch2/catch_test_macros.hpp>

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
	BattleState battleState{};
	EffectContext context{};
	RegistryProvider provider{getDefaultInitializedRegistryProvider()};

	WeatherID rainID{toWeatherID(BuiltinWeatherID::Rain)};
	WeatherID hailID{toWeatherID(BuiltinWeatherID::Hail)};

	GIVEN("no active weathers")
	{

		WHEN("rain weather is applied")
		{
			setRainHandler.apply(battleState, context, provider);

			THEN("rain weather becomes active")
			{
				CHECK(std::ranges::contains(battleState.mWeatherIDs, rainID));
			}

			THEN("rain weather is only added once")
			{
				CHECK((std::ranges::count(battleState.mWeatherIDs, rainID) == 1));
			}
		}
	}

	GIVEN("rain weather is already active")
	{
		battleState.mWeatherIDs = {rainID};

		WHEN("rain weather is applied again")
		{
			setRainHandler.apply(battleState, context, provider);

			THEN("rain weather remains active exactly once")
			{
				CHECK((std::ranges::count(battleState.mWeatherIDs, rainID) == 1));
			}
		}
	}

	GIVEN("a different weather is active and replacement is allowed")
	{
		battleState.mWeatherIDs = {hailID};
		battleState.mRuleset.mMaxWeathers = 1;
		battleState.mRuleset.mReplaceWeatherWhenFull = true;

		WHEN("rain weather is applied")
		{
			setRainHandler.apply(battleState, context, provider);

			THEN("rain weather replaces the existing weather")
			{
				CHECK(std::ranges::contains(battleState.mWeatherIDs, rainID));
				CHECK_FALSE(std::ranges::contains(battleState.mWeatherIDs, hailID));
			}
		}
	}

	GIVEN("a different weather is active and replacement is not allowed")
	{
		battleState.mWeatherIDs = {hailID};
		battleState.mRuleset.mMaxWeathers = 1;
		battleState.mRuleset.mReplaceWeatherWhenFull = false;

		WHEN("rain weather is applied")
		{
			setRainHandler.apply(battleState, context, provider);

			THEN("the existing weather is retained and rain weather is not added")
			{
				CHECK(std::ranges::contains(battleState.mWeatherIDs, hailID));
				CHECK_FALSE(std::ranges::contains(battleState.mWeatherIDs, rainID));
			}
		}
	}

	GIVEN("a free slot remains below the weather cap")
	{
		battleState.mWeatherIDs = {hailID};
		battleState.mRuleset.mMaxWeathers = 2;
		battleState.mRuleset.mReplaceWeatherWhenFull = true;

		WHEN("rain weather is applied")
		{
			setRainHandler.apply(battleState, context, provider);

			THEN("rain weather is added alongside the existing weather")
			{
				CHECK(std::ranges::contains(battleState.mWeatherIDs, hailID));
				CHECK(std::ranges::contains(battleState.mWeatherIDs, rainID));
			}
		}
	}

	GIVEN("the ruleset permits no active weathers")
	{
		battleState.mRuleset.mMaxWeathers = 0;

		WHEN("rain weather is applied")
		{
			setRainHandler.apply(battleState, context, provider);

			THEN("no weather becomes active")
			{
				CHECK_FALSE(std::ranges::contains(battleState.mWeatherIDs, rainID));
			}
		}
	}

	GIVEN("the free function and the handler operate on equivalent states")
	{
		BattleState handlerState{};
		BattleState freeFunctionState{};
		EffectContext handlerContext{};
		EffectContext freeFunctionContext{};

		WHEN("applied to empty active weathers")
		{
			setRainHandler.apply(handlerState, handlerContext, provider);
			applySetRain(freeFunctionState, freeFunctionContext, provider);

			THEN("the free function yields the same active weathers as the handler")
			{
				CHECK((freeFunctionState.mWeatherIDs == handlerState.mWeatherIDs));
				CHECK(std::ranges::contains(freeFunctionState.mWeatherIDs, rainID));
			}
		}

		WHEN("sun weather is already active")
		{
			handlerState.mWeatherIDs = {rainID};
			freeFunctionState.mWeatherIDs = {rainID};

			setRainHandler.apply(handlerState, handlerContext, provider);
			applySetRain(freeFunctionState, freeFunctionContext, provider);

			THEN("both leave sun weather active exactly once")
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

			setRainHandler.apply(handlerState, handlerContext, provider);
			applySetRain(freeFunctionState, freeFunctionContext, provider);

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

			setRainHandler.apply(handlerState, handlerContext, provider);
			applySetRain(freeFunctionState, freeFunctionContext, provider);

			THEN("neither activates a weather")
			{
				CHECK((freeFunctionState.mWeatherIDs == handlerState.mWeatherIDs));
				CHECK_FALSE(std::ranges::contains(freeFunctionState.mWeatherIDs, rainID));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
