/*! @file battleValidation.test.cpp
	@brief C++ file for running tests for the Battle validation functions.
	@date 10/20/2026
	@since 0.12.51
	@version 0.12.51
	@author Matthew Moore
*/

#include "Validation/Battle/battleValidation.h"

#include <expected>

#include "Battle/battleAction.h"
#include "Battle/battleState.h"
#include "Battle/battleState.testHelper.h"
#include "Effect/effectContext.h"
#include "Pokemon/pokemon.h"
#include "Pokemon/pokemon.testHelper.h"
#include "Validation/Battle/battleError.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Battle::BattleState;
using PocketCore::Battle::MoveAction;
using PocketCore::Battle::SwitchAction;
using PocketCore::Effect::Side;
using PocketCore::Pokemon::Pokemon;
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

SCENARIO("validateMoveAction") {}

SCENARIO("getValidationResult") {}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
