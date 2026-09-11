/*! @file typeRegistryConfiguration.test.cpp
	@brief C++ file for running tests for the TypeRegistryConfiguration.
	@date 09/10/2026
	@since 0.2.19
	@version 0.12.20
	@author Matthew Moore
*/

#include "Configuration/typeRegistryConfiguration.h"

#include <algorithm>
#include <array>
#include <cstddef>
#include <expected>
#include <format>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "Configuration/constants.h"
#include "Core/typedefs.h"
#include "Registry/registryError.h"
#include "Registry/typeRegistry.h"
#include "Types/builtInTypeID.h"
#include "Types/constants.h"
#include "Types/typeEffectiveness.h"
#include "Types/typeID.h"
#include "Types/typeMeta.h"
#include "Utility/Debug/Logging/logging.testHelper.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Configuration::MatchupPair;
using PocketCore::Configuration::MAX_TYPES;
using PocketCore::Configuration::RegistryError;
using PocketCore::Configuration::TypeDefinition;
using PocketCore::Configuration::TypeRegistryConfiguration;
using PocketCore::Core::ub;
using PocketCore::Core::us;
using PocketCore::Registry::RegistryErrorInfo;
using PocketCore::Registry::Type::TypeRegistry;
using PocketCore::Registry::UnspecifiedMatchup;
using PocketCore::Testing::ensureLoggerInitialized;
using PocketCore::Type::BuiltinTypeID;
using PocketCore::Type::FIRE_TYPE_MATCHUP;
using PocketCore::Type::NO_TYPE_ID;
using PocketCore::Type::toTypeID;
using PocketCore::Type::TYPE_NAME_BUG;
using PocketCore::Type::TYPE_NAME_FIRE;
using PocketCore::Type::TYPE_NAME_NONE;
using PocketCore::Type::TYPE_NAME_WATER;
using PocketCore::Type::TypeEffectiveness;
using PocketCore::Type::TypeID;
using PocketCore::Type::TypeMeta;
using enum TypeEffectiveness;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("TypeRegistryConfiguration")
{
	ensureLoggerInitialized("type registry configuration test", "typeRegistryConfiguration_test.log");

	TypeRegistryConfiguration config{};
	TypeRegistry registry{};
	ub finalTypeUnderlyingValue{std::to_underlying(BuiltinTypeID::FinalType)};

	GIVEN("getAmountRegistered")
	{
		THEN("the default registry returns the expected amount of built-in types")
		{
			CHECK((config.getAmountRegistered() == finalTypeUnderlyingValue));
		}
	}

	GIVEN("getRuntimeRegistry")
	{
		THEN("the runtime registry is the same as the default registry")
		{
			CHECK((config.getRuntimeRegistry() == registry));
		}
	}

	GIVEN("getTypeMetadata")
	{
		THEN("unknown IDs are absent")
		{
			CHECK((config.getTypeMetadata(TypeID{200}) == nullptr));
		}

		THEN("the metadata is retrieved when accessed by a valid Type ID")
		{
			TypeMeta expected{
				.mName = std::string(TYPE_NAME_NONE),
				.mTypeID = toTypeID(BuiltinTypeID::None),
			};

			CHECK((expected == *config.getTypeMetadata(NO_TYPE_ID)));
		}
	}

	GIVEN("getMatchup")
	{
		WHEN("calling with an invalid attacker name")
		{
			std::expected<TypeEffectiveness, RegistryErrorInfo> result{config.getMatchup("Unknown", TYPE_NAME_NONE)};

			THEN("an error is returned")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::TypeNotFound));
			}
		}

		WHEN("calling with an invalid defender name")
		{
			std::expected<TypeEffectiveness, RegistryErrorInfo> result{config.getMatchup(TYPE_NAME_NONE, "Unknown")};

			THEN("an error is returned")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::TypeNotFound));
			}
		}

		WHEN("calling with valid identifiers")
		{
			std::expected<TypeEffectiveness, RegistryErrorInfo> result{config.getMatchup(TYPE_NAME_FIRE, TYPE_NAME_BUG)};

			THEN("the appropriate type match is returned")
			{
				REQUIRE(result.has_value());
				CHECK((result.value() == TypeEffectiveness::SE));
			}
		}
	}

	GIVEN("getMatchupRow")
	{
		WHEN("calling with an invalid attacker name")
		{
			std::expected<std::array<TypeEffectiveness, MAX_TYPES>, RegistryErrorInfo> result{config.getMatchupRow("Unknown")};

			THEN("an error is returned")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::TypeNotFound));
			}
		}

		WHEN("calling with a valid attacker name")
		{
			std::expected<std::array<TypeEffectiveness, MAX_TYPES>, RegistryErrorInfo> result{config.getMatchupRow(TYPE_NAME_FIRE)};

			THEN("the appropriate type match is returned")
			{
				REQUIRE(result.has_value());
				CHECK((result.value() == FIRE_TYPE_MATCHUP));
			}
		}
	}

	GIVEN("getDefensiveColumn")
	{
		WHEN("calling with an invalid defender name")
		{
			std::expected<std::array<TypeEffectiveness, MAX_TYPES>, RegistryErrorInfo> result{config.getDefensiveColumn("Unknown")};

			THEN("an error is returned")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::TypeNotFound));
			}
		}

		WHEN("calling with a valid attacker name")
		{
			std::expected<std::array<TypeEffectiveness, MAX_TYPES>, RegistryErrorInfo> result{config.getDefensiveColumn(TYPE_NAME_FIRE)};

			std::array<TypeEffectiveness, MAX_TYPES> expectedMatchup{
				NOT_DEFINED, E, NVE, SE, E, NVE, NVE, E, E, SE, E, E, NVE, SE, E, E, E, NVE, NVE, // NOLINT(readability-trailing-comma)
			};

			THEN("the appropriate type match is returned")
			{
				REQUIRE(result.has_value());
				CHECK((result.value() == expectedMatchup));
			}
		}
	}

	GIVEN("getTypeID")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(config.getTypeID("Unknown").has_value());
		}

		THEN("the Type ID is retrieved by valid Type name")
		{
			std::optional<TypeID> typeID{config.getTypeID(TYPE_NAME_NONE)};

			REQUIRE(typeID.has_value());

			CHECK((typeID.value() == toTypeID(BuiltinTypeID::None)));
		}
	}

	GIVEN("getTypeName")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(config.getTypeName(TypeID{200}).has_value());
		}

		THEN("a registered type name is returned by stable ID")
		{
			std::optional<std::string_view> typeName{config.getTypeName(toTypeID(BuiltinTypeID::None))};

			REQUIRE(typeName.has_value());

			CHECK((typeName.value() == TYPE_NAME_NONE));
		}
	}

	GIVEN("getRegisteredTypes")
	{
		THEN("the amount of types returned matches the amount that are built-in")
		{
			CHECK((config.getRegisteredTypes().size() == finalTypeUnderlyingValue));
		}
	}

	GIVEN("setMatchup")
	{
		WHEN("calling with an invalid attacker name")
		{
			std::expected<void, RegistryErrorInfo> result{config.setMatchup("Unknown", TYPE_NAME_NONE, SE)};

			THEN("an error is returned")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::TypeNotFound));
			}
		}

		WHEN("calling with an invalid defender name")
		{
			std::expected<void, RegistryErrorInfo> result{config.setMatchup(TYPE_NAME_NONE, "Unknown", SE)};

			THEN("an error is returned")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::TypeNotFound));
			}
		}

		WHEN("calling with valid identifiers")
		{
			std::expected<void, RegistryErrorInfo> result{config.setMatchup(TYPE_NAME_FIRE, TYPE_NAME_BUG, NVE)};

			THEN("the appropriate type match is returned")
			{
				REQUIRE(result.has_value());
				CHECK((config.getMatchup(TYPE_NAME_FIRE, TYPE_NAME_BUG) == NVE));
			}
		}
	}

	GIVEN("setMatchupRow")
	{
		WHEN("calling the TypeEffectiveness overload")
		{
			WHEN("calling with an a row greater than the capacity of the registry")
			{
				std::array<TypeEffectiveness, static_cast<std::size_t>(MAX_TYPES + 1)> matchupRow{};

				std::expected<void, RegistryErrorInfo> result{config.setMatchupRow(TYPE_NAME_FIRE, matchupRow)};

				THEN("an error is returned")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::MatchupMismatch));
				}
			}

			WHEN("calling with an invalid attacker name")
			{
				std::expected<void, RegistryErrorInfo> result{config.setMatchupRow("Unknown", FIRE_TYPE_MATCHUP)};

				THEN("an error is returned")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::TypeNotFound));
				}
			}

			WHEN("calling with a valid attacker name")
			{
				std::expected<void, RegistryErrorInfo> result{config.setMatchupRow(TYPE_NAME_NONE, FIRE_TYPE_MATCHUP)};

				THEN("the appropriate type match is returned")
				{
					REQUIRE(result.has_value());
					REQUIRE(config.getMatchupRow(TYPE_NAME_NONE).has_value());
					CHECK((config.getMatchupRow(TYPE_NAME_NONE).value() == FIRE_TYPE_MATCHUP));
				}
			}
		}

		WHEN("calling the MatchupPair overload")
		{
			std::array<MatchupPair, 3> matchupRow{};
			matchupRow.at(0) = {.mTypeName = std::string(TYPE_NAME_FIRE), .mValue = SE};
			matchupRow.at(1) = {.mTypeName = std::string(TYPE_NAME_WATER), .mValue = NE};
			matchupRow.at(2) = {.mTypeName = std::string(TYPE_NAME_BUG), .mValue = NVE};

			WHEN("calling with an a row greater than the capacity of the registry")
			{
				std::array<MatchupPair, static_cast<std::size_t>(MAX_TYPES + 1)> overflowRow{};

				std::expected<void, RegistryErrorInfo> result{config.setMatchupRow(TYPE_NAME_FIRE, overflowRow)};

				THEN("an error is returned")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::MatchupMismatch));
				}
			}

			WHEN("calling with an invalid attacker name")
			{
				std::expected<void, RegistryErrorInfo> result{config.setMatchupRow("Unknown", matchupRow)};

				THEN("an error is returned")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::TypeNotFound));
				}
			}

			WHEN("calling with an an invalid row name after some successful lookups")
			{
				matchupRow.at(0) = {.mTypeName = std::string(TYPE_NAME_FIRE), .mValue = SE};
				matchupRow.at(1) = {.mTypeName = std::string(TYPE_NAME_WATER), .mValue = NVE};
				matchupRow.at(2) = {.mTypeName = "Unknown", .mValue = NVE};

				std::expected<void, RegistryErrorInfo> result{config.setMatchupRow(TYPE_NAME_FIRE, matchupRow)};

				THEN("an error is returned and the registry is not updated")
				{
					REQUIRE_FALSE(result.has_value());
					REQUIRE(config.getMatchupRow(TYPE_NAME_BUG).has_value());
					CHECK((result.error().mKind == RegistryError::TypeNotFound));
					CHECK((config.getMatchupRow(TYPE_NAME_FIRE).value() == FIRE_TYPE_MATCHUP));
				}
			}

			WHEN("calling with a valid attacker name")
			{
				std::expected<void, RegistryErrorInfo> result{config.setMatchupRow(TYPE_NAME_BUG, matchupRow)};

				std::array<TypeEffectiveness, MAX_TYPES> expected{
					NOT_DEFINED, E,	 SE,  NE, E,   SE, E,  NVE, NVE, E,
					NVE,		 SE, NVE, E,  NVE, E,  SE, NVE, NVE, // NOLINT(readability-trailing-comma)
				};

				THEN("the appropriate type match is returned")
				{
					REQUIRE(result.has_value());
					REQUIRE(config.getMatchupRow(TYPE_NAME_BUG).has_value());
					CHECK((config.getMatchupRow(TYPE_NAME_BUG).value() == expected));
				}
			}
		}
	}

	GIVEN("setDefensiveColumn")
	{
		WHEN("calling the TypeEffectiveness overload")
		{
			WHEN("calling with an a row greater than the capacity of the registry")
			{
				std::array<TypeEffectiveness, static_cast<std::size_t>(MAX_TYPES + 1)> matchupRow{};

				std::expected<void, RegistryErrorInfo> result{config.setDefensiveColumn(TYPE_NAME_FIRE, matchupRow)};

				THEN("an error is returned")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::MatchupMismatch));
				}
			}

			WHEN("calling with an invalid defender name")
			{
				std::expected<void, RegistryErrorInfo> result{config.setDefensiveColumn("Unknown", FIRE_TYPE_MATCHUP)};

				THEN("an error is returned")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::TypeNotFound));
				}
			}

			WHEN("calling with a valid defender name")
			{
				std::expected<void, RegistryErrorInfo> result{config.setDefensiveColumn(TYPE_NAME_NONE, FIRE_TYPE_MATCHUP)};

				THEN("the appropriate type match is returned")
				{
					REQUIRE(result.has_value());
					REQUIRE(config.getDefensiveColumn(TYPE_NAME_NONE).has_value());
					CHECK((config.getDefensiveColumn(TYPE_NAME_NONE).value() == FIRE_TYPE_MATCHUP));
				}
			}
		}

		WHEN("calling the MatchupPair overload")
		{
			std::array<MatchupPair, 3> matchupRow{};
			matchupRow.at(0) = {.mTypeName = std::string(TYPE_NAME_FIRE), .mValue = SE};
			matchupRow.at(1) = {.mTypeName = std::string(TYPE_NAME_WATER), .mValue = NE};
			matchupRow.at(2) = {.mTypeName = std::string(TYPE_NAME_BUG), .mValue = NVE};

			WHEN("calling with an a row greater than the capacity of the registry")
			{
				std::array<MatchupPair, static_cast<std::size_t>(MAX_TYPES + 1)> overflowRow{};

				std::expected<void, RegistryErrorInfo> result{config.setDefensiveColumn(TYPE_NAME_FIRE, overflowRow)};

				THEN("an error is returned")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::MatchupMismatch));
				}
			}

			WHEN("calling with an invalid attacker name")
			{
				std::expected<void, RegistryErrorInfo> result{config.setDefensiveColumn("Unknown", matchupRow)};

				THEN("an error is returned")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::TypeNotFound));
				}
			}

			WHEN("calling with an an invalid row name after some successful lookups")
			{
				matchupRow.at(0) = {.mTypeName = std::string(TYPE_NAME_FIRE), .mValue = SE};
				matchupRow.at(1) = {.mTypeName = std::string(TYPE_NAME_WATER), .mValue = NVE};
				matchupRow.at(2) = {.mTypeName = "Unknown", .mValue = NVE};

				std::expected<void, RegistryErrorInfo> result{config.setDefensiveColumn(TYPE_NAME_FIRE, matchupRow)};

				std::array<TypeEffectiveness, MAX_TYPES> expected{
					NOT_DEFINED, E, NVE, SE, E, NVE, NVE, E, E, SE, E, E, NVE, SE, E, E, E, NVE, NVE, // NOLINT(readability-trailing-comma)
				};

				THEN("an error is returned and the registry is not updated")
				{
					REQUIRE_FALSE(result.has_value());
					REQUIRE(config.getDefensiveColumn(TYPE_NAME_BUG).has_value());
					CHECK((result.error().mKind == RegistryError::TypeNotFound));
					CHECK((config.getDefensiveColumn(TYPE_NAME_FIRE).value() == expected));
				}
			}

			WHEN("calling with a valid attacker name")
			{
				std::expected<void, RegistryErrorInfo> result{config.setDefensiveColumn(TYPE_NAME_BUG, matchupRow)};

				std::array<TypeEffectiveness, MAX_TYPES> expected{
					NOT_DEFINED, E, SE, NE, E, NVE, E, NVE, E, NVE, SE, E, NVE, SE, E, E, E, E, E, // NOLINT(readability-trailing-comma)
				};

				THEN("the appropriate type match is returned")
				{
					REQUIRE(result.has_value());
					REQUIRE(config.getDefensiveColumn(TYPE_NAME_BUG).has_value());
					CHECK((config.getDefensiveColumn(TYPE_NAME_BUG).value() == expected));
				}
			}
		}
	}

	GIVEN("hasType")
	{
		WHEN("calling the string_view overload")
		{
			THEN("an unknown type name has no entry")
			{
				CHECK_FALSE(config.hasType("Unknown"));
			}

			THEN("a known type name has an entry")
			{
				CHECK(config.hasType(TYPE_NAME_NONE));
			}
		}

		WHEN("calling the TypeID overload")
		{
			THEN("an unknown type ID has no entry")
			{
				CHECK_FALSE(config.hasType(TypeID{200}));
			}

			THEN("a known type ID has an entry")
			{
				CHECK(config.hasType(NO_TYPE_ID));
			}
		}
	}

	GIVEN("addType")
	{
		WHEN("trying to add an type past the capacity")
		{
			us newTypeCount{finalTypeUnderlyingValue};

			for (us i{0}; i < MAX_TYPES - finalTypeUnderlyingValue; ++i)
			{
				std::string name{std::format("String_{:04}", i)};
				TypeDefinition definition{.mName = name};
				std::expected<TypeID, RegistryErrorInfo> result{config.addType(definition)};

				REQUIRE(result.has_value());

				TypeID assignedID{result.value()};
				CHECK((assignedID.getValue() == newTypeCount++));
			}

			std::string name{std::format("String_{:04}", MAX_TYPES + 1)};

			TypeDefinition definition{.mName = name};
			std::expected<TypeID, RegistryErrorInfo> result{config.addType(definition)};

			THEN("registration reports a max capacity and nothing is added")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::MaxCapacity));
				CHECK((config.getAmountRegistered() == MAX_TYPES));
			}
		}

		WHEN("an type whose name is already in use is added")
		{
			TypeDefinition definition{.mName = std::string(TYPE_NAME_BUG)};
			std::expected<TypeID, RegistryErrorInfo> result{config.addType(definition)};

			THEN("registration reports a duplicate type and nothing is added")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::DuplicateType));
				CHECK((config.getAmountRegistered() == finalTypeUnderlyingValue));
			}
		}

		WHEN("a unique type definition is added where default behavior is Neutral")
		{
			TypeDefinition definition{.mName = "TestTypeName"};

			std::expected<TypeID, RegistryErrorInfo> result{config.addType(definition, UnspecifiedMatchup::Neutral)};

			THEN("it receives the first custom stable ID and the type matchups are defaulted to Effective (1x)")
			{
				REQUIRE(result.has_value());
				TypeID assignedID{result.value()};
				CHECK((assignedID.getValue() == finalTypeUnderlyingValue));
				CHECK((config.getAmountRegistered() == finalTypeUnderlyingValue + 1));

				const TypeMeta *metadata{config.getTypeMetadata(assignedID)};

				REQUIRE((metadata != nullptr));
				REQUIRE(config.getMatchupRow("TestTypeName").has_value());
				CHECK((metadata->mName == "TestTypeName"));
				CHECK((std::ranges::all_of(config.getMatchupRow("TestTypeName").value(),
										   [](TypeEffectiveness matchup) { return matchup == E; })));
			}
		}

		WHEN("a unique type definition is added where default behavior is NotDefined")
		{
			TypeDefinition definition{.mName = "TestTypeName"};

			std::expected<TypeID, RegistryErrorInfo> result{config.addType(definition, UnspecifiedMatchup::NotDefined)};

			THEN("it receives the first custom stable ID and the type matchups are defaulted to NotDefined (0x)")
			{
				REQUIRE(result.has_value());
				TypeID assignedID{result.value()};
				CHECK((assignedID.getValue() == finalTypeUnderlyingValue));
				CHECK((config.getAmountRegistered() == finalTypeUnderlyingValue + 1));

				const TypeMeta *metadata{config.getTypeMetadata(assignedID)};

				REQUIRE((metadata != nullptr));
				REQUIRE(config.getMatchupRow("TestTypeName").has_value());
				CHECK((metadata->mName == "TestTypeName"));
				CHECK((std::ranges::all_of(config.getMatchupRow("TestTypeName").value(),
										   [](TypeEffectiveness matchup) { return matchup == NOT_DEFINED; })));
			}
		}

		WHEN("a unique type definition is added where the offensive matchup is the self-matchup")
		{
			std::vector<MatchupPair> matchups{{.mTypeName = "TestTypeName", .mValue = SE}};

			TypeDefinition definition{.mName = "TestTypeName", .mOffensiveMatchups = matchups};

			std::expected<TypeID, RegistryErrorInfo> result{config.addType(definition, UnspecifiedMatchup::Neutral)};

			THEN("it receives the first custom stable ID and the type matchups are defaulted to Effective (1x)")
			{
				REQUIRE(result.has_value());
				TypeID assignedID{result.value()};
				CHECK((assignedID.getValue() == finalTypeUnderlyingValue));
				CHECK((config.getAmountRegistered() == finalTypeUnderlyingValue + 1));

				const TypeMeta *metadata{config.getTypeMetadata(assignedID)};

				std::array<TypeEffectiveness, MAX_TYPES> expectedMatchups{E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, SE};
				std::ranges::for_each(expectedMatchups, [](TypeEffectiveness &matchup) {
					if (matchup == NOT_DEFINED)
					{
						matchup = E;
					}
				});

				REQUIRE((metadata != nullptr));
				REQUIRE(config.getMatchupRow("TestTypeName").has_value());
				CHECK((metadata->mName == "TestTypeName"));
				CHECK((config.getMatchupRow("TestTypeName").value() == expectedMatchups));
			}
		}

		WHEN("a unique type definition is added where the offensive matchup points to a non-existent type")
		{
			std::vector<MatchupPair> matchups{{.mTypeName = "Unknown", .mValue = SE}};

			TypeDefinition definition{.mName = "TestTypeName", .mOffensiveMatchups = matchups};

			std::expected<TypeID, RegistryErrorInfo> result{config.addType(definition, UnspecifiedMatchup::Neutral)};

			THEN("an error will be returned and the type will not be added")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::TypeNotFound));
				CHECK((config.getAmountRegistered() == finalTypeUnderlyingValue));
			}
		}

		WHEN("a unique type definition is added where the defensive matchup points to a non-existent type")
		{
			std::vector<MatchupPair> matchups{{.mTypeName = "Unknown", .mValue = SE}};

			TypeDefinition definition{.mName = "TestTypeName", .mDefensiveMatchups = matchups};

			std::expected<TypeID, RegistryErrorInfo> result{config.addType(definition, UnspecifiedMatchup::Neutral)};

			THEN("an error will be returned and the type will not be added")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::TypeNotFound));
				CHECK((config.getAmountRegistered() == finalTypeUnderlyingValue));
			}
		}

		WHEN("a unique type definition is added where the offensive matchup and defensive matchup point to the same type")
		{
			std::vector<MatchupPair> matchups{{.mTypeName = std::string(TYPE_NAME_FIRE), .mValue = SE}};

			TypeDefinition definition{.mName = "TestTypeName", .mOffensiveMatchups = matchups, .mDefensiveMatchups = matchups};

			std::expected<TypeID, RegistryErrorInfo> result{config.addType(definition, UnspecifiedMatchup::Neutral)};

			THEN("it receives the first custom stable ID and the type matchups are defaulted to Effective (1x)")
			{
				REQUIRE(result.has_value());
				TypeID assignedID{result.value()};
				CHECK((assignedID.getValue() == finalTypeUnderlyingValue));
				CHECK((config.getAmountRegistered() == finalTypeUnderlyingValue + 1));

				const TypeMeta *metadata{config.getTypeMetadata(assignedID)};

				std::array<TypeEffectiveness, MAX_TYPES> expectedMatchupRow{E, E, SE, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E};
				std::ranges::for_each(expectedMatchupRow, [](TypeEffectiveness &matchup) {
					if (matchup == NOT_DEFINED)
					{
						matchup = E;
					}
				});

				std::array<TypeEffectiveness, MAX_TYPES> expectedDefensiveColumn{
					E, E, SE, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, // NOLINT(readability-trailing-comma)
				};

				REQUIRE((metadata != nullptr));
				REQUIRE(config.getMatchupRow("TestTypeName").has_value());
				REQUIRE(config.getDefensiveColumn("TestTypeName").has_value());
				CHECK((metadata->mName == "TestTypeName"));
				CHECK((config.getMatchupRow("TestTypeName").value() == expectedMatchupRow));
				CHECK((config.getDefensiveColumn("TestTypeName").value() == expectedDefensiveColumn));
			}
		}

		WHEN("a unique type definition is added where the offensive matchup and defensive matchup point to different types")
		{
			std::vector<MatchupPair> offensiveMatchups{{.mTypeName = std::string(TYPE_NAME_FIRE), .mValue = SE}};
			std::vector<MatchupPair> defensiveMatchups{{.mTypeName = std::string(TYPE_NAME_WATER), .mValue = SE}};

			TypeDefinition definition{
				.mName = "TestTypeName",
				.mOffensiveMatchups = offensiveMatchups,
				.mDefensiveMatchups = defensiveMatchups,
			};

			std::expected<TypeID, RegistryErrorInfo> result{config.addType(definition, UnspecifiedMatchup::Neutral)};

			THEN("it receives the first custom stable ID and the type matchups are defaulted to Effective (1x)")
			{
				REQUIRE(result.has_value());
				TypeID assignedID{result.value()};
				CHECK((assignedID.getValue() == finalTypeUnderlyingValue));
				CHECK((config.getAmountRegistered() == finalTypeUnderlyingValue + 1));

				const TypeMeta *metadata{config.getTypeMetadata(assignedID)};

				std::array<TypeEffectiveness, MAX_TYPES> expectedMatchupRow{E, E, SE, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E};
				std::ranges::for_each(expectedMatchupRow, [](TypeEffectiveness &matchup) {
					if (matchup == NOT_DEFINED)
					{
						matchup = E;
					}
				});

				std::array<TypeEffectiveness, MAX_TYPES> expectedDefensiveColumn{
					E, E, E, SE, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, // NOLINT(readability-trailing-comma)
				};

				REQUIRE((metadata != nullptr));
				REQUIRE(config.getMatchupRow("TestTypeName").has_value());
				REQUIRE(config.getDefensiveColumn("TestTypeName").has_value());
				CHECK((metadata->mName == "TestTypeName"));
				CHECK((config.getMatchupRow("TestTypeName").value() == expectedMatchupRow));
				CHECK((config.getDefensiveColumn("TestTypeName").value() == expectedDefensiveColumn));
			}
		}
	}

	GIVEN("addTypes")
	{
		WHEN("trying to add no types")
		{
			std::array<TypeDefinition, 0> typeDefinitions{};
			std::expected<void, RegistryErrorInfo> result{config.addTypes(typeDefinitions)};

			THEN("the registry reports no error and no types are added")
			{
				REQUIRE(result.has_value());
				CHECK((config.getAmountRegistered() == finalTypeUnderlyingValue));
			}
		}

		WHEN("trying to add an type past the capacity")
		{
			std::vector<TypeDefinition> typeMetas;

			for (us i{0}; i < MAX_TYPES - finalTypeUnderlyingValue; ++i)
			{
				std::string name{std::format("String_{:04}", i)};
				typeMetas.push_back({.mName = name});
			}

			std::string name{std::format("String_{:04}", MAX_TYPES + 1)};

			typeMetas.push_back({.mName = name});
			std::expected<void, RegistryErrorInfo> result{config.addTypes(typeMetas)};

			THEN("registration reports a max capacity and nothing is added")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::MaxCapacity));
				CHECK((config.getAmountRegistered() == finalTypeUnderlyingValue));
			}
		}

		WHEN("a type whose name is already in use is added")
		{
			std::vector<TypeDefinition> typeMetas{};

			for (us i{0}; i < 20; ++i)
			{
				std::string name{std::format("String_{:04}", i)};
				typeMetas.push_back({.mName = name});
			}

			typeMetas.push_back({.mName = std::string(TYPE_NAME_BUG)});
			std::expected<void, RegistryErrorInfo> result{config.addTypes(typeMetas)};

			THEN("registration reports a duplicate type and the registry is rollback to the checkpoint before the erroneous addition")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::DuplicateType));
				CHECK((config.getAmountRegistered() == finalTypeUnderlyingValue));
			}
		}

		WHEN("a type whose name is already was added in the batch beforehand is attempted to be added again")
		{
			std::vector<TypeDefinition> typeMetas{};

			for (us i{0}; i < 20; ++i)
			{
				std::string name{std::format("String_{:04}", i)};
				typeMetas.push_back({.mName = name});
			}

			std::string name{std::format("String_{:04}", 0)};
			typeMetas.push_back({.mName = name});
			std::expected<void, RegistryErrorInfo> result{config.addTypes(typeMetas)};

			THEN("registration reports a duplicate type and the registry is rollback to the checkpoint before the erroneous addition")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::DuplicateType));
				CHECK((config.getAmountRegistered() == finalTypeUnderlyingValue));
			}
		}

		WHEN("a unique type definition is added")
		{
			std::vector<TypeDefinition> typeMetas{{.mName = "TestTypeName"}};

			std::expected<void, RegistryErrorInfo> result{config.addTypes(typeMetas)};

			THEN("the registry reports no error and all the type definitions are added")
			{
				REQUIRE(result.has_value());
				CHECK((config.getAmountRegistered() == finalTypeUnderlyingValue + 1));
			}
		}
	}

	GIVEN("renameType")
	{
		WHEN("calling with an invalid type name")
		{
			std::expected<void, RegistryErrorInfo> result{config.renameType("ThisIsInvalid", "NewName")};

			THEN("the registry reports an error and there is no update to the registry")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::TypeNotFound));
				CHECK((config.getAmountRegistered() == finalTypeUnderlyingValue));
			}
		}

		WHEN("trying to rename to a name already in the registry")
		{
			std::expected<void, RegistryErrorInfo> result{config.renameType(TYPE_NAME_NONE, TYPE_NAME_BUG)};

			THEN("the registry reports an error and there is no update to the registry")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::DuplicateType));
				CHECK((config.getAmountRegistered() == finalTypeUnderlyingValue));
			}
		}

		WHEN("updating an existing type definition")
		{
			std::expected<void, RegistryErrorInfo> result{config.renameType(TYPE_NAME_NONE, "NewName")};

			THEN("the registry reports no error and the name is appropriately updated")
			{
				REQUIRE(result.has_value());
				CHECK((config.getAmountRegistered() == finalTypeUnderlyingValue));
				CHECK((config.getTypeMetadata(toTypeID(BuiltinTypeID::None))->mName == "NewName"));
			}
		}
	}

	GIVEN("removeType")
	{
		WHEN("calling the string_view overload")
		{
			WHEN("calling with an unknown type name")
			{
				std::expected<TypeID, RegistryErrorInfo> result{config.removeType("Unknown")};

				THEN("the result is an error and the registry is not updated")
				{
					CHECK_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::TypeNotFound));
					CHECK((config.getAmountRegistered() == finalTypeUnderlyingValue));
				}
			}

			WHEN("calling with a known type name")
			{
				std::expected<TypeID, RegistryErrorInfo> result{config.removeType(TYPE_NAME_NONE)};

				THEN("the result is a success and the registry is updated")
				{
					CHECK(result.has_value());
					CHECK((result.value() == toTypeID(BuiltinTypeID::None)));
					CHECK((config.getAmountRegistered() == finalTypeUnderlyingValue - 1));
				}
			}
		}

		WHEN("calling the TypeID overload")
		{
			WHEN("calling with an unknown type ID")
			{
				std::expected<TypeID, RegistryErrorInfo> result{config.removeType(TypeID{200})};

				THEN("the result is an error and the registry is not updated")
				{
					CHECK_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::TypeNotFound));
					CHECK((config.getAmountRegistered() == finalTypeUnderlyingValue));
				}
			}

			WHEN("calling with a known type ID")
			{
				std::expected<TypeID, RegistryErrorInfo> result{config.removeType(toTypeID(BuiltinTypeID::None))};

				THEN("the result is a success and the registry is updated")
				{
					CHECK(result.has_value());
					CHECK((result.value() == toTypeID(BuiltinTypeID::None)));
					CHECK((config.getAmountRegistered() == finalTypeUnderlyingValue - 1));
				}
			}
		}
	}

	GIVEN("resetMatchups")
	{
		WHEN("calling the string_view overload")
		{
			WHEN("calling with an unknown type name")
			{
				std::expected<void, RegistryErrorInfo> result{config.resetMatchups("Unknown")};

				THEN("the result is an error and the registry is not updated")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::TypeNotFound));
					CHECK((config.getAmountRegistered() == finalTypeUnderlyingValue));
				}
			}

			WHEN("calling with a known type name")
			{
				std::expected<void, RegistryErrorInfo> result{config.resetMatchups(TYPE_NAME_FIRE)};

				std::array<TypeEffectiveness, MAX_TYPES> expected{};
				expected.fill(NOT_DEFINED);

				THEN("the rows and column for the type are set to NOT_DEFINED")
				{
					REQUIRE(result.has_value());
					REQUIRE(config.getMatchupRow(TYPE_NAME_FIRE).has_value());
					REQUIRE(config.getDefensiveColumn(TYPE_NAME_FIRE).has_value());
					CHECK((config.getMatchupRow(TYPE_NAME_FIRE).value() == expected));
					CHECK((config.getDefensiveColumn(TYPE_NAME_FIRE).value() == expected));
				}
			}
		}

		WHEN("calling the TypeID overload")
		{
			WHEN("calling with an unknown type ID")
			{
				std::expected<void, RegistryErrorInfo> result{config.resetMatchups(TypeID{200})};

				THEN("the result is an error and the registry is not updated")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::TypeNotFound));
					CHECK((config.getAmountRegistered() == finalTypeUnderlyingValue));
				}
			}

			WHEN("calling with a known type ID")
			{
				std::expected<void, RegistryErrorInfo> result{config.resetMatchups(toTypeID(BuiltinTypeID::Fire))};

				std::array<TypeEffectiveness, MAX_TYPES> expected{};
				expected.fill(NOT_DEFINED);

				THEN("the rows and column for the type are set to NOT_DEFINED")
				{
					REQUIRE(result.has_value());
					REQUIRE(config.getMatchupRow(TYPE_NAME_FIRE).has_value());
					REQUIRE(config.getDefensiveColumn(TYPE_NAME_FIRE).has_value());
					CHECK((config.getMatchupRow(TYPE_NAME_FIRE).value() == expected));
					CHECK((config.getDefensiveColumn(TYPE_NAME_FIRE).value() == expected));
				}
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
