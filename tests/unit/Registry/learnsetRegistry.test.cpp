/*! @file learnsetRegistry.test.cpp
	@brief C++ file for running tests for the LearnsetRegistry.
	@date 08/08/2026
	@since 0.12.50
	@version 0.12.50
	@author Matthew Moore
*/

#include "Registry/learnsetRegistry.h"

#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <utility>

#include "Core/typedefs.h"
#include "Learnset/builtInLearnsetID.h"
#include "Learnset/constants.h"
#include "Learnset/learnsetID.h"
#include "Learnset/learnsetMeta.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Core::ub;
using PocketCore::Learnset::BuiltinLearnsetID;
using PocketCore::Learnset::LEARNSET_NAME_BLASTOISE;
using PocketCore::Learnset::LEARNSET_NAME_BULBASAUR;
using PocketCore::Learnset::LEARNSET_NAME_CHARIZARD;
using PocketCore::Learnset::LEARNSET_NAME_CHARMANDER;
using PocketCore::Learnset::LEARNSET_NAME_CHARMELEON;
using PocketCore::Learnset::LEARNSET_NAME_IVYSAUR;
using PocketCore::Learnset::LEARNSET_NAME_NONE;
using PocketCore::Learnset::LEARNSET_NAME_SQUIRTLE;
using PocketCore::Learnset::LEARNSET_NAME_VENUSAUR;
using PocketCore::Learnset::LEARNSET_NAME_WARTORTLE;
using PocketCore::Learnset::LearnsetID;
using PocketCore::Learnset::LearnsetMeta;
using PocketCore::Learnset::NO_LEARNSET_ID;
using PocketCore::Learnset::toLearnsetID;
using PocketCore::Registry::Learnset::LearnsetRegistry;

template <typename Registry>
concept PubliclyStructurallyMutable = requires(Registry &registry) { registry.setAmountRegistered(0); };

