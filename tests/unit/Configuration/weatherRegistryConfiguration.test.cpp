#include "Configuration/weatherRegistryConfiguration.h"

#include <algorithm>
#include <array>
#include <string_view>

#include <catch2/catch_test_macros.hpp>

#include "Utility/Debug/Logging/logger.h"

using PocketCore::Configuration::RegistryError;
using PocketCore::Configuration::WeatherRegistryConfiguration;
using PocketCore::Utility::Debug::Logging::Logger;
using PocketCore::Weather::WeatherID;
using PocketCore::Weather::WeatherMeta;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

namespace
{
	void ensureWeatherLoggerInitialized() // NOLINT(llvm-prefer-static-over-anonymous-namespace)
	{
		static bool initialized{false};

		if (!initialized)
		{
			initialized = Logger::initialize("wrc_test", "weatherRegistryConfiguration_test.log", true);
		}
	}

	WeatherMeta makeWeather(const std::string_view name)
	{
		return WeatherMeta{.mName = name};
	}
} // namespace

SCENARIO("WeatherRegistryConfiguration addWeather")
{
	ensureWeatherLoggerInitialized();
	WeatherRegistryConfiguration configuration{};

	GIVEN("a unique weather definition")
	{
		auto result{configuration.addWeather(makeWeather("Custom Weather"))};

		THEN("it is registered and queryable")
		{
			REQUIRE(result.has_value());
			WeatherID customIdentifier{result.value()};
			CHECK(configuration.hasWeather(customIdentifier));
			CHECK(configuration.hasWeather("Custom Weather"));

			auto nameResult{configuration.getWeatherName(customIdentifier)};
			REQUIRE(nameResult.has_value());
			CHECK((nameResult.value() == "Custom Weather"));

			const WeatherMeta *metadata{configuration.getWeatherMetadata(customIdentifier)};
			REQUIRE((metadata != nullptr));
			CHECK((metadata->mName == "Custom Weather"));
		}
	}

	GIVEN("a duplicate weather name")
	{
		auto firstResult{configuration.addWeather(makeWeather("Duplicate Weather"))};
		REQUIRE(firstResult.has_value());

		THEN("registration returns DuplicateWeather")
		{
			auto secondResult{configuration.addWeather(makeWeather("Duplicate Weather"))};
			REQUIRE_FALSE(secondResult.has_value());
			CHECK((secondResult.error().mKind == RegistryError::DuplicateWeather));
		}
	}
}

SCENARIO("WeatherRegistryConfiguration addWeathers")
{
	ensureWeatherLoggerInitialized();
	WeatherRegistryConfiguration configuration{};

	GIVEN("a batch with duplicate names")
	{
		std::array<WeatherMeta, 3> definitions{{
			makeWeather("Batch One"),
			makeWeather("Batch Two"),
			makeWeather("Batch One"),
		}};

		WHEN("the batch is added")
		{
			auto result{configuration.addWeathers(definitions)};

			THEN("all additions are rolled back")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::DuplicateWeather));
				CHECK_FALSE(configuration.hasWeather("Batch One"));
				CHECK_FALSE(configuration.hasWeather("Batch Two"));
			}
		}
	}
}

SCENARIO("WeatherRegistryConfiguration metadata lifecycle")
{
	ensureWeatherLoggerInitialized();
	WeatherRegistryConfiguration configuration{};

	GIVEN("a registered custom weather")
	{
		auto addResult{configuration.addWeather(makeWeather("Custom Weather"))};
		REQUIRE(addResult.has_value());
		WeatherID customIdentifier{addResult.value()};

		WHEN("it is renamed")
		{
			auto renameResult{configuration.renameWeather("Custom Weather", "Renamed Weather")};

			THEN("the stable ID remains associated with renamed metadata")
			{
				REQUIRE(renameResult.has_value());
				CHECK_FALSE(configuration.hasWeather("Custom Weather"));
				CHECK(configuration.hasWeather("Renamed Weather"));

				auto idResult{configuration.getWeatherID("Renamed Weather")};
				REQUIRE(idResult.has_value());
				CHECK((idResult.value() == customIdentifier));
			}
		}

		WHEN("it is updated by name")
		{
			auto updateResult{configuration.updateWeather("Custom Weather", makeWeather("Replacement Weather"))};

			THEN("replacement metadata is discoverable")
			{
				REQUIRE(updateResult.has_value());
				CHECK(configuration.hasWeather("Replacement Weather"));
				CHECK_FALSE(configuration.hasWeather("Custom Weather"));
			}
		}

		WHEN("it is updated by stable ID")
		{
			auto updateResult{configuration.updateWeather(customIdentifier, makeWeather("Replacement By ID"))};

			THEN("replacement metadata is discoverable")
			{
				REQUIRE(updateResult.has_value());
				CHECK(configuration.hasWeather("Replacement By ID"));
			}
		}

		WHEN("it is removed by name")
		{
			auto removeResult{configuration.removeWeather("Custom Weather")};

			THEN("the removed ID is returned")
			{
				REQUIRE(removeResult.has_value());
				CHECK((removeResult.value() == customIdentifier));
				CHECK_FALSE(configuration.hasWeather(customIdentifier));
			}
		}

		WHEN("it is removed then another entry is added")
		{
			auto removeResult{configuration.removeWeather(customIdentifier)};
			REQUIRE(removeResult.has_value());

			auto laterResult{configuration.addWeather(makeWeather("Later Weather"))};

			THEN("the removed ID is not reused")
			{
				REQUIRE(laterResult.has_value());
				CHECK((laterResult.value() != customIdentifier));
				CHECK((configuration.getWeatherMetadata(customIdentifier) == nullptr));
			}
		}
	}

	GIVEN("an unknown weather")
	{
		THEN("rename update and remove report WeatherNotFound")
		{
			auto renameResult{configuration.renameWeather("Missing", "Renamed")};
			auto updateResult{configuration.updateWeather("Missing", makeWeather("Updated"))};
			auto removeResult{configuration.removeWeather("Missing")};

			REQUIRE_FALSE(renameResult.has_value());
			REQUIRE_FALSE(updateResult.has_value());
			REQUIRE_FALSE(removeResult.has_value());
			CHECK((renameResult.error().mKind == RegistryError::WeatherNotFound));
			CHECK((updateResult.error().mKind == RegistryError::WeatherNotFound));
			CHECK((removeResult.error().mKind == RegistryError::WeatherNotFound));
		}
	}
}

SCENARIO("WeatherRegistryConfiguration registered span")
{
	ensureWeatherLoggerInitialized();
	WeatherRegistryConfiguration configuration{};

	GIVEN("a custom registration")
	{
		auto addResult{configuration.addWeather(makeWeather("Span Weather"))};
		REQUIRE(addResult.has_value());

		THEN("registered span contains the new name")
		{
			const auto registeredWeathers{configuration.getRegisteredWeathers()};
			auto found = std::find_if(registeredWeathers.begin(), registeredWeathers.end(),
				[](const WeatherMeta &metadata) { return metadata.mName == "Span Weather"; });
			CHECK((found != registeredWeathers.end()));
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)