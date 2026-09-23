/*! @file statusMeta.test.cpp
	@brief C++ file for running tests for the StatusMeta.
	@date 09/23/2026
	@since 0.12.44
	@version 0.12.44
	@author Matthew Moore
*/

#include "Status/statusMeta.h"

#include "Interaction/interaction.h"
#include "Status/builtInStatusID.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Interaction::InteractionAction;
using PocketCore::Status::BuiltinStatusID;
using PocketCore::Status::StatusClassification;
using PocketCore::Status::StatusMeta;
using PocketCore::Status::toStatusID;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("StatusMeta")
{
	StatusMeta meta{};

	WHEN("operator==")
	{
		GIVEN("two default constructed metas")
		{
			StatusMeta other{};

			THEN("they are equal")
			{
				CHECK((meta == other));
			}
		}

		GIVEN("for mName modified in one meta")
		{
			StatusMeta other{.mName = "Test"};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mStatusInteractions modified in one meta")
		{
			StatusMeta other{
				.mStatusInteractions
				= {{.mExistingID = toStatusID(BuiltinStatusID::Paralysis), .mAction = InteractionAction::BlockIncoming}},
			};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mStatusID modified in one meta")
		{
			StatusMeta other{.mStatusID = toStatusID(BuiltinStatusID::Freeze)};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mStatusClassification modified in one meta")
		{
			StatusMeta other{.mStatusClassification = StatusClassification::Volatile};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("two metas modified the same way")
		{
			StatusMeta other{.mName = "Same Name"};
			meta.mName = "Same Name";

			THEN("they are equal")
			{
				CHECK((meta == other));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
