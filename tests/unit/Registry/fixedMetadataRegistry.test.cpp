/*! @file fixedMetadataRegistry.test.cpp
	@brief C++ file for running tests for the FixedMetadataRegistry.
	@date 09/03/2026
	@since 0.7.0
	@version 0.12.18
	@author Matthew Moore
*/

#include "Registry/fixedMetadataRegistry.h"

#include <optional>
#include <span>
#include <string_view>

#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "ID/idInterface.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Core::ub;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity,misc-use-internal-linkage)

namespace PocketCore::Test
{
	/*! @brief Distinguishes ability identifiers from all other stable identifier domains. */
	struct FixedMetadataIDTag;
	using FixedMetaDataID = PocketCore::ID::IDInterface<FixedMetadataIDTag, 0>;
	constexpr PocketCore::Core::us CAPACITY{1'000};

	enum class BuiltinFixedMetaDataID : ub
	{
		None,
		Test1,
		Test2,
		Test3,
	};

	constexpr std::string_view NONE_NAME{"None"};
	constexpr std::string_view TEST1_NAME{"Test1"};
	constexpr std::string_view TEST2_NAME{"Test2"};
	constexpr std::string_view TEST3_NAME{"Test3"};

	ATTR_NODISCARD constexpr FixedMetaDataID toFixedMetaDataID(const BuiltinFixedMetaDataID builtinFixedMetaDataID) noexcept
	{
		return FixedMetaDataID{static_cast<ub>(builtinFixedMetaDataID)};
	}

	struct Metadata
	{
		public:
			std::string_view mName{};
			FixedMetaDataID mID{};
	};

	class FixedRegistry : private PocketCore::Registry::FixedMetadataRegistry<Metadata, FixedMetaDataID, CAPACITY, &Metadata::mID>
	{
		private:
			using Base = PocketCore::Registry::FixedMetadataRegistry<Metadata, FixedMetaDataID, CAPACITY, &Metadata::mID>;

		public:
			ATTR_NOINLINE explicit constexpr FixedRegistry()
				: Base{static_cast<PocketCore::Core::us>(toFixedMetaDataID(BuiltinFixedMetaDataID::Test3).getValue() + 1U)}
			{
				addBuiltin({.mName = NONE_NAME, .mID = toFixedMetaDataID(BuiltinFixedMetaDataID::None)});
				addBuiltin({.mName = TEST1_NAME, .mID = toFixedMetaDataID(BuiltinFixedMetaDataID::Test1)});
				addBuiltin({.mName = TEST2_NAME, .mID = toFixedMetaDataID(BuiltinFixedMetaDataID::Test2)});
				addBuiltin({.mName = TEST3_NAME, .mID = toFixedMetaDataID(BuiltinFixedMetaDataID::Test3)});
			}

			using Base::addEntry;
			using Base::createCheckpoint;
			using Base::decrementAmountRegistered;
			using Base::findIndexByID;
			using Base::getAmountRegistered;
			using Base::getEntry;
			using Base::getID;
			using Base::getMetadata;
			using Base::getName;
			using Base::getNextID;
			using Base::getRegisteredEntries;
			using Base::hasEntry;
			using Base::incrementAmountRegistered;
			using Base::restoreCheckpoint;
			using Base::setAmountRegistered;
			using Base::setEntry;
	};

} // namespace PocketCore::Test

using PocketCore::Test::BuiltinFixedMetaDataID;
using PocketCore::Test::FixedMetaDataID;
using PocketCore::Test::FixedRegistry;
using PocketCore::Test::Metadata;
using PocketCore::Test::NONE_NAME;
using PocketCore::Test::TEST1_NAME;
using PocketCore::Test::TEST2_NAME;
using PocketCore::Test::TEST3_NAME;
using PocketCore::Test::toFixedMetaDataID;

SCENARIO("FixedMetadataRegistry")

{
	FixedRegistry registry{};

	GIVEN("a default-constructed registry")
	{
		THEN("all built-in abilities preserve their catalog identifiers")
		{
			CHECK((registry.getAmountRegistered() == 4));
			CHECK((registry.getNextID() == 4));

			std::optional<FixedMetaDataID> noneIdentifier{registry.getID(NONE_NAME)};
			std::optional<FixedMetaDataID> test1Identifier{registry.getID(TEST1_NAME)};
			std::optional<FixedMetaDataID> test2Identifier{registry.getID(TEST2_NAME)};

			REQUIRE(noneIdentifier.has_value());
			REQUIRE(test1Identifier.has_value());
			REQUIRE(test2Identifier.has_value());
			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((noneIdentifier.value() == toFixedMetaDataID(BuiltinFixedMetaDataID::None)));
			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((test1Identifier.value() == toFixedMetaDataID(BuiltinFixedMetaDataID::Test1)));
			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((test2Identifier.value() == toFixedMetaDataID(BuiltinFixedMetaDataID::Test2)));
		}

		THEN("Check name is set properly")
		{
			const Metadata *metadata{registry.getMetadata(toFixedMetaDataID(BuiltinFixedMetaDataID::Test1))};
			REQUIRE((metadata != nullptr));

			CHECK((metadata->mName == TEST1_NAME));
		}

		THEN("unknown names and IDs are absent")
		{
			CHECK_FALSE(registry.getID("Unknown").has_value());
			CHECK((registry.getMetadata(FixedMetaDataID{200}) == nullptr));
			CHECK_FALSE(registry.getName(FixedMetaDataID{200}).has_value());
		}

		THEN("an unknown stable ID has no internal index")
		{
			std::optional<ub> registryIndex{registry.findIndexByID(FixedMetaDataID{200})};
			CHECK_FALSE(registryIndex.has_value());
		}

		THEN("a registered registry name is returned by stable ID")
		{
			std::optional<std::string_view> registryName{registry.getName(toFixedMetaDataID(BuiltinFixedMetaDataID::Test2))};
			REQUIRE(registryName.has_value());
			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((registryName.value() == TEST2_NAME));
		}

		THEN("the registered span contains exactly the built-in entries")
		{
			std::span<const Metadata> abilities{registry.getRegisteredEntries()};
			REQUIRE((abilities.size() == 4U));
			CHECK((abilities.front().mName == NONE_NAME));
			CHECK((abilities.back().mName == TEST3_NAME));
			CHECK(registry.hasEntry(toFixedMetaDataID(BuiltinFixedMetaDataID::Test1)));
			CHECK(registry.hasEntry(TEST2_NAME));
		}

		THEN("the registered amount can be restored directly")
		{
			registry.setAmountRegistered(2);
			CHECK((registry.getAmountRegistered() == 2));
		}

		THEN("appended entries receive consecutive stable IDs")
		{
			FixedMetaDataID firstID{registry.addEntry(Metadata{.mName = "Custom 1"})};
			FixedMetaDataID secondID{registry.addEntry(Metadata{.mName = "Custom 2"})};
			CHECK((secondID.getValue() == firstID.getValue() + 1U));
		}

		THEN("restoring a checkpoint discards appended entries and restores ID assignment")
		{
			auto checkpoint{registry.createCheckpoint()};
			FixedMetaDataID assignedID{registry.addEntry(Metadata{.mName = "Custom"})};

			registry.restoreCheckpoint(checkpoint);

			CHECK((registry.getAmountRegistered() == 4U));
			CHECK_FALSE(registry.hasEntry(assignedID));
			CHECK((registry.addEntry(Metadata{.mName = "Replacement"}) == assignedID));
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity,misc-use-internal-linkage)
