/*! @file fixedMetadataRegistry.test.cpp
	@brief C++ file for running tests for the FixedMetadataRegistry.
	@date 10/28/2026
	@since 0.7.0
	@version 0.12.53
	@author Matthew Moore
*/

#include <expected>
#include <memory>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <utility>

#include "Configuration/fixedMetadataRegistryConfiguration.testHelper.h"
#include "Core/typedefs.h"
#include "Registry/fixedMetadataRegistry.testHelper.h"
#include "Validation/Registry/registryError.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Core::ub;
using PocketCore::Core::us;
using PocketCore::Testing::BuiltinFixedMetaDataID;
using PocketCore::Testing::CheckpointRegistry;
using PocketCore::Testing::FixedMetaDataID;
using PocketCore::Testing::FixedRegistry;
using PocketCore::Testing::FixedRegistryConfiguration;
using PocketCore::Testing::Metadata;
using PocketCore::Testing::NO_ID;
using PocketCore::Testing::NONE_NAME;
using PocketCore::Testing::TEST1_NAME;
using PocketCore::Testing::TEST2_NAME;
using PocketCore::Testing::TEST3_NAME;
using PocketCore::Testing::toFixedMetaDataID;
using PocketCore::Validation::Registry::RegistryError;

template <typename Registry>
concept PubliclyStructurallyMutable = requires(Registry &registry) { registry.setAmountRegistered(0); };

