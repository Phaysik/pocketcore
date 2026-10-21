/*! @file battleAction.test.cpp
	@brief C++ file for running tests for the objects in battleAction.h.
	@date 10/21/2026
	@since 0.12.51
	@version 0.12.51
	@author Matthew Moore
*/

#include "Battle/battleAction.h"

#include "Effect/effectContext.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Battle::BattleTarget;
using PocketCore::Battle::MoveAction;
using PocketCore::Effect::Side;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("BattleTarget")
{
	BattleTarget target{};

	WHEN("operator==")
	{
		GIVEN("two default constructed targets")
		{
			BattleTarget other{};

			THEN("they are equal")
			{
				CHECK((target == other));
			}
		}

		GIVEN("for mSide modified in one target")
		{
			BattleTarget other{.mSide = Side::B};

			THEN("they are not equal")
			{
				CHECK((target != other));
			}
		}

		GIVEN("for mSlotIndex modified in one target")
		{
			BattleTarget other{.mSlotIndex = 2};

			THEN("they are not equal")
			{
				CHECK((target != other));
			}
		}

		GIVEN("two targets modified the same way")
		{
			BattleTarget other{.mSlotIndex = 5};
			target.mSlotIndex = 5;

			THEN("they are equal")
			{
				CHECK((target == other));
			}
		}
	}
}

SCENARIO("MoveAction")
{
	MoveAction action{};

	WHEN("operator==")
	{
		GIVEN("two default constructed actions")
		{
			MoveAction other{};

			THEN("they are equal")
			{
				CHECK((action == other));
			}
		}

		GIVEN("for mSelectedTarget modified in one action")
		{
			BattleTarget target{.mSide = Side::B, .mSlotIndex = 2};
			MoveAction other{.mSelectedTarget = target};

			THEN("they are not equal")
			{
				CHECK((action != other));
			}
		}

		GIVEN("for mSide modified in one action")
		{
			MoveAction other{.mSide = Side::B};

			THEN("they are not equal")
			{
				CHECK((action != other));
			}
		}

		GIVEN("for mUserSlotIndex modified in one action")
		{
			MoveAction other{.mUserSlotIndex = 2};

			THEN("they are not equal")
			{
				CHECK((action != other));
			}
		}

		GIVEN("for mMoveSlotIndex modified in one action")
		{
			MoveAction other{.mMoveSlotIndex = 2};

			THEN("they are not equal")
			{
				CHECK((action != other));
			}
		}

		GIVEN("two actions modified the same way")
		{
			MoveAction other{.mMoveSlotIndex = 5};
			action.mMoveSlotIndex = 5;

			THEN("they are equal")
			{
				CHECK((action == other));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
