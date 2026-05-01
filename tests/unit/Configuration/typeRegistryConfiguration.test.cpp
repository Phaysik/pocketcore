#include "Configuration/typeRegistryConfiguration.h"

#include <array>
#include <expected>
#include <optional>
#include <span>
#include <string_view>

#include "Configuration/constants.h"
#include "Core/typedefs.h"
#include "Types/types.h"
#include "Utility/Debug/Logging/logger.h"

#include <catch2/catch_test_macros.hpp>

using Pokemon::Configuration::MatchupPair;
using Pokemon::Configuration::MAX_TYPES;
using Pokemon::Configuration::RegistryError;
using Pokemon::Configuration::RegistryErrorInfo;
using Pokemon::Configuration::TypeDefinition;
using Pokemon::Configuration::TypeRegistryConfiguration;
using Pokemon::Configuration::UnspecifiedMatchup;
using Pokemon::Core::ub;
using Pokemon::Types::TypeEffectiveness;
using Pokemon::Types::Types;
using Pokemon::Utility::Debug::Logging::Logger;

using enum TypeEffectiveness;

// TODO: This is a dummy generated file and will be refined later

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

namespace
{
	constexpr std::string_view LOG_FILE{"typeRegistryConfiguration_test.log"};

	void ensureLoggerInitialized() // NOLINT(llvm-prefer-static-over-anonymous-namespace)
	{
		static bool initialized{false};

		if (!initialized)
		{
			initialized = Logger::initialize("trc_test", LOG_FILE, true);
		}
	}
} // namespace

SCENARIO("TypeRegistryConfiguration getMatchup")
{
	ensureLoggerInitialized();
	TypeRegistryConfiguration config{};

	GIVEN("two valid type names")
	{
		THEN("returns the correct effectiveness value")
		{
			auto result = config.getMatchup("Normal", "Normal");
			REQUIRE(result.has_value());
			CHECK((result.value() == E));
		}

		THEN("Normal versus Ghost has no effect")
		{
			auto result = config.getMatchup("Normal", "Ghost");
			REQUIRE(result.has_value());
			CHECK((result.value() == NE));
		}

		THEN("Fire versus Grass is super effective")
		{
			auto result = config.getMatchup("Fire", "Grass");
			REQUIRE(result.has_value());
			CHECK((result.value() == SE));
		}

		THEN("Water versus Fire is super effective")
		{
			auto result = config.getMatchup("Water", "Fire");
			REQUIRE(result.has_value());
			CHECK((result.value() == SE));
		}
	}

	GIVEN("an unknown attacker name")
	{
		THEN("returns TypeNotFound error")
		{
			auto result = config.getMatchup("Shadow", "Normal");
			REQUIRE_FALSE(result.has_value());
			CHECK((result.error().mKind == RegistryError::TypeNotFound));
			CHECK((result.error().mContext == "Shadow"));
		}
	}

	GIVEN("an unknown defender name")
	{
		THEN("returns TypeNotFound error")
		{
			auto result = config.getMatchup("Normal", "Shadow");
			REQUIRE_FALSE(result.has_value());
			CHECK((result.error().mKind == RegistryError::TypeNotFound));
			CHECK((result.error().mContext == "Shadow"));
		}
	}
}

SCENARIO("TypeRegistryConfiguration getMatchupRow")
{
	ensureLoggerInitialized();
	TypeRegistryConfiguration config{};

	GIVEN("a valid type name")
	{
		THEN("returns a row of effectiveness values")
		{
			auto result = config.getMatchupRow("Normal");
			REQUIRE(result.has_value());
			auto row = result.value();
			CHECK((row.at(0) == E));   // Normal vs Normal
			CHECK((row.at(5) == NVE)); // Normal vs Rock
			CHECK((row.at(7) == NE));  // Normal vs Ghost
		}
	}

	GIVEN("an unknown type name")
	{
		THEN("returns TypeNotFound error")
		{
			auto result = config.getMatchupRow("Shadow");
			REQUIRE_FALSE(result.has_value());
			CHECK((result.error().mKind == RegistryError::TypeNotFound));
		}
	}
}

SCENARIO("TypeRegistryConfiguration getDefensiveColumn")
{
	ensureLoggerInitialized();
	TypeRegistryConfiguration config{};

	GIVEN("a valid type name")
	{
		THEN("returns a column of effectiveness values")
		{
			auto result = config.getDefensiveColumn("Normal");
			REQUIRE(result.has_value());
			auto column = result.value();
			// Fighting (index 1) vs Normal => SE
			CHECK((column.at(1) == SE));
			// Ghost (index 7) vs Normal => NE
			CHECK((column.at(7) == NE));
		}
	}

	GIVEN("an unknown type name")
	{
		THEN("returns TypeNotFound error")
		{
			auto result = config.getDefensiveColumn("Shadow");
			REQUIRE_FALSE(result.has_value());
			CHECK((result.error().mKind == RegistryError::TypeNotFound));
		}
	}
}

