/*! @file typeRegistry.test.cpp
	@brief C++ file for running tests for the TypeRegistry.
	@date 09/03/2026
	@since 0.1.0
	@version 0.12.18
	@author Matthew Moore
*/

#include "Registry/typeRegistry.h"

#include <array>
#include <cstddef>
#include <optional>
#include <span>
#include <string_view>

#include "Configuration/constants.h"
#include "Core/typedefs.h"
#include "Types/builtInTypeID.h"
#include "Types/typeEffectiveness.h"
#include "Types/typeID.h"
#include "Types/typeMeta.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Configuration::MAX_TYPES;
using PocketCore::Core::us;
using PocketCore::Registry::Type::TypeMeta;
using PocketCore::Registry::Type::TypeRegistry;
using PocketCore::Type::BuiltinTypeID;
using PocketCore::Type::toTypeID;
using PocketCore::Type::TypeEffectiveness;
using PocketCore::Type::TypeID;

using enum TypeEffectiveness;

template <typename Registry>
concept PubliclyTypeChartMutable = requires(Registry &registry) { registry.setTypeChartCell(0, 0, E); };

static_assert(!PubliclyTypeChartMutable<TypeRegistry>);

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("TypeRegistry")
{
	TypeRegistry registry{};

	GIVEN("a default-constructed registry")
	{
		THEN("amount registered is 19")
		{
			us amountRegistered{registry.getAmountRegistered()};
			CHECK((amountRegistered == 19));
		}

		THEN("next type id is 19")
		{
			TypeID nextTypeIdentifier{registry.getNextTypeID()};
			CHECK((nextTypeIdentifier.getValue() == 19));
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
			TypeMeta firstEntry{registry.getEntry(0)};
			CHECK((firstEntry.mTypeID == toTypeID(BuiltinTypeID::Normal)));
			CHECK((firstEntry.mName == "Normal"));
		}

		THEN("the last builtin entry is Stellar")
		{
			TypeMeta lastEntry{registry.getEntry(18)};
			CHECK((lastEntry.mTypeID == toTypeID(BuiltinTypeID::Stellar)));
			CHECK((lastEntry.mName == "Stellar"));
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

	GIVEN("getTypeID")
	{
		THEN("selected builtin ids match enum values")
		{
			std::optional<TypeID> normalIdentifier{registry.getTypeID("Normal")};
			REQUIRE(normalIdentifier.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((normalIdentifier.value() == toTypeID(BuiltinTypeID::Normal)));

			std::optional<TypeID> fireIdentifier{registry.getTypeID("Fire")};
			REQUIRE(fireIdentifier.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((fireIdentifier.value() == toTypeID(BuiltinTypeID::Fire)));

			std::optional<TypeID> fightingIdentifier{registry.getTypeID("Fighting")};
			REQUIRE(fightingIdentifier.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((fightingIdentifier.value() == toTypeID(BuiltinTypeID::Fighting)));

			std::optional<TypeID> waterIdentifier{registry.getTypeID("Water")};
			REQUIRE(waterIdentifier.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((waterIdentifier.value() == toTypeID(BuiltinTypeID::Water)));

			std::optional<TypeID> stellarIdentifier{registry.getTypeID("Stellar")};
			REQUIRE(stellarIdentifier.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((stellarIdentifier.value() == toTypeID(BuiltinTypeID::Stellar)));
		}

		THEN("looking up a nonexistent name returns nullopt")
		{
			std::optional<TypeID> lookupResult{registry.getTypeID("Shadow")};
			CHECK_FALSE(lookupResult.has_value());
		}

		THEN("looking up an empty name returns nullopt")
		{
			std::optional<TypeID> lookupResult{registry.getTypeID("")};
			CHECK_FALSE(lookupResult.has_value());
		}

		THEN("name lookup is case-sensitive")
		{
			std::optional<TypeID> lowercaseResult{registry.getTypeID("normal")};
			CHECK_FALSE(lowercaseResult.has_value());

			std::optional<TypeID> uppercaseResult{registry.getTypeID("FIRE")};
			CHECK_FALSE(uppercaseResult.has_value());
		}
	}

	GIVEN("getTypeName")
	{
		THEN("looking up Normal id returns Normal")
		{
			std::optional<std::string_view> typeName{registry.getTypeName(toTypeID(BuiltinTypeID::Normal))};
			REQUIRE(typeName.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((typeName.value() == "Normal"));
		}

		THEN("looking up Fairy id returns Fairy")
		{
			std::optional<std::string_view> typeName{registry.getTypeName(toTypeID(BuiltinTypeID::Fairy))};
			REQUIRE(typeName.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((typeName.value() == "Fairy"));
		}

		THEN("looking up a nonexistent id returns nullopt")
		{
			std::optional<std::string_view> typeName{registry.getTypeName(TypeID{255})};
			CHECK_FALSE(typeName.has_value());
		}
	}

	GIVEN("getRegisteredTypes")
	{

		THEN("registered types span has size 19")
		{
			std::span<const TypeMeta> registeredTypes{registry.getRegisteredTypes()};
			CHECK((registeredTypes.size() == 19U));
		}

		THEN("registered types first entry is Normal")
		{
			std::span<const TypeMeta> registeredTypes{registry.getRegisteredTypes()};
			REQUIRE_FALSE(registeredTypes.empty());

			CHECK((registeredTypes.front().mName == "Normal"));
		}

		THEN("registered types last entry is Stellar")
		{
			std::span<const TypeMeta> registeredTypes{registry.getRegisteredTypes()};
			REQUIRE_FALSE(registeredTypes.empty());

			CHECK((registeredTypes.back().mName == "Stellar"));
		}
	}

	GIVEN("findIndexByTypeID")
	{

		THEN("finding index by Normal id returns zero")
		{
			std::optional<us> indexResult{registry.findIndexByTypeID(toTypeID(BuiltinTypeID::Normal))};
			REQUIRE(indexResult.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((indexResult.value() == 0));
		}

		THEN("finding index by Stellar id returns eighteen")
		{
			std::optional<us> indexResult{registry.findIndexByTypeID(toTypeID(BuiltinTypeID::Stellar))};
			REQUIRE(indexResult.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((indexResult.value() == 18));
		}

		THEN("finding index by nonexistent id returns nullopt")
		{
			std::optional<us> indexResult{registry.findIndexByTypeID(TypeID{200})};
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
			bool hasTypeByIdentifier{registry.hasType(toTypeID(BuiltinTypeID::Normal))};
			CHECK(hasTypeByIdentifier);
		}

		THEN("hasType by id returns true for Stellar")
		{
			bool hasTypeByIdentifier{registry.hasType(toTypeID(BuiltinTypeID::Stellar))};
			CHECK(hasTypeByIdentifier);
		}

		THEN("hasType by id returns false for nonexistent id")
		{
			TypeID nonexistentIdentifier{200};
			bool hasTypeByIdentifier{registry.hasType(nonexistentIdentifier)};
			CHECK_FALSE(hasTypeByIdentifier);
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
