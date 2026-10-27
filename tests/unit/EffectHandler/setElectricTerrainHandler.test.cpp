/*! @file setElectricTerrainHandler.test.cpp
	@brief C++ file for running tests for the SetElectricTerrainHandler.
	@date 10/27/2026
	@since 0.12.43
	@version 0.12.53
	@author Matthew Moore
*/

#include "EffectHandler/setElectricTerrainHandler.h"

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
using PocketCore::Effect::applySetElectric;
using PocketCore::Effect::EffectContext;
using PocketCore::Effect::SetElectricTerrainHandler;
using PocketCore::Registry::RegistryProvider;
using PocketCore::Terrain::BuiltinTerrainID;
using PocketCore::Terrain::TerrainID;
using PocketCore::Terrain::toTerrainID;
using PocketCore::Testing::getDefaultInitializedRegistryProvider;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

SCENARIO("SetElectricTerrainHandler")
{
	SetElectricTerrainHandler setElectricTerrainHandler{};
	RegistryProvider provider{getDefaultInitializedRegistryProvider()};
	BattleRandomSources randomSources{};

	TerrainID electricID{toTerrainID(BuiltinTerrainID::Electric)};
	TerrainID mistyID{toTerrainID(BuiltinTerrainID::Misty)};

	BattleState handlerState{};
	BattleState freeFunctionState{};
	EffectContext handlerContext{};
	EffectContext freeFunctionContext{};

	WHEN("applied to empty active terrains")
	{
		setElectricTerrainHandler.apply(handlerState, handlerContext, provider, randomSources);
		applySetElectric(freeFunctionState, freeFunctionContext, provider, randomSources);

		WHEN("electric terrain is applied")
		{
			CHECK((freeFunctionState.mTerrainIDs == handlerState.mTerrainIDs));
			CHECK(std::ranges::contains(freeFunctionState.mTerrainIDs, electricID));
		}
	}

	GIVEN("electric terrain is already active")
	{
		handlerState.mTerrainIDs = {electricID};
		freeFunctionState.mTerrainIDs = {electricID};

		setElectricTerrainHandler.apply(handlerState, handlerContext, provider, randomSources);
		applySetElectric(freeFunctionState, freeFunctionContext, provider, randomSources);

		THEN("both leave electric terrain active exactly once")
		{
			CHECK((freeFunctionState.mTerrainIDs == handlerState.mTerrainIDs));
			CHECK((std::ranges::count(freeFunctionState.mTerrainIDs, electricID) == 1));
		}
	}

	GIVEN("a different terrain is active and replacement is allowed")
	{
		handlerState.mTerrainIDs = {mistyID};
		handlerState.mRuleset.mMaxTerrains = 1;
		handlerState.mRuleset.mReplaceTerrainWhenFull = true;
		freeFunctionState.mTerrainIDs = {mistyID};
		freeFunctionState.mRuleset.mMaxTerrains = 1;
		freeFunctionState.mRuleset.mReplaceTerrainWhenFull = true;

		setElectricTerrainHandler.apply(handlerState, handlerContext, provider, randomSources);
		applySetElectric(freeFunctionState, freeFunctionContext, provider, randomSources);

		THEN("both replace the existing terrain identically")
		{
			CHECK((freeFunctionState.mTerrainIDs == handlerState.mTerrainIDs));
			CHECK(std::ranges::contains(freeFunctionState.mTerrainIDs, electricID));
			CHECK_FALSE(std::ranges::contains(freeFunctionState.mTerrainIDs, mistyID));
		}
	}

	WHEN("the ruleset permits no active terrains")
	{
		handlerState.mRuleset.mMaxTerrains = 0;
		freeFunctionState.mRuleset.mMaxTerrains = 0;

		setElectricTerrainHandler.apply(handlerState, handlerContext, provider, randomSources);
		applySetElectric(freeFunctionState, freeFunctionContext, provider, randomSources);

		THEN("neither activates a terrain")
		{
			CHECK((freeFunctionState.mTerrainIDs == handlerState.mTerrainIDs));
			CHECK_FALSE(std::ranges::contains(freeFunctionState.mTerrainIDs, electricID));
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
