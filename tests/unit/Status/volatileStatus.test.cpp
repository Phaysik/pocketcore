/*! @file volatileStatus.test.cpp
	@brief C++ file for running tests for the VolatileStatus.
	@date 09/28/2026
	@since 0.12.45
	@version 0.12.45
	@author Matthew Moore
*/

#include "Status/volatileStatus.h"

#include "Status/builtInStatusID.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Status::BuiltinStatusID;
using PocketCore::Status::toStatusID;
using PocketCore::Status::VolatileStatus;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("VolatileStatus")
{
	VolatileStatus meta{};

	WHEN("operator==")
	{
		GIVEN("two default constructed metas")
		{
			VolatileStatus other{};

			THEN("they are equal")
			{
				CHECK((meta == other));
			}
		}

		GIVEN("for mStatusID modified in one meta")
		{
			VolatileStatus other{.mStatusID = toStatusID(BuiltinStatusID::Autotomize)};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("two metas modified the same way")
		{
			VolatileStatus other{.mStatusID = toStatusID(BuiltinStatusID::Autotomize)};
			meta.mStatusID = toStatusID(BuiltinStatusID::Autotomize);

			THEN("they are equal")
			{
				CHECK((meta == other));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
