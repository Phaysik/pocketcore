#include "Move/moveMeta.h"

#include <algorithm>

#include "Effect/effectType.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Effect::EffectTypeID;
using PocketCore::Move::baseAttackEffects;
using PocketCore::Move::baseAttackWithFlinch;
using PocketCore::Move::baseAttackWithRecoil;
using PocketCore::Move::baseAttackWithStatus;
using PocketCore::Move::fieldEffectEffects;
using PocketCore::Move::protectEffects;
using PocketCore::Move::statusChangeEffects;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

SCENARIO("MoveMeta effect lists")
{
	GIVEN("the base attack effect sequence")
	{
		auto effects{baseAttackEffects()};

		THEN("it contains the expected ordered baseline effects")
		{
			REQUIRE((effects.size() == 11U));
			CHECK((effects.front() == EffectTypeID::PsychicTerrainPriorityBlock));
			CHECK((effects.at(1) == EffectTypeID::AccuracyCheck));
			CHECK((effects.at(2) == EffectTypeID::CriticalHit));
			CHECK((effects.back() == EffectTypeID::BurnDamageReduction));
		}
	}

	GIVEN("a base attack with recoil")
	{
		auto effects{baseAttackWithRecoil()};

		THEN("it appends recoil to the base sequence")
		{
			REQUIRE((effects.size() == 12U));
			CHECK((effects.back() == EffectTypeID::Recoil));
			CHECK((effects.at(0) == EffectTypeID::PsychicTerrainPriorityBlock));
			CHECK((effects.at(10) == EffectTypeID::BurnDamageReduction));
		}
	}

	GIVEN("a base attack with status")
	{
		auto effects{baseAttackWithStatus()};

		THEN("it appends status apply to the base sequence")
		{
			REQUIRE((effects.size() == 12U));
			CHECK((effects.back() == EffectTypeID::StatusApply));
			CHECK((effects.at(10) == EffectTypeID::BurnDamageReduction));
		}
	}

	GIVEN("a base attack with flinch")
	{
		auto effects{baseAttackWithFlinch()};

		THEN("it appends flinch to the base sequence")
		{
			REQUIRE((effects.size() == 12U));
			CHECK((effects.back() == EffectTypeID::Flinch));
			CHECK((effects.at(10) == EffectTypeID::BurnDamageReduction));
		}
	}

	GIVEN("status change effect sequence")
	{
		auto effects{statusChangeEffects()};

		THEN("it contains priority block accuracy check and stat change")
		{
			REQUIRE((effects.size() == 3U));
			CHECK((effects.at(0) == EffectTypeID::PsychicTerrainPriorityBlock));
			CHECK((effects.at(1) == EffectTypeID::AccuracyCheck));
			CHECK((effects.at(2) == EffectTypeID::StatChange));
		}
	}

	GIVEN("protect effect sequence")
	{
		auto effects{protectEffects()};

		THEN("it contains only priority block")
		{
			REQUIRE((effects.size() == 1U));
			CHECK((effects.front() == EffectTypeID::PsychicTerrainPriorityBlock));
		}
	}

	GIVEN("field effect sequence")
	{
		auto effects{fieldEffectEffects()};

		THEN("it contains priority block and accuracy check")
		{
			REQUIRE((effects.size() == 2U));
			CHECK((effects.at(0) == EffectTypeID::PsychicTerrainPriorityBlock));
			CHECK((effects.at(1) == EffectTypeID::AccuracyCheck));
		}
	}

	GIVEN("derived effect lists")
	{
		auto base{baseAttackEffects()};
		auto recoil{baseAttackWithRecoil()};
		auto status{baseAttackWithStatus()};
		auto flinch{baseAttackWithFlinch()};

		THEN("all derived lists preserve the full base prefix")
		{
			CHECK(std::equal(base.begin(), base.end(), recoil.begin()));
			CHECK(std::equal(base.begin(), base.end(), status.begin()));
			CHECK(std::equal(base.begin(), base.end(), flinch.begin()));
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