SCENARIO("TypeRegistryConfiguration getTypeID and getTypeName")
{
	ensureLoggerInitialized();
	TypeRegistryConfiguration config{};

	GIVEN("a valid builtin type name")
	{
		THEN("getTypeID returns the correct id")
		{
			auto result = config.getTypeID("Normal");
			REQUIRE(result.has_value());
			CHECK((result.value() == static_cast<ub>(Types::Normal)));
		}

		THEN("getTypeName returns the correct name")
		{
			auto result = config.getTypeName(static_cast<ub>(Types::Fire));
			REQUIRE(result.has_value());
			CHECK((result.value() == "Fire"));
		}
	}

	GIVEN("a nonexistent type")
	{
		THEN("getTypeID returns nullopt")
		{
			auto result = config.getTypeID("Shadow");
			CHECK_FALSE(result.has_value());
		}

		THEN("getTypeName returns nullopt for unknown id")
		{
			auto result = config.getTypeName(255);
			CHECK_FALSE(result.has_value());
		}
	}
}

SCENARIO("TypeRegistryConfiguration getRegisteredTypes and getAmountRegistered")
{
	ensureLoggerInitialized();
	TypeRegistryConfiguration config{};

	GIVEN("a default-constructed configuration")
	{
		THEN("amount registered is 19")
		{
			ub count = config.getAmountRegistered();
			CHECK((count == 19));
		}

		THEN("registered types span has 19 entries")
		{
			auto types = config.getRegisteredTypes();
			CHECK((types.size() == 19U));
		}
	}
}

SCENARIO("TypeRegistryConfiguration setMatchup")
{
	ensureLoggerInitialized();
	TypeRegistryConfiguration config{};

	GIVEN("two valid type names")
	{
		THEN("updates the effectiveness value")
		{
			auto setResult = config.setMatchup("Normal", "Normal", SE);
			REQUIRE(setResult.has_value());

			auto getResult = config.getMatchup("Normal", "Normal");
			REQUIRE(getResult.has_value());
			CHECK((getResult.value() == SE));
		}
	}

	GIVEN("an unknown attacker name")
	{
		THEN("returns TypeNotFound error")
		{
			auto result = config.setMatchup("Shadow", "Normal", SE);
			REQUIRE_FALSE(result.has_value());
			CHECK((result.error().mKind == RegistryError::TypeNotFound));
		}
	}

	GIVEN("an unknown defender name")
	{
		THEN("returns TypeNotFound error")
		{
			auto result = config.setMatchup("Normal", "Shadow", SE);
			REQUIRE_FALSE(result.has_value());
			CHECK((result.error().mKind == RegistryError::TypeNotFound));
		}
	}
}

SCENARIO("TypeRegistryConfiguration setMatchupRow with positional span")
{
	ensureLoggerInitialized();
	TypeRegistryConfiguration config{};

	GIVEN("a valid type name and a full row")
	{
		THEN("updates all values in the row")
		{
			std::array<TypeEffectiveness, MAX_TYPES> newRow{};
			newRow.fill(SE);

			auto result = config.setMatchupRow("Normal", std::span<const TypeEffectiveness>{newRow});
			REQUIRE(result.has_value());

			auto rowResult = config.getMatchupRow("Normal");
			REQUIRE(rowResult.has_value());

			ub registered = config.getAmountRegistered();
			for (ub idx{0}; idx < registered; ++idx)
			{
				CHECK((rowResult.value().at(idx) == SE));
			}
		}
	}

	GIVEN("a span shorter than the registered count")
	{
		THEN("fills remaining columns with NOT_DEFINED")
		{
			std::array<TypeEffectiveness, 3> shortRow{SE, NVE, E};

			auto result = config.setMatchupRow("Normal", std::span<const TypeEffectiveness>{shortRow});
			REQUIRE(result.has_value());

			auto rowResult = config.getMatchupRow("Normal");
			REQUIRE(rowResult.has_value());

			ub registered = config.getAmountRegistered();
			for (ub idx{0}; idx < 3; ++idx)
			{
				CHECK((rowResult.value().at(idx) == shortRow.at(idx)));
			}
			for (ub idx{3}; idx < registered; ++idx)
			{
				CHECK((rowResult.value().at(idx) == NOT_DEFINED));
			}
		}
	}

	GIVEN("a span larger than MAX_TYPES")
	{
		THEN("returns MatchupMismatch error")
		{
			std::array<TypeEffectiveness, MAX_TYPES + 1> oversized{};
			auto result = config.setMatchupRow("Normal", std::span<const TypeEffectiveness>{oversized});
			REQUIRE_FALSE(result.has_value());
			CHECK((result.error().mKind == RegistryError::MatchupMismatch));
		}
	}

	GIVEN("an unknown type name")
	{
		THEN("returns TypeNotFound error")
		{
			std::array<TypeEffectiveness, 1> smallRow{E};
			auto result = config.setMatchupRow("Shadow", std::span<const TypeEffectiveness>{smallRow});
			REQUIRE_FALSE(result.has_value());
			CHECK((result.error().mKind == RegistryError::TypeNotFound));
		}
	}
}

