/*! @file eggGroupRegistry.test.cpp
	@brief C++ file for running tests for the EggGroupRegistry.
	@date 10/28/2026
	@since 0.12.53
	@version 0.12.53
	@author Matthew Moore
*/

#include "Registry/eggGroupRegistry.h"

#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <utility>

#include "Configuration/eggGroupRegistryConfiguration.h"
#include "Core/typedefs.h"
#include "EggGroup/builtInEggGroupID.h"
#include "EggGroup/constants.h"
#include "EggGroup/eggGroupID.h"
#include "EggGroup/eggGroupMeta.h"
#include "Validation/Registry/registryError.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Configuration::EggGroupRegistryConfiguration;
using PocketCore::Core::ub;
using PocketCore::EggGroup::BuiltinEggGroupID;
using PocketCore::EggGroup::EGG_GROUP_NAME_AMORPHOUS;
using PocketCore::EggGroup::EGG_GROUP_NAME_BUG;
using PocketCore::EggGroup::EGG_GROUP_NAME_DITTO;
using PocketCore::EggGroup::EGG_GROUP_NAME_DRAGON;
using PocketCore::EggGroup::EGG_GROUP_NAME_FAIRY;
using PocketCore::EggGroup::EGG_GROUP_NAME_FIELD;
using PocketCore::EggGroup::EGG_GROUP_NAME_FLYING;
using PocketCore::EggGroup::EGG_GROUP_NAME_GRASS;
using PocketCore::EggGroup::EGG_GROUP_NAME_HUMAN_LIKE;
using PocketCore::EggGroup::EGG_GROUP_NAME_MINERAL;
using PocketCore::EggGroup::EGG_GROUP_NAME_MONSTER;
using PocketCore::EggGroup::EGG_GROUP_NAME_NO_EGGS;
using PocketCore::EggGroup::EGG_GROUP_NAME_NONE;
using PocketCore::EggGroup::EGG_GROUP_NAME_WATER1;
using PocketCore::EggGroup::EGG_GROUP_NAME_WATER2;
using PocketCore::EggGroup::EGG_GROUP_NAME_WATER3;
using PocketCore::EggGroup::EggGroupID;
using PocketCore::EggGroup::EggGroupMeta;
using PocketCore::EggGroup::NO_EGG_GROUP_ID;
using PocketCore::EggGroup::toEggGroupID;
using PocketCore::Registry::EggGroup::EggGroupRegistry;
using PocketCore::Validation::Registry::RegistryError;

template <typename Registry>
concept PubliclyStructurallyMutable = requires(Registry &registry) { registry.setAmountRegistered(0); };

