/*! @file fixedMetadataRegistryConfiguration.test.cpp
	@brief C++ file for running tests for the FixedMetadataRegistryConfiguration.
	@date 10/09/2026
	@since 0.12.50
	@version 0.12.50
	@author Matthew Moore
*/

#include <expected>
#include <format>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "Configuration/constants.h"
#include "Configuration/fixedMetadataConfiguration.testHelper.h"
#include "Core/typedefs.h"
#include "Registry/fixedMetadataRegistry.testHelper.h"
#include "Registry/registryError.h"
#include "Utility/Debug/Logging/logging.testHelper.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Configuration::MAX_WEATHERS;
using PocketCore::Configuration::RegistryError;
using PocketCore::Core::ub;
using PocketCore::Core::us;
using PocketCore::Registry::RegistryErrorInfo;
using PocketCore::Testing::BuiltinFixedMetaDataID;
using PocketCore::Testing::ensureLoggerInitialized;
using PocketCore::Testing::FixedConfiguration;
using PocketCore::Testing::FixedMetaDataID;
using PocketCore::Testing::FixedRegistry;
using PocketCore::Testing::Metadata;
using PocketCore::Testing::NO_ID;
using PocketCore::Testing::NONE_NAME;
using PocketCore::Testing::TEST1_NAME;
using PocketCore::Testing::toFixedMetaDataID;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("FixedMetadataRegistryConfiguration")
{
	ensureLoggerInitialized("weather registry configuration test", "FixedConfiguration_test.log");

	FixedConfiguration config{};
	FixedRegistry registry{};
	ub finalWeatherUnderlyingValue{std::to_underlying(BuiltinFixedMetaDataID::Final)};

	GIVEN("getAmountRegistered")
	{
		THEN("the default registry returns the expected amount of built-in weathers")
		{
			CHECK((config.getAmountRegistered() == finalWeatherUnderlyingValue));
		}
	}

	GIVEN("getRegistry")
	{
		THEN("the runtime registry is the same as the default registry")
		{
			CHECK((config.getRegistry() == registry));
		}
	}

	GIVEN("getMetadata")
	{
		THEN("unknown IDs are absent")
		{
			CHECK((config.getMetadata(FixedMetaDataID{200}) == nullptr));
		}

		THEN("the metadata is retrieved when accessed by a valid Weather ID")
		{
			Metadata expected{
				.mName = std::string(NONE_NAME),
				.mID = toFixedMetaDataID(BuiltinFixedMetaDataID::None),
			};

			CHECK((expected == *config.getMetadata(NO_ID)));
		}
	}

	GIVEN("getID")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(config.getID("Unknown").has_value());
		}

		THEN("the Weather ID is retrieved by valid Weather name")
		{
			std::optional<FixedMetaDataID> FixedMetaDataID{config.getID(NONE_NAME)};

			REQUIRE(FixedMetaDataID.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((FixedMetaDataID.value() == toFixedMetaDataID(BuiltinFixedMetaDataID::None)));
		}
	}

	GIVEN("getName")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(config.getName(FixedMetaDataID{200}).has_value());
		}

		THEN("a registered weather name is returned by stable ID")
		{
			std::optional<std::string_view> weatherName{config.getName(toFixedMetaDataID(BuiltinFixedMetaDataID::None))};

			REQUIRE(weatherName.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((weatherName.value() == NONE_NAME));
		}
	}

	GIVEN("getRegisteredEntries")
	{
		THEN("the amount of weathers returned matches the amount that are built-in")
		{
			CHECK((config.getRegisteredEntries().size() == finalWeatherUnderlyingValue));
		}
	}

	GIVEN("hasEntry")
	{
		WHEN("calling the string_view overload")
		{
			THEN("an unknown weather name has no entry")
			{
				CHECK_FALSE(config.hasEntry("Unknown"));
			}

			THEN("a known weather name has an entry")
			{
				CHECK(config.hasEntry(NONE_NAME));
			}
		}

		WHEN("calling the FixedMetaDataID overload")
		{
			THEN("an unknown weather ID has no entry")
			{
				CHECK_FALSE(config.hasEntry(FixedMetaDataID{200}));
			}

			THEN("a known weather ID has an entry")
			{
				CHECK(config.hasEntry(NO_ID));
			}
		}
	}

	GIVEN("addMetadata")
	{
		WHEN("trying to add an weather past the capacity")
		{
			us newWeatherCount{finalWeatherUnderlyingValue};

			for (us i{0}; i < MAX_WEATHERS - finalWeatherUnderlyingValue; ++i)
			{
				std::string name{std::format("String_{:04}", i)};
				Metadata definition{.mName = name};
				std::expected<FixedMetaDataID, RegistryErrorInfo> result{config.addMetadata(definition)};

				REQUIRE(result.has_value());

				FixedMetaDataID assignedID{result.value()};
				CHECK((assignedID.getValue() == newWeatherCount++));
			}

			std::string name{std::format("String_{:04}", MAX_WEATHERS + 1)};

			Metadata definition{.mName = name};
			std::expected<FixedMetaDataID, RegistryErrorInfo> result{config.addMetadata(definition)};

			THEN("registration reports a max capacity and nothing is added")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::MaxCapacity));
				CHECK((config.getAmountRegistered() == MAX_WEATHERS));
			}
		}

		WHEN("an weather whose name is already in use is added")
		{
			Metadata definition{.mName = std::string(TEST1_NAME)};
			std::expected<FixedMetaDataID, RegistryErrorInfo> result{config.addMetadata(definition)};

			THEN("registration reports a duplicate weather and nothing is added")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::DuplicateMetadata));
				CHECK((config.getAmountRegistered() == finalWeatherUnderlyingValue));
			}
		}

		WHEN("a unique weather definition is added")
		{
			Metadata definition{.mName = "TestWeatherName"};

			std::expected<FixedMetaDataID, RegistryErrorInfo> result{config.addMetadata(definition)};

			THEN("it receives the first custom stable ID and owns its trigger data")
			{
				REQUIRE(result.has_value());
				FixedMetaDataID assignedID{result.value()};
				CHECK((assignedID.getValue() == finalWeatherUnderlyingValue));

				const Metadata *metadata{config.getMetadata(assignedID)};

				REQUIRE((metadata != nullptr));
				definition.mID = assignedID;
				CHECK((*metadata == definition));
			}
		}
	}

	GIVEN("addMetadataBatch")
	{
		WHEN("trying to add an weather past the capacity")
		{
			std::vector<Metadata> Metadatas;

			for (us i{0}; i < MAX_WEATHERS - finalWeatherUnderlyingValue; ++i)
			{
				std::string name{std::format("String_{:04}", i)};
				Metadatas.push_back({.mName = name});
			}

			std::string name{std::format("String_{:04}", MAX_WEATHERS + 1)};

			Metadatas.push_back({.mName = name});
			std::expected<void, RegistryErrorInfo> result{
				config.addMetadataBatch<Metadata>(Metadatas, [](const Metadata &definition) { return Metadata{definition}; }),
			};

			THEN("registration reports a max capacity and nothing is added")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::MaxCapacity));
				CHECK((config.getAmountRegistered() == finalWeatherUnderlyingValue));
			}
		}

		WHEN("an weather whose name is already in use is added")
		{
			std::vector<Metadata> Metadatas{};

			for (us i{0}; i < 20; ++i)
			{
				std::string name{std::format("String_{:04}", i)};
				Metadatas.push_back({.mName = name});
			}

			Metadatas.push_back({.mName = std::string(TEST1_NAME)});
			const FixedRegistry beforeBatch{config.getRegistry()};
			std::expected<void, RegistryErrorInfo> result{
				config.addMetadataBatch<Metadata>(Metadatas, [](const Metadata &definition) { return Metadata{definition}; }),
			};

			THEN("registration reports a duplicate weather and the registry is rollback to the checkpoint before the erroneous addition")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::DuplicateMetadata));
				CHECK((config.getAmountRegistered() == finalWeatherUnderlyingValue));
				CHECK((config.getAmountRegistered() == finalWeatherUnderlyingValue));
				CHECK((config.getRegistry() == beforeBatch));
				CHECK((config.getRegistry().getNextID() == beforeBatch.getNextID()));
				CHECK_FALSE(config.hasEntry("String_0000"));
			}
		}

		WHEN("a unique weather definition is added")
		{
			std::vector<Metadata> Metadatas{};

			Metadatas.push_back({.mName = "TestWeatherName"});

			std::expected<void, RegistryErrorInfo> result{
				config.addMetadataBatch<Metadata>(Metadatas, [](const Metadata &definition) { return Metadata{definition}; }),
			};

			THEN("the registry reports no error and all the weather definitions are added")
			{
				REQUIRE(result.has_value());
				CHECK((config.getAmountRegistered() == finalWeatherUnderlyingValue + 1));
			}
		}
	}

	GIVEN("renameMetadata")
	{
		WHEN("calling with an invalid weather name")
		{
			std::expected<void, RegistryErrorInfo> result{config.renameMetadata("ThisIsInvalid", "NewName")};

			THEN("the registry reports an error and there is no update to the registry")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::MetadataNotFound));
				CHECK((config.getAmountRegistered() == finalWeatherUnderlyingValue));
			}
		}

		WHEN("trying to rename to a name already in the registry")
		{
			std::expected<void, RegistryErrorInfo> result{
				config.renameMetadata(NONE_NAME, TEST1_NAME),
			};

			THEN("the registry reports an error and there is no update to the registry")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::DuplicateMetadata));
				CHECK((config.getAmountRegistered() == finalWeatherUnderlyingValue));
			}
		}

		WHEN("updating an existing weather definition")
		{
			std::expected<void, RegistryErrorInfo> result{config.renameMetadata(NONE_NAME, "NewName")};

			THEN("the registry reports no error and the name is appropriately updated")
			{
				REQUIRE(result.has_value());
				CHECK((config.getAmountRegistered() == finalWeatherUnderlyingValue));
				CHECK((config.getMetadata(toFixedMetaDataID(BuiltinFixedMetaDataID::None))->mName == "NewName"));
			}
		}
	}

	GIVEN("mutateMetadata")
	{
		Metadata definition{.mName = "TestWeatherName"};

		WHEN("calling the string_view overload")
		{
			WHEN("calling with an invalid weather name")
			{
				std::expected<void, RegistryErrorInfo> result{
					config.mutateMetadata("ThisIsInvalid", "FixedMetadataConfiguration",
										  [&definition](Metadata &metadata) { metadata = definition; }),
				};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::MetadataNotFound));
					CHECK((config.getAmountRegistered() == finalWeatherUnderlyingValue));
				}
			}

			WHEN("trying to rename to a name already in the registry")
			{
				std::expected<void, RegistryErrorInfo> result{
					config.mutateMetadata(NONE_NAME, "FixedMetadataConfiguration",
										  [](Metadata &metadata) { metadata = {.mName = std::string(TEST1_NAME)}; }),
				};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::DuplicateMetadata));
					CHECK((config.getAmountRegistered() == finalWeatherUnderlyingValue));
				}
			}

			WHEN("updating an existing weather definition")
			{
				std::expected<void, RegistryErrorInfo> result{
					config.mutateMetadata(NONE_NAME, "FixedMetadataConfiguration",
										  [&definition](Metadata &metadata) { metadata = definition; }),
				};

				THEN("the registry reports no error and the target is appropriately updated")
				{
					REQUIRE(result.has_value());
					CHECK((config.getAmountRegistered() == finalWeatherUnderlyingValue));
					CHECK((config.getMetadata(toFixedMetaDataID(BuiltinFixedMetaDataID::None))->mName == "TestWeatherName"));
				}
			}
		}

		WHEN("calling the FixedMetaDataID overload")
		{
			WHEN("calling with an invalid weather name")
			{
				std::expected<void, RegistryErrorInfo> result{
					config.mutateMetadata(FixedMetaDataID{200}, "FixedMetadataConfiguration",
										  [&definition](Metadata &metadata) { metadata = definition; }),
				};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::MetadataNotFound));
					CHECK((config.getAmountRegistered() == finalWeatherUnderlyingValue));
				}
			}

			WHEN("trying to rename to a name already in the registry")
			{
				std::expected<void, RegistryErrorInfo> result{
					config.mutateMetadata(toFixedMetaDataID(BuiltinFixedMetaDataID::None), "FixedMetadataConfiguration",
										  [](Metadata &metadata) { metadata = {.mName = std::string(TEST1_NAME)}; }),
				};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::DuplicateMetadata));
					CHECK((config.getAmountRegistered() == finalWeatherUnderlyingValue));
				}
			}

			WHEN("updating an existing weather definition")
			{
				std::expected<void, RegistryErrorInfo> result{
					config.mutateMetadata(toFixedMetaDataID(BuiltinFixedMetaDataID::None), "FixedMetadataConfiguration",
										  [&definition](Metadata &metadata) { metadata = definition; }),
				};

				THEN("the registry reports no error and the target is appropriately updated")
				{
					REQUIRE(result.has_value());
					CHECK((config.getAmountRegistered() == finalWeatherUnderlyingValue));
					CHECK((config.getMetadata(toFixedMetaDataID(BuiltinFixedMetaDataID::None))->mName == "TestWeatherName"));
				}
			}
		}
	}

	GIVEN("removeMetadata")
	{
		WHEN("calling the string_view overload")
		{
			WHEN("calling with an unknown weather name")
			{
				std::expected<FixedMetaDataID, RegistryErrorInfo> result{config.removeMetadata("Unknown")};

				THEN("the result is an error and the registry is not updated")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::MetadataNotFound));
					CHECK((config.getAmountRegistered() == finalWeatherUnderlyingValue));
				}
			}

			WHEN("calling with a known weather name")
			{
				std::expected<FixedMetaDataID, RegistryErrorInfo> result{config.removeMetadata(NONE_NAME)};

				THEN("the result is a success and the registry is updated")
				{
					REQUIRE(result.has_value());
					CHECK((result.value() == toFixedMetaDataID(BuiltinFixedMetaDataID::None)));
					CHECK((config.getAmountRegistered() == finalWeatherUnderlyingValue - 1));
				}
			}
		}

		WHEN("calling the FixedMetaDataID overload")
		{
			WHEN("calling with an unknown weather ID")
			{
				std::expected<FixedMetaDataID, RegistryErrorInfo> result{config.removeMetadata(FixedMetaDataID{200})};

				THEN("the result is an error and the registry is not updated")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::MetadataNotFound));
					CHECK((config.getAmountRegistered() == finalWeatherUnderlyingValue));
				}
			}

			WHEN("calling with a known weather ID")
			{
				std::expected<FixedMetaDataID, RegistryErrorInfo> result{
					config.removeMetadata(toFixedMetaDataID(BuiltinFixedMetaDataID::None)),
				};

				THEN("the result is a success and the registry is updated")
				{
					REQUIRE(result.has_value());
					CHECK((result.value() == toFixedMetaDataID(BuiltinFixedMetaDataID::None)));
					CHECK((config.getAmountRegistered() == finalWeatherUnderlyingValue - 1));
				}
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
