/*! @file itemRegistry.test.cpp
	@brief C++ file for running tests for the ItemRegistry.
	@date 09/03/2026
	@since 0.5.0
	@version 0.12.19
	@author Matthew Moore
*/

#include "Registry/itemRegistry.h"

#include <optional>
#include <span>
#include <string_view>

#include "Battle/battleTargetsAndTriggers.h"
#include "Core/typedefs.h"
#include "Effect/builtInEffectID.h"
#include "Item/builtInItemID.h"
#include "Item/constants.h"
#include "Item/itemID.h"
#include "Item/itemMeta.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Battle::BattleEventID;
using PocketCore::Battle::BattleEventRole;
using PocketCore::Battle::BattleTargetID;
using PocketCore::Core::ub;
using PocketCore::Effect::BuiltinEffectID;
using PocketCore::Effect::toEffectID;
using PocketCore::Item::BuiltinItemID;
using PocketCore::Item::ITEM_NAME_CHERI_BERRY;
using PocketCore::Item::ITEM_NAME_CHESTO_BERRY;
using PocketCore::Item::ITEM_NAME_NONE;
using PocketCore::Item::ItemID;
using PocketCore::Item::ItemMeta;
using PocketCore::Item::NO_ITEM_ID;
using PocketCore::Item::toItemID;
using PocketCore::Registry::Item::ItemRegistry;

template <typename Registry>
concept PubliclyStructurallyMutable = requires(Registry &registry) { registry.setAmountRegistered(0); };

static_assert(!PubliclyStructurallyMutable<ItemRegistry>);

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("ItemRegistry")
{
	ItemRegistry registry{};
	ub finalItemUnderlyingValue{std::to_underlying(BuiltinItemID::FinalItem)};

	GIVEN("a default constructed item registry")
	{
		THEN("Cheri Berry has the appropriate properties")
		{
			ItemMeta expected{
				.mTriggers = {{
					.mEffects = {toEffectID(BuiltinEffectID::StatusRemove)},
					.mTrigger = BattleEventID::StatusChanged,
					.mRole = BattleEventRole::Target,
				},},
				.mName = ITEM_NAME_CHERI_BERRY,
				.mItemID = toItemID(BuiltinItemID::CheriBerry),
				.mTargetID = BattleTargetID::Self,
				.mIsConsumable = true,
			};

			const ItemMeta *actual{registry.getItemMetadata(toItemID(BuiltinItemID::CheriBerry))};

			CHECK((expected == *actual));
		}

		THEN("Chesto Berry has the appropriate properties")
		{
			ItemMeta expected{
				.mTriggers = {{
					.mEffects = {toEffectID(BuiltinEffectID::StatusRemove)},
					.mTrigger = BattleEventID::StatusChanged,
					.mRole = BattleEventRole::Target,
				},},
				.mName = ITEM_NAME_CHESTO_BERRY,
				.mItemID = toItemID(BuiltinItemID::ChestoBerry),
				.mTargetID = BattleTargetID::Self,
				.mIsConsumable = true,
			};

			const ItemMeta *actual{registry.getItemMetadata(toItemID(BuiltinItemID::ChestoBerry))};

			CHECK((expected == *actual));
		}
	}

	GIVEN("getItemMetadata")
	{
		THEN("unknown IDs are absent")
		{
			CHECK((registry.getItemMetadata(ItemID{200}) == nullptr));
		}

		THEN("the metadata is retrieved when accessed by a valid Item ID")
		{
			ItemMeta expected{
				.mTriggers = {},
				.mName = ITEM_NAME_NONE,
				.mItemID = toItemID(BuiltinItemID::None),
			};

			CHECK((expected == *registry.getItemMetadata(NO_ITEM_ID)));
		}
	}

	GIVEN("getItemID")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(registry.getItemID("Unknown").has_value());
		}

		THEN("the Item ID is retrieved by valid Item name")
		{
			std::optional<ItemID> itemID{registry.getItemID(ITEM_NAME_NONE)};

			REQUIRE(itemID.has_value());

			CHECK((itemID.value() == toItemID(BuiltinItemID::None)));
		}
	}

	GIVEN("getItemName")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(registry.getItemName(ItemID{200}).has_value());
		}

		THEN("a registered item name is returned by stable ID")
		{
			std::optional<std::string_view> itemName{registry.getItemName(toItemID(BuiltinItemID::None))};

			REQUIRE(itemName.has_value());

			CHECK((itemName.value() == ITEM_NAME_NONE));
		}
	}

	GIVEN("getAmountRegistered")
	{
		THEN("the registered span contains the exact amount of built-in entries")
		{
			CHECK((registry.getAmountRegistered() == finalItemUnderlyingValue));
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
			ItemMeta expected{
				.mTriggers = {},
				.mName = ITEM_NAME_NONE,
				.mItemID = toItemID(BuiltinItemID::None),
			};

			const ItemMeta *itemMeta{registry.getEntry(0)};

			REQUIRE((itemMeta != nullptr));
			CHECK((*itemMeta == expected));
		}
	}

	GIVEN("getRegisteredItems")
	{
		THEN("the amount of items returned matches the amount that are built-in")
		{
			CHECK((registry.getRegisteredItems().size() == finalItemUnderlyingValue));
		}
	}

	GIVEN("getNextItemID")
	{
		THEN("the next available stable Item ID is after all built in item IDs")
		{
			CHECK((registry.getNextItemID() == finalItemUnderlyingValue));
		}
	}

	GIVEN("findIndexByItemID")
	{
		THEN("an unknown stable ID has no internal index")
		{
			std::optional<ub> itemIndex{registry.findIndexByItemID(ItemID{200})};
			CHECK_FALSE(itemIndex.has_value());
		}

		THEN("the internal array index is retrieved by valid Item ID")
		{
			std::optional<ub> itemIndex{registry.findIndexByItemID(NO_ITEM_ID)};

			REQUIRE(itemIndex.has_value());
			CHECK((itemIndex.value() == 0));
		}
	}

	GIVEN("hasItem")
	{
		WHEN("calling the string_view overload")
		{
			THEN("an unknown item name has no entry")
			{
				CHECK_FALSE(registry.hasItem("Unknown"));
			}

			THEN("a known item name has an entry")
			{
				CHECK(registry.hasItem(ITEM_NAME_NONE));
			}
		}

		WHEN("calling the ItemID overload")
		{
			THEN("an unknown item ID has no entry")
			{
				CHECK_FALSE(registry.hasItem(ItemID{200}));
			}

			THEN("a known item ID has an entry")
			{
				CHECK(registry.hasItem(NO_ITEM_ID));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
