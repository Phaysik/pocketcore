/*! @file setPsychicTerrainHandler.test.cpp
	@brief C++ file for running tests for the SetPsychicTerrainHandler.
	@date 09/23/2026
	@since 0.12.43
	@version 0.12.43
	@author Matthew Moore
*/

#include "EffectHandler/setPsychicTerrainHandler.h"

#include <algorithm>

#include "Battle/battleState.h"
#include "Effect/effectContext.h"
#include "Registry/registryProvider.h"
#include "Registry/registryProvider.testHelper.h"
#include "Terrain/builtInTerrainID.h"
#include "Terrain/terrainID.h"

#include <catch2/catch_test_macros.hpp>

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
	BattleState battleState{};
	EffectContext context{};
	RegistryProvider provider{getDefaultInitializedRegistryProvider()};

	TerrainID psychicID{toTerrainID(BuiltinTerrainID::Psychic)};
	TerrainID electricID{toTerrainID(BuiltinTerrainID::Electric)};

	GIVEN("no active terrains")
	{

		WHEN("psychic terrain is applied")
		{
			setPsychicTerrainHandler.apply(battleState, context, provider);

			THEN("psychic terrain becomes active")
			{
				CHECK(std::ranges::contains(battleState.mTerrainIDs, psychicID));
			}

			THEN("psychic terrain is only added once")
			{
				CHECK((std::ranges::count(battleState.mTerrainIDs, psychicID) == 1));
			}
		}
	}

	GIVEN("psychic terrain is already active")
	{
		battleState.mTerrainIDs = {psychicID};

		WHEN("psychic terrain is applied again")
		{
			setPsychicTerrainHandler.apply(battleState, context, provider);

			THEN("psychic terrain remains active exactly once")
			{
				CHECK((std::ranges::count(battleState.mTerrainIDs, psychicID) == 1));
			}
		}
	}

	GIVEN("a different terrain is active and replacement is allowed")
	{
		battleState.mTerrainIDs = {electricID};
		battleState.mRuleset.mMaxTerrains = 1;
		battleState.mRuleset.mReplaceTerrainWhenFull = true;

		WHEN("psychic terrain is applied")
		{
			setPsychicTerrainHandler.apply(battleState, context, provider);

			THEN("psychic terrain replaces the existing terrain")
			{
				CHECK(std::ranges::contains(battleState.mTerrainIDs, psychicID));
				CHECK_FALSE(std::ranges::contains(battleState.mTerrainIDs, electricID));
			}
		}
	}

	GIVEN("a different terrain is active and replacement is not allowed")
	{
		battleState.mTerrainIDs = {electricID};
		battleState.mRuleset.mMaxTerrains = 1;
		battleState.mRuleset.mReplaceTerrainWhenFull = false;

		WHEN("psychic terrain is applied")
		{
			setPsychicTerrainHandler.apply(battleState, context, provider);

			THEN("the existing terrain is retained and psychic terrain is not added")
			{
				CHECK(std::ranges::contains(battleState.mTerrainIDs, electricID));
				CHECK_FALSE(std::ranges::contains(battleState.mTerrainIDs, psychicID));
			}
		}
	}

	GIVEN("a free slot remains below the terrain cap")
	{
		battleState.mTerrainIDs = {electricID};
		battleState.mRuleset.mMaxTerrains = 2;
		battleState.mRuleset.mReplaceTerrainWhenFull = true;

		WHEN("psychic terrain is applied")
		{
			setPsychicTerrainHandler.apply(battleState, context, provider);

			THEN("psychic terrain is added alongside the existing terrain")
			{
				CHECK(std::ranges::contains(battleState.mTerrainIDs, electricID));
				CHECK(std::ranges::contains(battleState.mTerrainIDs, psychicID));
			}
		}
	}

	GIVEN("the ruleset permits no active terrains")
	{
		battleState.mRuleset.mMaxTerrains = 0;

		WHEN("psychic terrain is applied")
		{
			setPsychicTerrainHandler.apply(battleState, context, provider);

			THEN("no terrain becomes active")
			{
				CHECK_FALSE(std::ranges::contains(battleState.mTerrainIDs, psychicID));
			}
		}
	}

	GIVEN("the free function and the handler operate on equivalent states")
	{
		BattleState handlerState{};
		BattleState freeFunctionState{};
		EffectContext handlerContext{};
		EffectContext freeFunctionContext{};

		WHEN("applied to empty active terrains")
		{
			setPsychicTerrainHandler.apply(handlerState, handlerContext, provider);
			applySetPsychic(freeFunctionState, freeFunctionContext, provider);

			THEN("the free function yields the same active terrains as the handler")
			{
				CHECK((freeFunctionState.mTerrainIDs == handlerState.mTerrainIDs));
				CHECK(std::ranges::contains(freeFunctionState.mTerrainIDs, psychicID));
			}
		}

		WHEN("psychic terrain is already active")
		{
			handlerState.mTerrainIDs = {psychicID};
			freeFunctionState.mTerrainIDs = {psychicID};

			setPsychicTerrainHandler.apply(handlerState, handlerContext, provider);
			applySetPsychic(freeFunctionState, freeFunctionContext, provider);

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

			setPsychicTerrainHandler.apply(handlerState, handlerContext, provider);
			applySetPsychic(freeFunctionState, freeFunctionContext, provider);

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

			setPsychicTerrainHandler.apply(handlerState, handlerContext, provider);
			applySetPsychic(freeFunctionState, freeFunctionContext, provider);

			THEN("neither activates a terrain")
			{
				CHECK((freeFunctionState.mTerrainIDs == handlerState.mTerrainIDs));
				CHECK_FALSE(std::ranges::contains(freeFunctionState.mTerrainIDs, psychicID));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
