/*! @file rulesetPolicy.test.cpp
	@brief C++ file for running tests for the RulesetPresets.
	@date 09/14/2026
	@since 0.12.33
	@version 0.12.33
	@author Matthew Moore
*/

#include "Ruleset/rulesetPresets.h"

#include "Ruleset/rulesetPolicy.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Ruleset::GEN6_BASE_GAME;
using PocketCore::Ruleset::GEN7_BASE_GAME;
using PocketCore::Ruleset::GEN8_BASE_GAME;
using PocketCore::Ruleset::GEN9_BASE_GAME;
using PocketCore::Ruleset::GEN9_SMOGON_SINGLES;
using PocketCore::Ruleset::GEN9_VGC_DOUBLES;
using PocketCore::Ruleset::RulesetPolicy;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("RulesetPresets")
{
	WHEN("handling the base games presets")
	{
		GIVEN("GEN9")
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

			THEN("the defaults are properly assigned")
			{
				CHECK((GEN9_BASE_GAME == expected));
			}
		}

		GIVEN("GEN8")
		{
			RulesetPolicy expected{
				.mMaxSideSize = 2,
				.mMaxNonVolatileStatuses = 1,
				.mMaxVolatileStatuses = 8,
				.mMaxWeathers = 1,
				.mMaxTerrains = 1,
				.mAllowTera = false,
				.mAllowGmax = true,
				.mAllowZMoves = false,
				.mAllowMega = false,
			};

			THEN("the defaults are properly assigned")
			{
				CHECK((GEN8_BASE_GAME == expected));
			}
		}

		GIVEN("GEN7")
		{
			RulesetPolicy expected{
				.mMaxSideSize = 2,
				.mMaxNonVolatileStatuses = 1,
				.mMaxVolatileStatuses = 8,
				.mMaxWeathers = 1,
				.mMaxTerrains = 1,
				.mAllowTera = false,
				.mAllowGmax = false,
				.mAllowZMoves = true,
				.mAllowMega = true,
			};

			THEN("the defaults are properly assigned")
			{
				CHECK((GEN7_BASE_GAME == expected));
			}
		}

		GIVEN("GEN6")
		{
			RulesetPolicy expected{
				.mMaxSideSize = 2,
				.mMaxNonVolatileStatuses = 1,
				.mMaxVolatileStatuses = 8,
				.mMaxWeathers = 1,
				.mMaxTerrains = 1,
				.mAllowTera = false,
				.mAllowGmax = false,
				.mAllowZMoves = false,
				.mAllowMega = true,
			};

			THEN("the defaults are properly assigned")
			{
				CHECK((GEN6_BASE_GAME == expected));
			}
		}
	}

	WHEN("handling the vgc double presets")
	{
		GIVEN("GEN9")
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

			THEN("the defaults are properly assigned")
			{
				CHECK((GEN9_VGC_DOUBLES == expected));
			}
		}
	}

	WHEN("handling the smogon singles presets")
	{
		GIVEN("GEN9")
		{
			RulesetPolicy expected{
				.mMaxSideSize = 1,
				.mMaxNonVolatileStatuses = 1,
				.mMaxVolatileStatuses = 8,
				.mMaxWeathers = 1,
				.mMaxTerrains = 1,
				.mAllowTera = true,
				.mAllowGmax = false,
				.mAllowZMoves = false,
				.mAllowMega = false,
			};

			THEN("the defaults are properly assigned")
			{
				CHECK((GEN9_SMOGON_SINGLES == expected));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