SCENARIO("TypeRegistryConfiguration setMatchupRow with MatchupPair span")
{
	ensureLoggerInitialized();
	TypeRegistryConfiguration config{};

	GIVEN("valid name-keyed pairs")
	{
		THEN("updates the specified matchups and resets others to NOT_DEFINED")
		{
			std::array<MatchupPair, 2> pairs{{{.typeName = "Fire", .value = SE}, {.typeName = "Water", .value = NVE}}};

			auto result = config.setMatchupRow("Normal", std::span<const MatchupPair>{pairs});
			REQUIRE(result.has_value());

			auto fireMatchup = config.getMatchup("Normal", "Fire");
			REQUIRE(fireMatchup.has_value());
			CHECK((fireMatchup.value() == SE));

			auto waterMatchup = config.getMatchup("Normal", "Water");
			REQUIRE(waterMatchup.has_value());
			CHECK((waterMatchup.value() == NVE));

			// Unspecified matchups should be NOT_DEFINED
			auto grassMatchup = config.getMatchup("Normal", "Grass");
			REQUIRE(grassMatchup.has_value());
			CHECK((grassMatchup.value() == NOT_DEFINED));
		}
	}

	GIVEN("a pair referencing an unknown type")
	{
		THEN("returns TypeNotFound error")
		{
			std::array<MatchupPair, 1> pairs{{{.typeName = "Shadow", .value = SE}}};
			auto result = config.setMatchupRow("Normal", std::span<const MatchupPair>{pairs});
			REQUIRE_FALSE(result.has_value());
			CHECK((result.error().mKind == RegistryError::TypeNotFound));
		}
	}

	GIVEN("an unknown attacker name")
	{
		THEN("returns TypeNotFound error")
		{
			std::array<MatchupPair, 1> pairs{{{.typeName = "Fire", .value = SE}}};
			auto result = config.setMatchupRow("Shadow", std::span<const MatchupPair>{pairs});
			REQUIRE_FALSE(result.has_value());
			CHECK((result.error().mKind == RegistryError::TypeNotFound));
		}
	}
}

SCENARIO("TypeRegistryConfiguration setDefensiveColumn with positional span")
{
	ensureLoggerInitialized();
	TypeRegistryConfiguration config{};

	GIVEN("a valid type name and a full column")
	{
		THEN("updates all values in the column")
		{
			std::array<TypeEffectiveness, MAX_TYPES> newCol{};
			newCol.fill(NVE);

			auto result = config.setDefensiveColumn("Normal", std::span<const TypeEffectiveness>{newCol});
			REQUIRE(result.has_value());

			auto colResult = config.getDefensiveColumn("Normal");
			REQUIRE(colResult.has_value());

			ub registered = config.getAmountRegistered();
			for (ub idx{0}; idx < registered; ++idx)
			{
				CHECK((colResult.value().at(idx) == NVE));
			}
		}
	}

	GIVEN("a span shorter than the registered count")
	{
		THEN("fills remaining rows with NOT_DEFINED")
		{
			std::array<TypeEffectiveness, 3> shortCol{SE, NVE, E};

			auto result = config.setDefensiveColumn("Normal", std::span<const TypeEffectiveness>{shortCol});
			REQUIRE(result.has_value());

			auto colResult = config.getDefensiveColumn("Normal");
			REQUIRE(colResult.has_value());

			ub registered = config.getAmountRegistered();
			for (ub idx{0}; idx < 3; ++idx)
			{
				CHECK((colResult.value().at(idx) == shortCol.at(idx)));
			}
			for (ub idx{3}; idx < registered; ++idx)
			{
				CHECK((colResult.value().at(idx) == NOT_DEFINED));
			}
		}
	}

	GIVEN("a span larger than MAX_TYPES")
	{
		THEN("returns MatchupMismatch error")
		{
			std::array<TypeEffectiveness, MAX_TYPES + 1> oversized{};
			auto result = config.setDefensiveColumn("Normal", std::span<const TypeEffectiveness>{oversized});
			REQUIRE_FALSE(result.has_value());
			CHECK((result.error().mKind == RegistryError::MatchupMismatch));
		}
	}

	GIVEN("an unknown type name")
	{
		THEN("returns TypeNotFound error")
		{
			std::array<TypeEffectiveness, 1> smallCol{E};
			auto result = config.setDefensiveColumn("Shadow", std::span<const TypeEffectiveness>{smallCol});
			REQUIRE_FALSE(result.has_value());
			CHECK((result.error().mKind == RegistryError::TypeNotFound));
		}
	}
}

