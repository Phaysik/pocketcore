/*! @file baseDamageHandler.test.cpp
	@brief C++ file for running tests for the BaseDamageHandler.
	@date 10/27/2026
	@since 0.8.7
	@version 0.12.53
	@author Matthew Moore
*/

#include "EffectHandler/baseDamageHandler.h"

#include <limits>

#include "Battle/battleRandomSources.h"
#include "Battle/battleState.h"
#include "Battle/battleState.testHelper.h"
#include "Core/typedefs.h"
#include "Effect/effectContext.h"
#include "Effect/effectContext.testHelper.h"
#include "Pokemon/pokemon.h"
#include "Pokemon/pokemon.testHelper.h"
#include "Registry/registryProvider.h"
#include "Registry/registryProvider.testHelper.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Battle::BattleRandomSources;
using PocketCore::Battle::BattleState;
using PocketCore::Core::us;
using PocketCore::Effect::applyBaseDamage;
using PocketCore::Effect::BaseDamageHandler;
using PocketCore::Effect::EffectContext;
using PocketCore::Effect::Side;
using PocketCore::Pokemon::Pokemon;
using PocketCore::Registry::RegistryProvider;
using PocketCore::Testing::getDefaultInitializedRegistryProvider;
using PocketCore::Testing::makeBattleState;
using PocketCore::Testing::makeEffectContext;
using PocketCore::Testing::makePokemon;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

