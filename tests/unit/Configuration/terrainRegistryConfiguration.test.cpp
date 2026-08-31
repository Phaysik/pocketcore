/*! @file terrainRegistryConfiguration.test.cpp
	@brief C++ file for running tests for the TerrainRegistryConfiguration.
	@date 08/30/2026
	@since 0.8.7
	@version 0.12.12
	@author Matthew Moore
*/

#include "Configuration/terrainRegistryConfiguration.h"

#include <algorithm>
#include <array>
#include <string_view>

#include "Registry/registryError.h"
#include "Terrain/terrainID.h"
#include "Terrain/terrainMeta.h"
#include "Utility/Debug/Logging/logger.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Configuration::RegistryError;
using PocketCore::Configuration::TerrainRegistryConfiguration;
using PocketCore::Terrain::TerrainID;
using PocketCore::Terrain::TerrainMeta;
using PocketCore::Utility::Debug::Logging::Logger;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity,llvm-prefer-static-over-anonymous-namespace)

namespace
{
	void ensureTerrainLoggerInitialized()
	{
		static bool initialized{false};

		if (!initialized)
		{
			initialized = Logger::initialize("trc_test", "terrainRegistryConfiguration_test.log", true);
		}
	}

	TerrainMeta makeTerrain(const std::string_view &name)
	{
		return TerrainMeta{.mName = name};
	}
} // namespace

SCENARIO("TerrainRegistryConfiguration addTerrain")
{
	ensureTerrainLoggerInitialized();
	TerrainRegistryConfiguration configuration{};

	GIVEN("a unique terrain definition")
	{
		auto result{configuration.addTerrain(makeTerrain("Custom Terrain"))};

		THEN("it is registered and queryable")
		{
			REQUIRE(result.has_value());
			TerrainID customIdentifier{result.value()};
			CHECK(configuration.hasTerrain(customIdentifier));
			CHECK(configuration.hasTerrain("Custom Terrain"));

			auto nameResult{configuration.getTerrainName(customIdentifier)};
			REQUIRE(nameResult.has_value());
			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((nameResult.value() == "Custom Terrain"));

			const TerrainMeta *metadata{configuration.getTerrainMetadata(customIdentifier)};
			REQUIRE((metadata != nullptr));
			CHECK((metadata->mName == "Custom Terrain"));
		}
	}

	GIVEN("a duplicate terrain name")
	{
		auto firstResult{configuration.addTerrain(makeTerrain("Duplicate Terrain"))};
		REQUIRE(firstResult.has_value());

		THEN("registration returns DuplicateTerrain")
		{
			auto secondResult{configuration.addTerrain(makeTerrain("Duplicate Terrain"))};
			REQUIRE_FALSE(secondResult.has_value());
			CHECK((secondResult.error().mKind == RegistryError::DuplicateTerrain));
		}
	}
}

SCENARIO("TerrainRegistryConfiguration addTerrains")
{
	ensureTerrainLoggerInitialized();
	TerrainRegistryConfiguration configuration{};

	GIVEN("a batch with duplicate names")
	{
		std::array<TerrainMeta, 3> definitions{
			{
				makeTerrain("Batch One"),
				makeTerrain("Batch Two"),
				makeTerrain("Batch One"),
			},
		};

		WHEN("the batch is added")
		{
			auto result{configuration.addTerrains(definitions)};

			THEN("all additions are rolled back")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::DuplicateTerrain));
				CHECK_FALSE(configuration.hasTerrain("Batch One"));
				CHECK_FALSE(configuration.hasTerrain("Batch Two"));
			}
		}
	}
}

