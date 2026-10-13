/*! @file itemMeta.test.cpp
	@brief C++ file for running tests for the ItemMeta.
	@date 10/13/2026
	@since 0.12.51
	@version 0.12.51
	@author Matthew Moore
*/

#include "Item/itemMeta.h"

#include "Battle/battleTargetsAndTriggers.h"
#include "Item/builtInItemID.h"
#include "Item/itemID.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Battle::BattleTargetID;
using PocketCore::Item::BuiltinItemID;
using PocketCore::Item::ItemID;
using PocketCore::Item::ItemMeta;
using PocketCore::Item::toItemID;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("ItemMeta")
{
	ItemMeta meta{};

	WHEN("operator==")
	{
		GIVEN("two default constructed metas")
		{
			ItemMeta other{};

			THEN("they are equal")
			{
				CHECK((meta == other));
			}
		}

		GIVEN("for mName modified in one meta")
		{
			ItemMeta other{.mName = "Test"};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mTriggers modified in one meta")
		{
			ItemMeta other{.mTriggers = {{{.mSuppressionRuleCount = 2}}}};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mItemID modified in one meta")
		{
			ItemMeta other{.mItemID = toItemID(BuiltinItemID::CheriBerry)};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mTargetID modified in one meta")
		{
			ItemMeta other{.mTargetID = BattleTargetID::AllExceptSelf};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mIsConsumable modified in one meta")
		{
			ItemMeta other{.mIsConsumable = true};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("two metas modified the same way")
		{
			ItemMeta other{.mName = "Same Name"};
			meta.mName = "Same Name";

			THEN("they are equal")
			{
				CHECK((meta == other));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
