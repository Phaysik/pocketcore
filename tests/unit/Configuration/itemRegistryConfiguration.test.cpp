#include "Configuration/itemRegistryConfiguration.h"

#include <array>
#include <optional>
#include <string_view>
#include <vector>

#include "Configuration/constants.h"
#include "Effect/effectType.h"
#include "Item/builtinItemID.h"
#include "Item/itemID.h"
#include "Item/itemMeta.h"
#include "Item/itemTargetsAndTriggers.h"
#include "Utility/Debug/Logging/logger.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Configuration::ItemDefinition;
using PocketCore::Configuration::ItemRegistryConfiguration;
using PocketCore::Configuration::RegistryError;
using PocketCore::Effect::EffectTypeID;
using PocketCore::Item::BuiltinItemID;
using PocketCore::Item::ItemEffectTrigger;
using PocketCore::Item::ItemID;
using PocketCore::Item::ItemTriggerID;
using PocketCore::Item::toItemID;
using PocketCore::Utility::Debug::Logging::Logger;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

namespace
{
	void ensureItemLoggerInitialized() // NOLINT(llvm-prefer-static-over-anonymous-namespace)
	{
		static bool initialized{false};

		if (!initialized)
		{
			initialized = Logger::initialize("irc_test", "itemRegistryConfiguration_test.log", true);
		}
	}
} // namespace

SCENARIO("ItemRegistryConfiguration addItem")
{
	ensureItemLoggerInitialized();
	ItemRegistryConfiguration configuration{};

	GIVEN("a unique item definition")
	{
		std::vector<ItemEffectTrigger> triggers{
			{.mTrigger = ItemTriggerID::OnMoveUse, .mEffects = {EffectTypeID::Recoil, EffectTypeID::StatusApply}},
		};
		ItemDefinition definition{.name = "Life Orb", .triggers = triggers};

		WHEN("the item is added")
		{
			auto result{configuration.addItem(definition)};

			THEN("it receives the first custom stable ID and owns its trigger data")
			{
				REQUIRE(result.has_value());
				ItemID assignedIdentifier{result.value()};
				CHECK((assignedIdentifier.getValue() == 3U));

				triggers.clear();
				auto metadata{configuration.getItemMetadata(assignedIdentifier)};
				REQUIRE(metadata.has_value());
				// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
				CHECK((metadata->get().mName == "Life Orb"));
				// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
				REQUIRE((metadata->get().mTriggers.size() == 1U));
				// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
				CHECK((metadata->get().mTriggers.front().mEffects.size() == 2U));
			}
		}
	}

	GIVEN("an item whose name is already registered")
	{
		ItemDefinition definition{.name = "Cheri Berry", .triggers = {}};

		THEN("registration reports a duplicate item")
		{
			auto result{configuration.addItem(definition)};
			REQUIRE_FALSE(result.has_value());
			CHECK((result.error().mKind == RegistryError::DuplicateItem));
			CHECK((configuration.getAmountRegistered() == 3));
		}
	}
}

SCENARIO("ItemRegistryConfiguration addItems")
{
	ensureItemLoggerInitialized();
	ItemRegistryConfiguration configuration{};

	GIVEN("a batch containing a duplicate name")
	{
		std::array<ItemDefinition, 3> definitions{
			{
				{.name = "First Custom", .triggers = {}},
				{.name = "Second Custom", .triggers = {}},
				{.name = "First Custom", .triggers = {}},
			},
		};

		WHEN("the batch is added")
		{
			auto result{configuration.addItems(definitions)};

			THEN("the entire batch is rolled back")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::DuplicateItem));
				CHECK((configuration.getAmountRegistered() == 3));
				CHECK_FALSE(configuration.hasItem("First Custom"));
				CHECK_FALSE(configuration.hasItem("Second Custom"));
			}
		}
	}
}

