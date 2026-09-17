/*! @file rulesetPolicyError.test.cpp
	@brief C++ file for running tests for the RulesetPolicyError file.
	@date 09/17/2026
	@since 0.12.36
	@version 0.12.36
	@author Matthew Moore
*/

#include "Ruleset/rulesetPolicyError.h"

#include "Ruleset/rulesetPolicy.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Ruleset::RulesetPolicy;
using PocketCore::Ruleset::RulesetPolicyError;
using PocketCore::Ruleset::validateRulesetPolicy;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("validatePolicyError")
{
	RulesetPolicy policy{};

	WHEN("calling with the minimum accepted values")
	{
		GIVEN("mMaxSideSize")
		{
			policy.mMaxSideSize = 0;

			THEN("it is a valid object")
			{
				CHECK(validateRulesetPolicy(policy).has_value());
			}
		}

		GIVEN("mMaxNonVolatileStatuses")
		{
			policy.mMaxNonVolatileStatuses = 0;

			THEN("it is a valid object")
			{
				CHECK(validateRulesetPolicy(policy).has_value());
			}
		}

		GIVEN("mMaxVolatileStatuses")
		{
			policy.mMaxVolatileStatuses = 0;

			THEN("it is a valid object")
			{
				CHECK(validateRulesetPolicy(policy).has_value());
			}
		}

		GIVEN("mMaxWeathers")
		{
			policy.mMaxWeathers = 0;

			THEN("it is a valid object")
			{
				CHECK(validateRulesetPolicy(policy).has_value());
			}
		}

		GIVEN("mMaxTerrains")
		{
			policy.mMaxTerrains = 0;

			THEN("it is a valid object")
			{
				CHECK(validateRulesetPolicy(policy).has_value());
			}
		}
	}

	WHEN("calling with the maximum accepted values")
	{
		GIVEN("mMaxSideSize")
		{
			policy.mMaxSideSize = 2;

			THEN("it is a valid object")
			{
				CHECK(validateRulesetPolicy(policy).has_value());
			}
		}

		GIVEN("mMaxNonVolatileStatuses")
		{
			policy.mMaxNonVolatileStatuses = 1;

			THEN("it is a valid object")
			{
				CHECK(validateRulesetPolicy(policy).has_value());
			}
		}

		GIVEN("mMaxVolatileStatuses")
		{
			policy.mMaxVolatileStatuses = 8;

			THEN("it is a valid object")
			{
				CHECK(validateRulesetPolicy(policy).has_value());
			}
		}

		GIVEN("mMaxWeathers")
		{
			policy.mMaxWeathers = 1;

			THEN("it is a valid object")
			{
				CHECK(validateRulesetPolicy(policy).has_value());
			}
		}

		GIVEN("mMaxTerrains")
		{
			policy.mMaxTerrains = 1;

			THEN("it is a valid object")
			{
				CHECK(validateRulesetPolicy(policy).has_value());
			}
		}
	}

	WHEN("calling with a value greater by one than the accepted values")
	{
		GIVEN("mMaxSideSize")
		{
			policy.mMaxSideSize = 7;

			THEN("it is a valid object")
			{
				REQUIRE_FALSE(validateRulesetPolicy(policy).has_value());
				CHECK((validateRulesetPolicy(policy).error() == RulesetPolicyError::MAX_SIDE_SIZE_ERROR));
			}
		}

		GIVEN("mMaxNonVolatileStatuses")
		{
			policy.mMaxNonVolatileStatuses = 6;

			THEN("it is a valid object")
			{
				REQUIRE_FALSE(validateRulesetPolicy(policy).has_value());
				CHECK((validateRulesetPolicy(policy).error() == RulesetPolicyError::MAX_NON_VOLATIVE_STATUSES_ERROR));
			}
		}

		GIVEN("mMaxVolatileStatuses")
		{
			policy.mMaxVolatileStatuses = 9;

			THEN("it is a valid object")
			{
				REQUIRE_FALSE(validateRulesetPolicy(policy).has_value());
				CHECK((validateRulesetPolicy(policy).error() == RulesetPolicyError::MAX_VOLATILE_STATUSES_ERROR));
			}
		}

		GIVEN("mMaxWeathers")
		{
			policy.mMaxWeathers = 6;

			THEN("it is a valid object")
			{
				REQUIRE_FALSE(validateRulesetPolicy(policy).has_value());
				CHECK((validateRulesetPolicy(policy).error() == RulesetPolicyError::MAX_WEATHERS_ON_FIELD_ERROR));
			}
		}

		GIVEN("mMaxTerrains")
		{
			policy.mMaxTerrains = 6;

			THEN("it is a valid object")
			{
				REQUIRE_FALSE(validateRulesetPolicy(policy).has_value());
				CHECK((validateRulesetPolicy(policy).error() == RulesetPolicyError::MAX_TERRAINS_ON_FIELD_ERROR));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
