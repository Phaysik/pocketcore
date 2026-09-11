/*! @file terrainRegistry.test.cpp
	@brief C++ file for running tests for the TerrainRegistry.
	@date 09/10/2026
	@since 0.12.20
	@version 0.12.20
	@author Matthew Moore
*/

#include "Registry/terrainRegistry.h"

#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <utility>

#include "Core/typedefs.h"
#include "Terrain/builtInTerrainID.h"
#include "Terrain/constants.h"
#include "Terrain/terrainID.h"
#include "Terrain/terrainMeta.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Core::ub;
using PocketCore::Registry::Terrain::TerrainRegistry;
using PocketCore::Terrain::BuiltinTerrainID;
using PocketCore::Terrain::NO_TERRAIN_ID;
using PocketCore::Terrain::TERRAIN_NAME_ELECTRIC;
using PocketCore::Terrain::TERRAIN_NAME_GRASSY;
using PocketCore::Terrain::TERRAIN_NAME_MISTY;
using PocketCore::Terrain::TERRAIN_NAME_NONE;
using PocketCore::Terrain::TERRAIN_NAME_PSYCHIC;
using PocketCore::Terrain::TerrainID;
using PocketCore::Terrain::TerrainMeta;
using PocketCore::Terrain::toTerrainID;

template <typename Registry>
concept PubliclyStructurallyMutable = requires(Registry &registry) { registry.setAmountRegistered(0); };

static_assert(!PubliclyStructurallyMutable<TerrainRegistry>);

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("TerrainRegistry")
{
	TerrainRegistry registry{};
	ub finalTerrainUnderlyingValue{std::to_underlying(BuiltinTerrainID::FinalTerrain)};

	GIVEN("a default constructed terrain registry")
	{
		THEN("Electric has the appropriate properties")
		{
			TerrainMeta expected{
				.mName = std::string(TERRAIN_NAME_ELECTRIC),
				.mTerrainID = toTerrainID(BuiltinTerrainID::Electric),
			};

			const TerrainMeta *actual{registry.getTerrainMetadata(toTerrainID(BuiltinTerrainID::Electric))};

			CHECK((expected == *actual));
		}

		THEN("Grassy has the appropriate properties")
		{
			TerrainMeta expected{
				.mName = std::string(TERRAIN_NAME_GRASSY),
				.mTerrainID = toTerrainID(BuiltinTerrainID::Grassy),
			};

			const TerrainMeta *actual{registry.getTerrainMetadata(toTerrainID(BuiltinTerrainID::Grassy))};

			CHECK((expected == *actual));
		}

		THEN("Misty has the appropriate properties")
		{
			TerrainMeta expected{
				.mName = std::string(TERRAIN_NAME_MISTY),
				.mTerrainID = toTerrainID(BuiltinTerrainID::Misty),
			};

			const TerrainMeta *actual{registry.getTerrainMetadata(toTerrainID(BuiltinTerrainID::Misty))};

			CHECK((expected == *actual));
		}

		THEN("Psychic has the appropriate properties")
		{
			TerrainMeta expected{
				.mName = std::string(TERRAIN_NAME_PSYCHIC),
				.mTerrainID = toTerrainID(BuiltinTerrainID::Psychic),
			};

			const TerrainMeta *actual{registry.getTerrainMetadata(toTerrainID(BuiltinTerrainID::Psychic))};

			CHECK((expected == *actual));
		}
	}

	GIVEN("getTerrainMetadata")
	{
		THEN("unknown IDs are absent")
		{
			CHECK((registry.getTerrainMetadata(TerrainID{200}) == nullptr));
		}

		THEN("the metadata is retrieved when accessed by a valid Terrain ID")
		{
			TerrainMeta expected{
				.mName = std::string(TERRAIN_NAME_NONE),
				.mTerrainID = toTerrainID(BuiltinTerrainID::None),
			};

			CHECK((expected == *registry.getTerrainMetadata(NO_TERRAIN_ID)));
		}
	}

	GIVEN("getTerrainID")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(registry.getTerrainID("Unknown").has_value());
		}

		THEN("the Terrain ID is retrieved by valid Terrain name")
		{
			std::optional<TerrainID> terrainID{registry.getTerrainID(TERRAIN_NAME_NONE)};

			REQUIRE(terrainID.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((terrainID.value() == toTerrainID(BuiltinTerrainID::None)));
		}
	}

	GIVEN("getTerrainName")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(registry.getTerrainName(TerrainID{200}).has_value());
		}

		THEN("a registered terrain name is returned by stable ID")
		{
			std::optional<std::string_view> terrainName{registry.getTerrainName(toTerrainID(BuiltinTerrainID::None))};

			REQUIRE(terrainName.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((terrainName.value() == TERRAIN_NAME_NONE));
		}
	}

	GIVEN("getAmountRegistered")
	{
		THEN("the registered span contains the exact amount of built-in entries")
		{
			CHECK((registry.getAmountRegistered() == finalTerrainUnderlyingValue));
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
			TerrainMeta expected{
				.mName = std::string(TERRAIN_NAME_NONE),
				.mTerrainID = toTerrainID(BuiltinTerrainID::None),
			};

			const TerrainMeta *terrainMeta{registry.getEntry(0)};

			REQUIRE((terrainMeta != nullptr));
			CHECK((*terrainMeta == expected));
		}
	}

	GIVEN("getRegisteredTerrains")
	{
		THEN("the amount of terrains returned matches the amount that are built-in")
		{
			CHECK((registry.getRegisteredTerrains().size() == finalTerrainUnderlyingValue));
		}
	}

	GIVEN("getNextTerrainID")
	{
		THEN("the next available stable Terrain ID is after all built in terrain IDs")
		{
			CHECK((registry.getNextTerrainID() == finalTerrainUnderlyingValue));
		}
	}

	GIVEN("findIndexByTerrainID")
	{
		THEN("an unknown stable ID has no internal index")
		{
			std::optional<ub> terrainIndex{registry.findIndexByTerrainID(TerrainID{200})};
			CHECK_FALSE(terrainIndex.has_value());
		}

		THEN("the internal array index is retrieved by valid Terrain ID")
		{
			std::optional<ub> terrainIndex{registry.findIndexByTerrainID(NO_TERRAIN_ID)};

			REQUIRE(terrainIndex.has_value());
			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((terrainIndex.value() == 0));
		}
	}

	GIVEN("hasTerrain")
	{
		WHEN("calling the string_view overload")
		{
			THEN("an unknown terrain name has no entry")
			{
				CHECK_FALSE(registry.hasTerrain("Unknown"));
			}

			THEN("a known terrain name has an entry")
			{
				CHECK(registry.hasTerrain(TERRAIN_NAME_NONE));
			}
		}

		WHEN("calling the TerrainID overload")
		{
			THEN("an unknown terrain ID has no entry")
			{
				CHECK_FALSE(registry.hasTerrain(TerrainID{200}));
			}

			THEN("a known terrain ID has an entry")
			{
				CHECK(registry.hasTerrain(NO_TERRAIN_ID));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
