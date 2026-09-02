/*! @file terrainHandler.test.cpp
	@brief C++ file for running tests for the TerrainHandler.
	@date 09/02/2026
	@since 0.9.10
	@version 0.12.17
	@author Matthew Moore
*/

#include "EffectHandler/terrainHandler.h"

#include "Battle/battleState.h"
#include "Battle/battleState.testHelper.h"
#include "Effect/effectContext.h"
#include "Effect/effectContext.testHelper.h"
#include "EffectHandler/terrainHandler.testHelper.h"
#include "Pokemon/pokemon.h"
#include "Pokemon/pokemon.testHelper.h"
#include "Registry/registryProvider.h"
#include "Terrain/builtInTerrainID.h"
#include "Types/builtInTypeID.h"
#include "Types/typeID.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Battle::BattleState;
using PocketCore::Effect::EffectContext;
using PocketCore::Effect::Side;
using PocketCore::Effect::TerrainHandler;
using PocketCore::Pokemon::Pokemon;
using PocketCore::Registry::RegistryProvider;
using PocketCore::Terrain::BuiltinTerrainID;
using PocketCore::Terrain::toTerrainID;
using PocketCore::Testing::hasDragonDebuffInTerrain;
using PocketCore::Testing::hasElectricBuffInTerrain;
using PocketCore::Testing::hasGrassBuffInTerrain;
using PocketCore::Testing::hasPsychicBuffInTerrain;
using PocketCore::Testing::makeBattleState;
using PocketCore::Testing::makeEffectContext;
using PocketCore::Testing::makePokemon;
using PocketCore::Type::BuiltinTypeID;
using PocketCore::Type::toTypeID;
using PocketCore::Type::TypeID;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

