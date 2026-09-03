/*! @file typeRegistry.test.cpp
	@brief C++ file for running tests for the TypeRegistry.
	@date 09/03/2026
	@since 0.1.0
	@version 0.12.19
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
#include "Types/constants.h"
#include "Types/typeEffectiveness.h"
#include "Types/typeID.h"
#include "Types/typeMeta.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Configuration::MAX_TYPES;
using PocketCore::Core::ub;
using PocketCore::Core::us;
using PocketCore::Registry::Type::TypeMeta;
using PocketCore::Registry::Type::TypeRegistry;
using PocketCore::Type::BUG_TYPE_MATCHUP;
using PocketCore::Type::BuiltinTypeID;
using PocketCore::Type::DARK_TYPE_MATCHUP;
using PocketCore::Type::DRAGON_TYPE_MATCHUP;
using PocketCore::Type::ELECTRIC_TYPE_MATCHUP;
using PocketCore::Type::FAIRY_TYPE_MATCHUP;
using PocketCore::Type::FIGHTING_TYPE_MATCHUP;
using PocketCore::Type::FIRE_TYPE_MATCHUP;
using PocketCore::Type::FLYING_TYPE_MATCHUP;
using PocketCore::Type::GHOST_TYPE_MATCHUP;
using PocketCore::Type::GRASS_TYPE_MATCHUP;
using PocketCore::Type::GROUND_TYPE_MATCHUP;
using PocketCore::Type::ICE_TYPE_MATCHUP;
using PocketCore::Type::INVALID_TYPE_CHART_ROW;
using PocketCore::Type::NO_TYPE_ID;
using PocketCore::Type::NORMAL_TYPE_MATCHUP;
using PocketCore::Type::POISON_TYPE_MATCHUP;
using PocketCore::Type::PSYCHIC_TYPE_MATCHUP;
using PocketCore::Type::ROCK_TYPE_MATCHUP;
using PocketCore::Type::STEEL_TYPE_MATCHUP;
using PocketCore::Type::toTypeID;
using PocketCore::Type::TYPE_NAME_BUG;
using PocketCore::Type::TYPE_NAME_DARK;
using PocketCore::Type::TYPE_NAME_DRAGON;
using PocketCore::Type::TYPE_NAME_ELECTRIC;
using PocketCore::Type::TYPE_NAME_FAIRY;
using PocketCore::Type::TYPE_NAME_FIGHTING;
using PocketCore::Type::TYPE_NAME_FIRE;
using PocketCore::Type::TYPE_NAME_FLYING;
using PocketCore::Type::TYPE_NAME_GHOST;
using PocketCore::Type::TYPE_NAME_GRASS;
using PocketCore::Type::TYPE_NAME_GROUND;
using PocketCore::Type::TYPE_NAME_ICE;
using PocketCore::Type::TYPE_NAME_NONE;
using PocketCore::Type::TYPE_NAME_NORMAL;
using PocketCore::Type::TYPE_NAME_POISON;
using PocketCore::Type::TYPE_NAME_PSYCHIC;
using PocketCore::Type::TYPE_NAME_ROCK;
using PocketCore::Type::TYPE_NAME_STEEL;
using PocketCore::Type::TYPE_NAME_STELLAR;
using PocketCore::Type::TYPE_NAME_WATER;
using PocketCore::Type::TypeEffectiveness;
using PocketCore::Type::TypeID;
using PocketCore::Type::WATER_TYPE_MATCHUP;

using enum TypeEffectiveness;

template <typename Registry>
concept PubliclyStructurallyMutable = requires(Registry &registry) {
	registry.setAmountRegistered(0);
	registry.setTypeChartCell(0, 0, E);
};

static_assert(!PubliclyStructurallyMutable<TypeRegistry>);

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("TypeRegistry")
{
	TypeRegistry registry{};
	ub finalTypeUnderlyingValue{std::to_underlying(BuiltinTypeID::FinalType)};

	GIVEN("a default constructed type registry")
	{
		THEN("Normal has the appropriate properties")
		{
			TypeMeta expected{
				.mOffensiveMatchups = NORMAL_TYPE_MATCHUP,
				.mName = TYPE_NAME_NORMAL,
				.mTypeID = toTypeID(BuiltinTypeID::Normal),
			};

			const TypeMeta *actual{registry.getTypeMetadata(toTypeID(BuiltinTypeID::Normal))};

			CHECK((expected == *actual));
		}

		THEN("Fighting has the appropriate properties")
		{
			TypeMeta expected{
				.mOffensiveMatchups = FIGHTING_TYPE_MATCHUP,
				.mName = TYPE_NAME_FIGHTING,
				.mTypeID = toTypeID(BuiltinTypeID::Fighting),
			};

			const TypeMeta *actual{registry.getTypeMetadata(toTypeID(BuiltinTypeID::Fighting))};

			CHECK((expected == *actual));
		}

		THEN("Flying has the appropriate properties")
		{
			TypeMeta expected{
				.mOffensiveMatchups = FLYING_TYPE_MATCHUP,
				.mName = TYPE_NAME_FLYING,
				.mTypeID = toTypeID(BuiltinTypeID::Flying),
			};

			const TypeMeta *actual{registry.getTypeMetadata(toTypeID(BuiltinTypeID::Flying))};

			CHECK((expected == *actual));
		}

		THEN("Poison has the appropriate properties")
		{
			TypeMeta expected{
				.mOffensiveMatchups = POISON_TYPE_MATCHUP,
				.mName = TYPE_NAME_POISON,
				.mTypeID = toTypeID(BuiltinTypeID::Poison),
			};

			const TypeMeta *actual{registry.getTypeMetadata(toTypeID(BuiltinTypeID::Poison))};

			CHECK((expected == *actual));
		}

		THEN("Ground has the appropriate properties")
		{
			TypeMeta expected{
				.mOffensiveMatchups = GROUND_TYPE_MATCHUP,
				.mName = TYPE_NAME_GROUND,
				.mTypeID = toTypeID(BuiltinTypeID::Ground),
			};

			const TypeMeta *actual{registry.getTypeMetadata(toTypeID(BuiltinTypeID::Ground))};

			CHECK((expected == *actual));
		}

		THEN("Rock has the appropriate properties")
		{
			TypeMeta expected{
				.mOffensiveMatchups = ROCK_TYPE_MATCHUP,
				.mName = TYPE_NAME_ROCK,
				.mTypeID = toTypeID(BuiltinTypeID::Rock),
			};

			const TypeMeta *actual{registry.getTypeMetadata(toTypeID(BuiltinTypeID::Rock))};

			CHECK((expected == *actual));
		}

		THEN("Bug has the appropriate properties")
		{
			TypeMeta expected{
				.mOffensiveMatchups = BUG_TYPE_MATCHUP,
				.mName = TYPE_NAME_BUG,
				.mTypeID = toTypeID(BuiltinTypeID::Bug),
			};

			const TypeMeta *actual{registry.getTypeMetadata(toTypeID(BuiltinTypeID::Bug))};

			CHECK((expected == *actual));
		}

		THEN("Ghost has the appropriate properties")
		{
			TypeMeta expected{
				.mOffensiveMatchups = GHOST_TYPE_MATCHUP,
				.mName = TYPE_NAME_GHOST,
				.mTypeID = toTypeID(BuiltinTypeID::Ghost),
			};

			const TypeMeta *actual{registry.getTypeMetadata(toTypeID(BuiltinTypeID::Ghost))};

			CHECK((expected == *actual));
		}

		THEN("Steel has the appropriate properties")
		{
			TypeMeta expected{
				.mOffensiveMatchups = STEEL_TYPE_MATCHUP,
				.mName = TYPE_NAME_STEEL,
				.mTypeID = toTypeID(BuiltinTypeID::Steel),
			};

			const TypeMeta *actual{registry.getTypeMetadata(toTypeID(BuiltinTypeID::Steel))};

			CHECK((expected == *actual));
		}

		THEN("Fire has the appropriate properties")
		{
			TypeMeta expected{
				.mOffensiveMatchups = FIRE_TYPE_MATCHUP,
				.mName = TYPE_NAME_FIRE,
				.mTypeID = toTypeID(BuiltinTypeID::Fire),
			};

			const TypeMeta *actual{registry.getTypeMetadata(toTypeID(BuiltinTypeID::Fire))};

			CHECK((expected == *actual));
		}

		THEN("Water has the appropriate properties")
		{
			TypeMeta expected{
				.mOffensiveMatchups = WATER_TYPE_MATCHUP,
				.mName = TYPE_NAME_WATER,
				.mTypeID = toTypeID(BuiltinTypeID::Water),
			};

			const TypeMeta *actual{registry.getTypeMetadata(toTypeID(BuiltinTypeID::Water))};

			CHECK((expected == *actual));
		}

		THEN("Grass has the appropriate properties")
		{
			TypeMeta expected{
				.mOffensiveMatchups = GRASS_TYPE_MATCHUP,
				.mName = TYPE_NAME_GRASS,
				.mTypeID = toTypeID(BuiltinTypeID::Grass),
			};

			const TypeMeta *actual{registry.getTypeMetadata(toTypeID(BuiltinTypeID::Grass))};

			CHECK((expected == *actual));
		}

		THEN("Electric has the appropriate properties")
		{
			TypeMeta expected{
				.mOffensiveMatchups = ELECTRIC_TYPE_MATCHUP,
				.mName = TYPE_NAME_ELECTRIC,
				.mTypeID = toTypeID(BuiltinTypeID::Electric),
			};

			const TypeMeta *actual{registry.getTypeMetadata(toTypeID(BuiltinTypeID::Electric))};

			CHECK((expected == *actual));
		}

		THEN("Psychic has the appropriate properties")
		{
			TypeMeta expected{
				.mOffensiveMatchups = PSYCHIC_TYPE_MATCHUP,
				.mName = TYPE_NAME_PSYCHIC,
				.mTypeID = toTypeID(BuiltinTypeID::Psychic),
			};

			const TypeMeta *actual{registry.getTypeMetadata(toTypeID(BuiltinTypeID::Psychic))};

			CHECK((expected == *actual));
		}

		THEN("Ice has the appropriate properties")
		{
			TypeMeta expected{
				.mOffensiveMatchups = ICE_TYPE_MATCHUP,
				.mName = TYPE_NAME_ICE,
				.mTypeID = toTypeID(BuiltinTypeID::Ice),
			};

			const TypeMeta *actual{registry.getTypeMetadata(toTypeID(BuiltinTypeID::Ice))};

			CHECK((expected == *actual));
		}

		THEN("Dragon has the appropriate properties")
		{
			TypeMeta expected{
				.mOffensiveMatchups = DRAGON_TYPE_MATCHUP,
				.mName = TYPE_NAME_DRAGON,
				.mTypeID = toTypeID(BuiltinTypeID::Dragon),
			};

			const TypeMeta *actual{registry.getTypeMetadata(toTypeID(BuiltinTypeID::Dragon))};

			CHECK((expected == *actual));
		}

		THEN("Dark has the appropriate properties")
		{
			TypeMeta expected{
				.mOffensiveMatchups = DARK_TYPE_MATCHUP,
				.mName = TYPE_NAME_DARK,
				.mTypeID = toTypeID(BuiltinTypeID::Dark),
			};

			const TypeMeta *actual{registry.getTypeMetadata(toTypeID(BuiltinTypeID::Dark))};

			CHECK((expected == *actual));
		}

		THEN("Fairy has the appropriate properties")
		{
			TypeMeta expected{
				.mOffensiveMatchups = FAIRY_TYPE_MATCHUP,
				.mName = TYPE_NAME_FAIRY,
				.mTypeID = toTypeID(BuiltinTypeID::Fairy),
			};

			const TypeMeta *actual{registry.getTypeMetadata(toTypeID(BuiltinTypeID::Fairy))};

			CHECK((expected == *actual));
		}

		THEN("Stellar has the appropriate properties")
		{
			TypeMeta expected{
				.mName = TYPE_NAME_STELLAR,
				.mTypeID = toTypeID(BuiltinTypeID::Stellar),
			};

			const TypeMeta *actual{registry.getTypeMetadata(toTypeID(BuiltinTypeID::Stellar))};

			CHECK((expected == *actual));
		}
	}

	GIVEN("getTypeMetadata")
	{
		THEN("unknown IDs are absent")
		{
			CHECK((registry.getTypeMetadata(TypeID{200}) == nullptr));
		}

		THEN("the metadata is retrieved when accessed by a valid Type ID")
		{
			TypeMeta expected{
				.mName = TYPE_NAME_NONE,
				.mTypeID = toTypeID(BuiltinTypeID::None),
			};

			CHECK((expected == *registry.getTypeMetadata(NO_TYPE_ID)));
		}
	}

	GIVEN("getTypeID")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(registry.getTypeID("Unknown").has_value());
		}

		THEN("the Type ID is retrieved by valid Type name")
		{
			std::optional<TypeID> typeID{registry.getTypeID(TYPE_NAME_NONE)};

			REQUIRE(typeID.has_value());

			CHECK((typeID.value() == toTypeID(BuiltinTypeID::None)));
		}
	}

	GIVEN("getTypeName")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(registry.getTypeName(TypeID{200}).has_value());
		}

		THEN("a registered type name is returned by stable ID")
		{
			std::optional<std::string_view> typeName{registry.getTypeName(toTypeID(BuiltinTypeID::None))};

			REQUIRE(typeName.has_value());

			CHECK((typeName.value() == TYPE_NAME_NONE));
		}
	}

	GIVEN("getAmountRegistered")
	{
		THEN("the registered span contains the exact amount of built-in entries")
		{
			CHECK((registry.getAmountRegistered() == finalTypeUnderlyingValue));
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
			TypeMeta expected{
				.mName = TYPE_NAME_NONE,
				.mTypeID = toTypeID(BuiltinTypeID::None),
			};

			const TypeMeta *typeMeta{registry.getEntry(0)};

			REQUIRE((typeMeta != nullptr));
			CHECK((*typeMeta == expected));
		}
	}

	GIVEN("getRegisteredTypes")
	{
		THEN("the amount of types returned matches the amount that are built-in")
		{
			CHECK((registry.getRegisteredTypes().size() == finalTypeUnderlyingValue));
		}
	}

	GIVEN("getNextTypeID")
	{
		THEN("the next available stable Type ID is after all built in type IDs")
		{
			CHECK((registry.getNextTypeID() == finalTypeUnderlyingValue));
		}
	}

	GIVEN("findIndexByTypeID")
	{
		THEN("an unknown stable ID has no internal index")
		{
			std::optional<ub> typeIndex{registry.findIndexByTypeID(TypeID{200})};
			CHECK_FALSE(typeIndex.has_value());
		}

		THEN("the internal array index is retrieved by valid Type ID")
		{
			std::optional<ub> typeIndex{registry.findIndexByTypeID(NO_TYPE_ID)};

			REQUIRE(typeIndex.has_value());
			CHECK((typeIndex.value() == 0));
		}
	}

	GIVEN("hasType")
	{
		WHEN("calling the string_view overload")
		{
			THEN("an unknown type name has no entry")
			{
				CHECK_FALSE(registry.hasType("Unknown"));
			}

			THEN("a known type name has an entry")
			{
				CHECK(registry.hasType(TYPE_NAME_NONE));
			}
		}

		WHEN("calling the TypeID overload")
		{
			THEN("an unknown type ID has no entry")
			{
				CHECK_FALSE(registry.hasType(TypeID{200}));
			}

			THEN("a known type ID has an entry")
			{
				CHECK(registry.hasType(NO_TYPE_ID));
			}
		}
	}

	GIVEN("getTypeChartCell")
	{
		WHEN("calling the integer based overload")
		{
			THEN("a NOT_DEFINED is returned when the row is out of bounds")
			{
				CHECK((registry.getTypeChartCell(2'000, 5) == NOT_DEFINED));
			}

			THEN("a NOT_DEFINED is returned when the col is out of bounds")
			{
				CHECK((registry.getTypeChartCell(5, 2'000) == NOT_DEFINED));
			}

			THEN("an unoccupied slot defaults to NOT_DEFINED")
			{
				TypeEffectiveness chartCell{registry.getTypeChartCell(20, 1)};
				CHECK((chartCell == NOT_DEFINED));
			}

			THEN("Normal versus Normal is effective")
			{
				TypeEffectiveness chartCell{registry.getTypeChartCell(1, 1)};
				CHECK((chartCell == E));
			}

			THEN("Normal versus Rock is not very effective")
			{
				TypeEffectiveness chartCell{registry.getTypeChartCell(1, 6)};
				CHECK((chartCell == NVE));
			}

			THEN("Normal versus Ghost has no effect")
			{
				TypeEffectiveness chartCell{registry.getTypeChartCell(1, 8)};
				CHECK((chartCell == NE));
			}

			THEN("Grass versus Water is super effective")
			{
				TypeEffectiveness chartCell{registry.getTypeChartCell(12, 11)};
				CHECK((chartCell == SE));
			}
		}

		WHEN("calling the TypeId overload")
		{
			THEN("passing in an invalid attacking typeID returns nullptr")
			{
				CHECK((registry.getTypeChartCell(TypeID{200}, toTypeID(BuiltinTypeID::Fire)) == nullptr));
			}

			THEN("passing in an invalid defending typeID returns nullptr")
			{
				CHECK((registry.getTypeChartCell(toTypeID(BuiltinTypeID::Fire), TypeID{2'000}) == nullptr));
			}

			THEN("Normal versus Normal is effective")
			{
				TypeEffectiveness chartCell{*registry.getTypeChartCell(toTypeID(BuiltinTypeID::Normal), toTypeID(BuiltinTypeID::Normal))};
				CHECK((chartCell == E));
			}

			THEN("Normal versus Rock is not very effective")
			{
				TypeEffectiveness chartCell{*registry.getTypeChartCell(toTypeID(BuiltinTypeID::Normal), toTypeID(BuiltinTypeID::Rock))};
				CHECK((chartCell == NVE));
			}

			THEN("Normal versus Ghost has no effect")
			{
				TypeEffectiveness chartCell{*registry.getTypeChartCell(toTypeID(BuiltinTypeID::Normal), toTypeID(BuiltinTypeID::Ghost))};
				CHECK((chartCell == NE));
			}

			THEN("Grass versus Water is super effective")
			{
				TypeEffectiveness chartCell{*registry.getTypeChartCell(toTypeID(BuiltinTypeID::Grass), toTypeID(BuiltinTypeID::Water))};
				CHECK((chartCell == SE));
			}
		}
	}

	GIVEN("getTypeChartRow")
	{
		WHEN("calling the integer based overload")
		{
			THEN("an invalid type chart row is returned when the row is out of bounds")
			{
				CHECK((registry.getTypeChartRow(2'000) == INVALID_TYPE_CHART_ROW));
			}

			THEN("the Stellar row is all NOT_DEFINED")
			{
				std::array<TypeEffectiveness, MAX_TYPES> typeChartRow{registry.getTypeChartRow(19)};

				for (std::size_t indexValue{0}; indexValue < MAX_TYPES; ++indexValue)
				{
					CHECK((typeChartRow.at(indexValue) == NOT_DEFINED));
				}
			}

			THEN("the Normal row matches known matchups")
			{
				std::array<TypeEffectiveness, MAX_TYPES> typeChartRow{registry.getTypeChartRow(1)};

				CHECK((typeChartRow.at(0) == NOT_DEFINED));
				CHECK((typeChartRow.at(1) == E));
				CHECK((typeChartRow.at(6) == NVE)); // Rock
				CHECK((typeChartRow.at(8) == NE));	// Ghost
				CHECK((typeChartRow.at(9) == NVE)); // Steel
			}
		}

		WHEN("calling the TypeID overload")
		{
			THEN("passing in an invalid typeID returns nullptr")
			{
				CHECK((registry.getTypeChartRow(TypeID{200}) == nullptr));
			}

			THEN("the Stellar row is all NOT_DEFINED")
			{
				std::array<TypeEffectiveness, MAX_TYPES> typeChartRow{*registry.getTypeChartRow(toTypeID(BuiltinTypeID::None))};

				for (std::size_t indexValue{0}; indexValue < MAX_TYPES; ++indexValue)
				{
					CHECK((typeChartRow.at(indexValue) == NOT_DEFINED));
				}
			}

			THEN("the Normal row matches known matchups")
			{
				std::array<TypeEffectiveness, MAX_TYPES> typeChartRow{*registry.getTypeChartRow(toTypeID(BuiltinTypeID::Normal))};

				CHECK((typeChartRow.at(0) == NOT_DEFINED));
				CHECK((typeChartRow.at(1) == E));
				CHECK((typeChartRow.at(6) == NVE)); // Rock
				CHECK((typeChartRow.at(8) == NE));	// Ghost
				CHECK((typeChartRow.at(9) == NVE)); // Steel
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
