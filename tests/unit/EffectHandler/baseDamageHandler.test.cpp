/*! @file baseDamageHandler.test.cpp
	@brief C++ file for running tests for the BaseDamageHandler.
	@date 08/03/2026
	@since 0.8.7
	@version 0.9.0
	@author Matthew Moore
*/

#include "EffectHandler/baseDamageHandler.h"

#include <limits>

#include "Ability/abilityID.h"
#include "Battle/battleState.h"
#include "Core/typedefs.h"
#include "Effect/effectContext.h"
#include "Item/itemID.h"
#include "Pokemon/pokemon.h"
#include "Registry/registryProvider.h"
#include "Types/typeID.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Ability::NO_ABILITY_ID;
using PocketCore::Battle::BattleSlot;
using PocketCore::Battle::BattleState;
using PocketCore::Battle::DamageFormulaModifiers;
using PocketCore::Battle::StatStages;
using PocketCore::Core::us;
using PocketCore::Effect::BaseDamageHandler;
using PocketCore::Effect::EffectContext;
using PocketCore::Effect::Side;
using PocketCore::Item::NO_ITEM_ID;
using PocketCore::Pokemon::Pokemon;
using PocketCore::Registry::RegistryProvider;
using PocketCore::Type::NO_TYPE_ID;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity,llvm-prefer-static-over-anonymous-namespace))

namespace
{
	RegistryProvider makeNullProvider()
	{
		return RegistryProvider{
			.abilityRegistry = nullptr,
			.moveRegistry = nullptr,
			.itemRegistry = nullptr,
			.typeRegistry = nullptr,
			.statusRegistry = nullptr,
			.weatherRegistry = nullptr,
			.terrainRegistry = nullptr,
			.multiplierRegistry = nullptr,
			.natureRegistry = nullptr,
			.pokemonRegistry = nullptr,
		};
	}

	Pokemon makePokemon(const us level, const us attack, const us defense, const us specialAttack, const us specialDefense)
	{
		return Pokemon{"UnitMon",
					   attack,
					   defense,
					   100U,
					   80U,
					   specialAttack,
					   specialDefense,
					   level,
					   {NO_ABILITY_ID},
					   {NO_ITEM_ID},
					   {NO_TYPE_ID, NO_TYPE_ID}};
	}

} // namespace

