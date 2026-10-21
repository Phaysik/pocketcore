/*! @file battleValidation.test.cpp
	@brief C++ file for running tests for the Battle validation functions.
	@date 10/21/2026
	@since 0.12.51
	@version 0.12.51
	@author Matthew Moore
*/

#include "Validation/Battle/battleValidation.h"

#include <expected>

#include "Battle/battleAction.h"
#include "Battle/battleState.h"
#include "Battle/battleState.testHelper.h"
#include "Battle/battleValidation.h"
#include "Effect/effectContext.h"
#include "Move/builtInMoveID.h"
#include "Move/moveID.h"
#include "Pokemon/pokemon.h"
#include "Pokemon/pokemon.testHelper.h"
#include "Registry/moveRegistry.h"
#include "Registry/registryProvider.h"
#include "Registry/registryProvider.testHelper.h"
#include "Validation/Battle/battleError.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Battle::BattlePhase;
using PocketCore::Battle::BattleState;
using PocketCore::Battle::BattleTarget;
using PocketCore::Battle::MoveAction;
using PocketCore::Battle::SwitchAction;
using PocketCore::Effect::Side;
using PocketCore::Move::BuiltinMoveID;
using PocketCore::Move::MoveID;
using PocketCore::Move::toMoveID;
using PocketCore::Pokemon::Pokemon;
using PocketCore::Registry::Move::MoveRegistry;
using PocketCore::Registry::RegistryProvider;
using PocketCore::Testing::getDefaultInitializedRegistryProvider;
using PocketCore::Testing::makeBattleState;
using PocketCore::Testing::makePokemon;
using PocketCore::Validation::Battle::BattleEngineError;
using PocketCore::Validation::Battle::getValidationResult;
using PocketCore::Validation::Battle::validateMoveAction;
using PocketCore::Validation::Battle::validateSwitchAction;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("validateSwitchAction")
{
	Pokemon member1{makePokemon({.mHealth = 50})};
	Pokemon member2{makePokemon({.mHealth = 50})};
	Pokemon member3{makePokemon({.mHealth = 50})};
	Pokemon member4{makePokemon({.mHealth = 50})};
	Pokemon member5{makePokemon({.mHealth = 50})};
	Pokemon member6{makePokemon({.mHealth = 50})};

	BattleState state{
		makeBattleState({
			.mSideA = {{{.mPokemon = &member1}, {.mPokemon = &member2}}},
			.mSideB = {{{.mPokemon = &member6}}},
			.mPartyA = {&member1, &member2, &member3, &member4, &member5},
			.mPartyB = {&member6},
			.mBattleStarted = true,
		}),
	};
	SwitchAction action{.mSide = Side::A, .mActiveSlotIndex = 0, .mPartyIndex = 4};

	WHEN("calling when the battle hasn't started yet")
	{
		state.mBattleStarted = false;
		std::expected<void, BattleEngineError> result{validateSwitchAction(state, action)};

		THEN("it returns BattleNotStarted")
		{
			REQUIRE_FALSE(result.has_value());
			CHECK((result.error() == BattleEngineError::BattleNotStarted));
		}
	}

	WHEN("calling when the battle isn't in progress")
	{
		member1.setHealth(0);
		member2.setHealth(0);
		member3.setHealth(0);
		member4.setHealth(0);
		member5.setHealth(0);
		member6.setHealth(0);
		std::expected<void, BattleEngineError> result{validateSwitchAction(state, action)};

		THEN("it returns BattleFinished")
		{
			REQUIRE_FALSE(result.has_value());
			CHECK((result.error() == BattleEngineError::BattleFinished));
		}
	}

	WHEN("calling when the active slot index is higher than the amount of Pokemon in the active field")
	{
		action.mActiveSlotIndex = 5;
		std::expected<void, BattleEngineError> result{validateSwitchAction(state, action)};

		THEN("it returns InvalidActiveSlot")
		{
			REQUIRE_FALSE(result.has_value());
			CHECK((result.error() == BattleEngineError::InvalidActiveSlot));
		}
	}

	WHEN("calling when the party index is higher than the amount of Pokemon in the party")
	{
		action.mPartyIndex = 7;
		std::expected<void, BattleEngineError> result{validateSwitchAction(state, action)};

		THEN("it returns InvalidPartyIndex")
		{
			REQUIRE_FALSE(result.has_value());
			CHECK((result.error() == BattleEngineError::InvalidPartyIndex));
		}
	}

	WHEN("calling when the selected party Pokemon is fainted")
	{
		member5.setHealth(0);
		action.mPartyIndex = 4;
		std::expected<void, BattleEngineError> result{validateSwitchAction(state, action)};

		THEN("it returns PokemonFainted")
		{
			REQUIRE_FALSE(result.has_value());
			CHECK((result.error() == BattleEngineError::PokemonFainted));
		}
	}

	WHEN("calling when the selected party Pokemon is trying to be switched into itself")
	{
		action.mPartyIndex = 0;
		std::expected<void, BattleEngineError> result{validateSwitchAction(state, action)};

		THEN("it returns PokemonAlreadyActive")
		{
			REQUIRE_FALSE(result.has_value());
			CHECK((result.error() == BattleEngineError::PokemonAlreadyActive));
		}
	}

	WHEN("calling when the selected party Pokemon is trying to be switched into another active slot")
	{
		action.mPartyIndex = 0;
		action.mActiveSlotIndex = 1;
		std::expected<void, BattleEngineError> result{validateSwitchAction(state, action)};

		THEN("it returns PokemonAlreadyActive")
		{
			REQUIRE_FALSE(result.has_value());
			CHECK((result.error() == BattleEngineError::PokemonAlreadyActive));
		}
	}

	WHEN("calling when the selected party Pokemon can replace the active slot")
	{
		action.mPartyIndex = 3;
		std::expected<void, BattleEngineError> result{validateSwitchAction(state, action)};

		THEN("it succeeds")
		{
			REQUIRE(result.has_value());
		}
	}
}

