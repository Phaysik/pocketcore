/*! @file setElectricTerrainHandler.test.cpp
	@brief C++ file for running tests for the SetElectricTerrainHandler.
	@date 09/23/2026
	@since 0.12.43
	@version 0.12.43
	@author Matthew Moore
*/

#include "EffectHandler/setElectricTerrainHandler.h"

#include <algorithm>

#include "Battle/battleState.h"
#include "Effect/effectContext.h"
#include "Registry/registryProvider.h"
#include "Registry/registryProvider.testHelper.h"
#include "Terrain/builtInTerrainID.h"
#include "Terrain/terrainID.h"

#include <catch2/catch_test_macros.hpp>

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
	BattleState battleState{};
	EffectContext context{};
	RegistryProvider provider{getDefaultInitializedRegistryProvider()};

	TerrainID electricID{toTerrainID(BuiltinTerrainID::Electric)};
	TerrainID mistyID{toTerrainID(BuiltinTerrainID::Misty)};

	GIVEN("no active terrains")
	{

		WHEN("electric terrain is applied")
		{
			setElectricTerrainHandler.apply(battleState, context, provider);

			THEN("electric terrain becomes active")
			{
				CHECK(std::ranges::contains(battleState.mTerrainIDs, electricID));
			}

			THEN("electric terrain is only added once")
			{
				CHECK((std::ranges::count(battleState.mTerrainIDs, electricID) == 1));
			}
		}
	}

	GIVEN("electric terrain is already active")
	{
		battleState.mTerrainIDs = {electricID};

		WHEN("electric terrain is applied again")
		{
			setElectricTerrainHandler.apply(battleState, context, provider);

			THEN("electric terrain remains active exactly once")
			{
				CHECK((std::ranges::count(battleState.mTerrainIDs, electricID) == 1));
			}
		}
	}

	GIVEN("a different terrain is active and replacement is allowed")
	{
		battleState.mTerrainIDs = {mistyID};
		battleState.mRuleset.mMaxTerrains = 1;
		battleState.mRuleset.mReplaceTerrainWhenFull = true;

		WHEN("electric terrain is applied")
		{
			setElectricTerrainHandler.apply(battleState, context, provider);

			THEN("electric terrain replaces the existing terrain")
			{
				CHECK(std::ranges::contains(battleState.mTerrainIDs, electricID));
				CHECK_FALSE(std::ranges::contains(battleState.mTerrainIDs, mistyID));
			}
		}
	}

	GIVEN("a different terrain is active and replacement is not allowed")
	{
		battleState.mTerrainIDs = {mistyID};
		battleState.mRuleset.mMaxTerrains = 1;
		battleState.mRuleset.mReplaceTerrainWhenFull = false;

		WHEN("electric terrain is applied")
		{
			setElectricTerrainHandler.apply(battleState, context, provider);

			THEN("the existing terrain is retained and electric terrain is not added")
			{
				CHECK(std::ranges::contains(battleState.mTerrainIDs, mistyID));
				CHECK_FALSE(std::ranges::contains(battleState.mTerrainIDs, electricID));
			}
		}
	}

	GIVEN("a free slot remains below the terrain cap")
	{
		battleState.mTerrainIDs = {mistyID};
		battleState.mRuleset.mMaxTerrains = 2;
		battleState.mRuleset.mReplaceTerrainWhenFull = true;

		WHEN("electric terrain is applied")
		{
			setElectricTerrainHandler.apply(battleState, context, provider);

			THEN("electric terrain is added alongside the existing terrain")
			{
				CHECK(std::ranges::contains(battleState.mTerrainIDs, mistyID));
				CHECK(std::ranges::contains(battleState.mTerrainIDs, electricID));
			}
		}
	}

	GIVEN("the ruleset permits no active terrains")
	{
		battleState.mRuleset.mMaxTerrains = 0;

		WHEN("electric terrain is applied")
		{
			setElectricTerrainHandler.apply(battleState, context, provider);

			THEN("no terrain becomes active")
			{
				CHECK_FALSE(std::ranges::contains(battleState.mTerrainIDs, electricID));
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
			setElectricTerrainHandler.apply(handlerState, handlerContext, provider);
			applySetElectric(freeFunctionState, freeFunctionContext, provider);

			THEN("the free function yields the same active terrains as the handler")
			{
				CHECK((freeFunctionState.mTerrainIDs == handlerState.mTerrainIDs));
				CHECK(std::ranges::contains(freeFunctionState.mTerrainIDs, electricID));
			}
		}

		WHEN("psychic terrain is already active")
		{
			handlerState.mTerrainIDs = {electricID};
			freeFunctionState.mTerrainIDs = {electricID};

			setElectricTerrainHandler.apply(handlerState, handlerContext, provider);
			applySetElectric(freeFunctionState, freeFunctionContext, provider);

			THEN("both leave psychic terrain active exactly once")
			{
				CHECK((freeFunctionState.mTerrainIDs == handlerState.mTerrainIDs));
				CHECK((std::ranges::count(freeFunctionState.mTerrainIDs, electricID) == 1));
			}
		}

		WHEN("a different terrain is active and replacement is allowed")
		{
			handlerState.mTerrainIDs = {mistyID};
			handlerState.mRuleset.mMaxTerrains = 1;
			handlerState.mRuleset.mReplaceTerrainWhenFull = true;
			freeFunctionState.mTerrainIDs = {mistyID};
			freeFunctionState.mRuleset.mMaxTerrains = 1;
			freeFunctionState.mRuleset.mReplaceTerrainWhenFull = true;

			setElectricTerrainHandler.apply(handlerState, handlerContext, provider);
			applySetElectric(freeFunctionState, freeFunctionContext, provider);

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

			setElectricTerrainHandler.apply(handlerState, handlerContext, provider);
			applySetElectric(freeFunctionState, freeFunctionContext, provider);

			THEN("neither activates a terrain")
			{
				CHECK((freeFunctionState.mTerrainIDs == handlerState.mTerrainIDs));
				CHECK_FALSE(std::ranges::contains(freeFunctionState.mTerrainIDs, electricID));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
