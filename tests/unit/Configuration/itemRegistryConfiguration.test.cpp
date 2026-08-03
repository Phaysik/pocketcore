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

using PocketCore::Configuration::ItemRegistryConfiguration;
using PocketCore::Configuration::RegistryError;
using PocketCore::Effect::EffectTypeID;
using PocketCore::Item::BuiltinItemID;
using PocketCore::Item::ItemEffectTrigger;
using PocketCore::Item::ItemID;
using PocketCore::Item::ItemMeta;
using PocketCore::Item::ItemTargetID;
using PocketCore::Item::ItemTriggerID;
using PocketCore::Item::toItemID;
using PocketCore::Utility::Debug::Logging::Logger;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity,llvm-prefer-static-over-anonymous-namespace)

namespace
{
	void ensureItemLoggerInitialized()
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
			{.mEffects = {EffectTypeID::Recoil, EffectTypeID::StatusApply}, .mTrigger = ItemTriggerID::OnMoveUse},
		};
		ItemMeta definition{.mTriggers = triggers, .mName = "Life Orb"};

		WHEN("the item is added")
		{
			auto result{configuration.addItem(definition)};

			THEN("it receives the first custom stable ID and owns its trigger data")
			{
				REQUIRE(result.has_value());
				ItemID assignedIdentifier{result.value()};
				CHECK((assignedIdentifier.getValue() == 3U));

				triggers.clear();
				const ItemMeta *metadata{configuration.getItemMetadata(assignedIdentifier)};
				REQUIRE((metadata != nullptr));
				CHECK((metadata->mName == "Life Orb"));
				REQUIRE((metadata->mTriggers.size() == 1U));
				CHECK((metadata->mTriggers.front().mEffects.size() == 2U));
			}
		}
	}

	GIVEN("an item whose name is already registered")
	{
		ItemMeta definition{.mTriggers = {}, .mName = "Cheri Berry"};

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
		std::array<ItemMeta, 3> definitions{
			{
				{.mTriggers = {}, .mName = "First Custom"},
				{.mTriggers = {}, .mName = "Second Custom"},
				{.mTriggers = {}, .mName = "First Custom"},
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
		ItemMeta definition{.mTriggers = {}, .mName = "Custom Item"};
		auto addResult{configuration.addItem(definition)};
		REQUIRE(addResult.has_value());
		ItemID customIdentifier{addResult.value()};

		WHEN("its triggers are replaced by name")
		{
			std::array<ItemEffectTrigger, 1> replacement{
				{{.mEffects = {EffectTypeID::StatusRemove}, .mTrigger = ItemTriggerID::OnFaint}},
			};
			auto setResult{configuration.setItemTriggers("Custom Item", replacement)};

			THEN("the updated metadata is visible through its stable ID")
			{
				REQUIRE(setResult.has_value());
				const ItemMeta *metadata{configuration.getItemMetadata(customIdentifier)};
				REQUIRE((metadata != nullptr));
				CHECK((metadata->mTriggers.front().mTrigger == ItemTriggerID::OnFaint));
				CHECK((metadata->mTriggers.front().mEffects.front() == EffectTypeID::StatusRemove));
			}
		}

		WHEN("its triggers are replaced by stable ID")
		{
			std::array<ItemEffectTrigger, 1> replacement{
				{{.mEffects = {EffectTypeID::Flinch}, .mTrigger = ItemTriggerID::OnSwitchIn}},
			};
			auto setResult{configuration.setItemTriggers(customIdentifier, replacement)};

			THEN("the updated metadata remains associated with the same ID")
			{
				REQUIRE(setResult.has_value());
				const ItemMeta *metadata{configuration.getItemMetadata(customIdentifier)};
				REQUIRE((metadata != nullptr));
				CHECK((metadata->mTriggers.front().mTrigger == ItemTriggerID::OnSwitchIn));
			}
		}

		WHEN("its target is replaced by name")
		{
			auto setResult{configuration.setItemTarget("Custom Item", ItemTargetID::AllOpponents)};

			THEN("target metadata changes")
			{
				REQUIRE(setResult.has_value());
				const ItemMeta *metadata{configuration.getItemMetadata(customIdentifier)};
				REQUIRE((metadata != nullptr));
				CHECK((metadata->mTargetID == ItemTargetID::AllOpponents));
			}
		}

		WHEN("its target is replaced by stable ID")
		{
			auto setResult{configuration.setItemTarget(customIdentifier, ItemTargetID::SingleOpponent)};

			THEN("target metadata changes")
			{
				REQUIRE(setResult.has_value());
				const ItemMeta *metadata{configuration.getItemMetadata(customIdentifier)};
				REQUIRE((metadata != nullptr));
				CHECK((metadata->mTargetID == ItemTargetID::SingleOpponent));
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

		WHEN("it is updated by name")
		{
			ItemMeta replacement{
				.mTriggers = {{.mEffects = {EffectTypeID::StatusApply}, .mTrigger = ItemTriggerID::OnTurnEnd}},
				.mName = "Replacement Item",
				.mTargetID = ItemTargetID::Self,
			};
			auto updateResult{configuration.updateItem("Custom Item", replacement)};

			THEN("stored metadata is replaced")
			{
				REQUIRE(updateResult.has_value());
				CHECK(configuration.hasItem("Replacement Item"));
				CHECK_FALSE(configuration.hasItem("Custom Item"));
			}
		}

		WHEN("it is updated by stable ID")
		{
			ItemMeta replacement{
				.mTriggers = {{.mEffects = {EffectTypeID::Recoil}, .mTrigger = ItemTriggerID::OnMoveUse}},
				.mName = "Replacement Item By ID",
				.mTargetID = ItemTargetID::AllAllies,
			};
			auto updateResult{configuration.updateItem(customIdentifier, replacement)};

			THEN("stored metadata is replaced")
			{
				REQUIRE(updateResult.has_value());
				CHECK(configuration.hasItem("Replacement Item By ID"));
			}
		}

		WHEN("it is removed by name")
		{
			auto removeResult{configuration.removeItem("Custom Item")};

			THEN("the removed stable ID is returned")
			{
				REQUIRE(removeResult.has_value());
				CHECK((removeResult.value() == customIdentifier));
				CHECK_FALSE(configuration.hasItem(customIdentifier));
			}
		}

		WHEN("it is removed and another item is added")
		{
			auto removeResult{configuration.removeItem(customIdentifier)};
			REQUIRE(removeResult.has_value());

			ItemMeta laterDefinition{.mTriggers = {}, .mName = "Later Item"};
			auto laterResult{configuration.addItem(laterDefinition)};

			THEN("the removed ID is not reused")
			{
				REQUIRE(laterResult.has_value());
				CHECK((laterResult.value() != customIdentifier));
				CHECK((configuration.getItemMetadata(customIdentifier) == nullptr));
			}
		}
	}

	GIVEN("an unknown item")
	{
		THEN("metadata mutation and removal report ItemNotFound")
		{
			auto setResult{configuration.setItemTriggers("Missing", {})};
			auto setTargetResult{configuration.setItemTarget("Missing", ItemTargetID::Self)};
			auto updateResult{configuration.updateItem("Missing", {.mTriggers = {}, .mName = "Updated"})};
			auto removeResult{configuration.removeItem("Missing")};

			REQUIRE_FALSE(setResult.has_value());
			REQUIRE_FALSE(setTargetResult.has_value());
			REQUIRE_FALSE(updateResult.has_value());
			REQUIRE_FALSE(removeResult.has_value());
			CHECK((setResult.error().mKind == RegistryError::ItemNotFound));
			CHECK((setTargetResult.error().mKind == RegistryError::ItemNotFound));
			CHECK((updateResult.error().mKind == RegistryError::ItemNotFound));
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

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity,llvm-prefer-static-over-anonymous-namespace)
