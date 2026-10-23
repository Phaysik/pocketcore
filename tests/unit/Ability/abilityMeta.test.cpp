/*! @file abilityMeta.test.cpp
	@brief C++ file for running tests for the AbilityMeta.
	@date 10/23/2026
	@since 0.12.51
	@version 0.12.53
	@author Matthew Moore
*/

#include "Ability/abilityMeta.h"

#include "Ability/abilityID.h"
#include "Ability/builtInAbilityID.h"
#include "Battle/battleTargetsAndTriggers.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Ability::AbilityClassification;
using PocketCore::Ability::AbilityID;
using PocketCore::Ability::AbilityMeta;
using PocketCore::Ability::BuiltinAbilityID;
using PocketCore::Ability::toAbilityID;
using PocketCore::Battle::BattleTargetID;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("AbilityMeta")
{
	AbilityMeta meta{};

	WHEN("operator==")
	{
		GIVEN("two default constructed metas")
		{
			AbilityMeta other{};

			THEN("they are equal")
			{
				CHECK((meta == other));
			}
		}

		GIVEN("for mName modified in one meta")
		{
			AbilityMeta other{.mName = "Test"};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mTriggers modified in one meta")
		{
			AbilityMeta other{.mTriggers = {{{.mSuppressionRuleCount = 2}}}};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mAbilityID modified in one meta")
		{
			AbilityMeta other{.mAbilityID = toAbilityID(BuiltinAbilityID::Drizzle)};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mTargetID modified in one meta")
		{
			AbilityMeta other{.mTargetID = BattleTargetID::AllExceptSelf};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mClassification modified in one meta")
		{
			AbilityMeta other{.mClassification = AbilityClassification::Hidden};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("two metas modified the same way")
		{
			AbilityMeta other{.mName = "Same Name"};
			meta.mName = "Same Name";

			THEN("they are equal")
			{
				CHECK((meta == other));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
