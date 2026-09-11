/*! @file weatherRegistry.test.cpp
	@brief C++ file for running tests for the WeatherRegistry.
	@date 09/10/2026
	@since 0.12.20
	@version 0.12.20
	@author Matthew Moore
*/

#include "Registry/weatherRegistry.h"

#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <utility>

#include "Core/typedefs.h"
#include "Weather/builtInWeatherID.h"
#include "Weather/constants.h"
#include "Weather/weatherID.h"
#include "Weather/weatherMeta.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Core::ub;
using PocketCore::Registry::Weather::WeatherRegistry;
using PocketCore::Weather::BuiltinWeatherID;
using PocketCore::Weather::NO_WEATHER_ID;
using PocketCore::Weather::toWeatherID;
using PocketCore::Weather::WEATHER_NAME_EXTREMELY_HARSH_SUNLIGHT;
using PocketCore::Weather::WEATHER_NAME_FOG;
using PocketCore::Weather::WEATHER_NAME_HAIL;
using PocketCore::Weather::WEATHER_NAME_HARSH_SUNLIGHT;
using PocketCore::Weather::WEATHER_NAME_HEAVY_RAIN;
using PocketCore::Weather::WEATHER_NAME_NONE;
using PocketCore::Weather::WEATHER_NAME_RAIN;
using PocketCore::Weather::WEATHER_NAME_SANDSTORM;
using PocketCore::Weather::WEATHER_NAME_SHADOWY_AURA;
using PocketCore::Weather::WEATHER_NAME_SNOW;
using PocketCore::Weather::WEATHER_NAME_STRONG_WINDS;
using PocketCore::Weather::WeatherID;
using PocketCore::Weather::WeatherMeta;

template <typename Registry>
concept PubliclyStructurallyMutable = requires(Registry &registry) { registry.setAmountRegistered(0); };

