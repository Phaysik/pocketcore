/*! @file effectHandlerInterface.test.cpp
	@brief C++ file for running tests for the EffectHandlerInterface.
	@date 09/01/2026
	@since 0.8.7
	@version 0.12.14
	@author Matthew Moore
*/

#include "EffectHandler/effectHandlerInterface.h"

#include <memory>

#include "Battle/battleState.h"
#include "Battle/battleState.testHelper.h"
#include "Effect/effectContext.h"
#include "Effect/effectContext.testHelper.h"
#include "Registry/registryProvider.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Battle::BattleSlot;
using PocketCore::Battle::BattleState;
using PocketCore::Effect::EffectContext;
using PocketCore::Effect::IEffectHandler;
using PocketCore::Effect::Side;
using PocketCore::Registry::RegistryProvider;
using PocketCore::Testing::makeBattleState;
using PocketCore::Testing::makeEffectContext;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("IEffectHandler")
{
	GIVEN("a dynamically allocated concrete handler")
	{
		class NoopEffectHandler final : public IEffectHandler
		{
			public:
				void apply(BattleState & /*state*/, EffectContext & /*context*/, const RegistryProvider & /*provider*/) const override {}
		};

		std::unique_ptr<IEffectHandler> handler{std::make_unique<NoopEffectHandler>()};

		THEN("deletion through interface pointer invokes virtual deleting destructor")
		{
			handler.reset();
			CHECK((handler == nullptr));
		}
	}

	BattleState battleState{makeBattleState({.mSideA = {{}}, .mSideB = {{}}})};
	EffectContext context{makeEffectContext({.mUserIndex = 0, .mTargetIndex = 0, .mUserSide = Side::A, .mTargetSide = Side::B})};

	GIVEN("get user battle slot")
	{
		THEN("with constness")
		{
			const BattleSlot &userSlot{IEffectHandler::getConstUserBattleSlot(battleState, context)};

			CHECK((&userSlot == &battleState.mSideA.at(0)));
		}

		THEN("without constness")
		{
			BattleSlot &userSlot{IEffectHandler::getUserBattleSlot(battleState, context)};

			CHECK((&userSlot == &battleState.mSideA.at(0)));
		}
	}

	GIVEN("get target battle slot")
	{

		THEN("with constness")
		{
			const BattleSlot &targetSlot{IEffectHandler::getConstTargetBattleSlot(battleState, context)};

			CHECK((&targetSlot == &battleState.mSideB.at(0)));
		}

		THEN("without constness")
		{
			BattleSlot &targetSlot{IEffectHandler::getTargetBattleSlot(battleState, context)};

			CHECK((&targetSlot == &battleState.mSideB.at(0)));
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
