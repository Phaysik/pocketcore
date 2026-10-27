/*! @file terrainHandler.test.cpp
	@brief C++ file for running tests for the TerrainHandler.
	@date 10/27/2026
	@since 0.9.10
	@version 0.12.53
	@author Matthew Moore
*/

#include "EffectHandler/terrainHandler.h"

#include "Battle/battleRandomSources.h"
#include "Battle/battleState.h"
#include "Battle/battleState.testHelper.h"
#include "Effect/effectContext.h"
#include "Effect/effectContext.testHelper.h"
#include "EffectHandler/terrainHandler.testHelper.h"
#include "Pokemon/pokemon.h"
#include "Pokemon/pokemon.testHelper.h"
#include "Registry/registryProvider.h"
#include "Registry/registryProvider.testHelper.h"
#include "Terrain/builtInTerrainID.h"
#include "Type/builtInTypeID.h"
#include "Type/typeID.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Battle::BattleRandomSources;
using PocketCore::Battle::BattleState;
using PocketCore::Effect::applyTerrain;
using PocketCore::Effect::EffectContext;
using PocketCore::Effect::Side;
using PocketCore::Effect::TerrainHandler;
using PocketCore::Pokemon::Pokemon;
using PocketCore::Registry::RegistryProvider;
using PocketCore::Terrain::BuiltinTerrainID;
using PocketCore::Terrain::toTerrainID;
using PocketCore::Testing::getDefaultInitializedRegistryProvider;
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
	Pokemon userPokemon{makePokemon({.mMetadata = {.mTypeIDs = {toTypeID(BuiltinTypeID::Normal)}}})};
	TerrainHandler terrainHandler{};
	RegistryProvider provider{getDefaultInitializedRegistryProvider()};
	BattleRandomSources randomSources{};

	BattleState handlerState{
		makeBattleState({.mSideA = {{.mPokemon = &userPokemon}}, .mTerrainIDs = {toTerrainID(BuiltinTerrainID::Electric)}}),
	};
	BattleState freeFunctionState{
		makeBattleState({.mSideA = {{.mPokemon = &userPokemon}}, .mTerrainIDs = {toTerrainID(BuiltinTerrainID::Electric)}}),
	};

	EffectContext handlerContext{
		makeEffectContext({
			.mMoveTypeID = toTypeID(BuiltinTypeID::Electric),
			.mUserIndex = 0,
			.mTargetIndex = 0,
			.mUserSide = Side::A,
			.mTargetSide = Side::B,
		}),
	};
	EffectContext freeFunctionContext{
		makeEffectContext({
			.mMoveTypeID = toTypeID(BuiltinTypeID::Electric),
			.mUserIndex = 0,
			.mTargetIndex = 0,
			.mUserSide = Side::A,
			.mTargetSide = Side::B,
		}),
	};

	GIVEN("electric terrain with an electric move")
	{
		GIVEN("and the target isn't flying")
		{
			Pokemon groundedTarget{makePokemon({.mMetadata = {.mTypeIDs = {toTypeID(BuiltinTypeID::Normal)}}})};

			handlerState.mSideB.push_back({.mPokemon = &groundedTarget});
			freeFunctionState.mSideB.push_back({.mPokemon = &groundedTarget});

			WHEN("terrain effects are applied")
			{
				terrainHandler.apply(handlerState, handlerContext, provider, randomSources);
				applyTerrain(freeFunctionState, freeFunctionContext, provider, randomSources);

				THEN("the terrain attack boost is applied")
				{
					CHECK(hasElectricBuffInTerrain(handlerState, 0));
					CHECK(hasElectricBuffInTerrain(freeFunctionState, 0));
				}
			}
		}

		GIVEN("and the target is flying")
		{
			Pokemon groundedTarget{makePokemon({.mMetadata = {.mTypeIDs = {toTypeID(BuiltinTypeID::Flying)}}})};

			handlerState.mSideB.push_back({.mPokemon = &groundedTarget});
			freeFunctionState.mSideB.push_back({.mPokemon = &groundedTarget});

			WHEN("terrain effects are applied")
			{
				terrainHandler.apply(handlerState, handlerContext, provider, randomSources);
				applyTerrain(freeFunctionState, freeFunctionContext, provider, randomSources);

				THEN("the terrain attack boost is not applied")
				{
					CHECK_FALSE(hasElectricBuffInTerrain(handlerState, 0));
					CHECK_FALSE(hasElectricBuffInTerrain(freeFunctionState, 0));
				}
			}
		}

		GIVEN("and the target is flying but is forcibly grounded")
		{
			Pokemon groundedTarget{makePokemon({.mMetadata = {.mTypeIDs = {toTypeID(BuiltinTypeID::Flying)}}})};

			handlerState.mSideB.push_back({.mPokemon = &groundedTarget, .mIsGrounded = true});
			freeFunctionState.mSideB.push_back({.mPokemon = &groundedTarget, .mIsGrounded = true});

			WHEN("terrain effects are applied")
			{
				terrainHandler.apply(handlerState, handlerContext, provider, randomSources);
				applyTerrain(freeFunctionState, freeFunctionContext, provider, randomSources);

				THEN("the terrain attack boost is applied")
				{
					CHECK(hasElectricBuffInTerrain(handlerState, 0));
					CHECK(hasElectricBuffInTerrain(freeFunctionState, 0));
				}
			}
		}
	}

	GIVEN("electric terrain without an electric move")
	{
		handlerContext.mMoveTypeID = toTypeID(BuiltinTypeID::Normal);
		freeFunctionContext.mMoveTypeID = toTypeID(BuiltinTypeID::Normal);

		GIVEN("and the target isn't flying")
		{
			Pokemon groundedTarget{makePokemon({.mMetadata = {.mTypeIDs = {toTypeID(BuiltinTypeID::Normal)}}})};

			handlerState.mSideB.push_back({.mPokemon = &groundedTarget});
			freeFunctionState.mSideB.push_back({.mPokemon = &groundedTarget});

			WHEN("terrain effects are applied")
			{
				terrainHandler.apply(handlerState, handlerContext, provider, randomSources);
				applyTerrain(freeFunctionState, freeFunctionContext, provider, randomSources);

				THEN("the terrain attack boost is not applied")
				{
					CHECK_FALSE(hasElectricBuffInTerrain(handlerState, 0));
					CHECK_FALSE(hasElectricBuffInTerrain(freeFunctionState, 0));
				}
			}
		}

		GIVEN("and the target is flying")
		{
			Pokemon groundedTarget{makePokemon({.mMetadata = {.mTypeIDs = {toTypeID(BuiltinTypeID::Flying)}}})};

			handlerState.mSideB.push_back({.mPokemon = &groundedTarget});
			freeFunctionState.mSideB.push_back({.mPokemon = &groundedTarget});

			WHEN("terrain effects are applied")
			{
				terrainHandler.apply(handlerState, handlerContext, provider, randomSources);
				applyTerrain(freeFunctionState, freeFunctionContext, provider, randomSources);

				THEN("the terrain attack boost is not applied")
				{
					CHECK_FALSE(hasElectricBuffInTerrain(handlerState, 0));
					CHECK_FALSE(hasElectricBuffInTerrain(freeFunctionState, 0));
				}
			}
		}

		GIVEN("and the target is flying but is forcibly grounded")
		{
			Pokemon groundedTarget{makePokemon({.mMetadata = {.mTypeIDs = {toTypeID(BuiltinTypeID::Flying)}}})};

			handlerState.mSideB.push_back({.mPokemon = &groundedTarget, .mIsGrounded = true});
			freeFunctionState.mSideB.push_back({.mPokemon = &groundedTarget, .mIsGrounded = true});

			WHEN("terrain effects are applied")
			{
				terrainHandler.apply(handlerState, handlerContext, provider, randomSources);
				applyTerrain(freeFunctionState, freeFunctionContext, provider, randomSources);

				THEN("the terrain attack boost is not applied")
				{
					CHECK_FALSE(hasElectricBuffInTerrain(handlerState, 0));
					CHECK_FALSE(hasElectricBuffInTerrain(freeFunctionState, 0));
				}
			}
		}
	}

	GIVEN("grass terrain with a grass move")
	{
		handlerState.mTerrainIDs = {toTerrainID(BuiltinTerrainID::Grassy)};
		freeFunctionState.mTerrainIDs = {toTerrainID(BuiltinTerrainID::Grassy)};

		handlerContext.mMoveTypeID = toTypeID(BuiltinTypeID::Grass);
		freeFunctionContext.mMoveTypeID = toTypeID(BuiltinTypeID::Grass);

		GIVEN("and the target isn't flying")
		{
			Pokemon groundedTarget{makePokemon({.mMetadata = {.mTypeIDs = {toTypeID(BuiltinTypeID::Normal)}}})};

			handlerState.mSideB.push_back({.mPokemon = &groundedTarget});
			freeFunctionState.mSideB.push_back({.mPokemon = &groundedTarget});

			WHEN("terrain effects are applied")
			{
				terrainHandler.apply(handlerState, handlerContext, provider, randomSources);
				applyTerrain(freeFunctionState, freeFunctionContext, provider, randomSources);

				THEN("the terrain attack boost is applied")
				{
					CHECK(hasGrassBuffInTerrain(handlerState, 0));
					CHECK(hasGrassBuffInTerrain(freeFunctionState, 0));
				}
			}
		}

		GIVEN("and the target is flying")
		{
			Pokemon groundedTarget{makePokemon({.mMetadata = {.mTypeIDs = {toTypeID(BuiltinTypeID::Flying)}}})};

			handlerState.mSideB.push_back({.mPokemon = &groundedTarget});
			freeFunctionState.mSideB.push_back({.mPokemon = &groundedTarget});

			WHEN("terrain effects are applied")
			{
				terrainHandler.apply(handlerState, handlerContext, provider, randomSources);
				applyTerrain(freeFunctionState, freeFunctionContext, provider, randomSources);

				THEN("the terrain attack boost is not applied")
				{
					CHECK_FALSE(hasGrassBuffInTerrain(handlerState, 0));
					CHECK_FALSE(hasGrassBuffInTerrain(freeFunctionState, 0));
				}
			}
		}

		GIVEN("and the target is flying but is forcibly grounded")
		{
			Pokemon groundedTarget{makePokemon({.mMetadata = {.mTypeIDs = {toTypeID(BuiltinTypeID::Flying)}}})};

			handlerState.mSideB.push_back({.mPokemon = &groundedTarget, .mIsGrounded = true});
			freeFunctionState.mSideB.push_back({.mPokemon = &groundedTarget, .mIsGrounded = true});

			WHEN("terrain effects are applied")
			{
				terrainHandler.apply(handlerState, handlerContext, provider, randomSources);
				applyTerrain(freeFunctionState, freeFunctionContext, provider, randomSources);

				THEN("the terrain attack boost is applied")
				{
					CHECK(hasGrassBuffInTerrain(handlerState, 0));
					CHECK(hasGrassBuffInTerrain(freeFunctionState, 0));
				}
			}
		}
	}

	GIVEN("grass terrain without a grass move")
	{
		handlerState.mTerrainIDs = {toTerrainID(BuiltinTerrainID::Grassy)};
		freeFunctionState.mTerrainIDs = {toTerrainID(BuiltinTerrainID::Grassy)};

		handlerContext.mMoveTypeID = toTypeID(BuiltinTypeID::Normal);
		freeFunctionContext.mMoveTypeID = toTypeID(BuiltinTypeID::Normal);

		GIVEN("and the target isn't flying")
		{
			Pokemon groundedTarget{makePokemon({.mMetadata = {.mTypeIDs = {toTypeID(BuiltinTypeID::Normal)}}})};

			handlerState.mSideB.push_back({.mPokemon = &groundedTarget});
			freeFunctionState.mSideB.push_back({.mPokemon = &groundedTarget});

			WHEN("terrain effects are applied")
			{
				terrainHandler.apply(handlerState, handlerContext, provider, randomSources);
				applyTerrain(freeFunctionState, freeFunctionContext, provider, randomSources);

				THEN("the terrain attack boost is not applied")
				{
					CHECK_FALSE(hasGrassBuffInTerrain(handlerState, 0));
					CHECK_FALSE(hasGrassBuffInTerrain(freeFunctionState, 0));
				}
			}
		}

		GIVEN("and the target is flying")
		{
			Pokemon groundedTarget{makePokemon({.mMetadata = {.mTypeIDs = {toTypeID(BuiltinTypeID::Flying)}}})};

			handlerState.mSideB.push_back({.mPokemon = &groundedTarget});
			freeFunctionState.mSideB.push_back({.mPokemon = &groundedTarget});

			WHEN("terrain effects are applied")
			{
				terrainHandler.apply(handlerState, handlerContext, provider, randomSources);
				applyTerrain(freeFunctionState, freeFunctionContext, provider, randomSources);

				THEN("the terrain attack boost is not applied")
				{
					CHECK_FALSE(hasGrassBuffInTerrain(handlerState, 0));
					CHECK_FALSE(hasGrassBuffInTerrain(freeFunctionState, 0));
				}
			}
		}

		GIVEN("and the target is flying but is forcibly grounded")
		{
			Pokemon groundedTarget{makePokemon({.mMetadata = {.mTypeIDs = {toTypeID(BuiltinTypeID::Flying)}}})};

			handlerState.mSideB.push_back({.mPokemon = &groundedTarget, .mIsGrounded = true});
			freeFunctionState.mSideB.push_back({.mPokemon = &groundedTarget, .mIsGrounded = true});

			WHEN("terrain effects are applied")
			{
				terrainHandler.apply(handlerState, handlerContext, provider, randomSources);
				applyTerrain(freeFunctionState, freeFunctionContext, provider, randomSources);

				THEN("the terrain attack boost is not applied")
				{
					CHECK_FALSE(hasGrassBuffInTerrain(handlerState, 0));
					CHECK_FALSE(hasGrassBuffInTerrain(freeFunctionState, 0));
				}
			}
		}
	}

	GIVEN("psychic terrain with a psychic move")
	{
		handlerState.mTerrainIDs = {toTerrainID(BuiltinTerrainID::Psychic)};
		freeFunctionState.mTerrainIDs = {toTerrainID(BuiltinTerrainID::Psychic)};

		handlerContext.mMoveTypeID = toTypeID(BuiltinTypeID::Psychic);
		freeFunctionContext.mMoveTypeID = toTypeID(BuiltinTypeID::Psychic);

		GIVEN("and the target isn't flying")
		{
			Pokemon groundedTarget{makePokemon({.mMetadata = {.mTypeIDs = {toTypeID(BuiltinTypeID::Normal)}}})};

			handlerState.mSideB.push_back({.mPokemon = &groundedTarget});
			freeFunctionState.mSideB.push_back({.mPokemon = &groundedTarget});

			WHEN("terrain effects are applied")
			{
				terrainHandler.apply(handlerState, handlerContext, provider, randomSources);
				applyTerrain(freeFunctionState, freeFunctionContext, provider, randomSources);

				THEN("the terrain attack boost is applied")
				{
					CHECK(hasPsychicBuffInTerrain(handlerState, 0));
					CHECK(hasPsychicBuffInTerrain(freeFunctionState, 0));
				}
			}
		}

		GIVEN("and the target is flying")
		{
			Pokemon groundedTarget{makePokemon({.mMetadata = {.mTypeIDs = {toTypeID(BuiltinTypeID::Flying)}}})};

			handlerState.mSideB.push_back({.mPokemon = &groundedTarget});
			freeFunctionState.mSideB.push_back({.mPokemon = &groundedTarget});

			WHEN("terrain effects are applied")
			{
				terrainHandler.apply(handlerState, handlerContext, provider, randomSources);
				applyTerrain(freeFunctionState, freeFunctionContext, provider, randomSources);

				THEN("the terrain attack boost is not applied")
				{
					CHECK_FALSE(hasPsychicBuffInTerrain(handlerState, 0));
					CHECK_FALSE(hasPsychicBuffInTerrain(freeFunctionState, 0));
				}
			}
		}

		GIVEN("and the target is flying but is forcibly grounded")
		{
			Pokemon groundedTarget{makePokemon({.mMetadata = {.mTypeIDs = {toTypeID(BuiltinTypeID::Flying)}}})};

			handlerState.mSideB.push_back({.mPokemon = &groundedTarget, .mIsGrounded = true});
			freeFunctionState.mSideB.push_back({.mPokemon = &groundedTarget, .mIsGrounded = true});

			WHEN("terrain effects are applied")
			{
				terrainHandler.apply(handlerState, handlerContext, provider, randomSources);
				applyTerrain(freeFunctionState, freeFunctionContext, provider, randomSources);

				THEN("the terrain attack boost is applied")
				{
					CHECK(hasPsychicBuffInTerrain(handlerState, 0));
					CHECK(hasPsychicBuffInTerrain(freeFunctionState, 0));
				}
			}
		}
	}

	GIVEN("psychic terrain without a psychic move")
	{
		handlerState.mTerrainIDs = {toTerrainID(BuiltinTerrainID::Psychic)};
		freeFunctionState.mTerrainIDs = {toTerrainID(BuiltinTerrainID::Psychic)};

		handlerContext.mMoveTypeID = toTypeID(BuiltinTypeID::Normal);
		freeFunctionContext.mMoveTypeID = toTypeID(BuiltinTypeID::Normal);

		GIVEN("and the target isn't flying")
		{
			Pokemon groundedTarget{makePokemon({.mMetadata = {.mTypeIDs = {toTypeID(BuiltinTypeID::Normal)}}})};

			handlerState.mSideB.push_back({.mPokemon = &groundedTarget});
			freeFunctionState.mSideB.push_back({.mPokemon = &groundedTarget});

			WHEN("terrain effects are applied")
			{
				terrainHandler.apply(handlerState, handlerContext, provider, randomSources);
				applyTerrain(freeFunctionState, freeFunctionContext, provider, randomSources);

				THEN("the terrain attack boost is not applied")
				{
					CHECK_FALSE(hasPsychicBuffInTerrain(handlerState, 0));
					CHECK_FALSE(hasPsychicBuffInTerrain(freeFunctionState, 0));
				}
			}
		}

		GIVEN("and the target is flying")
		{
			Pokemon groundedTarget{makePokemon({.mMetadata = {.mTypeIDs = {toTypeID(BuiltinTypeID::Flying)}}})};

			handlerState.mSideB.push_back({.mPokemon = &groundedTarget});
			freeFunctionState.mSideB.push_back({.mPokemon = &groundedTarget});

			WHEN("terrain effects are applied")
			{
				terrainHandler.apply(handlerState, handlerContext, provider, randomSources);
				applyTerrain(freeFunctionState, freeFunctionContext, provider, randomSources);

				THEN("the terrain attack boost is not applied")
				{
					CHECK_FALSE(hasPsychicBuffInTerrain(handlerState, 0));
					CHECK_FALSE(hasPsychicBuffInTerrain(freeFunctionState, 0));
				}
			}
		}

		GIVEN("and the target is flying but is forcibly grounded")
		{
			Pokemon groundedTarget{makePokemon({.mMetadata = {.mTypeIDs = {toTypeID(BuiltinTypeID::Flying)}}})};

			handlerState.mSideB.push_back({.mPokemon = &groundedTarget, .mIsGrounded = true});
			freeFunctionState.mSideB.push_back({.mPokemon = &groundedTarget, .mIsGrounded = true});

			WHEN("terrain effects are applied")
			{
				terrainHandler.apply(handlerState, handlerContext, provider, randomSources);
				applyTerrain(freeFunctionState, freeFunctionContext, provider, randomSources);

				THEN("the terrain attack boost is not applied")
				{
					CHECK_FALSE(hasPsychicBuffInTerrain(handlerState, 0));
					CHECK_FALSE(hasPsychicBuffInTerrain(freeFunctionState, 0));
				}
			}
		}
	}

	GIVEN("misty terrain with a dragon move")
	{
		handlerState.mTerrainIDs = {toTerrainID(BuiltinTerrainID::Misty)};
		freeFunctionState.mTerrainIDs = {toTerrainID(BuiltinTerrainID::Misty)};

		handlerContext.mMoveTypeID = toTypeID(BuiltinTypeID::Dragon);
		freeFunctionContext.mMoveTypeID = toTypeID(BuiltinTypeID::Dragon);

		GIVEN("and the target isn't flying")
		{
			Pokemon groundedTarget{makePokemon({.mMetadata = {.mTypeIDs = {toTypeID(BuiltinTypeID::Normal)}}})};

			handlerState.mSideB.push_back({.mPokemon = &groundedTarget});
			freeFunctionState.mSideB.push_back({.mPokemon = &groundedTarget});

			WHEN("terrain effects are applied")
			{
				terrainHandler.apply(handlerState, handlerContext, provider, randomSources);
				applyTerrain(freeFunctionState, freeFunctionContext, provider, randomSources);

				THEN("the terrain attack boost is applied")
				{
					CHECK(hasDragonDebuffInTerrain(handlerState, 0));
					CHECK(hasDragonDebuffInTerrain(freeFunctionState, 0));
				}
			}
		}

		GIVEN("and the target is flying")
		{
			Pokemon groundedTarget{makePokemon({.mMetadata = {.mTypeIDs = {toTypeID(BuiltinTypeID::Flying)}}})};

			handlerState.mSideB.push_back({.mPokemon = &groundedTarget});
			freeFunctionState.mSideB.push_back({.mPokemon = &groundedTarget});

			WHEN("terrain effects are applied")
			{
				terrainHandler.apply(handlerState, handlerContext, provider, randomSources);
				applyTerrain(freeFunctionState, freeFunctionContext, provider, randomSources);

				THEN("the terrain attack boost is not applied")
				{
					CHECK_FALSE(hasDragonDebuffInTerrain(handlerState, 0));
					CHECK_FALSE(hasDragonDebuffInTerrain(freeFunctionState, 0));
				}
			}
		}

		GIVEN("and the target is flying but is forcibly grounded")
		{
			Pokemon groundedTarget{makePokemon({.mMetadata = {.mTypeIDs = {toTypeID(BuiltinTypeID::Flying)}}})};

			handlerState.mSideB.push_back({.mPokemon = &groundedTarget, .mIsGrounded = true});
			freeFunctionState.mSideB.push_back({.mPokemon = &groundedTarget, .mIsGrounded = true});

			WHEN("terrain effects are applied")
			{
				terrainHandler.apply(handlerState, handlerContext, provider, randomSources);
				applyTerrain(freeFunctionState, freeFunctionContext, provider, randomSources);

				THEN("the terrain attack boost is applied")
				{
					CHECK(hasDragonDebuffInTerrain(handlerState, 0));
					CHECK(hasDragonDebuffInTerrain(freeFunctionState, 0));
				}
			}
		}
	}

	GIVEN("misty terrain without a dragon move")
	{
		handlerState.mTerrainIDs = {toTerrainID(BuiltinTerrainID::Misty)};
		freeFunctionState.mTerrainIDs = {toTerrainID(BuiltinTerrainID::Misty)};

		handlerContext.mMoveTypeID = toTypeID(BuiltinTypeID::Normal);
		freeFunctionContext.mMoveTypeID = toTypeID(BuiltinTypeID::Normal);

		GIVEN("and the target isn't flying")
		{
			Pokemon groundedTarget{makePokemon({.mMetadata = {.mTypeIDs = {toTypeID(BuiltinTypeID::Normal)}}})};

			handlerState.mSideB.push_back({.mPokemon = &groundedTarget});
			freeFunctionState.mSideB.push_back({.mPokemon = &groundedTarget});

			WHEN("terrain effects are applied")
			{
				terrainHandler.apply(handlerState, handlerContext, provider, randomSources);
				applyTerrain(freeFunctionState, freeFunctionContext, provider, randomSources);

				THEN("the terrain attack boost is not applied")
				{
					CHECK_FALSE(hasDragonDebuffInTerrain(handlerState, 0));
					CHECK_FALSE(hasDragonDebuffInTerrain(freeFunctionState, 0));
				}
			}
		}

		GIVEN("and the target is flying")
		{
			Pokemon groundedTarget{makePokemon({.mMetadata = {.mTypeIDs = {toTypeID(BuiltinTypeID::Flying)}}})};

			handlerState.mSideB.push_back({.mPokemon = &groundedTarget});
			freeFunctionState.mSideB.push_back({.mPokemon = &groundedTarget});

			WHEN("terrain effects are applied")
			{
				terrainHandler.apply(handlerState, handlerContext, provider, randomSources);
				applyTerrain(freeFunctionState, freeFunctionContext, provider, randomSources);

				THEN("the terrain attack boost is not applied")
				{
					CHECK_FALSE(hasDragonDebuffInTerrain(handlerState, 0));
					CHECK_FALSE(hasDragonDebuffInTerrain(freeFunctionState, 0));
				}
			}
		}

		GIVEN("and the target is flying but is forcibly grounded")
		{
			Pokemon groundedTarget{makePokemon({.mMetadata = {.mTypeIDs = {toTypeID(BuiltinTypeID::Flying)}}})};

			handlerState.mSideB.push_back({.mPokemon = &groundedTarget, .mIsGrounded = true});
			freeFunctionState.mSideB.push_back({.mPokemon = &groundedTarget, .mIsGrounded = true});

			WHEN("terrain effects are applied")
			{
				terrainHandler.apply(handlerState, handlerContext, provider, randomSources);
				applyTerrain(freeFunctionState, freeFunctionContext, provider, randomSources);

				THEN("the terrain attack boost is not applied")
				{
					CHECK_FALSE(hasDragonDebuffInTerrain(handlerState, 0));
					CHECK_FALSE(hasDragonDebuffInTerrain(freeFunctionState, 0));
				}
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
