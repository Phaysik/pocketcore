/*! @file baseDamageHandler.test.cpp
	@brief C++ file for running tests for the BaseDamageHandler.
	@date 09/01/2026
	@since 0.8.7
	@version 0.12.14
	@author Matthew Moore
*/

#include "EffectHandler/baseDamageHandler.h"

#include <limits>

#include "Ability/abilityID.h"
#include "Battle/battleState.h"
#include "Battle/battleState.testHelper.h"
#include "Core/typedefs.h"
#include "Effect/effectContext.h"
#include "Effect/effectContext.testHelper.h"
#include "Item/itemID.h"
#include "Pokemon/pokemon.h"
#include "Pokemon/pokemon.testHelper.h"
#include "Registry/registryProvider.h"
#include "Types/typeID.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Ability::NO_ABILITY_ID;
using PocketCore::Battle::BattleState;
using PocketCore::Core::us;
using PocketCore::Effect::BaseDamageHandler;
using PocketCore::Effect::EffectContext;
using PocketCore::Effect::Side;
using PocketCore::Item::NO_ITEM_ID;
using PocketCore::Pokemon::Pokemon;
using PocketCore::Registry::RegistryProvider;
using PocketCore::Testing::makeBattleState;
using PocketCore::Testing::makeEffectContext;
using PocketCore::Testing::makePokemon;
using PocketCore::Type::NO_TYPE_ID;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

