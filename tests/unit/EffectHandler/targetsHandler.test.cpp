/*! @file targetsHandler.test.cpp
	@brief C++ file for running tests for the TargetsHandler.
	@date 08/13/2026
	@since 0.8.7
	@version 0.10.1
	@author Matthew Moore
*/

#include "EffectHandler/targetsHandler.h"

#include <algorithm>
#include <cmath>

#include "Battle/battleState.h"
#include "Battle/battleTargetsAndTriggers.h"
#include "Configuration/constants.h"
#include "Effect/effectContext.h"
#include "Move/builtInMoveID.h"
#include "Move/moveID.h"
#include "Move/moveMeta.h"
#include "Multiplier/builtInMultiplierID.h"
#include "Registry/moveRegistry.h"
#include "Registry/registryProvider.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Battle::BattleRangeID;
using PocketCore::Battle::BattleState;
using PocketCore::Battle::BattleTargetID;
using PocketCore::Configuration::TARGETS_HIT_MULTIPLIER_VALUE;
using PocketCore::Effect::EffectContext;
using PocketCore::Effect::TargetsHandler;
using PocketCore::Move::BuiltinMoveID;
using PocketCore::Move::MoveID;
using PocketCore::Move::MoveMeta;
using PocketCore::Move::toMoveID;
using PocketCore::Multiplier::BuiltinMultiplierID;
using PocketCore::Multiplier::toMultiplierID;
using PocketCore::Registry::Move::MoveRegistry;
using PocketCore::Registry::RegistryProvider;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,llvm-prefer-static-over-anonymous-namespace)

namespace
{
	MoveID registerMoveWithTarget(MoveRegistry &moveRegistry, const BattleTargetID moveTargetIdentifier)
	{
		const MoveID customMoveIdentifier{moveRegistry.getNextMoveID()};
		const auto customMoveIndex{moveRegistry.getAmountRegistered()};

		moveRegistry.setEntry(customMoveIndex, MoveMeta{
												   .mTriggers = {},
												   .mName = "test-move",
												   .mMoveID = customMoveIdentifier,
												   .mTargetID = moveTargetIdentifier,
												   .mRangeID = BattleRangeID::Adjacent,
											   });

		moveRegistry.incrementAmountRegistered();
		moveRegistry.incrementNextMoveID();

		return customMoveIdentifier;
	}

	bool hasTargetsMultiplier(const EffectContext &effectContext)
	{
		return std::ranges::any_of(effectContext.getActiveMultipliers(), [](const auto &multiplierPair) {
			return multiplierPair.first == toMultiplierID(BuiltinMultiplierID::Targets)
				&& std::fabs(multiplierPair.second - TARGETS_HIT_MULTIPLIER_VALUE) < 0.0001;
		});
	}
} // namespace

SCENARIO("TargetsHandler")
{
	TargetsHandler targetsHandler{};
	BattleState battleState{};
	MoveRegistry moveRegistry{};

	RegistryProvider provider{
		.abilityRegistry = nullptr,
		.moveRegistry = &moveRegistry,
		.itemRegistry = nullptr,
		.typeRegistry = nullptr,
		.statusRegistry = nullptr,
		.weatherRegistry = nullptr,
		.terrainRegistry = nullptr,
		.multiplierRegistry = nullptr,
		.natureRegistry = nullptr,
		.pokemonRegistry = nullptr,
	};

	GIVEN("a move identifier that is not registered")
	{
		EffectContext effectContext{};
		effectContext.mMoveID = MoveID{9'999U};

		WHEN("applying target handling")
		{
			targetsHandler.apply(battleState, effectContext, provider);

			THEN("no target multiplier is added")
			{
				CHECK_FALSE(hasTargetsMultiplier(effectContext));
			}
		}
	}

	GIVEN("a move that targets a single opponent")
	{
		EffectContext effectContext{};
		effectContext.mMoveID = toMoveID(BuiltinMoveID::Pound);

		WHEN("applying target handling")
		{
			targetsHandler.apply(battleState, effectContext, provider);

			THEN("no targets spread multiplier is added")
			{
				CHECK_FALSE(hasTargetsMultiplier(effectContext));
			}
		}
	}

	GIVEN("a move that targets only self")
	{
		EffectContext effectContext{};
		effectContext.mMoveID = registerMoveWithTarget(moveRegistry, BattleTargetID::Self);

		WHEN("applying target handling")
		{
			targetsHandler.apply(battleState, effectContext, provider);

			THEN("no targets spread multiplier is added")
			{
				CHECK_FALSE(hasTargetsMultiplier(effectContext));
			}
		}
	}

	GIVEN("a move that can hit multiple targets")
	{
		EffectContext effectContext{};
		effectContext.mMoveID = registerMoveWithTarget(moveRegistry, BattleTargetID::AllOpponents);

		WHEN("applying target handling")
		{
			targetsHandler.apply(battleState, effectContext, provider);

			THEN("the targets spread multiplier is added")
			{
				const auto activeMultipliers{effectContext.getActiveMultipliers()};
				REQUIRE((activeMultipliers.size() == 1U));
				CHECK((activeMultipliers.front().first == toMultiplierID(BuiltinMultiplierID::Targets)));
				CHECK((std::fabs(activeMultipliers.front().second - TARGETS_HIT_MULTIPLIER_VALUE) < 0.0001));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,llvm-prefer-static-over-anonymous-namespace)
