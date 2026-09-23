/*! @file setGrassyTerrainHandler.test.cpp
	@brief C++ file for running tests for the SetGrassyTerrainHandler.
	@date 09/23/2026
	@since 0.12.43
	@version 0.12.43
	@author Matthew Moore
*/

#include "EffectHandler/setGrassyTerrainHandler.h"

#include <algorithm>

#include "Battle/battleState.h"
#include "Effect/effectContext.h"
#include "Registry/registryProvider.h"
#include "Registry/registryProvider.testHelper.h"
#include "Terrain/builtInTerrainID.h"
#include "Terrain/terrainID.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Battle::BattleState;
using PocketCore::Effect::EffectContext;
using PocketCore::Effect::SetGrassyTerrainHandler;
using PocketCore::Registry::RegistryProvider;
using PocketCore::Terrain::BuiltinTerrainID;
using PocketCore::Terrain::TerrainID;
using PocketCore::Terrain::toTerrainID;
using PocketCore::Testing::getDefaultInitializedRegistryProvider;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

SCENARIO("SetGrassyTerrainHandler")
{
	SetGrassyTerrainHandler setGrassyTerrainHandler{};
	BattleState battleState{};
	EffectContext context{};
	RegistryProvider provider{getDefaultInitializedRegistryProvider()};

	TerrainID grassyID{toTerrainID(BuiltinTerrainID::Grassy)};
	TerrainID electricID{toTerrainID(BuiltinTerrainID::Electric)};

	GIVEN("no active terrains")
	{

		WHEN("grassy terrain is applied")
		{
			setGrassyTerrainHandler.apply(battleState, context, provider);

			THEN("grassy terrain becomes active")
			{
				CHECK(std::ranges::contains(battleState.mTerrainIDs, grassyID));
			}

			THEN("grassy terrain is only added once")
			{
				CHECK((std::ranges::count(battleState.mTerrainIDs, grassyID) == 1));
			}
		}
	}

	GIVEN("grassy terrain is already active")
	{
		battleState.mTerrainIDs = {grassyID};

		WHEN("grassy terrain is applied again")
		{
			setGrassyTerrainHandler.apply(battleState, context, provider);

			THEN("grassy terrain remains active exactly once")
			{
				CHECK((std::ranges::count(battleState.mTerrainIDs, grassyID) == 1));
			}
		}
	}

	GIVEN("a different terrain is active and replacement is allowed")
	{
		battleState.mTerrainIDs = {electricID};
		battleState.mRuleset.mMaxTerrains = 1;
		battleState.mRuleset.mReplaceTerrainWhenFull = true;

		WHEN("grassy terrain is applied")
		{
			setGrassyTerrainHandler.apply(battleState, context, provider);

			THEN("grassy terrain replaces the existing terrain")
			{
				CHECK(std::ranges::contains(battleState.mTerrainIDs, grassyID));
				CHECK_FALSE(std::ranges::contains(battleState.mTerrainIDs, electricID));
			}
		}
	}

	GIVEN("a different terrain is active and replacement is not allowed")
	{
		battleState.mTerrainIDs = {electricID};
		battleState.mRuleset.mMaxTerrains = 1;
		battleState.mRuleset.mReplaceTerrainWhenFull = false;

		WHEN("grassy terrain is applied")
		{
			setGrassyTerrainHandler.apply(battleState, context, provider);

			THEN("the existing terrain is retained and grassy terrain is not added")
			{
				CHECK(std::ranges::contains(battleState.mTerrainIDs, electricID));
				CHECK_FALSE(std::ranges::contains(battleState.mTerrainIDs, grassyID));
			}
		}
	}

	GIVEN("a free slot remains below the terrain cap")
	{
		battleState.mTerrainIDs = {electricID};
		battleState.mRuleset.mMaxTerrains = 2;
		battleState.mRuleset.mReplaceTerrainWhenFull = true;

		WHEN("grassy terrain is applied")
		{
			setGrassyTerrainHandler.apply(battleState, context, provider);

			THEN("grassy terrain is added alongside the existing terrain")
			{
				CHECK(std::ranges::contains(battleState.mTerrainIDs, electricID));
				CHECK(std::ranges::contains(battleState.mTerrainIDs, grassyID));
			}
		}
	}

	GIVEN("the ruleset permits no active terrains")
	{
		battleState.mRuleset.mMaxTerrains = 0;

		WHEN("grassy terrain is applied")
		{
			setGrassyTerrainHandler.apply(battleState, context, provider);

			THEN("no terrain becomes active")
			{
				CHECK_FALSE(std::ranges::contains(battleState.mTerrainIDs, grassyID));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
