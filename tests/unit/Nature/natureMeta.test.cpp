/*! @file natureMeta.test.cpp
	@brief C++ file for running tests for the NatureMeta.
	@date 10/13/2026
	@since 0.12.51
	@version 0.12.51
	@author Matthew Moore
*/

#include "Nature/natureMeta.h"

#include "Battle/battleTargetsAndTriggers.h"
#include "Nature/builtInNatureID.h"
#include "Nature/natureID.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Battle::BattleTargetID;
using PocketCore::Nature::BuiltinNatureID;
using PocketCore::Nature::NatureID;
using PocketCore::Nature::NatureMeta;
using PocketCore::Nature::toNatureID;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("NatureMeta")
{
	NatureMeta meta{};

	WHEN("operator==")
	{
		GIVEN("two default constructed metas")
		{
			NatureMeta other{};

			THEN("they are equal")
			{
				CHECK((meta == other));
			}
		}

		GIVEN("for mStatMultipliers modified in one meta")
		{
			NatureMeta other{.mStatMultipliers = {{0.2, 1.2}}};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mName modified in one meta")
		{
			NatureMeta other{.mName = "Test"};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mTriggers modified in one meta")
		{
			NatureMeta other{.mTriggers = {{{.mSuppressionRuleCount = 2}}}};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mTargetID modified in one meta")
		{
			NatureMeta other{.mTargetID = BattleTargetID::AllExceptSelf};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mNatureID modified in one meta")
		{
			NatureMeta other{.mNatureID = toNatureID(BuiltinNatureID::Hasty)};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("two metas modified the same way")
		{
			NatureMeta other{.mName = "Same Name"};
			meta.mName = "Same Name";

			THEN("they are equal")
			{
				CHECK((meta == other));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
