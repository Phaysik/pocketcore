#include "Registry/typeRegistry.h"

#include <array>
#include <cstddef>
#include <optional>
#include <span>
#include <string_view>

#include "Configuration/constants.h"
#include "Core/typedefs.h"
#include "Types/typeEffectiveness.h"
#include "Types/types.h"

#include <catch2/catch_test_macros.hpp>

using Pokemon::Configuration::MAX_TYPES;
using Pokemon::Core::ub;
using Pokemon::Registry::Types::TypeEntry;
using Pokemon::Registry::Types::TypeRegistry;
using Pokemon::Types::TypeEffectiveness;
using Pokemon::Types::Types;

using enum TypeEffectiveness;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("TypeRegistry")
{
	TypeRegistry registry{};

	GIVEN("a default-constructed registry")
	{
		THEN("amount registered is 19")
		{
			ub amountRegistered{registry.getAmountRegistered()};
			CHECK((amountRegistered == 19));
		}

		THEN("next type id is 19")
		{
			ub nextTypeIdentifier{registry.getNextTypeId()};
			CHECK((nextTypeIdentifier == 19));
		}

		THEN("all builtin type names are present")
		{
			CHECK(registry.hasType(std::string_view{"Normal"}));
			CHECK(registry.hasType(std::string_view{"Fighting"}));
			CHECK(registry.hasType(std::string_view{"Flying"}));
			CHECK(registry.hasType(std::string_view{"Poison"}));
			CHECK(registry.hasType(std::string_view{"Ground"}));
			CHECK(registry.hasType(std::string_view{"Rock"}));
			CHECK(registry.hasType(std::string_view{"Bug"}));
			CHECK(registry.hasType(std::string_view{"Ghost"}));
			CHECK(registry.hasType(std::string_view{"Steel"}));
			CHECK(registry.hasType(std::string_view{"Fire"}));
			CHECK(registry.hasType(std::string_view{"Water"}));
			CHECK(registry.hasType(std::string_view{"Grass"}));
			CHECK(registry.hasType(std::string_view{"Electric"}));
			CHECK(registry.hasType(std::string_view{"Psychic"}));
			CHECK(registry.hasType(std::string_view{"Ice"}));
			CHECK(registry.hasType(std::string_view{"Dragon"}));
			CHECK(registry.hasType(std::string_view{"Dark"}));
			CHECK(registry.hasType(std::string_view{"Fairy"}));
			CHECK(registry.hasType(std::string_view{"Stellar"}));
		}

		THEN("the Stellar row is all NOT_DEFINED")
		{
			std::array<TypeEffectiveness, MAX_TYPES> typeChartRow{registry.getTypeChartRow(18)};
			for (std::size_t indexValue{0}; indexValue < MAX_TYPES; ++indexValue)
			{
				CHECK((typeChartRow.at(indexValue) == NOT_DEFINED));
			}
		}
	}

	GIVEN("getEntry")
	{
		THEN("the first entry is Normal")
		{
			TypeEntry firstEntry{registry.getEntry(0)};
			CHECK((firstEntry.typeId == static_cast<ub>(Types::Normal)));
			CHECK((firstEntry.name == "Normal"));
		}

		THEN("the last builtin entry is Stellar")
		{
			TypeEntry lastEntry{registry.getEntry(18)};
			CHECK((lastEntry.typeId == static_cast<ub>(Types::Stellar)));
			CHECK((lastEntry.name == "Stellar"));
		}
	}

	GIVEN("getTypeChartCell")
	{
		THEN("Normal versus Normal is effective")
		{
			// Normal (index 0) attacking Normal (index 0) => E
			TypeEffectiveness chartCell{registry.getTypeChartCell(0, 0)};
			CHECK((chartCell == E));
		}

		THEN("Normal versus Rock is not very effective")
		{
			// Normal (index 0) attacking Rock (index 5) => NVE
			// Rock is the 6th type registered (index 5 in mEntries)
			TypeEffectiveness chartCell{registry.getTypeChartCell(0, 5)};
			CHECK((chartCell == NVE));
		}

		THEN("Normal versus Ghost has no effect")
		{
			// Normal (index 0) attacking Ghost (index 7) => NE
			TypeEffectiveness chartCell{registry.getTypeChartCell(0, 7)};
			CHECK((chartCell == NE));
		}

		THEN("Fighting versus Normal is super effective")
		{
			// Fighting (index 1) attacking Normal (index 0) => SE
			TypeEffectiveness chartCell{registry.getTypeChartCell(1, 0)};
			CHECK((chartCell == SE));
		}

		THEN("an unoccupied slot defaults to NOT_DEFINED")
		{
			TypeEffectiveness chartCell{registry.getTypeChartCell(19, 0)};
			CHECK((chartCell == NOT_DEFINED));
		}

		THEN("Fire versus Grass is super effective")
		{
			TypeEffectiveness chartCell{registry.getTypeChartCell(9, 11)};
			CHECK((chartCell == SE));
		}

		THEN("Water versus Fire is super effective")
		{
			TypeEffectiveness chartCell{registry.getTypeChartCell(10, 9)};
			CHECK((chartCell == SE));
		}

		THEN("Grass versus Water is super effective")
		{
			TypeEffectiveness chartCell{registry.getTypeChartCell(11, 10)};
			CHECK((chartCell == SE));
		}

		THEN("Ghost versus Normal has no effect")
		{
			TypeEffectiveness chartCell{registry.getTypeChartCell(7, 0)};
			CHECK((chartCell == NE));
		}

		THEN("Electric versus Ground has no effect")
		{
			TypeEffectiveness chartCell{registry.getTypeChartCell(12, 4)};
			CHECK((chartCell == NE));
		}

		THEN("Dragon versus Fairy has no effect")
		{
			TypeEffectiveness chartCell{registry.getTypeChartCell(15, 17)};
			CHECK((chartCell == NE));
		}
	}

	GIVEN("getTypeChartRow")
	{

		THEN("the Normal row matches known matchups")
		{
			std::array<TypeEffectiveness, MAX_TYPES> typeChartRow{registry.getTypeChartRow(0)};
			CHECK((typeChartRow.at(0) == E));
			CHECK((typeChartRow.at(5) == NVE));
			CHECK((typeChartRow.at(7) == NE));
			CHECK((typeChartRow.at(8) == NVE));
		}
	}

	GIVEN("getTypeId")
	{
		THEN("selected builtin ids match enum values")
		{
			std::optional<ub> normalIdentifier{registry.getTypeId("Normal")};
			REQUIRE(normalIdentifier.has_value());
			CHECK((normalIdentifier.value() == static_cast<ub>(Types::Normal)));

			std::optional<ub> fireIdentifier{registry.getTypeId("Fire")};
			REQUIRE(fireIdentifier.has_value());
			CHECK((fireIdentifier.value() == static_cast<ub>(Types::Fire)));

			std::optional<ub> fightingIdentifier{registry.getTypeId("Fighting")};
			REQUIRE(fightingIdentifier.has_value());
			CHECK((fightingIdentifier.value() == static_cast<ub>(Types::Fighting)));

			std::optional<ub> waterIdentifier{registry.getTypeId("Water")};
			REQUIRE(waterIdentifier.has_value());
			CHECK((waterIdentifier.value() == static_cast<ub>(Types::Water)));

			std::optional<ub> stellarIdentifier{registry.getTypeId("Stellar")};
			REQUIRE(stellarIdentifier.has_value());
			CHECK((stellarIdentifier.value() == static_cast<ub>(Types::Stellar)));
		}

		THEN("looking up a nonexistent name returns nullopt")
		{
			std::optional<ub> lookupResult{registry.getTypeId("Shadow")};
			CHECK_FALSE(lookupResult.has_value());
		}

		THEN("looking up an empty name returns nullopt")
		{
			std::optional<ub> lookupResult{registry.getTypeId("")};
			CHECK_FALSE(lookupResult.has_value());
		}

		THEN("name lookup is case-sensitive")
		{
			std::optional<ub> lowercaseResult{registry.getTypeId("normal")};
			CHECK_FALSE(lowercaseResult.has_value());

			std::optional<ub> uppercaseResult{registry.getTypeId("FIRE")};
			CHECK_FALSE(uppercaseResult.has_value());
		}
	}

	GIVEN("getTypeName")
	{
		THEN("looking up Normal id returns Normal")
		{
			std::optional<std::string_view> typeName{registry.getTypeName(static_cast<ub>(Types::Normal))};
			REQUIRE(typeName.has_value());

			CHECK((typeName.value() == "Normal"));
		}

		THEN("looking up Fairy id returns Fairy")
		{
			std::optional<std::string_view> typeName{registry.getTypeName(static_cast<ub>(Types::Fairy))};
			REQUIRE(typeName.has_value());

			CHECK((typeName.value() == "Fairy"));
		}

		THEN("looking up a nonexistent id returns nullopt")
		{
			std::optional<std::string_view> typeName{registry.getTypeName(255)};
			CHECK_FALSE(typeName.has_value());
		}
	}

	GIVEN("getRegisteredTypes")
	{

		THEN("registered types span has size 19")
		{
			std::span<const TypeEntry> registeredTypes{registry.getRegisteredTypes()};
			CHECK((registeredTypes.size() == 19U));
		}

		THEN("registered types first entry is Normal")
		{
			std::span<const TypeEntry> registeredTypes{registry.getRegisteredTypes()};
			REQUIRE_FALSE(registeredTypes.empty());

			CHECK((registeredTypes.front().name == "Normal"));
		}

		THEN("registered types last entry is Stellar")
		{
			std::span<const TypeEntry> registeredTypes{registry.getRegisteredTypes()};
			REQUIRE_FALSE(registeredTypes.empty());

			CHECK((registeredTypes.back().name == "Stellar"));
		}
	}

	GIVEN("setEntry")
	{

		THEN("setting an entry updates that entry")
		{
			TypeEntry replacementEntry{.typeId = 99, .name = "Custom"};
			registry.setEntry(0, replacementEntry);

			TypeEntry updatedEntry{registry.getEntry(0)};
			CHECK((updatedEntry.typeId == 99));
			CHECK((updatedEntry.name == "Custom"));
		}

		THEN("setting an entry updates name lookups")
		{
			TypeEntry replacementEntry{.typeId = 50, .name = "Cosmic"};
			registry.setEntry(0, replacementEntry);

			std::optional<ub> cosmicIdentifier{registry.getTypeId("Cosmic")};
			REQUIRE(cosmicIdentifier.has_value());
			CHECK((cosmicIdentifier.value() == 50));

			std::optional<ub> formerIdentifier{registry.getTypeId("Normal")};
			CHECK_FALSE(formerIdentifier.has_value());
		}
	}

	GIVEN("setTypeChartCell")
	{
		THEN("setting a chart cell updates the value")
		{
			registry.setTypeChartCell(0, 0, SE);
			TypeEffectiveness chartCell{registry.getTypeChartCell(0, 0)};
			CHECK((chartCell == SE));
		}

		THEN("setting and getting one cell roundtrips")
		{
			registry.setTypeChartCell(0, 0, NE);
			TypeEffectiveness firstReadback{registry.getTypeChartCell(0, 0)};
			CHECK((firstReadback == NE));

			registry.setTypeChartCell(0, 0, SE);
			TypeEffectiveness secondReadback{registry.getTypeChartCell(0, 0)};
			CHECK((secondReadback == SE));
		}
	}

	GIVEN("setTypeChartRow")
	{
		THEN("setting a chart row updates all values")
		{
			std::array<TypeEffectiveness, MAX_TYPES> replacementRow{};
			replacementRow.fill(SE);
			registry.setTypeChartRow(0, replacementRow);

			std::array<TypeEffectiveness, MAX_TYPES> updatedRow{registry.getTypeChartRow(0)};
			for (std::size_t indexValue = 0; indexValue < MAX_TYPES; ++indexValue)
			{
				CHECK((updatedRow.at(indexValue) == SE));
			}
		}
	}

	GIVEN("setAmountRegistered")
	{
		THEN("setting amount registered changes the value")
		{
			registry.setAmountRegistered(5);
			ub amountRegistered{registry.getAmountRegistered()};
			CHECK((amountRegistered == 5));
		}
	}

	GIVEN("getNextTypeId")
	{
		THEN("multiple next type id increments are monotonic")
		{
			ub firstValue{registry.getNextTypeId()};
			registry.incrementNextTypeId();
			ub secondValue{registry.getNextTypeId()};
			registry.incrementNextTypeId();
			ub thirdValue{registry.getNextTypeId()};

			CHECK((secondValue == firstValue + 1));
			CHECK((thirdValue == firstValue + 2));
		}
	}

	GIVEN("setNextTypeId")
	{
		THEN("setting next type id changes the value")
		{
			registry.setNextTypeId(42);
			ub nextTypeIdentifier{registry.getNextTypeId()};
			CHECK((nextTypeIdentifier == 42));
		}
	}

	GIVEN("findIndexByTypeId")
	{

		THEN("finding index by Normal id returns zero")
		{
			std::optional<ub> indexResult{registry.findIndexByTypeId(static_cast<ub>(Types::Normal))};
			REQUIRE(indexResult.has_value());

			CHECK((indexResult.value() == 0));
		}

		THEN("finding index by Stellar id returns eighteen")
		{
			std::optional<ub> indexResult{registry.findIndexByTypeId(static_cast<ub>(Types::Stellar))};
			REQUIRE(indexResult.has_value());

			CHECK((indexResult.value() == 18));
		}

		THEN("finding index by nonexistent id returns nullopt")
		{
			std::optional<ub> indexResult{registry.findIndexByTypeId(200)};
			CHECK_FALSE(indexResult.has_value());
		}
	}

	GIVEN("hasType (by name)")
	{
		THEN("hasType by name returns true for Normal")
		{
			bool hasTypeByName{registry.hasType(std::string_view{"Normal"})};
			CHECK(hasTypeByName);
		}

		THEN("hasType by name returns true for Stellar")
		{
			bool hasTypeByName{registry.hasType(std::string_view{"Stellar"})};
			CHECK(hasTypeByName);
		}

		THEN("hasType by name returns false for nonexistent type")
		{
			bool hasTypeByName{registry.hasType(std::string_view{"Shadow"})};
			CHECK_FALSE(hasTypeByName);
		}

		THEN("hasType by name returns false for empty name")
		{
			bool hasTypeByName{registry.hasType(std::string_view{""})};
			CHECK_FALSE(hasTypeByName);
		}
	}

	GIVEN("hasType (by id)")
	{
		THEN("hasType by id returns true for Normal")
		{
			bool hasTypeByIdentifier{registry.hasType(static_cast<ub>(Types::Normal))};
			CHECK(hasTypeByIdentifier);
		}

		THEN("hasType by id returns true for Stellar")
		{
			bool hasTypeByIdentifier{registry.hasType(static_cast<ub>(Types::Stellar))};
			CHECK(hasTypeByIdentifier);
		}

		THEN("hasType by id returns false for nonexistent id")
		{
			ub nonexistentIdentifier{200};
			bool hasTypeByIdentifier{registry.hasType(nonexistentIdentifier)};
			CHECK_FALSE(hasTypeByIdentifier);
		}
	}

	GIVEN("incrementNextTypeId")
	{
		THEN("incrementing next type id increases by one")
		{
			ub beforeIncrement{registry.getNextTypeId()};
			registry.incrementNextTypeId();
			ub afterIncrement{registry.getNextTypeId()};

			CHECK((afterIncrement == beforeIncrement + 1));
		}
	}

	GIVEN("incrementAmountRegistered")
	{
		THEN("incrementing amount registered increases by one")
		{
			ub beforeIncrement{registry.getAmountRegistered()};
			registry.incrementAmountRegistered();
			ub afterIncrement{registry.getAmountRegistered()};

			CHECK((afterIncrement == beforeIncrement + 1));
		}
	}

	GIVEN("decrementAmountRegistered")
	{
		THEN("decrementing amount registered decreases by one")
		{
			ub beforeDecrement{registry.getAmountRegistered()};
			registry.decrementAmountRegistered();
			ub afterDecrement{registry.getAmountRegistered()};

			CHECK((afterDecrement == beforeDecrement - 1));
		}

		THEN("decrement then increment restores span growth")
		{
			registry.decrementAmountRegistered();
			std::size_t sizeBefore{registry.getRegisteredTypes().size()};
			registry.incrementAmountRegistered();
			std::size_t sizeAfter{registry.getRegisteredTypes().size()};
			CHECK((sizeAfter == sizeBefore + 1));
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
