/*! @file effectHandlerInterface.test.cpp
	@brief C++ file for running tests for the EffectHandlerInterface.
	@date 08/23/2026
	@since 0.8.7
	@version 0.12.1
	@author Matthew Moore
*/

#include "EffectHandler/effectHandlerInterface.h"

#include <memory>

#include "Battle/battleState.h"
#include "Effect/effectContext.h"
#include "Registry/registryProvider.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Battle::BattleSlot;
using PocketCore::Battle::BattleState;
using PocketCore::Effect::EffectContext;
using PocketCore::Effect::IEffectHandler;
using PocketCore::Effect::Side;
using PocketCore::Registry::RegistryProvider;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

namespace
{
	class NoopEffectHandler final : public IEffectHandler
	{
		public:
			void apply(BattleState & /*state*/, EffectContext & /*context*/, const RegistryProvider & /*provider*/) const override {}
	};
} // namespace

SCENARIO("IEffectHandler")
{
	GIVEN("a dynamically allocated concrete handler")
	{
		std::unique_ptr<IEffectHandler> handler{std::make_unique<NoopEffectHandler>()};

		THEN("deletion through interface pointer invokes virtual deleting destructor")
		{
			handler.reset();
			CHECK((handler == nullptr));
		}
	}

	GIVEN("battle state with one user and one target slot")
	{
		BattleState battleState{};
		battleState.mSideA.push_back(BattleSlot{});
		battleState.mSideB.push_back(BattleSlot{});

		EffectContext effectContext{};
		effectContext.mUserSide = Side::A;
		effectContext.mTargetSide = Side::B;
		effectContext.mUserIndex = 0;
		effectContext.mTargetIndex = 0;

		THEN("static slot accessors return the expected slots")
		{
			const BattleSlot &userSlot{IEffectHandler::getUserBattleSlot(battleState, effectContext)};
			const BattleSlot &targetSlot{IEffectHandler::getTargetBattleSlot(battleState, effectContext)};

			CHECK((&userSlot == &battleState.mSideA.at(0)));
			CHECK((&targetSlot == &battleState.mSideB.at(0)));
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