SCENARIO("BaseDamageHandler")
{
	BaseDamageHandler baseDamageHandler{};
	RegistryProvider provider{};

	GIVEN("invalid pokemon pointers")
	{
		GIVEN("null user pokemon")
		{
			Pokemon targetPokemon{makePokemon({})};

			BattleState battleState{makeBattleState({.mSideA = {{.mPokemon = nullptr}}, .mSideB = {{.mPokemon = &targetPokemon}}})};

			EffectContext context{makeEffectContext({.mMoveBasePower = 65, .mUserSide = Side::A, .mTargetSide = Side::B})};

			WHEN("base damage is applied before external modifiers")
			{
				baseDamageHandler.apply(battleState, context, provider);

				THEN("the initial damage returns 0 since no valid user")
				{
					CHECK((context.mDamage.mDamage == 0));
				}
			}
		}

		GIVEN("null target pokemon")
		{
			Pokemon userPokemon{makePokemon({})};

			BattleState battleState{makeBattleState({.mSideA = {{.mPokemon = &userPokemon}}, .mSideB = {{.mPokemon = nullptr}}})};

			EffectContext context{makeEffectContext({.mMoveBasePower = 65, .mUserSide = Side::A, .mTargetSide = Side::B})};

			WHEN("base damage is applied before external modifiers")
			{
				baseDamageHandler.apply(battleState, context, provider);

				THEN("the initial damage returns 0 since no valid target")
				{
					CHECK((context.mDamage.mDamage == 0));
				}
			}
		}
	}

	GIVEN("no move base power")
	{
		Pokemon userPokemon{makePokemon({.mAttack = 123, .mLevel = 75})};
		Pokemon targetPokemon{makePokemon({.mDefense = 163, .mLevel = 50})};

		BattleState battleState{makeBattleState({.mSideA = {{.mPokemon = &userPokemon}}, .mSideB = {{.mPokemon = &targetPokemon}}})};

		EffectContext context{makeEffectContext({.mMoveBasePower = 0, .mUserSide = Side::A, .mTargetSide = Side::B})};

		WHEN("base damage is applied before external modifiers")
		{
			baseDamageHandler.apply(battleState, context, provider);

			THEN("the initial damage returns 0 since no move base power")
			{
				CHECK((context.mDamage.mDamage == 0));
			}
		}
	}

	GIVEN("a target with 0 defense")
	{
		Pokemon userPokemon{makePokemon({.mAttack = 123, .mLevel = 75})};
		Pokemon targetPokemon{makePokemon({.mDefense = 0, .mLevel = 50})};

		BattleState battleState{makeBattleState({.mSideA = {{.mPokemon = &userPokemon}}, .mSideB = {{.mPokemon = &targetPokemon}}})};

		EffectContext context{makeEffectContext({.mMoveBasePower = 65, .mUserSide = Side::A, .mTargetSide = Side::B})};

		WHEN("base damage is applied before external modifiers")
		{
			baseDamageHandler.apply(battleState, context, provider);

			THEN("the initial damage calculation clamps the defense to 1.0")
			{
				CHECK((context.mDamage.mDamage == 5'118));
			}
		}
	}

	GIVEN("a user with a non-finite modifiers")
	{
		Pokemon userPokemon{makePokemon({.mAttack = 123, .mLevel = 75})};
		Pokemon targetPokemon{makePokemon({.mDefense = 163, .mLevel = 50})};

		EffectContext context{makeEffectContext({.mMoveBasePower = 65, .mUserSide = Side::A, .mTargetSide = Side::B})};

		GIVEN("specifically the attack modifier")
		{
			BattleState battleState{
				makeBattleState({
					.mSideA = {{
						.mDamageFormulaModifiers = {.mAttackModifier = std::numeric_limits<double>::quiet_NaN()},
						.mPokemon = &userPokemon,
					},},
					.mSideB = {{.mPokemon = &targetPokemon}},
				}),
			};

			WHEN("base damage is applied before external modifiers")
			{
				baseDamageHandler.apply(battleState, context, provider);

				THEN("the initial damage returns 0 since non-finite attack")
				{
					CHECK((context.mDamage.mDamage == 0));
				}
			}
		}

		GIVEN("specifically the defense modifier")
		{
			BattleState battleState{
				makeBattleState({
					.mSideA = {{.mPokemon = &userPokemon}},
					.mSideB = {{
						.mDamageFormulaModifiers = {.mDefenseModifier = std::numeric_limits<double>::quiet_NaN()},
						.mPokemon = &targetPokemon,
					},},
				}),
			};

			WHEN("base damage is applied before external modifiers")
			{
				baseDamageHandler.apply(battleState, context, provider);

				THEN("the initial damage returns 0 since non-finite defense")
				{
					CHECK((context.mDamage.mDamage == 0));
				}
			}
		}
	}

	GIVEN("a user has a positive attack stat")
	{
		Pokemon userPokemon{makePokemon({.mAttack = 123, .mLevel = 75})};
		Pokemon targetPokemon{makePokemon({.mDefense = 163, .mLevel = 50})};

		EffectContext context{makeEffectContext({.mMoveBasePower = 65, .mUserSide = Side::A, .mTargetSide = Side::B})};

		GIVEN("with a negative attack modifier")
		{
			BattleState battleState{
				makeBattleState({
					.mSideA = {{.mDamageFormulaModifiers = {.mAttackModifier = -1.0}, .mPokemon = &userPokemon}},
					.mSideB = {{.mPokemon = &targetPokemon}},
				}),
			};

			WHEN("base damage is applied before external modifiers")
			{
				baseDamageHandler.apply(battleState, context, provider);

				THEN("the initial damage returns 0 since the attack was below 0")
				{
					CHECK((context.mDamage.mDamage == 0));
				}
			}
		}

		GIVEN("with a 0 attack modifier")
		{
			BattleState battleState{
				makeBattleState({
					.mSideA = {{.mDamageFormulaModifiers = {.mAttackModifier = 0.0}, .mPokemon = &userPokemon}},
					.mSideB = {{.mPokemon = &targetPokemon}},
				}),
			};

			WHEN("base damage is applied before external modifiers")
			{
				baseDamageHandler.apply(battleState, context, provider);

				THEN("the initial damage returns 0 since the attack was 0")
				{
					CHECK((context.mDamage.mDamage == 0));
				}
			}
		}
	}

	GIVEN("the Generation V onward")
	{
		Pokemon userPokemon{makePokemon({.mAttack = 123, .mSpecialAttack = 100, .mLevel = 75})};
		Pokemon targetPokemon{makePokemon({.mDefense = 163, .mSpecialDefense = 124, .mLevel = 50})};

		BattleState battleState{makeBattleState({.mSideA = {{.mPokemon = &userPokemon}}, .mSideB = {{.mPokemon = &targetPokemon}}})};

		GIVEN("Bulbapedia base-damage attack example")
		{
			EffectContext context{makeEffectContext({.mMoveBasePower = 65, .mUserSide = Side::A, .mTargetSide = Side::B})};

			WHEN("base damage is applied before external modifiers")
			{
				baseDamageHandler.apply(battleState, context, provider);

				THEN("the initial damage calculation follows each floor boundary")
				{
					CHECK((context.mDamage.mDamage == 33));
				}
			}
		}

		GIVEN("example using special attack")
		{
			EffectContext context{
				makeEffectContext({.mMoveBasePower = 65, .mUserSide = Side::A, .mTargetSide = Side::B, .mIsSpecial = true}),
			};

			WHEN("base damage is applied before external modifiers")
			{
				baseDamageHandler.apply(battleState, context, provider);

				THEN("the initial damage calculation follows each floor boundary")
				{
					CHECK((context.mDamage.mDamage == 35));
				}
			}
		}
	}

	GIVEN("modified stat stages")
	{
		Pokemon userPokemon{makePokemon({.mAttack = 123, .mSpecialAttack = 100, .mLevel = 75})};
		Pokemon targetPokemon{makePokemon({.mDefense = 163, .mSpecialDefense = 124, .mLevel = 50})};

		GIVEN("user has increased attack stages and target has increased defense stages")
		{
			BattleState battleState{
				makeBattleState({
					.mSideA = {{.mPokemon = &userPokemon, .mStatStages = {.mAttack = 4}}},
					.mSideB = {{.mPokemon = &targetPokemon, .mStatStages = {.mDefense = 2}}},
				}),
			};

			WHEN("base damage is applied before external modifiers")
			{
				EffectContext context{makeEffectContext({.mMoveBasePower = 65, .mUserSide = Side::A, .mTargetSide = Side::B})};

				baseDamageHandler.apply(battleState, context, provider);

				THEN("the initial damage calculation follows each floor boundary")
				{
					CHECK(context.mDamage.mDamage == 49);
				}
			}

			WHEN("the hit is critical")
			{
				EffectContext context{
					makeEffectContext(
						{.mDamage = {.mIsCritical = true}, .mMoveBasePower = 65, .mUserSide = Side::A, .mTargetSide = Side::B}),
				};

				baseDamageHandler.apply(battleState, context, provider);

				THEN("the increased defense stat stages are ignored")
				{
					CHECK((context.mDamage.mDamage == 96));
				}
			}
		}

		GIVEN("user has decreased attack stages and target has increased defense stages")
		{
			BattleState battleState{
				makeBattleState({
					.mSideA = {{.mPokemon = &userPokemon, .mStatStages = {.mAttack = -4}}},
					.mSideB = {{.mPokemon = &targetPokemon, .mStatStages = {.mDefense = 2}}},
				}),
			};

			WHEN("base damage is applied before external modifiers")
			{
				EffectContext context{makeEffectContext({.mMoveBasePower = 65, .mUserSide = Side::A, .mTargetSide = Side::B})};

				baseDamageHandler.apply(battleState, context, provider);

				THEN("the initial damage calculation follows each floor boundary")
				{
					CHECK((context.mDamage.mDamage == 7));
				}
			}

			WHEN("the hit is critical")
			{
				EffectContext context{
					makeEffectContext(
						{.mDamage = {.mIsCritical = true}, .mMoveBasePower = 65, .mUserSide = Side::A, .mTargetSide = Side::B}),
				};

				baseDamageHandler.apply(battleState, context, provider);

				THEN("the increased defense stat stages and lowered attack stat stages are ignored")
				{
					CHECK((context.mDamage.mDamage == 33));
				}
			}
		}

		GIVEN("user has increased attack stages and target has decreased defense stages")
		{
			BattleState battleState{
				makeBattleState({
					.mSideA = {{.mPokemon = &userPokemon, .mStatStages = {.mAttack = 4}}},
					.mSideB = {{.mPokemon = &targetPokemon, .mStatStages = {.mDefense = -2}}},
				}),
			};

			WHEN("base damage is applied before external modifiers")
			{
				EffectContext context{makeEffectContext({.mMoveBasePower = 65, .mUserSide = Side::A, .mTargetSide = Side::B})};

				baseDamageHandler.apply(battleState, context, provider);

				THEN("the initial damage calculation follows each floor boundary")
				{
					CHECK((context.mDamage.mDamage == 190));
				}
			}

			WHEN("the hit is critical")
			{
				EffectContext context{
					makeEffectContext(
						{.mDamage = {.mIsCritical = true}, .mMoveBasePower = 65, .mUserSide = Side::A, .mTargetSide = Side::B}),
				};

				baseDamageHandler.apply(battleState, context, provider);

				THEN("the base damage does not change")
				{
					CHECK((context.mDamage.mDamage == 190));
				}
			}
		}

		GIVEN("user has increased special attack stages and target has increased special defense stages")
		{
			BattleState battleState{
				makeBattleState({
					.mSideA = {{.mPokemon = &userPokemon, .mStatStages = {.mSpAttack = 6}}},
					.mSideB = {{.mPokemon = &targetPokemon, .mStatStages = {.mSpDefense = 3}}},
				}),
			};

			WHEN("base damage is applied before external modifiers")
			{
				EffectContext context{
					makeEffectContext({.mMoveBasePower = 65, .mUserSide = Side::A, .mTargetSide = Side::B, .mIsSpecial = true}),
				};

				baseDamageHandler.apply(battleState, context, provider);

				THEN("the initial damage calculation follows each floor boundary")
				{
					CHECK((context.mDamage.mDamage == 55));
				}
			}

			WHEN("the hit is critical")
			{
				EffectContext context{
					makeEffectContext({
						.mDamage = {.mIsCritical = true},
						.mMoveBasePower = 65,
						.mUserSide = Side::A,
						.mTargetSide = Side::B,
						.mIsSpecial = true,
					}),
				};

				baseDamageHandler.apply(battleState, context, provider);

				THEN("the increased defense stat stages are ignored")
				{
					CHECK((context.mDamage.mDamage == 136));
				}
			}
		}

		GIVEN("user has decreased special attack stages and target has increased special defense stages")
		{
			BattleState battleState{
				makeBattleState({
					.mSideA = {{.mPokemon = &userPokemon, .mStatStages = {.mSpAttack = -2}}},
					.mSideB = {{.mPokemon = &targetPokemon, .mStatStages = {.mSpDefense = 3}}},
				}),
			};

			WHEN("base damage is applied before external modifiers")
			{
				EffectContext context{
					makeEffectContext({.mMoveBasePower = 65, .mUserSide = Side::A, .mTargetSide = Side::B, .mIsSpecial = true}),
				};

				baseDamageHandler.apply(battleState, context, provider);

				THEN("the initial damage calculation follows each floor boundary")
				{
					CHECK((context.mDamage.mDamage == 8));
				}
			}

			WHEN("the hit is critical")
			{
				EffectContext context{
					makeEffectContext({
						.mDamage = {.mIsCritical = true},
						.mMoveBasePower = 65,
						.mUserSide = Side::A,
						.mTargetSide = Side::B,
						.mIsSpecial = true,
					}),
				};

				baseDamageHandler.apply(battleState, context, provider);

				THEN("the increased defense stat stages and lowered attack stat stages are ignored")
				{
					CHECK((context.mDamage.mDamage == 35));
				}
			}
		}

		GIVEN("user has increased special attack stages and target has decreased special defense stages")
		{
			BattleState battleState{
				makeBattleState({
					.mSideA = {{.mPokemon = &userPokemon, .mStatStages = {.mSpAttack = 6}}},
					.mSideB = {{.mPokemon = &targetPokemon, .mStatStages = {.mSpDefense = -3}}},
				}),
			};

			WHEN("base damage is applied before external modifiers")
			{
				EffectContext context{
					makeEffectContext({.mMoveBasePower = 65, .mUserSide = Side::A, .mTargetSide = Side::B, .mIsSpecial = true}),
				};

				baseDamageHandler.apply(battleState, context, provider);

				THEN("the initial damage calculation follows each floor boundary")
				{
					CHECK((context.mDamage.mDamage == 337));
				}
			}

			WHEN("the hit is critical")
			{
				EffectContext context{
					makeEffectContext({
						.mDamage = {.mIsCritical = true},
						.mMoveBasePower = 65,
						.mUserSide = Side::A,
						.mTargetSide = Side::B,
						.mIsSpecial = true,
					}),
				};

				baseDamageHandler.apply(battleState, context, provider);

				THEN("the base damage does not change")
				{
					CHECK((context.mDamage.mDamage == 337));
				}
			}
		}
	}

	GIVEN("a damage calculation that exceeds the result type")
	{
		Pokemon userPokemon{makePokemon({.mAttack = 65'535, .mLevel = 65'535})};
		Pokemon targetPokemon{makePokemon({.mDefense = 1, .mLevel = 50})};

		BattleState battleState{makeBattleState({.mSideA = {{.mPokemon = &userPokemon}}, .mSideB = {{.mPokemon = &targetPokemon}}})};

		EffectContext context{makeEffectContext({.mMoveBasePower = 255, .mUserSide = Side::A, .mTargetSide = Side::B})};

		WHEN("base damage is applied before external modifiers")
		{
			baseDamageHandler.apply(battleState, context, provider);

			THEN("the initial damage calculation saturates at the largest representable value")
			{
				CHECK((context.mDamage.mDamage == std::numeric_limits<us>::max()));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