SCENARIO("ItemRegistryConfiguration metadata lifecycle")
{
	ensureItemLoggerInitialized();
	ItemRegistryConfiguration configuration{};

	GIVEN("a registered custom item")
	{
		ItemDefinition definition{.name = "Custom Item", .triggers = {}};
		auto addResult{configuration.addItem(definition)};
		REQUIRE(addResult.has_value());
		ItemID customIdentifier{addResult.value()};

		WHEN("its triggers are replaced by name")
		{
			std::array<ItemEffectTrigger, 1> replacement{
				{{.mTrigger = ItemTriggerID::OnFaint, .mEffects = {EffectTypeID::StatusRemove}}},
			};
			auto setResult{configuration.setItemTriggers("Custom Item", replacement)};

			THEN("the updated metadata is visible through its stable ID")
			{
				REQUIRE(setResult.has_value());
				auto metadata{configuration.getItemMetadata(customIdentifier)};
				REQUIRE(metadata.has_value());
				// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
				CHECK((metadata->get().mTriggers.front().mTrigger == ItemTriggerID::OnFaint));
				// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
				CHECK((metadata->get().mTriggers.front().mEffects.front() == EffectTypeID::StatusRemove));
			}
		}

		WHEN("its triggers are replaced by stable ID")
		{
			std::array<ItemEffectTrigger, 1> replacement{
				{{.mTrigger = ItemTriggerID::OnSwitchIn, .mEffects = {EffectTypeID::Flinch}}},
			};
			auto setResult{configuration.setItemTriggers(customIdentifier, replacement)};

			THEN("the updated metadata remains associated with the same ID")
			{
				REQUIRE(setResult.has_value());
				auto metadata{configuration.getItemMetadata(customIdentifier)};
				REQUIRE(metadata.has_value());
				// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
				CHECK((metadata->get().mTriggers.front().mTrigger == ItemTriggerID::OnSwitchIn));
			}
		}

		WHEN("it is renamed")
		{
			auto renameResult{configuration.renameItem("Custom Item", "Renamed Item")};

			THEN("the stable ID remains unchanged")
			{
				REQUIRE(renameResult.has_value());
				CHECK_FALSE(configuration.hasItem("Custom Item"));
				auto renamedIdentifier{configuration.getItemID("Renamed Item")};
				REQUIRE(renamedIdentifier.has_value());
				// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
				CHECK((renamedIdentifier.value() == customIdentifier));
			}
		}

		WHEN("it is removed and another item is added")
		{
			auto removeResult{configuration.removeItem(customIdentifier)};
			REQUIRE(removeResult.has_value());

			ItemDefinition laterDefinition{.name = "Later Item", .triggers = {}};
			auto laterResult{configuration.addItem(laterDefinition)};

			THEN("the removed ID is not reused")
			{
				REQUIRE(laterResult.has_value());
				CHECK((laterResult.value() != customIdentifier));
				CHECK_FALSE(configuration.getItemMetadata(customIdentifier).has_value());
			}
		}
	}

	GIVEN("an unknown item")
	{
		THEN("metadata mutation and removal report ItemNotFound")
		{
			auto setResult{configuration.setItemTriggers("Missing", {})};
			auto removeResult{configuration.removeItem("Missing")};

			REQUIRE_FALSE(setResult.has_value());
			REQUIRE_FALSE(removeResult.has_value());
			CHECK((setResult.error().mKind == RegistryError::ItemNotFound));
			CHECK((removeResult.error().mKind == RegistryError::ItemNotFound));
		}
	}
}

SCENARIO("ItemRegistryConfiguration queries")
{
	ensureItemLoggerInitialized();
	ItemRegistryConfiguration configuration{};

	GIVEN("the built-in item registry")
	{
		THEN("item IDs names spans and predicates are exposed")
		{
			auto chestoIdentifier{configuration.getItemID("Chesto Berry")};
			REQUIRE(chestoIdentifier.has_value());
			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((chestoIdentifier.value() == toItemID(BuiltinItemID::ChestoBerry)));
			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((configuration.getItemName(chestoIdentifier.value()) == "Chesto Berry"));
			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK(configuration.hasItem(chestoIdentifier.value()));
			CHECK((configuration.getRegisteredItems().size() == 3U));
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
