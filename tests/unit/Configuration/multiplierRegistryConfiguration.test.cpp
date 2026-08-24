/*! @file multiplierRegistryConfiguration.test.cpp
	@brief C++ file for running tests for the MultiplierRegistryConfiguration.
	@date --/--/----
	@since x.x.x
	@version x.x.x
	@author Matthew Moore
*/

#include "Configuration/multiplierRegistryConfiguration.h"

#include <algorithm>
#include <array>
#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

#include "Configuration/constants.h"
#include "Multiplier/multiplierID.h"
#include "Multiplier/multiplierMeta.h"
#include "Utility/Debug/Logging/logger.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Configuration::MAX_MULTIPLIERS;
using PocketCore::Configuration::MultiplierRegistryConfiguration;
using PocketCore::Configuration::RegistryError;
using PocketCore::Multiplier::MultiplierID;
using PocketCore::Multiplier::MultiplierMeta;
using PocketCore::Utility::Debug::Logging::Logger;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity,llvm-prefer-static-over-anonymous-namespace)

namespace
{
	void ensureMultiplierLoggerInitialized()
	{
		static bool initialized{false};

		if (!initialized)
		{
			initialized = Logger::initialize("mulrc_test", "multiplierRegistryConfiguration_test.log", true);
		}
	}

	MultiplierMeta makeMultiplier(const std::string_view &name)
	{
		return MultiplierMeta{.mName = name};
	}
} // namespace

SCENARIO("MultiplierRegistryConfiguration addMultiplier")
{
	ensureMultiplierLoggerInitialized();
	MultiplierRegistryConfiguration configuration{};

	GIVEN("a unique multiplier definition")
	{
		auto result{configuration.addMultiplier(makeMultiplier("Custom Multiplier"))};

		THEN("it is registered and queryable by ID and name")
		{
			REQUIRE(result.has_value());
			MultiplierID customIdentifier{result.value()};
			CHECK(configuration.hasMultiplier(customIdentifier));
			CHECK(configuration.hasMultiplier("Custom Multiplier"));

			auto nameResult{configuration.getMultiplierName(customIdentifier)};
			REQUIRE(nameResult.has_value());
			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((nameResult.value() == "Custom Multiplier"));

			const MultiplierMeta *metadata{configuration.getMultiplierMetadata(customIdentifier)};
			REQUIRE((metadata != nullptr));
			CHECK((metadata->mName == "Custom Multiplier"));
		}
	}

	GIVEN("a duplicate multiplier name")
	{
		auto firstResult{configuration.addMultiplier(makeMultiplier("Duplicate Multiplier"))};
		REQUIRE(firstResult.has_value());

		THEN("registration returns DuplicateMultiplier")
		{
			auto secondResult{configuration.addMultiplier(makeMultiplier("Duplicate Multiplier"))};
			REQUIRE_FALSE(secondResult.has_value());
			CHECK((secondResult.error().mKind == RegistryError::DuplicateMultiplier));
		}
	}
}

SCENARIO("MultiplierRegistryConfiguration addMultipliers")
{
	ensureMultiplierLoggerInitialized();
	MultiplierRegistryConfiguration configuration{};

	GIVEN("a batch with duplicate names")
	{
		std::array<MultiplierMeta, 3> definitions{
			{
				makeMultiplier("Batch One"),
				makeMultiplier("Batch Two"),
				makeMultiplier("Batch One"),
			},
		};

		WHEN("the batch is added")
		{
			auto result{configuration.addMultipliers(definitions)};

			THEN("all additions are rolled back")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::DuplicateMultiplier));
				CHECK_FALSE(configuration.hasMultiplier("Batch One"));
				CHECK_FALSE(configuration.hasMultiplier("Batch Two"));
			}
		}
	}

	GIVEN("a batch that exceeds remaining capacity")
	{
		std::vector<std::string> names{};
		names.reserve(static_cast<std::size_t>(MAX_MULTIPLIERS));
		std::vector<MultiplierMeta> definitions{};
		definitions.reserve(static_cast<std::size_t>(MAX_MULTIPLIERS));

		for (std::size_t index{0}; index < static_cast<std::size_t>(MAX_MULTIPLIERS); ++index)
		{
			names.push_back("Overflow Batch " + std::to_string(index));
			definitions.push_back(makeMultiplier(names.back()));
		}

		WHEN("the oversized batch is added")
		{
			auto result{configuration.addMultipliers(definitions)};

			THEN("a MaxCapacity error is returned")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::MaxCapacity));
			}
		}
	}
}

