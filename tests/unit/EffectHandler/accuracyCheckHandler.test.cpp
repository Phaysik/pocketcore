#include "EffectHandler/accuracyCheckHandler.h"

#include <limits>

#include "Ability/abilityID.h"
#include "Battle/battleState.h"
#include "Configuration/constants.h"
#include "Effect/effectContext.h"
#include "Item/itemID.h"
#include "Pokemon/pokemon.h"
#include "Registry/registryProvider.h"
#include "Types/typeID.h"
#include "Utility/random.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Ability::NO_ABILITY_ID;
using PocketCore::Battle::BattleSlot;
using PocketCore::Battle::BattleState;
using PocketCore::Battle::StatStages;
using PocketCore::Configuration::MAX_ACCURACY_HIT_VALUE;
using PocketCore::Configuration::MIN_ACCURACY_HIT_VALUE;
using PocketCore::Effect::AccuracyCheckHandler;
using PocketCore::Effect::EffectContext;
using PocketCore::Effect::Side;
using PocketCore::Item::NO_ITEM_ID;
using PocketCore::Pokemon::Pokemon;
using PocketCore::Registry::RegistryProvider;
using PocketCore::Types::NO_TYPE_ID;
using PocketCore::Utility::Random;

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
			.effectRegistry = nullptr,
		};
	}

	Pokemon makePokemon()
	{
		return Pokemon{"UnitMon", 90U, 90U, 100U, 90U, 90U, 90U, 50U, NO_ABILITY_ID, NO_ITEM_ID, {NO_TYPE_ID, NO_TYPE_ID}};
	}
} // namespace

