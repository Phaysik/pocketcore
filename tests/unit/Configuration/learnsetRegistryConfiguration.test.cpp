/*! @file learnsetRegistryConfiguration.test.cpp
	@brief C++ file for running tests for the LearnsetRegistryConfiguration.
	@date 10/08/2026
	@since 0.12.50
	@version 0.12.50
	@author Matthew Moore
*/

#include "Configuration/learnsetRegistryConfiguration.h"

#include <expected>
#include <format>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "Configuration/constants.h"
#include "Core/typedefs.h"
#include "Learnset/builtInLearnsetID.h"
#include "Learnset/constants.h"
#include "Learnset/learnsetID.h"
#include "Learnset/learnsetMeta.h"
#include "Move/builtInMoveID.h"
#include "Registry/learnsetRegistry.h"
#include "Registry/registryError.h"
#include "Utility/Debug/Logging/logging.testHelper.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Configuration::LearnsetRegistryConfiguration;
using PocketCore::Configuration::MAX_LEARNSETS;
using PocketCore::Configuration::RegistryError;
using PocketCore::Core::ub;
using PocketCore::Core::us;
using PocketCore::Learnset::AcquisitionMethod;
using PocketCore::Learnset::BuiltinLearnsetID;
using PocketCore::Learnset::LEARNSET_NAME_BULBASAUR;
using PocketCore::Learnset::LEARNSET_NAME_NONE;
using PocketCore::Learnset::LearnsetID;
using PocketCore::Learnset::LearnsetMeta;
using PocketCore::Learnset::NO_LEARNSET_ID;
using PocketCore::Learnset::toLearnsetID;
using PocketCore::Move::BuiltinMoveID;
using PocketCore::Move::toMoveID;
using PocketCore::Registry::Learnset::LearnsetRegistry;
using PocketCore::Registry::RegistryErrorInfo;
using PocketCore::Testing::ensureLoggerInitialized;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("LearnsetRegistryConfiguration")
{
	ensureLoggerInitialized("learnset registry configuration test", "learnsetRegistryConfiguration_test.log");

	LearnsetRegistryConfiguration config{};
	LearnsetRegistry registry{};
	ub finalLearnsetUnderlyingValue{std::to_underlying(BuiltinLearnsetID::FinalLearnset)};

	GIVEN("getAmountRegistered")
	{
		THEN("the default registry returns the expected amount of built-in learnsets")
		{
			CHECK((config.getAmountRegistered() == finalLearnsetUnderlyingValue));
		}
	}

	GIVEN("getRuntimeRegistry")
	{
		THEN("the runtime registry is the same as the default registry")
		{
			CHECK((config.getRuntimeRegistry() == registry));
		}
	}

	GIVEN("getLearnsetMetadata")
	{
		THEN("unknown IDs are absent")
		{
			CHECK((config.getLearnsetMetadata(LearnsetID{200}) == nullptr));
		}

		THEN("the metadata is retrieved when accessed by a valid Learnset ID")
		{
			LearnsetMeta expected{
				.mName = std::string(LEARNSET_NAME_NONE),
				.mLearnsetID = toLearnsetID(BuiltinLearnsetID::None),
			};

			CHECK((expected == *config.getLearnsetMetadata(NO_LEARNSET_ID)));
		}
	}

	GIVEN("getLearnsetID")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(config.getLearnsetID("Unknown").has_value());
		}

		THEN("the Learnset ID is retrieved by valid Learnset name")
		{
			std::optional<LearnsetID> learnsetID{config.getLearnsetID(LEARNSET_NAME_NONE)};

			REQUIRE(learnsetID.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((learnsetID.value() == toLearnsetID(BuiltinLearnsetID::None)));
		}
	}

	GIVEN("getLearnsetName")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(config.getLearnsetName(LearnsetID{200}).has_value());
		}

		THEN("a registered learnset name is returned by stable ID")
		{
			std::optional<std::string_view> learnsetName{config.getLearnsetName(toLearnsetID(BuiltinLearnsetID::None))};

			REQUIRE(learnsetName.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((learnsetName.value() == LEARNSET_NAME_NONE));
		}
	}

	GIVEN("getRegisteredLearnsets")
	{
		THEN("the amount of learnsets returned matches the amount that are built-in")
		{
			CHECK((config.getRegisteredLearnsets().size() == finalLearnsetUnderlyingValue));
		}
	}

	GIVEN("hasLearnset")
	{
		WHEN("calling the string_view overload")
		{
			THEN("an unknown learnset name has no entry")
			{
				CHECK_FALSE(config.hasLearnset("Unknown"));
			}

			THEN("a known learnset name has an entry")
			{
				CHECK(config.hasLearnset(LEARNSET_NAME_NONE));
			}
		}

		WHEN("calling the LearnsetID overload")
		{
			THEN("an unknown learnset ID has no entry")
			{
				CHECK_FALSE(config.hasLearnset(LearnsetID{200}));
			}

			THEN("a known learnset ID has an entry")
			{
				CHECK(config.hasLearnset(NO_LEARNSET_ID));
			}
		}
	}

	GIVEN("addLearnset")
	{
		WHEN("trying to add an learnset past the capacity")
		{
			us newLearnsetCount{finalLearnsetUnderlyingValue};

			for (us i{0}; i < MAX_LEARNSETS - finalLearnsetUnderlyingValue; ++i)
			{
				std::string name{std::format("String_{:04}", i)};
				LearnsetMeta definition{.mName = name};
				std::expected<LearnsetID, RegistryErrorInfo> result{config.addLearnset(definition)};

				REQUIRE(result.has_value());

				LearnsetID assignedID{result.value()};
				CHECK((assignedID.getValue() == newLearnsetCount++));
			}

			std::string name{std::format("String_{:04}", MAX_LEARNSETS + 1)};

			LearnsetMeta definition{.mName = name};
			std::expected<LearnsetID, RegistryErrorInfo> result{config.addLearnset(definition)};

			THEN("registration reports a max capacity and nothing is added")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::MaxCapacity));
				CHECK((config.getAmountRegistered() == MAX_LEARNSETS));
			}
		}

		WHEN("an learnset whose name is already in use is added")
		{
			LearnsetMeta definition{.mName = std::string(LEARNSET_NAME_BULBASAUR)};
			std::expected<LearnsetID, RegistryErrorInfo> result{config.addLearnset(definition)};

			THEN("registration reports a duplicate learnset and nothing is added")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::DuplicateLearnset));
				CHECK((config.getAmountRegistered() == finalLearnsetUnderlyingValue));
			}
		}

		WHEN("a unique learnset definition is added")
		{
			LearnsetMeta definition{
				.mName = "TestLearnsetName",
				.mEntries
				= {{.mMoveID = toMoveID(BuiltinMoveID::Facade), .mLevelRequirement = 20, .mAcquisitionMethod = AcquisitionMethod::TM}},
			};

			std::expected<LearnsetID, RegistryErrorInfo> result{config.addLearnset(definition)};

			THEN("it receives the first custom stable ID and owns its trigger data")
			{
				REQUIRE(result.has_value());
				LearnsetID assignedID{result.value()};
				CHECK((assignedID.getValue() == finalLearnsetUnderlyingValue));

				const LearnsetMeta *metadata{config.getLearnsetMetadata(assignedID)};

				REQUIRE((metadata != nullptr));
				definition.mLearnsetID = assignedID;
				CHECK((*metadata == definition));
			}
		}
	}

	GIVEN("addLearnsets")
	{
		WHEN("trying to add an learnset past the capacity")
		{
			std::vector<LearnsetMeta> learnsetMetas;

			for (us i{0}; i < MAX_LEARNSETS - finalLearnsetUnderlyingValue; ++i)
			{
				std::string name{std::format("String_{:04}", i)};
				learnsetMetas.push_back({.mName = name});
			}

			std::string name{std::format("String_{:04}", MAX_LEARNSETS + 1)};

			learnsetMetas.push_back({.mName = name});
			std::expected<void, RegistryErrorInfo> result{config.addLearnsets(learnsetMetas)};

			THEN("registration reports a max capacity and nothing is added")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::MaxCapacity));
				CHECK((config.getAmountRegistered() == finalLearnsetUnderlyingValue));
			}
		}

		WHEN("a learnset whose name is already in use is added")
		{
			std::vector<LearnsetMeta> learnsetMetas{};

			for (us i{0}; i < 20; ++i)
			{
				std::string name{std::format("String_{:04}", i)};
				learnsetMetas.push_back({.mName = name});
			}

			learnsetMetas.push_back({.mName = std::string(LEARNSET_NAME_BULBASAUR)});
			const LearnsetRegistry beforeBatch{config.getRuntimeRegistry()};
			std::expected<void, RegistryErrorInfo> result{config.addLearnsets(learnsetMetas)};

			THEN("registration reports a duplicate learnset and the registry is rollback to the checkpoint before the erroneous addition")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::DuplicateLearnset));
				CHECK((config.getAmountRegistered() == finalLearnsetUnderlyingValue));
				CHECK((config.getRuntimeRegistry() == beforeBatch));
				CHECK((config.getRuntimeRegistry().getNextLearnsetID() == beforeBatch.getNextLearnsetID()));
				CHECK_FALSE(config.hasLearnset("String_0000"));
			}
		}

		WHEN("a unique learnset definition is added")
		{
			std::vector<LearnsetMeta> learnsetMetas{};

			learnsetMetas.push_back({.mName = "TestLearnsetName"});

			std::expected<void, RegistryErrorInfo> result{config.addLearnsets(learnsetMetas)};

			THEN("the registry reports no error and all the learnset definitions are added")
			{
				REQUIRE(result.has_value());
				CHECK((config.getAmountRegistered() == finalLearnsetUnderlyingValue + 1));
			}
		}
	}

	GIVEN("renameLearnset")
	{
		WHEN("calling with an invalid learnset name")
		{
			std::expected<void, RegistryErrorInfo> result{config.renameLearnset("ThisIsInvalid", "NewName")};

			THEN("the registry reports an error and there is no update to the registry")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::LearnsetNotFound));
				CHECK((config.getAmountRegistered() == finalLearnsetUnderlyingValue));
			}
		}

		WHEN("trying to rename to a name already in the registry")
		{
			std::expected<void, RegistryErrorInfo> result{
				config.renameLearnset(LEARNSET_NAME_NONE, LEARNSET_NAME_BULBASAUR),
			};

			THEN("the registry reports an error and there is no update to the registry")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::DuplicateLearnset));
				CHECK((config.getAmountRegistered() == finalLearnsetUnderlyingValue));
			}
		}

		WHEN("updating an existing learnset definition")
		{
			std::expected<void, RegistryErrorInfo> result{config.renameLearnset(LEARNSET_NAME_NONE, "NewName")};

			THEN("the registry reports no error and the name is appropriately updated")
			{
				REQUIRE(result.has_value());
				CHECK((config.getAmountRegistered() == finalLearnsetUnderlyingValue));
				CHECK((config.getLearnsetMetadata(toLearnsetID(BuiltinLearnsetID::None))->mName == "NewName"));
			}
		}
	}

	GIVEN("updateLearnset")
	{
		LearnsetMeta definition{.mName = "TestLearnsetName"};

		WHEN("calling the string_view overload")
		{
			WHEN("calling with an invalid learnset name")
			{
				std::expected<void, RegistryErrorInfo> result{config.updateLearnset("ThisIsInvalid", definition)};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::LearnsetNotFound));
					CHECK((config.getAmountRegistered() == finalLearnsetUnderlyingValue));
				}
			}

			WHEN("trying to rename to a name already in the registry")
			{
				std::expected<void, RegistryErrorInfo> result{
					config.updateLearnset(LEARNSET_NAME_NONE, {.mName = std::string(LEARNSET_NAME_BULBASAUR)}),
				};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::DuplicateLearnset));
					CHECK((config.getAmountRegistered() == finalLearnsetUnderlyingValue));
				}
			}

			WHEN("updating an existing learnset definition")
			{
				std::expected<void, RegistryErrorInfo> result{config.updateLearnset(LEARNSET_NAME_NONE, definition)};

				THEN("the registry reports no error and the target is appropriately updated")
				{
					REQUIRE(result.has_value());
					CHECK((config.getAmountRegistered() == finalLearnsetUnderlyingValue));
					CHECK((config.getLearnsetMetadata(toLearnsetID(BuiltinLearnsetID::None))->mName == "TestLearnsetName"));
				}
			}
		}

		WHEN("calling the LearnsetID overload")
		{
			WHEN("calling with an invalid learnset name")
			{
				std::expected<void, RegistryErrorInfo> result{config.updateLearnset(LearnsetID{200}, definition)};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::LearnsetNotFound));
					CHECK((config.getAmountRegistered() == finalLearnsetUnderlyingValue));
				}
			}

			WHEN("trying to rename to a name already in the registry")
			{
				std::expected<void, RegistryErrorInfo> result{
					config.updateLearnset(toLearnsetID(BuiltinLearnsetID::None), {.mName = std::string(LEARNSET_NAME_BULBASAUR)}),
				};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::DuplicateLearnset));
					CHECK((config.getAmountRegistered() == finalLearnsetUnderlyingValue));
				}
			}

			WHEN("updating an existing learnset definition")
			{
				std::expected<void, RegistryErrorInfo> result{config.updateLearnset(toLearnsetID(BuiltinLearnsetID::None), definition)};

				THEN("the registry reports no error and the target is appropriately updated")
				{
					REQUIRE(result.has_value());
					CHECK((config.getAmountRegistered() == finalLearnsetUnderlyingValue));
					CHECK((config.getLearnsetMetadata(toLearnsetID(BuiltinLearnsetID::None))->mName == "TestLearnsetName"));
				}
			}
		}
	}

	GIVEN("removeLearnset")
	{
		WHEN("calling the string_view overload")
		{
			WHEN("calling with an unknown learnset name")
			{
				std::expected<LearnsetID, RegistryErrorInfo> result{config.removeLearnset("Unknown")};

				THEN("the result is an error and the registry is not updated")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::LearnsetNotFound));
					CHECK((config.getAmountRegistered() == finalLearnsetUnderlyingValue));
				}
			}

			WHEN("calling with a known learnset name")
			{
				std::expected<LearnsetID, RegistryErrorInfo> result{config.removeLearnset(LEARNSET_NAME_NONE)};

				THEN("the result is a success and the registry is updated")
				{
					REQUIRE(result.has_value());
					CHECK((result.value() == toLearnsetID(BuiltinLearnsetID::None)));
					CHECK((config.getAmountRegistered() == finalLearnsetUnderlyingValue - 1));
				}
			}
		}

		WHEN("calling the LearnsetID overload")
		{
			WHEN("calling with an unknown learnset ID")
			{
				std::expected<LearnsetID, RegistryErrorInfo> result{config.removeLearnset(LearnsetID{200})};

				THEN("the result is an error and the registry is not updated")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::LearnsetNotFound));
					CHECK((config.getAmountRegistered() == finalLearnsetUnderlyingValue));
				}
			}

			WHEN("calling with a known learnset ID")
			{
				std::expected<LearnsetID, RegistryErrorInfo> result{config.removeLearnset(toLearnsetID(BuiltinLearnsetID::None))};

				THEN("the result is a success and the registry is updated")
				{
					REQUIRE(result.has_value());
					CHECK((result.value() == toLearnsetID(BuiltinLearnsetID::None)));
					CHECK((config.getAmountRegistered() == finalLearnsetUnderlyingValue - 1));
				}
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