SCENARIO("BaseDamageHandler")
{
	Pokemon targetPokemon{makePokemon({})};
	Pokemon userPokemon{makePokemon({})};
	BaseDamageHandler baseDamageHandler{};
	RegistryProvider provider{getDefaultInitializedRegistryProvider()};
	BattleRandomSources randomSources{};

	BattleState handlerState{makeBattleState({.mSideA = {{.mPokemon = &userPokemon}}, .mSideB = {{.mPokemon = &targetPokemon}}})};
	BattleState freeFunctionState{makeBattleState({.mSideA = {{.mPokemon = &userPokemon}}, .mSideB = {{.mPokemon = &targetPokemon}}})};

	EffectContext handlerContext{makeEffectContext({.mMoveBasePower = 65, .mUserSide = Side::A, .mTargetSide = Side::B})};
	EffectContext freeFunctionContext{makeEffectContext({.mMoveBasePower = 65, .mUserSide = Side::A, .mTargetSide = Side::B})};

	GIVEN("invalid pokemon pointers")
	{
		GIVEN("null user pokemon")
		{
			handlerState.mSideA = {{.mPokemon = nullptr}};
			freeFunctionState.mSideA = {{.mPokemon = nullptr}};

			WHEN("base damage is applied before external modifiers")
			{
				baseDamageHandler.apply(handlerState, handlerContext, provider, randomSources);
				applyBaseDamage(freeFunctionState, freeFunctionContext, provider, randomSources);

				THEN("the initial damage returns 0 since no valid user")
				{
					CHECK((handlerContext.mDamage.mDamage == 0));
					CHECK((freeFunctionContext.mDamage.mDamage == 0));
				}
			}
		}

		GIVEN("null target pokemon")
		{
			handlerState.mSideB = {{.mPokemon = nullptr}};
			freeFunctionState.mSideB = {{.mPokemon = nullptr}};

			WHEN("base damage is applied before external modifiers")
			{
				baseDamageHandler.apply(handlerState, handlerContext, provider, randomSources);
				applyBaseDamage(freeFunctionState, freeFunctionContext, provider, randomSources);

				THEN("the initial damage returns 0 since no valid target")
				{
					CHECK((handlerContext.mDamage.mDamage == 0));
					CHECK((freeFunctionContext.mDamage.mDamage == 0));
				}
			}
		}
	}

	GIVEN("no move base power")
	{
		userPokemon.setLevel(75);
		userPokemon.setAttack(123);
		targetPokemon.setLevel(50);
		targetPokemon.setDefense(163);

		handlerContext.mMoveBasePower = 0;
		freeFunctionContext.mMoveBasePower = 0;

		WHEN("base damage is applied before external modifiers")
		{
			baseDamageHandler.apply(handlerState, handlerContext, provider, randomSources);
			applyBaseDamage(freeFunctionState, freeFunctionContext, provider, randomSources);

			THEN("the initial damage returns 0 since no move base power")
			{
				CHECK((handlerContext.mDamage.mDamage == 0));
				CHECK((freeFunctionContext.mDamage.mDamage == 0));
			}
		}
	}

	GIVEN("a user with a non-finite modifiers")
	{
		userPokemon.setLevel(75);
		userPokemon.setAttack(123);
		targetPokemon.setLevel(50);
		targetPokemon.setDefense(163);

		GIVEN("specifically the attack modifier")
		{
			handlerState.mSideA = {
				{
					.mDamageFormulaModifiers = {.mAttackModifier = std::numeric_limits<double>::quiet_NaN()},
					.mPokemon = &userPokemon,
				},
			};

			freeFunctionState.mSideA = {
				{
					.mDamageFormulaModifiers = {.mAttackModifier = std::numeric_limits<double>::quiet_NaN()},
					.mPokemon = &userPokemon,
				},
			};

			WHEN("base damage is applied before external modifiers")
			{
				baseDamageHandler.apply(handlerState, handlerContext, provider, randomSources);
				applyBaseDamage(freeFunctionState, freeFunctionContext, provider, randomSources);

				THEN("the initial damage returns 0 since non-finite attack")
				{
					CHECK((handlerContext.mDamage.mDamage == 0));
					CHECK((freeFunctionContext.mDamage.mDamage == 0));
				}
			}
		}

		GIVEN("specifically the defense modifier")
		{
			handlerState.mSideB = {
				{
					.mDamageFormulaModifiers = {.mDefenseModifier = std::numeric_limits<double>::quiet_NaN()},
					.mPokemon = &targetPokemon,
				},
			};

			freeFunctionState.mSideB = {
				{
					.mDamageFormulaModifiers = {.mDefenseModifier = std::numeric_limits<double>::quiet_NaN()},
					.mPokemon = &targetPokemon,
				},
			};

			WHEN("base damage is applied before external modifiers")
			{
				baseDamageHandler.apply(handlerState, handlerContext, provider, randomSources);
				applyBaseDamage(freeFunctionState, freeFunctionContext, provider, randomSources);

				THEN("the initial damage returns 0 since non-finite defense")
				{
					CHECK((handlerContext.mDamage.mDamage == 0));
					CHECK((freeFunctionContext.mDamage.mDamage == 0));
				}
			}
		}
	}

	GIVEN("a user has a positive attack stat")
	{
		userPokemon.setLevel(75);
		userPokemon.setAttack(123);
		targetPokemon.setLevel(50);
		targetPokemon.setDefense(163);

		GIVEN("with a negative attack modifier")
		{
			handlerState.mSideA = {
				{
					.mDamageFormulaModifiers = {.mAttackModifier = -1.0},
					.mPokemon = &userPokemon,
				},
			};

			freeFunctionState.mSideA = {
				{
					.mDamageFormulaModifiers = {.mAttackModifier = -1.0},
					.mPokemon = &userPokemon,
				},
			};

			WHEN("base damage is applied before external modifiers")
			{
				baseDamageHandler.apply(handlerState, handlerContext, provider, randomSources);
				applyBaseDamage(freeFunctionState, freeFunctionContext, provider, randomSources);

				THEN("the initial damage returns 0 since the attack was below 0")
				{
					CHECK((handlerContext.mDamage.mDamage == 0));
					CHECK((freeFunctionContext.mDamage.mDamage == 0));
				}
			}
		}

		GIVEN("with a 0 attack modifier")
		{
			handlerState.mSideA = {
				{
					.mDamageFormulaModifiers = {.mAttackModifier = 0},
					.mPokemon = &userPokemon,
				},
			};

			freeFunctionState.mSideA = {
				{
					.mDamageFormulaModifiers = {.mAttackModifier = 0},
					.mPokemon = &userPokemon,
				},
			};

			WHEN("base damage is applied before external modifiers")
			{
				baseDamageHandler.apply(handlerState, handlerContext, provider, randomSources);
				applyBaseDamage(freeFunctionState, freeFunctionContext, provider, randomSources);

				THEN("the initial damage returns 0 since the attack was 0")
				{
					CHECK((handlerContext.mDamage.mDamage == 0));
					CHECK((freeFunctionContext.mDamage.mDamage == 0));
				}
			}
		}
	}

	GIVEN("the Generation V onward")
	{
		userPokemon.setLevel(75);
		userPokemon.setAttack(123);
		userPokemon.setSpAttack(100);
		targetPokemon.setLevel(50);
		targetPokemon.setDefense(163);
		targetPokemon.setSpDefense(124);

		GIVEN("Bulbapedia base-damage attack example")
		{
			WHEN("base damage is applied before external modifiers")
			{
				baseDamageHandler.apply(handlerState, handlerContext, provider, randomSources);
				applyBaseDamage(freeFunctionState, freeFunctionContext, provider, randomSources);

				THEN("the initial damage calculation follows each floor boundary")
				{
					CHECK((handlerContext.mDamage.mDamage == 48));
					CHECK((freeFunctionContext.mDamage.mDamage == 48));
				}
			}
		}

		GIVEN("example using special attack")
		{
			handlerContext.mIsSpecial = true;
			freeFunctionContext.mIsSpecial = true;

			WHEN("base damage is applied before external modifiers")
			{
				baseDamageHandler.apply(handlerState, handlerContext, provider, randomSources);
				applyBaseDamage(freeFunctionState, freeFunctionContext, provider, randomSources);

				THEN("the initial damage calculation follows each floor boundary")
				{
					CHECK((handlerContext.mDamage.mDamage == 51));
					CHECK((freeFunctionContext.mDamage.mDamage == 51));
				}
			}
		}
	}

	GIVEN("modified stat stages")
	{
		userPokemon.setLevel(75);
		userPokemon.setAttack(123);
		userPokemon.setSpAttack(100);
		targetPokemon.setLevel(50);
		targetPokemon.setDefense(163);
		targetPokemon.setSpDefense(124);

		GIVEN("user has increased attack stages and target has increased defense stages")
		{
			handlerState.mSideA = {
				{
					.mPokemon = &userPokemon,
					.mStatStages = {.mAttack = 4},
				},
			};

			handlerState.mSideB = {
				{
					.mPokemon = &targetPokemon,
					.mStatStages = {.mDefense = 2},
				},
			};

			freeFunctionState.mSideA = {
				{
					.mPokemon = &userPokemon,
					.mStatStages = {.mAttack = 4},
				},
			};

			freeFunctionState.mSideB = {
				{
					.mPokemon = &targetPokemon,
					.mStatStages = {.mDefense = 2},
				},
			};

			WHEN("base damage is applied before external modifiers")
			{
				baseDamageHandler.apply(handlerState, handlerContext, provider, randomSources);
				applyBaseDamage(freeFunctionState, freeFunctionContext, provider, randomSources);

				THEN("the initial damage calculation follows each floor boundary")
				{
					CHECK((handlerContext.mDamage.mDamage == 72));
					CHECK((freeFunctionContext.mDamage.mDamage == 72));
				}
			}

			WHEN("the hit is critical")
			{
				handlerContext.mDamage = {.mIsCritical = true};
				freeFunctionContext.mDamage = {.mIsCritical = true};

				baseDamageHandler.apply(handlerState, handlerContext, provider, randomSources);
				applyBaseDamage(freeFunctionState, freeFunctionContext, provider, randomSources);

				THEN("the increased defense stat stages are ignored")
				{
					CHECK((handlerContext.mDamage.mDamage == 142));
					CHECK((freeFunctionContext.mDamage.mDamage == 142));
				}
			}
		}

		GIVEN("user has decreased attack stages and target has increased defense stages")
		{
			handlerState.mSideA = {
				{
					.mPokemon = &userPokemon,
					.mStatStages = {.mAttack = -4},
				},
			};

			handlerState.mSideB = {
				{
					.mPokemon = &targetPokemon,
					.mStatStages = {.mDefense = 2},
				},
			};

			freeFunctionState.mSideA = {
				{
					.mPokemon = &userPokemon,
					.mStatStages = {.mAttack = -4},
				},
			};

			freeFunctionState.mSideB = {
				{
					.mPokemon = &targetPokemon,
					.mStatStages = {.mDefense = 2},
				},
			};

			WHEN("base damage is applied before external modifiers")
			{
				baseDamageHandler.apply(handlerState, handlerContext, provider, randomSources);
				applyBaseDamage(freeFunctionState, freeFunctionContext, provider, randomSources);

				THEN("the initial damage calculation follows each floor boundary")
				{
					CHECK((handlerContext.mDamage.mDamage == 9));
					CHECK((freeFunctionContext.mDamage.mDamage == 9));
				}
			}

			WHEN("the hit is critical")
			{
				handlerContext.mDamage = {.mIsCritical = true};
				freeFunctionContext.mDamage = {.mIsCritical = true};

				baseDamageHandler.apply(handlerState, handlerContext, provider, randomSources);
				applyBaseDamage(freeFunctionState, freeFunctionContext, provider, randomSources);

				THEN("the increased defense stat stages and lowered attack stat stages are ignored")
				{
					CHECK((handlerContext.mDamage.mDamage == 48));
					CHECK((freeFunctionContext.mDamage.mDamage == 48));
				}
			}
		}

		GIVEN("user has increased attack stages and target has decreased defense stages")
		{
			handlerState.mSideA = {
				{
					.mPokemon = &userPokemon,
					.mStatStages = {.mAttack = 4},
				},
			};

			handlerState.mSideB = {
				{
					.mPokemon = &targetPokemon,
					.mStatStages = {.mDefense = -2},
				},
			};

			freeFunctionState.mSideA = {
				{
					.mPokemon = &userPokemon,
					.mStatStages = {.mAttack = 4},
				},
			};

			freeFunctionState.mSideB = {
				{
					.mPokemon = &targetPokemon,
					.mStatStages = {.mDefense = -2},
				},
			};

			WHEN("base damage is applied before external modifiers")
			{
				baseDamageHandler.apply(handlerState, handlerContext, provider, randomSources);
				applyBaseDamage(freeFunctionState, freeFunctionContext, provider, randomSources);

				THEN("the initial damage calculation follows each floor boundary")
				{
					CHECK((handlerContext.mDamage.mDamage == 282));
					CHECK((freeFunctionContext.mDamage.mDamage == 282));
				}
			}

			WHEN("the hit is critical")
			{
				handlerContext.mDamage = {.mIsCritical = true};
				freeFunctionContext.mDamage = {.mIsCritical = true};

				baseDamageHandler.apply(handlerState, handlerContext, provider, randomSources);
				applyBaseDamage(freeFunctionState, freeFunctionContext, provider, randomSources);

				THEN("the base damage does not change")
				{
					CHECK((handlerContext.mDamage.mDamage == 282));
					CHECK((freeFunctionContext.mDamage.mDamage == 282));
				}
			}
		}

		GIVEN("user has increased special attack stages and target has increased special defense stages")
		{
			handlerState.mSideA = {
				{
					.mPokemon = &userPokemon,
					.mStatStages = {.mSpAttack = 6},
				},
			};

			handlerState.mSideB = {
				{
					.mPokemon = &targetPokemon,
					.mStatStages = {.mSpDefense = 3},
				},
			};

			freeFunctionState.mSideA = {
				{
					.mPokemon = &userPokemon,
					.mStatStages = {.mSpAttack = 6},
				},
			};

			freeFunctionState.mSideB = {
				{
					.mPokemon = &targetPokemon,
					.mStatStages = {.mSpDefense = 3},
				},
			};

			WHEN("base damage is applied before external modifiers")
			{
				handlerContext.mIsSpecial = true;
				freeFunctionContext.mIsSpecial = true;

				baseDamageHandler.apply(handlerState, handlerContext, provider, randomSources);
				applyBaseDamage(freeFunctionState, freeFunctionContext, provider, randomSources);

				THEN("the initial damage calculation follows each floor boundary")
				{
					CHECK((handlerContext.mDamage.mDamage == 81));
					CHECK((freeFunctionContext.mDamage.mDamage == 81));
				}
			}

			WHEN("the hit is critical")
			{
				handlerContext.mDamage = {.mIsCritical = true};
				handlerContext.mIsSpecial = true;
				freeFunctionContext.mDamage = {.mIsCritical = true};
				freeFunctionContext.mIsSpecial = true;

				baseDamageHandler.apply(handlerState, handlerContext, provider, randomSources);
				applyBaseDamage(freeFunctionState, freeFunctionContext, provider, randomSources);

				THEN("the increased defense stat stages are ignored")
				{
					CHECK((handlerContext.mDamage.mDamage == 201));
					CHECK((freeFunctionContext.mDamage.mDamage == 201));
				}
			}
		}

		GIVEN("user has decreased special attack stages and target has increased special defense stages")
		{
			handlerState.mSideA = {
				{
					.mPokemon = &userPokemon,
					.mStatStages = {.mSpAttack = -2},
				},
			};

			handlerState.mSideB = {
				{
					.mPokemon = &targetPokemon,
					.mStatStages = {.mSpDefense = 3},
				},
			};

			freeFunctionState.mSideA = {
				{
					.mPokemon = &userPokemon,
					.mStatStages = {.mSpAttack = -2},
				},
			};

			freeFunctionState.mSideB = {
				{
					.mPokemon = &targetPokemon,
					.mStatStages = {.mSpDefense = 3},
				},
			};

			WHEN("base damage is applied before external modifiers")
			{
				handlerContext.mIsSpecial = true;
				freeFunctionContext.mIsSpecial = true;

				baseDamageHandler.apply(handlerState, handlerContext, provider, randomSources);
				applyBaseDamage(freeFunctionState, freeFunctionContext, provider, randomSources);

				THEN("the initial damage calculation follows each floor boundary")
				{
					CHECK((handlerContext.mDamage.mDamage == 11));
					CHECK((freeFunctionContext.mDamage.mDamage == 11));
				}
			}

			WHEN("the hit is critical")
			{
				handlerContext.mDamage = {.mIsCritical = true};
				handlerContext.mIsSpecial = true;
				freeFunctionContext.mDamage = {.mIsCritical = true};
				freeFunctionContext.mIsSpecial = true;

				baseDamageHandler.apply(handlerState, handlerContext, provider, randomSources);
				applyBaseDamage(freeFunctionState, freeFunctionContext, provider, randomSources);

				THEN("the increased defense stat stages and lowered attack stat stages are ignored")
				{
					CHECK((handlerContext.mDamage.mDamage == 51));
					CHECK((freeFunctionContext.mDamage.mDamage == 51));
				}
			}
		}

		GIVEN("user has increased special attack stages and target has decreased special defense stages")
		{
			handlerState.mSideA = {
				{
					.mPokemon = &userPokemon,
					.mStatStages = {.mSpAttack = 6},
				},
			};

			handlerState.mSideB = {
				{
					.mPokemon = &targetPokemon,
					.mStatStages = {.mSpDefense = -3},
				},
			};

			freeFunctionState.mSideA = {
				{
					.mPokemon = &userPokemon,
					.mStatStages = {.mSpAttack = 6},
				},
			};

			freeFunctionState.mSideB = {
				{
					.mPokemon = &targetPokemon,
					.mStatStages = {.mSpDefense = -3},
				},
			};

			WHEN("base damage is applied before external modifiers")
			{
				handlerContext.mIsSpecial = true;
				freeFunctionContext.mIsSpecial = true;

				baseDamageHandler.apply(handlerState, handlerContext, provider, randomSources);
				applyBaseDamage(freeFunctionState, freeFunctionContext, provider, randomSources);

				THEN("the initial damage calculation follows each floor boundary")
				{
					CHECK((handlerContext.mDamage.mDamage == 501));
					CHECK((freeFunctionContext.mDamage.mDamage == 501));
				}
			}

			WHEN("the hit is critical")
			{
				handlerContext.mDamage = {.mIsCritical = true};
				handlerContext.mIsSpecial = true;
				freeFunctionContext.mDamage = {.mIsCritical = true};
				freeFunctionContext.mIsSpecial = true;

				baseDamageHandler.apply(handlerState, handlerContext, provider, randomSources);
				applyBaseDamage(freeFunctionState, freeFunctionContext, provider, randomSources);

				THEN("the base damage does not change")
				{
					CHECK((handlerContext.mDamage.mDamage == 501));
					CHECK((freeFunctionContext.mDamage.mDamage == 501));
				}
			}
		}
	}

	GIVEN("a damage calculation that exceeds the result type")
	{
		userPokemon.setLevel(65'535);
		userPokemon.setAttack(65'535);
		targetPokemon.setLevel(50);
		targetPokemon.setDefense(1);

		handlerContext.mMoveBasePower = 255;
		freeFunctionContext.mMoveBasePower = 255;

		WHEN("base damage is applied before external modifiers")
		{
			baseDamageHandler.apply(handlerState, handlerContext, provider, randomSources);
			applyBaseDamage(freeFunctionState, freeFunctionContext, provider, randomSources);

			THEN("the initial damage calculation saturates at the largest representable value")
			{
				CHECK((handlerContext.mDamage.mDamage == std::numeric_limits<us>::max()));
				CHECK((freeFunctionContext.mDamage.mDamage == std::numeric_limits<us>::max()));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
