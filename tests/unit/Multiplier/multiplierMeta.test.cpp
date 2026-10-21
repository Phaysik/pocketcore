/*! @file multiplierMeta.test.cpp
	@brief C++ file for running tests for the MultiplierMeta.
	@date 10/13/2026
	@since 0.12.51
	@version 0.12.51
	@author Matthew Moore
*/

#include "Multiplier/multiplierMeta.h"

#include "Multiplier/builtInMultiplierID.h"
#include "Multiplier/multiplierID.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Multiplier::BuiltinMultiplierID;
using PocketCore::Multiplier::MultiplierApplicationPolicy;
using PocketCore::Multiplier::MultiplierID;
using PocketCore::Multiplier::MultiplierMeta;
using PocketCore::Multiplier::toMultiplierID;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("MultiplierMeta")
{
	MultiplierMeta meta{};

	WHEN("operator==")
	{
		GIVEN("two default constructed metas")
		{
			MultiplierMeta other{};

			THEN("they are equal")
			{
				CHECK((meta == other));
			}
		}

		GIVEN("for mName modified in one meta")
		{
			MultiplierMeta other{.mName = "Test"};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mMultiplierID modified in one meta")
		{
			MultiplierMeta other{.mMultiplierID = toMultiplierID(BuiltinMultiplierID::Critical)};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mApplicationPolicy modified in one meta")
		{
			MultiplierMeta other{.mApplicationPolicy = MultiplierApplicationPolicy::Floor};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("two metas modified the same way")
		{
			MultiplierMeta other{.mName = "Same Name"};
			meta.mName = "Same Name";

			THEN("they are equal")
			{
				CHECK((meta == other));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