static_assert(!PubliclyStructurallyMutable<WeatherRegistry>);

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("WeatherRegistry")
{
	WeatherRegistry registry{};
	ub finalWeatherUnderlyingValue{std::to_underlying(BuiltinWeatherID::FinalWeather)};

	GIVEN("a default constructed weather registry")
	{
		THEN("Harsh Sunlight has the appropriate properties")
		{
			WeatherMeta expected{
				.mName = std::string(WEATHER_NAME_HARSH_SUNLIGHT),
				.mWeatherID = toWeatherID(BuiltinWeatherID::HarshSunlight),
			};

			const WeatherMeta *actual{registry.getWeatherMetadata(toWeatherID(BuiltinWeatherID::HarshSunlight))};

			CHECK((expected == *actual));
		}

		THEN("Rain has the appropriate properties")
		{
			WeatherMeta expected{
				.mName = std::string(WEATHER_NAME_RAIN),
				.mWeatherID = toWeatherID(BuiltinWeatherID::Rain),
			};

			const WeatherMeta *actual{registry.getWeatherMetadata(toWeatherID(BuiltinWeatherID::Rain))};

			CHECK((expected == *actual));
		}

		THEN("Sandstorm has the appropriate properties")
		{
			WeatherMeta expected{
				.mName = std::string(WEATHER_NAME_SANDSTORM),
				.mWeatherID = toWeatherID(BuiltinWeatherID::Sandstorm),
			};

			const WeatherMeta *actual{registry.getWeatherMetadata(toWeatherID(BuiltinWeatherID::Sandstorm))};

			CHECK((expected == *actual));
		}

		THEN("Hail has the appropriate properties")
		{
			WeatherMeta expected{
				.mName = std::string(WEATHER_NAME_HAIL),
				.mWeatherID = toWeatherID(BuiltinWeatherID::Hail),
			};

			const WeatherMeta *actual{registry.getWeatherMetadata(toWeatherID(BuiltinWeatherID::Hail))};

			CHECK((expected == *actual));
		}

		THEN("Snow has the appropriate properties")
		{
			WeatherMeta expected{
				.mName = std::string(WEATHER_NAME_SNOW),
				.mWeatherID = toWeatherID(BuiltinWeatherID::Snow),
			};

			const WeatherMeta *actual{registry.getWeatherMetadata(toWeatherID(BuiltinWeatherID::Snow))};

			CHECK((expected == *actual));
		}

		THEN("Fog has the appropriate properties")
		{
			WeatherMeta expected{
				.mName = std::string(WEATHER_NAME_FOG),
				.mWeatherID = toWeatherID(BuiltinWeatherID::Fog),
			};

			const WeatherMeta *actual{registry.getWeatherMetadata(toWeatherID(BuiltinWeatherID::Fog))};

			CHECK((expected == *actual));
		}

		THEN("Extremely Harsh Sunlight has the appropriate properties")
		{
			WeatherMeta expected{
				.mName = std::string(WEATHER_NAME_EXTREMELY_HARSH_SUNLIGHT),
				.mWeatherID = toWeatherID(BuiltinWeatherID::ExtremelyHarshSunlight),
			};

			const WeatherMeta *actual{registry.getWeatherMetadata(toWeatherID(BuiltinWeatherID::ExtremelyHarshSunlight))};

			CHECK((expected == *actual));
		}

		THEN("Heavy Rain has the appropriate properties")
		{
			WeatherMeta expected{
				.mName = std::string(WEATHER_NAME_HEAVY_RAIN),
				.mWeatherID = toWeatherID(BuiltinWeatherID::HeavyRain),
			};

			const WeatherMeta *actual{registry.getWeatherMetadata(toWeatherID(BuiltinWeatherID::HeavyRain))};

			CHECK((expected == *actual));
		}

		THEN("Strong Winds has the appropriate properties")
		{
			WeatherMeta expected{
				.mName = std::string(WEATHER_NAME_STRONG_WINDS),
				.mWeatherID = toWeatherID(BuiltinWeatherID::StrongWinds),
			};

			const WeatherMeta *actual{registry.getWeatherMetadata(toWeatherID(BuiltinWeatherID::StrongWinds))};

			CHECK((expected == *actual));
		}

		THEN("Shadowy Aura has the appropriate properties")
		{
			WeatherMeta expected{
				.mName = std::string(WEATHER_NAME_SHADOWY_AURA),
				.mWeatherID = toWeatherID(BuiltinWeatherID::ShadowyAura),
			};

			const WeatherMeta *actual{registry.getWeatherMetadata(toWeatherID(BuiltinWeatherID::ShadowyAura))};

			CHECK((expected == *actual));
		}
	}

	GIVEN("getWeatherMetadata")
	{
		THEN("unknown IDs are absent")
		{
			CHECK((registry.getWeatherMetadata(WeatherID{200}) == nullptr));
		}

		THEN("the metadata is retrieved when accessed by a valid Weather ID")
		{
			WeatherMeta expected{
				.mName = std::string(WEATHER_NAME_NONE),
				.mWeatherID = toWeatherID(BuiltinWeatherID::None),
			};

			CHECK((expected == *registry.getWeatherMetadata(NO_WEATHER_ID)));
		}
	}

	GIVEN("getWeatherID")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(registry.getWeatherID("Unknown").has_value());
		}

		THEN("the Weather ID is retrieved by valid Weather name")
		{
			std::optional<WeatherID> weatherID{registry.getWeatherID(WEATHER_NAME_NONE)};

			REQUIRE(weatherID.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((weatherID.value() == toWeatherID(BuiltinWeatherID::None)));
		}
	}

	GIVEN("getWeatherName")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(registry.getWeatherName(WeatherID{200}).has_value());
		}

		THEN("a registered weather name is returned by stable ID")
		{
			std::optional<std::string_view> weatherName{registry.getWeatherName(toWeatherID(BuiltinWeatherID::None))};

			REQUIRE(weatherName.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((weatherName.value() == WEATHER_NAME_NONE));
		}
	}

	GIVEN("getAmountRegistered")
	{
		THEN("the registered span contains the exact amount of built-in entries")
		{
			CHECK((registry.getAmountRegistered() == finalWeatherUnderlyingValue));
		}
	}

	GIVEN("getEntry")
	{
		THEN("an invalid internal array index has no metadata")
		{
			CHECK((registry.getEntry(2'000) == nullptr));
		}

		THEN("a valid internal array index has metadata")
		{
			WeatherMeta expected{
				.mName = std::string(WEATHER_NAME_NONE),
				.mWeatherID = toWeatherID(BuiltinWeatherID::None),
			};

			const WeatherMeta *weatherMeta{registry.getEntry(0)};

			REQUIRE((weatherMeta != nullptr));
			CHECK((*weatherMeta == expected));
		}
	}

	GIVEN("getRegisteredWeathers")
	{
		THEN("the amount of weathers returned matches the amount that are built-in")
		{
			CHECK((registry.getRegisteredWeathers().size() == finalWeatherUnderlyingValue));
		}
	}

	GIVEN("getNextWeatherID")
	{
		THEN("the next available stable Weather ID is after all built in weather IDs")
		{
			CHECK((registry.getNextWeatherID() == finalWeatherUnderlyingValue));
		}
	}

	GIVEN("findIndexByWeatherID")
	{
		THEN("an unknown stable ID has no internal index")
		{
			std::optional<ub> weatherIndex{registry.findIndexByWeatherID(WeatherID{200})};
			CHECK_FALSE(weatherIndex.has_value());
		}

		THEN("the internal array index is retrieved by valid Weather ID")
		{
			std::optional<ub> weatherIndex{registry.findIndexByWeatherID(NO_WEATHER_ID)};

			REQUIRE(weatherIndex.has_value());
			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((weatherIndex.value() == 0));
		}
	}

	GIVEN("hasWeather")
	{
		WHEN("calling the string_view overload")
		{
			THEN("an unknown weather name has no entry")
			{
				CHECK_FALSE(registry.hasWeather("Unknown"));
			}

			THEN("a known weather name has an entry")
			{
				CHECK(registry.hasWeather(WEATHER_NAME_NONE));
			}
		}

		WHEN("calling the WeatherID overload")
		{
			THEN("an unknown weather ID has no entry")
			{
				CHECK_FALSE(registry.hasWeather(WeatherID{200}));
			}

			THEN("a known weather ID has an entry")
			{
				CHECK(registry.hasWeather(NO_WEATHER_ID));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
