/*! @file moveHitPolicy.test.cpp
	@brief C++ file for running tests for the MoveHitPolicy.
	@date 09/11/2026
	@since 0.12.22
	@version 0.12.22
	@author Matthew Moore
*/

#include "Move/moveHitPolicy.h"

#include <vector>

#include <catch2/catch_test_macros.hpp>

using PocketCore::Move::FixedHitCount;
using PocketCore::Move::WeightedHitCount;
using PocketCore::Move::WeightedHitCountOutcome;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

SCENARIO("FixedHitCount")
{
	FixedHitCount fixedHitCount{};

	WHEN("operator==")
	{
		GIVEN("two default constructed fixed hit counts")
		{
			FixedHitCount other{};

			THEN("they are equal")
			{
				CHECK((fixedHitCount == other));
			}
		}

		GIVEN("for mHitCount modified in on fixed hit count")
		{
			FixedHitCount other{.mHitCount = 2};

			THEN("they are not equal")
			{
				CHECK((fixedHitCount != other));
			}
		}

		GIVEN("two fixed hit counts modified the same way")
		{
			FixedHitCount other{.mHitCount = 2};
			fixedHitCount.mHitCount = 2;

			THEN("they are equal")
			{
				CHECK((fixedHitCount == other));
			}
		}
	}
}

SCENARIO("WeightedHitCountOutcome")
{
	WeightedHitCountOutcome weightedHitCountOutcome{};

	WHEN("operator==")
	{
		GIVEN("two default constructed weighted hit count outcomes")
		{
			WeightedHitCountOutcome other{};

			THEN("they are equal")
			{
				CHECK((weightedHitCountOutcome == other));
			}
		}

		GIVEN("for mHitCount modified in on weighted hit count outcome")
		{
			WeightedHitCountOutcome other{.mHitCount = 2};

			THEN("they are not equal")
			{
				CHECK((weightedHitCountOutcome != other));
			}
		}

		GIVEN("for mWeight modified in on weighted hit count outcome")
		{
			WeightedHitCountOutcome other{.mWeight = 2.0};

			THEN("they are not equal")
			{
				CHECK((weightedHitCountOutcome != other));
			}
		}

		GIVEN("two weighted hit count outcomes modified the same way")
		{
			WeightedHitCountOutcome other{.mHitCount = 2};
			weightedHitCountOutcome.mHitCount = 2;

			THEN("they are equal")
			{
				CHECK((weightedHitCountOutcome == other));
			}
		}
	}
}

SCENARIO("WeightedHitCount")
{
	WeightedHitCount weightedHitCount{};

	WHEN("operator==")
	{
		GIVEN("two default constructed weighted hit counts")
		{
			WeightedHitCount other{};

			THEN("they are equal")
			{
				CHECK((weightedHitCount == other));
			}
		}

		GIVEN("for mHitCount modified in on weighted hit count")
		{
			std::vector<WeightedHitCountOutcome> outcomes{{.mHitCount = 2}};

			WeightedHitCount other{.mOutcomes = outcomes};

			THEN("they are not equal")
			{
				CHECK((weightedHitCount != other));
			}
		}

		GIVEN("two weighted hit counts modified the same way")
		{
			std::vector<WeightedHitCountOutcome> outcomes{{.mHitCount = 3, .mWeight = 2.0}};

			WeightedHitCount other{.mOutcomes = outcomes};
			weightedHitCount.mOutcomes = outcomes;

			THEN("they are equal")
			{
				CHECK((weightedHitCount == other));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
