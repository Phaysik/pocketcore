/*! @file abilityRegistry.test.cpp
	@brief C++ file for running tests for the AbilityRegistry.
	@date 09/03/2026
	@since 0.4.0
	@version 0.12.19
	@author Matthew Moore
*/

#include "Registry/abilityRegistry.h"

#include <optional>
#include <string_view>
#include <utility>

#include "Ability/abilityID.h"
#include "Ability/abilityMeta.h"
#include "Ability/builtInAbilityID.h"
#include "Ability/constants.h"
#include "Battle/battleTargetsAndTriggers.h"
#include "Core/typedefs.h"
#include "Effect/builtInEffectID.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Ability::ABILITY_NAME_DRIZZLE;
using PocketCore::Ability::ABILITY_NAME_NONE;
using PocketCore::Ability::ABILITY_NAME_STENCH;
using PocketCore::Ability::AbilityID;
using PocketCore::Ability::AbilityMeta;
using PocketCore::Ability::BuiltinAbilityID;
using PocketCore::Ability::NO_ABILITY_ID;
using PocketCore::Ability::toAbilityID;
using PocketCore::Battle::BattleEventID;
using PocketCore::Battle::BattleEventRole;
using PocketCore::Battle::BattleTargetID;
using PocketCore::Core::ub;
using PocketCore::Effect::BuiltinEffectID;
using PocketCore::Effect::toEffectID;
using PocketCore::Registry::Ability::AbilityRegistry;

template <typename Registry>
concept PubliclyStructurallyMutable = requires(Registry &registry) { registry.setAmountRegistered(0); };

static_assert(!PubliclyStructurallyMutable<AbilityRegistry>);

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("AbilityRegistry")
{
	AbilityRegistry registry{};
	ub finalAbilityUnderlyingValue{std::to_underlying(BuiltinAbilityID::FinalAbility)};

	GIVEN("a default constructed ability registry")
	{
		THEN("Stench has the appropriate properties")
		{
			AbilityMeta expected{
				.mTriggers = {{
					.mEffects = {toEffectID(BuiltinEffectID::Flinch)},
					.mTrigger = BattleEventID::Hit,
					.mRole = BattleEventRole::User,
				},},
				.mName = ABILITY_NAME_STENCH,
				.mAbilityID = toAbilityID(BuiltinAbilityID::Stench),
				.mTargetID = BattleTargetID::SingleOpponent,
			};

			const AbilityMeta *actual{registry.getAbilityMetadata(toAbilityID(BuiltinAbilityID::Stench))};

			CHECK((expected == *actual));
		}

		THEN("Drizzle has the appropriate properties")
		{
			AbilityMeta expected{
				.mTriggers = {{
					.mEffects = {toEffectID(BuiltinEffectID::SetRain)},
					.mTrigger = BattleEventID::SwitchIn,
				},},
				.mName = ABILITY_NAME_DRIZZLE,
				.mAbilityID = toAbilityID(BuiltinAbilityID::Drizzle),
				.mTargetID = BattleTargetID::Self,
			};

			const AbilityMeta *actual{registry.getAbilityMetadata(toAbilityID(BuiltinAbilityID::Drizzle))};

			CHECK((expected == *actual));
		}
	}

	GIVEN("getAbilityMetadata")
	{
		THEN("unknown IDs are absent")
		{
			CHECK((registry.getAbilityMetadata(AbilityID{200}) == nullptr));
		}

		THEN("the metadata is retrieved when accessed by a valid Ability ID")
		{
			AbilityMeta expected{
				.mTriggers = {},
				.mName = ABILITY_NAME_NONE,
				.mAbilityID = toAbilityID(BuiltinAbilityID::None),
			};

			CHECK((expected == *registry.getAbilityMetadata(NO_ABILITY_ID)));
		}
	}

	GIVEN("getAbilityID")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(registry.getAbilityID("Unknown").has_value());
		}

		THEN("the Ability ID is retrieved by valid Ability name")
		{
			std::optional<AbilityID> abilityID{registry.getAbilityID(ABILITY_NAME_NONE)};

			REQUIRE(abilityID.has_value());

			CHECK((abilityID.value() == toAbilityID(BuiltinAbilityID::None)));
		}
	}

	GIVEN("getAbilityName")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(registry.getAbilityName(AbilityID{200}).has_value());
		}

		THEN("a registered ability name is returned by stable ID")
		{
			std::optional<std::string_view> abilityName{registry.getAbilityName(toAbilityID(BuiltinAbilityID::None))};

			REQUIRE(abilityName.has_value());

			CHECK((abilityName.value() == ABILITY_NAME_NONE));
		}
	}

	GIVEN("getAmountRegistered")
	{
		THEN("the registered span contains the exact amount of built-in entries")
		{
			CHECK((registry.getAmountRegistered() == finalAbilityUnderlyingValue));
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
			AbilityMeta expected{
				.mTriggers = {},
				.mName = ABILITY_NAME_NONE,
				.mAbilityID = toAbilityID(BuiltinAbilityID::None),
			};

			const AbilityMeta *abilityMeta{registry.getEntry(0)};

			REQUIRE((abilityMeta != nullptr));
			CHECK((*abilityMeta == expected));
		}
	}

	GIVEN("getRegisteredAbilities")
	{
		THEN("the amount of abilities returned matches the amount that are built-in")
		{
			CHECK((registry.getRegisteredAbilities().size() == finalAbilityUnderlyingValue));
		}
	}

	GIVEN("getNextAbilityID")
	{
		THEN("the next available stable Ability ID is after all built in ability IDs")
		{
			CHECK((registry.getNextAbilityID() == finalAbilityUnderlyingValue));
		}
	}

	GIVEN("findIndexByAbilityID")
	{
		THEN("an unknown stable ID has no internal index")
		{
			std::optional<ub> abilityIndex{registry.findIndexByAbilityID(AbilityID{200})};
			CHECK_FALSE(abilityIndex.has_value());
		}

		THEN("the internal array index is retrieved by valid Ability ID")
		{
			std::optional<ub> abilityIndex{registry.findIndexByAbilityID(NO_ABILITY_ID)};

			REQUIRE(abilityIndex.has_value());
			CHECK((abilityIndex.value() == 0));
		}
	}

	GIVEN("hasAbility")
	{
		WHEN("calling the string_view overload")
		{
			THEN("an unknown ability name has no entry")
			{
				CHECK_FALSE(registry.hasAbility("Unknown"));
			}

			THEN("a known ability name has an entry")
			{
				CHECK(registry.hasAbility(ABILITY_NAME_NONE));
			}
		}

		WHEN("calling the AbilityID overload")
		{
			THEN("an unknown ability ID has no entry")
			{
				CHECK_FALSE(registry.hasAbility(AbilityID{200}));
			}

			THEN("a known ability ID has an entry")
			{
				CHECK(registry.hasAbility(NO_ABILITY_ID));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