SCENARIO("TerrainHandler")
{
	TerrainHandler terrainHandler{};
	RegistryProvider provider{};

	GIVEN("electric terrain with an electric move")
	{
		Pokemon userPokemon{makePokemon({.mTypesIDs = {toTypeID(BuiltinTypeID::Normal)}})};
		BattleState battleState{
			makeBattleState({.mSideA = {{.mPokemon = &userPokemon}}, .mTerrainID = toTerrainID(BuiltinTerrainID::Electric)}),
		};

		EffectContext context{
			makeEffectContext({
				.mMoveTypeID = toTypeID(BuiltinTypeID::Electric),
				.mUserIndex = 0,
				.mTargetIndex = 0,
				.mUserSide = Side::A,
				.mTargetSide = Side::B,
			}),
		};

		GIVEN("and the target isn't flying")
		{
			Pokemon groundedTarget{makePokemon({.mTypesIDs = {toTypeID(BuiltinTypeID::Normal)}})};

			battleState.mSideB.push_back({.mPokemon = &groundedTarget});

			WHEN("terrain effects are applied")
			{
				terrainHandler.apply(battleState, context, provider);

				THEN("the terrain attack boost is applied")
				{
					CHECK(hasElectricBuffInTerrain(battleState, 0));
				}
			}
		}

		GIVEN("and the target is flying")
		{
			Pokemon groundedTarget{makePokemon({.mTypesIDs = {toTypeID(BuiltinTypeID::Flying)}})};

			battleState.mSideB.push_back({.mPokemon = &groundedTarget});

			WHEN("terrain effects are applied")
			{
				terrainHandler.apply(battleState, context, provider);

				THEN("the terrain attack boost is not applied")
				{
					CHECK_FALSE(hasElectricBuffInTerrain(battleState, 0));
				}
			}
		}

		GIVEN("and the target is flying but is forcibly grounded")
		{
			Pokemon groundedTarget{makePokemon({.mTypesIDs = {toTypeID(BuiltinTypeID::Flying)}})};

			battleState.mSideB.push_back({.mPokemon = &groundedTarget, .mIsGrounded = true});

			WHEN("terrain effects are applied")
			{
				terrainHandler.apply(battleState, context, provider);

				THEN("the terrain attack boost is applied")
				{
					CHECK(hasElectricBuffInTerrain(battleState, 0));
				}
			}
		}
	}

	GIVEN("electric terrain without an electric move")
	{
		Pokemon userPokemon{makePokemon({.mTypesIDs = {toTypeID(BuiltinTypeID::Normal)}})};
		BattleState battleState{
			makeBattleState({.mSideA = {{.mPokemon = &userPokemon}}, .mTerrainID = toTerrainID(BuiltinTerrainID::Electric)}),
		};

		EffectContext context{
			makeEffectContext({
				.mMoveTypeID = toTypeID(BuiltinTypeID::Normal),
				.mUserIndex = 0,
				.mTargetIndex = 0,
				.mUserSide = Side::A,
				.mTargetSide = Side::B,
			}),
		};

		GIVEN("and the target isn't flying")
		{
			Pokemon groundedTarget{makePokemon({.mTypesIDs = {toTypeID(BuiltinTypeID::Normal)}})};

			battleState.mSideB.push_back({.mPokemon = &groundedTarget});

			WHEN("terrain effects are applied")
			{
				terrainHandler.apply(battleState, context, provider);

				THEN("the terrain attack boost is not applied")
				{
					CHECK_FALSE(hasElectricBuffInTerrain(battleState, 0));
				}
			}
		}

		GIVEN("and the target is flying")
		{
			Pokemon groundedTarget{makePokemon({.mTypesIDs = {toTypeID(BuiltinTypeID::Flying)}})};

			battleState.mSideB.push_back({.mPokemon = &groundedTarget});

			WHEN("terrain effects are applied")
			{
				terrainHandler.apply(battleState, context, provider);

				THEN("the terrain attack boost is not applied")
				{
					CHECK_FALSE(hasElectricBuffInTerrain(battleState, 0));
				}
			}
		}

		GIVEN("and the target is flying but is forcibly grounded")
		{
			Pokemon groundedTarget{makePokemon({.mTypesIDs = {toTypeID(BuiltinTypeID::Flying)}})};

			battleState.mSideB.push_back({.mPokemon = &groundedTarget, .mIsGrounded = true});

			WHEN("terrain effects are applied")
			{
				terrainHandler.apply(battleState, context, provider);

				THEN("the terrain attack boost is not applied")
				{
					CHECK_FALSE(hasElectricBuffInTerrain(battleState, 0));
				}
			}
		}
	}

	GIVEN("grass terrain with a grass move")
	{
		Pokemon userPokemon{makePokemon({.mTypesIDs = {toTypeID(BuiltinTypeID::Normal)}})};
		BattleState battleState{
			makeBattleState({.mSideA = {{.mPokemon = &userPokemon}}, .mTerrainID = toTerrainID(BuiltinTerrainID::Grassy)}),
		};

		EffectContext context{
			makeEffectContext({
				.mMoveTypeID = toTypeID(BuiltinTypeID::Grass),
				.mUserIndex = 0,
				.mTargetIndex = 0,
				.mUserSide = Side::A,
				.mTargetSide = Side::B,
			}),
		};

		GIVEN("and the target isn't flying")
		{
			Pokemon groundedTarget{makePokemon({.mTypesIDs = {toTypeID(BuiltinTypeID::Normal)}})};

			battleState.mSideB.push_back({.mPokemon = &groundedTarget});

			WHEN("terrain effects are applied")
			{
				terrainHandler.apply(battleState, context, provider);

				THEN("the terrain attack boost is applied")
				{
					CHECK(hasGrassBuffInTerrain(battleState, 0));
				}
			}
		}

		GIVEN("and the target is flying")
		{
			Pokemon groundedTarget{makePokemon({.mTypesIDs = {toTypeID(BuiltinTypeID::Flying)}})};

			battleState.mSideB.push_back({.mPokemon = &groundedTarget});

			WHEN("terrain effects are applied")
			{
				terrainHandler.apply(battleState, context, provider);

				THEN("the terrain attack boost is not applied")
				{
					CHECK_FALSE(hasGrassBuffInTerrain(battleState, 0));
				}
			}
		}

		GIVEN("and the target is flying but is forcibly grounded")
		{
			Pokemon groundedTarget{makePokemon({.mTypesIDs = {toTypeID(BuiltinTypeID::Flying)}})};

			battleState.mSideB.push_back({.mPokemon = &groundedTarget, .mIsGrounded = true});

			WHEN("terrain effects are applied")
			{
				terrainHandler.apply(battleState, context, provider);

				THEN("the terrain attack boost is applied")
				{
					CHECK(hasGrassBuffInTerrain(battleState, 0));
				}
			}
		}
	}

	GIVEN("grass terrain without a grass move")
	{
		Pokemon userPokemon{makePokemon({.mTypesIDs = {toTypeID(BuiltinTypeID::Normal)}})};
		BattleState battleState{
			makeBattleState({.mSideA = {{.mPokemon = &userPokemon}}, .mTerrainID = toTerrainID(BuiltinTerrainID::Grassy)}),
		};

		EffectContext context{
			makeEffectContext({
				.mMoveTypeID = toTypeID(BuiltinTypeID::Normal),
				.mUserIndex = 0,
				.mTargetIndex = 0,
				.mUserSide = Side::A,
				.mTargetSide = Side::B,
			}),
		};

		GIVEN("and the target isn't flying")
		{
			Pokemon groundedTarget{makePokemon({.mTypesIDs = {toTypeID(BuiltinTypeID::Normal)}})};

			battleState.mSideB.push_back({.mPokemon = &groundedTarget});

			WHEN("terrain effects are applied")
			{
				terrainHandler.apply(battleState, context, provider);

				THEN("the terrain attack boost is not applied")
				{
					CHECK_FALSE(hasGrassBuffInTerrain(battleState, 0));
				}
			}
		}

		GIVEN("and the target is flying")
		{
			Pokemon groundedTarget{makePokemon({.mTypesIDs = {toTypeID(BuiltinTypeID::Flying)}})};

			battleState.mSideB.push_back({.mPokemon = &groundedTarget});

			WHEN("terrain effects are applied")
			{
				terrainHandler.apply(battleState, context, provider);

				THEN("the terrain attack boost is not applied")
				{
					CHECK_FALSE(hasGrassBuffInTerrain(battleState, 0));
				}
			}
		}

		GIVEN("and the target is flying but is forcibly grounded")
		{
			Pokemon groundedTarget{makePokemon({.mTypesIDs = {toTypeID(BuiltinTypeID::Flying)}})};

			battleState.mSideB.push_back({.mPokemon = &groundedTarget, .mIsGrounded = true});

			WHEN("terrain effects are applied")
			{
				terrainHandler.apply(battleState, context, provider);

				THEN("the terrain attack boost is not applied")
				{
					CHECK_FALSE(hasGrassBuffInTerrain(battleState, 0));
				}
			}
		}
	}

	GIVEN("psychic terrain with a psychic move")
	{
		Pokemon userPokemon{makePokemon({.mTypesIDs = {toTypeID(BuiltinTypeID::Normal)}})};
		BattleState battleState{
			makeBattleState({.mSideA = {{.mPokemon = &userPokemon}}, .mTerrainID = toTerrainID(BuiltinTerrainID::Psychic)}),
		};

		EffectContext context{
			makeEffectContext({
				.mMoveTypeID = toTypeID(BuiltinTypeID::Psychic),
				.mUserIndex = 0,
				.mTargetIndex = 0,
				.mUserSide = Side::A,
				.mTargetSide = Side::B,
			}),
		};

		GIVEN("and the target isn't flying")
		{
			Pokemon groundedTarget{makePokemon({.mTypesIDs = {toTypeID(BuiltinTypeID::Normal)}})};

			battleState.mSideB.push_back({.mPokemon = &groundedTarget});

			WHEN("terrain effects are applied")
			{
				terrainHandler.apply(battleState, context, provider);

				THEN("the terrain attack boost is applied")
				{
					CHECK(hasPsychicBuffInTerrain(battleState, 0));
				}
			}
		}

		GIVEN("and the target is flying")
		{
			Pokemon groundedTarget{makePokemon({.mTypesIDs = {toTypeID(BuiltinTypeID::Flying)}})};

			battleState.mSideB.push_back({.mPokemon = &groundedTarget});

			WHEN("terrain effects are applied")
			{
				terrainHandler.apply(battleState, context, provider);

				THEN("the terrain attack boost is not applied")
				{
					CHECK_FALSE(hasPsychicBuffInTerrain(battleState, 0));
				}
			}
		}

		GIVEN("and the target is flying but is forcibly grounded")
		{
			Pokemon groundedTarget{makePokemon({.mTypesIDs = {toTypeID(BuiltinTypeID::Flying)}})};

			battleState.mSideB.push_back({.mPokemon = &groundedTarget, .mIsGrounded = true});

			WHEN("terrain effects are applied")
			{
				terrainHandler.apply(battleState, context, provider);

				THEN("the terrain attack boost is applied")
				{
					CHECK(hasPsychicBuffInTerrain(battleState, 0));
				}
			}
		}
	}

	GIVEN("psychic terrain without a psychic move")
	{
		Pokemon userPokemon{makePokemon({.mTypesIDs = {toTypeID(BuiltinTypeID::Normal)}})};
		BattleState battleState{
			makeBattleState({.mSideA = {{.mPokemon = &userPokemon}}, .mTerrainID = toTerrainID(BuiltinTerrainID::Psychic)}),
		};

		EffectContext context{
			makeEffectContext({
				.mMoveTypeID = toTypeID(BuiltinTypeID::Normal),
				.mUserIndex = 0,
				.mTargetIndex = 0,
				.mUserSide = Side::A,
				.mTargetSide = Side::B,
			}),
		};

		GIVEN("and the target isn't flying")
		{
			Pokemon groundedTarget{makePokemon({.mTypesIDs = {toTypeID(BuiltinTypeID::Normal)}})};

			battleState.mSideB.push_back({.mPokemon = &groundedTarget});

			WHEN("terrain effects are applied")
			{
				terrainHandler.apply(battleState, context, provider);

				THEN("the terrain attack boost is not applied")
				{
					CHECK_FALSE(hasPsychicBuffInTerrain(battleState, 0));
				}
			}
		}

		GIVEN("and the target is flying")
		{
			Pokemon groundedTarget{makePokemon({.mTypesIDs = {toTypeID(BuiltinTypeID::Flying)}})};

			battleState.mSideB.push_back({.mPokemon = &groundedTarget});

			WHEN("terrain effects are applied")
			{
				terrainHandler.apply(battleState, context, provider);

				THEN("the terrain attack boost is not applied")
				{
					CHECK_FALSE(hasPsychicBuffInTerrain(battleState, 0));
				}
			}
		}

		GIVEN("and the target is flying but is forcibly grounded")
		{
			Pokemon groundedTarget{makePokemon({.mTypesIDs = {toTypeID(BuiltinTypeID::Flying)}})};

			battleState.mSideB.push_back({.mPokemon = &groundedTarget, .mIsGrounded = true});

			WHEN("terrain effects are applied")
			{
				terrainHandler.apply(battleState, context, provider);

				THEN("the terrain attack boost is not applied")
				{
					CHECK_FALSE(hasPsychicBuffInTerrain(battleState, 0));
				}
			}
		}
	}

	GIVEN("misty terrain with a dragon move")
	{
		Pokemon userPokemon{makePokemon({.mTypesIDs = {toTypeID(BuiltinTypeID::Normal)}})};
		BattleState battleState{
			makeBattleState({.mSideA = {{.mPokemon = &userPokemon}}, .mTerrainID = toTerrainID(BuiltinTerrainID::Misty)}),
		};

		EffectContext context{
			makeEffectContext({
				.mMoveTypeID = toTypeID(BuiltinTypeID::Dragon),
				.mUserIndex = 0,
				.mTargetIndex = 0,
				.mUserSide = Side::A,
				.mTargetSide = Side::B,
			}),
		};

		GIVEN("and the target isn't flying")
		{
			Pokemon groundedTarget{makePokemon({.mTypesIDs = {toTypeID(BuiltinTypeID::Normal)}})};

			battleState.mSideB.push_back({.mPokemon = &groundedTarget});

			WHEN("terrain effects are applied")
			{
				terrainHandler.apply(battleState, context, provider);

				THEN("the terrain attack boost is applied")
				{
					CHECK(hasDragonDebuffInTerrain(battleState, 0));
				}
			}
		}

		GIVEN("and the target is flying")
		{
			Pokemon groundedTarget{makePokemon({.mTypesIDs = {toTypeID(BuiltinTypeID::Flying)}})};

			battleState.mSideB.push_back({.mPokemon = &groundedTarget});

			WHEN("terrain effects are applied")
			{
				terrainHandler.apply(battleState, context, provider);

				THEN("the terrain attack boost is not applied")
				{
					CHECK_FALSE(hasDragonDebuffInTerrain(battleState, 0));
				}
			}
		}

		GIVEN("and the target is flying but is forcibly grounded")
		{
			Pokemon groundedTarget{makePokemon({.mTypesIDs = {toTypeID(BuiltinTypeID::Flying)}})};

			battleState.mSideB.push_back({.mPokemon = &groundedTarget, .mIsGrounded = true});

			WHEN("terrain effects are applied")
			{
				terrainHandler.apply(battleState, context, provider);

				THEN("the terrain attack boost is applied")
				{
					CHECK(hasDragonDebuffInTerrain(battleState, 0));
				}
			}
		}
	}

	GIVEN("misty terrain without a dragon move")
	{
		Pokemon userPokemon{makePokemon({.mTypesIDs = {toTypeID(BuiltinTypeID::Normal)}})};
		BattleState battleState{
			makeBattleState({.mSideA = {{.mPokemon = &userPokemon}}, .mTerrainID = toTerrainID(BuiltinTerrainID::Misty)}),
		};

		EffectContext context{
			makeEffectContext({
				.mMoveTypeID = toTypeID(BuiltinTypeID::Normal),
				.mUserIndex = 0,
				.mTargetIndex = 0,
				.mUserSide = Side::A,
				.mTargetSide = Side::B,
			}),
		};

		GIVEN("and the target isn't flying")
		{
			Pokemon groundedTarget{makePokemon({.mTypesIDs = {toTypeID(BuiltinTypeID::Normal)}})};

			battleState.mSideB.push_back({.mPokemon = &groundedTarget});

			WHEN("terrain effects are applied")
			{
				terrainHandler.apply(battleState, context, provider);

				THEN("the terrain attack boost is not applied")
				{
					CHECK_FALSE(hasDragonDebuffInTerrain(battleState, 0));
				}
			}
		}

		GIVEN("and the target is flying")
		{
			Pokemon groundedTarget{makePokemon({.mTypesIDs = {toTypeID(BuiltinTypeID::Flying)}})};

			battleState.mSideB.push_back({.mPokemon = &groundedTarget});

			WHEN("terrain effects are applied")
			{
				terrainHandler.apply(battleState, context, provider);

				THEN("the terrain attack boost is not applied")
				{
					CHECK_FALSE(hasDragonDebuffInTerrain(battleState, 0));
				}
			}
		}

		GIVEN("and the target is flying but is forcibly grounded")
		{
			Pokemon groundedTarget{makePokemon({.mTypesIDs = {toTypeID(BuiltinTypeID::Flying)}})};

			battleState.mSideB.push_back({.mPokemon = &groundedTarget, .mIsGrounded = true});

			WHEN("terrain effects are applied")
			{
				terrainHandler.apply(battleState, context, provider);

				THEN("the terrain attack boost is not applied")
				{
					CHECK_FALSE(hasDragonDebuffInTerrain(battleState, 0));
				}
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
