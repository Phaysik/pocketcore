/*! @file itemValidation.test.cpp
	@brief C++ file for running tests for the Item validation functions.
	@date 10/13/2026
	@since 0.12.51
	@version 0.12.51
	@author Matthew Moore
*/

#include "Validation/Item/itemValidation.h"

#include <array>
#include <expected>
#include <memory>

#include "Configuration/constants.h"
#include "Core/typedefs.h"
#include "Item/builtInItemID.h"
#include "Item/itemID.h"
#include "Registry/itemRegistry.h"
#include "Validation/Item/itemError.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Configuration::MAX_ITEMS_PER_POKEMON;
using PocketCore::Core::us;
using PocketCore::Item::BuiltinItemID;
using PocketCore::Item::ItemID;
using PocketCore::Item::toItemID;
using PocketCore::Registry::Item::ItemRegistry;
using PocketCore::Validation::Item::ItemError;
using PocketCore::Validation::Item::validateItemMetadata;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("validateItemMetadata")
{
	std::unique_ptr<ItemRegistry> registryStorage{std::make_unique<ItemRegistry>()};
	ItemRegistry &registry{*registryStorage};

	GIVEN("an array of valid metadata")
	{
		std::array<ItemID, MAX_ITEMS_PER_POKEMON> validMetadata{toItemID(BuiltinItemID::CheriBerry)};

		std::expected<void, ItemError> result{validateItemMetadata(validMetadata, registry)};

		THEN("the metadata is valid")
		{
			REQUIRE(result.has_value());
		}
	}

	GIVEN("an array with invalid metadata")
	{
		std::array<ItemID, MAX_ITEMS_PER_POKEMON> invalidMetadata{ItemID{200}};

		std::expected<void, ItemError> result{validateItemMetadata(invalidMetadata, registry)};

		THEN("the metadata is invalid")
		{
			REQUIRE_FALSE(result.has_value());
			CHECK((result.error() == ItemError::MISSING_ITEM_METADATA));
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