SCENARIO("validateMoveAction")
{
	Pokemon member1{makePokemon({.mMoveIDs = {toMoveID(BuiltinMoveID::Pound), MoveID{200}}, .mCurrentPP = {20, 20}, .mHealth = 50})};
	Pokemon member2{makePokemon({.mHealth = 50})};
	Pokemon member3{makePokemon({.mMoveIDs = {toMoveID(BuiltinMoveID::Pound), MoveID{200}}, .mCurrentPP = {20, 20}, .mHealth = 50})};
	Pokemon member4{makePokemon({.mHealth = 50})};
	Pokemon member5{makePokemon({.mHealth = 50})};
	Pokemon member6{makePokemon({.mHealth = 50})};

	BattleState state{
		makeBattleState({
			.mSideA
			= {{{.mPokemon = &member1, .mPosition = 1}, {.mPokemon = &member2, .mPosition = 2}, {.mPokemon = &member3, .mPosition = 3}}},
			.mSideB = {{{.mPokemon = &member6, .mPosition = 1}}},
			.mPartyA = {&member1, &member2, &member3, &member4, &member5},
			.mPartyB = {&member6},
			.mBattleStarted = true,
		}),
	};
	BattleTarget target{.mSide = Side::B, .mSlotIndex = 0};
	MoveAction action{.mSelectedTarget = target, .mSide = Side::A, .mUserSlotIndex = 0, .mMoveSlotIndex = 0};
	BattlePhase phase{BattlePhase::AwaitingActions};
	RegistryProvider provider{getDefaultInitializedRegistryProvider()};
	const MoveRegistry *moveRegistry{provider.mMoveRegistry};

	WHEN("calling when the battle hasn't started yet")
	{
		state.mBattleStarted = false;
		std::expected<void, BattleEngineError> result{validateMoveAction(state, action, phase, moveRegistry)};

		THEN("it returns BattleNotStarted")
		{
			REQUIRE_FALSE(result.has_value());
			CHECK((result.error() == BattleEngineError::BattleNotStarted));
		}
	}

	WHEN("calling when the battle is awaiting replacements")
	{
		phase = BattlePhase::AwaitingReplacements;
		std::expected<void, BattleEngineError> result{validateMoveAction(state, action, phase, moveRegistry)};

		THEN("it returns ReplacementRequired")
		{
			REQUIRE_FALSE(result.has_value());
			CHECK((result.error() == BattleEngineError::ReplacementRequired));
		}
	}

	WHEN("calling when the battle isn't in progress")
	{
		member1.setHealth(0);
		member2.setHealth(0);
		member3.setHealth(0);
		member4.setHealth(0);
		member5.setHealth(0);
		member6.setHealth(0);
		std::expected<void, BattleEngineError> result{validateMoveAction(state, action, phase, moveRegistry)};

		THEN("it returns BattleFinished")
		{
			REQUIRE_FALSE(result.has_value());
			CHECK((result.error() == BattleEngineError::BattleFinished));
		}
	}

	WHEN("calling when the active slot index is higher than the amount of Pokemon in the active field")
	{
		action.mUserSlotIndex = 5;
		std::expected<void, BattleEngineError> result{validateMoveAction(state, action, phase, moveRegistry)};

		THEN("it returns InvalidActiveSlot")
		{
			REQUIRE_FALSE(result.has_value());
			CHECK((result.error() == BattleEngineError::InvalidActiveSlot));
		}
	}

	WHEN("calling when the selected party Pokemon is fainted")
	{
		member1.setHealth(0);
		std::expected<void, BattleEngineError> result{validateMoveAction(state, action, phase, moveRegistry)};

		THEN("it returns PokemonFainted")
		{
			REQUIRE_FALSE(result.has_value());
			CHECK((result.error() == BattleEngineError::PokemonFainted));
		}
	}

	WHEN("calling when the selected move is outside the bounds of the move array")
	{
		action.mMoveSlotIndex = 6;
		std::expected<void, BattleEngineError> result{validateMoveAction(state, action, phase, moveRegistry)};

		THEN("it returns InvalidMoveSlot")
		{
			REQUIRE_FALSE(result.has_value());
			CHECK((result.error() == BattleEngineError::InvalidMoveSlot));
		}
	}

	WHEN("calling when the selected move has invalid metadata")
	{
		action.mMoveSlotIndex = 1;
		std::expected<void, BattleEngineError> result{validateMoveAction(state, action, phase, moveRegistry)};

		THEN("it returns MoveNotFound")
		{
			REQUIRE_FALSE(result.has_value());
			CHECK((result.error() == BattleEngineError::MoveNotFound));
		}
	}

	WHEN("calling when the selected move has no remaining PP")
	{
		member1.setCurrentPP(0, 0);
		std::expected<void, BattleEngineError> result{validateMoveAction(state, action, phase, moveRegistry)};

		THEN("it returns NoPP")
		{
			REQUIRE_FALSE(result.has_value());
			CHECK((result.error() == BattleEngineError::NoPP));
		}
	}

	WHEN("calling when no targets can be resolved from the current move")
	{
		action.mUserSlotIndex = 2;
		std::expected<void, BattleEngineError> result{validateMoveAction(state, action, phase, moveRegistry)};

		THEN("it returns InvalidTarget")
		{
			REQUIRE_FALSE(result.has_value());
			CHECK((result.error() == BattleEngineError::InvalidTarget));
		}
	}

	WHEN("calling when the selected move successfully finds a target")
	{
		std::expected<void, BattleEngineError> result{validateMoveAction(state, action, phase, moveRegistry)};

		THEN("it succeeds")
		{
			REQUIRE(result.has_value());
		}
	}
}

