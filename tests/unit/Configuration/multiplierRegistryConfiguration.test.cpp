/*! @file multiplierRegistryConfiguration.test.cpp
	@brief C++ file for running tests for the MultiplierRegistryConfiguration.
	@date 09/10/2026
	@since 0.8.7
	@version 0.12.20
	@author Matthew Moore
*/

#include "Configuration/multiplierRegistryConfiguration.h"

#include <expected>
#include <format>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "Configuration/constants.h"
#include "Core/typedefs.h"
#include "Multiplier/builtInMultiplierID.h"
#include "Multiplier/constants.h"
#include "Multiplier/multiplierID.h"
#include "Multiplier/multiplierMeta.h"
#include "Registry/multiplierRegistry.h"
#include "Registry/registryError.h"
#include "Utility/Debug/Logging/logging.testHelper.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Configuration::MAX_MULTIPLIERS;
using PocketCore::Configuration::MultiplierRegistryConfiguration;
using PocketCore::Configuration::RegistryError;
using PocketCore::Core::ub;
using PocketCore::Core::us;
using PocketCore::Multiplier::BuiltinMultiplierID;
using PocketCore::Multiplier::MULTIPLIER_NAME_CRITICAL;
using PocketCore::Multiplier::MULTIPLIER_NAME_NONE;
using PocketCore::Multiplier::MultiplierID;
using PocketCore::Multiplier::MultiplierMeta;
using PocketCore::Multiplier::NO_MULTIPLIER_ID;
using PocketCore::Multiplier::toMultiplierID;
using PocketCore::Registry::Multiplier::MultiplierRegistry;
using PocketCore::Registry::RegistryErrorInfo;
using PocketCore::Testing::ensureLoggerInitialized;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("MultiplierRegistryConfiguration")
{
	ensureLoggerInitialized("multiplier registry configuration test", "multiplierRegistryConfiguration_test.log");

	MultiplierRegistryConfiguration config{};
	MultiplierRegistry registry{};
	ub finalMultiplierUnderlyingValue{std::to_underlying(BuiltinMultiplierID::FinalMultiplier)};

	GIVEN("getAmountRegistered")
	{
		THEN("the default registry returns the expected amount of built-in multipliers")
		{
			CHECK((config.getAmountRegistered() == finalMultiplierUnderlyingValue));
		}
	}

	GIVEN("getRuntimeRegistry")
	{
		THEN("the runtime registry is the same as the default registry")
		{
			CHECK((config.getRuntimeRegistry() == registry));
		}
	}

	GIVEN("getMultiplierMetadata")
	{
		THEN("unknown IDs are absent")
		{
			CHECK((config.getMultiplierMetadata(MultiplierID{200}) == nullptr));
		}

		THEN("the metadata is retrieved when accessed by a valid Multiplier ID")
		{
			MultiplierMeta expected{
				.mName = std::string(MULTIPLIER_NAME_NONE),
				.mMultiplierID = toMultiplierID(BuiltinMultiplierID::None),
			};

			CHECK((expected == *config.getMultiplierMetadata(NO_MULTIPLIER_ID)));
		}
	}

	GIVEN("getMultiplierID")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(config.getMultiplierID("Unknown").has_value());
		}

		THEN("the Multiplier ID is retrieved by valid Multiplier name")
		{
			std::optional<MultiplierID> multiplierID{config.getMultiplierID(MULTIPLIER_NAME_NONE)};

			REQUIRE(multiplierID.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((multiplierID.value() == toMultiplierID(BuiltinMultiplierID::None)));
		}
	}

	GIVEN("getMultiplierName")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(config.getMultiplierName(MultiplierID{200}).has_value());
		}

		THEN("a registered multiplier name is returned by stable ID")
		{
			std::optional<std::string_view> multiplierName{config.getMultiplierName(toMultiplierID(BuiltinMultiplierID::None))};

			REQUIRE(multiplierName.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((multiplierName.value() == MULTIPLIER_NAME_NONE));
		}
	}

	GIVEN("getRegisteredMultipliers")
	{
		THEN("the amount of multipliers returned matches the amount that are built-in")
		{
			CHECK((config.getRegisteredMultipliers().size() == finalMultiplierUnderlyingValue));
		}
	}

	GIVEN("hasMultiplier")
	{
		WHEN("calling the string_view overload")
		{
			THEN("an unknown multiplier name has no entry")
			{
				CHECK_FALSE(config.hasMultiplier("Unknown"));
			}

			THEN("a known multiplier name has an entry")
			{
				CHECK(config.hasMultiplier(MULTIPLIER_NAME_NONE));
			}
		}

		WHEN("calling the MultiplierID overload")
		{
			THEN("an unknown multiplier ID has no entry")
			{
				CHECK_FALSE(config.hasMultiplier(MultiplierID{200}));
			}

			THEN("a known multiplier ID has an entry")
			{
				CHECK(config.hasMultiplier(NO_MULTIPLIER_ID));
			}
		}
	}

	GIVEN("addMultiplier")
	{
		WHEN("trying to add an multiplier past the capacity")
		{
			us newMultiplierCount{finalMultiplierUnderlyingValue};

			for (us i{0}; i < MAX_MULTIPLIERS - finalMultiplierUnderlyingValue; ++i)
			{
				std::string name{std::format("String_{:04}", i)};
				MultiplierMeta definition{.mName = name};
				std::expected<MultiplierID, RegistryErrorInfo> result{config.addMultiplier(definition)};

				REQUIRE(result.has_value());

				MultiplierID assignedID{result.value()};
				CHECK((assignedID.getValue() == newMultiplierCount++));
			}

			std::string name{std::format("String_{:04}", MAX_MULTIPLIERS + 1)};

			MultiplierMeta definition{.mName = name};
			std::expected<MultiplierID, RegistryErrorInfo> result{config.addMultiplier(definition)};

			THEN("registration reports a max capacity and nothing is added")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::MaxCapacity));
				CHECK((config.getAmountRegistered() == MAX_MULTIPLIERS));
			}
		}

		WHEN("an multiplier whose name is already in use is added")
		{
			MultiplierMeta definition{.mName = std::string(MULTIPLIER_NAME_CRITICAL)};
			std::expected<MultiplierID, RegistryErrorInfo> result{config.addMultiplier(definition)};

			THEN("registration reports a duplicate multiplier and nothing is added")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::DuplicateMultiplier));
				CHECK((config.getAmountRegistered() == finalMultiplierUnderlyingValue));
			}
		}

		WHEN("a unique multiplier definition is added")
		{
			MultiplierMeta definition{.mName = "TestMultiplierName"};

			std::expected<MultiplierID, RegistryErrorInfo> result{config.addMultiplier(definition)};

			THEN("it receives the first custom stable ID and owns its trigger data")
			{
				REQUIRE(result.has_value());
				MultiplierID assignedID{result.value()};
				CHECK((assignedID.getValue() == finalMultiplierUnderlyingValue));

				const MultiplierMeta *metadata{config.getMultiplierMetadata(assignedID)};

				REQUIRE((metadata != nullptr));
				CHECK((metadata->mName == "TestMultiplierName"));
			}
		}
	}

	GIVEN("addMultipliers")
	{
		WHEN("trying to add an multiplier past the capacity")
		{
			std::vector<MultiplierMeta> multiplierMetas;

			for (us i{0}; i < MAX_MULTIPLIERS - finalMultiplierUnderlyingValue; ++i)
			{
				std::string name{std::format("String_{:04}", i)};
				multiplierMetas.push_back({.mName = name});
			}

			std::string name{std::format("String_{:04}", MAX_MULTIPLIERS + 1)};

			multiplierMetas.push_back({.mName = name});
			std::expected<void, RegistryErrorInfo> result{config.addMultipliers(multiplierMetas)};

			THEN("registration reports a max capacity and nothing is added")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::MaxCapacity));
				CHECK((config.getAmountRegistered() == finalMultiplierUnderlyingValue));
			}
		}

		WHEN("an multiplier whose name is already in use is added")
		{
			std::vector<MultiplierMeta> multiplierMetas{};

			for (us i{0}; i < 20; ++i)
			{
				std::string name{std::format("String_{:04}", i)};
				multiplierMetas.push_back({.mName = name});
			}

			multiplierMetas.push_back({.mName = std::string(MULTIPLIER_NAME_CRITICAL)});
			std::expected<void, RegistryErrorInfo> result{config.addMultipliers(multiplierMetas)};

			THEN("registration reports a duplicate multiplier and the registry is rollback to the checkpoint before the erroneous addition")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::DuplicateMultiplier));
				CHECK((config.getAmountRegistered() == finalMultiplierUnderlyingValue));
			}
		}

		WHEN("a unique multiplier definition is added")
		{
			std::vector<MultiplierMeta> multiplierMetas{};

			multiplierMetas.push_back({.mName = "TestMultiplierName"});

			std::expected<void, RegistryErrorInfo> result{config.addMultipliers(multiplierMetas)};

			THEN("the registry reports no error and all the multiplier definitions are added")
			{
				REQUIRE(result.has_value());
				CHECK((config.getAmountRegistered() == finalMultiplierUnderlyingValue + 1));
			}
		}
	}

	GIVEN("renameMultiplier")
	{
		WHEN("calling with an invalid multiplier name")
		{
			std::expected<void, RegistryErrorInfo> result{config.renameMultiplier("ThisIsInvalid", "NewName")};

			THEN("the registry reports an error and there is no update to the registry")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::MultiplierNotFound));
				CHECK((config.getAmountRegistered() == finalMultiplierUnderlyingValue));
			}
		}

		WHEN("trying to rename to a name already in the registry")
		{
			std::expected<void, RegistryErrorInfo> result{
				config.renameMultiplier(MULTIPLIER_NAME_NONE, MULTIPLIER_NAME_CRITICAL),
			};

			THEN("the registry reports an error and there is no update to the registry")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::DuplicateMultiplier));
				CHECK((config.getAmountRegistered() == finalMultiplierUnderlyingValue));
			}
		}

		WHEN("updating an existing multiplier definition")
		{
			std::expected<void, RegistryErrorInfo> result{config.renameMultiplier(MULTIPLIER_NAME_NONE, "NewName")};

			THEN("the registry reports no error and the name is appropriately updated")
			{
				REQUIRE(result.has_value());
				CHECK((config.getAmountRegistered() == finalMultiplierUnderlyingValue));
				CHECK((config.getMultiplierMetadata(toMultiplierID(BuiltinMultiplierID::None))->mName == "NewName"));
			}
		}
	}

	GIVEN("updateMultiplier")
	{
		MultiplierMeta definition{.mName = "TestMultiplierName"};

		WHEN("calling the string_view overload")
		{
			WHEN("calling with an invalid multiplier name")
			{
				std::expected<void, RegistryErrorInfo> result{config.updateMultiplier("ThisIsInvalid", definition)};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::MultiplierNotFound));
					CHECK((config.getAmountRegistered() == finalMultiplierUnderlyingValue));
				}
			}

			WHEN("trying to rename to a name already in the registry")
			{
				std::expected<void, RegistryErrorInfo> result{
					config.updateMultiplier(MULTIPLIER_NAME_NONE, {.mName = std::string(MULTIPLIER_NAME_CRITICAL)}),
				};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::DuplicateMultiplier));
					CHECK((config.getAmountRegistered() == finalMultiplierUnderlyingValue));
				}
			}

			WHEN("updating an existing multiplier definition")
			{
				std::expected<void, RegistryErrorInfo> result{config.updateMultiplier(MULTIPLIER_NAME_NONE, definition)};

				THEN("the registry reports no error and the target is appropriately updated")
				{
					REQUIRE(result.has_value());
					CHECK((config.getAmountRegistered() == finalMultiplierUnderlyingValue));
					CHECK((config.getMultiplierMetadata(toMultiplierID(BuiltinMultiplierID::None))->mName == "TestMultiplierName"));
				}
			}
		}

		WHEN("calling the MultiplierID overload")
		{
			WHEN("calling with an invalid multiplier name")
			{
				std::expected<void, RegistryErrorInfo> result{config.updateMultiplier(MultiplierID{200}, definition)};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::MultiplierNotFound));
					CHECK((config.getAmountRegistered() == finalMultiplierUnderlyingValue));
				}
			}

			WHEN("trying to rename to a name already in the registry")
			{
				std::expected<void, RegistryErrorInfo> result{
					config.updateMultiplier(toMultiplierID(BuiltinMultiplierID::None), {.mName = std::string(MULTIPLIER_NAME_CRITICAL)}),
				};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::DuplicateMultiplier));
					CHECK((config.getAmountRegistered() == finalMultiplierUnderlyingValue));
				}
			}

			WHEN("updating an existing multiplier definition")
			{
				std::expected<void, RegistryErrorInfo> result{
					config.updateMultiplier(toMultiplierID(BuiltinMultiplierID::None), definition),
				};

				THEN("the registry reports no error and the target is appropriately updated")
				{
					REQUIRE(result.has_value());
					CHECK((config.getAmountRegistered() == finalMultiplierUnderlyingValue));
					CHECK((config.getMultiplierMetadata(toMultiplierID(BuiltinMultiplierID::None))->mName == "TestMultiplierName"));
				}
			}
		}
	}

	GIVEN("removeMultiplier")
	{
		WHEN("calling the string_view overload")
		{
			WHEN("calling with an unknown multiplier name")
			{
				std::expected<MultiplierID, RegistryErrorInfo> result{config.removeMultiplier("Unknown")};

				THEN("the result is an error and the registry is not updated")
				{
					CHECK_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::MultiplierNotFound));
					CHECK((config.getAmountRegistered() == finalMultiplierUnderlyingValue));
				}
			}

			WHEN("calling with a known multiplier name")
			{
				std::expected<MultiplierID, RegistryErrorInfo> result{config.removeMultiplier(MULTIPLIER_NAME_NONE)};

				THEN("the result is a success and the registry is updated")
				{
					CHECK(result.has_value());
					CHECK((result.value() == toMultiplierID(BuiltinMultiplierID::None)));
					CHECK((config.getAmountRegistered() == finalMultiplierUnderlyingValue - 1));
				}
			}
		}

		WHEN("calling the MultiplierID overload")
		{
			WHEN("calling with an unknown multiplier ID")
			{
				std::expected<MultiplierID, RegistryErrorInfo> result{config.removeMultiplier(MultiplierID{200})};

				THEN("the result is an error and the registry is not updated")
				{
					CHECK_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::MultiplierNotFound));
					CHECK((config.getAmountRegistered() == finalMultiplierUnderlyingValue));
				}
			}

			WHEN("calling with a known multiplier ID")
			{
				std::expected<MultiplierID, RegistryErrorInfo> result{config.removeMultiplier(toMultiplierID(BuiltinMultiplierID::None))};

				THEN("the result is a success and the registry is updated")
				{
					CHECK(result.has_value());
					CHECK((result.value() == toMultiplierID(BuiltinMultiplierID::None)));
					CHECK((config.getAmountRegistered() == finalMultiplierUnderlyingValue - 1));
				}
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
