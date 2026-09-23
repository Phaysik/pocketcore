/*! @file setHarshSunlightHandler.test.cpp
	@brief C++ file for running tests for the SetHarshSunlightHandler.
	@date 09/23/2026
	@since 0.12.43
	@version 0.12.43
	@author Matthew Moore
*/

#include "EffectHandler/setHarshSunlightHandler.h"

#include <algorithm>

#include "Battle/battleState.h"
#include "Effect/effectContext.h"
#include "Registry/registryProvider.h"
#include "Registry/registryProvider.testHelper.h"
#include "Weather/builtInWeatherID.h"
#include "Weather/weatherID.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Battle::BattleState;
using PocketCore::Effect::EffectContext;
using PocketCore::Effect::SetHarshSunlightHandler;
using PocketCore::Registry::RegistryProvider;
using PocketCore::Testing::getDefaultInitializedRegistryProvider;
using PocketCore::Weather::BuiltinWeatherID;
using PocketCore::Weather::toWeatherID;
using PocketCore::Weather::WeatherID;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

SCENARIO("SetHarshSunlightHandler")
{
	SetHarshSunlightHandler setHarshSunlightHandler{};
	BattleState battleState{};
	EffectContext context{};
	RegistryProvider provider{getDefaultInitializedRegistryProvider()};

	WeatherID sunID{toWeatherID(BuiltinWeatherID::HarshSunlight)};
	WeatherID hailID{toWeatherID(BuiltinWeatherID::Hail)};

	GIVEN("no active weathers")
	{

		WHEN("sun weather is applied")
		{
			setHarshSunlightHandler.apply(battleState, context, provider);

			THEN("sun weather becomes active")
			{
				CHECK(std::ranges::contains(battleState.mWeatherIDs, sunID));
			}

			THEN("sun weather is only added once")
			{
				CHECK((std::ranges::count(battleState.mWeatherIDs, sunID) == 1));
			}
		}
	}

	GIVEN("sun weather is already active")
	{
		battleState.mWeatherIDs = {sunID};

		WHEN("sun weather is applied again")
		{
			setHarshSunlightHandler.apply(battleState, context, provider);

			THEN("sun weather remains active exactly once")
			{
				CHECK((std::ranges::count(battleState.mWeatherIDs, sunID) == 1));
			}
		}
	}

	GIVEN("a different weather is active and replacement is allowed")
	{
		battleState.mWeatherIDs = {hailID};
		battleState.mRuleset.mMaxWeathers = 1;
		battleState.mRuleset.mReplaceWeatherWhenFull = true;

		WHEN("sun weather is applied")
		{
			setHarshSunlightHandler.apply(battleState, context, provider);

			THEN("sun weather replaces the existing weather")
			{
				CHECK(std::ranges::contains(battleState.mWeatherIDs, sunID));
				CHECK_FALSE(std::ranges::contains(battleState.mWeatherIDs, hailID));
			}
		}
	}

	GIVEN("a different weather is active and replacement is not allowed")
	{
		battleState.mWeatherIDs = {hailID};
		battleState.mRuleset.mMaxWeathers = 1;
		battleState.mRuleset.mReplaceWeatherWhenFull = false;

		WHEN("sun weather is applied")
		{
			setHarshSunlightHandler.apply(battleState, context, provider);

			THEN("the existing weather is retained and sun weather is not added")
			{
				CHECK(std::ranges::contains(battleState.mWeatherIDs, hailID));
				CHECK_FALSE(std::ranges::contains(battleState.mWeatherIDs, sunID));
			}
		}
	}

	GIVEN("a free slot remains below the weather cap")
	{
		battleState.mWeatherIDs = {hailID};
		battleState.mRuleset.mMaxWeathers = 2;
		battleState.mRuleset.mReplaceWeatherWhenFull = true;

		WHEN("sun weather is applied")
		{
			setHarshSunlightHandler.apply(battleState, context, provider);

			THEN("sun weather is added alongside the existing weather")
			{
				CHECK(std::ranges::contains(battleState.mWeatherIDs, hailID));
				CHECK(std::ranges::contains(battleState.mWeatherIDs, sunID));
			}
		}
	}

	GIVEN("the ruleset permits no active weathers")
	{
		battleState.mRuleset.mMaxWeathers = 0;

		WHEN("sun weather is applied")
		{
			setHarshSunlightHandler.apply(battleState, context, provider);

			THEN("no weather becomes active")
			{
				CHECK_FALSE(std::ranges::contains(battleState.mWeatherIDs, sunID));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
