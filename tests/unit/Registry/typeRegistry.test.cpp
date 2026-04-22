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

#include <gtest/gtest.h>

#include <catch2/matchers/catch_matchers.hpp>

using Pokemon::Configuration::MAX_TYPES;
using Pokemon::Core::ub;
using Pokemon::Registry::Types::TypeEntry;
using Pokemon::Registry::Types::TypeRegistry;
using Pokemon::Types::TypeEffectiveness;
using Pokemon::Types::Types;

using enum TypeEffectiveness;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

namespace
{
	// ---------------------------------------------------------------------------
	// Fixture
	// ---------------------------------------------------------------------------

	struct TypeRegistryTest : public ::testing::Test
	{
		public:
			TypeRegistry registry{};
	};
} // namespace

// ---------------------------------------------------------------------------
// Constructor Tests
// ---------------------------------------------------------------------------

TEST_F(TypeRegistryTest, GivenDefaultConstructionWhenQueryingAmountRegisteredThenReturns19)
{
	ub amount{registry.getAmountRegistered()};
	EXPECT_EQ(amount, 19);
}

TEST_F(TypeRegistryTest, GivenDefaultConstructionWhenQueryingNextTypeIdThenReturns19)
{
	ub nextId{registry.getNextTypeId()};
	EXPECT_EQ(nextId, 19);
}

// ---------------------------------------------------------------------------
// getEntry Tests
// ---------------------------------------------------------------------------

TEST_F(TypeRegistryTest, GivenDefaultRegistryWhenGettingFirstEntryThenReturnsNormal)
{
	TypeEntry entry{registry.getEntry(0)};
	EXPECT_EQ(entry.typeId, static_cast<ub>(Types::Normal));
	EXPECT_EQ(entry.name, "Normal");
}

TEST_F(TypeRegistryTest, GivenDefaultRegistryWhenGettingLastBuiltinEntryThenReturnsStellar)
{
	TypeEntry entry{registry.getEntry(18)};
	EXPECT_EQ(entry.typeId, static_cast<ub>(Types::Stellar));
	EXPECT_EQ(entry.name, "Stellar");
}

// ---------------------------------------------------------------------------
// getTypeChartCell Tests
// ---------------------------------------------------------------------------

TEST_F(TypeRegistryTest, GivenDefaultRegistryWhenQueryingNormalVsNormalThenReturnsEffective)
{
	// Normal (index 0) attacking Normal (index 0) => E
	TypeEffectiveness cell{registry.getTypeChartCell(0, 0)};
	EXPECT_EQ(cell, E);
}

TEST_F(TypeRegistryTest, GivenDefaultRegistryWhenQueryingNormalVsRockThenReturnsNotVeryEffective)
{
	// Normal (index 0) attacking Rock (index 5) => NVE
	// Rock is the 6th type registered (index 5 in mEntries)
	TypeEffectiveness cell{registry.getTypeChartCell(0, 5)};
	EXPECT_EQ(cell, NVE);
}

TEST_F(TypeRegistryTest, GivenDefaultRegistryWhenQueryingNormalVsGhostThenReturnsNoEffect)
{
	// Normal (index 0) attacking Ghost (index 7) => NE
	TypeEffectiveness cell{registry.getTypeChartCell(0, 7)};
	EXPECT_EQ(cell, NE);
}

TEST_F(TypeRegistryTest, GivenDefaultRegistryWhenQueryingFightingVsNormalThenReturnsSuperEffective)
{
	// Fighting (index 1) attacking Normal (index 0) => SE
	TypeEffectiveness cell{registry.getTypeChartCell(1, 0)};
	EXPECT_EQ(cell, SE);
}

// ---------------------------------------------------------------------------
// getTypeChartRow Tests
// ---------------------------------------------------------------------------

TEST_F(TypeRegistryTest, GivenDefaultRegistryWhenGettingNormalRowThenMatchesPredefinedMatchup)
{
	std::array<TypeEffectiveness, MAX_TYPES> row{registry.getTypeChartRow(0)};

	// Normal matchup has 18 values; remaining slots should be NOT_DEFINED
	EXPECT_EQ(row.at(0), E);   // vs Normal
	EXPECT_EQ(row.at(5), NVE); // vs Rock
	EXPECT_EQ(row.at(7), NE);  // vs Ghost
	EXPECT_EQ(row.at(8), NVE); // vs Steel
}

