/*! @file abilityRegistryConfiguration.test.cpp
	@brief C++ file for running tests for the AbilityRegistryConfiguration.
	@date 09/03/2026
	@since 0.4.0
	@version 0.12.18
	@author Matthew Moore
*/

#include "Configuration/abilityRegistryConfiguration.h"

#include <array>
#include <optional>
#include <string_view>
#include <vector>

#include "Ability/abilityID.h"
#include "Ability/abilityMeta.h"
#include "Battle/battleTargetsAndTriggers.h"
#include "Effect/builtInEffectID.h"
#include "Effect/effectTrigger.h"
#include "Registry/registryError.h"
#include "Utility/Debug/Logging/logger.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Ability::AbilityID;
using PocketCore::Ability::AbilityMeta;
using PocketCore::Battle::BattleEventID;
using PocketCore::Battle::BattleEventRole;
using PocketCore::Battle::BattleTargetID;
using PocketCore::Configuration::AbilityRegistryConfiguration;
using PocketCore::Configuration::RegistryError;
using PocketCore::Effect::BuiltinEffectID;
using PocketCore::Effect::EffectTrigger;
using PocketCore::Effect::toEffectID;
using PocketCore::Utility::Debug::Logging::Logger;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity,llvm-prefer-static-over-anonymous-namespace)

namespace
{
	void ensureAbilityLoggerInitialized()
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
		std::vector<EffectTrigger> triggers{
			{
				.mEffects = {toEffectID(BuiltinEffectID::Recoil), toEffectID(BuiltinEffectID::StatusTick)},
				.mTrigger = BattleEventID::TurnEnd,
			},
		};
		AbilityMeta definition{.mTriggers = triggers, .mName = "Regenerator"};

		WHEN("the ability is added")
		{
			auto result{configuration.addAbility(definition)};

			THEN("it receives the first custom stable ID and owns its trigger data")
			{
				REQUIRE(result.has_value());
				AbilityID assignedIdentifier{result.value()};
				CHECK((assignedIdentifier.getValue() == 8U));

				triggers.clear();
				const AbilityMeta *metadata{configuration.getAbilityMetadata(assignedIdentifier)};
				REQUIRE((metadata != nullptr));
				CHECK((metadata->mName == "Regenerator"));
				REQUIRE((metadata->mTriggers.size() == 1U));
				CHECK((metadata->mTriggers.front().mEffects.size() == 2U));
			}
		}
	}

	GIVEN("an ability whose name is already registered")
	{
		AbilityMeta definition{.mTriggers = {}, .mName = "Drizzle"};

		THEN("registration reports a duplicate ability")
		{
			auto result{configuration.addAbility(definition)};
			REQUIRE_FALSE(result.has_value());
			CHECK((result.error().mKind == RegistryError::DuplicateAbility));
			CHECK((configuration.getAmountRegistered() == 4));
		}
	}
}

