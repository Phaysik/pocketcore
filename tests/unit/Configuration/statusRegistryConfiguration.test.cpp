/*! @file statusRegistryConfiguration.test.cpp
	@brief C++ file for running tests for the StatusRegistryConfiguration.
	@date 09/10/2026
	@since 0.8.7
	@version 0.12.20
	@author Matthew Moore
*/

#include "Configuration/statusRegistryConfiguration.h"

#include <expected>
#include <format>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "Configuration/constants.h"
#include "Core/typedefs.h"
#include "Registry/registryError.h"
#include "Registry/statusRegistry.h"
#include "Status/builtInStatusID.h"
#include "Status/constants.h"
#include "Status/statusID.h"
#include "Status/statusMeta.h"
#include "Utility/Debug/Logging/logging.testHelper.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Configuration::MAX_STATUSES;
using PocketCore::Configuration::RegistryError;
using PocketCore::Configuration::StatusRegistryConfiguration;
using PocketCore::Core::ub;
using PocketCore::Core::us;
using PocketCore::Registry::RegistryErrorInfo;
using PocketCore::Registry::Status::StatusRegistry;
using PocketCore::Status::BuiltinStatusID;
using PocketCore::Status::NO_STATUS_ID;
using PocketCore::Status::STATUS_NAME_BURN;
using PocketCore::Status::STATUS_NAME_NONE;
using PocketCore::Status::StatusID;
using PocketCore::Status::StatusMeta;
using PocketCore::Status::toStatusID;
using PocketCore::Testing::ensureLoggerInitialized;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("StatusRegistryConfiguration")
{
	ensureLoggerInitialized("status registry configuration test", "statusRegistryConfiguration_test.log");

	StatusRegistryConfiguration config{};
	StatusRegistry registry{};
	ub finalStatusUnderlyingValue{std::to_underlying(BuiltinStatusID::FinalStatus)};

	GIVEN("getAmountRegistered")
	{
		THEN("the default registry returns the expected amount of built-in statuses")
		{
			CHECK((config.getAmountRegistered() == finalStatusUnderlyingValue));
		}
	}

	GIVEN("getRuntimeRegistry")
	{
		THEN("the runtime registry is the same as the default registry")
		{
			CHECK((config.getRuntimeRegistry() == registry));
		}
	}

	GIVEN("getStatusMetadata")
	{
		THEN("unknown IDs are absent")
		{
			CHECK((config.getStatusMetadata(StatusID{200}) == nullptr));
		}

		THEN("the metadata is retrieved when accessed by a valid Status ID")
		{
			StatusMeta expected{
				.mName = std::string(STATUS_NAME_NONE),
				.mStatusID = toStatusID(BuiltinStatusID::None),
			};

			CHECK((expected == *config.getStatusMetadata(NO_STATUS_ID)));
		}
	}

	GIVEN("getStatusID")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(config.getStatusID("Unknown").has_value());
		}

		THEN("the Status ID is retrieved by valid Status name")
		{
			std::optional<StatusID> statusID{config.getStatusID(STATUS_NAME_NONE)};

			REQUIRE(statusID.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((statusID.value() == toStatusID(BuiltinStatusID::None)));
		}
	}

	GIVEN("getStatusName")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(config.getStatusName(StatusID{200}).has_value());
		}

		THEN("a registered status name is returned by stable ID")
		{
			std::optional<std::string_view> statusName{config.getStatusName(toStatusID(BuiltinStatusID::None))};

			REQUIRE(statusName.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((statusName.value() == STATUS_NAME_NONE));
		}
	}

	GIVEN("getRegisteredStatuses")
	{
		THEN("the amount of statuses returned matches the amount that are built-in")
		{
			CHECK((config.getRegisteredStatuses().size() == finalStatusUnderlyingValue));
		}
	}

	GIVEN("hasStatus")
	{
		WHEN("calling the string_view overload")
		{
			THEN("an unknown status name has no entry")
			{
				CHECK_FALSE(config.hasStatus("Unknown"));
			}

			THEN("a known status name has an entry")
			{
				CHECK(config.hasStatus(STATUS_NAME_NONE));
			}
		}

		WHEN("calling the StatusID overload")
		{
			THEN("an unknown status ID has no entry")
			{
				CHECK_FALSE(config.hasStatus(StatusID{200}));
			}

			THEN("a known status ID has an entry")
			{
				CHECK(config.hasStatus(NO_STATUS_ID));
			}
		}
	}

	GIVEN("addStatus")
	{
		WHEN("trying to add an status past the capacity")
		{
			us newStatusCount{finalStatusUnderlyingValue};

			for (us i{0}; i < MAX_STATUSES - finalStatusUnderlyingValue; ++i)
			{
				std::string name{std::format("String_{:04}", i)};
				StatusMeta definition{.mName = name};
				std::expected<StatusID, RegistryErrorInfo> result{config.addStatus(definition)};

				REQUIRE(result.has_value());

				StatusID assignedID{result.value()};
				CHECK((assignedID.getValue() == newStatusCount++));
			}

			std::string name{std::format("String_{:04}", MAX_STATUSES + 1)};

			StatusMeta definition{.mName = name};
			std::expected<StatusID, RegistryErrorInfo> result{config.addStatus(definition)};

			THEN("registration reports a max capacity and nothing is added")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::MaxCapacity));
				CHECK((config.getAmountRegistered() == MAX_STATUSES));
			}
		}

		WHEN("an status whose name is already in use is added")
		{
			StatusMeta definition{.mName = std::string(STATUS_NAME_BURN)};
			std::expected<StatusID, RegistryErrorInfo> result{config.addStatus(definition)};

			THEN("registration reports a duplicate status and nothing is added")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::DuplicateStatus));
				CHECK((config.getAmountRegistered() == finalStatusUnderlyingValue));
			}
		}

		WHEN("a unique status definition is added")
		{
			StatusMeta definition{.mName = "TestStatusName"};

			std::expected<StatusID, RegistryErrorInfo> result{config.addStatus(definition)};

			THEN("it receives the first custom stable ID and owns its trigger data")
			{
				REQUIRE(result.has_value());
				StatusID assignedID{result.value()};
				CHECK((assignedID.getValue() == finalStatusUnderlyingValue));

				const StatusMeta *metadata{config.getStatusMetadata(assignedID)};

				REQUIRE((metadata != nullptr));
				CHECK((metadata->mName == "TestStatusName"));
			}
		}
	}

	GIVEN("addStatuses")
	{
		WHEN("trying to add an status past the capacity")
		{
			std::vector<StatusMeta> statusMetas;

			for (us i{0}; i < MAX_STATUSES - finalStatusUnderlyingValue; ++i)
			{
				std::string name{std::format("String_{:04}", i)};
				statusMetas.push_back({.mName = name});
			}

			std::string name{std::format("String_{:04}", MAX_STATUSES + 1)};

			statusMetas.push_back({.mName = name});
			std::expected<void, RegistryErrorInfo> result{config.addStatuses(statusMetas)};

			THEN("registration reports a max capacity and nothing is added")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::MaxCapacity));
				CHECK((config.getAmountRegistered() == finalStatusUnderlyingValue));
			}
		}

		WHEN("an status whose name is already in use is added")
		{
			std::vector<StatusMeta> statusMetas{};

			for (us i{0}; i < 20; ++i)
			{
				std::string name{std::format("String_{:04}", i)};
				statusMetas.push_back({.mName = name});
			}

			statusMetas.push_back({.mName = std::string(STATUS_NAME_BURN)});
			std::expected<void, RegistryErrorInfo> result{config.addStatuses(statusMetas)};

			THEN("registration reports a duplicate status and the registry is rollback to the checkpoint before the erroneous addition")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::DuplicateStatus));
				CHECK((config.getAmountRegistered() == finalStatusUnderlyingValue));
			}
		}

		WHEN("a unique status definition is added")
		{
			std::vector<StatusMeta> statusMetas{};

			statusMetas.push_back({.mName = "TestStatusName"});

			std::expected<void, RegistryErrorInfo> result{config.addStatuses(statusMetas)};

			THEN("the registry reports no error and all the status definitions are added")
			{
				REQUIRE(result.has_value());
				CHECK((config.getAmountRegistered() == finalStatusUnderlyingValue + 1));
			}
		}
	}

	GIVEN("renameStatus")
	{
		WHEN("calling with an invalid status name")
		{
			std::expected<void, RegistryErrorInfo> result{config.renameStatus("ThisIsInvalid", "NewName")};

			THEN("the registry reports an error and there is no update to the registry")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::StatusNotFound));
				CHECK((config.getAmountRegistered() == finalStatusUnderlyingValue));
			}
		}

		WHEN("trying to rename to a name already in the registry")
		{
			std::expected<void, RegistryErrorInfo> result{config.renameStatus(STATUS_NAME_NONE, STATUS_NAME_BURN)};

			THEN("the registry reports an error and there is no update to the registry")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::DuplicateStatus));
				CHECK((config.getAmountRegistered() == finalStatusUnderlyingValue));
			}
		}

		WHEN("updating an existing status definition")
		{
			std::expected<void, RegistryErrorInfo> result{config.renameStatus(STATUS_NAME_NONE, "NewName")};

			THEN("the registry reports no error and the name is appropriately updated")
			{
				REQUIRE(result.has_value());
				CHECK((config.getAmountRegistered() == finalStatusUnderlyingValue));
				CHECK((config.getStatusMetadata(toStatusID(BuiltinStatusID::None))->mName == "NewName"));
			}
		}
	}

	GIVEN("updateStatus")
	{
		StatusMeta definition{.mName = "TestStatusName"};

		WHEN("calling the string_view overload")
		{
			WHEN("calling with an invalid status name")
			{
				std::expected<void, RegistryErrorInfo> result{config.updateStatus("ThisIsInvalid", definition)};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::StatusNotFound));
					CHECK((config.getAmountRegistered() == finalStatusUnderlyingValue));
				}
			}

			WHEN("trying to rename to a name already in the registry")
			{
				std::expected<void, RegistryErrorInfo> result{
					config.updateStatus(STATUS_NAME_NONE, {.mName = std::string(STATUS_NAME_BURN)}),
				};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::DuplicateStatus));
					CHECK((config.getAmountRegistered() == finalStatusUnderlyingValue));
				}
			}

			WHEN("updating an existing status definition")
			{
				std::expected<void, RegistryErrorInfo> result{config.updateStatus(STATUS_NAME_NONE, definition)};

				THEN("the registry reports no error and the target is appropriately updated")
				{
					REQUIRE(result.has_value());
					CHECK((config.getAmountRegistered() == finalStatusUnderlyingValue));
					CHECK((config.getStatusMetadata(toStatusID(BuiltinStatusID::None))->mName == "TestStatusName"));
				}
			}
		}

		WHEN("calling the StatusID overload")
		{
			WHEN("calling with an invalid status name")
			{
				std::expected<void, RegistryErrorInfo> result{config.updateStatus(StatusID{200}, definition)};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::StatusNotFound));
					CHECK((config.getAmountRegistered() == finalStatusUnderlyingValue));
				}
			}

			WHEN("trying to rename to a name already in the registry")
			{
				std::expected<void, RegistryErrorInfo> result{
					config.updateStatus(toStatusID(BuiltinStatusID::None), {.mName = std::string(STATUS_NAME_BURN)}),
				};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::DuplicateStatus));
					CHECK((config.getAmountRegistered() == finalStatusUnderlyingValue));
				}
			}

			WHEN("updating an existing status definition")
			{
				std::expected<void, RegistryErrorInfo> result{config.updateStatus(toStatusID(BuiltinStatusID::None), definition)};

				THEN("the registry reports no error and the target is appropriately updated")
				{
					REQUIRE(result.has_value());
					CHECK((config.getAmountRegistered() == finalStatusUnderlyingValue));
					CHECK((config.getStatusMetadata(toStatusID(BuiltinStatusID::None))->mName == "TestStatusName"));
				}
			}
		}
	}

	GIVEN("removeStatus")
	{
		WHEN("calling the string_view overload")
		{
			WHEN("calling with an unknown status name")
			{
				std::expected<StatusID, RegistryErrorInfo> result{config.removeStatus("Unknown")};

				THEN("the result is an error and the registry is not updated")
				{
					CHECK_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::StatusNotFound));
					CHECK((config.getAmountRegistered() == finalStatusUnderlyingValue));
				}
			}

			WHEN("calling with a known status name")
			{
				std::expected<StatusID, RegistryErrorInfo> result{config.removeStatus(STATUS_NAME_NONE)};

				THEN("the result is a success and the registry is updated")
				{
					CHECK(result.has_value());
					CHECK((result.value() == toStatusID(BuiltinStatusID::None)));
					CHECK((config.getAmountRegistered() == finalStatusUnderlyingValue - 1));
				}
			}
		}

		WHEN("calling the StatusID overload")
		{
			WHEN("calling with an unknown status ID")
			{
				std::expected<StatusID, RegistryErrorInfo> result{config.removeStatus(StatusID{200})};

				THEN("the result is an error and the registry is not updated")
				{
					CHECK_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::StatusNotFound));
					CHECK((config.getAmountRegistered() == finalStatusUnderlyingValue));
				}
			}

			WHEN("calling with a known status ID")
			{
				std::expected<StatusID, RegistryErrorInfo> result{config.removeStatus(toStatusID(BuiltinStatusID::None))};

				THEN("the result is a success and the registry is updated")
				{
					CHECK(result.has_value());
					CHECK((result.value() == toStatusID(BuiltinStatusID::None)));
					CHECK((config.getAmountRegistered() == finalStatusUnderlyingValue - 1));
				}
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
