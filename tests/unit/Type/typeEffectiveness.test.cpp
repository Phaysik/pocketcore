/*! @file typeEffectiveness.test.cpp
	@brief C++ file for running tests for the TypeEffectiveness.
	@date 10/13/2026
	@since 0.12.51
	@version 0.12.51
	@author Matthew Moore
*/

#include "Type/typeEffectiveness.h"

#include "Utility/Math/floatUtility.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Type::getEffectivenessValue;
using PocketCore::Type::TypeEffectiveness;
using PocketCore::Utility::Math::approximatelyEqualAbsRel;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("getEffectivenessValue")
{
	WHEN("calling with NVE")
	{
		double result{getEffectivenessValue(TypeEffectiveness::NVE)};

		THEN("it returns 0.5")
		{
			CHECK(approximatelyEqualAbsRel(result, 0.5));
		}
	}

	WHEN("calling with E")
	{
		double result{getEffectivenessValue(TypeEffectiveness::E)};

		THEN("it returns 1.0")
		{
			CHECK(approximatelyEqualAbsRel(result, 1.0));
		}
	}

	WHEN("calling with SE")
	{
		double result{getEffectivenessValue(TypeEffectiveness::SE)};

		THEN("it returns 2.0")
		{
			CHECK(approximatelyEqualAbsRel(result, 2.0));
		}
	}

	WHEN("calling with NE")
	{
		double result{getEffectivenessValue(TypeEffectiveness::NE)};

		THEN("it returns 0.0")
		{
			CHECK(approximatelyEqualAbsRel(result, 0.0));
		}
	}

	WHEN("calling with NOT_DEFINED")
	{
		double result{getEffectivenessValue(TypeEffectiveness::NOT_DEFINED)};

		THEN("it returns 0.0")
		{
			CHECK(approximatelyEqualAbsRel(result, 0.0));
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
