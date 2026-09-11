/*! @file natureRegistry.test.cpp
	@brief C++ file for running tests for the NatureRegistry.
	@date 09/10/2026
	@since 0.12.20
	@version 0.12.20
	@author Matthew Moore
*/

#include "Registry/natureRegistry.h"

#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <utility>

#include "Configuration/constants.h"
#include "Core/typedefs.h"
#include "Nature/builtInNatureID.h"
#include "Nature/constants.h"
#include "Nature/natureID.h"
#include "Nature/natureMeta.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Configuration::NATURE_STAT_BASE_MULTIPLIER;
using PocketCore::Configuration::NATURE_STAT_BOOST_MULTIPLIER;
using PocketCore::Configuration::NATURE_STAT_WEAKNESS_MULTIPLIER;
using PocketCore::Core::ub;
using PocketCore::Nature::BuiltinNatureID;
using PocketCore::Nature::NATURE_NAME_ADAMANT;
using PocketCore::Nature::NATURE_NAME_BASHFUL;
using PocketCore::Nature::NATURE_NAME_BOLD;
using PocketCore::Nature::NATURE_NAME_BRAVE;
using PocketCore::Nature::NATURE_NAME_CALM;
using PocketCore::Nature::NATURE_NAME_CAREFUL;
using PocketCore::Nature::NATURE_NAME_DOCILE;
using PocketCore::Nature::NATURE_NAME_GENTLE;
using PocketCore::Nature::NATURE_NAME_HARDY;
using PocketCore::Nature::NATURE_NAME_HASTY;
using PocketCore::Nature::NATURE_NAME_IMPISH;
using PocketCore::Nature::NATURE_NAME_JOLLY;
using PocketCore::Nature::NATURE_NAME_LAX;
using PocketCore::Nature::NATURE_NAME_LONELY;
using PocketCore::Nature::NATURE_NAME_MILD;
using PocketCore::Nature::NATURE_NAME_MODEST;
using PocketCore::Nature::NATURE_NAME_NAIVE;
using PocketCore::Nature::NATURE_NAME_NAUGHTY;
using PocketCore::Nature::NATURE_NAME_NONE;
using PocketCore::Nature::NATURE_NAME_QUIET;
using PocketCore::Nature::NATURE_NAME_QUIRKY;
using PocketCore::Nature::NATURE_NAME_RASH;
using PocketCore::Nature::NATURE_NAME_RELAXED;
using PocketCore::Nature::NATURE_NAME_SASSY;
using PocketCore::Nature::NATURE_NAME_SERIOUS;
using PocketCore::Nature::NATURE_NAME_TIMID;
using PocketCore::Nature::NatureID;
using PocketCore::Nature::NatureMeta;
using PocketCore::Nature::NO_NATURE_ID;
using PocketCore::Nature::toNatureID;
using PocketCore::Registry::Nature::NatureRegistry;

template <typename Registry>
concept PubliclyStructurallyMutable = requires(Registry &registry) { registry.setAmountRegistered(0); };