// ---------------------------------------------------------------------------
// getTypeId Tests
// ---------------------------------------------------------------------------

TEST_F(TypeRegistryTest, GivenDefaultRegistryWhenLookingUpNormalByNameThenReturnsCorrectId)
{
	std::optional<ub> result{registry.getTypeId("Normal")};
	ASSERT_TRUE(result.has_value());

	// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
	EXPECT_EQ(result.value(), static_cast<ub>(Types::Normal));
}

TEST_F(TypeRegistryTest, GivenDefaultRegistryWhenLookingUpFireByNameThenReturnsCorrectId)
{
	std::optional<ub> result{registry.getTypeId("Fire")};
	ASSERT_TRUE(result.has_value());

	// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
	EXPECT_EQ(result.value(), static_cast<ub>(Types::Fire));
}

TEST_F(TypeRegistryTest, GivenDefaultRegistryWhenLookingUpStellarByNameThenReturnsCorrectId)
{
	std::optional<ub> result{registry.getTypeId("Stellar")};
	ASSERT_TRUE(result.has_value());

	// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
	EXPECT_EQ(result.value(), static_cast<ub>(Types::Stellar));
}

TEST_F(TypeRegistryTest, GivenDefaultRegistryWhenLookingUpNonexistentNameThenReturnsNullopt)
{
	std::optional<ub> result{registry.getTypeId("Shadow")};
	EXPECT_FALSE(result.has_value());
}

TEST_F(TypeRegistryTest, GivenDefaultRegistryWhenLookingUpEmptyNameThenReturnsNullopt)
{
	std::optional<ub> result{registry.getTypeId("")};
	EXPECT_FALSE(result.has_value());
}

// ---------------------------------------------------------------------------
// getTypeName Tests
// ---------------------------------------------------------------------------

TEST_F(TypeRegistryTest, GivenDefaultRegistryWhenLookingUpNormalIdThenReturnsNormal)
{
	std::optional<std::string_view> result{registry.getTypeName(static_cast<ub>(Types::Normal))};
	ASSERT_TRUE(result.has_value());

	// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
	EXPECT_EQ(result.value(), "Normal");
}

TEST_F(TypeRegistryTest, GivenDefaultRegistryWhenLookingUpFairyIdThenReturnsFairy)
{
	std::optional<std::string_view> result{registry.getTypeName(static_cast<ub>(Types::Fairy))};
	ASSERT_TRUE(result.has_value());

	// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
	EXPECT_EQ(result.value(), "Fairy");
}

TEST_F(TypeRegistryTest, GivenDefaultRegistryWhenLookingUpNonexistentIdThenReturnsNullopt)
{
	std::optional<std::string_view> result{registry.getTypeName(255)};
	EXPECT_FALSE(result.has_value());
}

// ---------------------------------------------------------------------------
// getRegisteredTypes Tests
// ---------------------------------------------------------------------------

TEST_F(TypeRegistryTest, GivenDefaultRegistryWhenGettingRegisteredTypesThenSpanSizeEquals19)
{
	std::span<const TypeEntry> span{registry.getRegisteredTypes()};
	EXPECT_EQ(span.size(), 19U);
}

TEST_F(TypeRegistryTest, GivenDefaultRegistryWhenGettingRegisteredTypesThenFirstEntryIsNormal)
{
	std::span<const TypeEntry> span{registry.getRegisteredTypes()};
	ASSERT_FALSE(span.empty());
	EXPECT_EQ(span.front().name, "Normal");
}

TEST_F(TypeRegistryTest, GivenDefaultRegistryWhenGettingRegisteredTypesThenLastEntryIsStellar)
{
	std::span<const TypeEntry> span{registry.getRegisteredTypes()};
	ASSERT_FALSE(span.empty());
	EXPECT_EQ(span.back().name, "Stellar");
}

// ---------------------------------------------------------------------------
// setEntry Tests
// ---------------------------------------------------------------------------

