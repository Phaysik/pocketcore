/*! @file fixedMetadataRegistryConfiguration.test.cpp
	@brief C++ file for running tests for the FixedMetadataRegistryConfiguration.
	@date 10/28/2026
	@since 0.12.50
	@version 0.12.53
	@author Matthew Moore
*/

#include <expected>
#include <format>
#include <limits>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "Configuration/fixedMetadataRegistryConfiguration.testHelper.h"
#include "Core/typedefs.h"
#include "Registry/fixedMetadataRegistry.testHelper.h"
#include "Utility/Debug/Logging/logging.testHelper.h"
#include "Validation/Registry/registryError.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Core::ub;
using PocketCore::Core::us;
using PocketCore::Testing::BuiltinFixedMetaDataID;
using PocketCore::Testing::CAPACITY;
using PocketCore::Testing::ensureLoggerInitialized;
using PocketCore::Testing::FixedMetaDataID;
using PocketCore::Testing::FixedRegistry;
using PocketCore::Testing::FixedRegistryConfiguration;
using PocketCore::Testing::Metadata;
using PocketCore::Testing::NO_ID;
using PocketCore::Testing::NONE_NAME;
using PocketCore::Testing::TEST1_NAME;
using PocketCore::Testing::toFixedMetaDataID;
using PocketCore::Validation::Registry::RegistryError;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("FixedMetadataRegistryConfiguration")
{
	ensureLoggerInitialized("fixed metadata registry configuration test", "FixedRegistryConfiguration_test.log");

	std::unique_ptr<FixedRegistryConfiguration> configRegistryStorage{std::make_unique<FixedRegistryConfiguration>()};
	FixedRegistryConfiguration &config{*configRegistryStorage};
	std::unique_ptr<FixedRegistry> registryStorage{std::make_unique<FixedRegistry>()};
	FixedRegistry &registry{*registryStorage};
	ub finalFixedUnderlyingValue{std::to_underlying(BuiltinFixedMetaDataID::Final)};

	GIVEN("getAmountRegistered")
	{
		THEN("the default registry returns the expected amount of built-in fixed entities")
		{
			CHECK((config.getAmountRegistered() == finalFixedUnderlyingValue));
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

		THEN("the metadata is retrieved when accessed by a valid Fixed Metadata ID")
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

		THEN("the Fixed Metadata ID is retrieved by valid Fixed Metadata name")
		{
			std::optional<FixedMetaDataID> FixedMetaDataID{config.getID(NONE_NAME)};

			REQUIRE(FixedMetaDataID.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((FixedMetaDataID == toFixedMetaDataID(BuiltinFixedMetaDataID::None)));
		}
	}

	GIVEN("getName")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(config.getName(FixedMetaDataID{200}).has_value());
		}

		THEN("a registered Fixed Metadata name is returned by stable ID")
		{
			std::optional<std::string_view> fixedMetadataName{config.getName(toFixedMetaDataID(BuiltinFixedMetaDataID::None))};

			REQUIRE(fixedMetadataName.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((fixedMetadataName == NONE_NAME));
		}
	}

	GIVEN("getRegisteredEntries")
	{
		THEN("the amount of fixed entities returned matches the amount that are built-in")
		{
			CHECK((config.getRegisteredEntries().size() == finalFixedUnderlyingValue));
		}
	}

	GIVEN("hasEntry")
	{
		WHEN("calling the string_view overload")
		{
			THEN("an unknown Fixed Metadata name has no entry")
			{
				CHECK_FALSE(config.hasEntry("Unknown"));
			}

			THEN("a known Fixed Metadata name has an entry")
			{
				CHECK(config.hasEntry(NONE_NAME));
			}
		}

		WHEN("calling the FixedMetaDataID overload")
		{
			THEN("an unknown Fixed Metadata ID has no entry")
			{
				CHECK_FALSE(config.hasEntry(FixedMetaDataID{200}));
			}

			THEN("a known Fixed Metadata ID has an entry")
			{
				CHECK(config.hasEntry(NO_ID));
			}
		}
	}

	GIVEN("addMetadata")
	{
		WHEN("trying to add a Fixed Metadata past the capacity")
		{
			us newFixedMetadataCount{finalFixedUnderlyingValue};

			for (us i{0}; i < CAPACITY - finalFixedUnderlyingValue; ++i)
			{
				std::string name{std::format("String_{:04}", i)};
				Metadata definition{.mName = name};
				std::expected<FixedMetaDataID, RegistryError> result{config.addMetadata(definition)};

				REQUIRE(result.has_value());

				FixedMetaDataID assignedID{result.value()};
				CHECK((assignedID.getValue() == newFixedMetadataCount++));
			}

			std::string name{std::format("String_{:04}", CAPACITY + 1)};

			Metadata definition{.mName = name};
			std::expected<FixedMetaDataID, RegistryError> result{config.addMetadata(definition)};

			THEN("registration reports a max capacity and nothing is added")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error() == RegistryError::MaxCapacity));
				CHECK((config.getAmountRegistered() == CAPACITY));
			}
		}

		WHEN("stable IDs are exhausted while storage capacity remains available")
		{
			us maximumNextID{std::numeric_limits<us>::max()};
			Metadata definition{.mName = "ReusedCustomMetadata"};

			for (us nextID{config.getRegistry().getNextID()}; nextID < maximumNextID; ++nextID)
			{
				std::expected<FixedMetaDataID, RegistryError> added{config.addMetadata(definition)};
				REQUIRE(added.has_value());
				REQUIRE((added.value().getValue() == nextID));

				std::expected<FixedMetaDataID, RegistryError> removed{config.removeMetadata(added.value())};
				REQUIRE(removed.has_value());
			}

			REQUIRE((config.getAmountRegistered() < CAPACITY));
			REQUIRE((config.getRegistry().getNextID() == maximumNextID));
			FixedRegistry beforeAddition{config.getRegistry()};
			std::expected<FixedMetaDataID, RegistryError> result{config.addMetadata(definition)};

			THEN("registration reports max capacity without changing registry state or wrapping stable IDs")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error() == RegistryError::MaxCapacity));
				CHECK((config.getAmountRegistered() == finalFixedUnderlyingValue));
				CHECK((config.getRegistry().getNextID() == maximumNextID));
				CHECK((config.getRegistry() == beforeAddition));
				CHECK_FALSE(config.hasEntry(definition.mName));
			}
		}

		WHEN("a Fixed Metadata whose name is already in use is added")
		{
			Metadata definition{.mName = std::string(TEST1_NAME)};
			std::expected<FixedMetaDataID, RegistryError> result{config.addMetadata(definition)};

			THEN("registration reports a duplicate Fixed Metadata and nothing is added")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error() == RegistryError::DuplicateMetadata));
				CHECK((config.getAmountRegistered() == finalFixedUnderlyingValue));
			}
		}

		WHEN("a unique Fixed Metadata definition is added")
		{
			Metadata definition{.mName = "TestFixedMetadataName"};

			std::expected<FixedMetaDataID, RegistryError> result{config.addMetadata(definition)};

			THEN("it receives the first custom stable ID and owns its trigger data")
			{
				REQUIRE(result.has_value());
				FixedMetaDataID assignedID{result.value()};
				CHECK((assignedID.getValue() == finalFixedUnderlyingValue));

				const Metadata *metadata{config.getMetadata(assignedID)};

				REQUIRE((metadata != nullptr));
				definition.mID = assignedID;
				CHECK((*metadata == definition));
			}
		}
	}

	GIVEN("addMetadataBatch")
	{
		WHEN("trying to add a Fixed Metadata past the capacity")
		{
			std::vector<Metadata> Metadatas;

			for (us i{0}; i < CAPACITY - finalFixedUnderlyingValue; ++i)
			{
				std::string name{std::format("String_{:04}", i)};
				Metadatas.push_back({.mName = name});
			}

			std::string name{std::format("String_{:04}", CAPACITY + 1)};

			Metadatas.push_back({.mName = name});
			std::expected<void, RegistryError> result{
				config.addMetadataBatch<Metadata>(Metadatas, [](const Metadata &definition) { return Metadata{definition}; }),
			};

			THEN("registration reports a max capacity and nothing is added")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error() == RegistryError::MaxCapacity));
				CHECK((config.getAmountRegistered() == finalFixedUnderlyingValue));
			}
		}

		WHEN("a Fixed Metadata whose name is already in use is added")
		{
			std::vector<Metadata> Metadatas{};

			for (us i{0}; i < 20; ++i)
			{
				std::string name{std::format("String_{:04}", i)};
				Metadatas.push_back({.mName = name});
			}

			Metadatas.push_back({.mName = std::string(TEST1_NAME)});
			const FixedRegistry beforeBatch{config.getRegistry()};
			std::expected<void, RegistryError> result{
				config.addMetadataBatch<Metadata>(Metadatas, [](const Metadata &definition) { return Metadata{definition}; }),
			};

			THEN("registration reports a duplicate Fixed Metadata and the registry is rollback to the checkpoint before the erroneous "
				 "addition")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error() == RegistryError::DuplicateMetadata));
				CHECK((config.getAmountRegistered() == finalFixedUnderlyingValue));
				CHECK((config.getAmountRegistered() == finalFixedUnderlyingValue));
				CHECK((config.getRegistry() == beforeBatch));
				CHECK((config.getRegistry().getNextID() == beforeBatch.getNextID()));
				CHECK_FALSE(config.hasEntry("String_0000"));
			}
		}

		WHEN("a unique Fixed Metadata definition is added")
		{
			std::vector<Metadata> Metadatas{};

			Metadatas.push_back({.mName = "TestFixedMetadataName"});

			std::expected<void, RegistryError> result{
				config.addMetadataBatch<Metadata>(Metadatas, [](const Metadata &definition) { return Metadata{definition}; }),
			};

			THEN("the registry reports no error and all the Fixed Metadata definitions are added")
			{
				REQUIRE(result.has_value());
				CHECK((config.getAmountRegistered() == finalFixedUnderlyingValue + 1));
			}
		}
	}

	GIVEN("renameMetadata")
	{
		WHEN("calling with an invalid Fixed Metadata name")
		{
			std::expected<void, RegistryError> result{config.renameMetadata("ThisIsInvalid", "NewName")};

			THEN("the registry reports an error and there is no update to the registry")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error() == RegistryError::MetadataNotFound));
				CHECK((config.getAmountRegistered() == finalFixedUnderlyingValue));
			}
		}

		WHEN("trying to rename to a name already in the registry")
		{
			std::expected<void, RegistryError> result{
				config.renameMetadata(NONE_NAME, TEST1_NAME),
			};

			THEN("the registry reports an error and there is no update to the registry")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error() == RegistryError::DuplicateMetadata));
				CHECK((config.getAmountRegistered() == finalFixedUnderlyingValue));
			}
		}

		WHEN("updating an existing Fixed Metadata definition")
		{
			std::expected<void, RegistryError> result{config.renameMetadata(NONE_NAME, "NewName")};

			THEN("the registry reports no error and the name is appropriately updated")
			{
				REQUIRE(result.has_value());
				CHECK((config.getAmountRegistered() == finalFixedUnderlyingValue));
				CHECK((config.getMetadata(toFixedMetaDataID(BuiltinFixedMetaDataID::None))->mName == "NewName"));
			}
		}
	}

	GIVEN("mutateMetadata")
	{
		Metadata definition{.mName = "TestFixedMetadataName"};

		WHEN("calling the string_view overload")
		{
			WHEN("calling with an invalid Fixed Metadata name")
			{
				std::expected<void, RegistryError> result{
					config.mutateMetadata("ThisIsInvalid", "FixedMetadataConfiguration",
										  [&definition](Metadata &metadata) { metadata = definition; }),
				};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error() == RegistryError::MetadataNotFound));
					CHECK((config.getAmountRegistered() == finalFixedUnderlyingValue));
				}
			}

			WHEN("trying to rename to a name already in the registry")
			{
				std::expected<void, RegistryError> result{
					config.mutateMetadata(NONE_NAME, "FixedMetadataConfiguration",
										  [](Metadata &metadata) { metadata = {.mName = std::string(TEST1_NAME)}; }),
				};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error() == RegistryError::DuplicateMetadata));
					CHECK((config.getAmountRegistered() == finalFixedUnderlyingValue));
				}
			}

			WHEN("updating an existing Fixed Metadata definition")
			{
				std::expected<void, RegistryError> result{
					config.mutateMetadata(NONE_NAME, "FixedMetadataConfiguration",
										  [&definition](Metadata &metadata) { metadata = definition; }),
				};

				THEN("the registry reports no error and the target is appropriately updated")
				{
					REQUIRE(result.has_value());
					CHECK((config.getAmountRegistered() == finalFixedUnderlyingValue));
					CHECK((config.getMetadata(toFixedMetaDataID(BuiltinFixedMetaDataID::None))->mName == "TestFixedMetadataName"));
				}
			}
		}

		WHEN("calling the FixedMetaDataID overload")
		{
			WHEN("calling with an invalid Fixed Metadata name")
			{
				std::expected<void, RegistryError> result{
					config.mutateMetadata(FixedMetaDataID{200}, "FixedMetadataConfiguration",
										  [&definition](Metadata &metadata) { metadata = definition; }),
				};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error() == RegistryError::MetadataNotFound));
					CHECK((config.getAmountRegistered() == finalFixedUnderlyingValue));
				}
			}

			WHEN("trying to rename to a name already in the registry")
			{
				std::expected<void, RegistryError> result{
					config.mutateMetadata(toFixedMetaDataID(BuiltinFixedMetaDataID::None), "FixedMetadataConfiguration",
										  [](Metadata &metadata) { metadata = {.mName = std::string(TEST1_NAME)}; }),
				};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error() == RegistryError::DuplicateMetadata));
					CHECK((config.getAmountRegistered() == finalFixedUnderlyingValue));
				}
			}

			WHEN("updating an existing Fixed Metadata definition")
			{
				std::expected<void, RegistryError> result{
					config.mutateMetadata(toFixedMetaDataID(BuiltinFixedMetaDataID::None), "FixedMetadataConfiguration",
										  [&definition](Metadata &metadata) { metadata = definition; }),
				};

				THEN("the registry reports no error and the target is appropriately updated")
				{
					REQUIRE(result.has_value());
					CHECK((config.getAmountRegistered() == finalFixedUnderlyingValue));
					CHECK((config.getMetadata(toFixedMetaDataID(BuiltinFixedMetaDataID::None))->mName == "TestFixedMetadataName"));
				}
			}
		}
	}

	GIVEN("removeMetadata")
	{
		WHEN("calling the string_view overload")
		{
			WHEN("calling with an unknown Fixed Metadata name")
			{
				std::expected<FixedMetaDataID, RegistryError> result{config.removeMetadata("Unknown")};

				THEN("the result is an error and the registry is not updated")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error() == RegistryError::MetadataNotFound));
					CHECK((config.getAmountRegistered() == finalFixedUnderlyingValue));
				}
			}

			WHEN("calling with a known Fixed Metadata name")
			{
				std::expected<FixedMetaDataID, RegistryError> result{config.removeMetadata(NONE_NAME)};

				THEN("the result is a success and the registry is updated")
				{
					REQUIRE(result.has_value());
					CHECK((result == toFixedMetaDataID(BuiltinFixedMetaDataID::None)));
					CHECK((config.getAmountRegistered() == finalFixedUnderlyingValue - 1));
				}
			}
		}

		WHEN("calling the FixedMetaDataID overload")
		{
			WHEN("calling with an unknown Fixed Metadata ID")
			{
				std::expected<FixedMetaDataID, RegistryError> result{config.removeMetadata(FixedMetaDataID{200})};

				THEN("the result is an error and the registry is not updated")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error() == RegistryError::MetadataNotFound));
					CHECK((config.getAmountRegistered() == finalFixedUnderlyingValue));
				}
			}

			WHEN("calling with a known Fixed Metadata ID")
			{
				std::expected<FixedMetaDataID, RegistryError> result{
					config.removeMetadata(toFixedMetaDataID(BuiltinFixedMetaDataID::None)),
				};

				THEN("the result is a success and the registry is updated")
				{
					REQUIRE(result.has_value());
					CHECK((result == toFixedMetaDataID(BuiltinFixedMetaDataID::None)));
					CHECK((config.getAmountRegistered() == finalFixedUnderlyingValue - 1));
				}
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
