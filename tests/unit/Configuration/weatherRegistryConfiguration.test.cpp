/*! @file weatherRegistryConfiguration.test.cpp
	@brief C++ file for running tests for the WeatherRegistryConfiguration.
	@date 09/10/2026
	@since 0.8.7
	@version 0.12.20
	@author Matthew Moore
*/

#include "Configuration/weatherRegistryConfiguration.h"

#include <expected>
#include <format>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "Configuration/constants.h"
#include "Core/typedefs.h"
#include "Registry/registryError.h"
#include "Registry/weatherRegistry.h"
#include "Utility/Debug/Logging/logging.testHelper.h"
#include "Weather/builtInWeatherID.h"
#include "Weather/constants.h"
#include "Weather/weatherID.h"
#include "Weather/weatherMeta.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Configuration::MAX_WEATHERS;
using PocketCore::Configuration::RegistryError;
using PocketCore::Configuration::WeatherRegistryConfiguration;
using PocketCore::Core::ub;
using PocketCore::Core::us;
using PocketCore::Registry::RegistryErrorInfo;
using PocketCore::Registry::Weather::WeatherRegistry;
using PocketCore::Testing::ensureLoggerInitialized;
using PocketCore::Weather::BuiltinWeatherID;
using PocketCore::Weather::NO_WEATHER_ID;
using PocketCore::Weather::toWeatherID;
using PocketCore::Weather::WEATHER_NAME_FOG;
using PocketCore::Weather::WEATHER_NAME_NONE;
using PocketCore::Weather::WeatherID;
using PocketCore::Weather::WeatherMeta;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("WeatherRegistryConfiguration")
{
	ensureLoggerInitialized("weather registry configuration test", "weatherRegistryConfiguration_test.log");

	WeatherRegistryConfiguration config{};
	WeatherRegistry registry{};
	ub finalWeatherUnderlyingValue{std::to_underlying(BuiltinWeatherID::FinalWeather)};

	GIVEN("getAmountRegistered")
	{
		THEN("the default registry returns the expected amount of built-in weathers")
		{
			CHECK((config.getAmountRegistered() == finalWeatherUnderlyingValue));
		}
	}

	GIVEN("getRuntimeRegistry")
	{
		THEN("the runtime registry is the same as the default registry")
		{
			CHECK((config.getRuntimeRegistry() == registry));
		}
	}

	GIVEN("getWeatherMetadata")
	{
		THEN("unknown IDs are absent")
		{
			CHECK((config.getWeatherMetadata(WeatherID{200}) == nullptr));
		}

		THEN("the metadata is retrieved when accessed by a valid Weather ID")
		{
			WeatherMeta expected{
				.mName = std::string(WEATHER_NAME_NONE),
				.mWeatherID = toWeatherID(BuiltinWeatherID::None),
			};

			CHECK((expected == *config.getWeatherMetadata(NO_WEATHER_ID)));
		}
	}

	GIVEN("getWeatherID")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(config.getWeatherID("Unknown").has_value());
		}

		THEN("the Weather ID is retrieved by valid Weather name")
		{
			std::optional<WeatherID> weatherID{config.getWeatherID(WEATHER_NAME_NONE)};

			REQUIRE(weatherID.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((weatherID.value() == toWeatherID(BuiltinWeatherID::None)));
		}
	}

	GIVEN("getWeatherName")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(config.getWeatherName(WeatherID{200}).has_value());
		}

		THEN("a registered weather name is returned by stable ID")
		{
			std::optional<std::string_view> weatherName{config.getWeatherName(toWeatherID(BuiltinWeatherID::None))};

			REQUIRE(weatherName.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((weatherName.value() == WEATHER_NAME_NONE));
		}
	}

	GIVEN("getRegisteredWeathers")
	{
		THEN("the amount of weathers returned matches the amount that are built-in")
		{
			CHECK((config.getRegisteredWeathers().size() == finalWeatherUnderlyingValue));
		}
	}

	GIVEN("hasWeather")
	{
		WHEN("calling the string_view overload")
		{
			THEN("an unknown weather name has no entry")
			{
				CHECK_FALSE(config.hasWeather("Unknown"));
			}

			THEN("a known weather name has an entry")
			{
				CHECK(config.hasWeather(WEATHER_NAME_NONE));
			}
		}

		WHEN("calling the WeatherID overload")
		{
			THEN("an unknown weather ID has no entry")
			{
				CHECK_FALSE(config.hasWeather(WeatherID{200}));
			}

			THEN("a known weather ID has an entry")
			{
				CHECK(config.hasWeather(NO_WEATHER_ID));
			}
		}
	}

	GIVEN("addWeather")
	{
		WHEN("trying to add an weather past the capacity")
		{
			us newWeatherCount{finalWeatherUnderlyingValue};

			for (us i{0}; i < MAX_WEATHERS - finalWeatherUnderlyingValue; ++i)
			{
				std::string name{std::format("String_{:04}", i)};
				WeatherMeta definition{.mName = name};
				std::expected<WeatherID, RegistryErrorInfo> result{config.addWeather(definition)};

				REQUIRE(result.has_value());

				WeatherID assignedID{result.value()};
				CHECK((assignedID.getValue() == newWeatherCount++));
			}

			std::string name{std::format("String_{:04}", MAX_WEATHERS + 1)};

			WeatherMeta definition{.mName = name};
			std::expected<WeatherID, RegistryErrorInfo> result{config.addWeather(definition)};

			THEN("registration reports a max capacity and nothing is added")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::MaxCapacity));
				CHECK((config.getAmountRegistered() == MAX_WEATHERS));
			}
		}

		WHEN("an weather whose name is already in use is added")
		{
			WeatherMeta definition{.mName = std::string(WEATHER_NAME_FOG)};
			std::expected<WeatherID, RegistryErrorInfo> result{config.addWeather(definition)};

			THEN("registration reports a duplicate weather and nothing is added")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::DuplicateWeather));
				CHECK((config.getAmountRegistered() == finalWeatherUnderlyingValue));
			}
		}

		WHEN("a unique weather definition is added")
		{
			WeatherMeta definition{.mName = "TestWeatherName"};

			std::expected<WeatherID, RegistryErrorInfo> result{config.addWeather(definition)};

			THEN("it receives the first custom stable ID and owns its trigger data")
			{
				REQUIRE(result.has_value());
				WeatherID assignedID{result.value()};
				CHECK((assignedID.getValue() == finalWeatherUnderlyingValue));

				const WeatherMeta *metadata{config.getWeatherMetadata(assignedID)};

				REQUIRE((metadata != nullptr));
				CHECK((metadata->mName == "TestWeatherName"));
			}
		}
	}

	GIVEN("addWeathers")
	{
		WHEN("trying to add an weather past the capacity")
		{
			std::vector<WeatherMeta> weatherMetas;

			for (us i{0}; i < MAX_WEATHERS - finalWeatherUnderlyingValue; ++i)
			{
				std::string name{std::format("String_{:04}", i)};
				weatherMetas.push_back({.mName = name});
			}

			std::string name{std::format("String_{:04}", MAX_WEATHERS + 1)};

			weatherMetas.push_back({.mName = name});
			std::expected<void, RegistryErrorInfo> result{config.addWeathers(weatherMetas)};

			THEN("registration reports a max capacity and nothing is added")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::MaxCapacity));
				CHECK((config.getAmountRegistered() == finalWeatherUnderlyingValue));
			}
		}

		WHEN("an weather whose name is already in use is added")
		{
			std::vector<WeatherMeta> weatherMetas{};

			for (us i{0}; i < 20; ++i)
			{
				std::string name{std::format("String_{:04}", i)};
				weatherMetas.push_back({.mName = name});
			}

			weatherMetas.push_back({.mName = std::string(WEATHER_NAME_FOG)});
			std::expected<void, RegistryErrorInfo> result{config.addWeathers(weatherMetas)};

			THEN("registration reports a duplicate weather and the registry is rollback to the checkpoint before the erroneous addition")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::DuplicateWeather));
				CHECK((config.getAmountRegistered() == finalWeatherUnderlyingValue));
			}
		}

		WHEN("a unique weather definition is added")
		{
			std::vector<WeatherMeta> weatherMetas{};

			weatherMetas.push_back({.mName = "TestWeatherName"});

			std::expected<void, RegistryErrorInfo> result{config.addWeathers(weatherMetas)};

			THEN("the registry reports no error and all the weather definitions are added")
			{
				REQUIRE(result.has_value());
				CHECK((config.getAmountRegistered() == finalWeatherUnderlyingValue + 1));
			}
		}
	}

	GIVEN("renameWeather")
	{
		WHEN("calling with an invalid weather name")
		{
			std::expected<void, RegistryErrorInfo> result{config.renameWeather("ThisIsInvalid", "NewName")};

			THEN("the registry reports an error and there is no update to the registry")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::WeatherNotFound));
				CHECK((config.getAmountRegistered() == finalWeatherUnderlyingValue));
			}
		}

		WHEN("trying to rename to a name already in the registry")
		{
			std::expected<void, RegistryErrorInfo> result{
				config.renameWeather(WEATHER_NAME_NONE, WEATHER_NAME_FOG),
			};

			THEN("the registry reports an error and there is no update to the registry")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::DuplicateWeather));
				CHECK((config.getAmountRegistered() == finalWeatherUnderlyingValue));
			}
		}

		WHEN("updating an existing weather definition")
		{
			std::expected<void, RegistryErrorInfo> result{config.renameWeather(WEATHER_NAME_NONE, "NewName")};

			THEN("the registry reports no error and the name is appropriately updated")
			{
				REQUIRE(result.has_value());
				CHECK((config.getAmountRegistered() == finalWeatherUnderlyingValue));
				CHECK((config.getWeatherMetadata(toWeatherID(BuiltinWeatherID::None))->mName == "NewName"));
			}
		}
	}

	GIVEN("updateWeather")
	{
		WeatherMeta definition{.mName = "TestWeatherName"};

		WHEN("calling the string_view overload")
		{
			WHEN("calling with an invalid weather name")
			{
				std::expected<void, RegistryErrorInfo> result{config.updateWeather("ThisIsInvalid", definition)};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::WeatherNotFound));
					CHECK((config.getAmountRegistered() == finalWeatherUnderlyingValue));
				}
			}

			WHEN("trying to rename to a name already in the registry")
			{
				std::expected<void, RegistryErrorInfo> result{
					config.updateWeather(WEATHER_NAME_NONE, {.mName = std::string(WEATHER_NAME_FOG)}),
				};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::DuplicateWeather));
					CHECK((config.getAmountRegistered() == finalWeatherUnderlyingValue));
				}
			}

			WHEN("updating an existing weather definition")
			{
				std::expected<void, RegistryErrorInfo> result{config.updateWeather(WEATHER_NAME_NONE, definition)};

				THEN("the registry reports no error and the target is appropriately updated")
				{
					REQUIRE(result.has_value());
					CHECK((config.getAmountRegistered() == finalWeatherUnderlyingValue));
					CHECK((config.getWeatherMetadata(toWeatherID(BuiltinWeatherID::None))->mName == "TestWeatherName"));
				}
			}
		}

		WHEN("calling the WeatherID overload")
		{
			WHEN("calling with an invalid weather name")
			{
				std::expected<void, RegistryErrorInfo> result{config.updateWeather(WeatherID{200}, definition)};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::WeatherNotFound));
					CHECK((config.getAmountRegistered() == finalWeatherUnderlyingValue));
				}
			}

			WHEN("trying to rename to a name already in the registry")
			{
				std::expected<void, RegistryErrorInfo> result{
					config.updateWeather(toWeatherID(BuiltinWeatherID::None), {.mName = std::string(WEATHER_NAME_FOG)}),
				};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::DuplicateWeather));
					CHECK((config.getAmountRegistered() == finalWeatherUnderlyingValue));
				}
			}

			WHEN("updating an existing weather definition")
			{
				std::expected<void, RegistryErrorInfo> result{config.updateWeather(toWeatherID(BuiltinWeatherID::None), definition)};

				THEN("the registry reports no error and the target is appropriately updated")
				{
					REQUIRE(result.has_value());
					CHECK((config.getAmountRegistered() == finalWeatherUnderlyingValue));
					CHECK((config.getWeatherMetadata(toWeatherID(BuiltinWeatherID::None))->mName == "TestWeatherName"));
				}
			}
		}
	}

	GIVEN("removeWeather")
	{
		WHEN("calling the string_view overload")
		{
			WHEN("calling with an unknown weather name")
			{
				std::expected<WeatherID, RegistryErrorInfo> result{config.removeWeather("Unknown")};

				THEN("the result is an error and the registry is not updated")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::WeatherNotFound));
					CHECK((config.getAmountRegistered() == finalWeatherUnderlyingValue));
				}
			}

			WHEN("calling with a known weather name")
			{
				std::expected<WeatherID, RegistryErrorInfo> result{config.removeWeather(WEATHER_NAME_NONE)};

				THEN("the result is a success and the registry is updated")
				{
					REQUIRE(result.has_value());
					CHECK((result.value() == toWeatherID(BuiltinWeatherID::None)));
					CHECK((config.getAmountRegistered() == finalWeatherUnderlyingValue - 1));
				}
			}
		}

		WHEN("calling the WeatherID overload")
		{
			WHEN("calling with an unknown weather ID")
			{
				std::expected<WeatherID, RegistryErrorInfo> result{config.removeWeather(WeatherID{200})};

				THEN("the result is an error and the registry is not updated")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::WeatherNotFound));
					CHECK((config.getAmountRegistered() == finalWeatherUnderlyingValue));
				}
			}

			WHEN("calling with a known weather ID")
			{
				std::expected<WeatherID, RegistryErrorInfo> result{config.removeWeather(toWeatherID(BuiltinWeatherID::None))};

				THEN("the result is a success and the registry is updated")
				{
					REQUIRE(result.has_value());
					CHECK((result.value() == toWeatherID(BuiltinWeatherID::None)));
					CHECK((config.getAmountRegistered() == finalWeatherUnderlyingValue - 1));
				}
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