SCENARIO("TypeRegistryConfiguration setDefensiveColumn with MatchupPair span")
{
	ensureLoggerInitialized();
	TypeRegistryConfiguration config{};

	GIVEN("valid name-keyed pairs")
	{
		THEN("updates the specified matchups and resets others to NOT_DEFINED")
		{
			std::array<MatchupPair, 2> pairs{{{.typeName = "Fire", .value = SE}, {.typeName = "Water", .value = NVE}}};

			auto result = config.setDefensiveColumn("Normal", std::span<const MatchupPair>{pairs});
			REQUIRE(result.has_value());

			// Fire attacking Normal should be SE
			auto fireVsNormal = config.getMatchup("Fire", "Normal");
			REQUIRE(fireVsNormal.has_value());
			CHECK((fireVsNormal.value() == SE));

			// Water attacking Normal should be NVE
			auto waterVsNormal = config.getMatchup("Water", "Normal");
			REQUIRE(waterVsNormal.has_value());
			CHECK((waterVsNormal.value() == NVE));

			// Unspecified attackers should be NOT_DEFINED
			auto grassVsNormal = config.getMatchup("Grass", "Normal");
			REQUIRE(grassVsNormal.has_value());
			CHECK((grassVsNormal.value() == NOT_DEFINED));
		}
	}

	GIVEN("a pair referencing an unknown type")
	{
		THEN("returns TypeNotFound error")
		{
			std::array<MatchupPair, 1> pairs{{{.typeName = "Shadow", .value = SE}}};
			auto result = config.setDefensiveColumn("Normal", std::span<const MatchupPair>{pairs});
			REQUIRE_FALSE(result.has_value());
			CHECK((result.error().mKind == RegistryError::TypeNotFound));
		}
	}

	GIVEN("an unknown defender name")
	{
		THEN("returns TypeNotFound error")
		{
			std::array<MatchupPair, 1> pairs{{{.typeName = "Fire", .value = SE}}};
			auto result = config.setDefensiveColumn("Shadow", std::span<const MatchupPair>{pairs});
			REQUIRE_FALSE(result.has_value());
			CHECK((result.error().mKind == RegistryError::TypeNotFound));
		}
	}
}

