/*! @file terrainMeta.test.cpp
	@brief C++ file for running tests for the TerrainMeta.
	@date 09/23/2026
	@since 0.12.43
	@version 0.12.43
	@author Matthew Moore
*/

#include "Terrain/terrainMeta.h"

#include "Interaction/interaction.h"
#include "Terrain/builtInTerrainID.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Interaction::InteractionAction;
using PocketCore::Terrain::BuiltinTerrainID;
using PocketCore::Terrain::TerrainMeta;
using PocketCore::Terrain::toTerrainID;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("TerrainMeta")
{
	TerrainMeta meta{};

	WHEN("operator==")
	{
		GIVEN("two default constructed metas")
		{
			TerrainMeta other{};

			THEN("they are equal")
			{
				CHECK((meta == other));
			}
		}

		GIVEN("for mName modified in one meta")
		{
			TerrainMeta other{.mName = "Test"};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mTerrainInteractions modified in one meta")
		{
			TerrainMeta other{
				.mTerrainInteractions
				= {{.mExistingID = toTerrainID(BuiltinTerrainID::Grassy), .mAction = InteractionAction::BlockIncoming}},
			};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mTerrainID modified in one meta")
		{
			TerrainMeta other{.mTerrainID = toTerrainID(BuiltinTerrainID::Misty)};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("two metas modified the same way")
		{
			TerrainMeta other{.mName = "Same Name"};
			meta.mName = "Same Name";

			THEN("they are equal")
			{
				CHECK((meta == other));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
