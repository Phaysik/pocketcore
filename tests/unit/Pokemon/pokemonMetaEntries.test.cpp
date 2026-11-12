/*! @file pokemonMetaEntries.test.cpp
	@brief C++ file for running tests for the Pokemon Meta Entries.
	@date 11/09/2026
	@since 0.12.51
	@version 0.12.54
	@author Matthew Moore
*/

#include "Pokemon/pokemonMetaEntries.h"

#include "Ability/builtInAbilityID.h"
#include "Item/builtInItemID.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Ability::AbilityClassification;
using PocketCore::Ability::BuiltinAbilityID;
using PocketCore::Ability::toAbilityID;
using PocketCore::Item::BuiltinItemID;
using PocketCore::Item::toItemID;
using PocketCore::Pokemon::PokemonAbilityEntry;
using PocketCore::Pokemon::PokemonItemEntry;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("PokemonItemEntry")
{
	PokemonItemEntry itemEntry{};

	WHEN("operator==")
	{
		GIVEN("two default constructed ability entries")
		{
			PokemonItemEntry other{};

			THEN("they are equal")
			{
				CHECK((itemEntry == other));
			}
		}

		GIVEN("for mItemID modified in one ability entry")
		{
			PokemonItemEntry other{.mItemID = toItemID(BuiltinItemID::CheriBerry)};

			THEN("they are not equal")
			{
				CHECK((itemEntry != other));
			}
		}

		GIVEN("for mChance modified in one ability entry")
		{
			PokemonItemEntry other{.mChance = 0.2};

			THEN("they are not equal")
			{
				CHECK((itemEntry != other));
			}
		}

		GIVEN("two ability entries modified the same way")
		{
			PokemonItemEntry other{.mChance = 0.5};
			itemEntry.mChance = 0.5;

			THEN("they are equal")
			{
				CHECK((itemEntry == other));
			}
		}
	}
}

SCENARIO("PokemonAbilityEntry")
{
	PokemonAbilityEntry abilityEntry{};

	WHEN("operator==")
	{
		GIVEN("two default constructed ability entries")
		{
			PokemonAbilityEntry other{};

			THEN("they are equal")
			{
				CHECK((abilityEntry == other));
			}
		}

		GIVEN("for mAbilityID modified in one ability entry")
		{
			PokemonAbilityEntry other{.mAbilityID = toAbilityID(BuiltinAbilityID::CloudNine)};

			THEN("they are not equal")
			{
				CHECK((abilityEntry != other));
			}
		}

		GIVEN("for mClassification modified in one ability entry")
		{
			PokemonAbilityEntry other{.mClassification = AbilityClassification::Hidden};

			THEN("they are not equal")
			{
				CHECK((abilityEntry != other));
			}
		}

		GIVEN("two ability entries modified the same way")
		{
			PokemonAbilityEntry other{.mAbilityID = toAbilityID(BuiltinAbilityID::Elevate)};
			abilityEntry.mAbilityID = toAbilityID(BuiltinAbilityID::Elevate);

			THEN("they are equal")
			{
				CHECK((abilityEntry == other));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
