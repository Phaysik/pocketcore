/*! @file moveMeta.test.cpp
	@brief C++ file for running tests for the MoveMeta.
	@date 09/11/2026
	@since 0.8.7
	@version 0.12.24
	@author Matthew Moore
*/

#include "Move/moveMeta.h"

#include <algorithm>
#include <array>
#include <span>

#include "Battle/battleTargetsAndTriggers.h"
#include "Effect/builtInEffectID.h"
#include "Effect/effectID.h"
#include "Effect/effectTrigger.h"
#include "Move/moveHitPolicy.h"
#include "Move/moveID.h"
#include "Types/typeID.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Battle::BattleRangeID;
using PocketCore::Battle::BattleTargetID;
using PocketCore::Effect::BuiltinEffectID;
using PocketCore::Effect::EffectID;
using PocketCore::Effect::EffectTrigger;
using PocketCore::Effect::toEffectID;
using PocketCore::Move::baseAttackEffects;
using PocketCore::Move::baseAttackWithFlinch;
using PocketCore::Move::baseAttackWithRecoil;
using PocketCore::Move::baseAttackWithStatus;
using PocketCore::Move::FixedHitCount;
using PocketCore::Move::MoveID;
using PocketCore::Move::MoveMeta;
using PocketCore::Move::WeightedHitCount;
using PocketCore::Move::WeightedHitCountOutcome;
using PocketCore::Type::TypeID;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

