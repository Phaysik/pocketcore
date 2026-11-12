/*! @file pokemonEncounter.test.cpp
	@brief C++ file for running tests for the PokemonEncounter.
	@date 11/09/2026
	@since 0.12.51
	@version 0.12.54
	@author Matthew Moore
*/

#include "Pokemon/pokemonEncounter.h"

#include <array>

#include "Ability/builtInAbilityID.h"
#include "Configuration/constants.h"
#include "Core/typedefs.h"
#include "Item/builtInItemID.h"
#include "Item/itemID.h"
#include "Location/builtInLocationID.h"
#include "Location/locationID.h"
#include "Move/builtInMoveID.h"
#include "Move/moveID.h"
#include "Nature/builtInNatureID.h"
#include "Nature/natureID.h"
#include "Pokemon/pokemonMetaEntries.h"
#include "Pokemon/pokemonStats.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Ability::BuiltinAbilityID;
using PocketCore::Ability::toAbilityID;
using PocketCore::Configuration::MAX_ABILITIES_PER_POKEMON;
using PocketCore::Configuration::MAX_ITEMS_PER_POKEMON;
using PocketCore::Configuration::MAX_MOVES_PER_POKEMON;
using PocketCore::Configuration::MAX_NATURES_PER_POKEMON;
using PocketCore::Core::us;
using PocketCore::Item::BuiltinItemID;
using PocketCore::Item::ItemID;
using PocketCore::Item::toItemID;
using PocketCore::Location::BuiltinLocationID;
using PocketCore::Location::LocationID;
using PocketCore::Location::toLocationID;
using PocketCore::Move::BuiltinMoveID;
using PocketCore::Move::MoveID;
using PocketCore::Move::toMoveID;
using PocketCore::Nature::BuiltinNatureID;
using PocketCore::Nature::NatureID;
using PocketCore::Nature::toNatureID;
using PocketCore::Pokemon::POKEMON_STAT_COUNT;
using PocketCore::Pokemon::PokemonAbilityEntry;
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

		GIVEN("for mAbilityEntries modified in one meta")
		{
			std::array<PokemonAbilityEntry, MAX_ABILITIES_PER_POKEMON> abilityEntries{
				{{.mAbilityID = toAbilityID(BuiltinAbilityID::CloudNine)}},
			};
			PokemonEncounter other{.mAbilityEntries = abilityEntries};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mMoveIDs modified in one meta")
		{
			std::array<MoveID, MAX_MOVES_PER_POKEMON> moveIDs{toMoveID(BuiltinMoveID::HydroSteam)};
			PokemonEncounter other{.mMoveIDs = moveIDs};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mItemIDs modified in one meta")
		{
			std::array<ItemID, MAX_ITEMS_PER_POKEMON> itemIDs{toItemID(BuiltinItemID::CheriBerry)};
			PokemonEncounter other{.mItemIDs = itemIDs};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mNatureIDs modified in one meta")
		{
			std::array<NatureID, MAX_NATURES_PER_POKEMON> natureIDs{toNatureID(BuiltinNatureID::Gentle)};
			PokemonEncounter other{.mNatureIDs = natureIDs};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mIVs modified in one meta")
		{
			std::array<us, POKEMON_STAT_COUNT> ivs{2, 5, 12, 24, 14, 31};
			PokemonEncounter other{.mIVs = ivs};

			THEN("they are not equal")
			{
				CHECK((meta != other));
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
