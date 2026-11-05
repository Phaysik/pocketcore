/*! @file setGrassyTerrainHandler.test.cpp
	@brief C++ file for running tests for the SetGrassyTerrainHandler.
	@date 10/27/2026
	@since 0.12.43
	@version 0.12.53
	@author Matthew Moore
*/

#include "EffectHandler/setGrassyTerrainHandler.h"

#include <algorithm>

#include "Battle/battleRandomSources.h"
#include "Battle/battleState.h"
#include "Effect/effectContext.h"
#include "Registry/registryProvider.h"
#include "Registry/registryProvider.testHelper.h"
#include "Terrain/builtInTerrainID.h"
#include "Terrain/terrainID.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Battle::BattleRandomSources;
using PocketCore::Battle::BattleState;
using PocketCore::Effect::applySetGrassy;
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
	RegistryProvider provider{getDefaultInitializedRegistryProvider()};
	BattleRandomSources randomSources{};

	TerrainID grassyID{toTerrainID(BuiltinTerrainID::Grassy)};
	TerrainID electricID{toTerrainID(BuiltinTerrainID::Electric)};

	BattleState handlerState{};
	BattleState freeFunctionState{};
	EffectContext handlerContext{};
	EffectContext freeFunctionContext{};

	WHEN("applied to empty active terrains")
	{
		setGrassyTerrainHandler.apply(handlerState, handlerContext, provider, randomSources);
		applySetGrassy(freeFunctionState, freeFunctionContext, provider, randomSources);

		WHEN("grassy terrain is applied")
		{
			CHECK((freeFunctionState.mTerrainIDs == handlerState.mTerrainIDs));
			CHECK(std::ranges::contains(freeFunctionState.mTerrainIDs, grassyID));
		}
	}

	GIVEN("grassy terrain is already active")
	{
		handlerState.mTerrainIDs = {grassyID};
		freeFunctionState.mTerrainIDs = {grassyID};

		setGrassyTerrainHandler.apply(handlerState, handlerContext, provider, randomSources);
		applySetGrassy(freeFunctionState, freeFunctionContext, provider, randomSources);

		THEN("both leave grassy terrain active exactly once")
		{
			CHECK((freeFunctionState.mTerrainIDs == handlerState.mTerrainIDs));
			CHECK((std::ranges::count(freeFunctionState.mTerrainIDs, grassyID) == 1));
		}
	}

	WHEN("a different terrain is active and replacement is allowed")
	{
		handlerState.mTerrainIDs = {electricID};
		handlerState.mRuleset.mMaxTerrains = 1;
		handlerState.mRuleset.mReplaceTerrainWhenFull = true;
		freeFunctionState.mTerrainIDs = {electricID};
		freeFunctionState.mRuleset.mMaxTerrains = 1;
		freeFunctionState.mRuleset.mReplaceTerrainWhenFull = true;

		setGrassyTerrainHandler.apply(handlerState, handlerContext, provider, randomSources);
		applySetGrassy(freeFunctionState, freeFunctionContext, provider, randomSources);

		THEN("both replace the existing terrain identically")
		{
			CHECK((freeFunctionState.mTerrainIDs == handlerState.mTerrainIDs));
			CHECK(std::ranges::contains(freeFunctionState.mTerrainIDs, grassyID));
			CHECK_FALSE(std::ranges::contains(freeFunctionState.mTerrainIDs, electricID));
		}
	}

	WHEN("the ruleset permits no active terrains")
	{
		handlerState.mRuleset.mMaxTerrains = 0;
		freeFunctionState.mRuleset.mMaxTerrains = 0;

		setGrassyTerrainHandler.apply(handlerState, handlerContext, provider, randomSources);
		applySetGrassy(freeFunctionState, freeFunctionContext, provider, randomSources);

		THEN("neither activates a terrain")
		{
			CHECK((freeFunctionState.mTerrainIDs == handlerState.mTerrainIDs));
			CHECK_FALSE(std::ranges::contains(freeFunctionState.mTerrainIDs, grassyID));
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
