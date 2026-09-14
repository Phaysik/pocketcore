/*! @file rulesetPolicy.test.cpp
	@brief C++ file for running tests for the RulesetPolicy.
	@date 09/14/2026
	@since 0.12.31
	@version 0.12.34
	@author Matthew Moore
*/

#include "Ruleset/rulesetPolicy.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Ruleset::RulesetPolicy;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("RulesetPolicy")
{
	RulesetPolicy policy{};

	WHEN("creating a default RulesetPolicy")
	{
		THEN("they are equal")
		{
			RulesetPolicy expected{
				.mMaxSideSize = 2,
				.mMaxNonVolatileStatuses = 1,
				.mMaxVolatileStatuses = 8,
				.mMaxWeathers = 1,
				.mMaxTerrains = 1,
				.mAllowTera = true,
				.mAllowGmax = false,
				.mAllowZMoves = false,
				.mAllowMega = false,
			};

			CHECK((policy == expected));
		}
	}

	WHEN("operator==")
	{
		GIVEN("two default constructed ruleset policies")
		{
			RulesetPolicy other{};

			THEN("they are equal")
			{
				CHECK((policy == other));
			}
		}

		GIVEN("for mMaxSideSize modified in one ruleset policy")
		{
			RulesetPolicy other{.mMaxSideSize = 100};

			THEN("they are not equal")
			{
				CHECK((policy != other));
			}
		}

		GIVEN("for mMaxNonVolatileStatuses modified in one ruleset policy")
		{
			RulesetPolicy other{.mMaxNonVolatileStatuses = 100};

			THEN("they are not equal")
			{
				CHECK((policy != other));
			}
		}

		GIVEN("for mMaxVolatileStatuses modified in one ruleset policy")
		{
			RulesetPolicy other{.mMaxVolatileStatuses = 100};

			THEN("they are not equal")
			{
				CHECK((policy != other));
			}
		}

		GIVEN("for mMaxWeathers modified in one ruleset policy")
		{
			RulesetPolicy other{.mMaxWeathers = 100};

			THEN("they are not equal")
			{
				CHECK((policy != other));
			}
		}

		GIVEN("for mMaxTerrains modified in one ruleset policy")
		{
			RulesetPolicy other{.mMaxTerrains = 100};

			THEN("they are not equal")
			{
				CHECK((policy != other));
			}
		}

		GIVEN("for mAllowTera modified in one ruleset policy")
		{
			RulesetPolicy other{.mAllowTera = false};

			THEN("they are not equal")
			{
				CHECK((policy != other));
			}
		}

		GIVEN("for mAllowMega modified in one ruleset policy")
		{
			RulesetPolicy other{.mAllowMega = true};

			THEN("they are not equal")
			{
				CHECK((policy != other));
			}
		}

		GIVEN("for mAllowGmax modified in one ruleset policy")
		{
			RulesetPolicy other{.mAllowGmax = true};

			THEN("they are not equal")
			{
				CHECK((policy != other));
			}
		}

		GIVEN("for mAllowZMoves modified in one ruleset policy")
		{
			RulesetPolicy other{.mAllowZMoves = true};

			THEN("they are not equal")
			{
				CHECK((policy != other));
			}
		}

		GIVEN("two ruleset policies modified the same way")
		{
			RulesetPolicy other{.mMaxNonVolatileStatuses = 5};
			policy.mMaxNonVolatileStatuses = 5;

			THEN("they are equal")
			{
				CHECK((policy == other));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