SCENARIO("TypeRegistryConfiguration addType")
{
	ensureLoggerInitialized();
	TypeRegistryConfiguration config{};

	GIVEN("a valid type definition with Neutral default behavior")
	{
		THEN("adds the type and returns its stable id")
		{
			// Remove Stellar to make room (keeps registered < MAX_TYPES after add)
			auto removeResult = config.removeType("Stellar");
			REQUIRE(removeResult.has_value());

			std::array<MatchupPair, 2> offensive{{{.typeName = "Fire", .value = SE}, {.typeName = "Water", .value = NVE}}};
			std::array<MatchupPair, 1> defensive{{{.typeName = "Fire", .value = NVE}}};

			TypeDefinition definition{.name = "Custom", .offensiveMatchups = offensive, .defensiveMatchups = defensive};

			auto result = config.addType(definition, UnspecifiedMatchup::Neutral);
			REQUIRE(result.has_value());

			CHECK((config.getAmountRegistered() == 19));
			CHECK(config.hasType("Custom"));

			// Check specified offensive matchup
			auto customVsFire = config.getMatchup("Custom", "Fire");
			REQUIRE(customVsFire.has_value());
			CHECK((customVsFire.value() == SE));

			// Check unspecified offensive matchup filled with Neutral (E)
			auto customVsGrass = config.getMatchup("Custom", "Grass");
			REQUIRE(customVsGrass.has_value());
			CHECK((customVsGrass.value() == E));

			// Check specified defensive matchup
			auto fireVsCustom = config.getMatchup("Fire", "Custom");
			REQUIRE(fireVsCustom.has_value());
			CHECK((fireVsCustom.value() == NVE));
		}
	}

	GIVEN("a valid type definition with NotDefined default behavior")
	{
		THEN("fills unspecified matchups with NOT_DEFINED")
		{
			// Remove Stellar to make room
			auto removeResult = config.removeType("Stellar");
			REQUIRE(removeResult.has_value());

			std::array<MatchupPair, 1> offensive{{{.typeName = "Fire", .value = SE}}};

			TypeDefinition definition{.name = "Void", .offensiveMatchups = offensive, .defensiveMatchups = {}};

			auto result = config.addType(definition, UnspecifiedMatchup::NotDefined);
			REQUIRE(result.has_value());

			auto voidVsGrass = config.getMatchup("Void", "Grass");
			REQUIRE(voidVsGrass.has_value());
			CHECK((voidVsGrass.value() == NOT_DEFINED));

			auto voidVsFire = config.getMatchup("Void", "Fire");
			REQUIRE(voidVsFire.has_value());
			CHECK((voidVsFire.value() == SE));
		}
	}

	GIVEN("a type definition with a self-matchup in offensive pairs")
	{
		THEN("the self-matchup cell is set correctly")
		{
			// Remove Stellar to make room
			auto removeResult = config.removeType("Stellar");
			REQUIRE(removeResult.has_value());

			std::array<MatchupPair, 1> offensive{{{.typeName = "SelfRef", .value = NVE}}};

			TypeDefinition definition{.name = "SelfRef", .offensiveMatchups = offensive, .defensiveMatchups = {}};

			auto result = config.addType(definition, UnspecifiedMatchup::Neutral);
			REQUIRE(result.has_value());

			auto selfMatchup = config.getMatchup("SelfRef", "SelfRef");
			REQUIRE(selfMatchup.has_value());
			CHECK((selfMatchup.value() == NVE));
		}
	}

	GIVEN("a duplicate type name")
	{
		THEN("returns DuplicateType error")
		{
			TypeDefinition definition{.name = "Normal", .offensiveMatchups = {}, .defensiveMatchups = {}};

			auto result = config.addType(definition, UnspecifiedMatchup::Neutral);
			REQUIRE_FALSE(result.has_value());
			CHECK((result.error().mKind == RegistryError::DuplicateType));
			CHECK((result.error().mContext == "Normal"));
		}
	}

	GIVEN("an offensive matchup referencing an unknown type")
	{
		THEN("returns TypeNotFound error")
		{
			std::array<MatchupPair, 1> offensive{{{.typeName = "Shadow", .value = SE}}};

			TypeDefinition definition{.name = "NewType", .offensiveMatchups = offensive, .defensiveMatchups = {}};

			auto result = config.addType(definition, UnspecifiedMatchup::Neutral);
			REQUIRE_FALSE(result.has_value());
			CHECK((result.error().mKind == RegistryError::TypeNotFound));
			CHECK((result.error().mContext == "Shadow"));
		}
	}

	GIVEN("a defensive matchup referencing an unknown type")
	{
		THEN("returns TypeNotFound error")
		{
			std::array<MatchupPair, 1> defensive{{{.typeName = "Shadow", .value = SE}}};

			TypeDefinition definition{.name = "NewType", .offensiveMatchups = {}, .defensiveMatchups = defensive};

			auto result = config.addType(definition, UnspecifiedMatchup::Neutral);
			REQUIRE_FALSE(result.has_value());
			CHECK((result.error().mKind == RegistryError::TypeNotFound));
			CHECK((result.error().mContext == "Shadow"));
		}
	}

	GIVEN("registry at max capacity")
	{
		THEN("returns MaxCapacity error")
		{
			// Add types until registry is full (19 builtins, MAX_TYPES = 20, so we can add 1)
			TypeDefinition firstDef{.name = "Extra", .offensiveMatchups = {}, .defensiveMatchups = {}};
			auto firstResult = config.addType(firstDef, UnspecifiedMatchup::Neutral);
			REQUIRE(firstResult.has_value());

			TypeDefinition overflowDef{.name = "Overflow", .offensiveMatchups = {}, .defensiveMatchups = {}};
			auto overflowResult = config.addType(overflowDef, UnspecifiedMatchup::Neutral);
			REQUIRE_FALSE(overflowResult.has_value());
			CHECK((overflowResult.error().mKind == RegistryError::MaxCapacity));
		}
	}
}