SCENARIO("TerrainRegistryConfiguration metadata lifecycle")
{
	ensureTerrainLoggerInitialized();
	TerrainRegistryConfiguration configuration{};

	GIVEN("a registered custom terrain")
	{
		auto addResult{configuration.addTerrain(makeTerrain("Custom Terrain"))};
		REQUIRE(addResult.has_value());
		TerrainID customIdentifier{addResult.value()};

		WHEN("it is renamed")
		{
			auto renameResult{configuration.renameTerrain("Custom Terrain", "Renamed Terrain")};

			THEN("the stable ID remains associated with renamed metadata")
			{
				REQUIRE(renameResult.has_value());
				CHECK_FALSE(configuration.hasTerrain("Custom Terrain"));
				CHECK(configuration.hasTerrain("Renamed Terrain"));

				auto idResult{configuration.getTerrainID("Renamed Terrain")};
				REQUIRE(idResult.has_value());
				// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
				CHECK((idResult.value() == customIdentifier));
			}
		}

		WHEN("it is updated by name")
		{
			auto updateResult{configuration.updateTerrain("Custom Terrain", makeTerrain("Replacement Terrain"))};

			THEN("replacement metadata is discoverable")
			{
				REQUIRE(updateResult.has_value());
				CHECK(configuration.hasTerrain("Replacement Terrain"));
				CHECK_FALSE(configuration.hasTerrain("Custom Terrain"));
			}
		}

		WHEN("it is updated by stable ID")
		{
			auto updateResult{configuration.updateTerrain(customIdentifier, makeTerrain("Replacement By ID"))};

			THEN("replacement metadata is discoverable")
			{
				REQUIRE(updateResult.has_value());
				CHECK(configuration.hasTerrain("Replacement By ID"));
			}
		}

		WHEN("it is removed by name")
		{
			auto removeResult{configuration.removeTerrain("Custom Terrain")};

			THEN("the removed ID is returned")
			{
				REQUIRE(removeResult.has_value());
				CHECK((removeResult.value() == customIdentifier));
				CHECK_FALSE(configuration.hasTerrain(customIdentifier));
			}
		}

		WHEN("it is removed then another entry is added")
		{
			auto removeResult{configuration.removeTerrain(customIdentifier)};
			REQUIRE(removeResult.has_value());

			auto laterResult{configuration.addTerrain(makeTerrain("Later Terrain"))};

			THEN("the removed ID is not reused")
			{
				REQUIRE(laterResult.has_value());
				CHECK((laterResult.value() != customIdentifier));
				CHECK((configuration.getTerrainMetadata(customIdentifier) == nullptr));
			}
		}
	}

	GIVEN("an unknown terrain")
	{
		THEN("rename update and remove report TerrainNotFound")
		{
			auto renameResult{configuration.renameTerrain("Missing", "Renamed")};
			auto updateResult{configuration.updateTerrain("Missing", makeTerrain("Updated"))};
			auto removeResult{configuration.removeTerrain("Missing")};

			REQUIRE_FALSE(renameResult.has_value());
			REQUIRE_FALSE(updateResult.has_value());
			REQUIRE_FALSE(removeResult.has_value());
			CHECK((renameResult.error().mKind == RegistryError::TerrainNotFound));
			CHECK((updateResult.error().mKind == RegistryError::TerrainNotFound));
			CHECK((removeResult.error().mKind == RegistryError::TerrainNotFound));
		}
	}
}

SCENARIO("TerrainRegistryConfiguration registered span")
{
	ensureTerrainLoggerInitialized();
	TerrainRegistryConfiguration configuration{};

	GIVEN("a custom registration")
	{
		auto addResult{configuration.addTerrain(makeTerrain("Span Terrain"))};
		REQUIRE(addResult.has_value());

		THEN("registered span contains the new name")
		{
			const auto registeredTerrains{configuration.getRegisteredTerrains()};
			auto found = std::ranges::find_if(registeredTerrains.begin(), registeredTerrains.end(),
											  [](const TerrainMeta &metadata) { return metadata.mName == "Span Terrain"; });
			CHECK((found != registeredTerrains.end()));
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity,llvm-prefer-static-over-anonymous-namespace)
