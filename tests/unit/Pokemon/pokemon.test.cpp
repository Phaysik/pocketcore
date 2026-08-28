/*! @file pokemon.test.cpp
	@brief C++ file for running tests for the PokemonRegistry.
	@date 08/05/2026
	@since 0.4.0
	@version 0.9.15
	@author Matthew Moore
*/

#include "Pokemon/pokemon.h"

#include <array>
#include <sstream>
#include <string>

#include "Ability/abilityID.h"
#include "Configuration/constants.h"
#include "Core/typedefs.h"
#include "Item/itemID.h"
#include "Move/moveID.h"
#include "Registry/statusRegistry.h"
#include "Status/builtInStatusID.h"
#include "Status/statusID.h"
#include "Status/statusMeta.h"
#include "Types/builtInTypeID.h"
#include "Types/typeID.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Ability::AbilityID;
using PocketCore::Configuration::MAX_STATUSES_PER_POKEMON;
using PocketCore::Configuration::MAX_TYPES_PER_POKEMON;
using PocketCore::Core::ub;
using PocketCore::Item::ItemID;
using PocketCore::Move::MoveID;
using PocketCore::Pokemon::Pokemon;
using PocketCore::Registry::Status::StatusRegistry;
using PocketCore::Status::BuiltinStatusID;
using PocketCore::Status::StatusID;
using PocketCore::Status::StatusInteractionAction;
using PocketCore::Status::StatusMeta;
using PocketCore::Status::toStatusID;
using PocketCore::Type::BuiltInTypeID;
using PocketCore::Type::NO_TYPE_ID;
using PocketCore::Type::toTypeID;
using PocketCore::Type::TypeID;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("Pokemon type identifiers")
{
	GIVEN("a Pokemon constructed without explicit types")
	{
		Pokemon pokemon{"MissingNo", 1, 1, 1, 1, 1, 1, 10, {AbilityID{}}, {ItemID{}}};

		THEN("both type slots are unassigned")
		{
			CHECK((pokemon.getTypesArray().size() == MAX_TYPES_PER_POKEMON));
			CHECK((pokemon.getTypeID(0) == NO_TYPE_ID));
			CHECK((pokemon.getTypeID(1) == NO_TYPE_ID));
		}
	}

	GIVEN("built-in and custom stable type identifiers")
	{
		TypeID builtInTypeID{toTypeID(BuiltInTypeID::Fire)};
		TypeID customTypeID{42};
		std::array<TypeID, MAX_TYPES_PER_POKEMON> typeIDs{builtInTypeID, customTypeID};
		Pokemon pokemon{"Hybrid", 1, 1, 1, 1, 1, 1, 10, {AbilityID{}}, {ItemID{}}, typeIDs};

		THEN("the constructor preserves both identifiers")
		{
			CHECK((pokemon.getTypeID(0) == builtInTypeID));
			CHECK((pokemon.getTypeID(1) == customTypeID));
		}

		WHEN("one type slot is replaced")
		{
			TypeID replacementTypeID{toTypeID(BuiltInTypeID::Water)};
			pokemon.setType(1, replacementTypeID);

			THEN("only that slot changes")
			{
				CHECK((pokemon.getTypeID(0) == builtInTypeID));
				CHECK((pokemon.getTypeID(1) == replacementTypeID));
			}
		}

		WHEN("the complete type array is replaced")
		{
			std::array<TypeID, MAX_TYPES_PER_POKEMON> replacementTypeIDs{toTypeID(BuiltInTypeID::Grass), TypeID{43}};
			pokemon.setTypesArray(replacementTypeIDs);

			THEN("the replacement array is returned unchanged")
			{
				CHECK((pokemon.getTypesArray() == replacementTypeIDs));
			}
		}
	}
}

