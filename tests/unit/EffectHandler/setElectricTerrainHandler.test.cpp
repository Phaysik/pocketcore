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
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