TEST_F(TypeRegistryTest, GivenDefaultRegistryWhenSettingEntryThenEntryIsUpdated)
{
	TypeEntry customEntry{.typeId = 99, .name = "Custom"};
	registry.setEntry(0, customEntry);

	TypeEntry entry{registry.getEntry(0)};
	EXPECT_EQ(entry.typeId, 99);
	EXPECT_EQ(entry.name, "Custom");
}

// ---------------------------------------------------------------------------
// setTypeChartCell Tests
// ---------------------------------------------------------------------------

TEST_F(TypeRegistryTest, GivenDefaultRegistryWhenSettingCellThenCellIsUpdated)
{
	registry.setTypeChartCell(0, 0, SE);
	TypeEffectiveness cell{registry.getTypeChartCell(0, 0)};
	EXPECT_EQ(cell, SE);
}

// ---------------------------------------------------------------------------
// setTypeChartRow Tests
// ---------------------------------------------------------------------------

TEST_F(TypeRegistryTest, GivenDefaultRegistryWhenSettingRowThenRowIsUpdated)
{
	std::array<TypeEffectiveness, MAX_TYPES> newRow{};
	newRow.fill(SE);
	registry.setTypeChartRow(0, newRow);

	std::array<TypeEffectiveness, MAX_TYPES> row{registry.getTypeChartRow(0)};
	for (std::size_t idx = 0; idx < MAX_TYPES; ++idx)
	{
		EXPECT_EQ(row.at(idx), SE);
	}
}

// ---------------------------------------------------------------------------
// setAmountRegistered / setNextTypeId Tests
// ---------------------------------------------------------------------------

TEST_F(TypeRegistryTest, GivenDefaultRegistryWhenSettingAmountRegisteredThenValueChanges)
{
	registry.setAmountRegistered(5);
	ub amount{registry.getAmountRegistered()};
	EXPECT_EQ(amount, 5);
}

TEST_F(TypeRegistryTest, GivenDefaultRegistryWhenSettingNextTypeIdThenValueChanges)
{
	registry.setNextTypeId(42);
	ub nextId{registry.getNextTypeId()};
	EXPECT_EQ(nextId, 42);
}

// ---------------------------------------------------------------------------
// findIndexByTypeId Tests
// ---------------------------------------------------------------------------

TEST_F(TypeRegistryTest, GivenDefaultRegistryWhenFindingIndexByNormalIdThenReturnsZero)
{
	std::optional<ub> result{registry.findIndexByTypeId(static_cast<ub>(Types::Normal))};
	ASSERT_TRUE(result.has_value());

	// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
	EXPECT_EQ(result.value(), 0);
}

TEST_F(TypeRegistryTest, GivenDefaultRegistryWhenFindingIndexByStellarIdThenReturns18)
{
	std::optional<ub> result{registry.findIndexByTypeId(static_cast<ub>(Types::Stellar))};
	ASSERT_TRUE(result.has_value());

	// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
	EXPECT_EQ(result.value(), 18);
}

TEST_F(TypeRegistryTest, GivenDefaultRegistryWhenFindingIndexByNonexistentIdThenReturnsNullopt)
{
	std::optional<ub> result{registry.findIndexByTypeId(200)};
	EXPECT_FALSE(result.has_value());
}

// ---------------------------------------------------------------------------
// hasType (by name) Tests
// ---------------------------------------------------------------------------

TEST_F(TypeRegistryTest, GivenDefaultRegistryWhenCheckingHasTypeByNameNormalThenReturnsTrue)
{
	bool result{registry.hasType(std::string_view{"Normal"})};
	EXPECT_TRUE(result);
}

TEST_F(TypeRegistryTest, GivenDefaultRegistryWhenCheckingHasTypeByNameStellarThenReturnsTrue)
{
	bool result{registry.hasType(std::string_view{"Stellar"})};
	EXPECT_TRUE(result);
}

TEST_F(TypeRegistryTest, GivenDefaultRegistryWhenCheckingHasTypeByNameNonexistentThenReturnsFalse)
{
	bool result{registry.hasType(std::string_view{"Shadow"})};
	EXPECT_FALSE(result);
}