SCENARIO("Pokemon health bounds")
{
	Pokemon pokemon{"MissingNo", 1, 1, 100, 1, 1, 1, 10, {AbilityID{}}, {ItemID{}}};

	GIVEN("a Pokemon constructed at full health")
	{
		THEN("its maximum health is retained")
		{
			CHECK((pokemon.getMaximumHealth() == 100U));
		}

		WHEN("health is assigned above the maximum")
		{
			pokemon.setHealth(120U);

			THEN("current health is clamped to the maximum")
			{
				CHECK((pokemon.getHealth() == 100U));
			}
		}
	}
}

SCENARIO("Pokemon stream output")
{
	GIVEN("a Pokemon with populated identifiers, stats, statuses, moves, and PP")
	{
		std::array<MoveID, 4> moveIDs{MoveID{10U}, MoveID{11U}, MoveID{12U}, MoveID{13U}};
		std::array<ub, 4> maxPP{15U, 20U, 25U, 30U};
		std::array<ub, 4> currentPP{5U, 10U, 15U, 20U};
		std::array<TypeID, MAX_TYPES_PER_POKEMON> typeIDs{TypeID{2U}, TypeID{3U}};
		Pokemon pokemon{"Streammon", moveIDs, maxPP, currentPP,		  101U,			102U,	150U, 103U,
						104U,		 105U,	  50U,	 {AbilityID{6U}}, {ItemID{7U}}, typeIDs};
		std::array<StatusID, MAX_STATUSES_PER_POKEMON> statusIDs{StatusID{20U}, StatusID{21U}, StatusID{22U}, StatusID{23U}, StatusID{24U}};
		pokemon.setStatusesArray(statusIDs);

		WHEN("the Pokemon is written to a stream")
		{
			std::ostringstream output;
			output << pokemon;

			THEN("all public state is written with numeric identifiers and PP")
			{
				std::string expected{
					"Pokemon {\n"
					"  Name: Streammon\n"
					"  Level: 50\n"
					"  Level Damage Factor: 22\n"
					"  Health: 150/150\n"
					"  Attack: 101\n"
					"  Defense: 102\n"
					"  Special Attack: 104\n"
					"  Special Defense: 105\n"
					"  Speed: 103\n"
					"  Type IDs: [2, 3]\n"
					"  Ability IDs: [6]\n"
					"  Item IDs: [7]\n"
					"  Status IDs: [20, 21, 22, 23, 24]\n"
					"  Moves:\n"
					"    [0] ID: 10, PP: 5/15\n"
					"    [1] ID: 11, PP: 10/20\n"
					"    [2] ID: 12, PP: 15/25\n"
					"    [3] ID: 13, PP: 20/30\n"
					"}",
				};
				CHECK((output.str() == expected));
			}
		}
	}
}

