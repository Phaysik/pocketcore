/*! @file setSandstormHandler.test.cpp
	@brief C++ file for running tests for the SetSandstormHandler.
	@date 09/23/2026
	@since 0.12.43
	@version 0.12.43
	@author Matthew Moore
*/

#include "EffectHandler/setSandstormHandler.h"

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
	BattleState battleState{};
	EffectContext context{};
	RegistryProvider provider{getDefaultInitializedRegistryProvider()};

	WeatherID sandstormID{toWeatherID(BuiltinWeatherID::Sandstorm)};
	WeatherID hailID{toWeatherID(BuiltinWeatherID::Hail)};

	GIVEN("no active weathers")
	{

		WHEN("sandstorm weather is applied")
		{
			setSandstormHandler.apply(battleState, context, provider);

			THEN("sandstorm weather becomes active")
			{
				CHECK(std::ranges::contains(battleState.mWeatherIDs, sandstormID));
			}

			THEN("sandstorm weather is only added once")
			{
				CHECK((std::ranges::count(battleState.mWeatherIDs, sandstormID) == 1));
			}
		}
	}

	GIVEN("sandstorm weather is already active")
	{
		battleState.mWeatherIDs = {sandstormID};

		WHEN("sandstorm weather is applied again")
		{
			setSandstormHandler.apply(battleState, context, provider);

			THEN("sandstorm weather remains active exactly once")
			{
				CHECK((std::ranges::count(battleState.mWeatherIDs, sandstormID) == 1));
			}
		}
	}

	GIVEN("a different weather is active and replacement is allowed")
	{
		battleState.mWeatherIDs = {hailID};
		battleState.mRuleset.mMaxWeathers = 1;
		battleState.mRuleset.mReplaceWeatherWhenFull = true;

		WHEN("sandstorm weather is applied")
		{
			setSandstormHandler.apply(battleState, context, provider);

			THEN("sandstorm weather replaces the existing weather")
			{
				CHECK(std::ranges::contains(battleState.mWeatherIDs, sandstormID));
				CHECK_FALSE(std::ranges::contains(battleState.mWeatherIDs, hailID));
			}
		}
	}

	GIVEN("a different weather is active and replacement is not allowed")
	{
		battleState.mWeatherIDs = {hailID};
		battleState.mRuleset.mMaxWeathers = 1;
		battleState.mRuleset.mReplaceWeatherWhenFull = false;

		WHEN("sandstorm weather is applied")
		{
			setSandstormHandler.apply(battleState, context, provider);

			THEN("the existing weather is retained and sandstorm weather is not added")
			{
				CHECK(std::ranges::contains(battleState.mWeatherIDs, hailID));
				CHECK_FALSE(std::ranges::contains(battleState.mWeatherIDs, sandstormID));
			}
		}
	}

	GIVEN("a free slot remains below the weather cap")
	{
		battleState.mWeatherIDs = {hailID};
		battleState.mRuleset.mMaxWeathers = 2;
		battleState.mRuleset.mReplaceWeatherWhenFull = true;

		WHEN("sandstorm weather is applied")
		{
			setSandstormHandler.apply(battleState, context, provider);

			THEN("sandstorm weather is added alongside the existing weather")
			{
				CHECK(std::ranges::contains(battleState.mWeatherIDs, hailID));
				CHECK(std::ranges::contains(battleState.mWeatherIDs, sandstormID));
			}
		}
	}

	GIVEN("the ruleset permits no active weathers")
	{
		battleState.mRuleset.mMaxWeathers = 0;

		WHEN("sandstorm weather is applied")
		{
			setSandstormHandler.apply(battleState, context, provider);

			THEN("no weather becomes active")
			{
				CHECK_FALSE(std::ranges::contains(battleState.mWeatherIDs, sandstormID));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