TEST_F(TypeRegistryTest, GivenDefaultRegistryWhenCheckingHasTypeByEmptyNameThenReturnsFalse)
{
	bool result{registry.hasType(std::string_view{""})};
	EXPECT_FALSE(result);
}

// ---------------------------------------------------------------------------
// hasType (by ID) Tests
// ---------------------------------------------------------------------------

TEST_F(TypeRegistryTest, GivenDefaultRegistryWhenCheckingHasTypeByIdNormalThenReturnsTrue)
{
	bool result{registry.hasType(static_cast<ub>(Types::Normal))};
	EXPECT_TRUE(result);
}

TEST_F(TypeRegistryTest, GivenDefaultRegistryWhenCheckingHasTypeByIdStellarThenReturnsTrue)
{
	bool result{registry.hasType(static_cast<ub>(Types::Stellar))};
	EXPECT_TRUE(result);
}

TEST_F(TypeRegistryTest, GivenDefaultRegistryWhenCheckingHasTypeByNonexistentIdThenReturnsFalse)
{
	ub nonexistentId = 200;
	bool result{registry.hasType(nonexistentId)};
	EXPECT_FALSE(result);
}

// ---------------------------------------------------------------------------
// incrementNextTypeId Tests
// ---------------------------------------------------------------------------

TEST_F(TypeRegistryTest, GivenDefaultRegistryWhenIncrementingNextTypeIdThenIncrementsBy1)
{
	ub before{registry.getNextTypeId()};
	registry.incrementNextTypeId();
	ub after{registry.getNextTypeId()};
	EXPECT_EQ(after, before + 1);
}

// ---------------------------------------------------------------------------
// incrementAmountRegistered Tests
// ---------------------------------------------------------------------------

TEST_F(TypeRegistryTest, GivenDefaultRegistryWhenIncrementingAmountRegisteredThenIncrementsBy1)
{
	ub before{registry.getAmountRegistered()};
	registry.incrementAmountRegistered();
	ub after{registry.getAmountRegistered()};
	EXPECT_EQ(after, before + 1);
}

// ---------------------------------------------------------------------------
// decrementAmountRegistered Tests
// ---------------------------------------------------------------------------

TEST_F(TypeRegistryTest, GivenDefaultRegistryWhenDecrementingAmountRegisteredThenDecrementsBy1)
{
	ub before{registry.getAmountRegistered()};
	registry.decrementAmountRegistered();
	ub after{registry.getAmountRegistered()};
	EXPECT_EQ(after, before - 1);
}

// ---------------------------------------------------------------------------
// All 19 Built-in Types Registered Correctly
// ---------------------------------------------------------------------------

TEST_F(TypeRegistryTest, GivenDefaultRegistryWhenCheckingAllBuiltinTypesThenAllArePresent)
{
	EXPECT_TRUE(registry.hasType(std::string_view{"Normal"}));
	EXPECT_TRUE(registry.hasType(std::string_view{"Fighting"}));
	EXPECT_TRUE(registry.hasType(std::string_view{"Flying"}));
	EXPECT_TRUE(registry.hasType(std::string_view{"Poison"}));
	EXPECT_TRUE(registry.hasType(std::string_view{"Ground"}));
	EXPECT_TRUE(registry.hasType(std::string_view{"Rock"}));
	EXPECT_TRUE(registry.hasType(std::string_view{"Bug"}));
	EXPECT_TRUE(registry.hasType(std::string_view{"Ghost"}));
	EXPECT_TRUE(registry.hasType(std::string_view{"Steel"}));
	EXPECT_TRUE(registry.hasType(std::string_view{"Fire"}));
	EXPECT_TRUE(registry.hasType(std::string_view{"Water"}));
	EXPECT_TRUE(registry.hasType(std::string_view{"Grass"}));
	EXPECT_TRUE(registry.hasType(std::string_view{"Electric"}));
	EXPECT_TRUE(registry.hasType(std::string_view{"Psychic"}));
	EXPECT_TRUE(registry.hasType(std::string_view{"Ice"}));
	EXPECT_TRUE(registry.hasType(std::string_view{"Dragon"}));
	EXPECT_TRUE(registry.hasType(std::string_view{"Dark"}));
	EXPECT_TRUE(registry.hasType(std::string_view{"Fairy"}));
	EXPECT_TRUE(registry.hasType(std::string_view{"Stellar"}));
}