SCENARIO("MoveMeta")
{
	MoveMeta meta{};
	std::span<const EffectID> base{baseAttackEffects()};

	WHEN("operator==")
	{
		GIVEN("two default constructed move metas")
		{
			MoveMeta other{};

			THEN("they are equal")
			{
				CHECK((meta == other));
			}
		}

		GIVEN("for mHitCountPolicy modified with fixed hit count in one move meta")
		{
			MoveMeta other{.mHitCountPolicy = FixedHitCount{2}};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mHitCountPolicy modified with weighted hit count in one move meta")
		{
			std::vector<WeightedHitCountOutcome> outcomes{{.mHitCount = 2, .mWeight = 0.0}};

			MoveMeta other{.mHitCountPolicy = WeightedHitCount{.mOutcomes = outcomes}};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mName modified in one move meta")
		{
			MoveMeta other{.mName = "Test"};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mTriggers modified in one move meta")
		{
			std::vector<EffectTrigger> triggers{{.mSuppressionRuleCount = 5}};

			MoveMeta other{.mTriggers = triggers};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mMoveID modified in one move meta")
		{
			MoveMeta other{.mMoveID = MoveID{200}};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mTypeID modified in one move meta")
		{
			MoveMeta other{.mTypeID = TypeID{200}};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mPower modified in one move meta")
		{
			MoveMeta other{.mPower = 10};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mTargetID modified in one move meta")
		{
			MoveMeta other{.mTargetID = BattleTargetID::AllExceptSelf};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mRangeID modified in one move meta")
		{
			MoveMeta other{.mRangeID = BattleRangeID::Adjacent};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mAccuracy modified in one move meta")
		{
			MoveMeta other{.mAccuracy = 1};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mPriority modified in one move meta")
		{
			MoveMeta other{.mPriority = 1};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mPPMaxAmount modified in one move meta")
		{
			MoveMeta other{.mPPMaxAmount = 20};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mPPDefaultAmount modified in one move meta")
		{
			MoveMeta other{.mPPDefaultAmount = 20};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mSpecial modified in one move meta")
		{
			MoveMeta other{.mSpecial = true};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("two move metas modified the same way")
		{
			MoveMeta other{.mAccuracy = 2};
			meta.mAccuracy = 2;

			THEN("they are equal")
			{
				CHECK((meta == other));
			}
		}
	}

	WHEN("baseAttackEffects")
	{
		std::array<const EffectID, 10> expected{
			toEffectID(BuiltinEffectID::PsychicTerrainPriorityBlock),
			toEffectID(BuiltinEffectID::CriticalHit),
			toEffectID(BuiltinEffectID::BaseDamage),
			toEffectID(BuiltinEffectID::Targets),
			toEffectID(BuiltinEffectID::Weather),
			toEffectID(BuiltinEffectID::Terrain),
			toEffectID(BuiltinEffectID::Randomization),
			toEffectID(BuiltinEffectID::Stab),
			toEffectID(BuiltinEffectID::TypeEffectiveness),
			toEffectID(BuiltinEffectID::BurnDamageReduction),
		};

		THEN("the returned span of effect IDs is accurate")
		{
			CHECK((base.size() == expected.size()));
			CHECK(std::equal(base.begin(), base.end(), expected.begin()));
		}
	}

	WHEN("baseAttackWithRecoil")
	{
		std::span<const EffectID> recoil{baseAttackWithRecoil()};

		std::array<const EffectID, 11> expected{
			toEffectID(BuiltinEffectID::PsychicTerrainPriorityBlock),
			toEffectID(BuiltinEffectID::CriticalHit),
			toEffectID(BuiltinEffectID::BaseDamage),
			toEffectID(BuiltinEffectID::Targets),
			toEffectID(BuiltinEffectID::Weather),
			toEffectID(BuiltinEffectID::Terrain),
			toEffectID(BuiltinEffectID::Randomization),
			toEffectID(BuiltinEffectID::Stab),
			toEffectID(BuiltinEffectID::TypeEffectiveness),
			toEffectID(BuiltinEffectID::BurnDamageReduction),
			toEffectID(BuiltinEffectID::Recoil),
		};

		THEN("the returned span of effect IDs is accurate")
		{
			CHECK((recoil.size() == expected.size()));
			CHECK(std::equal(recoil.begin(), recoil.end(), expected.begin()));
		}

		THEN("this derived list prevserves the full base prefix")
		{
			CHECK(std::equal(base.begin(), base.end(), recoil.begin()));
		}
	}

	WHEN("baseAttackWithStatus")
	{
		std::span<const EffectID> status{baseAttackWithStatus()};

		std::array<const EffectID, 11> expected{
			toEffectID(BuiltinEffectID::PsychicTerrainPriorityBlock),
			toEffectID(BuiltinEffectID::CriticalHit),
			toEffectID(BuiltinEffectID::BaseDamage),
			toEffectID(BuiltinEffectID::Targets),
			toEffectID(BuiltinEffectID::Weather),
			toEffectID(BuiltinEffectID::Terrain),
			toEffectID(BuiltinEffectID::Randomization),
			toEffectID(BuiltinEffectID::Stab),
			toEffectID(BuiltinEffectID::TypeEffectiveness),
			toEffectID(BuiltinEffectID::BurnDamageReduction),
			toEffectID(BuiltinEffectID::StatusApply),
		};

		THEN("the returned span of effect IDs is accurate")
		{
			CHECK((status.size() == expected.size()));
			CHECK(std::equal(status.begin(), status.end(), expected.begin()));
		}

		THEN("this derived list prevserves the full base prefix")
		{
			CHECK(std::equal(base.begin(), base.end(), status.begin()));
		}
	}

	WHEN("baseAttackWithFlinch")
	{
		std::span<const EffectID> flinch{baseAttackWithFlinch()};

		std::array<const EffectID, 11> expected{
			toEffectID(BuiltinEffectID::PsychicTerrainPriorityBlock),
			toEffectID(BuiltinEffectID::CriticalHit),
			toEffectID(BuiltinEffectID::BaseDamage),
			toEffectID(BuiltinEffectID::Targets),
			toEffectID(BuiltinEffectID::Weather),
			toEffectID(BuiltinEffectID::Terrain),
			toEffectID(BuiltinEffectID::Randomization),
			toEffectID(BuiltinEffectID::Stab),
			toEffectID(BuiltinEffectID::TypeEffectiveness),
			toEffectID(BuiltinEffectID::BurnDamageReduction),
			toEffectID(BuiltinEffectID::Flinch),
		};

		THEN("the returned span of effect IDs is accurate")
		{
			CHECK((flinch.size() == expected.size()));
			CHECK(std::equal(flinch.begin(), flinch.end(), expected.begin()));
		}

		THEN("this derived list prevserves the full base prefix")
		{
			CHECK(std::equal(base.begin(), base.end(), flinch.begin()));
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
