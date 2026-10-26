/*! @file pokemonMeta.test.cpp
	@brief C++ file for running tests for the PokemonMeta.
	@date 10/26/2026
	@since 0.12.51
	@version 0.12.53
	@author Matthew Moore
*/

#include "Pokemon/pokemonMeta.h"

#include "Ability/abilityMeta.h"
#include "Ability/builtInAbilityID.h"
#include "EggGroup/builtInEggGroupID.h"
#include "EggGroup/eggGroupID.h"
#include "Learnset/builtInLearnsetID.h"
#include "Pokemon/builtInPokemonID.h"
#include "Pokemon/pokemonEncounter.h"
#include "Pokemon/pokemonID.h"
#include "Type/builtInTypeID.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Ability::AbilityClassification;
using PocketCore::Ability::BuiltinAbilityID;
using PocketCore::Ability::toAbilityID;
using PocketCore::EggGroup::BuiltinEggGroupID;
using PocketCore::EggGroup::EggGroupID;
using PocketCore::EggGroup::toEggGroupID;
using PocketCore::Learnset::BuiltinLearnsetID;
using PocketCore::Learnset::toLearnsetID;
using PocketCore::Pokemon::BuiltinPokemonID;
using PocketCore::Pokemon::PokemonAbilityEntry;
using PocketCore::Pokemon::PokemonGender;
using PocketCore::Pokemon::PokemonID;
using PocketCore::Pokemon::PokemonMeta;
using PocketCore::Pokemon::PokemonOrigin;
using PocketCore::Pokemon::toPokemonID;
using PocketCore::Type::BuiltinTypeID;
using PocketCore::Type::toTypeID;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

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

SCENARIO("PokemonMeta")
{
	PokemonMeta meta{};

	WHEN("operator==")
	{
		GIVEN("two default constructed metas")
		{
			PokemonMeta other{};

			THEN("they are equal")
			{
				CHECK((meta == other));
			}
		}

		GIVEN("for mName modified in one meta")
		{
			PokemonMeta other{.mName = "Test"};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mAbilityPool modified in one meta")
		{
			PokemonMeta other{.mAbilityPool = {{{.mAbilityID = toAbilityID(BuiltinAbilityID::AirLock)}}}};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mGenderPool modified in one meta")
		{
			PokemonMeta other{.mGenderPool = {{PokemonGender::Female}}};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mEncounters modified in one meta")
		{
			PokemonMeta other{.mEncounters = {{{.mOrigin = PokemonOrigin::Egg}}}};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mBaseStats modified in one meta")
		{
			PokemonMeta other{.mBaseStats = {.mAttack = 12}};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mTypeIDs modified in one meta")
		{
			PokemonMeta other{.mTypeIDs = {{{toTypeID(BuiltinTypeID::Dark)}}}};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mPokemonID modified in one meta")
		{
			PokemonMeta other{.mPokemonID = toPokemonID(BuiltinPokemonID::Charizard)};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mLearnsetID modified in one meta")
		{
			PokemonMeta other{.mLearnsetID = toLearnsetID(BuiltinLearnsetID::CharmanderLearnset)};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mEggGroupID modified in one meta")
		{
			PokemonMeta other{.mEggGroupID = toEggGroupID(BuiltinEggGroupID::Water1)};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("two metas modified the same way")
		{
			PokemonMeta other{.mName = "Same Name"};
			meta.mName = "Same Name";

			THEN("they are equal")
			{
				CHECK((meta == other));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
