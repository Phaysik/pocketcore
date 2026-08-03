#include "Configuration/statusRegistryConfiguration.h"

#include <algorithm>
#include <array>
#include <string_view>

#include <catch2/catch_test_macros.hpp>

#include "Utility/Debug/Logging/logger.h"

using PocketCore::Configuration::RegistryError;
using PocketCore::Configuration::StatusRegistryConfiguration;
using PocketCore::Status::StatusID;
using PocketCore::Status::StatusMeta;
using PocketCore::Utility::Debug::Logging::Logger;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

namespace
{
	void ensureStatusLoggerInitialized() // NOLINT(llvm-prefer-static-over-anonymous-namespace)
	{
		static bool initialized{false};

		if (!initialized)
		{
			initialized = Logger::initialize("src_test", "statusRegistryConfiguration_test.log", true);
		}
	}

	StatusMeta makeStatus(const std::string_view name)
	{
		return StatusMeta{.mName = name};
	}
} // namespace

SCENARIO("StatusRegistryConfiguration addStatus")
{
	ensureStatusLoggerInitialized();
	StatusRegistryConfiguration configuration{};

	GIVEN("a unique status definition")
	{
		auto result{configuration.addStatus(makeStatus("Custom Status"))};

		THEN("it is registered and queryable")
		{
			REQUIRE(result.has_value());
			StatusID customIdentifier{result.value()};
			CHECK(configuration.hasStatus(customIdentifier));
			CHECK(configuration.hasStatus("Custom Status"));

			auto nameResult{configuration.getStatusName(customIdentifier)};
			REQUIRE(nameResult.has_value());
			CHECK((nameResult.value() == "Custom Status"));

			const StatusMeta *metadata{configuration.getStatusMetadata(customIdentifier)};
			REQUIRE((metadata != nullptr));
			CHECK((metadata->mName == "Custom Status"));
		}
	}

	GIVEN("a duplicate status name")
	{
		auto firstResult{configuration.addStatus(makeStatus("Duplicate Status"))};
		REQUIRE(firstResult.has_value());

		THEN("registration returns DuplicateStatus")
		{
			auto secondResult{configuration.addStatus(makeStatus("Duplicate Status"))};
			REQUIRE_FALSE(secondResult.has_value());
			CHECK((secondResult.error().mKind == RegistryError::DuplicateStatus));
		}
	}
}

SCENARIO("StatusRegistryConfiguration addStatuses")
{
	ensureStatusLoggerInitialized();
	StatusRegistryConfiguration configuration{};

	GIVEN("a batch with duplicate names")
	{
		std::array<StatusMeta, 3> definitions{{
			makeStatus("Batch One"),
			makeStatus("Batch Two"),
			makeStatus("Batch One"),
		}};

		WHEN("the batch is added")
		{
			auto result{configuration.addStatuses(definitions)};

			THEN("all additions are rolled back")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::DuplicateStatus));
				CHECK_FALSE(configuration.hasStatus("Batch One"));
				CHECK_FALSE(configuration.hasStatus("Batch Two"));
			}
		}
	}
}

SCENARIO("StatusRegistryConfiguration metadata lifecycle")
{
	ensureStatusLoggerInitialized();
	StatusRegistryConfiguration configuration{};

	GIVEN("a registered custom status")
	{
		auto addResult{configuration.addStatus(makeStatus("Custom Status"))};
		REQUIRE(addResult.has_value());
		StatusID customIdentifier{addResult.value()};

		WHEN("it is renamed")
		{
			auto renameResult{configuration.renameStatus("Custom Status", "Renamed Status")};

			THEN("the stable ID remains associated with renamed metadata")
			{
				REQUIRE(renameResult.has_value());
				CHECK_FALSE(configuration.hasStatus("Custom Status"));
				CHECK(configuration.hasStatus("Renamed Status"));

				auto idResult{configuration.getStatusID("Renamed Status")};
				REQUIRE(idResult.has_value());
				CHECK((idResult.value() == customIdentifier));
			}
		}

		WHEN("it is updated by name")
		{
			auto updateResult{configuration.updateStatus("Custom Status", makeStatus("Replacement Status"))};

			THEN("replacement metadata is discoverable")
			{
				REQUIRE(updateResult.has_value());
				CHECK(configuration.hasStatus("Replacement Status"));
				CHECK_FALSE(configuration.hasStatus("Custom Status"));
			}
		}

		WHEN("it is updated by stable ID")
		{
			auto updateResult{configuration.updateStatus(customIdentifier, makeStatus("Replacement By ID"))};

			THEN("replacement metadata is discoverable")
			{
				REQUIRE(updateResult.has_value());
				CHECK(configuration.hasStatus("Replacement By ID"));
			}
		}

		WHEN("it is removed by name")
		{
			auto removeResult{configuration.removeStatus("Custom Status")};

			THEN("the removed ID is returned")
			{
				REQUIRE(removeResult.has_value());
				CHECK((removeResult.value() == customIdentifier));
				CHECK_FALSE(configuration.hasStatus(customIdentifier));
			}
		}

		WHEN("it is removed then another entry is added")
		{
			auto removeResult{configuration.removeStatus(customIdentifier)};
			REQUIRE(removeResult.has_value());

			auto laterResult{configuration.addStatus(makeStatus("Later Status"))};

			THEN("the removed ID is not reused")
			{
				REQUIRE(laterResult.has_value());
				CHECK((laterResult.value() != customIdentifier));
				CHECK((configuration.getStatusMetadata(customIdentifier) == nullptr));
			}
		}
	}

	GIVEN("an unknown status")
	{
		THEN("rename update and remove report StatusNotFound")
		{
			auto renameResult{configuration.renameStatus("Missing", "Renamed")};
			auto updateResult{configuration.updateStatus("Missing", makeStatus("Updated"))};
			auto removeResult{configuration.removeStatus("Missing")};

			REQUIRE_FALSE(renameResult.has_value());
			REQUIRE_FALSE(updateResult.has_value());
			REQUIRE_FALSE(removeResult.has_value());
			CHECK((renameResult.error().mKind == RegistryError::StatusNotFound));
			CHECK((updateResult.error().mKind == RegistryError::StatusNotFound));
			CHECK((removeResult.error().mKind == RegistryError::StatusNotFound));
		}
	}
}

SCENARIO("StatusRegistryConfiguration registered span")
{
	ensureStatusLoggerInitialized();
	StatusRegistryConfiguration configuration{};

	GIVEN("a custom registration")
	{
		auto addResult{configuration.addStatus(makeStatus("Span Status"))};
		REQUIRE(addResult.has_value());

		THEN("registered span contains the new name")
		{
			const auto registeredStatuses{configuration.getRegisteredStatuses()};
			auto found = std::find_if(registeredStatuses.begin(), registeredStatuses.end(),
				[](const StatusMeta &metadata) { return metadata.mName == "Span Status"; });
			CHECK((found != registeredStatuses.end()));
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)