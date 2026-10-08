/*! @file learnsetMeta.test.cpp
	@brief C++ file for running tests for the LearnsetMeta.
	@date 10/08/2026
	@since 0.12.50
	@version 0.12.50
	@author Matthew Moore
*/

#include "Learnset/learnsetMeta.h"

#include "Learnset/builtInLearnsetID.h"
#include "Move/builtInMoveID.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Learnset::AcquisitionMethod;
using PocketCore::Learnset::BuiltinLearnsetID;
using PocketCore::Learnset::LearnsetMeta;
using PocketCore::Learnset::MoveLearnEntry;
using PocketCore::Learnset::toLearnsetID;
using PocketCore::Move::BuiltinMoveID;
using PocketCore::Move::toMoveID;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("MoveLearnEntry")
{
	MoveLearnEntry meta{};

	WHEN("operator==")
	{
		GIVEN("two default constructed move learn entries")
		{
			MoveLearnEntry other{};

			THEN("they are equal")
			{
				CHECK((meta == other));
			}
		}

		GIVEN("for mMoveID modified in one meta")
		{
			MoveLearnEntry other{.mMoveID = toMoveID(BuiltinMoveID::HydroSteam)};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mLevelRequirement modified in one meta")
		{
			MoveLearnEntry other{.mLevelRequirement = 20};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mAcquisitionMethod modified in one meta")
		{
			MoveLearnEntry other{.mAcquisitionMethod = AcquisitionMethod::TM};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("two move learn entries modified the same way")
		{
			MoveLearnEntry other{.mLevelRequirement = 15};
			meta.mLevelRequirement = 15;

			THEN("they are equal")
			{
				CHECK((meta == other));
			}
		}
	}
}

SCENARIO("LearnsetMeta")
{
	LearnsetMeta meta{};

	WHEN("operator==")
	{
		GIVEN("two default constructed metas")
		{
			LearnsetMeta other{};

			THEN("they are equal")
			{
				CHECK((meta == other));
			}
		}

		GIVEN("for mName modified in one meta")
		{
			LearnsetMeta other{.mName = "Test"};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mEntries modified in one meta")
		{
			LearnsetMeta other{.mEntries = {{.mLevelRequirement = 20}}};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mLearnsetID modified in one meta")
		{
			LearnsetMeta other{.mLearnsetID = toLearnsetID(BuiltinLearnsetID::BulbasaurLearnset)};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("two metas modified the same way")
		{
			LearnsetMeta other{.mName = "Same Name"};
			meta.mName = "Same Name";

			THEN("they are equal")
			{
				CHECK((meta == other));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
