/*! @file weatherMeta.test.cpp
	@brief C++ file for running tests for the WeatherMeta.
	@date 09/23/2026
	@since 0.12.43
	@version 0.12.43
	@author Matthew Moore
*/

#include "Weather/weatherMeta.h"

#include "Interaction/interaction.h"
#include "Weather/builtInWeatherID.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Interaction::InteractionAction;
using PocketCore::Weather::BuiltinWeatherID;
using PocketCore::Weather::toWeatherID;
using PocketCore::Weather::WeatherMeta;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("WeatherMeta")
{
	WeatherMeta meta{};

	WHEN("operator==")
	{
		GIVEN("two default constructed metas")
		{
			WeatherMeta other{};

			THEN("they are equal")
			{
				CHECK((meta == other));
			}
		}

		GIVEN("for mName modified in one meta")
		{
			WeatherMeta other{.mName = "Test"};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mWeatherInteractions modified in one meta")
		{
			WeatherMeta other{
				.mWeatherInteractions = {{.mExistingID = toWeatherID(BuiltinWeatherID::Hail), .mAction = InteractionAction::BlockIncoming}},
			};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mWeatherID modified in one meta")
		{
			WeatherMeta other{.mWeatherID = toWeatherID(BuiltinWeatherID::Hail)};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("two metas modified the same way")
		{
			WeatherMeta other{.mName = "Same Name"};
			meta.mName = "Same Name";

			THEN("they are equal")
			{
				CHECK((meta == other));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
