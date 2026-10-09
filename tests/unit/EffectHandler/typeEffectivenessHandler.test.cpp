/*! @file typeEffectivenessHandler.test.cpp
	@brief C++ file for running tests for the TypeEffectivenessHandler.
	@date 10/09/2026
	@since 0.12.50
	@version 0.12.50
	@author Matthew Moore
*/

#include "EffectHandler/typeEffectivenessHandler.h"

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
#include "Types/builtInTypeID.h"
#include "Types/constants.h"
#include "Types/typeEffectiveness.h"
#include "Types/typeID.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Battle::BattleState;
using PocketCore::Configuration::TypeRegistryConfiguration;
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
	BattleState battleState{
		makeBattleState({.mSideA = {{.mPokemon = &userPokemon}}, .mSideB = {{.mPokemon = &targetPokemon}}}),
	};
	TypeRegistryConfiguration typeConfiguration{};
	const TypeRegistry &typeRegistry{typeConfiguration.getRuntimeRegistry()};
	RegistryProvider provider{.mTypeRegistry = &typeRegistry};
	EffectContext context{
		makeEffectContext(
			{.mMoveTypeID = toTypeID(BuiltinTypeID::Fire), .mMoveBasePower = 20, .mUserSide = Side::A, .mTargetSide = Side::B}),
	};

	GIVEN("a null user pokemon")
	{
		Pokemon *nullUser{nullptr};
		BattleState newBattleState{
			makeBattleState({.mSideA = {{.mPokemon = nullUser}}, .mSideB = {{.mPokemon = &targetPokemon}}}),
		};

		WHEN("applying type effectiveness handler")
		{
			typeEffectivenessHandler.apply(newBattleState, context, provider);

			THEN("no type effectiveness multiplier is added")
			{
				CHECK_FALSE(hasTypeEffectivenessMultiplier(context));
			}
		}
	}

	GIVEN("a null target pokemon")
	{
		Pokemon *nullTarget{nullptr};
		BattleState newBattleState{
			makeBattleState({.mSideA = {{.mPokemon = &userPokemon}}, .mSideB = {{.mPokemon = nullTarget}}}),
		};

		WHEN("applying type effectiveness handler")
		{
			typeEffectivenessHandler.apply(newBattleState, context, provider);

			THEN("no type effectiveness multiplier is added")
			{
				CHECK_FALSE(hasTypeEffectivenessMultiplier(context));
			}
		}
	}

	GIVEN("no move base power")
	{
		context.mMoveBasePower = 0;

		WHEN("applying type effectiveness handler")
		{
			typeEffectivenessHandler.apply(battleState, context, provider);

			THEN("no type effectiveness multiplier is added")
			{
				CHECK_FALSE(hasTypeEffectivenessMultiplier(context));
			}
		}
	}

	GIVEN("invalid type registry")
	{
		RegistryProvider nullProvider{getNullTypeRegistryProvider()};

		WHEN("applying type effectiveness handler")
		{
			typeEffectivenessHandler.apply(battleState, context, nullProvider);

			THEN("no type effectiveness multiplier is added")
			{
				CHECK_FALSE(hasTypeEffectivenessMultiplier(context));
			}
		}
	}

	GIVEN("invalid type ID")
	{
		context.mMoveTypeID = TypeID{200};

		WHEN("applying type effectiveness handler")
		{
			typeEffectivenessHandler.apply(battleState, context, provider);

			THEN("no type effectiveness multiplier is added")
			{
				CHECK_FALSE(hasTypeEffectivenessMultiplier(context));
			}
		}
	}

	GIVEN("a target pokemon with no type ID")
	{
		Pokemon newTarget{makePokemon({.mTypesIDs = {toTypeID(BuiltinTypeID::None), toTypeID(BuiltinTypeID::Normal)}})};
		BattleState newBattleState{
			makeBattleState({.mSideA = {{.mPokemon = &userPokemon}}, .mSideB = {{.mPokemon = &newTarget}}}),
		};

		WHEN("applying type effectiveness handler")
		{
			typeEffectivenessHandler.apply(newBattleState, context, provider);

			THEN("a base effectiveness is applied")
			{
				CHECK(hasTypeEffectivenessMultiplier(context, getEffectivenessValue(TypeEffectiveness::E)));
			}
		}
	}

	GIVEN("a target pokemon with an invalid type ID")
	{
		Pokemon newTarget{makePokemon({.mTypesIDs = {TypeID{200}, toTypeID(BuiltinTypeID::Normal)}})};
		BattleState newBattleState{
			makeBattleState({.mSideA = {{.mPokemon = &userPokemon}}, .mSideB = {{.mPokemon = &newTarget}}}),
		};

		WHEN("applying type effectiveness handler")
		{
			typeEffectivenessHandler.apply(newBattleState, context, provider);

			THEN("a base effectiveness is applied")
			{
				CHECK(hasTypeEffectivenessMultiplier(context, getEffectivenessValue(TypeEffectiveness::E)));
			}
		}
	}

	GIVEN("a target pokemon with an undefined matchup")
	{
		REQUIRE(typeConfiguration.setMatchup(TYPE_NAME_FIRE, TYPE_NAME_GRASS, TypeEffectiveness::NOT_DEFINED).has_value());
		REQUIRE((typeConfiguration.getMatchup(TYPE_NAME_FIRE, TYPE_NAME_GRASS) == TypeEffectiveness::NOT_DEFINED));

		Pokemon newTarget{makePokemon({.mTypesIDs = {toTypeID(BuiltinTypeID::Grass), toTypeID(BuiltinTypeID::Normal)}})};
		BattleState newBattleState{
			makeBattleState({.mSideA = {{.mPokemon = &userPokemon}}, .mSideB = {{.mPokemon = &newTarget}}}),
		};

		WHEN("applying type effectiveness handler")
		{
			typeEffectivenessHandler.apply(newBattleState, context, provider);

			THEN("a base effectiveness is applied")
			{
				CHECK(hasTypeEffectivenessMultiplier(context, getEffectivenessValue(TypeEffectiveness::E)));
			}
		}
	}

	GIVEN("a target pokemon which will fully resist a move")
	{
		Pokemon newTarget{makePokemon({.mTypesIDs = {toTypeID(BuiltinTypeID::Grass), toTypeID(BuiltinTypeID::Ghost)}})};
		BattleState newBattleState{
			makeBattleState({.mSideA = {{.mPokemon = &userPokemon}}, .mSideB = {{.mPokemon = &newTarget}}}),
		};
		context.mMoveTypeID = toTypeID(BuiltinTypeID::Normal);

		WHEN("applying type effectiveness handler")
		{
			typeEffectivenessHandler.apply(newBattleState, context, provider);

			THEN("a complete resistance is applied")
			{
				CHECK(hasTypeEffectivenessMultiplier(context, getEffectivenessValue(TypeEffectiveness::NE)));
			}
		}
	}

	GIVEN("a target pokemon which will quad resist a move")
	{
		Pokemon newTarget{makePokemon({.mTypesIDs = {toTypeID(BuiltinTypeID::Fire), toTypeID(BuiltinTypeID::Grass)}})};
		BattleState newBattleState{
			makeBattleState({.mSideA = {{.mPokemon = &userPokemon}}, .mSideB = {{.mPokemon = &newTarget}}}),
		};
		context.mMoveTypeID = toTypeID(BuiltinTypeID::Grass);

		WHEN("applying type effectiveness handler")
		{
			typeEffectivenessHandler.apply(newBattleState, context, provider);

			THEN("a quad resistance is applied")
			{
				CHECK(hasTypeEffectivenessMultiplier(context, getEffectivenessValue(TypeEffectiveness::NVE)
																  * getEffectivenessValue(TypeEffectiveness::NVE)));
			}
		}
	}

	GIVEN("a target pokemon which will half resist a move")
	{
		Pokemon newTarget{makePokemon({.mTypesIDs = {toTypeID(BuiltinTypeID::Electric), toTypeID(BuiltinTypeID::Steel)}})};
		BattleState newBattleState{
			makeBattleState({.mSideA = {{.mPokemon = &userPokemon}}, .mSideB = {{.mPokemon = &newTarget}}}),
		};
		context.mMoveTypeID = toTypeID(BuiltinTypeID::Ice);

		WHEN("applying type effectiveness handler")
		{
			typeEffectivenessHandler.apply(newBattleState, context, provider);

			THEN("a half resistance is applied")
			{
				CHECK(hasTypeEffectivenessMultiplier(context, getEffectivenessValue(TypeEffectiveness::NVE)));
			}
		}
	}

	GIVEN("a target pokemon which will have a half weakness to a move")
	{
		Pokemon newTarget{makePokemon({.mTypesIDs = {toTypeID(BuiltinTypeID::Grass), toTypeID(BuiltinTypeID::Dark)}})};
		BattleState newBattleState{
			makeBattleState({.mSideA = {{.mPokemon = &userPokemon}}, .mSideB = {{.mPokemon = &newTarget}}}),
		};
		context.mMoveTypeID = toTypeID(BuiltinTypeID::Fighting);

		WHEN("applying type effectiveness handler")
		{
			typeEffectivenessHandler.apply(newBattleState, context, provider);

			THEN("a half weakness is applied")
			{
				CHECK(hasTypeEffectivenessMultiplier(context, getEffectivenessValue(TypeEffectiveness::SE)));
			}
		}
	}

	GIVEN("a target pokemon which will have a quad weakness to a move")
	{
		Pokemon newTarget{makePokemon({.mTypesIDs = {toTypeID(BuiltinTypeID::Rock), toTypeID(BuiltinTypeID::Ground)}})};
		BattleState newBattleState{
			makeBattleState({.mSideA = {{.mPokemon = &userPokemon}}, .mSideB = {{.mPokemon = &newTarget}}}),
		};
		context.mMoveTypeID = toTypeID(BuiltinTypeID::Water);

		WHEN("applying type effectiveness handler")
		{
			typeEffectivenessHandler.apply(newBattleState, context, provider);

			THEN("a quad weakness is applied")
			{
				CHECK(hasTypeEffectivenessMultiplier(context, getEffectivenessValue(TypeEffectiveness::SE)
																  * getEffectivenessValue(TypeEffectiveness::SE)));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
