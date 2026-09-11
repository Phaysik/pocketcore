/*! @file terrainRegistryConfiguration.test.cpp
	@brief C++ file for running tests for the TerrainRegistryConfiguration.
	@date 09/10/2026
	@since 0.8.7
	@version 0.12.20
	@author Matthew Moore
*/

#include "Configuration/terrainRegistryConfiguration.h"

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
#include "Registry/terrainRegistry.h"
#include "Terrain/builtInTerrainID.h"
#include "Terrain/constants.h"
#include "Terrain/terrainID.h"
#include "Terrain/terrainMeta.h"
#include "Utility/Debug/Logging/logging.testHelper.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Configuration::MAX_TERRAINS;
using PocketCore::Configuration::RegistryError;
using PocketCore::Configuration::TerrainRegistryConfiguration;
using PocketCore::Core::ub;
using PocketCore::Core::us;
using PocketCore::Registry::RegistryErrorInfo;
using PocketCore::Registry::Terrain::TerrainRegistry;
using PocketCore::Terrain::BuiltinTerrainID;
using PocketCore::Terrain::NO_TERRAIN_ID;
using PocketCore::Terrain::TERRAIN_NAME_ELECTRIC;
using PocketCore::Terrain::TERRAIN_NAME_NONE;
using PocketCore::Terrain::TerrainID;
using PocketCore::Terrain::TerrainMeta;
using PocketCore::Terrain::toTerrainID;
using PocketCore::Testing::ensureLoggerInitialized;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("TerrainRegistryConfiguration")
{
	ensureLoggerInitialized("terrain registry configuration test", "terrainRegistryConfiguration_test.log");

	TerrainRegistryConfiguration config{};
	TerrainRegistry registry{};
	ub finalTerrainUnderlyingValue{std::to_underlying(BuiltinTerrainID::FinalTerrain)};

	GIVEN("getAmountRegistered")
	{
		THEN("the default registry returns the expected amount of built-in terrains")
		{
			CHECK((config.getAmountRegistered() == finalTerrainUnderlyingValue));
		}
	}

	GIVEN("getRuntimeRegistry")
	{
		THEN("the runtime registry is the same as the default registry")
		{
			CHECK((config.getRuntimeRegistry() == registry));
		}
	}

	GIVEN("getTerrainMetadata")
	{
		THEN("unknown IDs are absent")
		{
			CHECK((config.getTerrainMetadata(TerrainID{200}) == nullptr));
		}

		THEN("the metadata is retrieved when accessed by a valid Terrain ID")
		{
			TerrainMeta expected{
				.mName = std::string(TERRAIN_NAME_NONE),
				.mTerrainID = toTerrainID(BuiltinTerrainID::None),
			};

			CHECK((expected == *config.getTerrainMetadata(NO_TERRAIN_ID)));
		}
	}

	GIVEN("getTerrainID")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(config.getTerrainID("Unknown").has_value());
		}

		THEN("the Terrain ID is retrieved by valid Terrain name")
		{
			std::optional<TerrainID> terrainID{config.getTerrainID(TERRAIN_NAME_NONE)};

			REQUIRE(terrainID.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((terrainID.value() == toTerrainID(BuiltinTerrainID::None)));
		}
	}

	GIVEN("getTerrainName")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(config.getTerrainName(TerrainID{200}).has_value());
		}

		THEN("a registered terrain name is returned by stable ID")
		{
			std::optional<std::string_view> terrainName{config.getTerrainName(toTerrainID(BuiltinTerrainID::None))};

			REQUIRE(terrainName.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((terrainName.value() == TERRAIN_NAME_NONE));
		}
	}

	GIVEN("getRegisteredTerrains")
	{
		THEN("the amount of terrains returned matches the amount that are built-in")
		{
			CHECK((config.getRegisteredTerrains().size() == finalTerrainUnderlyingValue));
		}
	}

	GIVEN("hasTerrain")
	{
		WHEN("calling the string_view overload")
		{
			THEN("an unknown terrain name has no entry")
			{
				CHECK_FALSE(config.hasTerrain("Unknown"));
			}

			THEN("a known terrain name has an entry")
			{
				CHECK(config.hasTerrain(TERRAIN_NAME_NONE));
			}
		}

		WHEN("calling the TerrainID overload")
		{
			THEN("an unknown terrain ID has no entry")
			{
				CHECK_FALSE(config.hasTerrain(TerrainID{200}));
			}

			THEN("a known terrain ID has an entry")
			{
				CHECK(config.hasTerrain(NO_TERRAIN_ID));
			}
		}
	}

	GIVEN("addTerrain")
	{
		WHEN("trying to add an terrain past the capacity")
		{
			us newTerrainCount{finalTerrainUnderlyingValue};

			for (us i{0}; i < MAX_TERRAINS - finalTerrainUnderlyingValue; ++i)
			{
				std::string name{std::format("String_{:04}", i)};
				TerrainMeta definition{.mName = name};
				std::expected<TerrainID, RegistryErrorInfo> result{config.addTerrain(definition)};

				REQUIRE(result.has_value());

				TerrainID assignedID{result.value()};
				CHECK((assignedID.getValue() == newTerrainCount++));
			}

			std::string name{std::format("String_{:04}", MAX_TERRAINS + 1)};

			TerrainMeta definition{.mName = name};
			std::expected<TerrainID, RegistryErrorInfo> result{config.addTerrain(definition)};

			THEN("registration reports a max capacity and nothing is added")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::MaxCapacity));
				CHECK((config.getAmountRegistered() == MAX_TERRAINS));
			}
		}

		WHEN("an terrain whose name is already in use is added")
		{
			TerrainMeta definition{.mName = std::string(TERRAIN_NAME_ELECTRIC)};
			std::expected<TerrainID, RegistryErrorInfo> result{config.addTerrain(definition)};

			THEN("registration reports a duplicate terrain and nothing is added")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::DuplicateTerrain));
				CHECK((config.getAmountRegistered() == finalTerrainUnderlyingValue));
			}
		}

		WHEN("a unique terrain definition is added")
		{
			TerrainMeta definition{.mName = "TestTerrainName"};

			std::expected<TerrainID, RegistryErrorInfo> result{config.addTerrain(definition)};

			THEN("it receives the first custom stable ID and owns its trigger data")
			{
				REQUIRE(result.has_value());
				TerrainID assignedID{result.value()};
				CHECK((assignedID.getValue() == finalTerrainUnderlyingValue));

				const TerrainMeta *metadata{config.getTerrainMetadata(assignedID)};

				REQUIRE((metadata != nullptr));
				CHECK((metadata->mName == "TestTerrainName"));
			}
		}
	}

	GIVEN("addTerrains")
	{
		WHEN("trying to add an terrain past the capacity")
		{
			std::vector<TerrainMeta> terrainMetas;

			for (us i{0}; i < MAX_TERRAINS - finalTerrainUnderlyingValue; ++i)
			{
				std::string name{std::format("String_{:04}", i)};
				terrainMetas.push_back({.mName = name});
			}

			std::string name{std::format("String_{:04}", MAX_TERRAINS + 1)};

			terrainMetas.push_back({.mName = name});
			std::expected<void, RegistryErrorInfo> result{config.addTerrains(terrainMetas)};

			THEN("registration reports a max capacity and nothing is added")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::MaxCapacity));
				CHECK((config.getAmountRegistered() == finalTerrainUnderlyingValue));
			}
		}

		WHEN("an terrain whose name is already in use is added")
		{
			std::vector<TerrainMeta> terrainMetas{};

			for (us i{0}; i < 20; ++i)
			{
				std::string name{std::format("String_{:04}", i)};
				terrainMetas.push_back({.mName = name});
			}

			terrainMetas.push_back({.mName = std::string(TERRAIN_NAME_ELECTRIC)});
			std::expected<void, RegistryErrorInfo> result{config.addTerrains(terrainMetas)};

			THEN("registration reports a duplicate terrain and the registry is rollback to the checkpoint before the erroneous addition")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::DuplicateTerrain));
				CHECK((config.getAmountRegistered() == finalTerrainUnderlyingValue));
			}
		}

		WHEN("a unique terrain definition is added")
		{
			std::vector<TerrainMeta> terrainMetas{};

			terrainMetas.push_back({.mName = "TestTerrainName"});

			std::expected<void, RegistryErrorInfo> result{config.addTerrains(terrainMetas)};

			THEN("the registry reports no error and all the terrain definitions are added")
			{
				REQUIRE(result.has_value());
				CHECK((config.getAmountRegistered() == finalTerrainUnderlyingValue + 1));
			}
		}
	}

	GIVEN("renameTerrain")
	{
		WHEN("calling with an invalid terrain name")
		{
			std::expected<void, RegistryErrorInfo> result{config.renameTerrain("ThisIsInvalid", "NewName")};

			THEN("the registry reports an error and there is no update to the registry")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::TerrainNotFound));
				CHECK((config.getAmountRegistered() == finalTerrainUnderlyingValue));
			}
		}

		WHEN("trying to rename to a name already in the registry")
		{
			std::expected<void, RegistryErrorInfo> result{
				config.renameTerrain(TERRAIN_NAME_NONE, TERRAIN_NAME_ELECTRIC),
			};

			THEN("the registry reports an error and there is no update to the registry")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::DuplicateTerrain));
				CHECK((config.getAmountRegistered() == finalTerrainUnderlyingValue));
			}
		}

		WHEN("updating an existing terrain definition")
		{
			std::expected<void, RegistryErrorInfo> result{config.renameTerrain(TERRAIN_NAME_NONE, "NewName")};

			THEN("the registry reports no error and the name is appropriately updated")
			{
				REQUIRE(result.has_value());
				CHECK((config.getAmountRegistered() == finalTerrainUnderlyingValue));
				CHECK((config.getTerrainMetadata(toTerrainID(BuiltinTerrainID::None))->mName == "NewName"));
			}
		}
	}

	GIVEN("updateTerrain")
	{
		TerrainMeta definition{.mName = "TestTerrainName"};

		WHEN("calling the string_view overload")
		{
			WHEN("calling with an invalid terrain name")
			{
				std::expected<void, RegistryErrorInfo> result{config.updateTerrain("ThisIsInvalid", definition)};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::TerrainNotFound));
					CHECK((config.getAmountRegistered() == finalTerrainUnderlyingValue));
				}
			}

			WHEN("trying to rename to a name already in the registry")
			{
				std::expected<void, RegistryErrorInfo> result{
					config.updateTerrain(TERRAIN_NAME_NONE, {.mName = std::string(TERRAIN_NAME_ELECTRIC)}),
				};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::DuplicateTerrain));
					CHECK((config.getAmountRegistered() == finalTerrainUnderlyingValue));
				}
			}

			WHEN("updating an existing terrain definition")
			{
				std::expected<void, RegistryErrorInfo> result{config.updateTerrain(TERRAIN_NAME_NONE, definition)};

				THEN("the registry reports no error and the target is appropriately updated")
				{
					REQUIRE(result.has_value());
					CHECK((config.getAmountRegistered() == finalTerrainUnderlyingValue));
					CHECK((config.getTerrainMetadata(toTerrainID(BuiltinTerrainID::None))->mName == "TestTerrainName"));
				}
			}
		}

		WHEN("calling the TerrainID overload")
		{
			WHEN("calling with an invalid terrain name")
			{
				std::expected<void, RegistryErrorInfo> result{config.updateTerrain(TerrainID{200}, definition)};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::TerrainNotFound));
					CHECK((config.getAmountRegistered() == finalTerrainUnderlyingValue));
				}
			}

			WHEN("trying to rename to a name already in the registry")
			{
				std::expected<void, RegistryErrorInfo> result{
					config.updateTerrain(toTerrainID(BuiltinTerrainID::None), {.mName = std::string(TERRAIN_NAME_ELECTRIC)}),
				};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::DuplicateTerrain));
					CHECK((config.getAmountRegistered() == finalTerrainUnderlyingValue));
				}
			}

			WHEN("updating an existing terrain definition")
			{
				std::expected<void, RegistryErrorInfo> result{config.updateTerrain(toTerrainID(BuiltinTerrainID::None), definition)};

				THEN("the registry reports no error and the target is appropriately updated")
				{
					REQUIRE(result.has_value());
					CHECK((config.getAmountRegistered() == finalTerrainUnderlyingValue));
					CHECK((config.getTerrainMetadata(toTerrainID(BuiltinTerrainID::None))->mName == "TestTerrainName"));
				}
			}
		}
	}

	GIVEN("removeTerrain")
	{
		WHEN("calling the string_view overload")
		{
			WHEN("calling with an unknown terrain name")
			{
				std::expected<TerrainID, RegistryErrorInfo> result{config.removeTerrain("Unknown")};

				THEN("the result is an error and the registry is not updated")
				{
					CHECK_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::TerrainNotFound));
					CHECK((config.getAmountRegistered() == finalTerrainUnderlyingValue));
				}
			}

			WHEN("calling with a known terrain name")
			{
				std::expected<TerrainID, RegistryErrorInfo> result{config.removeTerrain(TERRAIN_NAME_NONE)};

				THEN("the result is a success and the registry is updated")
				{
					CHECK(result.has_value());
					CHECK((result.value() == toTerrainID(BuiltinTerrainID::None)));
					CHECK((config.getAmountRegistered() == finalTerrainUnderlyingValue - 1));
				}
			}
		}

		WHEN("calling the TerrainID overload")
		{
			WHEN("calling with an unknown terrain ID")
			{
				std::expected<TerrainID, RegistryErrorInfo> result{config.removeTerrain(TerrainID{200})};

				THEN("the result is an error and the registry is not updated")
				{
					CHECK_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::TerrainNotFound));
					CHECK((config.getAmountRegistered() == finalTerrainUnderlyingValue));
				}
			}

			WHEN("calling with a known terrain ID")
			{
				std::expected<TerrainID, RegistryErrorInfo> result{config.removeTerrain(toTerrainID(BuiltinTerrainID::None))};

				THEN("the result is a success and the registry is updated")
				{
					CHECK(result.has_value());
					CHECK((result.value() == toTerrainID(BuiltinTerrainID::None)));
					CHECK((config.getAmountRegistered() == finalTerrainUnderlyingValue - 1));
				}
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
