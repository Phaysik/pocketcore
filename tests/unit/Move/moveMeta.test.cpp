#include "Move/moveMeta.h"

#include <algorithm>

#include "Effect/builtInEffectID.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Effect::BuiltinEffectID;
using PocketCore::Effect::toEffectID;
using PocketCore::Move::baseAttackEffects;
using PocketCore::Move::baseAttackWithFlinch;
using PocketCore::Move::baseAttackWithRecoil;
using PocketCore::Move::baseAttackWithStatus;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

SCENARIO("MoveMeta effect lists")
{
	GIVEN("the base attack effect sequence")
	{
		auto effects{baseAttackEffects()};

		THEN("it contains the expected ordered baseline effects")
		{
			REQUIRE((effects.size() == 10U));
			CHECK((effects.front() == toEffectID(BuiltinEffectID::PsychicTerrainPriorityBlock)));
			CHECK((effects.at(1) == toEffectID(BuiltinEffectID::CriticalHit)));
			CHECK((effects.back() == toEffectID(BuiltinEffectID::BurnDamageReduction)));
		}
	}

	GIVEN("a base attack with recoil")
	{
		auto effects{baseAttackWithRecoil()};

		THEN("it appends recoil to the base sequence")
		{
			REQUIRE((effects.size() == 11U));
			CHECK((effects.back() == toEffectID(BuiltinEffectID::Recoil)));
			CHECK((effects.at(0) == toEffectID(BuiltinEffectID::PsychicTerrainPriorityBlock)));
			CHECK((effects.at(9) == toEffectID(BuiltinEffectID::BurnDamageReduction)));
		}
	}

	GIVEN("a base attack with status")
	{
		auto effects{baseAttackWithStatus()};

		THEN("it appends status apply to the base sequence")
		{
			REQUIRE((effects.size() == 11U));
			CHECK((effects.back() == toEffectID(BuiltinEffectID::StatusApply)));
			CHECK((effects.at(9) == toEffectID(BuiltinEffectID::BurnDamageReduction)));
		}
	}

	GIVEN("a base attack with flinch")
	{
		auto effects{baseAttackWithFlinch()};

		THEN("it appends flinch to the base sequence")
		{
			REQUIRE((effects.size() == 11U));
			CHECK((effects.back() == toEffectID(BuiltinEffectID::Flinch)));
			CHECK((effects.at(9) == toEffectID(BuiltinEffectID::BurnDamageReduction)));
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