static_assert(!PubliclyStructurallyMutable<LearnsetRegistry>);

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("LearnsetRegistry")
{
	LearnsetRegistry registry{};
	ub finalLearnsetUnderlyingValue{std::to_underlying(BuiltinLearnsetID::FinalLearnset)};

	GIVEN("a default constructed learnset registry")
	{
		THEN("Bulbasaur Learnset has the appropriate properties")
		{
			LearnsetMeta expected{
				.mName = std::string(LEARNSET_NAME_BULBASAUR),
				.mLearnsetID = toLearnsetID(BuiltinLearnsetID::BulbasaurLearnset),
			};

			const LearnsetMeta *actual{registry.getLearnsetMetadata(toLearnsetID(BuiltinLearnsetID::BulbasaurLearnset))};

			CHECK((expected == *actual));
		}

		THEN("Ivysaur Learnset has the appropriate properties")
		{
			LearnsetMeta expected{
				.mName = std::string(LEARNSET_NAME_IVYSAUR),
				.mLearnsetID = toLearnsetID(BuiltinLearnsetID::IvysaurLearnset),
			};

			const LearnsetMeta *actual{registry.getLearnsetMetadata(toLearnsetID(BuiltinLearnsetID::IvysaurLearnset))};

			CHECK((expected == *actual));
		}

		THEN("Venusaur Learnset has the appropriate properties")
		{
			LearnsetMeta expected{
				.mName = std::string(LEARNSET_NAME_VENUSAUR),
				.mLearnsetID = toLearnsetID(BuiltinLearnsetID::VenusaurLearnset),
			};

			const LearnsetMeta *actual{registry.getLearnsetMetadata(toLearnsetID(BuiltinLearnsetID::VenusaurLearnset))};

			CHECK((expected == *actual));
		}

		THEN("Squirtle Learnset has the appropriate properties")
		{
			LearnsetMeta expected{
				.mName = std::string(LEARNSET_NAME_SQUIRTLE),
				.mLearnsetID = toLearnsetID(BuiltinLearnsetID::SquirtleLearnset),
			};

			const LearnsetMeta *actual{registry.getLearnsetMetadata(toLearnsetID(BuiltinLearnsetID::SquirtleLearnset))};

			CHECK((expected == *actual));
		}

		THEN("Wartortle Learnset has the appropriate properties")
		{
			LearnsetMeta expected{
				.mName = std::string(LEARNSET_NAME_WARTORTLE),
				.mLearnsetID = toLearnsetID(BuiltinLearnsetID::WartortleLearnset),
			};

			const LearnsetMeta *actual{registry.getLearnsetMetadata(toLearnsetID(BuiltinLearnsetID::WartortleLearnset))};

			CHECK((expected == *actual));
		}

		THEN("Blastoise Learnset has the appropriate properties")
		{
			LearnsetMeta expected{
				.mName = std::string(LEARNSET_NAME_BLASTOISE),
				.mLearnsetID = toLearnsetID(BuiltinLearnsetID::BlastoiseLearnset),
			};

			const LearnsetMeta *actual{registry.getLearnsetMetadata(toLearnsetID(BuiltinLearnsetID::BlastoiseLearnset))};

			CHECK((expected == *actual));
		}

		THEN("Charmander Learnset has the appropriate properties")
		{
			LearnsetMeta expected{
				.mName = std::string(LEARNSET_NAME_CHARMANDER),
				.mLearnsetID = toLearnsetID(BuiltinLearnsetID::CharmanderLearnset),
			};

			const LearnsetMeta *actual{registry.getLearnsetMetadata(toLearnsetID(BuiltinLearnsetID::CharmanderLearnset))};

			CHECK((expected == *actual));
		}

		THEN("Charmeleon Learnset has the appropriate properties")
		{
			LearnsetMeta expected{
				.mName = std::string(LEARNSET_NAME_CHARMELEON),
				.mLearnsetID = toLearnsetID(BuiltinLearnsetID::CharmeleonLearnset),
			};

			const LearnsetMeta *actual{registry.getLearnsetMetadata(toLearnsetID(BuiltinLearnsetID::CharmeleonLearnset))};

			CHECK((expected == *actual));
		}

		THEN("Charizard Learnset has the appropriate properties")
		{
			LearnsetMeta expected{
				.mName = std::string(LEARNSET_NAME_CHARIZARD),
				.mLearnsetID = toLearnsetID(BuiltinLearnsetID::CharizardLearnset),
			};

			const LearnsetMeta *actual{registry.getLearnsetMetadata(toLearnsetID(BuiltinLearnsetID::CharizardLearnset))};

			CHECK((expected == *actual));
		}
	}

	GIVEN("getLearnsetMetadata")
	{
		THEN("unknown IDs are absent")
		{
			CHECK((registry.getLearnsetMetadata(LearnsetID{200}) == nullptr));
		}

		THEN("the metadata is retrieved when accessed by a valid Learnset ID")
		{
			LearnsetMeta expected{
				.mName = std::string(LEARNSET_NAME_NONE),
				.mLearnsetID = toLearnsetID(BuiltinLearnsetID::None),
			};

			CHECK((expected == *registry.getLearnsetMetadata(NO_LEARNSET_ID)));
		}
	}

	GIVEN("getLearnsetID")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(registry.getLearnsetID("Unknown").has_value());
		}

		THEN("the Learnset ID is retrieved by valid Learnset name")
		{
			std::optional<LearnsetID> learnsetID{registry.getLearnsetID(LEARNSET_NAME_NONE)};

			REQUIRE(learnsetID.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((learnsetID.value() == toLearnsetID(BuiltinLearnsetID::None)));
		}
	}

	GIVEN("getLearnsetName")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(registry.getLearnsetName(LearnsetID{200}).has_value());
		}

		THEN("a registered learnset name is returned by stable ID")
		{
			std::optional<std::string_view> learnsetName{registry.getLearnsetName(toLearnsetID(BuiltinLearnsetID::None))};

			REQUIRE(learnsetName.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((learnsetName.value() == LEARNSET_NAME_NONE));
		}
	}

	GIVEN("getAmountRegistered")
	{
		THEN("the registered span contains the exact amount of built-in entries")
		{
			CHECK((registry.getAmountRegistered() == finalLearnsetUnderlyingValue));
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
			LearnsetMeta expected{
				.mName = std::string(LEARNSET_NAME_NONE),
				.mLearnsetID = toLearnsetID(BuiltinLearnsetID::None),
			};

			const LearnsetMeta *learnsetMeta{registry.getEntry(0)};

			REQUIRE((learnsetMeta != nullptr));
			CHECK((*learnsetMeta == expected));
		}
	}

	GIVEN("getRegisteredLearnsets")
	{
		THEN("the amount of learnsets returned matches the amount that are built-in")
		{
			CHECK((registry.getRegisteredLearnsets().size() == finalLearnsetUnderlyingValue));
		}
	}

	GIVEN("getNextLearnsetID")
	{
		THEN("the next available stable Learnset ID is after all built in learnset IDs")
		{
			CHECK((registry.getNextLearnsetID() == finalLearnsetUnderlyingValue));
		}
	}

	GIVEN("findIndexByLearnsetID")
	{
		THEN("an unknown stable ID has no internal index")
		{
			std::optional<ub> learnsetIndex{registry.findIndexByLearnsetID(LearnsetID{200})};
			CHECK_FALSE(learnsetIndex.has_value());
		}

		THEN("the internal array index is retrieved by valid Learnset ID")
		{
			std::optional<ub> learnsetIndex{registry.findIndexByLearnsetID(NO_LEARNSET_ID)};

			REQUIRE(learnsetIndex.has_value());
			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((learnsetIndex.value() == 0));
		}
	}

	GIVEN("hasLearnset")
	{
		WHEN("calling the string_view overload")
		{
			THEN("an unknown learnset name has no entry")
			{
				CHECK_FALSE(registry.hasLearnset("Unknown"));
			}

			THEN("a known learnset name has an entry")
			{
				CHECK(registry.hasLearnset(LEARNSET_NAME_NONE));
			}
		}

		WHEN("calling the LearnsetID overload")
		{
			THEN("an unknown learnset ID has no entry")
			{
				CHECK_FALSE(registry.hasLearnset(LearnsetID{200}));
			}

			THEN("a known learnset ID has an entry")
			{
				CHECK(registry.hasLearnset(NO_LEARNSET_ID));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
