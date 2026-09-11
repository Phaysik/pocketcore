/*! @file fixedMetadataRegistry.test.cpp
	@brief C++ file for running tests for the FixedMetadataRegistry.
	@date 09/11/2026
	@since 0.7.0
	@version 0.12.22
	@author Matthew Moore
*/

#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <utility>

#include "Core/typedefs.h"
#include "Registry/fixedMetadataRegistry.testHelper.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Core::ub;
using PocketCore::Testing::BuiltinFixedMetaDataID;
using PocketCore::Testing::CheckpointRegistry;
using PocketCore::Testing::FixedMetaDataID;
using PocketCore::Testing::FixedRegistry;
using PocketCore::Testing::Metadata;
using PocketCore::Testing::NO_ID;
using PocketCore::Testing::NONE_NAME;
using PocketCore::Testing::TEST1_NAME;
using PocketCore::Testing::TEST2_NAME;
using PocketCore::Testing::TEST3_NAME;
using PocketCore::Testing::toFixedMetaDataID;

template <typename Registry>
concept PubliclyStructurallyMutable = requires(Registry &registry) { registry.setAmountRegistered(0); };

static_assert(!PubliclyStructurallyMutable<FixedRegistry>);

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("FixedMetadataRegistry")
{
	FixedRegistry registry{};
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
			CHECK((FixedMetaDataID.value() == toFixedMetaDataID(BuiltinFixedMetaDataID::None)));
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
			CHECK((weatherName.value() == NONE_NAME));
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
			CHECK((weatherIndex.value() == 0));
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

	GIVEN("restoreCheckpoint")
	{
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
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
