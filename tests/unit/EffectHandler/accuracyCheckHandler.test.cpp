/*! @file accuracyCheckHandler.test.cpp
	@brief C++ file for running tests for the AccuracyCheckHandler.
	@date 10/27/2026
	@since 0.8.7
	@version 0.12.53
	@author Matthew Moore
*/

#include "EffectHandler/accuracyCheckHandler.h"

#include <cstddef>

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
#include "Utility/Random/callableRandomSource.h"
#include "Utility/Random/random.testHelper.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Battle::BattleRandomSources;
using PocketCore::Battle::BattleSlot;
using PocketCore::Battle::BattleState;
using PocketCore::Core::sb;
using PocketCore::Effect::AccuracyCheckHandler;
using PocketCore::Effect::applyAccuracyCheck;
using PocketCore::Effect::EffectContext;
using PocketCore::Effect::Side;
using PocketCore::Pokemon::Pokemon;
using PocketCore::Registry::RegistryProvider;
using PocketCore::Testing::getDefaultInitializedRegistryProvider;
using PocketCore::Testing::makeBattleState;
using PocketCore::Testing::makeEffectContext;
using PocketCore::Testing::makePokemon;
using PocketCore::Testing::SequenceRandom;
using PocketCore::Utility::Random::CallableRandomSource;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("AccuracyCheckHandler")
{
	Pokemon userPokemon{makePokemon({})};
	Pokemon targetPokemon{makePokemon({})};

	AccuracyCheckHandler accuracyCheckHandler{};
	RegistryProvider provider{getDefaultInitializedRegistryProvider()};
	BattleRandomSources handlerRandomSources{};
	BattleRandomSources freeFunctionRandomSources{};

	BattleSlot userSlot{BattleSlot({.mPokemon = &userPokemon})};
	BattleSlot targetSlot{BattleSlot({.mPokemon = &targetPokemon})};

	BattleState handlerState{makeBattleState({.mSideA = {userSlot}, .mSideB = {targetSlot}})};
	BattleState freeFunctionState{makeBattleState({.mSideA = {userSlot}, .mSideB = {targetSlot}})};

	EffectContext handlerContext{
		makeEffectContext({
			.mUserIndex = 0,
			.mTargetIndex = 0,
			.mMoveAccuracy = 100U,
			.mUserSide = Side::A,
			.mTargetSide = Side::B,
		}),
	};
	EffectContext freeFunctionContext{
		makeEffectContext({
			.mUserIndex = 0,
			.mTargetIndex = 0,
			.mMoveAccuracy = 100U,
			.mUserSide = Side::A,
			.mTargetSide = Side::B,
		}),
	};

	GIVEN("perfect move accuracy and neutral stages")
	{
		WHEN("the handler is applied")
		{
			accuracyCheckHandler.apply(handlerState, handlerContext, provider, handlerRandomSources);
			applyAccuracyCheck(freeFunctionState, freeFunctionContext, provider, freeFunctionRandomSources);

			THEN("the attack never misses")
			{
				CHECK(handlerContext.mDamage.mShouldApplyDamage);
				CHECK(handlerContext.mDamage.mShouldContinue);
				CHECK_FALSE(handlerContext.mDamage.mIsMiss);

				CHECK(freeFunctionContext.mDamage.mShouldApplyDamage);
				CHECK(freeFunctionContext.mDamage.mShouldContinue);
				CHECK_FALSE(freeFunctionContext.mDamage.mIsMiss);
			}
		}
	}

	GIVEN("perfect move accuracy and increased target evasion stages")
	{
		targetSlot.mStatStages = {.mEvasion = 6};
		handlerState.mSideB = {targetSlot};
		freeFunctionState.mSideB = {targetSlot};

		WHEN("the handler is applied 3 times")
		{
			std::size_t handlerMissCount{0};
			std::size_t freeFunctionMissCount{0};
			CallableRandomSource<sb, SequenceRandom<sb>> handlerSignedSource{SequenceRandom<sb>{100, 100, 0}};
			CallableRandomSource<sb, SequenceRandom<sb>> freeFunctionSignedSource{SequenceRandom<sb>{100, 100, 0}};
			handlerRandomSources.mSigned = handlerSignedSource;
			freeFunctionRandomSources.mSigned = freeFunctionSignedSource;

			for (std::size_t i{0}; i < 3; ++i)
			{
				handlerContext.mDamage.mIsMiss = false;
				freeFunctionContext.mDamage.mIsMiss = false;
				accuracyCheckHandler.apply(handlerState, handlerContext, provider, handlerRandomSources);
				applyAccuracyCheck(freeFunctionState, freeFunctionContext, provider, freeFunctionRandomSources);

				if (handlerContext.mDamage.mIsMiss)
				{
					++handlerMissCount;
				}

				if (freeFunctionContext.mDamage.mIsMiss)
				{
					++freeFunctionMissCount;
				}
			}

			THEN("the attack will miss on average 66% of the time")
			{
				CHECK((handlerMissCount == 2));
				CHECK((freeFunctionMissCount == 2));
			}
		}
	}

	GIVEN("perfect move accuracy and decreased user accuracy stages")
	{
		userSlot.mStatStages = {.mAccuracy = -6};
		handlerState.mSideA = {userSlot};
		freeFunctionState.mSideA = {userSlot};

		WHEN("the handler is applied 3 times")
		{
			std::size_t handlerMissCount{0};
			std::size_t freeFunctionMissCount{0};

			CallableRandomSource<sb, SequenceRandom<sb>> handlerSignedSource{SequenceRandom<sb>{100, 100, 0}};
			CallableRandomSource<sb, SequenceRandom<sb>> freeFunctionSignedSource{SequenceRandom<sb>{100, 100, 0}};
			handlerRandomSources.mSigned = handlerSignedSource;
			freeFunctionRandomSources.mSigned = freeFunctionSignedSource;

			for (std::size_t i{0}; i < 3; ++i)
			{
				handlerContext.mDamage.mIsMiss = false;
				freeFunctionContext.mDamage.mIsMiss = false;

				accuracyCheckHandler.apply(handlerState, handlerContext, provider, handlerRandomSources);
				applyAccuracyCheck(freeFunctionState, freeFunctionContext, provider, freeFunctionRandomSources);

				if (handlerContext.mDamage.mIsMiss)
				{
					++handlerMissCount;
				}

				if (freeFunctionContext.mDamage.mIsMiss)
				{
					++freeFunctionMissCount;
				}
			}

			THEN("the attack will miss on average 66% of the time")
			{
				CHECK((handlerMissCount == 2));
				CHECK((freeFunctionMissCount == 2));
			}
		}
	}

	GIVEN("perfect move accuracy, equal increase of user accuracy stages and decreased target evasion stages")
	{
		userSlot.mStatStages = {.mAccuracy = 6};
		targetSlot.mStatStages = {.mEvasion = -6};
		handlerState.mSideA = {userSlot};
		freeFunctionState.mSideA = {userSlot};
		handlerState.mSideB = {targetSlot};
		freeFunctionState.mSideB = {targetSlot};

		WHEN("the handler is applied 100 times")
		{
			std::size_t handlerMissCount{0};
			std::size_t freeFunctionMissCount{0};

			for (std::size_t i{0}; i < 100; ++i)
			{
				handlerContext.mDamage.mIsMiss = false;
				freeFunctionContext.mDamage.mIsMiss = false;

				accuracyCheckHandler.apply(handlerState, handlerContext, provider, handlerRandomSources);
				applyAccuracyCheck(freeFunctionState, freeFunctionContext, provider, freeFunctionRandomSources);

				if (handlerContext.mDamage.mIsMiss)
				{
					++handlerMissCount;
				}

				if (freeFunctionContext.mDamage.mIsMiss)
				{
					++freeFunctionMissCount;
				}
			}

			THEN("the attack will never miss")
			{
				CHECK((handlerMissCount == 0));
				CHECK((freeFunctionMissCount == 0));
			}
		}
	}

	GIVEN("50% move accuracy, max user accuracy stages and -3 target evasion stages")
	{
		userSlot.mStatStages = {.mAccuracy = 6};
		targetSlot.mStatStages = {.mEvasion = -3};
		handlerState.mSideA = {userSlot};
		freeFunctionState.mSideA = {userSlot};
		handlerState.mSideB = {targetSlot};
		freeFunctionState.mSideB = {targetSlot};

		WHEN("the handler is applied 100 times")
		{
			std::size_t handlerMissCount{0};
			std::size_t freeFunctionMissCount{0};

			for (std::size_t i{0}; i < 100; ++i)
			{
				handlerContext.mDamage.mIsMiss = false;
				freeFunctionContext.mDamage.mIsMiss = false;

				handlerContext.mMoveAccuracy = 50U;
				freeFunctionContext.mMoveAccuracy = 50U;

				accuracyCheckHandler.apply(handlerState, handlerContext, provider, handlerRandomSources);
				applyAccuracyCheck(freeFunctionState, freeFunctionContext, provider, freeFunctionRandomSources);

				if (handlerContext.mDamage.mIsMiss)
				{
					++handlerMissCount;
				}

				if (freeFunctionContext.mDamage.mIsMiss)
				{
					++freeFunctionMissCount;
				}
			}

			THEN("the attack will never miss")
			{
				CHECK((handlerMissCount == 0));
				CHECK((freeFunctionMissCount == 0));
			}
		}
	}

	GIVEN("34% move accuracy and max user accuracy stages")
	{
		userSlot.mStatStages = {.mAccuracy = 6};
		handlerState.mSideA = {userSlot};
		freeFunctionState.mSideA = {userSlot};

		WHEN("the handler is applied 100 times")
		{
			std::size_t handlerMissCount{0};
			std::size_t freeFunctionMissCount{0};

			for (std::size_t i{0}; i < 100; ++i)
			{
				handlerContext.mDamage.mIsMiss = false;
				freeFunctionContext.mDamage.mIsMiss = false;

				handlerContext.mMoveAccuracy = 34U;
				freeFunctionContext.mMoveAccuracy = 34U;

				accuracyCheckHandler.apply(handlerState, handlerContext, provider, handlerRandomSources);
				applyAccuracyCheck(freeFunctionState, freeFunctionContext, provider, freeFunctionRandomSources);

				if (handlerContext.mDamage.mIsMiss)
				{
					++handlerMissCount;
				}

				if (freeFunctionContext.mDamage.mIsMiss)
				{
					++freeFunctionMissCount;
				}
			}

			THEN("the attack never misses")
			{
				CHECK((handlerMissCount == 0));
				CHECK((freeFunctionMissCount == 0));
			}
		}
	}

	GIVEN("10% move accuracy and max user accuracy stages")
	{
		userSlot.mStatStages = {.mAccuracy = 6};
		handlerState.mSideA = {userSlot};
		freeFunctionState.mSideA = {userSlot};

		WHEN("the handler is applied 3 times")
		{
			std::size_t handlerMissCount{0};
			std::size_t freeFunctionMissCount{0};

			CallableRandomSource<sb, SequenceRandom<sb>> handlerSignedSource{SequenceRandom<sb>{100, 100, 0}};
			CallableRandomSource<sb, SequenceRandom<sb>> freeFunctionSignedSource{SequenceRandom<sb>{100, 100, 0}};
			handlerRandomSources.mSigned = handlerSignedSource;
			freeFunctionRandomSources.mSigned = freeFunctionSignedSource;

			for (std::size_t i{0}; i < 3; ++i)
			{
				handlerContext.mDamage.mIsMiss = false;
				freeFunctionContext.mDamage.mIsMiss = false;

				handlerContext.mMoveAccuracy = 10U;
				freeFunctionContext.mMoveAccuracy = 10U;

				accuracyCheckHandler.apply(handlerState, handlerContext, provider, handlerRandomSources);
				applyAccuracyCheck(freeFunctionState, freeFunctionContext, provider, freeFunctionRandomSources);

				if (handlerContext.mDamage.mIsMiss)
				{
					++handlerMissCount;
				}

				if (freeFunctionContext.mDamage.mIsMiss)
				{
					++freeFunctionMissCount;
				}
			}

			THEN("the attack hits 66% of the time")
			{
				CHECK((handlerMissCount == 2));
				CHECK((freeFunctionMissCount == 2));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