static_assert(!PubliclyStructurallyMutable<EggGroupRegistry>);

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("EggGroupRegistry")
{
	std::unique_ptr<EggGroupRegistry> registryStorage{std::make_unique<EggGroupRegistry>()};
	EggGroupRegistry &registry{*registryStorage};
	ub finalEggGroupUnderlyingValue{std::to_underlying(BuiltinEggGroupID::FinalEggGroup)};

	GIVEN("a default constructed eggGroup registry")
	{
		THEN("Amorphous EggGroup has the appropriate properties")
		{
			EggGroupMeta expected{
				.mName = std::string(EGG_GROUP_NAME_AMORPHOUS),
				.mEggGroupID = toEggGroupID(BuiltinEggGroupID::Amorphous),
			};

			const EggGroupMeta *actual{registry.getEggGroupMetadata(toEggGroupID(BuiltinEggGroupID::Amorphous))};

			CHECK((expected == *actual));
		}

		THEN("Bug EggGroup has the appropriate properties")
		{
			EggGroupMeta expected{
				.mName = std::string(EGG_GROUP_NAME_BUG),
				.mEggGroupID = toEggGroupID(BuiltinEggGroupID::Bug),
			};

			const EggGroupMeta *actual{registry.getEggGroupMetadata(toEggGroupID(BuiltinEggGroupID::Bug))};

			CHECK((expected == *actual));
		}

		THEN("Dragon EggGroup has the appropriate properties")
		{
			EggGroupMeta expected{
				.mName = std::string(EGG_GROUP_NAME_DRAGON),
				.mEggGroupID = toEggGroupID(BuiltinEggGroupID::Dragon),
			};

			const EggGroupMeta *actual{registry.getEggGroupMetadata(toEggGroupID(BuiltinEggGroupID::Dragon))};

			CHECK((expected == *actual));
		}

		THEN("Fairy EggGroup has the appropriate properties")
		{
			EggGroupMeta expected{
				.mName = std::string(EGG_GROUP_NAME_FAIRY),
				.mEggGroupID = toEggGroupID(BuiltinEggGroupID::Fairy),
			};

			const EggGroupMeta *actual{registry.getEggGroupMetadata(toEggGroupID(BuiltinEggGroupID::Fairy))};

			CHECK((expected == *actual));
		}

		THEN("Field EggGroup has the appropriate properties")
		{
			EggGroupMeta expected{
				.mName = std::string(EGG_GROUP_NAME_FIELD),
				.mEggGroupID = toEggGroupID(BuiltinEggGroupID::Field),
			};

			const EggGroupMeta *actual{registry.getEggGroupMetadata(toEggGroupID(BuiltinEggGroupID::Field))};

			CHECK((expected == *actual));
		}

		THEN("Flying EggGroup has the appropriate properties")
		{
			EggGroupMeta expected{
				.mName = std::string(EGG_GROUP_NAME_FLYING),
				.mEggGroupID = toEggGroupID(BuiltinEggGroupID::Flying),
			};

			const EggGroupMeta *actual{registry.getEggGroupMetadata(toEggGroupID(BuiltinEggGroupID::Flying))};

			CHECK((expected == *actual));
		}

		THEN("Grass EggGroup has the appropriate properties")
		{
			EggGroupMeta expected{
				.mName = std::string(EGG_GROUP_NAME_GRASS),
				.mEggGroupID = toEggGroupID(BuiltinEggGroupID::Grass),
			};

			const EggGroupMeta *actual{registry.getEggGroupMetadata(toEggGroupID(BuiltinEggGroupID::Grass))};

			CHECK((expected == *actual));
		}

		THEN("HumanLike EggGroup has the appropriate properties")
		{
			EggGroupMeta expected{
				.mName = std::string(EGG_GROUP_NAME_HUMAN_LIKE),
				.mEggGroupID = toEggGroupID(BuiltinEggGroupID::HumanLike),
			};

			const EggGroupMeta *actual{registry.getEggGroupMetadata(toEggGroupID(BuiltinEggGroupID::HumanLike))};

			CHECK((expected == *actual));
		}

		THEN("Mineral EggGroup has the appropriate properties")
		{
			EggGroupMeta expected{
				.mName = std::string(EGG_GROUP_NAME_MINERAL),
				.mEggGroupID = toEggGroupID(BuiltinEggGroupID::Mineral),
			};

			const EggGroupMeta *actual{registry.getEggGroupMetadata(toEggGroupID(BuiltinEggGroupID::Mineral))};

			CHECK((expected == *actual));
		}

		THEN("Monster EggGroup has the appropriate properties")
		{
			EggGroupMeta expected{
				.mName = std::string(EGG_GROUP_NAME_MONSTER),
				.mEggGroupID = toEggGroupID(BuiltinEggGroupID::Monster),
			};

			const EggGroupMeta *actual{registry.getEggGroupMetadata(toEggGroupID(BuiltinEggGroupID::Monster))};

			CHECK((expected == *actual));
		}

		THEN("Water1 EggGroup has the appropriate properties")
		{
			EggGroupMeta expected{
				.mName = std::string(EGG_GROUP_NAME_WATER1),
				.mEggGroupID = toEggGroupID(BuiltinEggGroupID::Water1),
			};

			const EggGroupMeta *actual{registry.getEggGroupMetadata(toEggGroupID(BuiltinEggGroupID::Water1))};

			CHECK((expected == *actual));
		}

		THEN("Water2 EggGroup has the appropriate properties")
		{
			EggGroupMeta expected{
				.mName = std::string(EGG_GROUP_NAME_WATER2),
				.mEggGroupID = toEggGroupID(BuiltinEggGroupID::Water2),
			};

			const EggGroupMeta *actual{registry.getEggGroupMetadata(toEggGroupID(BuiltinEggGroupID::Water2))};

			CHECK((expected == *actual));
		}

		THEN("Water3 EggGroup has the appropriate properties")
		{
			EggGroupMeta expected{
				.mName = std::string(EGG_GROUP_NAME_WATER3),
				.mEggGroupID = toEggGroupID(BuiltinEggGroupID::Water3),
			};

			const EggGroupMeta *actual{registry.getEggGroupMetadata(toEggGroupID(BuiltinEggGroupID::Water3))};

			CHECK((expected == *actual));
		}

		THEN("Ditto EggGroup has the appropriate properties")
		{
			EggGroupMeta expected{
				.mName = std::string(EGG_GROUP_NAME_DITTO),
				.mEggGroupID = toEggGroupID(BuiltinEggGroupID::Ditto),
			};

			const EggGroupMeta *actual{registry.getEggGroupMetadata(toEggGroupID(BuiltinEggGroupID::Ditto))};

			CHECK((expected == *actual));
		}

		THEN("NoEggs EggGroup has the appropriate properties")
		{
			EggGroupMeta expected{
				.mName = std::string(EGG_GROUP_NAME_NO_EGGS),
				.mEggGroupID = toEggGroupID(BuiltinEggGroupID::NoEggs),
			};

			const EggGroupMeta *actual{registry.getEggGroupMetadata(toEggGroupID(BuiltinEggGroupID::NoEggs))};

			CHECK((expected == *actual));
		}
	}

	GIVEN("getEggGroupMetadata")
	{
		THEN("unknown IDs are absent")
		{
			CHECK((registry.getEggGroupMetadata(EggGroupID{200}) == nullptr));
		}

		THEN("the metadata is retrieved when accessed by a valid EggGroup ID")
		{
			EggGroupMeta expected{
				.mName = std::string(EGG_GROUP_NAME_NONE),
				.mEggGroupID = toEggGroupID(BuiltinEggGroupID::None),
			};

			CHECK((expected == *registry.getEggGroupMetadata(NO_EGG_GROUP_ID)));
		}
	}

	GIVEN("getEggGroupID")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(registry.getEggGroupID("Unknown").has_value());
		}

		THEN("the EggGroup ID is retrieved by valid EggGroup name")
		{
			std::optional<EggGroupID> eggGroupID{registry.getEggGroupID(EGG_GROUP_NAME_NONE)};

			REQUIRE(eggGroupID.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((eggGroupID == toEggGroupID(BuiltinEggGroupID::None)));
		}
	}

	GIVEN("getEggGroupName")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(registry.getEggGroupName(EggGroupID{200}).has_value());
		}

		THEN("a registered eggGroup name is returned by stable ID")
		{
			std::optional<std::string_view> eggGroupName{registry.getEggGroupName(toEggGroupID(BuiltinEggGroupID::None))};

			REQUIRE(eggGroupName.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((eggGroupName == EGG_GROUP_NAME_NONE));
		}
	}

	GIVEN("getAmountRegistered")
	{
		THEN("the registered span contains the exact amount of built-in entries")
		{
			CHECK((registry.getAmountRegistered() == finalEggGroupUnderlyingValue));
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
			EggGroupMeta expected{
				.mName = std::string(EGG_GROUP_NAME_NONE),
				.mEggGroupID = toEggGroupID(BuiltinEggGroupID::None),
			};

			const EggGroupMeta *eggGroupMeta{registry.getEntry(0)};

			REQUIRE((eggGroupMeta != nullptr));
			CHECK((*eggGroupMeta == expected));
		}
	}

	GIVEN("getRegisteredEggGroups")
	{
		THEN("the amount of eggGroups returned matches the amount that are built-in")
		{
			CHECK((registry.getRegisteredEggGroups().size() == finalEggGroupUnderlyingValue));
		}
	}

	GIVEN("getNextEggGroupID")
	{
		THEN("the next available stable EggGroup ID is after all built in eggGroup IDs")
		{
			CHECK((registry.getNextEggGroupID() == finalEggGroupUnderlyingValue));
		}
	}

	GIVEN("findIndexByEggGroupID")
	{
		THEN("an unknown stable ID has no internal index")
		{
			std::optional<ub> eggGroupIndex{registry.findIndexByEggGroupID(EggGroupID{200})};
			CHECK_FALSE(eggGroupIndex.has_value());
		}

		THEN("the internal array index is retrieved by valid EggGroup ID")
		{
			std::optional<ub> eggGroupIndex{registry.findIndexByEggGroupID(NO_EGG_GROUP_ID)};

			REQUIRE(eggGroupIndex.has_value());
			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((eggGroupIndex == 0));
		}
	}

	GIVEN("hasEggGroup")
	{
		WHEN("calling the string_view overload")
		{
			THEN("an unknown eggGroup name has no entry")
			{
				CHECK_FALSE(registry.hasEggGroup("Unknown"));
			}

			THEN("a known eggGroup name has an entry")
			{
				CHECK(registry.hasEggGroup(EGG_GROUP_NAME_NONE));
			}
		}

		WHEN("calling the EggGroupID overload")
		{
			THEN("an unknown eggGroup ID has no entry")
			{
				CHECK_FALSE(registry.hasEggGroup(EggGroupID{200}));
			}

			THEN("a known eggGroup ID has an entry")
			{
				CHECK(registry.hasEggGroup(NO_EGG_GROUP_ID));
			}
		}
	}

	WHEN("operator==")
	{
		GIVEN("two default constructed registries")
		{
			std::unique_ptr<EggGroupRegistry> other{std::make_unique<EggGroupRegistry>()};

			THEN("they are equal")
			{
				CHECK((registry == *other));
			}
		}

		GIVEN("for an entry added in one registry")
		{
			std::unique_ptr<EggGroupRegistryConfiguration> other{std::make_unique<EggGroupRegistryConfiguration>()};
			std::expected<EggGroupID, RegistryError> result{other->addEggGroup({.mName = "test"})};

			THEN("they are not equal")
			{
				REQUIRE(result.has_value());
				CHECK((registry != other->getRuntimeRegistry()));
			}
		}

		GIVEN("for an entry removed in one registry")
		{
			std::unique_ptr<EggGroupRegistryConfiguration> other{std::make_unique<EggGroupRegistryConfiguration>()};
			std::expected<EggGroupID, RegistryError> result{other->removeEggGroup(toEggGroupID(BuiltinEggGroupID::None))};

			THEN("they are not equal")
			{
				REQUIRE(result.has_value());
				CHECK((registry != other->getRuntimeRegistry()));
			}
		}

		GIVEN("for an entry modified in one registry")
		{
			std::unique_ptr<EggGroupRegistryConfiguration> other{std::make_unique<EggGroupRegistryConfiguration>()};
			std::expected<void, RegistryError> result{
				other->updateEggGroup(toEggGroupID(BuiltinEggGroupID::None), {.mName = "test"}),
			};

			THEN("they are not equal")
			{
				REQUIRE(result.has_value());
				CHECK((registry != other->getRuntimeRegistry()));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
