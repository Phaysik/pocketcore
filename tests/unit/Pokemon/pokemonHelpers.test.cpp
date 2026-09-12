/*! @file pokemonHelpers.test.cpp
	@brief C++ file for running tests for the Pokemon helper functions.
	@date 09/12/2026
	@since 0.12.28
	@version 0.12.28
	@author Matthew Moore
*/

#include "Pokemon/pokemonHelpers.h"

#include "Ability/builtInAbilityID.h"
#include "Item/builtInItemID.h"
#include "Move/builtInMoveID.h"
#include "Nature/builtInNatureID.h"
#include "Pokemon/pokemon.h"
#include "Pokemon/pokemon.testHelper.h"
#include "Registry/registryProvider.h"
#include "Registry/registryProvider.testHelper.h"
#include "Status/builtInStatusID.h"
#include "Types/builtInTypeID.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Ability::BuiltinAbilityID;
using PocketCore::Ability::toAbilityID;
using PocketCore::Item::BuiltinItemID;
using PocketCore::Item::toItemID;
using PocketCore::Move::BuiltinMoveID;
using PocketCore::Move::toMoveID;
using PocketCore::Nature::BuiltinNatureID;
using PocketCore::Nature::toNatureID;
using PocketCore::Pokemon::Pokemon;
using PocketCore::Registry::RegistryProvider;
using PocketCore::Status::BuiltinStatusID;
using PocketCore::Status::toStatusID;
using PocketCore::Testing::getDefaultInitializedRegistryProvider;
using PocketCore::Testing::makePokemon;
using PocketCore::Type::BuiltinTypeID;
using PocketCore::Type::toTypeID;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("Pokemon free function")
{
	GIVEN("printPokemonWithNames")
	{
		Pokemon pokemon{
			makePokemon({
				.mName = "TestMon",
				.mStats = {
					.mMaxHealth = 150,
					.mAttack = 101,
					.mDefense = 102,
					.mSpAttack = 103,
					.mSpDefense = 104,
					.mSpeed = 105,
				},
				.mStatusIDs
				= {toStatusID(BuiltinStatusID::Poison), toStatusID(BuiltinStatusID::Toxic), toStatusID(BuiltinStatusID::Paralysis),
				   toStatusID(BuiltinStatusID::Sleep), toStatusID(BuiltinStatusID::Freeze),},
				.mMoveIDs = {toMoveID(BuiltinMoveID::Pound), toMoveID(BuiltinMoveID::KarateChop), toMoveID(BuiltinMoveID::Facade), toMoveID(BuiltinMoveID::HydroSteam)},
				.mMaxPP = {15, 20, 25, 30},
				.mCurrentPP = {5, 10, 15, 20},
				.mTypesIDs = {toTypeID(BuiltinTypeID::Fire), toTypeID(BuiltinTypeID::Bug)},
				.mAbilityIDs = {toAbilityID(BuiltinAbilityID::AirLock)},
				.mItemIDs = {toItemID(BuiltinItemID::CheriBerry)},
				.mNatureIDs = {toNatureID(BuiltinNatureID::Hardy)},
				.mHealth = 150,
				.mLevel = 50,
			}),
		};

		RegistryProvider provider{getDefaultInitializedRegistryProvider()};

		WHEN("the Pokemon is written to a stream")
		{
			std::ostringstream output;
			printPokemonWithNames(output, pokemon, provider);

			THEN("all public state is written with numeric identifiers and PP")
			{
				std::string_view expected{
					"Pokemon {\n"
					"  Name: TestMon\n"
					"  Level: 50\n"
					"  Level Damage Factor: 22\n"
					"  Health: 150/210\n"
					"    IV: 0\n"
					"    EV: 0\n"
					"  Attack: 106\n"
					"    IV: 0\n"
					"    EV: 0\n"
					"  Defense: 107\n"
					"    IV: 0\n"
					"    EV: 0\n"
					"  Special Attack: 108\n"
					"    IV: 0\n"
					"    EV: 0\n"
					"  Special Defense: 109\n"
					"    IV: 0\n"
					"    EV: 0\n"
					"  Speed: 110\n"
					"    IV: 0\n"
					"    EV: 0\n"
					"  Types:\n"
					"    [0]:\n"
					"      ID: 2\n"
					"      Name: Fire\n"
					"    [1]:\n"
					"      ID: 12\n"
					"      Name: Bug\n"
					"  Natures:\n"
					"    [0]:\n"
					"      ID: 1\n"
					"      Name: Hardy\n"
					"  Abilities:\n"
					"    [0]:\n"
					"      ID: 6\n"
					"      Name: Air Lock\n"
					"  Items:\n"
					"    [0]:\n"
					"      ID: 1\n"
					"      Name: Cheri Berry\n"
					"  Statuses:\n"
					"    [0]:\n"
					"      ID: 5\n"
					"      Name: Poison\n"
					"    [1]:\n"
					"      ID: 6\n"
					"      Name: Toxic\n"
					"    [2]:\n"
					"      ID: 1\n"
					"      Name: Paralysis\n"
					"    [3]:\n"
					"      ID: 3\n"
					"      Name: Sleep\n"
					"    [4]:\n"
					"      ID: 4\n"
					"      Name: Freeze\n"
					"  Moves:\n"
					"    [0]:\n"
					"      ID: 1\n"
					"      Name: Pound\n"
					"      PP: 5/15\n"
					"    [1]:\n"
					"      ID: 2\n"
					"      Name: Karate Chop\n"
					"      PP: 10/20\n"
					"    [2]:\n"
					"      ID: 3\n"
					"      Name: Facade\n"
					"      PP: 15/25\n"
					"    [3]:\n"
					"      ID: 4\n"
					"      Name: Hydro Steam\n"
					"      PP: 20/30\n"
					"}",
				};

				CHECK((output.str() == expected));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
