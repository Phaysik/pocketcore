/*! @file effectSourceAndSuppression.test.cpp
	@brief C++ file for running tests for the SuppressionRule.
	@date 10/13/2026
	@since 0.12.51
	@version 0.12.51
	@author Matthew Moore
*/

#include "Effect/effectSourceAndSuppression.h"

#include "Ability/builtInAbilityID.h"
#include "Battle/battleTargetsAndTriggers.h"
#include "Item/builtInItemID.h"
#include "Move/builtInMoveID.h"
#include "Nature/builtInNatureID.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Ability::BuiltinAbilityID;
using PocketCore::Ability::toAbilityID;
using PocketCore::Battle::BattleEventID;
using PocketCore::Battle::BattleEventRole;
using PocketCore::Effect::EffectSource;
using PocketCore::Effect::SuppressionRule;
using PocketCore::Item::BuiltinItemID;
using PocketCore::Item::toItemID;
using PocketCore::Move::BuiltinMoveID;
using PocketCore::Move::toMoveID;
using PocketCore::Nature::BuiltinNatureID;
using PocketCore::Nature::toNatureID;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("SuppressionRule")
{
	SuppressionRule meta{};

	WHEN("operator==")
	{
		GIVEN("two default constructed metas")
		{
			SuppressionRule other{};

			THEN("they are equal")
			{
				CHECK((meta == other));
			}
		}

		GIVEN("for mTargetAbilityID modified in one meta")
		{
			SuppressionRule other{.mTargetAbilityID = toAbilityID(BuiltinAbilityID::CloudNine)};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mTargetItemID modified in one meta")
		{
			SuppressionRule other{.mTargetItemID = toItemID(BuiltinItemID::CheriBerry)};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mTargetMoveID modified in one meta")
		{
			SuppressionRule other{.mTargetMoveID = toMoveID(BuiltinMoveID::Pound)};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mTargetNatureID modified in one meta")
		{
			SuppressionRule other{.mTargetNatureID = toNatureID(BuiltinNatureID::Hasty)};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mTargetTrigger modified in one meta")
		{
			SuppressionRule other{.mTargetTrigger = BattleEventID::DamageCalculation};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mTargetRole modified in one meta")
		{
			SuppressionRule other{.mTargetRole = BattleEventRole::Target};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mTargetSource modified in one meta")
		{
			SuppressionRule other{.mTargetSource = EffectSource::Hazard};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("two metas modified the same way")
		{
			SuppressionRule other{.mTargetSource = EffectSource::Item};
			meta.mTargetSource = EffectSource::Item;

			THEN("they are equal")
			{
				CHECK((meta == other));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