SCENARIO("TypeRegistryConfiguration addTypes")
{
	ensureLoggerInitialized();
	TypeRegistryConfiguration config{};

	GIVEN("an empty definitions span")
	{
		THEN("returns success with no changes")
		{
			std::span<const TypeDefinition> empty{};
			auto result = config.addTypes(empty, UnspecifiedMatchup::Neutral);
			REQUIRE(result.has_value());
			CHECK((config.getAmountRegistered() == 19));
		}
	}

	GIVEN("a single valid definition")
	{
		THEN("adds the type successfully")
		{
			// Remove Stellar to make room so hasType lookup works after add
			auto removeResult = config.removeType("Stellar");
			REQUIRE(removeResult.has_value());

			TypeDefinition def{.name = "Custom", .offensiveMatchups = {}, .defensiveMatchups = {}};
			std::array<TypeDefinition, 1> defs{def};

			auto result = config.addTypes(std::span<const TypeDefinition>{defs}, UnspecifiedMatchup::Neutral);
			REQUIRE(result.has_value());
			CHECK((config.getAmountRegistered() == 19));
			CHECK(config.hasType("Custom"));
		}
	}

	GIVEN("definitions that exceed capacity")
	{
		THEN("returns MaxCapacity error without modifying registry")
		{
			// Need more than 1 extra to exceed (19 builtins + 2 > 20)
			TypeDefinition def1{.name = "Extra1", .offensiveMatchups = {}, .defensiveMatchups = {}};
			TypeDefinition def2{.name = "Extra2", .offensiveMatchups = {}, .defensiveMatchups = {}};
			std::array<TypeDefinition, 2> defs{def1, def2};

			auto result = config.addTypes(std::span<const TypeDefinition>{defs}, UnspecifiedMatchup::Neutral);
			REQUIRE_FALSE(result.has_value());
			CHECK((result.error().mKind == RegistryError::MaxCapacity));
			CHECK((config.getAmountRegistered() == 19));
		}
	}

	GIVEN("duplicate names within the batch")
	{
		THEN("returns DuplicateType error and rolls back")
		{
			// First remove a type to make room for 2
			auto removeResult = config.removeType("Stellar");
			REQUIRE(removeResult.has_value());

			auto removeResult2 = config.removeType("Fairy");
			REQUIRE(removeResult2.has_value());

			ub countBefore = config.getAmountRegistered();

			TypeDefinition def1{.name = "Dup", .offensiveMatchups = {}, .defensiveMatchups = {}};
			TypeDefinition def2{.name = "Dup", .offensiveMatchups = {}, .defensiveMatchups = {}};
			std::array<TypeDefinition, 2> defs{def1, def2};

			auto result = config.addTypes(std::span<const TypeDefinition>{defs}, UnspecifiedMatchup::Neutral);
			REQUIRE_FALSE(result.has_value());
			CHECK((result.error().mKind == RegistryError::DuplicateType));
			CHECK((config.getAmountRegistered() == countBefore));
		}
	}
}

SCENARIO("TypeRegistryConfiguration removeType by name")
{
	ensureLoggerInitialized();
	TypeRegistryConfiguration config{};

	GIVEN("a valid type name")
	{
		THEN("removes the type and returns its id")
		{
			auto result = config.removeType("Normal");
			REQUIRE(result.has_value());
			CHECK((result.value() == static_cast<ub>(Types::Normal)));
			CHECK((config.getAmountRegistered() == 18));
			CHECK_FALSE(config.hasType("Normal"));
		}
	}

	GIVEN("an unknown type name")
	{
		THEN("returns TypeNotFound error")
		{
			auto result = config.removeType("Shadow");
			REQUIRE_FALSE(result.has_value());
			CHECK((result.error().mKind == RegistryError::TypeNotFound));
			CHECK((result.error().mContext == "Shadow"));
		}
	}
}

SCENARIO("TypeRegistryConfiguration removeType by enum")
{
	ensureLoggerInitialized();
	TypeRegistryConfiguration config{};

	GIVEN("a valid enum type")
	{
		THEN("removes the type and returns its id")
		{
			auto result = config.removeType(Types::Fire);
			REQUIRE(result.has_value());
			CHECK((result.value() == static_cast<ub>(Types::Fire)));
			CHECK_FALSE(config.hasType(static_cast<ub>(Types::Fire)));
		}
	}
}

SCENARIO("TypeRegistryConfiguration removeType by stable id")
{
	ensureLoggerInitialized();
	TypeRegistryConfiguration config{};

	GIVEN("a valid stable type id")
	{
		THEN("removes the type and returns its id")
		{
			ub normalId = static_cast<ub>(Types::Normal);
			auto result = config.removeType(normalId);
			REQUIRE(result.has_value());
			CHECK((result.value() == normalId));
			CHECK_FALSE(config.hasType("Normal"));
		}
	}

	GIVEN("a nonexistent stable type id")
	{
		THEN("returns TypeNotFound error")
		{
			ub invalidId = 200;
			auto result = config.removeType(invalidId);
			REQUIRE_FALSE(result.has_value());
			CHECK((result.error().mKind == RegistryError::TypeNotFound));
		}
	}
}

SCENARIO("TypeRegistryConfiguration removeTypes")
{
	ensureLoggerInitialized();
	TypeRegistryConfiguration config{};

	GIVEN("a list of valid type names")
	{
		THEN("removes all types")
		{
			std::array<std::string_view, 2> names{{"Normal", "Fire"}};
			auto result = config.removeTypes(std::span<const std::string_view>{names});
			REQUIRE(result.has_value());
			CHECK_FALSE(config.hasType("Normal"));
			CHECK_FALSE(config.hasType("Fire"));
			CHECK((config.getAmountRegistered() == 17));
		}
	}

	GIVEN("a list containing an unknown type name")
	{
		THEN("rolls back all removals and returns error")
		{
			std::array<std::string_view, 2> names{{"Normal", "Shadow"}};
			auto result = config.removeTypes(std::span<const std::string_view>{names});
			REQUIRE_FALSE(result.has_value());
			CHECK((result.error().mKind == RegistryError::TypeNotFound));
			// Rollback: Normal should still be present
			CHECK(config.hasType("Normal"));
			CHECK((config.getAmountRegistered() == 19));
		}
	}
}

