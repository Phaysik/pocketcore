/*! @file eggGroupRegistryConfiguration.test.cpp
	@brief C++ file for running tests for the EggGroupRegistryConfiguration.
	@date 10/26/2026
	@since 0.12.53
	@version 0.12.53
	@author Matthew Moore
*/

#include "Configuration/eggGroupRegistryConfiguration.h"

#include <expected>
#include <format>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "Configuration/constants.h"
#include "Core/typedefs.h"
#include "EggGroup/builtInEggGroupID.h"
#include "EggGroup/constants.h"
#include "EggGroup/eggGroupID.h"
#include "EggGroup/eggGroupMeta.h"
#include "Registry/eggGroupRegistry.h"
#include "Utility/Debug/Logging/logging.testHelper.h"
#include "Validation/Registry/registryError.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Configuration::EggGroupRegistryConfiguration;
using PocketCore::Configuration::MAX_EGG_GROUPS;
using PocketCore::Configuration::RegistryError;
using PocketCore::Core::ub;
using PocketCore::Core::us;
using PocketCore::EggGroup::BuiltinEggGroupID;
using PocketCore::EggGroup::EGG_GROUP_NAME_NONE;
using PocketCore::EggGroup::EGG_GROUP_NAME_WATER1;
using PocketCore::EggGroup::EggGroupID;
using PocketCore::EggGroup::EggGroupMeta;
using PocketCore::EggGroup::NO_EGG_GROUP_ID;
using PocketCore::EggGroup::toEggGroupID;
using PocketCore::Registry::EggGroup::EggGroupRegistry;
using PocketCore::Testing::ensureLoggerInitialized;
using PocketCore::Validation::Registry::RegistryError;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("EggGroupRegistryConfiguration")
{
	ensureLoggerInitialized("eggGroup registry configuration test", "eggGroupRegistryConfiguration_test.log");

	std::unique_ptr<EggGroupRegistryConfiguration> configRegistryStorage{std::make_unique<EggGroupRegistryConfiguration>()};
	EggGroupRegistryConfiguration &config{*configRegistryStorage};
	std::unique_ptr<EggGroupRegistry> registryStorage{std::make_unique<EggGroupRegistry>()};
	EggGroupRegistry &registry{*registryStorage};
	ub finalEggGroupUnderlyingValue{std::to_underlying(BuiltinEggGroupID::FinalEggGroup)};

	GIVEN("getAmountRegistered")
	{
		THEN("the default registry returns the expected amount of built-in eggGroups")
		{
			CHECK((config.getAmountRegistered() == finalEggGroupUnderlyingValue));
		}
	}

	GIVEN("getRuntimeRegistry")
	{
		THEN("the runtime registry is the same as the default registry")
		{
			CHECK((config.getRuntimeRegistry() == registry));
		}
	}

	GIVEN("getEggGroupMetadata")
	{
		THEN("unknown IDs are absent")
		{
			CHECK((config.getEggGroupMetadata(EggGroupID{200}) == nullptr));
		}

		THEN("the metadata is retrieved when accessed by a valid EggGroup ID")
		{
			EggGroupMeta expected{
				.mName = std::string(EGG_GROUP_NAME_NONE),
				.mEggGroupID = toEggGroupID(BuiltinEggGroupID::None),
			};

			CHECK((expected == *config.getEggGroupMetadata(NO_EGG_GROUP_ID)));
		}
	}

	GIVEN("getEggGroupID")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(config.getEggGroupID("Unknown").has_value());
		}

		THEN("the EggGroup ID is retrieved by valid EggGroup name")
		{
			std::optional<EggGroupID> eggGroupID{config.getEggGroupID(EGG_GROUP_NAME_NONE)};

			REQUIRE(eggGroupID.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((eggGroupID.value() == toEggGroupID(BuiltinEggGroupID::None)));
		}
	}

	GIVEN("getEggGroupName")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(config.getEggGroupName(EggGroupID{200}).has_value());
		}

		THEN("a registered eggGroup name is returned by stable ID")
		{
			std::optional<std::string_view> eggGroupName{config.getEggGroupName(toEggGroupID(BuiltinEggGroupID::None))};

			REQUIRE(eggGroupName.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((eggGroupName.value() == EGG_GROUP_NAME_NONE));
		}
	}

	GIVEN("getRegisteredEggGroups")
	{
		THEN("the amount of eggGroups returned matches the amount that are built-in")
		{
			CHECK((config.getRegisteredEggGroups().size() == finalEggGroupUnderlyingValue));
		}
	}

	GIVEN("hasEggGroup")
	{
		WHEN("calling the string_view overload")
		{
			THEN("an unknown eggGroup name has no entry")
			{
				CHECK_FALSE(config.hasEggGroup("Unknown"));
			}

			THEN("a known eggGroup name has an entry")
			{
				CHECK(config.hasEggGroup(EGG_GROUP_NAME_NONE));
			}
		}

		WHEN("calling the EggGroupID overload")
		{
			THEN("an unknown eggGroup ID has no entry")
			{
				CHECK_FALSE(config.hasEggGroup(EggGroupID{200}));
			}

			THEN("a known eggGroup ID has an entry")
			{
				CHECK(config.hasEggGroup(NO_EGG_GROUP_ID));
			}
		}
	}

	GIVEN("addEggGroup")
	{
		WHEN("trying to add an eggGroup past the capacity")
		{
			us newEggGroupCount{finalEggGroupUnderlyingValue};

			for (us i{0}; i < MAX_EGG_GROUPS - finalEggGroupUnderlyingValue; ++i)
			{
				std::string name{std::format("String_{:04}", i)};
				EggGroupMeta definition{.mName = name};
				std::expected<EggGroupID, RegistryError> result{config.addEggGroup(definition)};

				REQUIRE(result.has_value());

				EggGroupID assignedID{result.value()};
				CHECK((assignedID.getValue() == newEggGroupCount++));
			}

			std::string name{std::format("String_{:04}", MAX_EGG_GROUPS + 1)};

			EggGroupMeta definition{.mName = name};
			std::expected<EggGroupID, RegistryError> result{config.addEggGroup(definition)};

			THEN("registration reports a max capacity and nothing is added")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error() == RegistryError::MaxCapacity));
				CHECK((config.getAmountRegistered() == MAX_EGG_GROUPS));
			}
		}

		WHEN("an eggGroup whose name is already in use is added")
		{
			EggGroupMeta definition{.mName = std::string(EGG_GROUP_NAME_WATER1)};
			std::expected<EggGroupID, RegistryError> result{config.addEggGroup(definition)};

			THEN("registration reports a duplicate eggGroup and nothing is added")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error() == RegistryError::DuplicateEggGroup));
				CHECK((config.getAmountRegistered() == finalEggGroupUnderlyingValue));
			}
		}

		WHEN("a unique eggGroup definition is added")
		{
			EggGroupMeta definition{.mName = "TestEggGroupName"};

			std::expected<EggGroupID, RegistryError> result{config.addEggGroup(definition)};

			THEN("it receives the first custom stable ID and owns its trigger data")
			{
				REQUIRE(result.has_value());
				EggGroupID assignedID{result.value()};
				CHECK((assignedID.getValue() == finalEggGroupUnderlyingValue));

				const EggGroupMeta *metadata{config.getEggGroupMetadata(assignedID)};

				REQUIRE((metadata != nullptr));
				definition.mEggGroupID = assignedID;
				CHECK((*metadata == definition));
			}
		}
	}

	GIVEN("addEggGroups")
	{
		WHEN("trying to add an eggGroup past the capacity")
		{
			std::vector<EggGroupMeta> eggGroupMetas;

			for (us i{0}; i < MAX_EGG_GROUPS - finalEggGroupUnderlyingValue; ++i)
			{
				std::string name{std::format("String_{:04}", i)};
				eggGroupMetas.push_back({.mName = name});
			}

			std::string name{std::format("String_{:04}", MAX_EGG_GROUPS + 1)};

			eggGroupMetas.push_back({.mName = name});
			std::expected<void, RegistryError> result{config.addEggGroups(eggGroupMetas)};

			THEN("registration reports a max capacity and nothing is added")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error() == RegistryError::MaxCapacity));
				CHECK((config.getAmountRegistered() == finalEggGroupUnderlyingValue));
			}
		}

		WHEN("a eggGroup whose name is already in use is added")
		{
			std::vector<EggGroupMeta> eggGroupMetas{};

			for (us i{0}; i < 20; ++i)
			{
				std::string name{std::format("String_{:04}", i)};
				eggGroupMetas.push_back({.mName = name});
			}

			eggGroupMetas.push_back({.mName = std::string(EGG_GROUP_NAME_WATER1)});
			const EggGroupRegistry beforeBatch{config.getRuntimeRegistry()};
			std::expected<void, RegistryError> result{config.addEggGroups(eggGroupMetas)};

			THEN("registration reports a duplicate eggGroup and the registry is rollback to the checkpoint before the erroneous addition")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error() == RegistryError::DuplicateEggGroup));
				CHECK((config.getAmountRegistered() == finalEggGroupUnderlyingValue));
				CHECK((config.getRuntimeRegistry() == beforeBatch));
				CHECK((config.getRuntimeRegistry().getNextEggGroupID() == beforeBatch.getNextEggGroupID()));
				CHECK_FALSE(config.hasEggGroup("String_0000"));
			}
		}

		WHEN("a unique eggGroup definition is added")
		{
			std::vector<EggGroupMeta> eggGroupMetas{};

			eggGroupMetas.push_back({.mName = "TestEggGroupName"});

			std::expected<void, RegistryError> result{config.addEggGroups(eggGroupMetas)};

			THEN("the registry reports no error and all the eggGroup definitions are added")
			{
				REQUIRE(result.has_value());
				CHECK((config.getAmountRegistered() == finalEggGroupUnderlyingValue + 1));
			}
		}
	}

	GIVEN("renameEggGroup")
	{
		WHEN("calling with an invalid eggGroup name")
		{
			std::expected<void, RegistryError> result{config.renameEggGroup("ThisIsInvalid", "NewName")};

			THEN("the registry reports an error and there is no update to the registry")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error() == RegistryError::EggGroupNotFound));
				CHECK((config.getAmountRegistered() == finalEggGroupUnderlyingValue));
			}
		}

		WHEN("trying to rename to a name already in the registry")
		{
			std::expected<void, RegistryError> result{
				config.renameEggGroup(EGG_GROUP_NAME_NONE, EGG_GROUP_NAME_WATER1),
			};

			THEN("the registry reports an error and there is no update to the registry")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error() == RegistryError::DuplicateEggGroup));
				CHECK((config.getAmountRegistered() == finalEggGroupUnderlyingValue));
			}
		}

		WHEN("updating an existing eggGroup definition")
		{
			std::expected<void, RegistryError> result{config.renameEggGroup(EGG_GROUP_NAME_NONE, "NewName")};

			THEN("the registry reports no error and the name is appropriately updated")
			{
				REQUIRE(result.has_value());
				CHECK((config.getAmountRegistered() == finalEggGroupUnderlyingValue));
				CHECK((config.getEggGroupMetadata(toEggGroupID(BuiltinEggGroupID::None))->mName == "NewName"));
			}
		}
	}

	GIVEN("updateEggGroup")
	{
		EggGroupMeta definition{.mName = "TestEggGroupName"};

		WHEN("calling the string_view overload")
		{
			WHEN("calling with an invalid eggGroup name")
			{
				std::expected<void, RegistryError> result{config.updateEggGroup("ThisIsInvalid", definition)};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error() == RegistryError::EggGroupNotFound));
					CHECK((config.getAmountRegistered() == finalEggGroupUnderlyingValue));
				}
			}

			WHEN("trying to rename to a name already in the registry")
			{
				std::expected<void, RegistryError> result{
					config.updateEggGroup(EGG_GROUP_NAME_NONE, {.mName = std::string(EGG_GROUP_NAME_WATER1)}),
				};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error() == RegistryError::DuplicateEggGroup));
					CHECK((config.getAmountRegistered() == finalEggGroupUnderlyingValue));
				}
			}

			WHEN("updating an existing eggGroup definition")
			{
				std::expected<void, RegistryError> result{config.updateEggGroup(EGG_GROUP_NAME_NONE, definition)};

				THEN("the registry reports no error and the target is appropriately updated")
				{
					REQUIRE(result.has_value());
					CHECK((config.getAmountRegistered() == finalEggGroupUnderlyingValue));
					CHECK((config.getEggGroupMetadata(toEggGroupID(BuiltinEggGroupID::None))->mName == "TestEggGroupName"));
				}
			}
		}

		WHEN("calling the EggGroupID overload")
		{
			WHEN("calling with an invalid eggGroup name")
			{
				std::expected<void, RegistryError> result{config.updateEggGroup(EggGroupID{200}, definition)};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error() == RegistryError::EggGroupNotFound));
					CHECK((config.getAmountRegistered() == finalEggGroupUnderlyingValue));
				}
			}

			WHEN("trying to rename to a name already in the registry")
			{
				std::expected<void, RegistryError> result{
					config.updateEggGroup(toEggGroupID(BuiltinEggGroupID::None), {.mName = std::string(EGG_GROUP_NAME_WATER1)}),
				};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error() == RegistryError::DuplicateEggGroup));
					CHECK((config.getAmountRegistered() == finalEggGroupUnderlyingValue));
				}
			}

			WHEN("updating an existing eggGroup definition")
			{
				std::expected<void, RegistryError> result{config.updateEggGroup(toEggGroupID(BuiltinEggGroupID::None), definition)};

				THEN("the registry reports no error and the target is appropriately updated")
				{
					REQUIRE(result.has_value());
					CHECK((config.getAmountRegistered() == finalEggGroupUnderlyingValue));
					CHECK((config.getEggGroupMetadata(toEggGroupID(BuiltinEggGroupID::None))->mName == "TestEggGroupName"));
				}
			}
		}
	}

	GIVEN("removeEggGroup")
	{
		WHEN("calling the string_view overload")
		{
			WHEN("calling with an unknown eggGroup name")
			{
				std::expected<EggGroupID, RegistryError> result{config.removeEggGroup("Unknown")};

				THEN("the result is an error and the registry is not updated")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error() == RegistryError::EggGroupNotFound));
					CHECK((config.getAmountRegistered() == finalEggGroupUnderlyingValue));
				}
			}

			WHEN("calling with a known eggGroup name")
			{
				std::expected<EggGroupID, RegistryError> result{config.removeEggGroup(EGG_GROUP_NAME_NONE)};

				THEN("the result is a success and the registry is updated")
				{
					REQUIRE(result.has_value());
					CHECK((result.value() == toEggGroupID(BuiltinEggGroupID::None)));
					CHECK((config.getAmountRegistered() == finalEggGroupUnderlyingValue - 1));
				}
			}
		}

		WHEN("calling the EggGroupID overload")
		{
			WHEN("calling with an unknown eggGroup ID")
			{
				std::expected<EggGroupID, RegistryError> result{config.removeEggGroup(EggGroupID{200})};

				THEN("the result is an error and the registry is not updated")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error() == RegistryError::EggGroupNotFound));
					CHECK((config.getAmountRegistered() == finalEggGroupUnderlyingValue));
				}
			}

			WHEN("calling with a known eggGroup ID")
			{
				std::expected<EggGroupID, RegistryError> result{config.removeEggGroup(toEggGroupID(BuiltinEggGroupID::None))};

				THEN("the result is a success and the registry is updated")
				{
					REQUIRE(result.has_value());
					CHECK((result.value() == toEggGroupID(BuiltinEggGroupID::None)));
					CHECK((config.getAmountRegistered() == finalEggGroupUnderlyingValue - 1));
				}
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