SCENARIO("getValidationResult")
{
	Pokemon member1{makePokemon({.mMoveIDs = {toMoveID(BuiltinMoveID::Pound), MoveID{200}}, .mCurrentPP = {20, 20}, .mHealth = 50})};
	Pokemon member2{makePokemon({.mHealth = 50})};
	Pokemon member3{makePokemon({.mMoveIDs = {toMoveID(BuiltinMoveID::Pound), MoveID{200}}, .mCurrentPP = {20, 20}, .mHealth = 50})};
	Pokemon member4{makePokemon({.mHealth = 50})};
	Pokemon member5{makePokemon({.mHealth = 50})};
	Pokemon member6{makePokemon({.mHealth = 50})};

	BattleState state{
		makeBattleState({
			.mSideA
			= {{{.mPokemon = &member1, .mPosition = 1}, {.mPokemon = &member2, .mPosition = 2}, {.mPokemon = &member3, .mPosition = 3}}},
			.mSideB = {{{.mPokemon = &member6, .mPosition = 1}}},
			.mPartyA = {&member1, &member2, &member3, &member4, &member5},
			.mPartyB = {&member6},
			.mBattleStarted = true,
		}),
	};
	BattleTarget target{.mSide = Side::B, .mSlotIndex = 0};
	SwitchAction switchAction{.mSide = Side::A, .mActiveSlotIndex = 0, .mPartyIndex = 4};
	MoveAction moveAction{.mSelectedTarget = target, .mSide = Side::A, .mUserSlotIndex = 0, .mMoveSlotIndex = 0};
	BattlePhase phase{BattlePhase::AwaitingActions};
	RegistryProvider provider{getDefaultInitializedRegistryProvider()};
	const MoveRegistry *moveRegistry{provider.mMoveRegistry};

	WHEN("calling with SwitchAction")
	{
		WHEN("calling when the selected party Pokemon is trying to be switched into itself")
		{
			switchAction.mPartyIndex = 0;
			std::expected<void, BattleEngineError> result{getValidationResult(state, switchAction, phase, moveRegistry)};

			THEN("it returns PokemonAlreadyActive")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error() == BattleEngineError::PokemonAlreadyActive));
			}
		}

		WHEN("calling when the selected party Pokemon can replace the active slot")
		{
			switchAction.mPartyIndex = 3;
			std::expected<void, BattleEngineError> result{getValidationResult(state, switchAction, phase, moveRegistry)};

			THEN("it succeeds")
			{
				REQUIRE(result.has_value());
			}
		}
	}

	WHEN("calling with MoveAction")
	{
		WHEN("calling when the selected move has no remaining PP")
		{
			member1.setCurrentPP(0, 0);
			std::expected<void, BattleEngineError> result{getValidationResult(state, moveAction, phase, moveRegistry)};

			THEN("it returns NoPP")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error() == BattleEngineError::NoPP));
			}
		}

		WHEN("calling when the selected move successfully finds a target")
		{
			std::expected<void, BattleEngineError> result{getValidationResult(state, moveAction, phase, moveRegistry)};

			THEN("it succeeds")
			{
				REQUIRE(result.has_value());
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
