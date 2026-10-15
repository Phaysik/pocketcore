/*! @file typeMeta.test.cpp
	@brief C++ file for running tests for the TypeMeta.
	@date 10/13/2026
	@since 0.12.51
	@version 0.12.51
	@author Matthew Moore
*/

#include "Type/typeMeta.h"

#include "Type/builtInTypeID.h"
#include "Type/typeEffectiveness.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Type::BuiltinTypeID;
using PocketCore::Type::toTypeID;
using PocketCore::Type::TypeEffectiveness;
using PocketCore::Type::TypeMeta;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("TypeMeta")
{
	TypeMeta meta{};

	WHEN("operator==")
	{
		GIVEN("two default constructed metas")
		{
			TypeMeta other{};

			THEN("they are equal")
			{
				CHECK((meta == other));
			}
		}

		GIVEN("for mOffensiveMatchups modified in one meta")
		{
			TypeMeta other{.mOffensiveMatchups = {{TypeEffectiveness::E}}};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mName modified in one meta")
		{
			TypeMeta other{.mName = "Test"};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mTypeID modified in one meta")
		{
			TypeMeta other{.mTypeID = toTypeID(BuiltinTypeID::Dark)};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("two metas modified the same way")
		{
			TypeMeta other{.mName = "Same Name"};
			meta.mName = "Same Name";

			THEN("they are equal")
			{
				CHECK((meta == other));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
