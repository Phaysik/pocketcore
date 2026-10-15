/*! @file battleAction.test.cpp
	@brief C++ file for running tests for the objects in battleAction.h.
	@date 10/15/2026
	@since 0.12.51
	@version 0.12.51
	@author Matthew Moore
*/

#include "Battle/battleAction.h"

#include "Effect/effectContext.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Battle::BattleTarget;
using PocketCore::Effect::Side;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("BattleTargets")
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

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
