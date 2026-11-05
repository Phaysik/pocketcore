/*! @file typeEffectivenessHandler.test.cpp
	@brief C++ file for running tests for the TypeEffectivenessHandler.
	@date 10/28/2026
	@since 0.12.50
	@version 0.12.53
	@author Matthew Moore
*/

#include "EffectHandler/typeEffectivenessHandler.h"

#include "Battle/battleRandomSources.h"
#include "Battle/battleState.h"
#include "Battle/battleState.testHelper.h"
#include "Configuration/typeRegistryConfiguration.h"
#include "Effect/effectContext.h"
#include "Effect/effectContext.testHelper.h"
#include "EffectHandler/typeEffectivenessHandler.testHelper.h"
#include "Pokemon/pokemon.h"
#include "Pokemon/pokemon.testHelper.h"
#include "Registry/registryProvider.h"
#include "Registry/registryProvider.testHelper.h"
#include "Registry/typeRegistry.h"
#include "Type/builtInTypeID.h"
#include "Type/constants.h"
#include "Type/typeEffectiveness.h"
#include "Type/typeID.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Battle::BattleRandomSources;
using PocketCore::Battle::BattleState;
using PocketCore::Configuration::TypeRegistryConfiguration;
using PocketCore::Effect::applyTypeEffectiveness;
using PocketCore::Effect::EffectContext;
using PocketCore::Effect::Side;
using PocketCore::Effect::TypeEffectivenessHandler;
using PocketCore::Pokemon::Pokemon;
using PocketCore::Registry::RegistryProvider;
using PocketCore::Registry::Type::TypeRegistry;
using PocketCore::Testing::getNullTypeRegistryProvider;
using PocketCore::Testing::hasTypeEffectivenessMultiplier;
using PocketCore::Testing::makeBattleState;
using PocketCore::Testing::makeEffectContext;
using PocketCore::Testing::makePokemon;
using PocketCore::Type::BuiltinTypeID;
using PocketCore::Type::getEffectivenessValue;
using PocketCore::Type::toTypeID;
using PocketCore::Type::TYPE_NAME_FIRE;
using PocketCore::Type::TYPE_NAME_GRASS;
using PocketCore::Type::TypeEffectiveness;
using PocketCore::Type::TypeID;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