SCENARIO("BaseDamageHandler")
{
	BaseDamageHandler baseDamageHandler{};
	RegistryProvider provider{makeNullProvider()};

	GIVEN("the Generation V onward Bulbapedia base-damage example")
	{
		Pokemon userPokemon{makePokemon(75U, 123U, 90U, 100U, 95U)};
		Pokemon targetPokemon{makePokemon(50U, 90U, 163U, 85U, 105U)};

		BattleState battleState{};
		battleState.mSideA.push_back(BattleSlot{.mPokemon = &userPokemon});
		battleState.mSideB.push_back(BattleSlot{.mPokemon = &targetPokemon});

		EffectContext effectContext{};
		effectContext.mUserSide = Side::A;
		effectContext.mTargetSide = Side::B;
		effectContext.mMoveBasePower = 65U;

		WHEN("base damage is applied before external modifiers")
		{
			baseDamageHandler.apply(battleState, effectContext, provider);

			THEN("the initial damage calculation follows each floor boundary")
			{
				CHECK((effectContext.mDamage.mDamage == 33U));
			}
		}
	}

	GIVEN("missing Pokemon data or zero move power")
	{
		Pokemon userPokemon{makePokemon(50U, 120U, 90U, 110U, 100U)};

		BattleState battleState{};
		battleState.mSideA.push_back(BattleSlot{.mPokemon = &userPokemon});
		battleState.mSideB.push_back(BattleSlot{.mPokemon = nullptr});

		EffectContext effectContext{};
		effectContext.mUserSide = Side::A;
		effectContext.mTargetSide = Side::B;
		effectContext.mUserIndex = 0;
		effectContext.mTargetIndex = 0;
		effectContext.mMoveBasePower = 0;
		effectContext.mDamage.mDamage = 777U;

		WHEN("base damage is applied")
		{
			baseDamageHandler.apply(battleState, effectContext, provider);

			THEN("damage remains unchanged")
			{
				CHECK((effectContext.mDamage.mDamage == 777U));
			}
		}
	}

	GIVEN("a physical attack with valid user and target stats")
	{
		Pokemon userPokemon{makePokemon(50U, 120U, 90U, 100U, 95U)};
		Pokemon targetPokemon{makePokemon(50U, 90U, 110U, 85U, 105U)};

		BattleSlot userSlot{};
		userSlot.mPokemon = &userPokemon;
		userSlot.mStatStages
			= StatStages{.mAttack = 2, .mDefense = 0, .mSpAttack = 0, .mSpDefense = 0, .mSpeed = 0, .mAccuracy = 0, .mEvasion = 0};
		userSlot.mDamageFormulaModifiers = DamageFormulaModifiers{
			.mHealthModifier = 1.0,
			.mAttackModifier = 1.1,
			.mDefenseModifier = 1.0,
			.mSpecialAttackModifier = 1.0,
			.mSpecialDefenseModifier = 1.0,
			.mSpeedModifier = 1.0,
		};

		BattleSlot targetSlot{};
		targetSlot.mPokemon = &targetPokemon;
		targetSlot.mStatStages
			= StatStages{.mAttack = 0, .mDefense = 1, .mSpAttack = 0, .mSpDefense = 0, .mSpeed = 0, .mAccuracy = 0, .mEvasion = 0};
		targetSlot.mDamageFormulaModifiers = DamageFormulaModifiers{
			.mHealthModifier = 1.0,
			.mAttackModifier = 1.0,
			.mDefenseModifier = 0.9,
			.mSpecialAttackModifier = 1.0,
			.mSpecialDefenseModifier = 1.0,
			.mSpeedModifier = 1.0,
		};

		BattleState battleState{};
		battleState.mSideA.push_back(userSlot);
		battleState.mSideB.push_back(targetSlot);

		EffectContext effectContext{};
		effectContext.mUserSide = Side::A;
		effectContext.mTargetSide = Side::B;
		effectContext.mUserIndex = 0;
		effectContext.mTargetIndex = 0;
		effectContext.mMoveBasePower = 80U;
		effectContext.mIsSpecial = false;

		WHEN("base damage is applied")
		{
			baseDamageHandler.apply(battleState, effectContext, provider);

			THEN("damage matches the formula")
			{
				CHECK((effectContext.mDamage.mDamage >= 1U));
			}
		}
	}

	GIVEN("a critical special attack with unfavorable user and favorable target stages")
	{
		Pokemon userPokemon{makePokemon(50U, 100U, 90U, 130U, 95U)};
		Pokemon targetPokemon{makePokemon(50U, 90U, 100U, 85U, 120U)};

		BattleSlot userSlot{};
		userSlot.mPokemon = &userPokemon;
		userSlot.mStatStages
			= StatStages{.mAttack = 0, .mDefense = 0, .mSpAttack = 1, .mSpDefense = 0, .mSpeed = 0, .mAccuracy = 0, .mEvasion = 0};
		userSlot.mDamageFormulaModifiers = DamageFormulaModifiers{
			.mHealthModifier = 1.0,
			.mAttackModifier = 1.0,
			.mDefenseModifier = 1.0,
			.mSpecialAttackModifier = 1.0,
			.mSpecialDefenseModifier = 1.0,
			.mSpeedModifier = 1.0,
		};

		BattleSlot targetSlot{};
		targetSlot.mPokemon = &targetPokemon;
		targetSlot.mStatStages
			= StatStages{.mAttack = 0, .mDefense = 0, .mSpAttack = 0, .mSpDefense = 6, .mSpeed = 0, .mAccuracy = 0, .mEvasion = 0};
		targetSlot.mDamageFormulaModifiers = DamageFormulaModifiers{
			.mHealthModifier = 1.0,
			.mAttackModifier = 1.0,
			.mDefenseModifier = 1.0,
			.mSpecialAttackModifier = 1.0,
			.mSpecialDefenseModifier = 1.0,
			.mSpeedModifier = 1.0,
		};

		BattleState battleState{};
		battleState.mSideA.push_back(userSlot);
		battleState.mSideB.push_back(targetSlot);

		EffectContext criticalContext{};
		criticalContext.mUserSide = Side::A;
		criticalContext.mTargetSide = Side::B;
		criticalContext.mUserIndex = 0;
		criticalContext.mTargetIndex = 0;
		criticalContext.mMoveBasePower = 90U;
		criticalContext.mIsSpecial = true;
		criticalContext.mDamage.mIsCritical = true;

		EffectContext nonCriticalContext{criticalContext};
		nonCriticalContext.mDamage.mIsCritical = false;

		WHEN("base damage is applied")
		{
			baseDamageHandler.apply(battleState, criticalContext, provider);
			baseDamageHandler.apply(battleState, nonCriticalContext, provider);

			THEN("critical damage uses clamped stages and is not lower than non-critical")
			{
				CHECK((criticalContext.mDamage.mDamage >= 1U));
				CHECK((nonCriticalContext.mDamage.mDamage >= 1U));
				CHECK((criticalContext.mDamage.mDamage >= nonCriticalContext.mDamage.mDamage));
			}
		}
	}

	GIVEN("a valid user but missing target and non-zero move power")
	{
		Pokemon userPokemon{makePokemon(50U, 120U, 90U, 110U, 100U)};

		BattleState battleState{};
		battleState.mSideA.push_back(BattleSlot{.mPokemon = &userPokemon});
		battleState.mSideB.push_back(BattleSlot{.mPokemon = nullptr});

		EffectContext effectContext{};
		effectContext.mUserSide = Side::A;
		effectContext.mTargetSide = Side::B;
		effectContext.mUserIndex = 0;
		effectContext.mTargetIndex = 0;
		effectContext.mMoveBasePower = 60U;
		effectContext.mDamage.mDamage = 555U;

		WHEN("base damage is applied")
		{
			baseDamageHandler.apply(battleState, effectContext, provider);

			THEN("the guard returns before changing damage")
			{
				CHECK((effectContext.mDamage.mDamage == 555U));
			}
		}
	}

	GIVEN("valid Pokemon pointers but zero move power")
	{
		Pokemon userPokemon{makePokemon(50U, 120U, 90U, 110U, 100U)};
		Pokemon targetPokemon{makePokemon(50U, 90U, 110U, 85U, 105U)};

		BattleState battleState{};
		battleState.mSideA.push_back(BattleSlot{.mPokemon = &userPokemon});
		battleState.mSideB.push_back(BattleSlot{.mPokemon = &targetPokemon});

		EffectContext effectContext{};
		effectContext.mUserSide = Side::A;
		effectContext.mTargetSide = Side::B;
		effectContext.mUserIndex = 0;
		effectContext.mTargetIndex = 0;
		effectContext.mMoveBasePower = 0U;
		effectContext.mDamage.mDamage = 444U;

		WHEN("base damage is applied")
		{
			baseDamageHandler.apply(battleState, effectContext, provider);

			THEN("the guard returns before damage math")
			{
				CHECK((effectContext.mDamage.mDamage == 444U));
			}
		}
	}

	GIVEN("critical damage with negative stages")
	{
		Pokemon userPokemon{makePokemon(50U, 110U, 90U, 130U, 95U)};
		Pokemon targetPokemon{makePokemon(50U, 90U, 100U, 85U, 120U)};

		BattleSlot userSlot{};
		userSlot.mPokemon = &userPokemon;
		userSlot.mStatStages = StatStages{
			.mAttack = 0,
			.mDefense = 0,
			.mSpAttack = static_cast<signed char>(-1),
			.mSpDefense = 0,
			.mSpeed = 0,
			.mAccuracy = 0,
			.mEvasion = 0,
		};

		BattleSlot targetSlot{};
		targetSlot.mPokemon = &targetPokemon;
		targetSlot.mStatStages = StatStages{
			.mAttack = 0,
			.mDefense = 0,
			.mSpAttack = 0,
			.mSpDefense = static_cast<signed char>(-1),
			.mSpeed = 0,
			.mAccuracy = 0,
			.mEvasion = 0,
		};

		BattleState battleState{};
		battleState.mSideA.push_back(userSlot);
		battleState.mSideB.push_back(targetSlot);

		EffectContext effectContext{};
		effectContext.mUserSide = Side::A;
		effectContext.mTargetSide = Side::B;
		effectContext.mUserIndex = 0;
		effectContext.mTargetIndex = 0;
		effectContext.mMoveBasePower = 90U;
		effectContext.mIsSpecial = true;
		effectContext.mDamage.mIsCritical = true;

		WHEN("base damage is applied")
		{
			baseDamageHandler.apply(battleState, effectContext, provider);

			THEN("valid negative stages produce damage without throwing")
			{
				CHECK((effectContext.mDamage.mDamage >= 1U));
			}
		}
	}

	GIVEN("missing user Pokemon pointer")
	{
		Pokemon targetPokemon{makePokemon(50U, 90U, 110U, 85U, 105U)};

		BattleState battleState{};
		battleState.mSideA.push_back(BattleSlot{.mPokemon = nullptr});
		battleState.mSideB.push_back(BattleSlot{.mPokemon = &targetPokemon});

		EffectContext effectContext{};
		effectContext.mUserSide = Side::A;
		effectContext.mTargetSide = Side::B;
		effectContext.mUserIndex = 0;
		effectContext.mTargetIndex = 0;
		effectContext.mMoveBasePower = 70U;
		effectContext.mDamage.mDamage = 333U;

		WHEN("base damage is applied")
		{
			baseDamageHandler.apply(battleState, effectContext, provider);

			THEN("the guard short-circuits on user pointer")
			{
				CHECK((effectContext.mDamage.mDamage == 333U));
			}
		}
	}

	GIVEN("non-critical special damage with a negative user special attack stage")
	{
		Pokemon userPokemon{makePokemon(50U, 110U, 90U, 130U, 95U)};
		Pokemon targetPokemon{makePokemon(50U, 90U, 100U, 85U, 120U)};

		BattleSlot userSlot{};
		userSlot.mPokemon = &userPokemon;
		userSlot.mStatStages = StatStages{
			.mAttack = 0,
			.mDefense = 0,
			.mSpAttack = static_cast<signed char>(-1),
			.mSpDefense = 0,
			.mSpeed = 0,
			.mAccuracy = 0,
			.mEvasion = 0,
		};

		BattleSlot targetSlot{};
		targetSlot.mPokemon = &targetPokemon;
		targetSlot.mStatStages
			= StatStages{.mAttack = 0, .mDefense = 0, .mSpAttack = 0, .mSpDefense = 0, .mSpeed = 0, .mAccuracy = 0, .mEvasion = 0};

		BattleState battleState{};
		battleState.mSideA.push_back(userSlot);
		battleState.mSideB.push_back(targetSlot);

		EffectContext effectContext{};
		effectContext.mUserSide = Side::A;
		effectContext.mTargetSide = Side::B;
		effectContext.mUserIndex = 0;
		effectContext.mTargetIndex = 0;
		effectContext.mMoveBasePower = 90U;
		effectContext.mIsSpecial = true;
		effectContext.mDamage.mIsCritical = false;

		EffectContext neutralContext{effectContext};
		battleState.mSideA.front().mStatStages.mSpAttack = 0;

		WHEN("base damage is applied")
		{
			baseDamageHandler.apply(battleState, neutralContext, provider);
			battleState.mSideA.front().mStatStages.mSpAttack = static_cast<signed char>(-1);
			baseDamageHandler.apply(battleState, effectContext, provider);

			THEN("the negative stage reduces damage without throwing")
			{
				CHECK((effectContext.mDamage.mDamage < neutralContext.mDamage.mDamage));
			}
		}
	}

	GIVEN("a target with zero defense")
	{
		Pokemon userPokemon{makePokemon(50U, 120U, 90U, 100U, 95U)};
		Pokemon targetPokemon{makePokemon(50U, 90U, 0U, 85U, 105U)};

		BattleState battleState{};
		battleState.mSideA.push_back(BattleSlot{.mPokemon = &userPokemon});
		battleState.mSideB.push_back(BattleSlot{.mPokemon = &targetPokemon});

		EffectContext effectContext{};
		effectContext.mUserSide = Side::A;
		effectContext.mTargetSide = Side::B;
		effectContext.mMoveBasePower = 80U;
		effectContext.mDamage.mDamage = 777U;

		WHEN("base damage is applied")
		{
			baseDamageHandler.apply(battleState, effectContext, provider);

			THEN("invalid defense leaves damage unchanged")
			{
				CHECK((effectContext.mDamage.mDamage == 777U));
			}
		}
	}

	GIVEN("a damage calculation that exceeds the result type")
	{
		Pokemon userPokemon{makePokemon(65'535U, 65'535U, 90U, 65'535U, 95U)};
		Pokemon targetPokemon{makePokemon(50U, 90U, 1U, 85U, 1U)};

		BattleState battleState{};
		battleState.mSideA.push_back(BattleSlot{.mPokemon = &userPokemon});
		battleState.mSideB.push_back(BattleSlot{.mPokemon = &targetPokemon});

		EffectContext effectContext{};
		effectContext.mUserSide = Side::A;
		effectContext.mTargetSide = Side::B;
		effectContext.mMoveBasePower = 255U;

		WHEN("base damage is applied")
		{
			baseDamageHandler.apply(battleState, effectContext, provider);

			THEN("damage saturates at the largest representable value")
			{
				CHECK((effectContext.mDamage.mDamage == std::numeric_limits<unsigned short>::max()));
			}
		}
	}

	GIVEN("stat stages outside the supported range")
	{
		Pokemon userPokemon{makePokemon(50U, 120U, 90U, 100U, 95U)};
		Pokemon targetPokemon{makePokemon(50U, 90U, 110U, 85U, 105U)};

		BattleState battleState{};
		battleState.mSideA.push_back(BattleSlot{
			.mPokemon = &userPokemon,
			.mStatStages = StatStages{.mAttack = std::numeric_limits<signed char>::max()},
		});
		battleState.mSideB.push_back(BattleSlot{
			.mPokemon = &targetPokemon,
			.mStatStages = StatStages{.mDefense = std::numeric_limits<signed char>::min()},
		});

		EffectContext effectContext{};
		effectContext.mUserSide = Side::A;
		effectContext.mTargetSide = Side::B;
		effectContext.mMoveBasePower = 80U;

		WHEN("base damage is applied")
		{
			baseDamageHandler.apply(battleState, effectContext, provider);

			THEN("the stages are clamped to valid cache bounds")
			{
				CHECK((effectContext.mDamage.mDamage >= 1U));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity,llvm-prefer-static-over-anonymous-namespace))