// ---------------------------------------------------------------------------
// Type IDs Match Their Enum Values
// ---------------------------------------------------------------------------

TEST_F(TypeRegistryTest, GivenDefaultRegistryWhenCheckingBuiltinIdsThenTheyMatchEnumValues)
{
	std::optional<ub> normalId{registry.getTypeId("Normal")};
	ASSERT_TRUE(normalId.has_value());

	// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
	EXPECT_EQ(normalId.value(), static_cast<ub>(Types::Normal));

	std::optional<ub> fireId{registry.getTypeId("Fire")};
	ASSERT_TRUE(fireId.has_value());

	// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
	EXPECT_EQ(fireId.value(), static_cast<ub>(Types::Fire));

	std::optional<ub> fightingId{registry.getTypeId("Fighting")};
	ASSERT_TRUE(fightingId.has_value());

	// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
	EXPECT_EQ(fightingId.value(), static_cast<ub>(Types::Fighting));

	std::optional<ub> waterId{registry.getTypeId("Water")};
	ASSERT_TRUE(waterId.has_value());

	// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
	EXPECT_EQ(waterId.value(), static_cast<ub>(Types::Water));

	std::optional<ub> stellarId{registry.getTypeId("Stellar")};
	ASSERT_TRUE(stellarId.has_value());

	// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
	EXPECT_EQ(stellarId.value(), static_cast<ub>(Types::Stellar));
}

// ---------------------------------------------------------------------------
// Stellar Has No Type Chart Row (all NOT_DEFINED)
// ---------------------------------------------------------------------------

TEST_F(TypeRegistryTest, GivenDefaultRegistryWhenGettingStellarRowThenAllNotDefined)
{
	// Stellar is registered at index 18 via the no-matchup overload
	std::array<TypeEffectiveness, MAX_TYPES> row{registry.getTypeChartRow(18)};
	for (std::size_t idx = 0; idx < MAX_TYPES; ++idx)
	{
		EXPECT_EQ(row.at(idx), NOT_DEFINED);
	}
}

// ---------------------------------------------------------------------------
// Unoccupied Slots Default to NOT_DEFINED
// ---------------------------------------------------------------------------

TEST_F(TypeRegistryTest, GivenDefaultRegistryWhenQueryingUnoccupiedSlotThenReturnsNotDefined)
{
	// Slot 19 is not used by any registered type (only indices 0-18 are populated)
	TypeEffectiveness cell{registry.getTypeChartCell(19, 0)};
	EXPECT_EQ(cell, NOT_DEFINED);
}

// ---------------------------------------------------------------------------
// Type Chart Spot-Check: Known Matchups
// ---------------------------------------------------------------------------

TEST_F(TypeRegistryTest, GivenDefaultRegistryWhenQueryingFireVsGrassThenReturnsSuperEffective)
{
	// Fire is registered at index 9, Grass at index 11
	TypeEffectiveness cell{registry.getTypeChartCell(9, 11)};
	EXPECT_EQ(cell, SE);
}

TEST_F(TypeRegistryTest, GivenDefaultRegistryWhenQueryingWaterVsFireThenReturnsSuperEffective)
{
	// Water is registered at index 10, Fire at index 9
	TypeEffectiveness cell{registry.getTypeChartCell(10, 9)};
	EXPECT_EQ(cell, SE);
}

TEST_F(TypeRegistryTest, GivenDefaultRegistryWhenQueryingGrassVsWaterThenReturnsSuperEffective)
{
	// Grass is registered at index 11, Water at index 10
	TypeEffectiveness cell{registry.getTypeChartCell(11, 10)};
	EXPECT_EQ(cell, SE);
}

TEST_F(TypeRegistryTest, GivenDefaultRegistryWhenQueryingGhostVsNormalThenReturnsNoEffect)
{
	// Ghost is registered at index 7, Normal at index 0
	TypeEffectiveness cell{registry.getTypeChartCell(7, 0)};
	EXPECT_EQ(cell, NE);
}