SCENARIO("TypeEffectivenessHandler")
{
	TypeEffectivenessHandler typeEffectivenessHandler{};
	Pokemon userPokemon{makePokemon({})};
	Pokemon targetPokemon{makePokemon({})};
	BattleState handlerState{
		makeBattleState({.mSideA = {{.mPokemon = &userPokemon}}, .mSideB = {{.mPokemon = &targetPokemon}}}),
	};
	BattleState freeFunctionState{
		makeBattleState({.mSideA = {{.mPokemon = &userPokemon}}, .mSideB = {{.mPokemon = &targetPokemon}}}),
	};
	TypeRegistryConfiguration typeConfiguration{};
	const TypeRegistry &typeRegistry{typeConfiguration.getRuntimeRegistry()};
	RegistryProvider provider{.mTypeRegistry = &typeRegistry};
	EffectContext handlerContext{
		makeEffectContext(
			{.mMoveTypeID = toTypeID(BuiltinTypeID::Fire), .mMoveBasePower = 20, .mUserSide = Side::A, .mTargetSide = Side::B}),
	};
	EffectContext freeFunctionContext{
		makeEffectContext(
			{.mMoveTypeID = toTypeID(BuiltinTypeID::Fire), .mMoveBasePower = 20, .mUserSide = Side::A, .mTargetSide = Side::B}),
	};
	BattleRandomSources randomSources{};

	GIVEN("a null user pokemon")
	{
		handlerState.mSideA = {{.mPokemon = nullptr}};
		freeFunctionState.mSideA = {{.mPokemon = nullptr}};

		WHEN("applying type effectiveness handler")
		{
			typeEffectivenessHandler.apply(handlerState, handlerContext, provider, randomSources);
			applyTypeEffectiveness(freeFunctionState, freeFunctionContext, provider, randomSources);

			THEN("no type effectiveness multiplier is added")
			{
				CHECK_FALSE(hasTypeEffectivenessMultiplier(handlerContext));
				CHECK_FALSE(hasTypeEffectivenessMultiplier(freeFunctionContext));
			}
		}
	}

	GIVEN("a null target pokemon")
	{
		handlerState.mSideB = {{.mPokemon = nullptr}};
		freeFunctionState.mSideB = {{.mPokemon = nullptr}};

		WHEN("applying type effectiveness handler")
		{
			typeEffectivenessHandler.apply(handlerState, handlerContext, provider, randomSources);
			applyTypeEffectiveness(freeFunctionState, freeFunctionContext, provider, randomSources);

			THEN("no type effectiveness multiplier is added")
			{
				CHECK_FALSE(hasTypeEffectivenessMultiplier(handlerContext));
				CHECK_FALSE(hasTypeEffectivenessMultiplier(freeFunctionContext));
			}
		}
	}

	GIVEN("no move base power")
	{
		handlerContext.mMoveBasePower = 0;
		freeFunctionContext.mMoveBasePower = 0;

		WHEN("applying type effectiveness handler")
		{
			typeEffectivenessHandler.apply(handlerState, handlerContext, provider, randomSources);
			applyTypeEffectiveness(freeFunctionState, freeFunctionContext, provider, randomSources);

			THEN("no type effectiveness multiplier is added")
			{
				CHECK_FALSE(hasTypeEffectivenessMultiplier(handlerContext));
				CHECK_FALSE(hasTypeEffectivenessMultiplier(freeFunctionContext));
			}
		}
	}

	GIVEN("invalid type registry")
	{
		RegistryProvider nullProvider{getNullTypeRegistryProvider()};

		WHEN("applying type effectiveness handler")
		{
			typeEffectivenessHandler.apply(handlerState, handlerContext, nullProvider, randomSources);
			applyTypeEffectiveness(freeFunctionState, freeFunctionContext, nullProvider, randomSources);

			THEN("no type effectiveness multiplier is added")
			{
				CHECK_FALSE(hasTypeEffectivenessMultiplier(handlerContext));
				CHECK_FALSE(hasTypeEffectivenessMultiplier(freeFunctionContext));
			}
		}
	}

	GIVEN("invalid type ID")
	{
		handlerContext.mMoveTypeID = TypeID{200};
		freeFunctionContext.mMoveTypeID = TypeID{200};

		WHEN("applying type effectiveness handler")
		{
			typeEffectivenessHandler.apply(handlerState, handlerContext, provider, randomSources);
			applyTypeEffectiveness(freeFunctionState, freeFunctionContext, provider, randomSources);

			THEN("no type effectiveness multiplier is added")
			{
				CHECK_FALSE(hasTypeEffectivenessMultiplier(handlerContext));
				CHECK_FALSE(hasTypeEffectivenessMultiplier(freeFunctionContext));
			}
		}
	}

	GIVEN("a target pokemon with no type ID")
	{
		targetPokemon.setTypeIDsArray({toTypeID(BuiltinTypeID::None), toTypeID(BuiltinTypeID::Normal)});

		WHEN("applying type effectiveness handler")
		{
			typeEffectivenessHandler.apply(handlerState, handlerContext, provider, randomSources);
			applyTypeEffectiveness(freeFunctionState, freeFunctionContext, provider, randomSources);

			THEN("a base effectiveness is applied")
			{
				CHECK(hasTypeEffectivenessMultiplier(handlerContext, getEffectivenessValue(TypeEffectiveness::E)));
				CHECK(hasTypeEffectivenessMultiplier(freeFunctionContext, getEffectivenessValue(TypeEffectiveness::E)));
			}
		}
	}

	GIVEN("a target pokemon with an invalid type ID")
	{
		targetPokemon.setTypeIDsArray({TypeID{200}, toTypeID(BuiltinTypeID::Normal)});

		WHEN("applying type effectiveness handler")
		{
			typeEffectivenessHandler.apply(handlerState, handlerContext, provider, randomSources);
			applyTypeEffectiveness(freeFunctionState, freeFunctionContext, provider, randomSources);

			THEN("a base effectiveness is applied")
			{
				CHECK(hasTypeEffectivenessMultiplier(handlerContext, getEffectivenessValue(TypeEffectiveness::E)));
				CHECK(hasTypeEffectivenessMultiplier(freeFunctionContext, getEffectivenessValue(TypeEffectiveness::E)));
			}
		}
	}

	GIVEN("a target pokemon with an undefined matchup")
	{
		REQUIRE(typeConfiguration.setMatchup(TYPE_NAME_FIRE, TYPE_NAME_GRASS, TypeEffectiveness::NOT_DEFINED).has_value());
		REQUIRE((typeConfiguration.getMatchup(TYPE_NAME_FIRE, TYPE_NAME_GRASS) == TypeEffectiveness::NOT_DEFINED));

		targetPokemon.setTypeIDsArray({toTypeID(BuiltinTypeID::Grass), toTypeID(BuiltinTypeID::Normal)});

		WHEN("applying type effectiveness handler")
		{
			typeEffectivenessHandler.apply(handlerState, handlerContext, provider, randomSources);
			applyTypeEffectiveness(freeFunctionState, freeFunctionContext, provider, randomSources);

			THEN("a base effectiveness is applied")
			{
				CHECK(hasTypeEffectivenessMultiplier(handlerContext, getEffectivenessValue(TypeEffectiveness::E)));
				CHECK(hasTypeEffectivenessMultiplier(freeFunctionContext, getEffectivenessValue(TypeEffectiveness::E)));
			}
		}
	}

	GIVEN("a target pokemon which will fully resist a move")
	{
		targetPokemon.setTypeIDsArray({toTypeID(BuiltinTypeID::Grass), toTypeID(BuiltinTypeID::Ghost)});

		handlerContext.mMoveTypeID = toTypeID(BuiltinTypeID::Normal);
		freeFunctionContext.mMoveTypeID = toTypeID(BuiltinTypeID::Normal);

		WHEN("applying type effectiveness handler")
		{
			typeEffectivenessHandler.apply(handlerState, handlerContext, provider, randomSources);
			applyTypeEffectiveness(freeFunctionState, freeFunctionContext, provider, randomSources);

			THEN("a complete resistance is applied")
			{
				CHECK(hasTypeEffectivenessMultiplier(handlerContext, getEffectivenessValue(TypeEffectiveness::NE)));
				CHECK(hasTypeEffectivenessMultiplier(freeFunctionContext, getEffectivenessValue(TypeEffectiveness::NE)));
			}
		}
	}

	GIVEN("a target pokemon which will quad resist a move")
	{
		targetPokemon.setTypeIDsArray({toTypeID(BuiltinTypeID::Fire), toTypeID(BuiltinTypeID::Grass)});

		handlerContext.mMoveTypeID = toTypeID(BuiltinTypeID::Grass);
		freeFunctionContext.mMoveTypeID = toTypeID(BuiltinTypeID::Grass);

		WHEN("applying type effectiveness handler")
		{
			typeEffectivenessHandler.apply(handlerState, handlerContext, provider, randomSources);
			applyTypeEffectiveness(freeFunctionState, freeFunctionContext, provider, randomSources);

			THEN("a quad resistance is applied")
			{
				CHECK(hasTypeEffectivenessMultiplier(handlerContext, getEffectivenessValue(TypeEffectiveness::NVE)
																		 * getEffectivenessValue(TypeEffectiveness::NVE)));
				CHECK(hasTypeEffectivenessMultiplier(freeFunctionContext, getEffectivenessValue(TypeEffectiveness::NVE)
																			  * getEffectivenessValue(TypeEffectiveness::NVE)));
			}
		}
	}

	GIVEN("a target pokemon which will half resist a move")
	{
		targetPokemon.setTypeIDsArray({toTypeID(BuiltinTypeID::Electric), toTypeID(BuiltinTypeID::Steel)});

		handlerContext.mMoveTypeID = toTypeID(BuiltinTypeID::Ice);
		freeFunctionContext.mMoveTypeID = toTypeID(BuiltinTypeID::Ice);

		WHEN("applying type effectiveness handler")
		{
			typeEffectivenessHandler.apply(handlerState, handlerContext, provider, randomSources);
			applyTypeEffectiveness(freeFunctionState, freeFunctionContext, provider, randomSources);

			THEN("a half resistance is applied")
			{
				CHECK(hasTypeEffectivenessMultiplier(handlerContext, getEffectivenessValue(TypeEffectiveness::NVE)));
				CHECK(hasTypeEffectivenessMultiplier(freeFunctionContext, getEffectivenessValue(TypeEffectiveness::NVE)));
			}
		}
	}

	GIVEN("a target pokemon which will have a half weakness to a move")
	{
		targetPokemon.setTypeIDsArray({toTypeID(BuiltinTypeID::Grass), toTypeID(BuiltinTypeID::Dark)});

		handlerContext.mMoveTypeID = toTypeID(BuiltinTypeID::Fighting);
		freeFunctionContext.mMoveTypeID = toTypeID(BuiltinTypeID::Fighting);

		WHEN("applying type effectiveness handler")
		{
			typeEffectivenessHandler.apply(handlerState, handlerContext, provider, randomSources);
			applyTypeEffectiveness(freeFunctionState, freeFunctionContext, provider, randomSources);

			THEN("a half weakness is applied")
			{
				CHECK(hasTypeEffectivenessMultiplier(handlerContext, getEffectivenessValue(TypeEffectiveness::SE)));
				CHECK(hasTypeEffectivenessMultiplier(freeFunctionContext, getEffectivenessValue(TypeEffectiveness::SE)));
			}
		}
	}

	GIVEN("a target pokemon which will have a quad weakness to a move")
	{
		targetPokemon.setTypeIDsArray({toTypeID(BuiltinTypeID::Rock), toTypeID(BuiltinTypeID::Ground)});

		handlerContext.mMoveTypeID = toTypeID(BuiltinTypeID::Water);
		freeFunctionContext.mMoveTypeID = toTypeID(BuiltinTypeID::Water);

		WHEN("applying type effectiveness handler")
		{
			typeEffectivenessHandler.apply(handlerState, handlerContext, provider, randomSources);
			applyTypeEffectiveness(freeFunctionState, freeFunctionContext, provider, randomSources);

			THEN("a quad weakness is applied")
			{
				CHECK(hasTypeEffectivenessMultiplier(handlerContext, getEffectivenessValue(TypeEffectiveness::SE)
																		 * getEffectivenessValue(TypeEffectiveness::SE)));
				CHECK(hasTypeEffectivenessMultiplier(freeFunctionContext, getEffectivenessValue(TypeEffectiveness::SE)
																			  * getEffectivenessValue(TypeEffectiveness::SE)));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