SCENARIO("TypeRegistryConfiguration renameType")
{
	ensureLoggerInitialized();
	TypeRegistryConfiguration config{};

	GIVEN("a valid old name and new name")
	{
		THEN("renames the type successfully")
		{
			auto result = config.renameType("Normal", "Typeless");
			REQUIRE(result.has_value());
			CHECK(config.hasType("Typeless"));
			CHECK_FALSE(config.hasType("Normal"));

			// Matchup data should still work via new name
			auto matchup = config.getMatchup("Typeless", "Typeless");
			REQUIRE(matchup.has_value());
			CHECK((matchup.value() == E));
		}
	}

	GIVEN("an unknown old name")
	{
		THEN("returns TypeNotFound error")
		{
			auto result = config.renameType("Shadow", "NewShadow");
			REQUIRE_FALSE(result.has_value());
			CHECK((result.error().mKind == RegistryError::TypeNotFound));
		}
	}

	GIVEN("a new name that already exists")
	{
		THEN("returns DuplicateType error")
		{
			auto result = config.renameType("Normal", "Fire");
			REQUIRE_FALSE(result.has_value());
			CHECK((result.error().mKind == RegistryError::DuplicateType));
			CHECK((result.error().mContext == "Fire"));
		}
	}
}

SCENARIO("TypeRegistryConfiguration resetMatchups by name")
{
	ensureLoggerInitialized();
	TypeRegistryConfiguration config{};

	GIVEN("a valid type name")
	{
		THEN("clears all offensive and defensive matchups to NOT_DEFINED")
		{
			auto result = config.resetMatchups("Normal");
			REQUIRE(result.has_value());

			ub registered = config.getAmountRegistered();

			// Check offensive row is cleared
			auto row = config.getMatchupRow("Normal");
			REQUIRE(row.has_value());
			for (ub idx{0}; idx < registered; ++idx)
			{
				CHECK((row.value().at(idx) == NOT_DEFINED));
			}

			// Check defensive column is cleared
			auto col = config.getDefensiveColumn("Normal");
			REQUIRE(col.has_value());
			for (ub idx{0}; idx < registered; ++idx)
			{
				CHECK((col.value().at(idx) == NOT_DEFINED));
			}

			// Type should still exist
			CHECK(config.hasType("Normal"));
		}
	}

	GIVEN("an unknown type name")
	{
		THEN("returns TypeNotFound error")
		{
			auto result = config.resetMatchups("Shadow");
			REQUIRE_FALSE(result.has_value());
			CHECK((result.error().mKind == RegistryError::TypeNotFound));
		}
	}
}

SCENARIO("TypeRegistryConfiguration resetMatchups by stable id")
{
	ensureLoggerInitialized();
	TypeRegistryConfiguration config{};

	GIVEN("a valid stable type id")
	{
		THEN("clears all matchups to NOT_DEFINED")
		{
			ub normalId = static_cast<ub>(Types::Normal);
			auto result = config.resetMatchups(normalId);
			REQUIRE(result.has_value());

			auto row = config.getMatchupRow("Normal");
			REQUIRE(row.has_value());

			ub registered = config.getAmountRegistered();
			for (ub idx{0}; idx < registered; ++idx)
			{
				CHECK((row.value().at(idx) == NOT_DEFINED));
			}
		}
	}

	GIVEN("a nonexistent stable type id")
	{
		THEN("returns TypeNotFound error")
		{
			ub invalidId = 200;
			auto result = config.resetMatchups(invalidId);
			REQUIRE_FALSE(result.has_value());
			CHECK((result.error().mKind == RegistryError::TypeNotFound));
		}
	}
}

SCENARIO("TypeRegistryConfiguration hasType")
{
	ensureLoggerInitialized();
	TypeRegistryConfiguration config{};

	GIVEN("a builtin type name")
	{
		THEN("hasType by name returns true")
		{
			bool found = config.hasType("Fire");
			CHECK(found);
		}
	}

	GIVEN("a nonexistent type name")
	{
		THEN("hasType by name returns false")
		{
			bool found = config.hasType("Shadow");
			CHECK_FALSE(found);
		}
	}

	GIVEN("a builtin type id")
	{
		THEN("hasType by id returns true")
		{
			bool found = config.hasType(static_cast<ub>(Types::Fire));
			CHECK(found);
		}
	}

	GIVEN("a nonexistent type id")
	{
		THEN("hasType by id returns false")
		{
			ub invalidId = 200;
			bool found = config.hasType(invalidId);
			CHECK_FALSE(found);
		}
	}
}

