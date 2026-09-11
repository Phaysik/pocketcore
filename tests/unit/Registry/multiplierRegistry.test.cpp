/*! @file multiplierRegistry.test.cpp
	@brief C++ file for running tests for the MultiplierRegistry.
	@date 09/10/2026
	@since 0.12.20
	@version 0.12.20
	@author Matthew Moore
*/

#include "Registry/multiplierRegistry.h"

#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <utility>

#include "Core/typedefs.h"
#include "Multiplier/builtInMultiplierID.h"
#include "Multiplier/constants.h"
#include "Multiplier/multiplierID.h"
#include "Multiplier/multiplierMeta.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Core::ub;
using PocketCore::Multiplier::BuiltinMultiplierID;
using PocketCore::Multiplier::MULTIPLIER_NAME_ABILITY;
using PocketCore::Multiplier::MULTIPLIER_NAME_BURN;
using PocketCore::Multiplier::MULTIPLIER_NAME_CRITICAL;
using PocketCore::Multiplier::MULTIPLIER_NAME_ITEM;
using PocketCore::Multiplier::MULTIPLIER_NAME_NONE;
using PocketCore::Multiplier::MULTIPLIER_NAME_POPULATION_BOMB;
using PocketCore::Multiplier::MULTIPLIER_NAME_RANDOMIZATION;
using PocketCore::Multiplier::MULTIPLIER_NAME_STAB;
using PocketCore::Multiplier::MULTIPLIER_NAME_TARGETS;
using PocketCore::Multiplier::MULTIPLIER_NAME_TYPE_EFFECTIVENESS;
using PocketCore::Multiplier::MULTIPLIER_NAME_WEATHER;
using PocketCore::Multiplier::MultiplierApplicationPolicy;
using PocketCore::Multiplier::MultiplierID;
using PocketCore::Multiplier::MultiplierMeta;
using PocketCore::Multiplier::NO_MULTIPLIER_ID;
using PocketCore::Multiplier::toMultiplierID;
using PocketCore::Registry::Multiplier::MultiplierRegistry;

template <typename Registry>
concept PubliclyStructurallyMutable = requires(Registry &registry) { registry.setAmountRegistered(0); };

