/*! @file pokemonEncounter.test.cpp
	@brief C++ file for running tests for the PokemonEncounter.
	@date 10/13/2026
	@since 0.12.51
	@version 0.12.51
	@author Matthew Moore
*/

#include "Pokemon/pokemonEncounter.h"

#include "Core/typedefs.h"
#include "Location/builtInLocationID.h"
#include "Location/locationID.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Core::us;
using PocketCore::Location::BuiltinLocationID;
using PocketCore::Location::LocationID;
using PocketCore::Location::toLocationID;
using PocketCore::Pokemon::PokemonEncounter;
using PocketCore::Pokemon::PokemonOrigin;
using PocketCore::Pokemon::SetLevelEncounter;
using PocketCore::Pokemon::VariableLevelEncounter;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("SetLevelEncounter")
{
	SetLevelEncounter variableEncounter{};

	WHEN("operator==")
	{
		GIVEN("two default constructed encounters")
		{
			SetLevelEncounter other{};

			THEN("they are equal")
			{
				CHECK((variableEncounter == other));
			}
		}

		GIVEN("for mLevel modified in one encounter")
		{
			SetLevelEncounter other{.mLevel = 23};

			THEN("they are not equal")
			{
				CHECK((variableEncounter != other));
			}
		}

		GIVEN("two encounters modified the same way")
		{
			SetLevelEncounter other{.mLevel = 36};
			variableEncounter.mLevel = 36;

			THEN("they are equal")
			{
				CHECK((variableEncounter == other));
			}
		}
	}
}

SCENARIO("VariableLevelEncounter")
{
	VariableLevelEncounter variableEncounter{};

	WHEN("operator==")
	{
		GIVEN("two default constructed encounters")
		{
			VariableLevelEncounter other{};

			THEN("they are equal")
			{
				CHECK((variableEncounter == other));
			}
		}

		GIVEN("for mMinLevel modified in one encounter")
		{
			VariableLevelEncounter other{.mMinLevel = 25};

			THEN("they are not equal")
			{
				CHECK((variableEncounter != other));
			}
		}

		GIVEN("for mMaxLevel modified in one encounter")
		{
			VariableLevelEncounter other{.mMaxLevel = 58};

			THEN("they are not equal")
			{
				CHECK((variableEncounter != other));
			}
		}

		GIVEN("two encounters modified the same way")
		{
			VariableLevelEncounter other{.mMaxLevel = 45};
			variableEncounter.mMaxLevel = 45;

			THEN("they are equal")
			{
				CHECK((variableEncounter == other));
			}
		}
	}
}

SCENARIO("PokemonEncounter")
{
	PokemonEncounter meta{};

	WHEN("operator==")
	{
		GIVEN("two default constructed encounters")
		{
			PokemonEncounter other{};

			THEN("they are equal")
			{
				CHECK((meta == other));
			}
		}

		GIVEN("for mLevel modified in one meta")
		{
			PokemonEncounter other{.mLevel = SetLevelEncounter{.mLevel = 23}};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mLocationID modified in one meta")
		{
			PokemonEncounter other{.mLocationID = toLocationID(BuiltinLocationID::Route1)};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mOrigin modified in one meta")
		{
			PokemonEncounter other{.mOrigin = PokemonOrigin::Trade};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("two encounters modified the same way")
		{
			PokemonEncounter other{.mOrigin = PokemonOrigin::Gift};
			meta.mOrigin = PokemonOrigin::Gift;

			THEN("they are equal")
			{
				CHECK((meta == other));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
