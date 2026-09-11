/*! @file natureRegistryConfiguration.test.cpp
	@brief C++ file for running tests for the NatureRegistryConfiguration.
	@date 09/10/2026
	@since 0.12.20
	@version 0.12.20
	@author Matthew Moore
*/

#include "Configuration/natureRegistryConfiguration.h"

#include <expected>
#include <format>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "Configuration/constants.h"
#include "Core/typedefs.h"
#include "Nature/builtInNatureID.h"
#include "Nature/constants.h"
#include "Nature/natureID.h"
#include "Nature/natureMeta.h"
#include "Registry/natureRegistry.h"
#include "Registry/registryError.h"
#include "Utility/Debug/Logging/logging.testHelper.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Configuration::MAX_NATURES;
using PocketCore::Configuration::NatureRegistryConfiguration;
using PocketCore::Configuration::RegistryError;
using PocketCore::Core::ub;
using PocketCore::Core::us;
using PocketCore::Nature::BuiltinNatureID;
using PocketCore::Nature::NATURE_NAME_HASTY;
using PocketCore::Nature::NATURE_NAME_NONE;
using PocketCore::Nature::NatureID;
using PocketCore::Nature::NatureMeta;
using PocketCore::Nature::NO_NATURE_ID;
using PocketCore::Nature::toNatureID;
using PocketCore::Registry::Nature::NatureRegistry;
using PocketCore::Registry::RegistryErrorInfo;
using PocketCore::Testing::ensureLoggerInitialized;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("NatureRegistryConfiguration")
{
	ensureLoggerInitialized("nature registry configuration test", "natureRegistryConfiguration_test.log");

	NatureRegistryConfiguration config{};
	NatureRegistry registry{};
	ub finalNatureUnderlyingValue{std::to_underlying(BuiltinNatureID::FinalNature)};

	GIVEN("getAmountRegistered")
	{
		THEN("the default registry returns the expected amount of built-in natures")
		{
			CHECK((config.getAmountRegistered() == finalNatureUnderlyingValue));
		}
	}

	GIVEN("getRuntimeRegistry")
	{
		THEN("the runtime registry is the same as the default registry")
		{
			CHECK((config.getRuntimeRegistry() == registry));
		}
	}

	GIVEN("getNatureMetadata")
	{
		THEN("unknown IDs are absent")
		{
			CHECK((config.getNatureMetadata(NatureID{200}) == nullptr));
		}

		THEN("the metadata is retrieved when accessed by a valid Nature ID")
		{
			NatureMeta expected{
				.mName = std::string(NATURE_NAME_NONE),
				.mNatureID = toNatureID(BuiltinNatureID::None),
			};

			CHECK((expected == *config.getNatureMetadata(NO_NATURE_ID)));
		}
	}

	GIVEN("getNatureID")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(config.getNatureID("Unknown").has_value());
		}

		THEN("the Nature ID is retrieved by valid Nature name")
		{
			std::optional<NatureID> natureID{config.getNatureID(NATURE_NAME_NONE)};

			REQUIRE(natureID.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((natureID.value() == toNatureID(BuiltinNatureID::None)));
		}
	}

	GIVEN("getNatureName")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(config.getNatureName(NatureID{200}).has_value());
		}

		THEN("a registered nature name is returned by stable ID")
		{
			std::optional<std::string_view> natureName{config.getNatureName(toNatureID(BuiltinNatureID::None))};

			REQUIRE(natureName.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((natureName.value() == NATURE_NAME_NONE));
		}
	}

	GIVEN("getRegisteredNatures")
	{
		THEN("the amount of natures returned matches the amount that are built-in")
		{
			CHECK((config.getRegisteredNatures().size() == finalNatureUnderlyingValue));
		}
	}

	GIVEN("hasNature")
	{
		WHEN("calling the string_view overload")
		{
			THEN("an unknown nature name has no entry")
			{
				CHECK_FALSE(config.hasNature("Unknown"));
			}

			THEN("a known nature name has an entry")
			{
				CHECK(config.hasNature(NATURE_NAME_NONE));
			}
		}

		WHEN("calling the NatureID overload")
		{
			THEN("an unknown nature ID has no entry")
			{
				CHECK_FALSE(config.hasNature(NatureID{200}));
			}

			THEN("a known nature ID has an entry")
			{
				CHECK(config.hasNature(NO_NATURE_ID));
			}
		}
	}

	GIVEN("addNature")
	{
		WHEN("trying to add an nature past the capacity")
		{
			us newNatureCount{finalNatureUnderlyingValue};

			for (us i{0}; i < MAX_NATURES - finalNatureUnderlyingValue; ++i)
			{
				std::string name{std::format("String_{:04}", i)};
				NatureMeta definition{.mName = name};
				std::expected<NatureID, RegistryErrorInfo> result{config.addNature(definition)};

				REQUIRE(result.has_value());

				NatureID assignedID{result.value()};
				CHECK((assignedID.getValue() == newNatureCount++));
			}

			std::string name{std::format("String_{:04}", MAX_NATURES + 1)};

			NatureMeta definition{.mName = name};
			std::expected<NatureID, RegistryErrorInfo> result{config.addNature(definition)};

			THEN("registration reports a max capacity and nothing is added")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::MaxCapacity));
				CHECK((config.getAmountRegistered() == MAX_NATURES));
			}
		}

		WHEN("an nature whose name is already in use is added")
		{
			NatureMeta definition{.mName = std::string(NATURE_NAME_HASTY)};
			std::expected<NatureID, RegistryErrorInfo> result{config.addNature(definition)};

			THEN("registration reports a duplicate nature and nothing is added")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::DuplicateNature));
				CHECK((config.getAmountRegistered() == finalNatureUnderlyingValue));
			}
		}

		WHEN("a unique nature definition is added")
		{
			NatureMeta definition{.mName = "TestNatureName"};

			std::expected<NatureID, RegistryErrorInfo> result{config.addNature(definition)};

			THEN("it receives the first custom stable ID and owns its trigger data")
			{
				REQUIRE(result.has_value());
				NatureID assignedID{result.value()};
				CHECK((assignedID.getValue() == finalNatureUnderlyingValue));

				const NatureMeta *metadata{config.getNatureMetadata(assignedID)};

				REQUIRE((metadata != nullptr));
				CHECK((metadata->mName == "TestNatureName"));
			}
		}
	}

	GIVEN("addNatures")
	{
		WHEN("trying to add an nature past the capacity")
		{
			std::vector<NatureMeta> natureMetas;

			for (us i{0}; i < MAX_NATURES - finalNatureUnderlyingValue; ++i)
			{
				std::string name{std::format("String_{:04}", i)};
				natureMetas.push_back({.mName = name});
			}

			std::string name{std::format("String_{:04}", MAX_NATURES + 1)};

			natureMetas.push_back({.mName = name});
			std::expected<void, RegistryErrorInfo> result{config.addNatures(natureMetas)};

			THEN("registration reports a max capacity and nothing is added")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::MaxCapacity));
				CHECK((config.getAmountRegistered() == finalNatureUnderlyingValue));
			}
		}

		WHEN("an nature whose name is already in use is added")
		{
			std::vector<NatureMeta> natureMetas{};

			for (us i{0}; i < 20; ++i)
			{
				std::string name{std::format("String_{:04}", i)};
				natureMetas.push_back({.mName = name});
			}

			natureMetas.push_back({.mName = std::string(NATURE_NAME_HASTY)});
			std::expected<void, RegistryErrorInfo> result{config.addNatures(natureMetas)};

			THEN("registration reports a duplicate nature and the registry is rollback to the checkpoint before the erroneous addition")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::DuplicateNature));
				CHECK((config.getAmountRegistered() == finalNatureUnderlyingValue));
			}
		}

		WHEN("a unique nature definition is added")
		{
			std::vector<NatureMeta> natureMetas{};

			natureMetas.push_back({.mName = "TestNatureName"});

			std::expected<void, RegistryErrorInfo> result{config.addNatures(natureMetas)};

			THEN("the registry reports no error and all the nature definitions are added")
			{
				REQUIRE(result.has_value());
				CHECK((config.getAmountRegistered() == finalNatureUnderlyingValue + 1));
			}
		}
	}

	GIVEN("renameNature")
	{
		WHEN("calling with an invalid nature name")
		{
			std::expected<void, RegistryErrorInfo> result{config.renameNature("ThisIsInvalid", "NewName")};

			THEN("the registry reports an error and there is no update to the registry")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::NatureNotFound));
				CHECK((config.getAmountRegistered() == finalNatureUnderlyingValue));
			}
		}

		WHEN("trying to rename to a name already in the registry")
		{
			std::expected<void, RegistryErrorInfo> result{
				config.renameNature(NATURE_NAME_NONE, NATURE_NAME_HASTY),
			};

			THEN("the registry reports an error and there is no update to the registry")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::DuplicateNature));
				CHECK((config.getAmountRegistered() == finalNatureUnderlyingValue));
			}
		}

		WHEN("updating an existing nature definition")
		{
			std::expected<void, RegistryErrorInfo> result{config.renameNature(NATURE_NAME_NONE, "NewName")};

			THEN("the registry reports no error and the name is appropriately updated")
			{
				REQUIRE(result.has_value());
				CHECK((config.getAmountRegistered() == finalNatureUnderlyingValue));
				CHECK((config.getNatureMetadata(toNatureID(BuiltinNatureID::None))->mName == "NewName"));
			}
		}
	}

	GIVEN("updateNature")
	{
		NatureMeta definition{.mName = "TestNatureName"};

		WHEN("calling the string_view overload")
		{
			WHEN("calling with an invalid nature name")
			{
				std::expected<void, RegistryErrorInfo> result{config.updateNature("ThisIsInvalid", definition)};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::NatureNotFound));
					CHECK((config.getAmountRegistered() == finalNatureUnderlyingValue));
				}
			}

			WHEN("trying to rename to a name already in the registry")
			{
				std::expected<void, RegistryErrorInfo> result{
					config.updateNature(NATURE_NAME_NONE, {.mName = std::string(NATURE_NAME_HASTY)}),
				};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::DuplicateNature));
					CHECK((config.getAmountRegistered() == finalNatureUnderlyingValue));
				}
			}

			WHEN("updating an existing nature definition")
			{
				std::expected<void, RegistryErrorInfo> result{config.updateNature(NATURE_NAME_NONE, definition)};

				THEN("the registry reports no error and the target is appropriately updated")
				{
					REQUIRE(result.has_value());
					CHECK((config.getAmountRegistered() == finalNatureUnderlyingValue));
					CHECK((config.getNatureMetadata(toNatureID(BuiltinNatureID::None))->mName == "TestNatureName"));
				}
			}
		}

		WHEN("calling the NatureID overload")
		{
			WHEN("calling with an invalid nature name")
			{
				std::expected<void, RegistryErrorInfo> result{config.updateNature(NatureID{200}, definition)};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::NatureNotFound));
					CHECK((config.getAmountRegistered() == finalNatureUnderlyingValue));
				}
			}

			WHEN("trying to rename to a name already in the registry")
			{
				std::expected<void, RegistryErrorInfo> result{
					config.updateNature(toNatureID(BuiltinNatureID::None), {.mName = std::string(NATURE_NAME_HASTY)}),
				};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::DuplicateNature));
					CHECK((config.getAmountRegistered() == finalNatureUnderlyingValue));
				}
			}

			WHEN("updating an existing nature definition")
			{
				std::expected<void, RegistryErrorInfo> result{config.updateNature(toNatureID(BuiltinNatureID::None), definition)};

				THEN("the registry reports no error and the target is appropriately updated")
				{
					REQUIRE(result.has_value());
					CHECK((config.getAmountRegistered() == finalNatureUnderlyingValue));
					CHECK((config.getNatureMetadata(toNatureID(BuiltinNatureID::None))->mName == "TestNatureName"));
				}
			}
		}
	}

	GIVEN("removeNature")
	{
		WHEN("calling the string_view overload")
		{
			WHEN("calling with an unknown nature name")
			{
				std::expected<NatureID, RegistryErrorInfo> result{config.removeNature("Unknown")};

				THEN("the result is an error and the registry is not updated")
				{
					CHECK_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::NatureNotFound));
					CHECK((config.getAmountRegistered() == finalNatureUnderlyingValue));
				}
			}

			WHEN("calling with a known nature name")
			{
				std::expected<NatureID, RegistryErrorInfo> result{config.removeNature(NATURE_NAME_NONE)};

				THEN("the result is a success and the registry is updated")
				{
					CHECK(result.has_value());
					CHECK((result.value() == toNatureID(BuiltinNatureID::None)));
					CHECK((config.getAmountRegistered() == finalNatureUnderlyingValue - 1));
				}
			}
		}

		WHEN("calling the NatureID overload")
		{
			WHEN("calling with an unknown nature ID")
			{
				std::expected<NatureID, RegistryErrorInfo> result{config.removeNature(NatureID{200})};

				THEN("the result is an error and the registry is not updated")
				{
					CHECK_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::NatureNotFound));
					CHECK((config.getAmountRegistered() == finalNatureUnderlyingValue));
				}
			}

			WHEN("calling with a known nature ID")
			{
				std::expected<NatureID, RegistryErrorInfo> result{config.removeNature(toNatureID(BuiltinNatureID::None))};

				THEN("the result is a success and the registry is updated")
				{
					CHECK(result.has_value());
					CHECK((result.value() == toNatureID(BuiltinNatureID::None)));
					CHECK((config.getAmountRegistered() == finalNatureUnderlyingValue - 1));
				}
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
