/*! @file abilityRegistry.test.cpp
	@brief C++ file for running tests for the AbilityRegistry.
	@date 08/18/2026
	@since 0.4.0
	@version 0.10.8
	@author Matthew Moore
*/

#include "Registry/abilityRegistry.h"

#include <optional>
#include <span>
#include <string_view>

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
using PocketCore::Ability::toAbilityID;
using PocketCore::Battle::BattleEventID;
using PocketCore::Battle::BattleEventRole;
using PocketCore::Core::ub;
using PocketCore::Effect::BuiltinEffectID;
using PocketCore::Effect::toEffectID;
using PocketCore::Registry::Ability::AbilityRegistry;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("AbilityRegistry")
{
	AbilityRegistry registry{};

	GIVEN("a default-constructed registry")
	{
		THEN("all built-in abilities preserve their catalog identifiers")
		{
			CHECK((registry.getAmountRegistered() == 3));
			CHECK((registry.getNextAbilityID() == 8));

			std::optional<AbilityID> noneIdentifier{registry.getAbilityID(ABILITY_NAME_NONE)};
			std::optional<AbilityID> stenchIdentifier{registry.getAbilityID(ABILITY_NAME_STENCH)};
			std::optional<AbilityID> drizzleIdentifier{registry.getAbilityID(ABILITY_NAME_DRIZZLE)};

			REQUIRE(noneIdentifier.has_value());
			REQUIRE(stenchIdentifier.has_value());
			REQUIRE(drizzleIdentifier.has_value());
			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((noneIdentifier.value() == toAbilityID(BuiltinAbilityID::None)));
			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((stenchIdentifier.value() == toAbilityID(BuiltinAbilityID::Stench)));
			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((drizzleIdentifier.value() == toAbilityID(BuiltinAbilityID::Drizzle)));
		}

		THEN("Stench retains its move-use flinch metadata")
		{
			const AbilityMeta *metadata{registry.getAbilityMetadata(toAbilityID(BuiltinAbilityID::Stench))};
			REQUIRE((metadata != nullptr));

			AbilityMeta stench{*metadata};
			REQUIRE((stench.mTriggers.size() == 1U));
			CHECK((stench.mTriggers.front().mTrigger == BattleEventID::Hit));
			CHECK((stench.mTriggers.front().mRole == BattleEventRole::User));
			REQUIRE((stench.mTriggers.front().mEffects.size() == 1U));
			CHECK((stench.mTriggers.front().mEffects.front() == toEffectID(BuiltinEffectID::Flinch)));
		}

		THEN("Drizzle retains its switch-in rain metadata")
		{
			const AbilityMeta *metadata{registry.getAbilityMetadata(toAbilityID(BuiltinAbilityID::Drizzle))};
			REQUIRE((metadata != nullptr));

			AbilityMeta drizzle{*metadata};
			REQUIRE((drizzle.mTriggers.size() == 1U));
			CHECK((drizzle.mTriggers.front().mTrigger == BattleEventID::SwitchIn));
			CHECK((drizzle.mTriggers.front().mEffects.front() == toEffectID(BuiltinEffectID::SetRain)));
		}

		THEN("unknown names and IDs are absent")
		{
			CHECK_FALSE(registry.getAbilityID("Unknown").has_value());
			CHECK((registry.getAbilityMetadata(AbilityID{200}) == nullptr));
			CHECK_FALSE(registry.getAbilityName(AbilityID{200}).has_value());
		}

		THEN("an unknown stable ID has no internal index")
		{
			std::optional<ub> abilityIndex{registry.findIndexByAbilityID(AbilityID{200})};
			CHECK_FALSE(abilityIndex.has_value());
		}

		THEN("a registered ability name is returned by stable ID")
		{
			std::optional<std::string_view> abilityName{registry.getAbilityName(toAbilityID(BuiltinAbilityID::Drizzle))};
			REQUIRE(abilityName.has_value());
			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((abilityName.value() == ABILITY_NAME_DRIZZLE));
		}

		THEN("the registered span contains exactly the built-in entries")
		{
			std::span<const AbilityMeta> abilities{registry.getRegisteredAbilities()};
			REQUIRE((abilities.size() == 3U));
			CHECK((abilities.front().mName == ABILITY_NAME_NONE));
			CHECK((abilities.back().mName == ABILITY_NAME_DRIZZLE));
			CHECK(registry.hasAbility(toAbilityID(BuiltinAbilityID::Stench)));
			CHECK(registry.hasAbility(ABILITY_NAME_DRIZZLE));
		}

		THEN("the registered amount can be restored directly")
		{
			registry.setAmountRegistered(2);
			CHECK((registry.getAmountRegistered() == 2));
		}

		THEN("the next stable ID can be restored directly")
		{
			registry.setNextAbilityID(42);
			CHECK((registry.getNextAbilityID() == 42));
		}

		THEN("the incrementNextAbilityID() method increments the next stable ID")
		{
			registry.setNextAbilityID(42);
			registry.incrementNextAbilityID();
			CHECK((registry.getNextAbilityID() == 43));
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
