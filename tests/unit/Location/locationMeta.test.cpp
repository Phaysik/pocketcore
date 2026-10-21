/*! @file locationMeta.test.cpp
	@brief C++ file for running tests for the LocationMeta.
	@date 10/13/2026
	@since 0.12.51
	@version 0.12.51
	@author Matthew Moore
*/

#include "Location/locationMeta.h"

#include "Location/builtInLocationID.h"
#include "Location/locationID.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Location::BuiltinLocationID;
using PocketCore::Location::LocationID;
using PocketCore::Location::LocationMeta;
using PocketCore::Location::toLocationID;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("LocationMeta")
{
	LocationMeta meta{};

	WHEN("operator==")
	{
		GIVEN("two default constructed metas")
		{
			LocationMeta other{};

			THEN("they are equal")
			{
				CHECK((meta == other));
			}
		}

		GIVEN("for mName modified in one meta")
		{
			LocationMeta other{.mName = "Test"};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mLocationID modified in one meta")
		{
			LocationMeta other{.mLocationID = toLocationID(BuiltinLocationID::Route1)};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("two metas modified the same way")
		{
			LocationMeta other{.mName = "Same Name"};
			meta.mName = "Same Name";

			THEN("they are equal")
			{
				CHECK((meta == other));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