SCENARIO("AbilityRegistryConfiguration addAbilities")
{
	ensureAbilityLoggerInitialized();
	AbilityRegistryConfiguration configuration{};

	GIVEN("a batch containing a duplicate name")
	{
		std::array<AbilityMeta, 3> definitions{
			{
				{.mTriggers = {}, .mName = "First Custom"},
				{.mTriggers = {}, .mName = "Second Custom"},
				{.mTriggers = {}, .mName = "First Custom"},
			},
		};

		WHEN("the batch is added")
		{
			auto result{configuration.addAbilities(definitions)};

			THEN("the entire batch is rolled back")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::DuplicateAbility));
				CHECK((configuration.getAmountRegistered() == 4));
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
		AbilityMeta definition{.mTriggers = {}, .mName = "Custom Ability"};
		auto addResult{configuration.addAbility(definition)};
		REQUIRE(addResult.has_value());
		AbilityID customIdentifier{addResult.value()};

		WHEN("its triggers are replaced by name")
		{
			std::array<EffectTrigger, 1> replacement{
				{
					{.mEffects = {toEffectID(BuiltinEffectID::StatusApply)}, .mTrigger = BattleEventID::StatusChanged},
				},
			};
			auto setResult{configuration.setAbilityTriggers("Custom Ability", replacement)};

			THEN("the updated metadata is visible through its stable ID")
			{
				REQUIRE(setResult.has_value());
				const AbilityMeta *metadata{configuration.getAbilityMetadata(customIdentifier)};
				REQUIRE((metadata != nullptr));
				CHECK((metadata->mTriggers.front().mTrigger == BattleEventID::StatusChanged));
				CHECK((metadata->mTriggers.front().mEffects.front() == toEffectID(BuiltinEffectID::StatusApply)));
			}
		}

		WHEN("its triggers are replaced by stable ID")
		{
			std::array<EffectTrigger, 1> replacement{
				{
					{.mEffects = {toEffectID(BuiltinEffectID::StatusRemove)}, .mTrigger = BattleEventID::Faint},
				},
			};
			auto setResult{configuration.setAbilityTriggers(customIdentifier, replacement)};

			THEN("the trigger update remains associated with the same ID")
			{
				REQUIRE(setResult.has_value());
				const AbilityMeta *metadata{configuration.getAbilityMetadata(customIdentifier)};
				REQUIRE((metadata != nullptr));
				CHECK((metadata->mTriggers.front().mTrigger == BattleEventID::Faint));
			}
		}

		WHEN("its target is replaced by name")
		{
			auto setResult{configuration.setAbilityTarget("Custom Ability", BattleTargetID::AllOpponents)};

			THEN("target metadata changes")
			{
				REQUIRE(setResult.has_value());
				const AbilityMeta *metadata{configuration.getAbilityMetadata(customIdentifier)};
				REQUIRE((metadata != nullptr));
				CHECK((metadata->mTargetID == BattleTargetID::AllOpponents));
			}
		}

		WHEN("its target is replaced by stable ID")
		{
			auto setResult{configuration.setAbilityTarget(customIdentifier, BattleTargetID::AllAllies)};

			THEN("target metadata changes")
			{
				REQUIRE(setResult.has_value());
				const AbilityMeta *metadata{configuration.getAbilityMetadata(customIdentifier)};
				REQUIRE((metadata != nullptr));
				CHECK((metadata->mTargetID == BattleTargetID::AllAllies));
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

		WHEN("it is updated by name")
		{
			AbilityMeta replacement{
				.mTriggers = {{.mEffects = {toEffectID(BuiltinEffectID::StatusApply)}, .mTrigger = BattleEventID::StatusChanged}},
				.mName = "Replacement Ability",
				.mTargetID = BattleTargetID::AllExceptSelf,
			};
			auto updateResult{configuration.updateAbility("Custom Ability", replacement)};

			THEN("stored metadata is replaced")
			{
				REQUIRE(updateResult.has_value());
				CHECK(configuration.hasAbility("Replacement Ability"));
				CHECK_FALSE(configuration.hasAbility("Custom Ability"));
			}
		}

		WHEN("it is updated by stable ID")
		{
			AbilityMeta replacement{
				.mTriggers
				= {{.mEffects = {toEffectID(BuiltinEffectID::Recoil)}, .mTrigger = BattleEventID::MoveUse, .mRole = BattleEventRole::User}},
				.mName = "Replacement Ability By ID",
				.mTargetID = BattleTargetID::Self,
			};
			auto updateResult{configuration.updateAbility(customIdentifier, replacement)};

			THEN("stored metadata is replaced")
			{
				REQUIRE(updateResult.has_value());
				CHECK(configuration.hasAbility("Replacement Ability By ID"));
			}
		}

		WHEN("it is removed by name")
		{
			auto removeResult{configuration.removeAbility("Custom Ability")};

			THEN("the removed stable ID is returned")
			{
				REQUIRE(removeResult.has_value());
				CHECK((removeResult.value() == customIdentifier));
				CHECK_FALSE(configuration.hasAbility(customIdentifier));
			}
		}

		WHEN("it is removed and another ability is added")
		{
			auto removeResult{configuration.removeAbility(customIdentifier)};
			REQUIRE(removeResult.has_value());

			AbilityMeta laterDefinition{.mTriggers = {}, .mName = "Later Ability"};
			auto laterResult{configuration.addAbility(laterDefinition)};

			THEN("the removed ID is not reused")
			{
				REQUIRE(laterResult.has_value());
				CHECK((laterResult.value() != customIdentifier));
				CHECK((configuration.getAbilityMetadata(customIdentifier) == nullptr));
			}
		}
	}

	GIVEN("an unknown ability")
	{
		THEN("metadata mutation and removal report AbilityNotFound")
		{
			auto setResult{configuration.setAbilityTriggers("Missing", {})};
			auto setTargetResult{configuration.setAbilityTarget("Missing", BattleTargetID::Self)};
			auto updateResult{configuration.updateAbility("Missing", {.mTriggers = {}, .mName = "Updated"})};
			auto removeResult{configuration.removeAbility("Missing")};

			REQUIRE_FALSE(setResult.has_value());
			REQUIRE_FALSE(setTargetResult.has_value());
			REQUIRE_FALSE(updateResult.has_value());
			REQUIRE_FALSE(removeResult.has_value());
			CHECK((setResult.error().mKind == RegistryError::AbilityNotFound));
			CHECK((setTargetResult.error().mKind == RegistryError::AbilityNotFound));
			CHECK((updateResult.error().mKind == RegistryError::AbilityNotFound));
			CHECK((removeResult.error().mKind == RegistryError::AbilityNotFound));
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity,llvm-prefer-static-over-anonymous-namespace)