TEST_F(TypeRegistryTest, GivenDefaultRegistryWhenQueryingElectricVsGroundThenReturnsNoEffect)
{
	// Electric is registered at index 12, Ground at index 4
	TypeEffectiveness cell{registry.getTypeChartCell(12, 4)};
	EXPECT_EQ(cell, NE);
}

TEST_F(TypeRegistryTest, GivenDefaultRegistryWhenQueryingDragonVsFairyThenReturnsNoEffect)
{
	// Dragon is registered at index 15, Fairy at index 17
	TypeEffectiveness cell{registry.getTypeChartCell(15, 17)};
	EXPECT_EQ(cell, NE);
}

// ---------------------------------------------------------------------------
// Setter Roundtrip Tests
// ---------------------------------------------------------------------------

TEST_F(TypeRegistryTest, GivenModifiedRegistryWhenSettingAndGettingCellThenRoundtrips)
{
	registry.setTypeChartCell(0, 0, NE);
	TypeEffectiveness cell{registry.getTypeChartCell(0, 0)};
	EXPECT_EQ(cell, NE);

	registry.setTypeChartCell(0, 0, SE);
	cell = registry.getTypeChartCell(0, 0);
	EXPECT_EQ(cell, SE);
}

TEST_F(TypeRegistryTest, GivenModifiedRegistryWhenSettingEntryThenLookupReflectsChange)
{
	TypeEntry replacement{.typeId = 50, .name = "Cosmic"};
	registry.setEntry(0, replacement);

	std::optional<ub> idResult{registry.getTypeId("Cosmic")};
	ASSERT_TRUE(idResult.has_value());

	// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
	EXPECT_EQ(idResult.value(), 50);

	// The old name should no longer resolve
	std::optional<ub> oldResult{registry.getTypeId("Normal")};
	EXPECT_FALSE(oldResult.has_value());
}

// ---------------------------------------------------------------------------
// Decrement Affects Span Size
// ---------------------------------------------------------------------------

TEST_F(TypeRegistryTest, GivenDefaultRegistryWhenDecrementingThenSpanShrinks)
{
	std::size_t sizeBefore{registry.getRegisteredTypes().size()};
	registry.decrementAmountRegistered();
	std::size_t sizeAfter{registry.getRegisteredTypes().size()};
	EXPECT_EQ(sizeAfter, sizeBefore - 1);
}

// ---------------------------------------------------------------------------
// Increment Affects Span Size
// ---------------------------------------------------------------------------

TEST_F(TypeRegistryTest, GivenRegistryWithRoomWhenIncrementingThenSpanGrows)
{
	// Decrement first so we have room to increment back
	registry.decrementAmountRegistered();
	std::size_t sizeBefore{registry.getRegisteredTypes().size()};
	registry.incrementAmountRegistered();
	std::size_t sizeAfter{registry.getRegisteredTypes().size()};
	EXPECT_EQ(sizeAfter, sizeBefore + 1);
}

// ---------------------------------------------------------------------------
// Name Lookup Is Case-Sensitive
// ---------------------------------------------------------------------------

TEST_F(TypeRegistryTest, GivenDefaultRegistryWhenLookingUpIncorrectCaseNameThenReturnsNullopt)
{
	std::optional<ub> result{registry.getTypeId("normal")};
	EXPECT_FALSE(result.has_value());

	result = registry.getTypeId("FIRE");
	EXPECT_FALSE(result.has_value());
}

// ---------------------------------------------------------------------------
// Multiple Increments of NextTypeId
// ---------------------------------------------------------------------------

TEST_F(TypeRegistryTest, GivenDefaultRegistryWhenIncrementingNextTypeIdMultipleTimesThenValuesAreMonotonic)
{
	ub first{registry.getNextTypeId()};
	registry.incrementNextTypeId();
	ub second{registry.getNextTypeId()};
	registry.incrementNextTypeId();
	ub third{registry.getNextTypeId()};

	EXPECT_EQ(second, first + 1);
	EXPECT_EQ(third, first + 2);
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
