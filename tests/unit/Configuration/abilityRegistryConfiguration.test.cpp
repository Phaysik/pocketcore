#include "Configuration/abilityRegistryConfiguration.h"

#include <array>
#include <optional>
#include <string_view>
#include <vector>

#include "Ability/abilityID.h"
#include "Ability/abilityMeta.h"
#include "Ability/abilityTargetsAndTriggers.h"
#include "Configuration/constants.h"
#include "Effect/effectType.h"
#include "Utility/Debug/Logging/logger.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Ability::AbilityEffectTrigger;
using PocketCore::Ability::AbilityID;
using PocketCore::Ability::AbilityTriggerID;
using PocketCore::Configuration::AbilityDefinition;
using PocketCore::Configuration::AbilityRegistryConfiguration;
using PocketCore::Configuration::RegistryError;
using PocketCore::Effect::EffectTypeID;
using PocketCore::Utility::Debug::Logging::Logger;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

namespace
{
	void ensureAbilityLoggerInitialized() // NOLINT(llvm-prefer-static-over-anonymous-namespace)
	{
		static bool initialized{false};

		if (!initialized)
		{
			initialized = Logger::initialize("arc_test", "abilityRegistryConfiguration_test.log", true);
		}
	}
} // namespace

SCENARIO("AbilityRegistryConfiguration addAbility")
{
	ensureAbilityLoggerInitialized();
	AbilityRegistryConfiguration configuration{};

	GIVEN("a unique ability definition")
	{
		std::vector<AbilityEffectTrigger> triggers{
			{.mTrigger = AbilityTriggerID::OnTurnEnd, .mEffects = {EffectTypeID::Recoil, EffectTypeID::StatusTick}},};
		AbilityDefinition definition{.name = "Regenerator", .triggers = triggers};

		WHEN("the ability is added")
		{
			auto result{configuration.addAbility(definition)};

			THEN("it receives the first custom stable ID and owns its trigger data")
			{
				REQUIRE(result.has_value());
				AbilityID assignedIdentifier{result.value()};
				CHECK((assignedIdentifier.getValue() == 3U));

				triggers.clear();
				auto metadata{configuration.getAbilityMetadata(assignedIdentifier)};
				REQUIRE(metadata.has_value());
				// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
				CHECK((metadata->get().mName == "Regenerator"));
				// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
				REQUIRE((metadata->get().mTriggers.size() == 1U));
				// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
				CHECK((metadata->get().mTriggers.front().mEffects.size() == 2U));
			}
		}
	}

	GIVEN("an ability whose name is already registered")
	{
		AbilityDefinition definition{.name = "Drizzle", .triggers = {}};

		THEN("registration reports a duplicate ability")
		{
			auto result{configuration.addAbility(definition)};
			REQUIRE_FALSE(result.has_value());
			CHECK((result.error().mKind == RegistryError::DuplicateAbility));
			CHECK((configuration.getAmountRegistered() == 3));
		}
	}
}

SCENARIO("AbilityRegistryConfiguration addAbilities")
{
	ensureAbilityLoggerInitialized();
	AbilityRegistryConfiguration configuration{};

	GIVEN("a batch containing a duplicate name")
	{
		std::array<AbilityDefinition, 3> definitions{
			{
				{.name = "First Custom", .triggers = {}},
				{.name = "Second Custom", .triggers = {}},
				{.name = "First Custom", .triggers = {}},
			},
		};

		WHEN("the batch is added")
		{
			auto result{configuration.addAbilities(definitions)};

			THEN("the entire batch is rolled back")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::DuplicateAbility));
				CHECK((configuration.getAmountRegistered() == 3));
				CHECK_FALSE(configuration.hasAbility("First Custom"));
				CHECK_FALSE(configuration.hasAbility("Second Custom"));
			}
		}
	}
}

SCENARIO("AbilityRegistryConfiguration metadata mutation")
{
	ensureAbilityLoggerInitialized();
	AbilityRegistryConfiguration configuration{};

	GIVEN("a registered custom ability")
	{
		AbilityDefinition definition{.name = "Custom Ability", .triggers = {}};
		auto addResult{configuration.addAbility(definition)};
		REQUIRE(addResult.has_value());
		AbilityID customIdentifier{addResult.value()};

		WHEN("its triggers are replaced by name")
		{
			std::array<AbilityEffectTrigger, 1> replacement{
				{
					{.mTrigger = AbilityTriggerID::OnStatus, .mEffects = {EffectTypeID::StatusApply}},
				},
			};
			auto setResult{configuration.setAbilityTriggers("Custom Ability", replacement)};

			THEN("the updated metadata is visible through its stable ID")
			{
				REQUIRE(setResult.has_value());
				auto metadata{configuration.getAbilityMetadata(customIdentifier)};
				REQUIRE(metadata.has_value());
				// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
				CHECK((metadata->get().mTriggers.front().mTrigger == AbilityTriggerID::OnStatus));
				// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
				CHECK((metadata->get().mTriggers.front().mEffects.front() == EffectTypeID::StatusApply));
			}
		}

		WHEN("it is renamed")
		{
			auto renameResult{configuration.renameAbility("Custom Ability", "Renamed Ability")};

			THEN("the stable ID remains unchanged")
			{
				REQUIRE(renameResult.has_value());
				CHECK_FALSE(configuration.hasAbility("Custom Ability"));
				auto renamedIdentifier{configuration.getAbilityID("Renamed Ability")};
				REQUIRE(renamedIdentifier.has_value());
				// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
				CHECK((renamedIdentifier.value() == customIdentifier));
			}
		}

		WHEN("it is removed and another ability is added")
		{
			auto removeResult{configuration.removeAbility(customIdentifier)};
			REQUIRE(removeResult.has_value());

			AbilityDefinition laterDefinition{.name = "Later Ability", .triggers = {}};
			auto laterResult{configuration.addAbility(laterDefinition)};

			THEN("the removed ID is not reused")
			{
				REQUIRE(laterResult.has_value());
				CHECK((laterResult.value() != customIdentifier));
				CHECK_FALSE(configuration.getAbilityMetadata(customIdentifier).has_value());
			}
		}
	}

	GIVEN("an unknown ability")
	{
		THEN("metadata mutation and removal report AbilityNotFound")
		{
			auto setResult{configuration.setAbilityTriggers("Missing", {})};
			auto removeResult{configuration.removeAbility("Missing")};

			REQUIRE_FALSE(setResult.has_value());
			REQUIRE_FALSE(removeResult.has_value());
			CHECK((setResult.error().mKind == RegistryError::AbilityNotFound));
			CHECK((removeResult.error().mKind == RegistryError::AbilityNotFound));
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
