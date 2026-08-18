#include "Registry/itemRegistry.h"

#include <optional>
#include <span>
#include <string_view>

#include "Battle/battleTargetsAndTriggers.h"
#include "Effect/builtInEffectID.h"
#include "Item/builtInItemID.h"
#include "Item/constants.h"
#include "Item/itemID.h"
#include "Item/itemMeta.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Battle::BattleEventID;
using PocketCore::Effect::BuiltinEffectID;
using PocketCore::Item::BuiltinItemID;
using PocketCore::Item::ITEM_NAME_CHERI_BERRY;
using PocketCore::Item::ITEM_NAME_CHESTO_BERRY;
using PocketCore::Item::ItemID;
using PocketCore::Item::ItemMeta;
using PocketCore::Item::toItemID;
using PocketCore::Registry::Item::ItemRegistry;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("ItemRegistry")
{
	ItemRegistry registry{};

	GIVEN("a default-constructed registry")
	{
		THEN("all built-in item metadata is registered")
		{
			CHECK((registry.getAmountRegistered() == 3));
			CHECK((registry.getNextItemID() == 3));

			std::optional<ItemID> cheriIdentifier{registry.getItemID(ITEM_NAME_CHERI_BERRY)};
			std::optional<ItemID> chestoIdentifier{registry.getItemID(ITEM_NAME_CHESTO_BERRY)};
			REQUIRE(cheriIdentifier.has_value());
			REQUIRE(chestoIdentifier.has_value());
			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((cheriIdentifier.value() == toItemID(BuiltinItemID::CheriBerry)));
			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((chestoIdentifier.value() == toItemID(BuiltinItemID::ChestoBerry)));
		}

		THEN("Chesto Berry retains its turn-end status removal metadata")
		{
			const ItemMeta *metadata{registry.getItemMetadata(toItemID(BuiltinItemID::ChestoBerry))};
			REQUIRE((metadata != nullptr));
			REQUIRE((metadata->mTriggers.size() == 1U));
			CHECK((metadata->mTriggers.front().mTrigger == BattleEventID::TurnEnd));
			REQUIRE((metadata->mTriggers.front().mEffects.size() == 1U));
			CHECK((metadata->mTriggers.front().mEffects.front() == BuiltinEffectID::StatusRemove));
		}

		THEN("unknown item lookups are absent")
		{
			ItemID unknownIdentifier{200};
			CHECK_FALSE(registry.getItemID("Unknown").has_value());
			CHECK((registry.getItemMetadata(unknownIdentifier) == nullptr));
			CHECK_FALSE(registry.getItemName(unknownIdentifier).has_value());
			CHECK_FALSE(registry.findIndexByItemID(unknownIdentifier).has_value());
			CHECK_FALSE(registry.hasItem(unknownIdentifier));
		}

		THEN("registered entries are exposed through the domain API")
		{
			std::span<const ItemMeta> items{registry.getRegisteredItems()};
			REQUIRE((items.size() == 3U));
			CHECK((registry.getItemName(toItemID(BuiltinItemID::CheriBerry)) == ITEM_NAME_CHERI_BERRY));
			CHECK(registry.hasItem(toItemID(BuiltinItemID::ChestoBerry)));
			CHECK(registry.hasItem(ITEM_NAME_CHESTO_BERRY));
		}

		THEN("the registered amount can be restored directly")
		{
			registry.setAmountRegistered(2);
			CHECK((registry.getAmountRegistered() == 2));
		}

		THEN("the next stable ID can be restored directly")
		{
			registry.setNextItemID(42);
			CHECK((registry.getNextItemID() == 42));
		}

		THEN("the incrementNextItemID() method increments the next stable ID")
		{
			registry.setNextItemID(42);
			registry.incrementNextItemID();
			CHECK((registry.getNextItemID() == 43));
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
