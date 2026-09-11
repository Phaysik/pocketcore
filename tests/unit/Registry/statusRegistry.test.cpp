/*! @file statusRegistry.test.cpp
	@brief C++ file for running tests for the StatusRegistry.
	@date 09/10/2026
	@since 0.12.20
	@version 0.12.20
	@author Matthew Moore
*/

#include "Registry/statusRegistry.h"

#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <utility>

#include "Core/typedefs.h"
#include "Interaction/interaction.h"
#include "Status/builtInStatusID.h"
#include "Status/constants.h"
#include "Status/statusID.h"
#include "Status/statusMeta.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Core::ub;
using PocketCore::Interaction::InteractionAction;
using PocketCore::Registry::Status::StatusRegistry;
using PocketCore::Status::BuiltinStatusID;
using PocketCore::Status::NO_STATUS_ID;
using PocketCore::Status::STATUS_NAME_BURN;
using PocketCore::Status::STATUS_NAME_FREEZE;
using PocketCore::Status::STATUS_NAME_NONE;
using PocketCore::Status::STATUS_NAME_PARALYSIS;
using PocketCore::Status::STATUS_NAME_POISON;
using PocketCore::Status::STATUS_NAME_SLEEP;
using PocketCore::Status::STATUS_NAME_TOXIC;
using PocketCore::Status::StatusID;
using PocketCore::Status::StatusMeta;
using PocketCore::Status::toStatusID;

template <typename Registry>
concept PubliclyStructurallyMutable = requires(Registry &registry) { registry.setAmountRegistered(0); };