SCENARIO("TypeRegistryConfiguration addType then removeType roundtrip")
{
	ensureLoggerInitialized();
	TypeRegistryConfiguration config{};

	GIVEN("a custom type is added")
	{
		WHEN("it is subsequently removed")
		{
			THEN("the registry count is restored")
			{
				// Remove Stellar to make room
				auto stellarRemove = config.removeType("Stellar");
				REQUIRE(stellarRemove.has_value());

				TypeDefinition definition{.name = "Cosmic", .offensiveMatchups = {}, .defensiveMatchups = {}};
				auto addResult = config.addType(definition, UnspecifiedMatchup::Neutral);
				REQUIRE(addResult.has_value());
				CHECK((config.getAmountRegistered() == 19));

				auto removeResult = config.removeType("Cosmic");
				REQUIRE(removeResult.has_value());
				CHECK((config.getAmountRegistered() == 18));
				CHECK_FALSE(config.hasType("Cosmic"));
			}
		}
	}
}

SCENARIO("TypeRegistryConfiguration RegistryErrorInfo fields")
{
	ensureLoggerInitialized();
	TypeRegistryConfiguration config{};

	GIVEN("an error from a failed operation")
	{
		THEN("error info contains the correct kind and context")
		{
			auto result = config.getMatchup("Shadow", "Normal");
			REQUIRE_FALSE(result.has_value());

			RegistryErrorInfo errorInfo = result.error();
			CHECK((errorInfo.mKind == RegistryError::TypeNotFound));
			CHECK((errorInfo.mContext == "Shadow"));
			CHECK((errorInfo.mErrorName == "TypeNotFound"));
		}
	}

	GIVEN("a MaxCapacity error kind")
	{
		THEN("errorKindToString returns MaxCapacity")
		{
			RegistryErrorInfo capError{RegistryError::MaxCapacity, "cap_context"};
			CHECK((capError.mErrorName == "MaxCapacity"));
		}
	}

	GIVEN("a DuplicateType error kind")
	{
		THEN("errorKindToString returns DuplicateType")
		{
			RegistryErrorInfo dupError{RegistryError::DuplicateType, "dup_context"};
			CHECK((dupError.mErrorName == "DuplicateType"));
		}
	}

	GIVEN("a MatchupMismatch error kind")
	{
		THEN("errorKindToString returns MatchupMismatch")
		{
			RegistryErrorInfo mmError{RegistryError::MatchupMismatch, "mm_context"};
			CHECK((mmError.mErrorName == "MatchupMismatch"));
		}
	}
}

SCENARIO("TypeRegistryConfiguration removeType by enum not found")
{
	ensureLoggerInitialized();
	TypeRegistryConfiguration config{};

	GIVEN("a type that has been removed by name")
	{
		THEN("removing it again by enum returns TypeNotFound")
		{
			auto firstRemove = config.removeType("Normal");
			REQUIRE(firstRemove.has_value());

			auto secondRemove = config.removeType(Types::Normal);
			REQUIRE_FALSE(secondRemove.has_value());
			CHECK((secondRemove.error().mKind == RegistryError::TypeNotFound));
		}
	}
}

SCENARIO("TypeRegistryConfiguration addTypes with bad matchup reference")
{
	ensureLoggerInitialized();
	TypeRegistryConfiguration config{};

	GIVEN("a batch where one definition has an invalid offensive matchup")
	{
		THEN("the entire batch is rolled back")
		{
			// Remove two types to make room for the batch
			auto removeResult1 = config.removeType("Stellar");
			REQUIRE(removeResult1.has_value());
			auto removeResult2 = config.removeType("Fairy");
			REQUIRE(removeResult2.has_value());

			ub countBefore = config.getAmountRegistered();

			std::array<MatchupPair, 1> badOffensive{{{.typeName = "NonExistent", .value = SE}}};

			TypeDefinition goodDef{.name = "Good", .offensiveMatchups = {}, .defensiveMatchups = {}};
			TypeDefinition badDef{.name = "Bad", .offensiveMatchups = badOffensive, .defensiveMatchups = {}};
			std::array<TypeDefinition, 2> defs{goodDef, badDef};

			auto result = config.addTypes(std::span<const TypeDefinition>{defs}, UnspecifiedMatchup::Neutral);
			REQUIRE_FALSE(result.has_value());
			CHECK((result.error().mKind == RegistryError::TypeNotFound));
			CHECK((config.getAmountRegistered() == countBefore));
			CHECK_FALSE(config.hasType("Good"));
			CHECK_FALSE(config.hasType("Bad"));
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