static_assert(!PubliclyStructurallyMutable<FixedRegistry>);

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("FixedMetadataRegistry")
{
	std::unique_ptr<FixedRegistry> registryStorage{std::make_unique<FixedRegistry>()};
	FixedRegistry &registry{*registryStorage};
	ub finalWeatherUnderlyingValue{std::to_underlying(BuiltinFixedMetaDataID::Final)};

	GIVEN("a default constructed FixedRegistry")
	{
		THEN("Test 1 has the appropriate properties")
		{
			Metadata expected{
				.mName = std::string(TEST1_NAME),
				.mID = toFixedMetaDataID(BuiltinFixedMetaDataID::Test1),
			};

			const Metadata *actual{registry.getMetadata(toFixedMetaDataID(BuiltinFixedMetaDataID::Test1))};

			CHECK((expected == *actual));
		}

		THEN("Test 2 has the appropriate properties")
		{
			Metadata expected{
				.mName = std::string(TEST2_NAME),
				.mID = toFixedMetaDataID(BuiltinFixedMetaDataID::Test2),
			};

			const Metadata *actual{registry.getMetadata(toFixedMetaDataID(BuiltinFixedMetaDataID::Test2))};

			CHECK((expected == *actual));
		}

		THEN("Test 3 has the appropriate properties")
		{
			Metadata expected{
				.mName = std::string(TEST3_NAME),
				.mID = toFixedMetaDataID(BuiltinFixedMetaDataID::Test3),
			};

			const Metadata *actual{registry.getMetadata(toFixedMetaDataID(BuiltinFixedMetaDataID::Test3))};

			CHECK((expected == *actual));
		}
	}

	GIVEN("getMetadata")
	{
		THEN("unknown IDs are absent")
		{
			CHECK((registry.getMetadata(FixedMetaDataID{200}) == nullptr));
		}

		THEN("the metadata is retrieved when accessed by a valid Weather ID")
		{
			Metadata expected{
				.mName = std::string(NONE_NAME),
				.mID = toFixedMetaDataID(BuiltinFixedMetaDataID::None),
			};

			CHECK((expected == *registry.getMetadata(NO_ID)));
		}
	}

	GIVEN("getID")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(registry.getID("Unknown").has_value());
		}

		THEN("the Weather ID is retrieved by valid Weather name")
		{
			std::optional<FixedMetaDataID> FixedMetaDataID{registry.getID(NONE_NAME)};

			REQUIRE(FixedMetaDataID.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((FixedMetaDataID == toFixedMetaDataID(BuiltinFixedMetaDataID::None)));
		}
	}

	GIVEN("getName")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(registry.getName(FixedMetaDataID{200}).has_value());
		}

		THEN("a registered weather name is returned by stable ID")
		{
			std::optional<std::string_view> weatherName{registry.getName(toFixedMetaDataID(BuiltinFixedMetaDataID::None))};

			REQUIRE(weatherName.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((weatherName == NONE_NAME));
		}
	}

	GIVEN("getAmountRegistered")
	{
		THEN("the registered span contains the exact amount of built-in entries")
		{
			CHECK((registry.getAmountRegistered() == finalWeatherUnderlyingValue));
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
			Metadata expected{
				.mName = std::string(NONE_NAME),
				.mID = toFixedMetaDataID(BuiltinFixedMetaDataID::None),
			};

			const Metadata *Metadata{registry.getEntry(0)};

			REQUIRE((Metadata != nullptr));
			CHECK((*Metadata == expected));
		}
	}

	GIVEN("getRegisteredEntries")
	{
		THEN("the amount of weathers returned matches the amount that are built-in")
		{
			CHECK((registry.getRegisteredEntries().size() == finalWeatherUnderlyingValue));
		}
	}

	GIVEN("getNextID")
	{
		THEN("the next available stable Weather ID is after all built in weather IDs")
		{
			CHECK((registry.getNextID() == finalWeatherUnderlyingValue));
		}
	}

	GIVEN("findIndexByID")
	{
		THEN("an unknown stable ID has no internal index")
		{
			std::optional<ub> weatherIndex{registry.findIndexByID(FixedMetaDataID{200})};
			CHECK_FALSE(weatherIndex.has_value());
		}

		THEN("the internal array index is retrieved by valid Weather ID")
		{
			std::optional<ub> weatherIndex{registry.findIndexByID(NO_ID)};

			REQUIRE(weatherIndex.has_value());
			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((weatherIndex == 0));
		}
	}

	GIVEN("hasEntry")
	{
		WHEN("calling the string_view overload")
		{
			THEN("an unknown weather name has no entry")
			{
				CHECK_FALSE(registry.hasEntry("Unknown"));
			}

			THEN("a known weather name has an entry")
			{
				CHECK(registry.hasEntry(NONE_NAME));
			}
		}

		WHEN("calling the FixedMetaDataID overload")
		{
			THEN("an unknown weather ID has no entry")
			{
				CHECK_FALSE(registry.hasEntry(FixedMetaDataID{200}));
			}

			THEN("a known weather ID has an entry")
			{
				CHECK(registry.hasEntry(NO_ID));
			}
		}
	}

	GIVEN("an unregistered storage slot")
	{
		CheckpointRegistry stagedRegistry{};
		CheckpointRegistry originalRegistry{stagedRegistry};
		us unregisteredIndex{stagedRegistry.getAmountRegistered()};
		Metadata stagedMetadata{.mName = "Staged", .mID = FixedMetaDataID{200}};

		WHEN("metadata is written without registering the slot")
		{
			stagedRegistry.setEntry(unregisteredIndex, stagedMetadata);

			THEN("the storage changes without exposing the entry through registered lookups")
			{
				const Metadata *storedMetadata{stagedRegistry.getEntry(unregisteredIndex)};
				REQUIRE((storedMetadata != nullptr));
				CHECK((*storedMetadata == stagedMetadata));
				CHECK((stagedRegistry == originalRegistry));
				CHECK((stagedRegistry.getAmountRegistered() == originalRegistry.getAmountRegistered()));
				CHECK((stagedRegistry.getNextID() == originalRegistry.getNextID()));
				CHECK_FALSE(stagedRegistry.hasEntry(stagedMetadata.mID));
				CHECK_FALSE(stagedRegistry.hasEntry(stagedMetadata.mName));
				CHECK_FALSE(stagedRegistry.getID(stagedMetadata.mName).has_value());
				CHECK((stagedRegistry.getMetadata(stagedMetadata.mID) == nullptr));
			}
		}
	}

	GIVEN("low-level metadata entries sharing a stable ID")
	{
		CheckpointRegistry tiedRegistry{};
		FixedMetaDataID sharedID{200};
		Metadata firstMetadata{.mName = "First shared ID", .mID = sharedID};
		Metadata lastMetadata{.mName = "Last shared ID", .mID = sharedID};
		us originalCount{tiedRegistry.getAmountRegistered()};
		us originalNextID{tiedRegistry.getNextID()};
		tiedRegistry.setEntry(0, firstMetadata);
		tiedRegistry.setEntry(static_cast<us>(originalCount - 1U), lastMetadata);

		THEN("stable-ID lookup deterministically returns the earliest registered entry")
		{
			const Metadata *foundMetadata{tiedRegistry.getMetadata(sharedID)};
			REQUIRE((foundMetadata != nullptr));
			CHECK((*foundMetadata == firstMetadata));
			CHECK((tiedRegistry.findIndexByID(sharedID) == 0));
		}

		WHEN("the last registered entry is removed from the lookup index")
		{
			tiedRegistry.decrementAmountRegistered();

			THEN("the earlier equal-ID entry remains registered and searchable")
			{
				const Metadata *foundMetadata{tiedRegistry.getMetadata(sharedID)};
				REQUIRE((foundMetadata != nullptr));
				CHECK((*foundMetadata == firstMetadata));
				CHECK((tiedRegistry.getAmountRegistered() == originalCount - 1U));
				CHECK((tiedRegistry.getNextID() == originalNextID));
				CHECK((tiedRegistry.findIndexByID(sharedID) == 0));
				CHECK(tiedRegistry.hasEntry(firstMetadata.mName));
				CHECK_FALSE(tiedRegistry.hasEntry(lastMetadata.mName));
			}
		}
	}

	GIVEN("the final storage entry is not last in stable-ID order")
	{
		CheckpointRegistry compactedRegistry{};
		const Metadata retainedMetadata{.mName = "Retained high ID", .mID = FixedMetaDataID{200}};
		const us originalCount{compactedRegistry.getAmountRegistered()};
		const us originalNextID{compactedRegistry.getNextID()};
		const FixedMetaDataID removedID{toFixedMetaDataID(BuiltinFixedMetaDataID::Test3)};
		compactedRegistry.setEntry(0, retainedMetadata);

		WHEN("the final stored entry is removed from the lookup index")
		{
			compactedRegistry.decrementAmountRegistered();

			THEN("later index mappings shift without losing retained entries")
			{
				const Metadata *foundMetadata{compactedRegistry.getMetadata(retainedMetadata.mID)};
				REQUIRE((foundMetadata != nullptr));
				CHECK((*foundMetadata == retainedMetadata));
				CHECK((compactedRegistry.findIndexByID(retainedMetadata.mID) == 0));
				CHECK((compactedRegistry.getAmountRegistered() == originalCount - 1U));
				CHECK((static_cast<us>(compactedRegistry.getRegisteredEntries().size()) == originalCount - 1U));
				CHECK((compactedRegistry.getNextID() == originalNextID));
				CHECK(compactedRegistry.hasEntry(toFixedMetaDataID(BuiltinFixedMetaDataID::Test1)));
				CHECK(compactedRegistry.hasEntry(toFixedMetaDataID(BuiltinFixedMetaDataID::Test2)));
				CHECK_FALSE(compactedRegistry.hasEntry(removedID));
				CHECK_FALSE(compactedRegistry.hasEntry(TEST3_NAME));
			}
		}
	}

	GIVEN("restoreCheckpoint")
	{
		WHEN("restoring a checkpoint after the same registry loses an entry")
		{
			CheckpointRegistry targetRegistry{};
			auto checkpoint{targetRegistry.createCheckpoint()};
			targetRegistry.eraseEntry(0);
			CheckpointRegistry beforeRestore{targetRegistry};

			targetRegistry.restoreCheckpoint(checkpoint);

			THEN("the checkpoint cannot resurrect entries or change the remaining registry")
			{
				CHECK((targetRegistry == beforeRestore));
				CHECK((targetRegistry.getAmountRegistered() == beforeRestore.getAmountRegistered()));
				CHECK((targetRegistry.getNextID() == beforeRestore.getNextID()));
				CHECK_FALSE(targetRegistry.hasEntry(NO_ID));
				CHECK_FALSE(targetRegistry.hasEntry(NONE_NAME));
				CHECK(targetRegistry.hasEntry(toFixedMetaDataID(BuiltinFixedMetaDataID::Test1)));
			}
		}

		WHEN("restoring a checkpoint created by another registry")
		{
			CheckpointRegistry sourceRegistry{};
			CheckpointRegistry targetRegistry{};
			auto checkpoint{sourceRegistry.createCheckpoint()};
			targetRegistry.eraseEntry(0);

			targetRegistry.restoreCheckpoint(checkpoint);

			THEN("the checkpoint does not apply and the target registry remains unchanged")
			{
				CHECK((targetRegistry.getAmountRegistered() == finalWeatherUnderlyingValue - 1));
				CHECK_FALSE(targetRegistry.hasEntry(NO_ID));
			}
		}
	}

	GIVEN("registry equality")
	{
		CheckpointRegistry actual{};
		const CheckpointRegistry original{actual};

		THEN("identical registered metadata and next IDs compare equal")
		{
			CHECK((actual == original));
		}

		WHEN("entries are appended and rolled back")
		{
			const auto checkpoint{actual.createCheckpoint()};
			const FixedMetaDataID appendedID{actual.addEntry({.mName = "Appended"})};
			actual.restoreCheckpoint(checkpoint);

			THEN("the restored registry compares equal in both directions")
			{
				CHECK((actual == original));
				CHECK((original == actual));
				CHECK((actual.getNextID() == original.getNextID()));
				CHECK_FALSE(actual.hasEntry(appendedID));
				CHECK_FALSE(actual.hasEntry("Appended"));
			}

			WHEN("the old checkpoint is reused after another append")
			{
				const FixedMetaDataID retainedID{actual.addEntry({.mName = "Retained"})};
				const CheckpointRegistry beforeRestore{actual};
				actual.restoreCheckpoint(checkpoint);

				THEN("the stale checkpoint is rejected and the new entry remains registered")
				{
					CHECK((actual == beforeRestore));
					CHECK(actual.hasEntry(retainedID));
					CHECK(actual.hasEntry("Retained"));
				}
			}
		}

		WHEN("the registered entry counts differ")
		{
			actual.addEntry({.mName = "Additional"});

			THEN("the registries compare unequal")
			{
				CHECK((actual != original));
			}
		}

		WHEN("the registered metadata differs with matching counts and next IDs")
		{
			CheckpointRegistry other{};
			actual.addEntry({.mName = "First"});
			other.addEntry({.mName = "Second"});

			THEN("the registries compare unequal")
			{
				CHECK((actual.getAmountRegistered() == other.getAmountRegistered()));
				CHECK((actual.getNextID() == other.getNextID()));
				CHECK((actual != other));
			}
		}

		WHEN("an entry is appended and removed without rolling back its assigned ID")
		{
			actual.addEntry({.mName = "Removed"});
			actual.eraseEntry(original.getAmountRegistered());

			THEN("matching registered metadata with different next IDs compares unequal")
			{
				REQUIRE((actual.getAmountRegistered() == original.getAmountRegistered()));
				for (PocketCore::Core::us index{0}; index < actual.getAmountRegistered(); ++index)
				{
					CHECK((*actual.getEntry(index) == *original.getEntry(index)));
				}
				CHECK((actual.getNextID() != original.getNextID()));
				CHECK((actual != original));
			}
		}
	}

	WHEN("operator==")
	{
		GIVEN("two default constructed registries")
		{
			std::unique_ptr<FixedRegistry> other{std::make_unique<FixedRegistry>()};

			THEN("they are equal")
			{
				CHECK((registry == *other));
			}
		}

		GIVEN("for an entry added in one registry")
		{
			std::unique_ptr<FixedRegistryConfiguration> other{std::make_unique<FixedRegistryConfiguration>()};
			std::expected<FixedMetaDataID, RegistryError> result{other->addMetadata({.mName = "test"})};

			THEN("they are not equal")
			{
				REQUIRE(result.has_value());
				CHECK((registry != other->getRegistry()));
			}
		}

		GIVEN("for an entry removed in one registry")
		{
			std::unique_ptr<FixedRegistryConfiguration> other{std::make_unique<FixedRegistryConfiguration>()};
			std::expected<FixedMetaDataID, RegistryError> result{
				other->removeMetadata(toFixedMetaDataID(BuiltinFixedMetaDataID::None)),
			};

			THEN("they are not equal")
			{
				REQUIRE(result.has_value());
				CHECK((registry != other->getRegistry()));
			}
		}

		GIVEN("for an entry modified in one registry")
		{
			std::unique_ptr<FixedRegistryConfiguration> other{std::make_unique<FixedRegistryConfiguration>()};
			std::expected<void, RegistryError> result{
				other->mutateMetadata(toFixedMetaDataID(BuiltinFixedMetaDataID::None), "FixedMetadata",
									  [](Metadata &metadata) { metadata = {.mName = "test"}; }),
			};

			THEN("they are not equal")
			{
				REQUIRE(result.has_value());
				CHECK((registry != other->getRegistry()));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