SCENARIO("MultiplierRegistryConfiguration metadata lifecycle")
{
	ensureMultiplierLoggerInitialized();
	MultiplierRegistryConfiguration configuration{};

	GIVEN("a registered custom multiplier")
	{
		auto addResult{configuration.addMultiplier(makeMultiplier("Custom Multiplier"))};
		REQUIRE(addResult.has_value());
		MultiplierID customIdentifier{addResult.value()};

		WHEN("it is renamed")
		{
			auto renameResult{configuration.renameMultiplier("Custom Multiplier", "Renamed Multiplier")};

			THEN("the stable ID remains associated with renamed metadata")
			{
				REQUIRE(renameResult.has_value());
				CHECK_FALSE(configuration.hasMultiplier("Custom Multiplier"));
				CHECK(configuration.hasMultiplier("Renamed Multiplier"));

				auto idResult{configuration.getMultiplierID("Renamed Multiplier")};
				REQUIRE(idResult.has_value());
				// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
				CHECK((idResult.value() == customIdentifier));
			}
		}

		WHEN("it is updated by name")
		{
			auto updateResult{configuration.updateMultiplier("Custom Multiplier", makeMultiplier("Replacement Multiplier"))};

			THEN("replacement metadata is discoverable")
			{
				REQUIRE(updateResult.has_value());
				CHECK(configuration.hasMultiplier("Replacement Multiplier"));
				CHECK_FALSE(configuration.hasMultiplier("Custom Multiplier"));
			}
		}

		WHEN("it is updated by stable ID")
		{
			auto updateResult{configuration.updateMultiplier(customIdentifier, makeMultiplier("Replacement By ID"))};

			THEN("replacement metadata is discoverable")
			{
				REQUIRE(updateResult.has_value());
				CHECK(configuration.hasMultiplier("Replacement By ID"));
			}
		}

		WHEN("it is removed by name")
		{
			auto removeResult{configuration.removeMultiplier("Custom Multiplier")};

			THEN("the removed ID is returned")
			{
				REQUIRE(removeResult.has_value());
				CHECK((removeResult.value() == customIdentifier));
				CHECK_FALSE(configuration.hasMultiplier(customIdentifier));
			}
		}

		WHEN("it is removed then another entry is added")
		{
			auto removeResult{configuration.removeMultiplier(customIdentifier)};
			REQUIRE(removeResult.has_value());

			auto laterResult{configuration.addMultiplier(makeMultiplier("Later Multiplier"))};

			THEN("the removed ID is not reused")
			{
				REQUIRE(laterResult.has_value());
				CHECK((laterResult.value() != customIdentifier));
				CHECK((configuration.getMultiplierMetadata(customIdentifier) == nullptr));
			}
		}

		WHEN("it is renamed to an existing built-in name")
		{
			auto renameResult{configuration.renameMultiplier("Custom Multiplier", "Ability")};

			THEN("a duplicate error is returned")
			{
				REQUIRE_FALSE(renameResult.has_value());
				CHECK((renameResult.error().mKind == RegistryError::DuplicateMultiplier));
			}
		}
	}

	GIVEN("an unknown multiplier")
	{
		THEN("rename update and remove report MultiplierNotFound")
		{
			const MultiplierID unknownIdentifier{250U};
			auto renameResult{configuration.renameMultiplier("Missing", "Renamed")};
			auto updateResult{configuration.updateMultiplier("Missing", makeMultiplier("Updated"))};
			auto updateByIdentifierResult{configuration.updateMultiplier(unknownIdentifier, makeMultiplier("Updated"))};
			auto removeResult{configuration.removeMultiplier("Missing")};
			auto removeByIdentifierResult{configuration.removeMultiplier(unknownIdentifier)};

			REQUIRE_FALSE(renameResult.has_value());
			REQUIRE_FALSE(updateResult.has_value());
			REQUIRE_FALSE(updateByIdentifierResult.has_value());
			REQUIRE_FALSE(removeResult.has_value());
			REQUIRE_FALSE(removeByIdentifierResult.has_value());
			CHECK((renameResult.error().mKind == RegistryError::MultiplierNotFound));
			CHECK((updateResult.error().mKind == RegistryError::MultiplierNotFound));
			CHECK((updateByIdentifierResult.error().mKind == RegistryError::MultiplierNotFound));
			CHECK((removeResult.error().mKind == RegistryError::MultiplierNotFound));
			CHECK((removeByIdentifierResult.error().mKind == RegistryError::MultiplierNotFound));
		}
	}

	GIVEN("registry at maximum capacity")
	{
		std::vector<std::string> names{};
		names.reserve(static_cast<std::size_t>(MAX_MULTIPLIERS));

		while (true)
		{
			names.push_back("Capacity Entry " + std::to_string(names.size()));
			auto result{configuration.addMultiplier(makeMultiplier(names.back()))};

			if (!result.has_value())
			{
				THEN("adding another multiplier returns MaxCapacity")
				{
					CHECK((result.error().mKind == RegistryError::MaxCapacity));
				}

				break;
			}
		}
	}

	GIVEN("built-in entries")
	{
		THEN("removing a middle built-in entry shifts remaining entries")
		{
			auto removeResult{configuration.removeMultiplier("Item")};
			REQUIRE(removeResult.has_value());
			CHECK_FALSE(configuration.hasMultiplier("Item"));
			CHECK(configuration.hasMultiplier("Weather"));
		}
	}
}

SCENARIO("MultiplierRegistryConfiguration registered span")
{
	ensureMultiplierLoggerInitialized();
	MultiplierRegistryConfiguration configuration{};

	GIVEN("a custom registration")
	{
		auto addResult{configuration.addMultiplier(makeMultiplier("Span Multiplier"))};
		REQUIRE(addResult.has_value());

		THEN("registered span contains the new name")
		{
			const auto registeredMultipliers{configuration.getRegisteredMultipliers()};
			auto found = std::ranges::find_if(registeredMultipliers.begin(), registeredMultipliers.end(),
											  [](const MultiplierMeta &metadata) { return metadata.mName == "Span Multiplier"; });
			CHECK((found != registeredMultipliers.end()));
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity,llvm-prefer-static-over-anonymous-namespace)
