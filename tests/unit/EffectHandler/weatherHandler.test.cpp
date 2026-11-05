/*! @file weatherHandler.test.cpp
	@brief C++ file for running tests for the WeatherHandler.
	@date 10/28/2026
	@since 0.12.53
	@version 0.12.53
	@author Matthew Moore
*/

#include "EffectHandler/weatherHandler.h"

#include "Ability/builtInAbilityID.h"
#include "Battle/battleRandomSources.h"
#include "Battle/battleState.h"
#include "Battle/battleState.testHelper.h"
#include "Configuration/constants.h"
#include "Effect/effectContext.h"
#include "Effect/effectContext.testHelper.h"
#include "EffectHandler/weatherHandler.testHelper.h"
#include "Move/builtInMoveID.h"
#include "Pokemon/pokemon.h"
#include "Pokemon/pokemon.testHelper.h"
#include "Registry/registryProvider.h"
#include "Registry/registryProvider.testHelper.h"
#include "Type/builtInTypeID.h"
#include "Weather/builtInWeatherID.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Ability::BuiltinAbilityID;
using PocketCore::Ability::toAbilityID;
using PocketCore::Battle::BattleRandomSources;
using PocketCore::Battle::BattleState;
using PocketCore::Configuration::FIRE_MOVE_IN_EXTREMELY_HARSH_SUNLIGHT_MULTIPLIER_VALUE;
using PocketCore::Configuration::FIRE_MOVE_IN_HARSH_SUNLIGHT_MULTIPLIER_VALUE;
using PocketCore::Configuration::FIRE_MOVE_IN_HEAVY_RAIN_MULTIPLIER_VALUE;
using PocketCore::Configuration::FIRE_MOVE_IN_RAIN_MULTIPLIER_VALUE;
using PocketCore::Configuration::HYDRO_STEAM_MOVE_IN_HARSH_SUNLIGHT_MULTIPLIER_VALUE;
using PocketCore::Configuration::WATER_MOVE_IN_EXTREMELY_HARSH_SUNLIGHT_MULTIPLIER_VALUE;
using PocketCore::Configuration::WATER_MOVE_IN_HARSH_SUNLIGHT_MULTIPLIER_VALUE;
using PocketCore::Configuration::WATER_MOVE_IN_HEAVY_RAIN_MULTIPLIER_VALUE;
using PocketCore::Configuration::WATER_MOVE_IN_RAIN_MULTIPLIER_VALUE;
using PocketCore::Configuration::WEATHER_NULLIFICATION_MULTIPLIER_VALUE;
using PocketCore::Effect::applyWeather;
using PocketCore::Effect::EffectContext;
using PocketCore::Effect::WeatherHandler;
using PocketCore::Move::BuiltinMoveID;
using PocketCore::Move::toMoveID;
using PocketCore::Pokemon::Pokemon;
using PocketCore::Registry::RegistryProvider;
using PocketCore::Testing::getDefaultInitializedRegistryProvider;
using PocketCore::Testing::hasWeatherMultiplier;
using PocketCore::Testing::makeBattleState;
using PocketCore::Testing::makeEffectContext;
using PocketCore::Testing::makePokemon;
using PocketCore::Type::BuiltinTypeID;
using PocketCore::Type::toTypeID;
using PocketCore::Weather::BuiltinWeatherID;
using PocketCore::Weather::toWeatherID;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

