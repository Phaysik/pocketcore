/*! @file eggGroupMeta.test.cpp
	@brief C++ file for running tests for the EggGroupMeta.
	@date 10/26/2026
	@since 0.12.53
	@version 0.12.53
	@author Matthew Moore
*/

#include "EggGroup/eggGroupMeta.h"

#include "EggGroup/builtInEggGroupID.h"
#include "EggGroup/eggGroupID.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::EggGroup::BuiltinEggGroupID;
using PocketCore::EggGroup::EggGroupID;
using PocketCore::EggGroup::EggGroupMeta;
using PocketCore::EggGroup::toEggGroupID;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("EggGroupMeta")
{
	EggGroupMeta abilityEntry{};

	WHEN("operator==")
	{
		GIVEN("two default constructed metas")
		{
			EggGroupMeta other{};

			THEN("they are equal")
			{
				CHECK((abilityEntry == other));
			}
		}

		GIVEN("for mName modified in one meta")
		{
			EggGroupMeta other{.mName = "Test"};

			THEN("they are not equal")
			{
				CHECK((abilityEntry != other));
			}
		}

		GIVEN("for mEggGroupID modified in one meta")
		{
			EggGroupMeta other{.mEggGroupID = toEggGroupID(BuiltinEggGroupID::Amorphous)};

			THEN("they are not equal")
			{
				CHECK((abilityEntry != other));
			}
		}

		GIVEN("two metas modified the same way")
		{
			EggGroupMeta other{.mEggGroupID = toEggGroupID(BuiltinEggGroupID::HumanLike)};
			abilityEntry.mEggGroupID = toEggGroupID(BuiltinEggGroupID::HumanLike);

			THEN("they are equal")
			{
				CHECK((abilityEntry == other));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