static_assert(!PubliclyStructurallyMutable<StatusRegistry>);

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("StatusRegistry")
{
	StatusRegistry registry{};
	ub finalStatusUnderlyingValue{std::to_underlying(BuiltinStatusID::FinalStatus)};

	GIVEN("a default constructed status registry")
	{
		THEN("Paralysis has the appropriate properties")
		{
			StatusMeta expected{
				.mName = std::string(STATUS_NAME_PARALYSIS),
				.mStatusInteractions = {{.mExistingID = toStatusID(BuiltinStatusID::Freeze), .mAction = InteractionAction::BlockIncoming}},
				.mStatusID = toStatusID(BuiltinStatusID::Paralysis),
			};

			const StatusMeta *actual{registry.getStatusMetadata(toStatusID(BuiltinStatusID::Paralysis))};

			CHECK((expected == *actual));
		}

		THEN("Burn has the appropriate properties")
		{
			StatusMeta expected{
				.mName = std::string(STATUS_NAME_BURN),
				.mStatusInteractions = {{.mExistingID = toStatusID(BuiltinStatusID::Freeze), .mAction = InteractionAction::BlockIncoming}},
				.mStatusID = toStatusID(BuiltinStatusID::Burn),
			};

			const StatusMeta *actual{registry.getStatusMetadata(toStatusID(BuiltinStatusID::Burn))};

			CHECK((expected == *actual));
		}

		THEN("Sleep has the appropriate properties")
		{
			StatusMeta expected{
				.mName = std::string(STATUS_NAME_SLEEP),
				.mStatusInteractions = {{.mExistingID = toStatusID(BuiltinStatusID::Freeze), .mAction = InteractionAction::BlockIncoming}},
				.mStatusID = toStatusID(BuiltinStatusID::Sleep),
			};

			const StatusMeta *actual{registry.getStatusMetadata(toStatusID(BuiltinStatusID::Sleep))};

			CHECK((expected == *actual));
		}

		THEN("Freeze has the appropriate properties")
		{
			StatusMeta expected{
    			.mName = std::string(STATUS_NAME_FREEZE),
    			.mStatusInteractions
    			= {{.mExistingID = toStatusID(BuiltinStatusID::Burn), .mAction = InteractionAction::RemoveCurrent},
    			   {.mExistingID = toStatusID(BuiltinStatusID::Sleep), .mAction = InteractionAction::RemoveCurrent},
    			   {.mExistingID = toStatusID(BuiltinStatusID::Paralysis), .mAction = InteractionAction::RemoveCurrent},
    			},
    			.mStatusID = toStatusID(BuiltinStatusID::Freeze),
			};

			const StatusMeta *actual{registry.getStatusMetadata(toStatusID(BuiltinStatusID::Freeze))};

			CHECK((expected == *actual));
		}

		THEN("Poison has the appropriate properties")
		{
			StatusMeta expected{
				.mName = std::string(STATUS_NAME_POISON),
				.mStatusInteractions = {{.mExistingID = toStatusID(BuiltinStatusID::Freeze), .mAction = InteractionAction::BlockIncoming}},
				.mStatusID = toStatusID(BuiltinStatusID::Poison),
			};

			const StatusMeta *actual{registry.getStatusMetadata(toStatusID(BuiltinStatusID::Poison))};

			CHECK((expected == *actual));
		}

		THEN("Toxic has the appropriate properties")
		{
			StatusMeta expected{
    			.mName = std::string(STATUS_NAME_TOXIC),
    			.mStatusInteractions
    			= {{.mExistingID = toStatusID(BuiltinStatusID::Freeze), .mAction = InteractionAction::BlockIncoming},
    			   {.mExistingID = toStatusID(BuiltinStatusID::Poison), .mAction = InteractionAction::ReplaceCurrent},},
    			.mStatusID = toStatusID(BuiltinStatusID::Toxic),
			};

			const StatusMeta *actual{registry.getStatusMetadata(toStatusID(BuiltinStatusID::Toxic))};

			CHECK((expected == *actual));
		}
	}

	GIVEN("getStatusMetadata")
	{
		THEN("unknown IDs are absent")
		{
			CHECK((registry.getStatusMetadata(StatusID{200}) == nullptr));
		}

		THEN("the metadata is retrieved when accessed by a valid Status ID")
		{
			StatusMeta expected{
				.mName = std::string(STATUS_NAME_NONE),
				.mStatusID = toStatusID(BuiltinStatusID::None),
			};

			CHECK((expected == *registry.getStatusMetadata(NO_STATUS_ID)));
		}
	}

	GIVEN("getStatusID")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(registry.getStatusID("Unknown").has_value());
		}

		THEN("the Status ID is retrieved by valid Status name")
		{
			std::optional<StatusID> statusID{registry.getStatusID(STATUS_NAME_NONE)};

			REQUIRE(statusID.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((statusID.value() == toStatusID(BuiltinStatusID::None)));
		}
	}

	GIVEN("getStatusName")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(registry.getStatusName(StatusID{200}).has_value());
		}

		THEN("a registered status name is returned by stable ID")
		{
			std::optional<std::string_view> statusName{registry.getStatusName(toStatusID(BuiltinStatusID::None))};

			REQUIRE(statusName.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((statusName.value() == STATUS_NAME_NONE));
		}
	}

	GIVEN("getAmountRegistered")
	{
		THEN("the registered span contains the exact amount of built-in entries")
		{
			CHECK((registry.getAmountRegistered() == finalStatusUnderlyingValue));
		}
	}

	GIVEN("getEntry")
	{
		THEN("an invalid internal array index has no metadata")
		{
			CHECK((registry.getEntry(2'000) == nullptr));
		}

		THEN("a valid internal array index has metadata")
		{
			StatusMeta expected{
				.mName = std::string(STATUS_NAME_NONE),
				.mStatusID = toStatusID(BuiltinStatusID::None),
			};

			const StatusMeta *statusMeta{registry.getEntry(0)};

			REQUIRE((statusMeta != nullptr));
			CHECK((*statusMeta == expected));
		}
	}

	GIVEN("getRegisteredStatuses")
	{
		THEN("the amount of statuses returned matches the amount that are built-in")
		{
			CHECK((registry.getRegisteredStatuses().size() == finalStatusUnderlyingValue));
		}
	}

	GIVEN("getNextStatusID")
	{
		THEN("the next available stable Status ID is after all built in status IDs")
		{
			CHECK((registry.getNextStatusID() == finalStatusUnderlyingValue));
		}
	}

	GIVEN("findIndexByStatusID")
	{
		THEN("an unknown stable ID has no internal index")
		{
			std::optional<ub> statusIndex{registry.findIndexByStatusID(StatusID{200})};
			CHECK_FALSE(statusIndex.has_value());
		}

		THEN("the internal array index is retrieved by valid Status ID")
		{
			std::optional<ub> statusIndex{registry.findIndexByStatusID(NO_STATUS_ID)};

			REQUIRE(statusIndex.has_value());
			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((statusIndex.value() == 0));
		}
	}

	GIVEN("hasStatus")
	{
		WHEN("calling the string_view overload")
		{
			THEN("an unknown status name has no entry")
			{
				CHECK_FALSE(registry.hasStatus("Unknown"));
			}

			THEN("a known status name has an entry")
			{
				CHECK(registry.hasStatus(STATUS_NAME_NONE));
			}
		}

		WHEN("calling the StatusID overload")
		{
			THEN("an unknown status ID has no entry")
			{
				CHECK_FALSE(registry.hasStatus(StatusID{200}));
			}

			THEN("a known status ID has an entry")
			{
				CHECK(registry.hasStatus(NO_STATUS_ID));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
