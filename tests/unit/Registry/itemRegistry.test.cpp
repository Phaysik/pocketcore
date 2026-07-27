#include "Registry/itemRegistry.h"

#include <optional>
#include <span>
#include <string_view>

#include "Effect/effectType.h"
#include "Item/builtinItemID.h"
#include "Item/itemID.h"
#include "Item/itemMeta.h"
#include "Item/itemTargetsAndTriggers.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Effect::EffectTypeID;
using PocketCore::Item::BuiltinItemID;
using PocketCore::Item::ItemID;
using PocketCore::Item::ItemMeta;
using PocketCore::Item::ItemTriggerID;
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

			std::optional<ItemID> cheriIdentifier{registry.getItemID("Cheri Berry")};
			std::optional<ItemID> chestoIdentifier{registry.getItemID("Chesto Berry")};
			REQUIRE(cheriIdentifier.has_value());
			REQUIRE(chestoIdentifier.has_value());
			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((cheriIdentifier.value() == toItemID(BuiltinItemID::CheriBerry)));
			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((chestoIdentifier.value() == toItemID(BuiltinItemID::ChestoBerry)));
		}

		THEN("Chesto Berry retains its turn-end status removal metadata")
		{
			auto metadata{registry.getItemMetadata(toItemID(BuiltinItemID::ChestoBerry))};
			REQUIRE(metadata.has_value());
			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			REQUIRE((metadata->get().mTriggers.size() == 1U));
			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((metadata->get().mTriggers.front().mTrigger == ItemTriggerID::OnTurnEnd));
			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			REQUIRE((metadata->get().mTriggers.front().mEffects.size() == 1U));
			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((metadata->get().mTriggers.front().mEffects.front() == EffectTypeID::StatusRemove));
		}

		THEN("unknown item lookups are absent")
		{
			ItemID unknownIdentifier{200};
			CHECK_FALSE(registry.getItemID("Unknown").has_value());
			CHECK_FALSE(registry.getItemMetadata(unknownIdentifier).has_value());
			CHECK_FALSE(registry.getItemName(unknownIdentifier).has_value());
			CHECK_FALSE(registry.findIndexByItemID(unknownIdentifier).has_value());
			CHECK_FALSE(registry.hasItem(unknownIdentifier));
		}

		THEN("registered entries are exposed through the domain API")
		{
			std::span<const ItemMeta> items{registry.getRegisteredItems()};
			REQUIRE((items.size() == 3U));
			CHECK((registry.getItemName(toItemID(BuiltinItemID::CheriBerry)) == "Cheri Berry"));
			CHECK(registry.hasItem("Chesto Berry"));
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)