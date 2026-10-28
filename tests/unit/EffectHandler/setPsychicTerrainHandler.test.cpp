/*! @file setPsychicTerrainHandler.test.cpp
	@brief C++ file for running tests for the SetPsychicTerrainHandler.
	@date 10/27/2026
	@since 0.12.43
	@version 0.12.53
	@author Matthew Moore
*/

#include "EffectHandler/setPsychicTerrainHandler.h"

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
using PocketCore::Effect::applySetPsychic;
using PocketCore::Effect::EffectContext;
using PocketCore::Effect::SetPsychicTerrainHandler;
using PocketCore::Registry::RegistryProvider;
using PocketCore::Terrain::BuiltinTerrainID;
using PocketCore::Terrain::TerrainID;
using PocketCore::Terrain::toTerrainID;
using PocketCore::Testing::getDefaultInitializedRegistryProvider;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

SCENARIO("SetPsychicTerrainHandler")
{
	SetPsychicTerrainHandler setPsychicTerrainHandler{};
	RegistryProvider provider{getDefaultInitializedRegistryProvider()};
	BattleRandomSources randomSources{};

	TerrainID psychicID{toTerrainID(BuiltinTerrainID::Psychic)};
	TerrainID electricID{toTerrainID(BuiltinTerrainID::Electric)};

	BattleState handlerState{};
	BattleState freeFunctionState{};
	EffectContext handlerContext{};
	EffectContext freeFunctionContext{};

	WHEN("applied to empty active terrains")
	{
		setPsychicTerrainHandler.apply(handlerState, handlerContext, provider, randomSources);
		applySetPsychic(freeFunctionState, freeFunctionContext, provider, randomSources);

		WHEN("psychic terrain is applied")
		{
			CHECK((freeFunctionState.mTerrainIDs == handlerState.mTerrainIDs));
			CHECK(std::ranges::contains(freeFunctionState.mTerrainIDs, psychicID));
		}
	}

	WHEN("psychic terrain is already active")
	{
		handlerState.mTerrainIDs = {psychicID};
		freeFunctionState.mTerrainIDs = {psychicID};

		setPsychicTerrainHandler.apply(handlerState, handlerContext, provider, randomSources);
		applySetPsychic(freeFunctionState, freeFunctionContext, provider, randomSources);

		THEN("both leave psychic terrain active exactly once")
		{
			CHECK((freeFunctionState.mTerrainIDs == handlerState.mTerrainIDs));
			CHECK((std::ranges::count(freeFunctionState.mTerrainIDs, psychicID) == 1));
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

		setPsychicTerrainHandler.apply(handlerState, handlerContext, provider, randomSources);
		applySetPsychic(freeFunctionState, freeFunctionContext, provider, randomSources);

		THEN("both replace the existing terrain identically")
		{
			CHECK((freeFunctionState.mTerrainIDs == handlerState.mTerrainIDs));
			CHECK(std::ranges::contains(freeFunctionState.mTerrainIDs, psychicID));
			CHECK_FALSE(std::ranges::contains(freeFunctionState.mTerrainIDs, electricID));
		}
	}

	WHEN("the ruleset permits no active terrains")
	{
		handlerState.mRuleset.mMaxTerrains = 0;
		freeFunctionState.mRuleset.mMaxTerrains = 0;

		setPsychicTerrainHandler.apply(handlerState, handlerContext, provider, randomSources);
		applySetPsychic(freeFunctionState, freeFunctionContext, provider, randomSources);

		THEN("neither activates a terrain")
		{
			CHECK((freeFunctionState.mTerrainIDs == handlerState.mTerrainIDs));
			CHECK_FALSE(std::ranges::contains(freeFunctionState.mTerrainIDs, psychicID));
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