static_assert(!PubliclyStructurallyMutable<NatureRegistry>);

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("NatureRegistry")
{
	NatureRegistry registry{};
	ub finalNatureUnderlyingValue{std::to_underlying(BuiltinNatureID::FinalNature)};

	GIVEN("a default constructed nature registry")
	{
		THEN("Hardy has the appropriate properties")
		{
			NatureMeta expected{
				.mStatMultipliers = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
				.mName = std::string(NATURE_NAME_HARDY),
				.mNatureID = toNatureID(BuiltinNatureID::Hardy),
			};

			const NatureMeta *actual{registry.getNatureMetadata(toNatureID(BuiltinNatureID::Hardy))};

			CHECK((expected == *actual));
		}

		THEN("Docile has the appropriate properties")
		{
			NatureMeta expected{
				.mStatMultipliers = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
				.mName = std::string(NATURE_NAME_DOCILE),
				.mNatureID = toNatureID(BuiltinNatureID::Docile),
			};

			const NatureMeta *actual{registry.getNatureMetadata(toNatureID(BuiltinNatureID::Docile))};

			CHECK((expected == *actual));
		}

		THEN("Serious has the appropriate properties")
		{
			NatureMeta expected{
				.mStatMultipliers = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
				.mName = std::string(NATURE_NAME_SERIOUS),
				.mNatureID = toNatureID(BuiltinNatureID::Serious),
			};

			const NatureMeta *actual{registry.getNatureMetadata(toNatureID(BuiltinNatureID::Serious))};

			CHECK((expected == *actual));
		}

		THEN("Bashful has the appropriate properties")
		{
			NatureMeta expected{
				.mStatMultipliers = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
				.mName = std::string(NATURE_NAME_BASHFUL),
				.mNatureID = toNatureID(BuiltinNatureID::Bashful),
			};

			const NatureMeta *actual{registry.getNatureMetadata(toNatureID(BuiltinNatureID::Bashful))};

			CHECK((expected == *actual));
		}

		THEN("Quirky has the appropriate properties")
		{
			NatureMeta expected{
				.mStatMultipliers = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
				.mName = std::string(NATURE_NAME_QUIRKY),
				.mNatureID = toNatureID(BuiltinNatureID::Quirky),
			};

			const NatureMeta *actual{registry.getNatureMetadata(toNatureID(BuiltinNatureID::Quirky))};

			CHECK((expected == *actual));
		}

		THEN("Lonely has the appropriate properties")
		{
			NatureMeta expected{
				.mStatMultipliers = {NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BOOST_MULTIPLIER, NATURE_STAT_WEAKNESS_MULTIPLIER,
									 NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER,},
				.mName = std::string(NATURE_NAME_LONELY),
				.mNatureID = toNatureID(BuiltinNatureID::Lonely),
			};

			const NatureMeta *actual{registry.getNatureMetadata(toNatureID(BuiltinNatureID::Lonely))};

			CHECK((expected == *actual));
		}

		THEN("Brave has the appropriate properties")
		{
			NatureMeta expected{
				.mStatMultipliers = {NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BOOST_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER,
									 NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_WEAKNESS_MULTIPLIER,},
				.mName = std::string(NATURE_NAME_BRAVE),
				.mNatureID = toNatureID(BuiltinNatureID::Brave),
			};

			const NatureMeta *actual{registry.getNatureMetadata(toNatureID(BuiltinNatureID::Brave))};

			CHECK((expected == *actual));
		}

		THEN("Adamant has the appropriate properties")
		{
			NatureMeta expected{
				.mStatMultipliers = {NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BOOST_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER,
									 NATURE_STAT_WEAKNESS_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER,},
				.mName = std::string(NATURE_NAME_ADAMANT),
				.mNatureID = toNatureID(BuiltinNatureID::Adamant),
			};

			const NatureMeta *actual{registry.getNatureMetadata(toNatureID(BuiltinNatureID::Adamant))};

			CHECK((expected == *actual));
		}

		THEN("Naughty has the appropriate properties")
		{
			NatureMeta expected{
				.mStatMultipliers = {NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BOOST_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER,
									 NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_WEAKNESS_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER,},
				.mName = std::string(NATURE_NAME_NAUGHTY),
				.mNatureID = toNatureID(BuiltinNatureID::Naughty),
			};

			const NatureMeta *actual{registry.getNatureMetadata(toNatureID(BuiltinNatureID::Naughty))};

			CHECK((expected == *actual));
		}

		THEN("Bold has the appropriate properties")
		{
			NatureMeta expected{
				.mStatMultipliers = {NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_WEAKNESS_MULTIPLIER, NATURE_STAT_BOOST_MULTIPLIER,
									 NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER,},
				.mName = std::string(NATURE_NAME_BOLD),
				.mNatureID = toNatureID(BuiltinNatureID::Bold),
			};

			const NatureMeta *actual{registry.getNatureMetadata(toNatureID(BuiltinNatureID::Bold))};

			CHECK((expected == *actual));
		}

		THEN("Relaxed has the appropriate properties")
		{
			NatureMeta expected{
				.mStatMultipliers = {NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BOOST_MULTIPLIER,
									 NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_WEAKNESS_MULTIPLIER,},
				.mName = std::string(NATURE_NAME_RELAXED),
				.mNatureID = toNatureID(BuiltinNatureID::Relaxed),
			};

			const NatureMeta *actual{registry.getNatureMetadata(toNatureID(BuiltinNatureID::Relaxed))};

			CHECK((expected == *actual));
		}

		THEN("Impish has the appropriate properties")
		{
			NatureMeta expected{
				.mStatMultipliers = {NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BOOST_MULTIPLIER,
									 NATURE_STAT_WEAKNESS_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER,},
				.mName = std::string(NATURE_NAME_IMPISH),
				.mNatureID = toNatureID(BuiltinNatureID::Impish),
			};

			const NatureMeta *actual{registry.getNatureMetadata(toNatureID(BuiltinNatureID::Impish))};

			CHECK((expected == *actual));
		}

		THEN("Lax has the appropriate properties")
		{
			NatureMeta expected{
				.mStatMultipliers = {NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BOOST_MULTIPLIER,
									 NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_WEAKNESS_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER,},
				.mName = std::string(NATURE_NAME_LAX),
				.mNatureID = toNatureID(BuiltinNatureID::Lax),
			};

			const NatureMeta *actual{registry.getNatureMetadata(toNatureID(BuiltinNatureID::Lax))};

			CHECK((expected == *actual));
		}

		THEN("Timid has the appropriate properties")
		{
			NatureMeta expected{
				.mStatMultipliers = {NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_WEAKNESS_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER,
									 NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BOOST_MULTIPLIER,},
				.mName = std::string(NATURE_NAME_TIMID),
				.mNatureID = toNatureID(BuiltinNatureID::Timid),
			};

			const NatureMeta *actual{registry.getNatureMetadata(toNatureID(BuiltinNatureID::Timid))};

			CHECK((expected == *actual));
		}

		THEN("Hasty has the appropriate properties")
		{
			NatureMeta expected{
				.mStatMultipliers = {NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_WEAKNESS_MULTIPLIER,
									 NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BOOST_MULTIPLIER,},
				.mName = std::string(NATURE_NAME_HASTY),
				.mNatureID = toNatureID(BuiltinNatureID::Hasty),
			};

			const NatureMeta *actual{registry.getNatureMetadata(toNatureID(BuiltinNatureID::Hasty))};

			CHECK((expected == *actual));
		}

		THEN("Jolly has the appropriate properties")
		{
			NatureMeta expected{
				.mStatMultipliers = {NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER,
									 NATURE_STAT_WEAKNESS_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BOOST_MULTIPLIER,},
				.mName = std::string(NATURE_NAME_JOLLY),
				.mNatureID = toNatureID(BuiltinNatureID::Jolly),
			};

			const NatureMeta *actual{registry.getNatureMetadata(toNatureID(BuiltinNatureID::Jolly))};

			CHECK((expected == *actual));
		}

		THEN("Naive has the appropriate properties")
		{
			NatureMeta expected{
				.mStatMultipliers = {NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER,
									 NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_WEAKNESS_MULTIPLIER, NATURE_STAT_BOOST_MULTIPLIER,},
				.mName = std::string(NATURE_NAME_NAIVE),
				.mNatureID = toNatureID(BuiltinNatureID::Naive),
			};

			const NatureMeta *actual{registry.getNatureMetadata(toNatureID(BuiltinNatureID::Naive))};

			CHECK((expected == *actual));
		}

		THEN("Modest has the appropriate properties")
		{
			NatureMeta expected{
				.mStatMultipliers = {NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_WEAKNESS_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER,
									 NATURE_STAT_BOOST_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER,},
				.mName = std::string(NATURE_NAME_MODEST),
				.mNatureID = toNatureID(BuiltinNatureID::Modest),
			};

			const NatureMeta *actual{registry.getNatureMetadata(toNatureID(BuiltinNatureID::Modest))};

			CHECK((expected == *actual));
		}

		THEN("Mild has the appropriate properties")
		{
			NatureMeta expected{
				.mStatMultipliers = {NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_WEAKNESS_MULTIPLIER,
									 NATURE_STAT_BOOST_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER,},
				.mName = std::string(NATURE_NAME_MILD),
				.mNatureID = toNatureID(BuiltinNatureID::Mild),
			};

			const NatureMeta *actual{registry.getNatureMetadata(toNatureID(BuiltinNatureID::Mild))};

			CHECK((expected == *actual));
		}

		THEN("Quiet has the appropriate properties")
		{
			NatureMeta expected{
				.mStatMultipliers = {NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER,
									 NATURE_STAT_BOOST_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_WEAKNESS_MULTIPLIER,},
				.mName = std::string(NATURE_NAME_QUIET),
				.mNatureID = toNatureID(BuiltinNatureID::Quiet),
			};

			const NatureMeta *actual{registry.getNatureMetadata(toNatureID(BuiltinNatureID::Quiet))};

			CHECK((expected == *actual));
		}

		THEN("Rash has the appropriate properties")
		{
			NatureMeta expected{
				.mStatMultipliers = {NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER,
									 NATURE_STAT_BOOST_MULTIPLIER, NATURE_STAT_WEAKNESS_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER,},
				.mName = std::string(NATURE_NAME_RASH),
				.mNatureID = toNatureID(BuiltinNatureID::Rash),
			};

			const NatureMeta *actual{registry.getNatureMetadata(toNatureID(BuiltinNatureID::Rash))};

			CHECK((expected == *actual));
		}

		THEN("Calm has the appropriate properties")
		{
			NatureMeta expected{
				.mStatMultipliers = {NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_WEAKNESS_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER,
									 NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BOOST_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER,},
				.mName = std::string(NATURE_NAME_CALM),
				.mNatureID = toNatureID(BuiltinNatureID::Calm),
			};

			const NatureMeta *actual{registry.getNatureMetadata(toNatureID(BuiltinNatureID::Calm))};

			CHECK((expected == *actual));
		}

		THEN("Gentle has the appropriate properties")
		{
			NatureMeta expected{
				.mStatMultipliers = {NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_WEAKNESS_MULTIPLIER,
									 NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BOOST_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER,},
				.mName = std::string(NATURE_NAME_GENTLE),
				.mNatureID = toNatureID(BuiltinNatureID::Gentle),
			};

			const NatureMeta *actual{registry.getNatureMetadata(toNatureID(BuiltinNatureID::Gentle))};

			CHECK((expected == *actual));
		}

		THEN("Sassy has the appropriate properties")
		{
			NatureMeta expected{
				.mStatMultipliers = {NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER,
									 NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BOOST_MULTIPLIER, NATURE_STAT_WEAKNESS_MULTIPLIER,},
				.mName = std::string(NATURE_NAME_SASSY),
				.mNatureID = toNatureID(BuiltinNatureID::Sassy),
			};

			const NatureMeta *actual{registry.getNatureMetadata(toNatureID(BuiltinNatureID::Sassy))};

			CHECK((expected == *actual));
		}

		THEN("Careful has the appropriate properties")
		{
			NatureMeta expected{
				.mStatMultipliers = {NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER,
									 NATURE_STAT_WEAKNESS_MULTIPLIER, NATURE_STAT_BOOST_MULTIPLIER, NATURE_STAT_BASE_MULTIPLIER,},
				.mName = std::string(NATURE_NAME_CAREFUL),
				.mNatureID = toNatureID(BuiltinNatureID::Careful),
			};

			const NatureMeta *actual{registry.getNatureMetadata(toNatureID(BuiltinNatureID::Careful))};

			CHECK((expected == *actual));
		}
	}

	GIVEN("getNatureMetadata")
	{
		THEN("unknown IDs are absent")
		{
			CHECK((registry.getNatureMetadata(NatureID{200}) == nullptr));
		}

		THEN("the metadata is retrieved when accessed by a valid Nature ID")
		{
			NatureMeta expected{
				.mName = std::string(NATURE_NAME_NONE),
				.mNatureID = toNatureID(BuiltinNatureID::None),
			};

			CHECK((expected == *registry.getNatureMetadata(NO_NATURE_ID)));
		}
	}

	GIVEN("getNatureID")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(registry.getNatureID("Unknown").has_value());
		}

		THEN("the Nature ID is retrieved by valid Nature name")
		{
			std::optional<NatureID> natureID{registry.getNatureID(NATURE_NAME_NONE)};

			REQUIRE(natureID.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((natureID.value() == toNatureID(BuiltinNatureID::None)));
		}
	}

	GIVEN("getNatureName")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(registry.getNatureName(NatureID{200}).has_value());
		}

		THEN("a registered nature name is returned by stable ID")
		{
			std::optional<std::string_view> natureName{registry.getNatureName(toNatureID(BuiltinNatureID::None))};

			REQUIRE(natureName.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((natureName.value() == NATURE_NAME_NONE));
		}
	}

	GIVEN("getAmountRegistered")
	{
		THEN("the registered span contains the exact amount of built-in entries")
		{
			CHECK((registry.getAmountRegistered() == finalNatureUnderlyingValue));
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
			NatureMeta expected{
				.mName = std::string(NATURE_NAME_NONE),
				.mNatureID = toNatureID(BuiltinNatureID::None),
			};

			const NatureMeta *natureMeta{registry.getEntry(0)};

			REQUIRE((natureMeta != nullptr));
			CHECK((*natureMeta == expected));
		}
	}

	GIVEN("getRegisteredNatures")
	{
		THEN("the amount of natures returned matches the amount that are built-in")
		{
			CHECK((registry.getRegisteredNatures().size() == finalNatureUnderlyingValue));
		}
	}

	GIVEN("getNextNatureID")
	{
		THEN("the next available stable Nature ID is after all built in nature IDs")
		{
			CHECK((registry.getNextNatureID() == finalNatureUnderlyingValue));
		}
	}

	GIVEN("findIndexByNatureID")
	{
		THEN("an unknown stable ID has no internal index")
		{
			std::optional<ub> natureIndex{registry.findIndexByNatureID(NatureID{200})};
			CHECK_FALSE(natureIndex.has_value());
		}

		THEN("the internal array index is retrieved by valid Nature ID")
		{
			std::optional<ub> natureIndex{registry.findIndexByNatureID(NO_NATURE_ID)};

			REQUIRE(natureIndex.has_value());
			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((natureIndex.value() == 0));
		}
	}

	GIVEN("hasNature")
	{
		WHEN("calling the string_view overload")
		{
			THEN("an unknown nature name has no entry")
			{
				CHECK_FALSE(registry.hasNature("Unknown"));
			}

			THEN("a known nature name has an entry")
			{
				CHECK(registry.hasNature(NATURE_NAME_NONE));
			}
		}

		WHEN("calling the NatureID overload")
		{
			THEN("an unknown nature ID has no entry")
			{
				CHECK_FALSE(registry.hasNature(NatureID{200}));
			}

			THEN("a known nature ID has an entry")
			{
				CHECK(registry.hasNature(NO_NATURE_ID));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