SCENARIO("AccuracyCheckHandler")
{
	AccuracyCheckHandler accuracyCheckHandler{};
	RegistryProvider provider{makeNullProvider()};

	GIVEN("perfect move accuracy and neutral stages")
	{
		Pokemon userPokemon{makePokemon()};
		Pokemon targetPokemon{makePokemon()};

		BattleSlot userSlot{};
		userSlot.mPokemon = &userPokemon;
		userSlot.mStatStages
			= StatStages{.mAttack = 0, .mDefense = 0, .mSpAttack = 0, .mSpDefense = 0, .mSpeed = 0, .mAccuracy = 6, .mEvasion = 0};

		BattleSlot targetSlot{};
		targetSlot.mPokemon = &targetPokemon;
		targetSlot.mStatStages
			= StatStages{.mAttack = 0, .mDefense = 0, .mSpAttack = 0, .mSpDefense = 0, .mSpeed = 0, .mAccuracy = 0, .mEvasion = 6};

		BattleState battleState{};
		battleState.mSideA.push_back(userSlot);
		battleState.mSideB.push_back(targetSlot);

		EffectContext effectContext{};
		effectContext.mUserSide = Side::A;
		effectContext.mTargetSide = Side::B;
		effectContext.mUserIndex = 0;
		effectContext.mTargetIndex = 0;
		effectContext.mMoveAccuracy = 100U;

		WHEN("the random roll is seeded deterministically")
		{
			Random::getTwister().seed(1U);
			accuracyCheckHandler.apply(battleState, effectContext, provider);

			THEN("the attack never misses")
			{
				CHECK_FALSE(effectContext.mDamage.mIsMiss);
				CHECK(effectContext.mDamage.mShouldApplyDamage);
				CHECK(effectContext.mDamage.mShouldContinue);
			}
		}
	}

	GIVEN("zero move accuracy")
	{
		Pokemon userPokemon{makePokemon()};
		Pokemon targetPokemon{makePokemon()};

		BattleState battleState{};
		battleState.mSideA.push_back(BattleSlot{.mPokemon = &userPokemon,
												.mStatStages = StatStages{
													.mAttack = 0,
													.mDefense = 0,
													.mSpAttack = 0,
													.mSpDefense = 0,
													.mSpeed = 0,
													.mAccuracy = 0,
													.mEvasion = 0,
												},});

		battleState.mSideB.push_back(BattleSlot{.mPokemon = &targetPokemon,
												.mStatStages = StatStages{
													.mAttack = 0,
													.mDefense = 0,
													.mSpAttack = 0,
													.mSpDefense = 0,
													.mSpeed = 0,
													.mAccuracy = 0,
													.mEvasion = 0,
												},});

		EffectContext effectContext{};
		effectContext.mUserSide = Side::A;
		effectContext.mTargetSide = Side::B;
		effectContext.mUserIndex = 0;
		effectContext.mTargetIndex = 0;
		effectContext.mMoveAccuracy = 0U;

		WHEN("the check is applied")
		{
			Random::getTwister().seed(7U);
			accuracyCheckHandler.apply(battleState, effectContext, provider);

			THEN("the attack is marked as a miss")
			{
				CHECK(effectContext.mDamage.mIsMiss);
				CHECK_FALSE(effectContext.mDamage.mShouldApplyDamage);
				CHECK_FALSE(effectContext.mDamage.mShouldContinue);
			}
		}
	}

	GIVEN("a low but non-zero move accuracy")
	{
		Pokemon userPokemon{makePokemon()};
		Pokemon targetPokemon{makePokemon()};

		BattleState battleState{};
		battleState.mSideA.push_back(BattleSlot{.mPokemon = &userPokemon,
												.mStatStages = StatStages{
													.mAttack = 0,
													.mDefense = 0,
													.mSpAttack = 0,
													.mSpDefense = 0,
													.mSpeed = 0,
													.mAccuracy = 0,
													.mEvasion = 0,
												},});

		battleState.mSideB.push_back(BattleSlot{.mPokemon = &targetPokemon,
												.mStatStages = StatStages{
													.mAttack = 0,
													.mDefense = 0,
													.mSpAttack = 0,
													.mSpDefense = 0,
													.mSpeed = 0,
													.mAccuracy = 0,
													.mEvasion = 0,
												},});

		EffectContext effectContext{};
		effectContext.mUserSide = Side::A;
		effectContext.mTargetSide = Side::B;
		effectContext.mUserIndex = 0;
		effectContext.mTargetIndex = 0;
		effectContext.mMoveAccuracy = 1U;

		WHEN("a deterministic random roll is sampled")
		{
			Random::getTwister().seed(2U);
			const auto roll = Random::get<signed char>(MIN_ACCURACY_HIT_VALUE, MAX_ACCURACY_HIT_VALUE);
			Random::getTwister().seed(2U);

			accuracyCheckHandler.apply(battleState, effectContext, provider);

			THEN("the miss flag matches the sampled threshold comparison")
			{
				const bool expectedMiss{roll > static_cast<signed char>(effectContext.mMoveAccuracy)};
				CHECK((effectContext.mDamage.mIsMiss == expectedMiss));
				CHECK((effectContext.mDamage.mShouldApplyDamage == !expectedMiss));
				CHECK((effectContext.mDamage.mShouldContinue == !expectedMiss));
			}
		}
	}

	GIVEN("user and target are on opposite sides compared with default setups")
	{
		Pokemon userPokemon{makePokemon()};
		Pokemon targetPokemon{makePokemon()};

		BattleState battleState{};
		battleState.mSideA.push_back(BattleSlot{.mPokemon = &targetPokemon,
												.mStatStages = StatStages{
													.mAttack = 0,
													.mDefense = 0,
													.mSpAttack = 0,
													.mSpDefense = 0,
													.mSpeed = 0,
													.mAccuracy = 6,
													.mEvasion = 0,
												},});

		battleState.mSideB.push_back(BattleSlot{.mPokemon = &userPokemon,
												.mStatStages = StatStages{
													.mAttack = 0,
													.mDefense = 0,
													.mSpAttack = 0,
													.mSpDefense = 0,
													.mSpeed = 0,
													.mAccuracy = 6,
													.mEvasion = 0,
												},});

		EffectContext effectContext{};
		effectContext.mUserSide = Side::B;
		effectContext.mTargetSide = Side::A;
		effectContext.mUserIndex = 0;
		effectContext.mTargetIndex = 0;
		effectContext.mMoveAccuracy = 100U;

		WHEN("the accuracy check resolves battle slots")
		{
			Random::getTwister().seed(23U);
			accuracyCheckHandler.apply(battleState, effectContext, provider);

			THEN("the attack still does not miss")
			{
				CHECK_FALSE(effectContext.mDamage.mIsMiss);
				CHECK(effectContext.mDamage.mShouldApplyDamage);
				CHECK(effectContext.mDamage.mShouldContinue);
			}
		}
	}

	GIVEN("matching negative accuracy and evasion stages")
	{
		Pokemon userPokemon{makePokemon()};
		Pokemon targetPokemon{makePokemon()};

		BattleState battleState{};
		battleState.mSideA.push_back(BattleSlot{.mPokemon = &userPokemon,
												.mStatStages = StatStages{
													.mAttack = 0,
													.mDefense = 0,
													.mSpAttack = 0,
													.mSpDefense = 0,
													.mSpeed = 0,
													.mAccuracy = static_cast<signed char>(-1),
													.mEvasion = 0,
												},});

		battleState.mSideB.push_back(BattleSlot{.mPokemon = &targetPokemon,
												.mStatStages = StatStages{
													.mAttack = 0,
													.mDefense = 0,
													.mSpAttack = 0,
													.mSpDefense = 0,
													.mSpeed = 0,
													.mAccuracy = 0,
													.mEvasion = static_cast<signed char>(-1),
												},});

		EffectContext effectContext{};
		effectContext.mUserSide = Side::A;
		effectContext.mTargetSide = Side::B;
		effectContext.mUserIndex = 0;
		effectContext.mTargetIndex = 0;
		effectContext.mMoveAccuracy = 50U;

		WHEN("the accuracy check is applied")
		{
			Random::getTwister().seed(31U);
			const auto roll = Random::get<signed char>(MIN_ACCURACY_HIT_VALUE, MAX_ACCURACY_HIT_VALUE);
			Random::getTwister().seed(31U);

			accuracyCheckHandler.apply(battleState, effectContext, provider);

			THEN("the opposing stage modifiers cancel without throwing")
			{
				const bool expectedMiss{roll > 50};
				CHECK((effectContext.mDamage.mIsMiss == expectedMiss));
				CHECK((effectContext.mDamage.mShouldApplyDamage == !expectedMiss));
				CHECK((effectContext.mDamage.mShouldContinue == !expectedMiss));
			}
		}
	}

	GIVEN("a valid user accuracy stage and negative target evasion stage")
	{
		Pokemon userPokemon{makePokemon()};
		Pokemon targetPokemon{makePokemon()};

		BattleState battleState{};
		battleState.mSideA.push_back(BattleSlot{.mPokemon = &userPokemon,
												.mStatStages = StatStages{
													.mAttack = 0,
													.mDefense = 0,
													.mSpAttack = 0,
													.mSpDefense = 0,
													.mSpeed = 0,
													.mAccuracy = 0,
													.mEvasion = 0,
												},});

		battleState.mSideB.push_back(BattleSlot{.mPokemon = &targetPokemon,
												.mStatStages = StatStages{
													.mAttack = 0,
													.mDefense = 0,
													.mSpAttack = 0,
													.mSpDefense = 0,
													.mSpeed = 0,
													.mAccuracy = 0,
													.mEvasion = static_cast<signed char>(-1),
												},});

		EffectContext effectContext{};
		effectContext.mUserSide = Side::A;
		effectContext.mTargetSide = Side::B;
		effectContext.mUserIndex = 0;
		effectContext.mTargetIndex = 0;
		effectContext.mMoveAccuracy = 50U;

		WHEN("the handler computes final accuracy")
		{
			Random::getTwister().seed(37U);
			const auto roll = Random::get<signed char>(MIN_ACCURACY_HIT_VALUE, MAX_ACCURACY_HIT_VALUE);
			Random::getTwister().seed(37U);

			accuracyCheckHandler.apply(battleState, effectContext, provider);

			THEN("negative evasion increases accuracy without throwing")
			{
				const bool expectedMiss{roll > 66};
				CHECK((effectContext.mDamage.mIsMiss == expectedMiss));
				CHECK((effectContext.mDamage.mShouldApplyDamage == !expectedMiss));
				CHECK((effectContext.mDamage.mShouldContinue == !expectedMiss));
			}
		}
	}

	GIVEN("accuracy stages outside the supported range")
	{
		Pokemon userPokemon{makePokemon()};
		Pokemon targetPokemon{makePokemon()};

		BattleState battleState{};
		battleState.mSideA.push_back(BattleSlot{
			.mPokemon = &userPokemon,
			.mStatStages = StatStages{.mAccuracy = std::numeric_limits<signed char>::max()},
		});
		battleState.mSideB.push_back(BattleSlot{
			.mPokemon = &targetPokemon,
			.mStatStages = StatStages{.mEvasion = std::numeric_limits<signed char>::min()},
		});

		EffectContext effectContext{};
		effectContext.mUserSide = Side::A;
		effectContext.mTargetSide = Side::B;
		effectContext.mMoveAccuracy = 100U;

		WHEN("the accuracy check is applied")
		{
			Random::getTwister().seed(41U);
			accuracyCheckHandler.apply(battleState, effectContext, provider);

			THEN("the stages are clamped to valid cache bounds")
			{
				CHECK_FALSE(effectContext.mDamage.mIsMiss);
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity,llvm-prefer-static-over-anonymous-namespace))