SCENARIO("Pokemon addStatus")
{
	StatusRegistry statusRegistry{};
	Pokemon pokemon{"MissingNo", 1, 1, 1, 1, 1, 1, 10, {AbilityID{}}, {ItemID{}}};

	GIVEN("a current status that blocks the incoming status")
	{
		std::array<StatusID, MAX_STATUSES_PER_POKEMON> statusIDs{};
		statusIDs.at(0) = toStatusID(BuiltinStatusID::Freeze);
		statusIDs.at(1) = toStatusID(BuiltinStatusID::Poison);
		pokemon.setStatusesArray(statusIDs);

		WHEN("the blocked status would otherwise replace another current status")
		{
			pokemon.addStatus(toStatusID(BuiltinStatusID::Toxic), statusRegistry);

			THEN("the incoming status is rejected before any current status changes")
			{
				CHECK((pokemon.getStatusesArray() == statusIDs));
			}
		}
	}

	GIVEN("a current status that the incoming status replaces")
	{
		std::array<StatusID, MAX_STATUSES_PER_POKEMON> statusIDs{};
		statusIDs.at(0) = toStatusID(BuiltinStatusID::Poison);
		pokemon.setStatusesArray(statusIDs);

		WHEN("the incoming status is applied")
		{
			pokemon.addStatus(toStatusID(BuiltinStatusID::Toxic), statusRegistry);

			THEN("the incoming status occupies the replaced status slot")
			{
				CHECK((pokemon.getStatusID(0) == toStatusID(BuiltinStatusID::Toxic)));
				CHECK((pokemon.getStatusID(1) == PocketCore::Status::NO_STATUS_ID));
			}
		}
	}

	GIVEN("several current statuses that the incoming status replaces")
	{
		StatusID incomingStatusID{toStatusID(BuiltinStatusID::Toxic)};
		auto incomingStatusIndex{statusRegistry.findIndexByStatusID(incomingStatusID)};
		REQUIRE(incomingStatusIndex.has_value());

		// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
		statusRegistry.setEntry(incomingStatusIndex.value(), StatusMeta{
			.mStatusInteractions
			= {{.mExistingStatusID = toStatusID(BuiltinStatusID::Burn), .mAction = StatusInteractionAction::ReplaceCurrent},
			   {.mExistingStatusID = toStatusID(BuiltinStatusID::Poison), .mAction = StatusInteractionAction::ReplaceCurrent},},
			.mName = "Toxic",
			.mStatusID = incomingStatusID,
		});

		std::array<StatusID, MAX_STATUSES_PER_POKEMON> statusIDs{
			toStatusID(BuiltinStatusID::Burn),
			toStatusID(BuiltinStatusID::Sleep),
			toStatusID(BuiltinStatusID::Poison),
			toStatusID(BuiltinStatusID::Paralysis),
		};
		pokemon.setStatusesArray(statusIDs);

		WHEN("the incoming status is applied")
		{
			pokemon.addStatus(incomingStatusID, statusRegistry);

			THEN("only the first replacement slot receives the incoming status")
			{
				std::array<StatusID, MAX_STATUSES_PER_POKEMON> expectedStatusIDs{
					incomingStatusID,
					toStatusID(BuiltinStatusID::Sleep),
					toStatusID(BuiltinStatusID::Paralysis),
				};
				CHECK((pokemon.getStatusesArray() == expectedStatusIDs));
			}
		}
	}

	GIVEN("several current statuses that the incoming status removes")
	{
		std::array<StatusID, MAX_STATUSES_PER_POKEMON> statusIDs{};
		statusIDs.at(0) = toStatusID(BuiltinStatusID::Burn);
		statusIDs.at(1) = toStatusID(BuiltinStatusID::Sleep);
		statusIDs.at(2) = toStatusID(BuiltinStatusID::Paralysis);
		statusIDs.at(3) = toStatusID(BuiltinStatusID::Toxic);
		pokemon.setStatusesArray(statusIDs);

		WHEN("the incoming status is applied")
		{
			pokemon.addStatus(toStatusID(BuiltinStatusID::Freeze), statusRegistry);

			THEN("the remaining statuses shift down and the incoming status uses the first empty slot")
			{
				CHECK((pokemon.getStatusID(0) == toStatusID(BuiltinStatusID::Toxic)));
				CHECK((pokemon.getStatusID(1) == toStatusID(BuiltinStatusID::Freeze)));
				CHECK((pokemon.getStatusID(2) == PocketCore::Status::NO_STATUS_ID));
				CHECK((pokemon.getStatusID(3) == PocketCore::Status::NO_STATUS_ID));
			}
		}
	}

	GIVEN("a full status array with no matching interaction")
	{
		std::array<StatusID, MAX_STATUSES_PER_POKEMON> statusIDs{
			toStatusID(BuiltinStatusID::Burn),	toStatusID(BuiltinStatusID::Sleep), toStatusID(BuiltinStatusID::Poison),
			toStatusID(BuiltinStatusID::Toxic), toStatusID(BuiltinStatusID::Burn),
		};
		pokemon.setStatusesArray(statusIDs);

		WHEN("another coexisting status is applied")
		{
			pokemon.addStatus(toStatusID(BuiltinStatusID::Paralysis), statusRegistry);

			THEN("the full status array remains unchanged")
			{
				CHECK((pokemon.getStatusesArray() == statusIDs));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
