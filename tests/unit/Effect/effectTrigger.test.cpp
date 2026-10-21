/*! @file effectTrigger.test.cpp
	@brief C++ file for running tests for the EffectTrigger.
	@date 10/13/2026
	@since 0.12.51
	@version 0.12.51
	@author Matthew Moore
*/

#include "Effect/effectTrigger.h"

#include "Battle/battleTargetsAndTriggers.h"
#include "Effect/builtInEffectID.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Battle::BattleEventID;
using PocketCore::Battle::BattleEventRole;
using PocketCore::Effect::BuiltinEffectID;
using PocketCore::Effect::EffectTrigger;
using PocketCore::Effect::toEffectID;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("EffectTrigger")
{
	EffectTrigger meta{};

	WHEN("operator==")
	{
		GIVEN("two default constructed metas")
		{
			EffectTrigger other{};

			THEN("they are equal")
			{
				CHECK((meta == other));
			}
		}

		GIVEN("for mSuppressionRules modified in one meta")
		{
			EffectTrigger other{.mSuppressionRules = {{{.mTargetRole = BattleEventRole::Target}}}};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mEffects modified in one meta")
		{
			EffectTrigger other{.mEffects = {toEffectID(BuiltinEffectID::AccuracyCheck)}};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mTrigger modified in one meta")
		{
			EffectTrigger other{.mTrigger = BattleEventID::DamageCalculation};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mRole modified in one meta")
		{
			EffectTrigger other{.mRole = BattleEventRole::Target};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mSuppressionRuleCount modified in one meta")
		{
			EffectTrigger other{.mSuppressionRuleCount = 20};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("two metas modified the same way")
		{
			EffectTrigger other{.mSuppressionRuleCount = 20};
			meta.mSuppressionRuleCount = 20;

			THEN("they are equal")
			{
				CHECK((meta == other));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
