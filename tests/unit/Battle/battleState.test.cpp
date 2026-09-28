/*! @file battleState.test.cpp
	@brief C++ file for running tests for the BattleState.
	@date 09/28/2026
	@since 0.12.45
	@version 0.12.45
	@author Matthew Moore
*/

#include "Battle/battleState.h"

#include <sstream>

#include "Ability/abilityID.h"
#include "Configuration/constants.h"
#include "Item/itemID.h"
#include "Move/builtInMoveID.h"
#include "Move/moveID.h"
#include "Nature/natureID.h"
#include "Pokemon/pokemon.h"
#include "Pokemon/pokemon.testHelper.h"
#include "Status/builtInStatusID.h"
#include "Status/statusID.h"
#include "Status/volatileStatus.h"
#include "Types/typeID.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Ability::AbilityID;
using PocketCore::Battle::BattleSlot;
using PocketCore::Battle::DamageFormulaModifiers;
using PocketCore::Battle::StatStages;
using PocketCore::Configuration::MAX_VOLATILE_STATUSES_PER_POKEMON;
using PocketCore::Item::ItemID;
using PocketCore::Move::BuiltinMoveID;
using PocketCore::Move::MoveID;
using PocketCore::Move::toMoveID;
using PocketCore::Nature::NatureID;
using PocketCore::Pokemon::Pokemon;
using PocketCore::Status::BuiltinStatusID;
using PocketCore::Status::StatusID;
using PocketCore::Status::toStatusID;
using PocketCore::Status::VolatileStatus;
using PocketCore::Testing::makePokemon;
using PocketCore::Type::TypeID;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("StatStages")
{
	GIVEN("operator<<")
	{
		StatStages stages{.mDefense = 2, .mSpAttack = 1, .mAccuracy = 4, .mEvasion = 3};

		WHEN("the StatStages are written to a stream")
		{
			std::ostringstream output;
			output << stages;

			THEN("all public state is written with numeric identifiers and PP")
			{
				std::string_view expected{
					"Stat Stages {\n"
					"  Attack: 0\n"
					"  Defense: 2\n"
					"  Special Attack: 1\n"
					"  Special Defense: 0\n"
					"  Speed: 0\n"
					"  Accuracy: 4\n"
					"  Evasion: 3\n"
					"}",
				};

				CHECK((output.str() == expected));
			}
		}
	}
}

SCENARIO("DamageFormulaModifiers")
{
	GIVEN("operator<<")
	{
		DamageFormulaModifiers modifiers{.mHealthModifier = 2.3, .mDefenseModifier = 1.2, .mSpeedModifier = 0.85};

		WHEN("the DamageFormulaModifiers are written to a stream")
		{
			std::ostringstream output;
			output << modifiers;

			THEN("all public state is written with numeric identifiers and PP")
			{
				std::string_view expected{
					"Damage Formula Modifiers {\n"
					"  Health: 2.3\n"
					"  Attack: 1\n"
					"  Defense: 1.2\n"
					"  Special Attack: 1\n"
					"  Special Defense: 1\n"
					"  Speed: 0.85\n"
					"}",
				};

				CHECK((output.str() == expected));
			}
		}
	}
}

