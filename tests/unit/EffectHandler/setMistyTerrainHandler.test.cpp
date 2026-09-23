/*! @file setMistyTerrainHandler.test.cpp
	@brief C++ file for running tests for the SetMistyTerrainHandler.
	@date 09/23/2026
	@since 0.12.43
	@version 0.12.43
	@author Matthew Moore
*/

#include "EffectHandler/setMistyTerrainHandler.h"

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
using PocketCore::Effect::SetMistyTerrainHandler;
using PocketCore::Registry::RegistryProvider;
using PocketCore::Terrain::BuiltinTerrainID;
using PocketCore::Terrain::TerrainID;
using PocketCore::Terrain::toTerrainID;
using PocketCore::Testing::getDefaultInitializedRegistryProvider;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

SCENARIO("SetMistyTerrainHandler")
{
	SetMistyTerrainHandler setMistyTerrainHandler{};
	BattleState battleState{};
	EffectContext context{};
	RegistryProvider provider{getDefaultInitializedRegistryProvider()};

	TerrainID mistyID{toTerrainID(BuiltinTerrainID::Misty)};
	TerrainID electricID{toTerrainID(BuiltinTerrainID::Electric)};

	GIVEN("no active terrains")
	{

		WHEN("misty terrain is applied")
		{
			setMistyTerrainHandler.apply(battleState, context, provider);

			THEN("misty terrain becomes active")
			{
				CHECK(std::ranges::contains(battleState.mTerrainIDs, mistyID));
			}

			THEN("misty terrain is only added once")
			{
				CHECK((std::ranges::count(battleState.mTerrainIDs, mistyID) == 1));
			}
		}
	}

	GIVEN("misty terrain is already active")
	{
		battleState.mTerrainIDs = {mistyID};

		WHEN("misty terrain is applied again")
		{
			setMistyTerrainHandler.apply(battleState, context, provider);

			THEN("misty terrain remains active exactly once")
			{
				CHECK((std::ranges::count(battleState.mTerrainIDs, mistyID) == 1));
			}
		}
	}

	GIVEN("a different terrain is active and replacement is allowed")
	{
		battleState.mTerrainIDs = {electricID};
		battleState.mRuleset.mMaxTerrains = 1;
		battleState.mRuleset.mReplaceTerrainWhenFull = true;

		WHEN("misty terrain is applied")
		{
			setMistyTerrainHandler.apply(battleState, context, provider);

			THEN("misty terrain replaces the existing terrain")
			{
				CHECK(std::ranges::contains(battleState.mTerrainIDs, mistyID));
				CHECK_FALSE(std::ranges::contains(battleState.mTerrainIDs, electricID));
			}
		}
	}

	GIVEN("a different terrain is active and replacement is not allowed")
	{
		battleState.mTerrainIDs = {electricID};
		battleState.mRuleset.mMaxTerrains = 1;
		battleState.mRuleset.mReplaceTerrainWhenFull = false;

		WHEN("misty terrain is applied")
		{
			setMistyTerrainHandler.apply(battleState, context, provider);

			THEN("the existing terrain is retained and misty terrain is not added")
			{
				CHECK(std::ranges::contains(battleState.mTerrainIDs, electricID));
				CHECK_FALSE(std::ranges::contains(battleState.mTerrainIDs, mistyID));
			}
		}
	}

	GIVEN("a free slot remains below the terrain cap")
	{
		battleState.mTerrainIDs = {electricID};
		battleState.mRuleset.mMaxTerrains = 2;
		battleState.mRuleset.mReplaceTerrainWhenFull = true;

		WHEN("misty terrain is applied")
		{
			setMistyTerrainHandler.apply(battleState, context, provider);

			THEN("misty terrain is added alongside the existing terrain")
			{
				CHECK(std::ranges::contains(battleState.mTerrainIDs, electricID));
				CHECK(std::ranges::contains(battleState.mTerrainIDs, mistyID));
			}
		}
	}

	GIVEN("the ruleset permits no active terrains")
	{
		battleState.mRuleset.mMaxTerrains = 0;

		WHEN("misty terrain is applied")
		{
			setMistyTerrainHandler.apply(battleState, context, provider);

			THEN("no terrain becomes active")
			{
				CHECK_FALSE(std::ranges::contains(battleState.mTerrainIDs, mistyID));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
