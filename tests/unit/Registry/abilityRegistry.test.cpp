#include "Registry/abilityRegistry.h"

#include <optional>
#include <span>
#include <string_view>

#include "Ability/abilityID.h"
#include "Ability/abilityMeta.h"
#include "Ability/abilityTargetsAndTriggers.h"
#include "Ability/builtinAbilityID.h"
#include "Core/typedefs.h"
#include "Effect/effectType.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Ability::AbilityID;
using PocketCore::Ability::AbilityMeta;
using PocketCore::Ability::AbilityTriggerID;
using PocketCore::Ability::BuiltinAbilityID;
using PocketCore::Ability::toAbilityID;
using PocketCore::Core::ub;
using PocketCore::Effect::EffectTypeID;
using PocketCore::Registry::Abilities::AbilityRegistry;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("AbilityRegistry")
{
	AbilityRegistry registry{};

	GIVEN("a default-constructed registry")
	{
		THEN("all built-in abilities preserve their catalog identifiers")
		{
			CHECK((registry.getAmountRegistered() == 3));
			CHECK((registry.getNextAbilityID() == 3));

			std::optional<AbilityID> noneIdentifier{registry.getAbilityID("None")};
			std::optional<AbilityID> stenchIdentifier{registry.getAbilityID("Stench")};
			std::optional<AbilityID> drizzleIdentifier{registry.getAbilityID("Drizzle")};

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
			auto metadata{registry.getAbilityMetadata(toAbilityID(BuiltinAbilityID::Stench))};
			REQUIRE(metadata.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			const AbilityMeta &stench{metadata->get()};
			REQUIRE((stench.mTriggers.size() == 1U));
			CHECK((stench.mTriggers.front().mTrigger == AbilityTriggerID::OnMoveUse));
			REQUIRE((stench.mTriggers.front().mEffects.size() == 1U));
			CHECK((stench.mTriggers.front().mEffects.front() == EffectTypeID::Flinch));
		}

		THEN("Drizzle retains its switch-in rain metadata")
		{
			auto metadata{registry.getAbilityMetadata(toAbilityID(BuiltinAbilityID::Drizzle))};
			REQUIRE(metadata.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			const AbilityMeta &drizzle{metadata->get()};
			REQUIRE((drizzle.mTriggers.size() == 1U));
			CHECK((drizzle.mTriggers.front().mTrigger == AbilityTriggerID::OnSwitchIn));
			CHECK((drizzle.mTriggers.front().mEffects.front() == EffectTypeID::SetRain));
		}

		THEN("unknown names and IDs are absent")
		{
			CHECK_FALSE(registry.getAbilityID("Unknown").has_value());
			CHECK_FALSE(registry.getAbilityMetadata(AbilityID{200}).has_value());
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
			CHECK((abilityName.value() == "Drizzle"));
		}

		THEN("the registered span contains exactly the built-in entries")
		{
			std::span<const AbilityMeta> abilities{registry.getRegisteredAbilities()};
			REQUIRE((abilities.size() == 3U));
			CHECK((abilities.front().mName == "None"));
			CHECK((abilities.back().mName == "Drizzle"));
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)