/*! @file fixedMetadataRegistry.testHelper.h
	@brief Test helper for dealing with FixedMetadataRegistry concepts.
	@date 09/11/2026
	@since 0.12.22
	@version 0.12.22
	@author Matthew Moore
*/

#ifndef TEST_INCLUDE_REGISTRY_FIXED_METADATA_REGISTRY_TEST_HELPER_H
#define TEST_INCLUDE_REGISTRY_FIXED_METADATA_REGISTRY_TEST_HELPER_H

#include <string>
#include <string_view>

#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "ID/idInterface.h"
#include "Registry/fixedMetadataRegistry.h"

namespace PocketCore::Testing
{
	using PocketCore::Core::ub;

	struct FixedMetadataIDTag;
	using FixedMetaDataID = PocketCore::ID::IDInterface<FixedMetadataIDTag, 0>;
	inline constexpr FixedMetaDataID NO_ID{};
	constexpr PocketCore::Core::us CAPACITY{1'000};

	enum class BuiltinFixedMetaDataID : ub
	{
		None,
		Test1,
		Test2,
		Test3,
		Final,
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
			ATTR_NODISCARD constexpr bool operator==(const Metadata &other) const noexcept = default;

			// NOLINTBEGIN(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)

			std::string mName{};

			FixedMetaDataID mID{};
			// NOLINTEND(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)
	};

	class FixedRegistry : private PocketCore::Registry::FixedMetadataRegistry<Metadata, FixedMetaDataID, CAPACITY, &Metadata::mID>
	{
		private:
			using Base = PocketCore::Registry::FixedMetadataRegistry<Metadata, FixedMetaDataID, CAPACITY, &Metadata::mID>;

		public:
			ATTR_NOINLINE explicit constexpr FixedRegistry()
				: Base{static_cast<PocketCore::Core::us>(toFixedMetaDataID(BuiltinFixedMetaDataID::Test3).getValue() + 1U)}
			{
				addBuiltin({.mName = std::string(NONE_NAME), .mID = toFixedMetaDataID(BuiltinFixedMetaDataID::None)});
				addBuiltin({.mName = std::string(TEST1_NAME), .mID = toFixedMetaDataID(BuiltinFixedMetaDataID::Test1)});
				addBuiltin({.mName = std::string(TEST2_NAME), .mID = toFixedMetaDataID(BuiltinFixedMetaDataID::Test2)});
				addBuiltin({.mName = std::string(TEST3_NAME), .mID = toFixedMetaDataID(BuiltinFixedMetaDataID::Test3)});
			}

		protected:
			using Base::addEntry;
			using Base::createCheckpoint;
			using Base::decrementAmountRegistered;
			using Base::eraseEntry;
			using Base::incrementAmountRegistered;
			using Base::restoreCheckpoint;
			using Base::setAmountRegistered;
			using Base::setEntry;

		public:
			using Base::findIndexByID;
			using Base::getAmountRegistered;
			using Base::getEntry;
			using Base::getID;
			using Base::getMetadata;
			using Base::getName;
			using Base::getNextID;
			using Base::getRegisteredEntries;
			using Base::hasEntry;
	};

	class CheckpointRegistry final : public FixedRegistry
	{
		public:
			constexpr CheckpointRegistry() : FixedRegistry{} {}

			using FixedRegistry::createCheckpoint;
			using FixedRegistry::eraseEntry;
			using FixedRegistry::restoreCheckpoint;
	};
} // namespace PocketCore::Testing

#endif
