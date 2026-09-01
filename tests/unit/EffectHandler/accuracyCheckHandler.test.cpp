/*! @file accuracyCheckHandler.test.cpp
	@brief C++ file for running tests for the AccuracyCheckHandler.
	@date 08/31/2026
	@since 0.8.7
	@version 0.12.13
	@author Matthew Moore
*/

#include "EffectHandler/accuracyCheckHandler.h"

#include <cstddef>

#include "Battle/battleState.h"
#include "Battle/battleState.testHelper.h"
#include "Effect/effectContext.h"
#include "Effect/effectContext.testHelper.h"
#include "Pokemon/pokemon.h"
#include "Pokemon/pokemon.testHelper.h"
#include "Registry/registryProvider.h"
#include "Utility/random.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Battle::BattleSlot;
using PocketCore::Battle::BattleState;
using PocketCore::Effect::AccuracyCheckHandler;
using PocketCore::Effect::EffectContext;
using PocketCore::Effect::Side;
using PocketCore::Pokemon::Pokemon;
using PocketCore::Registry::RegistryProvider;
using PocketCore::Testing::makeBattleState;
using PocketCore::Testing::makeEffectContext;
using PocketCore::Testing::makePokemon;
using PocketCore::Utility::Random;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("AccuracyCheckHandler")
{
	AccuracyCheckHandler accuracyCheckHandler{};
	RegistryProvider provider{};

	GIVEN("perfect move accuracy and neutral stages")
	{
		Pokemon userPokemon{makePokemon({})};
		Pokemon targetPokemon{makePokemon({})};

		BattleSlot userSlot{BattleSlot({.mPokemon = &userPokemon})};
		BattleSlot targetSlot{BattleSlot({.mPokemon = &targetPokemon})};

		BattleState battleState{makeBattleState({.mSideA = {userSlot}, .mSideB = {targetSlot}})};

		EffectContext effectContext{
			makeEffectContext({
				.mUserIndex = 0,
				.mTargetIndex = 0,
				.mMoveAccuracy = 100U,
				.mUserSide = Side::A,
				.mTargetSide = Side::B,
			}),
		};

		WHEN("the handler is applied")
		{
			accuracyCheckHandler.apply(battleState, effectContext, provider);

			THEN("the attack never misses")
			{
				CHECK(effectContext.mDamage.mShouldApplyDamage);
				CHECK(effectContext.mDamage.mShouldContinue);
				CHECK_FALSE(effectContext.mDamage.mIsMiss);
			}
		}
	}

	GIVEN("perfect move accuracy and increased target evasion stages")
	{
		Pokemon userPokemon{makePokemon({})};
		Pokemon targetPokemon{makePokemon({})};

		BattleSlot userSlot{BattleSlot({.mPokemon = &userPokemon})};
		BattleSlot targetSlot{BattleSlot({.mPokemon = &targetPokemon, .mStatStages = {.mEvasion = 6}})};

		BattleState battleState{makeBattleState({.mSideA = {userSlot}, .mSideB = {targetSlot}})};

		WHEN("the handler is applied 100 times")
		{
			std::size_t missCount{0};

			Random::setSeed(5);

			for (std::size_t i{0}; i < 100; ++i)
			{
				EffectContext effectContext{
					makeEffectContext({
						.mUserIndex = 0,
						.mTargetIndex = 0,
						.mMoveAccuracy = 100U,
						.mUserSide = Side::A,
						.mTargetSide = Side::B,
					}),
				};

				accuracyCheckHandler.apply(battleState, effectContext, provider);

				if (effectContext.mDamage.mIsMiss)
				{
					++missCount;
				}
			}

			THEN("the attack will miss on average 66% of the time")
			{
				CHECK((missCount == 66));
			}
		}
	}

	GIVEN("perfect move accuracy and decreased user accuracy stages")
	{
		Pokemon userPokemon{makePokemon({})};
		Pokemon targetPokemon{makePokemon({})};

		BattleSlot userSlot{BattleSlot({.mPokemon = &userPokemon, .mStatStages = {.mAccuracy = -6}})};
		BattleSlot targetSlot{BattleSlot({.mPokemon = &targetPokemon})};

		BattleState battleState{makeBattleState({.mSideA = {userSlot}, .mSideB = {targetSlot}})};

		WHEN("the handler is applied 100 times")
		{
			std::size_t missCount{0};

			Random::setSeed(5);

			for (std::size_t i{0}; i < 100; ++i)
			{
				EffectContext effectContext{
					makeEffectContext({
						.mUserIndex = 0,
						.mTargetIndex = 0,
						.mMoveAccuracy = 100U,
						.mUserSide = Side::A,
						.mTargetSide = Side::B,
					}),
				};

				accuracyCheckHandler.apply(battleState, effectContext, provider);

				if (effectContext.mDamage.mIsMiss)
				{
					++missCount;
				}
			}

			THEN("the attack will miss on average 66% of the time")
			{
				CHECK((missCount == 66));
			}
		}
	}

	GIVEN("perfect move accuracy, equal increase of user accuracy stages and decreased target evasion stages")
	{
		Pokemon userPokemon{makePokemon({})};
		Pokemon targetPokemon{makePokemon({})};

		BattleSlot userSlot{BattleSlot({.mPokemon = &userPokemon, .mStatStages = {.mAccuracy = 6}})};
		BattleSlot targetSlot{BattleSlot({.mPokemon = &targetPokemon, .mStatStages = {.mEvasion = -6}})};

		BattleState battleState{makeBattleState({.mSideA = {userSlot}, .mSideB = {targetSlot}})};

		WHEN("the handler is applied 100 times")
		{
			std::size_t missCount{0};

			for (std::size_t i{0}; i < 100; ++i)
			{
				EffectContext effectContext{
					makeEffectContext({
						.mUserIndex = 0,
						.mTargetIndex = 0,
						.mMoveAccuracy = 100U,
						.mUserSide = Side::A,
						.mTargetSide = Side::B,
					}),
				};

				accuracyCheckHandler.apply(battleState, effectContext, provider);

				if (effectContext.mDamage.mIsMiss)
				{
					++missCount;
				}
			}

			THEN("the attack will never miss")
			{
				CHECK((missCount == 0));
			}
		}
	}

	GIVEN("50% move accuracy, max user accuracy stages and -3 target evasion stages")
	{
		Pokemon userPokemon{makePokemon({})};
		Pokemon targetPokemon{makePokemon({})};

		BattleSlot userSlot{BattleSlot({.mPokemon = &userPokemon, .mStatStages = {.mAccuracy = 6}})};
		BattleSlot targetSlot{BattleSlot({.mPokemon = &targetPokemon, .mStatStages = {.mEvasion = -3}})};

		BattleState battleState{makeBattleState({.mSideA = {userSlot}, .mSideB = {targetSlot}})};

		WHEN("the handler is applied 100 times")
		{
			std::size_t missCount{0};

			for (std::size_t i{0}; i < 100; ++i)
			{
				EffectContext effectContext{
					makeEffectContext({
						.mUserIndex = 0,
						.mTargetIndex = 0,
						.mMoveAccuracy = 50U,
						.mUserSide = Side::A,
						.mTargetSide = Side::B,
					}),
				};

				accuracyCheckHandler.apply(battleState, effectContext, provider);

				if (effectContext.mDamage.mIsMiss)
				{
					++missCount;
				}
			}

			THEN("the attack will never miss")
			{
				CHECK((missCount == 0));
			}
		}
	}

	GIVEN("34% move accuracy and max user accuracy stages")
	{
		Pokemon userPokemon{makePokemon({})};
		Pokemon targetPokemon{makePokemon({})};

		BattleSlot userSlot{BattleSlot({.mPokemon = &userPokemon, .mStatStages = {.mAccuracy = 6}})};
		BattleSlot targetSlot{BattleSlot({.mPokemon = &targetPokemon})};

		BattleState battleState{makeBattleState({.mSideA = {userSlot}, .mSideB = {targetSlot}})};

		WHEN("the handler is applied 100 times")
		{
			std::size_t missCount{0};

			for (std::size_t i{0}; i < 100; ++i)
			{
				EffectContext effectContext{
					makeEffectContext({
						.mUserIndex = 0,
						.mTargetIndex = 0,
						.mMoveAccuracy = 34U,
						.mUserSide = Side::A,
						.mTargetSide = Side::B,
					}),
				};

				accuracyCheckHandler.apply(battleState, effectContext, provider);

				if (effectContext.mDamage.mIsMiss)
				{
					++missCount;
				}
			}

			THEN("the attack never misses")
			{
				CHECK((missCount == 0));
			}
		}
	}

	GIVEN("10% move accuracy and max user accuracy stages")
	{
		Pokemon userPokemon{makePokemon({})};
		Pokemon targetPokemon{makePokemon({})};

		BattleSlot userSlot{BattleSlot({.mPokemon = &userPokemon, .mStatStages = {.mAccuracy = 6}})};
		BattleSlot targetSlot{BattleSlot({.mPokemon = &targetPokemon})};

		BattleState battleState{makeBattleState({.mSideA = {userSlot}, .mSideB = {targetSlot}})};

		WHEN("the handler is applied 100 times")
		{
			std::size_t missCount{0};

			Random::setSeed(18);

			for (std::size_t i{0}; i < 100; ++i)
			{
				EffectContext effectContext{
					makeEffectContext({
						.mUserIndex = 0,
						.mTargetIndex = 0,
						.mMoveAccuracy = 10U,
						.mUserSide = Side::A,
						.mTargetSide = Side::B,
					}),
				};

				accuracyCheckHandler.apply(battleState, effectContext, provider);

				if (effectContext.mDamage.mIsMiss)
				{
					++missCount;
				}
			}

			THEN("the attack hits 30% of the time")
			{
				CHECK((missCount == 70));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
