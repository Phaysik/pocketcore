/*! @file itemRegistryConfiguration.test.cpp
	@brief C++ file for running tests for the ItemRegistryConfiguration.
	@date 09/10/2026
	@since 0.5.0
	@version 0.12.20
	@author Matthew Moore
*/

#include "Configuration/itemRegistryConfiguration.h"

#include <expected>
#include <format>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "Battle/battleTargetsAndTriggers.h"
#include "Configuration/constants.h"
#include "Core/typedefs.h"
#include "Effect/builtInEffectID.h"
#include "Effect/effectTrigger.h"
#include "Item/builtInItemID.h"
#include "Item/constants.h"
#include "Item/itemID.h"
#include "Item/itemMeta.h"
#include "Registry/itemRegistry.h"
#include "Registry/registryError.h"
#include "Utility/Debug/Logging/logging.testHelper.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Battle::BattleEventID;
using PocketCore::Battle::BattleEventRole;
using PocketCore::Battle::BattleTargetID;
using PocketCore::Configuration::ItemRegistryConfiguration;
using PocketCore::Configuration::MAX_ITEMS;
using PocketCore::Configuration::RegistryError;
using PocketCore::Core::ub;
using PocketCore::Core::us;
using PocketCore::Effect::BuiltinEffectID;
using PocketCore::Effect::EffectTrigger;
using PocketCore::Effect::toEffectID;
using PocketCore::Item::BuiltinItemID;
using PocketCore::Item::ITEM_NAME_CHERI_BERRY;
using PocketCore::Item::ITEM_NAME_NONE;
using PocketCore::Item::ItemID;
using PocketCore::Item::ItemMeta;
using PocketCore::Item::NO_ITEM_ID;
using PocketCore::Item::toItemID;
using PocketCore::Registry::Item::ItemRegistry;
using PocketCore::Registry::RegistryErrorInfo;
using PocketCore::Testing::ensureLoggerInitialized;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("ItemRegistryConfiguration")
{
	ensureLoggerInitialized("item registry configuration test", "itemRegistryConfiguration_test.log");

	ItemRegistryConfiguration config{};
	ItemRegistry registry{};
	ub finalItemUnderlyingValue{std::to_underlying(BuiltinItemID::FinalItem)};

	GIVEN("getAmountRegistered")
	{
		THEN("the default registry returns the expected amount of built-in items")
		{
			CHECK((config.getAmountRegistered() == finalItemUnderlyingValue));
		}
	}

	GIVEN("getRuntimeRegistry")
	{
		THEN("the runtime registry is the same as the default registry")
		{
			CHECK((config.getRuntimeRegistry() == registry));
		}
	}

	GIVEN("getItemMetadata")
	{
		THEN("unknown IDs are absent")
		{
			CHECK((config.getItemMetadata(ItemID{200}) == nullptr));
		}

		THEN("the metadata is retrieved when accessed by a valid Item ID")
		{
			ItemMeta expected{
				.mName = std::string(ITEM_NAME_NONE),
				.mTriggers = {},
				.mItemID = toItemID(BuiltinItemID::None),
			};

			CHECK((expected == *config.getItemMetadata(NO_ITEM_ID)));
		}
	}

	GIVEN("getItemID")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(config.getItemID("Unknown").has_value());
		}

		THEN("the Item ID is retrieved by valid Item name")
		{
			std::optional<ItemID> itemID{config.getItemID(ITEM_NAME_NONE)};

			REQUIRE(itemID.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((itemID.value() == toItemID(BuiltinItemID::None)));
		}
	}

	GIVEN("getItemName")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(config.getItemName(ItemID{200}).has_value());
		}

		THEN("a registered item name is returned by stable ID")
		{
			std::optional<std::string_view> itemName{config.getItemName(toItemID(BuiltinItemID::None))};

			REQUIRE(itemName.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((itemName.value() == ITEM_NAME_NONE));
		}
	}

	GIVEN("getRegisteredItems")
	{
		THEN("the amount of items returned matches the amount that are built-in")
		{
			CHECK((config.getRegisteredItems().size() == finalItemUnderlyingValue));
		}
	}

	GIVEN("hasItem")
	{
		WHEN("calling the string_view overload")
		{
			THEN("an unknown item name has no entry")
			{
				CHECK_FALSE(config.hasItem("Unknown"));
			}

			THEN("a known item name has an entry")
			{
				CHECK(config.hasItem(ITEM_NAME_NONE));
			}
		}

		WHEN("calling the ItemID overload")
		{
			THEN("an unknown item ID has no entry")
			{
				CHECK_FALSE(config.hasItem(ItemID{200}));
			}

			THEN("a known item ID has an entry")
			{
				CHECK(config.hasItem(NO_ITEM_ID));
			}
		}
	}

	GIVEN("addItem")
	{
		WHEN("trying to add an item past the capacity")
		{
			us newItemCount{finalItemUnderlyingValue};

			for (us i{0}; i < MAX_ITEMS - finalItemUnderlyingValue; ++i)
			{
				std::string name{std::format("String_{:04}", i)};
				ItemMeta definition{.mName = name, .mTriggers = {}};
				std::expected<ItemID, RegistryErrorInfo> result{config.addItem(definition)};

				REQUIRE(result.has_value());

				ItemID assignedID{result.value()};
				CHECK((assignedID.getValue() == newItemCount++));
			}

			std::string name{std::format("String_{:04}", MAX_ITEMS + 1)};

			ItemMeta definition{.mName = name, .mTriggers = {}};
			std::expected<ItemID, RegistryErrorInfo> result{config.addItem(definition)};

			THEN("registration reports a max capacity and nothing is added")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::MaxCapacity));
				CHECK((config.getAmountRegistered() == MAX_ITEMS));
			}
		}

		WHEN("an item whose name is already in use is added")
		{
			ItemMeta definition{.mName = std::string(ITEM_NAME_CHERI_BERRY), .mTriggers = {}};
			std::expected<ItemID, RegistryErrorInfo> result{config.addItem(definition)};

			THEN("registration reports a duplicate item and nothing is added")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::DuplicateItem));
				CHECK((config.getAmountRegistered() == finalItemUnderlyingValue));
			}
		}

		WHEN("a unique item definition is added")
		{
			std::vector<EffectTrigger> triggers{
				{
					.mEffects = {toEffectID(BuiltinEffectID::Recoil), toEffectID(BuiltinEffectID::StatusTick)},
					.mTrigger = BattleEventID::TurnEnd,
				},
			};

			ItemMeta definition{.mName = "TestItemName", .mTriggers = triggers};

			std::expected<ItemID, RegistryErrorInfo> result{config.addItem(definition)};

			THEN("it receives the first custom stable ID and owns its trigger data")
			{
				REQUIRE(result.has_value());
				ItemID assignedID{result.value()};
				CHECK((assignedID.getValue() == finalItemUnderlyingValue));

				triggers.clear();

				const ItemMeta *metadata{config.getItemMetadata(assignedID)};

				REQUIRE((metadata != nullptr));
				CHECK((metadata->mName == "TestItemName"));
				REQUIRE((metadata->mTriggers.size() == 1U));
				CHECK((metadata->mTriggers.front().mEffects.size() == 2U));
			}
		}
	}

	GIVEN("addItems")
	{
		WHEN("trying to add an item past the capacity")
		{
			std::vector<ItemMeta> itemMetas;

			for (us i{0}; i < MAX_ITEMS - finalItemUnderlyingValue; ++i)
			{
				std::string name{std::format("String_{:04}", i)};
				itemMetas.push_back({.mName = name, .mTriggers = {}});
			}

			std::string name{std::format("String_{:04}", MAX_ITEMS + 1)};

			itemMetas.push_back({.mName = name, .mTriggers = {}});
			std::expected<void, RegistryErrorInfo> result{config.addItems(itemMetas)};

			THEN("registration reports a max capacity and nothing is added")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::MaxCapacity));
				CHECK((config.getAmountRegistered() == finalItemUnderlyingValue));
			}
		}

		WHEN("an item whose name is already in use is added")
		{
			std::vector<ItemMeta> itemMetas{};

			for (us i{0}; i < 20; ++i)
			{
				std::string name{std::format("String_{:04}", i)};
				itemMetas.push_back({.mName = name, .mTriggers = {}});
			}

			itemMetas.push_back({.mName = std::string(ITEM_NAME_CHERI_BERRY), .mTriggers = {}});
			std::expected<void, RegistryErrorInfo> result{config.addItems(itemMetas)};

			THEN("registration reports a duplicate item and the registry is rollback to the checkpoint before the erroneous addition")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::DuplicateItem));
				CHECK((config.getAmountRegistered() == finalItemUnderlyingValue));
			}
		}

		WHEN("a unique item definition is added")
		{
			std::vector<ItemMeta> itemMetas{};
			std::vector<EffectTrigger> triggers{
				{
					.mEffects = {toEffectID(BuiltinEffectID::Recoil), toEffectID(BuiltinEffectID::StatusTick)},
					.mTrigger = BattleEventID::TurnEnd,
				},
			};

			itemMetas.push_back({.mName = "TestItemName", .mTriggers = triggers});

			std::expected<void, RegistryErrorInfo> result{config.addItems(itemMetas)};

			THEN("the registry reports no error and all the item definitions are added")
			{
				REQUIRE(result.has_value());
				CHECK((config.getAmountRegistered() == finalItemUnderlyingValue + 1));
			}
		}
	}

	GIVEN("setItemTriggers")
	{
		WHEN("calling the string_view overload")
		{
			WHEN("calling with an invalid item name")
			{
				std::vector<EffectTrigger> triggers{
					{
						.mEffects = {toEffectID(BuiltinEffectID::Recoil), toEffectID(BuiltinEffectID::StatusTick)},
						.mTrigger = BattleEventID::TurnEnd,
					},
				};

				std::expected<void, RegistryErrorInfo> result{config.setItemTriggers("ThisIsInvalid", triggers)};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::ItemNotFound));
					CHECK((config.getAmountRegistered() == finalItemUnderlyingValue));
				}
			}

			WHEN("updating an existing item definition")
			{
				std::vector<EffectTrigger> triggers{
					{
						.mEffects = {toEffectID(BuiltinEffectID::Recoil), toEffectID(BuiltinEffectID::StatusTick)},
						.mTrigger = BattleEventID::TurnEnd,
					},
				};

				std::expected<void, RegistryErrorInfo> result{config.setItemTriggers(ITEM_NAME_NONE, triggers)};

				THEN("the registry reports no error and the triggers are appropriately updated")
				{
					REQUIRE(result.has_value());
					CHECK((config.getAmountRegistered() == finalItemUnderlyingValue));
					CHECK((config.getItemMetadata(toItemID(BuiltinItemID::None))->mTriggers == triggers));
				}
			}
		}

		WHEN("calling the ItemID overload")
		{
			WHEN("calling with an invalid item name")
			{
				std::vector<EffectTrigger> triggers{
					{
						.mEffects = {toEffectID(BuiltinEffectID::Recoil), toEffectID(BuiltinEffectID::StatusTick)},
						.mTrigger = BattleEventID::TurnEnd,
					},
				};

				std::expected<void, RegistryErrorInfo> result{config.setItemTriggers(ItemID{200}, triggers)};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::ItemNotFound));
					CHECK((config.getAmountRegistered() == finalItemUnderlyingValue));
				}
			}

			WHEN("updating an existing item definition")
			{
				std::vector<EffectTrigger> triggers{
					{
						.mEffects = {toEffectID(BuiltinEffectID::Recoil), toEffectID(BuiltinEffectID::StatusTick)},
						.mTrigger = BattleEventID::TurnEnd,
					},
				};

				std::expected<void, RegistryErrorInfo> result{config.setItemTriggers(toItemID(BuiltinItemID::None), triggers)};

				THEN("the registry reports no error and the triggers are appropriately updated")
				{
					REQUIRE(result.has_value());
					CHECK((config.getAmountRegistered() == finalItemUnderlyingValue));
					CHECK((config.getItemMetadata(toItemID(BuiltinItemID::None))->mTriggers == triggers));
				}
			}
		}
	}

	GIVEN("setItemTarget")
	{
		WHEN("calling the string_view overload")
		{
			WHEN("calling with an invalid item name")
			{
				std::expected<void, RegistryErrorInfo> result{config.setItemTarget("ThisIsInvalid", BattleTargetID::AllAllies)};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::ItemNotFound));
					CHECK((config.getAmountRegistered() == finalItemUnderlyingValue));
				}
			}

			WHEN("updating an existing item definition")
			{
				std::expected<void, RegistryErrorInfo> result{config.setItemTarget(ITEM_NAME_NONE, BattleTargetID::AllAllies)};

				THEN("the registry reports no error and the target is appropriately updated")
				{
					REQUIRE(result.has_value());
					CHECK((config.getAmountRegistered() == finalItemUnderlyingValue));
					CHECK((config.getItemMetadata(toItemID(BuiltinItemID::None))->mTargetID == BattleTargetID::AllAllies));
				}
			}
		}

		WHEN("calling the ItemID overload")
		{
			WHEN("calling with an invalid item name")
			{
				std::expected<void, RegistryErrorInfo> result{config.setItemTarget(ItemID{200}, BattleTargetID::AllAllies)};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::ItemNotFound));
					CHECK((config.getAmountRegistered() == finalItemUnderlyingValue));
				}
			}

			WHEN("updating an existing item definition")
			{
				std::expected<void, RegistryErrorInfo> result{
					config.setItemTarget(toItemID(BuiltinItemID::None), BattleTargetID::AllAllies),
				};

				THEN("the registry reports no error and the target is appropriately updated")
				{
					REQUIRE(result.has_value());
					CHECK((config.getAmountRegistered() == finalItemUnderlyingValue));
					CHECK((config.getItemMetadata(toItemID(BuiltinItemID::None))->mTargetID == BattleTargetID::AllAllies));
				}
			}
		}
	}

	GIVEN("renameItem")
	{
		WHEN("calling with an invalid item name")
		{
			std::expected<void, RegistryErrorInfo> result{config.renameItem("ThisIsInvalid", "NewName")};

			THEN("the registry reports an error and there is no update to the registry")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::ItemNotFound));
				CHECK((config.getAmountRegistered() == finalItemUnderlyingValue));
			}
		}

		WHEN("trying to rename to a name already in the registry")
		{
			std::expected<void, RegistryErrorInfo> result{
				config.renameItem(ITEM_NAME_NONE, ITEM_NAME_CHERI_BERRY),
			};

			THEN("the registry reports an error and there is no update to the registry")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::DuplicateItem));
				CHECK((config.getAmountRegistered() == finalItemUnderlyingValue));
			}
		}

		WHEN("updating an existing item definition")
		{
			std::expected<void, RegistryErrorInfo> result{config.renameItem(ITEM_NAME_NONE, "NewName")};

			THEN("the registry reports no error and the name is appropriately updated")
			{
				REQUIRE(result.has_value());
				CHECK((config.getAmountRegistered() == finalItemUnderlyingValue));
				CHECK((config.getItemMetadata(toItemID(BuiltinItemID::None))->mName == "NewName"));
			}
		}
	}

	GIVEN("updateItem")
	{
		std::vector<EffectTrigger> triggers{
			{
				.mEffects = {toEffectID(BuiltinEffectID::Recoil), toEffectID(BuiltinEffectID::StatusTick)},
				.mTrigger = BattleEventID::TurnEnd,
			},
		};

		ItemMeta definition{.mName = "TestItemName", .mTriggers = triggers};

		WHEN("calling the string_view overload")
		{
			WHEN("calling with an invalid item name")
			{
				std::expected<void, RegistryErrorInfo> result{config.updateItem("ThisIsInvalid", definition)};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::ItemNotFound));
					CHECK((config.getAmountRegistered() == finalItemUnderlyingValue));
				}
			}

			WHEN("trying to rename to a name already in the registry")
			{
				std::expected<void, RegistryErrorInfo> result{
					config.updateItem(ITEM_NAME_NONE, {.mName = std::string(ITEM_NAME_CHERI_BERRY), .mTriggers = triggers}),
				};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::DuplicateItem));
					CHECK((config.getAmountRegistered() == finalItemUnderlyingValue));
				}
			}

			WHEN("updating an existing item definition")
			{
				std::expected<void, RegistryErrorInfo> result{config.updateItem(ITEM_NAME_NONE, definition)};

				THEN("the registry reports no error and the target is appropriately updated")
				{
					REQUIRE(result.has_value());
					CHECK((config.getAmountRegistered() == finalItemUnderlyingValue));
					CHECK((config.getItemMetadata(toItemID(BuiltinItemID::None))->mName == "TestItemName"));
					CHECK((config.getItemMetadata(toItemID(BuiltinItemID::None))->mTriggers == triggers));
				}
			}
		}

		WHEN("calling the ItemID overload")
		{
			WHEN("calling with an invalid item name")
			{
				std::expected<void, RegistryErrorInfo> result{config.updateItem(ItemID{200}, definition)};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::ItemNotFound));
					CHECK((config.getAmountRegistered() == finalItemUnderlyingValue));
				}
			}

			WHEN("trying to rename to a name already in the registry")
			{
				std::expected<void, RegistryErrorInfo> result{
					config.updateItem(toItemID(BuiltinItemID::None), {.mName = std::string(ITEM_NAME_CHERI_BERRY), .mTriggers = triggers}),
				};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::DuplicateItem));
					CHECK((config.getAmountRegistered() == finalItemUnderlyingValue));
				}
			}

			WHEN("updating an existing item definition")
			{
				std::expected<void, RegistryErrorInfo> result{config.updateItem(toItemID(BuiltinItemID::None), definition)};

				THEN("the registry reports no error and the target is appropriately updated")
				{
					REQUIRE(result.has_value());
					CHECK((config.getAmountRegistered() == finalItemUnderlyingValue));
					CHECK((config.getItemMetadata(toItemID(BuiltinItemID::None))->mName == "TestItemName"));
					CHECK((config.getItemMetadata(toItemID(BuiltinItemID::None))->mTriggers == triggers));
				}
			}
		}
	}

	GIVEN("removeItem")
	{
		WHEN("calling the string_view overload")
		{
			WHEN("calling with an unknown item name")
			{
				std::expected<ItemID, RegistryErrorInfo> result{config.removeItem("Unknown")};

				THEN("the result is an error and the registry is not updated")
				{
					CHECK_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::ItemNotFound));
					CHECK((config.getAmountRegistered() == finalItemUnderlyingValue));
				}
			}

			WHEN("calling with a known item name")
			{
				std::expected<ItemID, RegistryErrorInfo> result{config.removeItem(ITEM_NAME_NONE)};

				THEN("the result is a success and the registry is updated")
				{
					CHECK(result.has_value());
					CHECK((result.value() == toItemID(BuiltinItemID::None)));
					CHECK((config.getAmountRegistered() == finalItemUnderlyingValue - 1));
				}
			}
		}

		WHEN("calling the ItemID overload")
		{
			WHEN("calling with an unknown item ID")
			{
				std::expected<ItemID, RegistryErrorInfo> result{config.removeItem(ItemID{200})};

				THEN("the result is an error and the registry is not updated")
				{
					CHECK_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::ItemNotFound));
					CHECK((config.getAmountRegistered() == finalItemUnderlyingValue));
				}
			}

			WHEN("calling with a known item ID")
			{
				std::expected<ItemID, RegistryErrorInfo> result{config.removeItem(toItemID(BuiltinItemID::None))};

				THEN("the result is a success and the registry is updated")
				{
					CHECK(result.has_value());
					CHECK((result.value() == toItemID(BuiltinItemID::None)));
					CHECK((config.getAmountRegistered() == finalItemUnderlyingValue - 1));
				}
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