SCENARIO("WeatherHandler")
{
	WeatherHandler weatherHandler{};
	Pokemon partyA1{makePokemon({.mAbilityEntries = {{{.mAbilityID = toAbilityID(BuiltinAbilityID::BattleArmor)}}}})};
	Pokemon partyA2{makePokemon({})};
	Pokemon partyB1{makePokemon({.mAbilityEntries = {{{.mAbilityID = toAbilityID(BuiltinAbilityID::Guts)}}}})};
	Pokemon partyB2{makePokemon({})};
	BattleState handlerState{makeBattleState({.mPartyA = {&partyA1, &partyA2}, .mPartyB = {&partyB1, &partyB2}})};
	BattleState freeFunctionState{makeBattleState({.mPartyA = {&partyA1, &partyA2}, .mPartyB = {&partyB1, &partyB2}})};
	RegistryProvider provider{getDefaultInitializedRegistryProvider()};
	BattleRandomSources randomSources{};
	EffectContext handlerContext{makeEffectContext({})};
	EffectContext freeFunctionContext{makeEffectContext({})};

	GIVEN("partyA has weather a weather nullifying ability")
	{
		WHEN("calling with Air Lock")
		{
			partyA2.setAbilityEntry(0, {.mAbilityID = toAbilityID(BuiltinAbilityID::AirLock)});

			handlerState.mPartyA = {nullptr, &partyA2};
			freeFunctionState.mPartyA = {nullptr, &partyA2};

			weatherHandler.apply(handlerState, handlerContext, provider, randomSources);
			applyWeather(freeFunctionState, freeFunctionContext, provider, randomSources);

			THEN("the weather multiplier is set to 1.0")
			{
				CHECK(hasWeatherMultiplier(handlerContext, WEATHER_NULLIFICATION_MULTIPLIER_VALUE));
				CHECK(hasWeatherMultiplier(freeFunctionContext, WEATHER_NULLIFICATION_MULTIPLIER_VALUE));
			}
		}

		WHEN("calling with Cloud Nine")
		{
			partyA1.setAbilityEntry(0, {.mAbilityID = toAbilityID(BuiltinAbilityID::CloudNine)});

			weatherHandler.apply(handlerState, handlerContext, provider, randomSources);
			applyWeather(freeFunctionState, freeFunctionContext, provider, randomSources);

			THEN("the weather multiplier is set to 1.0")
			{
				CHECK(hasWeatherMultiplier(handlerContext, WEATHER_NULLIFICATION_MULTIPLIER_VALUE));
				CHECK(hasWeatherMultiplier(freeFunctionContext, WEATHER_NULLIFICATION_MULTIPLIER_VALUE));
			}
		}
	}

	GIVEN("partyB has weather a weather nullifying ability")
	{
		WHEN("calling with Air Lock")
		{
			partyB2.setAbilityEntry(0, {.mAbilityID = toAbilityID(BuiltinAbilityID::AirLock)});

			handlerState.mPartyB = {nullptr, &partyB2};
			freeFunctionState.mPartyB = {nullptr, &partyB2};

			weatherHandler.apply(handlerState, handlerContext, provider, randomSources);
			applyWeather(freeFunctionState, freeFunctionContext, provider, randomSources);

			THEN("the weather multiplier is set to 1.0")
			{
				CHECK(hasWeatherMultiplier(handlerContext, WEATHER_NULLIFICATION_MULTIPLIER_VALUE));
				CHECK(hasWeatherMultiplier(freeFunctionContext, WEATHER_NULLIFICATION_MULTIPLIER_VALUE));
			}
		}

		WHEN("calling with Cloud Nine")
		{
			partyB1.setAbilityEntry(0, {.mAbilityID = toAbilityID(BuiltinAbilityID::CloudNine)});

			weatherHandler.apply(handlerState, handlerContext, provider, randomSources);
			applyWeather(freeFunctionState, freeFunctionContext, provider, randomSources);

			THEN("the weather multiplier is set to 1.0")
			{
				CHECK(hasWeatherMultiplier(handlerContext, WEATHER_NULLIFICATION_MULTIPLIER_VALUE));
				CHECK(hasWeatherMultiplier(freeFunctionContext, WEATHER_NULLIFICATION_MULTIPLIER_VALUE));
			}
		}
	}

	GIVEN("the weather is rain")
	{
		handlerState.mWeatherIDs = {toWeatherID(BuiltinWeatherID::Rain)};
		freeFunctionState.mWeatherIDs = {toWeatherID(BuiltinWeatherID::Rain)};

		WHEN("calling with a water type move")
		{
			handlerContext.mMoveTypeID = toTypeID(BuiltinTypeID::Water);
			freeFunctionContext.mMoveTypeID = toTypeID(BuiltinTypeID::Water);

			weatherHandler.apply(handlerState, handlerContext, provider, randomSources);
			applyWeather(freeFunctionState, freeFunctionContext, provider, randomSources);

			THEN("the weather multiplier is set to 1.5")
			{
				CHECK(hasWeatherMultiplier(handlerContext, WATER_MOVE_IN_RAIN_MULTIPLIER_VALUE));
				CHECK(hasWeatherMultiplier(freeFunctionContext, WATER_MOVE_IN_RAIN_MULTIPLIER_VALUE));
			}
		}

		WHEN("calling with a fire type move")
		{
			handlerContext.mMoveTypeID = toTypeID(BuiltinTypeID::Fire);
			freeFunctionContext.mMoveTypeID = toTypeID(BuiltinTypeID::Fire);

			weatherHandler.apply(handlerState, handlerContext, provider, randomSources);
			applyWeather(freeFunctionState, freeFunctionContext, provider, randomSources);

			THEN("the weather multiplier is set to 0.5")
			{
				CHECK(hasWeatherMultiplier(handlerContext, FIRE_MOVE_IN_RAIN_MULTIPLIER_VALUE));
				CHECK(hasWeatherMultiplier(freeFunctionContext, FIRE_MOVE_IN_RAIN_MULTIPLIER_VALUE));
			}
		}

		WHEN("calling with a non water or fire type move")
		{
			handlerContext.mMoveTypeID = toTypeID(BuiltinTypeID::Normal);
			freeFunctionContext.mMoveTypeID = toTypeID(BuiltinTypeID::Normal);

			weatherHandler.apply(handlerState, handlerContext, provider, randomSources);
			applyWeather(freeFunctionState, freeFunctionContext, provider, randomSources);

			THEN("the weather multiplier is set to 1.0")
			{
				CHECK(hasWeatherMultiplier(handlerContext, WEATHER_NULLIFICATION_MULTIPLIER_VALUE));
				CHECK(hasWeatherMultiplier(freeFunctionContext, WEATHER_NULLIFICATION_MULTIPLIER_VALUE));
			}
		}
	}

	GIVEN("the weather is heavy rain")
	{
		handlerState.mWeatherIDs = {toWeatherID(BuiltinWeatherID::HeavyRain)};
		freeFunctionState.mWeatherIDs = {toWeatherID(BuiltinWeatherID::HeavyRain)};

		WHEN("calling with a water type move")
		{
			handlerContext.mMoveTypeID = toTypeID(BuiltinTypeID::Water);
			freeFunctionContext.mMoveTypeID = toTypeID(BuiltinTypeID::Water);

			weatherHandler.apply(handlerState, handlerContext, provider, randomSources);
			applyWeather(freeFunctionState, freeFunctionContext, provider, randomSources);

			THEN("the weather multiplier is set to 1.5")
			{
				CHECK(hasWeatherMultiplier(handlerContext, WATER_MOVE_IN_HEAVY_RAIN_MULTIPLIER_VALUE));
				CHECK(hasWeatherMultiplier(freeFunctionContext, WATER_MOVE_IN_HEAVY_RAIN_MULTIPLIER_VALUE));
			}
		}

		WHEN("calling with a fire type move")
		{
			handlerContext.mMoveTypeID = toTypeID(BuiltinTypeID::Fire);
			freeFunctionContext.mMoveTypeID = toTypeID(BuiltinTypeID::Fire);

			weatherHandler.apply(handlerState, handlerContext, provider, randomSources);
			applyWeather(freeFunctionState, freeFunctionContext, provider, randomSources);

			THEN("the weather multiplier is set to 0.0")
			{
				CHECK(hasWeatherMultiplier(handlerContext, FIRE_MOVE_IN_HEAVY_RAIN_MULTIPLIER_VALUE));
				CHECK(hasWeatherMultiplier(freeFunctionContext, FIRE_MOVE_IN_HEAVY_RAIN_MULTIPLIER_VALUE));
			}
		}

		WHEN("calling with a non water or fire type move")
		{
			handlerContext.mMoveTypeID = toTypeID(BuiltinTypeID::Normal);
			freeFunctionContext.mMoveTypeID = toTypeID(BuiltinTypeID::Normal);

			weatherHandler.apply(handlerState, handlerContext, provider, randomSources);
			applyWeather(freeFunctionState, freeFunctionContext, provider, randomSources);

			THEN("the weather multiplier is set to 1.0")
			{
				CHECK(hasWeatherMultiplier(handlerContext, WEATHER_NULLIFICATION_MULTIPLIER_VALUE));
				CHECK(hasWeatherMultiplier(freeFunctionContext, WEATHER_NULLIFICATION_MULTIPLIER_VALUE));
			}
		}
	}

	GIVEN("the weather is harsh sunlight")
	{
		handlerState.mWeatherIDs = {toWeatherID(BuiltinWeatherID::HarshSunlight)};
		freeFunctionState.mWeatherIDs = {toWeatherID(BuiltinWeatherID::HarshSunlight)};

		WHEN("calling with a water type move")
		{
			handlerContext.mMoveTypeID = toTypeID(BuiltinTypeID::Water);
			freeFunctionContext.mMoveTypeID = toTypeID(BuiltinTypeID::Water);

			weatherHandler.apply(handlerState, handlerContext, provider, randomSources);
			applyWeather(freeFunctionState, freeFunctionContext, provider, randomSources);

			THEN("the weather multiplier is set to 0.5")
			{
				CHECK(hasWeatherMultiplier(handlerContext, WATER_MOVE_IN_HARSH_SUNLIGHT_MULTIPLIER_VALUE));
				CHECK(hasWeatherMultiplier(freeFunctionContext, WATER_MOVE_IN_HARSH_SUNLIGHT_MULTIPLIER_VALUE));
			}
		}

		WHEN("calling with a specifically Hydro Steam")
		{
			handlerContext.mMoveTypeID = toTypeID(BuiltinTypeID::Water);
			freeFunctionContext.mMoveTypeID = toTypeID(BuiltinTypeID::Water);
			handlerContext.mMoveID = toMoveID(BuiltinMoveID::HydroSteam);
			freeFunctionContext.mMoveID = toMoveID(BuiltinMoveID::HydroSteam);

			weatherHandler.apply(handlerState, handlerContext, provider, randomSources);
			applyWeather(freeFunctionState, freeFunctionContext, provider, randomSources);

			THEN("the weather multiplier is set to 1.5")
			{
				CHECK(hasWeatherMultiplier(handlerContext, HYDRO_STEAM_MOVE_IN_HARSH_SUNLIGHT_MULTIPLIER_VALUE));
				CHECK(hasWeatherMultiplier(freeFunctionContext, HYDRO_STEAM_MOVE_IN_HARSH_SUNLIGHT_MULTIPLIER_VALUE));
			}
		}

		WHEN("calling with a fire type move")
		{
			handlerContext.mMoveTypeID = toTypeID(BuiltinTypeID::Fire);
			freeFunctionContext.mMoveTypeID = toTypeID(BuiltinTypeID::Fire);

			weatherHandler.apply(handlerState, handlerContext, provider, randomSources);
			applyWeather(freeFunctionState, freeFunctionContext, provider, randomSources);

			THEN("the weather multiplier is set to 1.5")
			{
				CHECK(hasWeatherMultiplier(handlerContext, FIRE_MOVE_IN_HARSH_SUNLIGHT_MULTIPLIER_VALUE));
				CHECK(hasWeatherMultiplier(freeFunctionContext, FIRE_MOVE_IN_HARSH_SUNLIGHT_MULTIPLIER_VALUE));
			}
		}

		WHEN("calling with a non water or fire type move")
		{
			handlerContext.mMoveTypeID = toTypeID(BuiltinTypeID::Normal);
			freeFunctionContext.mMoveTypeID = toTypeID(BuiltinTypeID::Normal);

			weatherHandler.apply(handlerState, handlerContext, provider, randomSources);
			applyWeather(freeFunctionState, freeFunctionContext, provider, randomSources);

			THEN("the weather multiplier is set to 1.0")
			{
				CHECK(hasWeatherMultiplier(handlerContext, WEATHER_NULLIFICATION_MULTIPLIER_VALUE));
				CHECK(hasWeatherMultiplier(freeFunctionContext, WEATHER_NULLIFICATION_MULTIPLIER_VALUE));
			}
		}
	}

	GIVEN("the weather is extremely harsh sunlight")
	{
		handlerState.mWeatherIDs = {toWeatherID(BuiltinWeatherID::ExtremelyHarshSunlight)};
		freeFunctionState.mWeatherIDs = {toWeatherID(BuiltinWeatherID::ExtremelyHarshSunlight)};

		WHEN("calling with a water type move")
		{
			handlerContext.mMoveTypeID = toTypeID(BuiltinTypeID::Water);
			freeFunctionContext.mMoveTypeID = toTypeID(BuiltinTypeID::Water);

			weatherHandler.apply(handlerState, handlerContext, provider, randomSources);
			applyWeather(freeFunctionState, freeFunctionContext, provider, randomSources);

			THEN("the weather multiplier is set to 0.0")
			{
				CHECK(hasWeatherMultiplier(handlerContext, WATER_MOVE_IN_EXTREMELY_HARSH_SUNLIGHT_MULTIPLIER_VALUE));
				CHECK(hasWeatherMultiplier(freeFunctionContext, WATER_MOVE_IN_EXTREMELY_HARSH_SUNLIGHT_MULTIPLIER_VALUE));
			}
		}

		WHEN("calling with a specifically Hydro Steam")
		{
			handlerContext.mMoveTypeID = toTypeID(BuiltinTypeID::Water);
			freeFunctionContext.mMoveTypeID = toTypeID(BuiltinTypeID::Water);
			handlerContext.mMoveID = toMoveID(BuiltinMoveID::HydroSteam);
			freeFunctionContext.mMoveID = toMoveID(BuiltinMoveID::HydroSteam);

			weatherHandler.apply(handlerState, handlerContext, provider, randomSources);
			applyWeather(freeFunctionState, freeFunctionContext, provider, randomSources);

			THEN("the weather multiplier is set to 0.0")
			{
				CHECK(hasWeatherMultiplier(handlerContext, WATER_MOVE_IN_EXTREMELY_HARSH_SUNLIGHT_MULTIPLIER_VALUE));
				CHECK(hasWeatherMultiplier(freeFunctionContext, WATER_MOVE_IN_EXTREMELY_HARSH_SUNLIGHT_MULTIPLIER_VALUE));
			}
		}

		WHEN("calling with a fire type move")
		{
			handlerContext.mMoveTypeID = toTypeID(BuiltinTypeID::Fire);
			freeFunctionContext.mMoveTypeID = toTypeID(BuiltinTypeID::Fire);

			weatherHandler.apply(handlerState, handlerContext, provider, randomSources);
			applyWeather(freeFunctionState, freeFunctionContext, provider, randomSources);

			THEN("the weather multiplier is set to 1.5")
			{
				CHECK(hasWeatherMultiplier(handlerContext, FIRE_MOVE_IN_EXTREMELY_HARSH_SUNLIGHT_MULTIPLIER_VALUE));
				CHECK(hasWeatherMultiplier(freeFunctionContext, FIRE_MOVE_IN_EXTREMELY_HARSH_SUNLIGHT_MULTIPLIER_VALUE));
			}
		}

		WHEN("calling with a non water or fire type move")
		{
			handlerContext.mMoveTypeID = toTypeID(BuiltinTypeID::Normal);
			freeFunctionContext.mMoveTypeID = toTypeID(BuiltinTypeID::Normal);

			weatherHandler.apply(handlerState, handlerContext, provider, randomSources);
			applyWeather(freeFunctionState, freeFunctionContext, provider, randomSources);

			THEN("the weather multiplier is set to 1.0")
			{
				CHECK(hasWeatherMultiplier(handlerContext, WEATHER_NULLIFICATION_MULTIPLIER_VALUE));
				CHECK(hasWeatherMultiplier(freeFunctionContext, WEATHER_NULLIFICATION_MULTIPLIER_VALUE));
			}
		}
	}

	GIVEN("there is no weather")
	{
		handlerState.mWeatherIDs = {};
		freeFunctionState.mWeatherIDs = {};

		WHEN("calling with a water type move")
		{
			handlerContext.mMoveTypeID = toTypeID(BuiltinTypeID::Water);
			freeFunctionContext.mMoveTypeID = toTypeID(BuiltinTypeID::Water);

			weatherHandler.apply(handlerState, handlerContext, provider, randomSources);
			applyWeather(freeFunctionState, freeFunctionContext, provider, randomSources);

			THEN("the weather multiplier is set to 1.0")
			{
				CHECK(hasWeatherMultiplier(handlerContext, WEATHER_NULLIFICATION_MULTIPLIER_VALUE));
				CHECK(hasWeatherMultiplier(freeFunctionContext, WEATHER_NULLIFICATION_MULTIPLIER_VALUE));
			}
		}

		WHEN("calling with a specifically Hydro Steam")
		{
			handlerContext.mMoveTypeID = toTypeID(BuiltinTypeID::Water);
			freeFunctionContext.mMoveTypeID = toTypeID(BuiltinTypeID::Water);
			handlerContext.mMoveID = toMoveID(BuiltinMoveID::HydroSteam);
			freeFunctionContext.mMoveID = toMoveID(BuiltinMoveID::HydroSteam);

			weatherHandler.apply(handlerState, handlerContext, provider, randomSources);
			applyWeather(freeFunctionState, freeFunctionContext, provider, randomSources);

			THEN("the weather multiplier is set to 1.0")
			{
				CHECK(hasWeatherMultiplier(handlerContext, WEATHER_NULLIFICATION_MULTIPLIER_VALUE));
				CHECK(hasWeatherMultiplier(freeFunctionContext, WEATHER_NULLIFICATION_MULTIPLIER_VALUE));
			}
		}

		WHEN("calling with a fire type move")
		{
			handlerContext.mMoveTypeID = toTypeID(BuiltinTypeID::Fire);
			freeFunctionContext.mMoveTypeID = toTypeID(BuiltinTypeID::Fire);

			weatherHandler.apply(handlerState, handlerContext, provider, randomSources);
			applyWeather(freeFunctionState, freeFunctionContext, provider, randomSources);

			THEN("the weather multiplier is set to 1.0")
			{
				CHECK(hasWeatherMultiplier(handlerContext, WEATHER_NULLIFICATION_MULTIPLIER_VALUE));
				CHECK(hasWeatherMultiplier(freeFunctionContext, WEATHER_NULLIFICATION_MULTIPLIER_VALUE));
			}
		}

		WHEN("calling with a non water or fire type move")
		{
			handlerContext.mMoveTypeID = toTypeID(BuiltinTypeID::Normal);
			freeFunctionContext.mMoveTypeID = toTypeID(BuiltinTypeID::Normal);

			weatherHandler.apply(handlerState, handlerContext, provider, randomSources);
			applyWeather(freeFunctionState, freeFunctionContext, provider, randomSources);

			THEN("the weather multiplier is set to 1.0")
			{
				CHECK(hasWeatherMultiplier(handlerContext, WEATHER_NULLIFICATION_MULTIPLIER_VALUE));
				CHECK(hasWeatherMultiplier(freeFunctionContext, WEATHER_NULLIFICATION_MULTIPLIER_VALUE));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