static_assert(!PubliclyStructurallyMutable<MultiplierRegistry>);

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("MultiplierRegistry")
{
	MultiplierRegistry registry{};
	ub finalMultiplierUnderlyingValue{std::to_underlying(BuiltinMultiplierID::FinalMultiplier)};

	GIVEN("a default constructed multiplier registry")
	{
		THEN("Ability has the appropriate properties")
		{
			MultiplierMeta expected{
				.mName = std::string(MULTIPLIER_NAME_ABILITY),
				.mMultiplierID = toMultiplierID(BuiltinMultiplierID::Ability),
				.mApplicationPolicy = MultiplierApplicationPolicy::Other,
			};

			const MultiplierMeta *actual{registry.getMultiplierMetadata(toMultiplierID(BuiltinMultiplierID::Ability))};

			CHECK((expected == *actual));
		}

		THEN("Item has the appropriate properties")
		{
			MultiplierMeta expected{
				.mName = std::string(MULTIPLIER_NAME_ITEM),
				.mMultiplierID = toMultiplierID(BuiltinMultiplierID::Item),
				.mApplicationPolicy = MultiplierApplicationPolicy::Other,
			};

			const MultiplierMeta *actual{registry.getMultiplierMetadata(toMultiplierID(BuiltinMultiplierID::Item))};

			CHECK((expected == *actual));
		}

		THEN("Targets has the appropriate properties")
		{
			MultiplierMeta expected{
				.mName = std::string(MULTIPLIER_NAME_TARGETS),
				.mMultiplierID = toMultiplierID(BuiltinMultiplierID::Targets),
			};

			const MultiplierMeta *actual{registry.getMultiplierMetadata(toMultiplierID(BuiltinMultiplierID::Targets))};

			CHECK((expected == *actual));
		}

		THEN("Population Bomb has the appropriate properties")
		{
			MultiplierMeta expected{
				.mName = std::string(MULTIPLIER_NAME_POPULATION_BOMB),
				.mMultiplierID = toMultiplierID(BuiltinMultiplierID::PopulationBomb),
			};

			const MultiplierMeta *actual{registry.getMultiplierMetadata(toMultiplierID(BuiltinMultiplierID::PopulationBomb))};

			CHECK((expected == *actual));
		}

		THEN("Weather has the appropriate properties")
		{
			MultiplierMeta expected{
				.mName = std::string(MULTIPLIER_NAME_WEATHER),
				.mMultiplierID = toMultiplierID(BuiltinMultiplierID::Weather),
			};

			const MultiplierMeta *actual{registry.getMultiplierMetadata(toMultiplierID(BuiltinMultiplierID::Weather))};

			CHECK((expected == *actual));
		}

		THEN("Critical has the appropriate properties")
		{
			MultiplierMeta expected{
				.mName = std::string(MULTIPLIER_NAME_CRITICAL),
				.mMultiplierID = toMultiplierID(BuiltinMultiplierID::Critical),
				.mApplicationPolicy = MultiplierApplicationPolicy::Floor,
			};

			const MultiplierMeta *actual{registry.getMultiplierMetadata(toMultiplierID(BuiltinMultiplierID::Critical))};

			CHECK((expected == *actual));
		}

		THEN("Randomization has the appropriate properties")
		{
			MultiplierMeta expected{
				.mName = std::string(MULTIPLIER_NAME_RANDOMIZATION),
				.mMultiplierID = toMultiplierID(BuiltinMultiplierID::Randomization),
				.mApplicationPolicy = MultiplierApplicationPolicy::Floor,
			};

			const MultiplierMeta *actual{registry.getMultiplierMetadata(toMultiplierID(BuiltinMultiplierID::Randomization))};

			CHECK((expected == *actual));
		}

		THEN("STAB has the appropriate properties")
		{
			MultiplierMeta expected{
				.mName = std::string(MULTIPLIER_NAME_STAB),
				.mMultiplierID = toMultiplierID(BuiltinMultiplierID::Stab),
			};

			const MultiplierMeta *actual{registry.getMultiplierMetadata(toMultiplierID(BuiltinMultiplierID::Stab))};

			CHECK((expected == *actual));
		}

		THEN("Type Effectiveness has the appropriate properties")
		{
			MultiplierMeta expected{
				.mName = std::string(MULTIPLIER_NAME_TYPE_EFFECTIVENESS),
				.mMultiplierID = toMultiplierID(BuiltinMultiplierID::TypeEffectiveness),
				.mApplicationPolicy = MultiplierApplicationPolicy::Floor,
			};

			const MultiplierMeta *actual{registry.getMultiplierMetadata(toMultiplierID(BuiltinMultiplierID::TypeEffectiveness))};

			CHECK((expected == *actual));
		}

		THEN("Burn has the appropriate properties")
		{
			MultiplierMeta expected{
				.mName = std::string(MULTIPLIER_NAME_BURN),
				.mMultiplierID = toMultiplierID(BuiltinMultiplierID::Burn),
			};

			const MultiplierMeta *actual{registry.getMultiplierMetadata(toMultiplierID(BuiltinMultiplierID::Burn))};

			CHECK((expected == *actual));
		}
	}

	GIVEN("getMultiplierMetadata")
	{
		THEN("unknown IDs are absent")
		{
			CHECK((registry.getMultiplierMetadata(MultiplierID{200}) == nullptr));
		}

		THEN("the metadata is retrieved when accessed by a valid Multiplier ID")
		{
			MultiplierMeta expected{
				.mName = std::string(MULTIPLIER_NAME_NONE),
				.mMultiplierID = toMultiplierID(BuiltinMultiplierID::None),
			};

			CHECK((expected == *registry.getMultiplierMetadata(NO_MULTIPLIER_ID)));
		}
	}

	GIVEN("getMultiplierID")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(registry.getMultiplierID("Unknown").has_value());
		}

		THEN("the Multiplier ID is retrieved by valid Multiplier name")
		{
			std::optional<MultiplierID> multiplierID{registry.getMultiplierID(MULTIPLIER_NAME_NONE)};

			REQUIRE(multiplierID.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((multiplierID.value() == toMultiplierID(BuiltinMultiplierID::None)));
		}
	}

	GIVEN("getMultiplierName")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(registry.getMultiplierName(MultiplierID{200}).has_value());
		}

		THEN("a registered multiplier name is returned by stable ID")
		{
			std::optional<std::string_view> multiplierName{registry.getMultiplierName(toMultiplierID(BuiltinMultiplierID::None))};

			REQUIRE(multiplierName.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((multiplierName.value() == MULTIPLIER_NAME_NONE));
		}
	}

	GIVEN("getAmountRegistered")
	{
		THEN("the registered span contains the exact amount of built-in entries")
		{
			CHECK((registry.getAmountRegistered() == finalMultiplierUnderlyingValue));
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
			MultiplierMeta expected{
				.mName = std::string(MULTIPLIER_NAME_NONE),
				.mMultiplierID = toMultiplierID(BuiltinMultiplierID::None),
			};

			const MultiplierMeta *multiplierMeta{registry.getEntry(0)};

			REQUIRE((multiplierMeta != nullptr));
			CHECK((*multiplierMeta == expected));
		}
	}

	GIVEN("getRegisteredMultipliers")
	{
		THEN("the amount of multipliers returned matches the amount that are built-in")
		{
			CHECK((registry.getRegisteredMultipliers().size() == finalMultiplierUnderlyingValue));
		}
	}

	GIVEN("getNextMultiplierID")
	{
		THEN("the next available stable Multiplier ID is after all built in multiplier IDs")
		{
			CHECK((registry.getNextMultiplierID() == finalMultiplierUnderlyingValue));
		}
	}

	GIVEN("findIndexByMultiplierID")
	{
		THEN("an unknown stable ID has no internal index")
		{
			std::optional<ub> multiplierIndex{registry.findIndexByMultiplierID(MultiplierID{200})};
			CHECK_FALSE(multiplierIndex.has_value());
		}

		THEN("the internal array index is retrieved by valid Multiplier ID")
		{
			std::optional<ub> multiplierIndex{registry.findIndexByMultiplierID(NO_MULTIPLIER_ID)};

			REQUIRE(multiplierIndex.has_value());
			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((multiplierIndex.value() == 0));
		}
	}

	GIVEN("hasMultiplier")
	{
		WHEN("calling the string_view overload")
		{
			THEN("an unknown multiplier name has no entry")
			{
				CHECK_FALSE(registry.hasMultiplier("Unknown"));
			}

			THEN("a known multiplier name has an entry")
			{
				CHECK(registry.hasMultiplier(MULTIPLIER_NAME_NONE));
			}
		}

		WHEN("calling the MultiplierID overload")
		{
			THEN("an unknown multiplier ID has no entry")
			{
				CHECK_FALSE(registry.hasMultiplier(MultiplierID{200}));
			}

			THEN("a known multiplier ID has an entry")
			{
				CHECK(registry.hasMultiplier(NO_MULTIPLIER_ID));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
