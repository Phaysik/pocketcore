#include "Pokemon/pokemon.h"

#include <array>

#include "Ability/abilityID.h"
#include "Configuration/constants.h"
#include "Item/itemID.h"
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
using PocketCore::Item::ItemID;
using PocketCore::Pokemon::Pokemon;
using PocketCore::Registry::Status::StatusRegistry;
using PocketCore::Status::BuiltinStatusID;
using PocketCore::Status::StatusID;
using PocketCore::Status::StatusInteractionAction;
using PocketCore::Status::StatusMeta;
using PocketCore::Status::toStatusID;
using PocketCore::Types::BuiltInTypeID;
using PocketCore::Types::NO_TYPE_ID;
using PocketCore::Types::toTypeID;
using PocketCore::Types::TypeID;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("Pokemon type identifiers")
{
	GIVEN("a Pokemon constructed without explicit types")
	{
		Pokemon pokemon{"MissingNo", 1, 1, 1, 1, 1, 1, 10, AbilityID{}, ItemID{}};

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
		Pokemon pokemon{"Hybrid", 1, 1, 1, 1, 1, 1, 10, AbilityID{}, ItemID{}, typeIDs};

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

SCENARIO("Pokemon addStatus")
{
	StatusRegistry statusRegistry{};
	Pokemon pokemon{"MissingNo", 1, 1, 1, 1, 1, 1, 10, AbilityID{}, ItemID{}};

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