SCENARIO("BattleSlot")
{
	GIVEN("operator<<")
	{
		StatStages stages{.mDefense = 2, .mSpAttack = 1, .mAccuracy = 4, .mEvasion = 3};
		DamageFormulaModifiers modifiers{.mHealthModifier = 2.3, .mDefenseModifier = 1.2, .mSpeedModifier = 0.85};
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
				.mStatusIDs = {StatusID{20}, StatusID{21}, StatusID{22}, StatusID{23}, StatusID{24}},
				.mMoveIDs = {MoveID{10}, MoveID{11}, MoveID{12}, MoveID{13}},
				.mMaxPP = {15, 20, 25, 30},
				.mCurrentPP = {5, 10, 15, 20},
				.mTypesIDs = {TypeID{2}, TypeID{3}},
				.mAbilityIDs = {AbilityID{6}},
				.mItemIDs = {ItemID{7}},
				.mNatureIDs = {NatureID{1}},
				.mHealth = 150,
				.mLevel = 50,
			}),
		};
		std::array<VolatileStatus, MAX_VOLATILE_STATUSES_PER_POKEMON> volatileStatuses{
			{{.mStatusID{toStatusID(BuiltinStatusID::Autotomize)}}, {.mStatusID{toStatusID(BuiltinStatusID::AquaRing)}}},
		};

		BattleSlot slot{
			.mDamageFormulaModifiers = modifiers,
			.mVolatileStatuses = volatileStatuses,
			.mPokemon = &pokemon,
			.mStatStages = stages,
			.mChoiceLockedMove = toMoveID(BuiltinMoveID::Pound),
			.mPosition = 3,
			.mSleepCounter = 2,
			.mToxicCounter = 3,
			.mProtectionCounter = 1,
			.mIsProtected = true,
			.mIsFlinched = false,
			.mIsGrounded = true,
			.mFaintProcessed = false,
		};

		WHEN("the pokemon is a nullptr")
		{
			std::ostringstream output;
			BattleSlot newSlot{.mPokemon = nullptr};

			output << newSlot;

			THEN("nothing is returned")
			{
				std::string_view expected{};

				CHECK((output.str() == expected));
			}
		}

		WHEN("the BattleSlot are written to a stream")
		{
			std::ostringstream output;
			output << slot;

			THEN("all public state is written with numeric identifiers and PP")
			{
				std::string_view expected{
					"Battle Slot {\n"
					"  Damage Formula Modifiers {\n"
					"    Health: 2.3\n"
					"    Attack: 1\n"
					"    Defense: 1.2\n"
					"    Special Attack: 1\n"
					"    Special Defense: 1\n"
					"    Speed: 0.85\n"
					"  }\n"
					"  Volatile Status IDs: [7, 8, 0, 0, 0, 0, 0, 0]\n"
					"  Pokemon {\n"
					"    Name: TestMon\n"
					"    Level: 50\n"
					"    Level Damage Factor: 22\n"
					"    Health: 150/210\n"
					"      IV: 0\n"
					"      EV: 0\n"
					"    Attack: 106\n"
					"      IV: 0\n"
					"      EV: 0\n"
					"    Defense: 107\n"
					"      IV: 0\n"
					"      EV: 0\n"
					"    Special Attack: 108\n"
					"      IV: 0\n"
					"      EV: 0\n"
					"    Special Defense: 109\n"
					"      IV: 0\n"
					"      EV: 0\n"
					"    Speed: 110\n"
					"      IV: 0\n"
					"      EV: 0\n"
					"    Type IDs: [2, 3]\n"
					"    Nature IDs: [1]\n"
					"    Ability IDs: [6]\n"
					"    Item IDs: [7]\n"
					"    Non-Volatile Status IDs: [20, 21, 22, 23, 24]\n"
					"    Moves:\n"
					"      [0] ID: 10, PP: 5/15\n"
					"      [1] ID: 11, PP: 10/20\n"
					"      [2] ID: 12, PP: 15/25\n"
					"      [3] ID: 13, PP: 20/30\n"
					"  }\n"
					"  Stat Stages {\n"
					"    Attack: 0\n"
					"    Defense: 2\n"
					"    Special Attack: 1\n"
					"    Special Defense: 0\n"
					"    Speed: 0\n"
					"    Accuracy: 4\n"
					"    Evasion: 3\n"
					"  }\n"
					"  Choice Locked Move ID: 1\n"
					"  Position: 3\n"
					"  Sleep: 2\n"
					"  Toxic: 3\n"
					"  Protection: 1\n"
					"  Is Protected: true\n"
					"  Is Flinched: false\n"
					"  Is Grounded: true\n"
					"  Faint Processed: false\n"
					"}",
				};

				CHECK((output.str() == expected));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
