/*! @file abilityRegistryConfiguration.test.cpp
	@brief C++ file for running tests for the AbilityRegistryConfiguration.
	@date 09/10/2026
	@since 0.4.0
	@version 0.12.20
	@author Matthew Moore
*/

#include "Configuration/abilityRegistryConfiguration.h"

#include <expected>
#include <format>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "Ability/abilityID.h"
#include "Ability/abilityMeta.h"
#include "Ability/builtInAbilityID.h"
#include "Ability/constants.h"
#include "Battle/battleTargetsAndTriggers.h"
#include "Configuration/constants.h"
#include "Core/typedefs.h"
#include "Effect/builtInEffectID.h"
#include "Effect/effectTrigger.h"
#include "Registry/abilityRegistry.h"
#include "Registry/registryError.h"
#include "Utility/Debug/Logging/logging.testHelper.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Ability::ABILITY_NAME_DRIZZLE;
using PocketCore::Ability::ABILITY_NAME_NONE;
using PocketCore::Ability::AbilityID;
using PocketCore::Ability::AbilityMeta;
using PocketCore::Ability::BuiltinAbilityID;
using PocketCore::Ability::NO_ABILITY_ID;
using PocketCore::Ability::toAbilityID;
using PocketCore::Battle::BattleEventID;
using PocketCore::Battle::BattleEventRole;
using PocketCore::Battle::BattleTargetID;
using PocketCore::Configuration::AbilityRegistryConfiguration;
using PocketCore::Configuration::MAX_ABILITIES;
using PocketCore::Configuration::RegistryError;
using PocketCore::Core::ub;
using PocketCore::Core::us;
using PocketCore::Effect::BuiltinEffectID;
using PocketCore::Effect::EffectTrigger;
using PocketCore::Effect::toEffectID;
using PocketCore::Registry::Ability::AbilityRegistry;
using PocketCore::Registry::RegistryErrorInfo;
using PocketCore::Testing::ensureLoggerInitialized;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("AbilityRegistryConfiguration")
{
	ensureLoggerInitialized("ability registry configuration test", "abilityRegistryConfiguration_test.log");

	AbilityRegistryConfiguration config{};
	AbilityRegistry registry{};
	ub finalAbilityUnderlyingValue{std::to_underlying(BuiltinAbilityID::FinalAbility)};

	GIVEN("getAmountRegistered")
	{
		THEN("the default registry returns the expected amount of built-in abilities")
		{
			CHECK((config.getAmountRegistered() == finalAbilityUnderlyingValue));
		}
	}

	GIVEN("getRuntimeRegistry")
	{
		THEN("the runtime registry is the same as the default registry")
		{
			CHECK((config.getRuntimeRegistry() == registry));
		}
	}

	GIVEN("getAbilityMetadata")
	{
		THEN("unknown IDs are absent")
		{
			CHECK((config.getAbilityMetadata(AbilityID{200}) == nullptr));
		}

		THEN("the metadata is retrieved when accessed by a valid Ability ID")
		{
			AbilityMeta expected{
				.mName = std::string(ABILITY_NAME_NONE),
				.mTriggers = {},
				.mAbilityID = toAbilityID(BuiltinAbilityID::None),
			};

			CHECK((expected == *config.getAbilityMetadata(NO_ABILITY_ID)));
		}
	}

	GIVEN("getAbilityID")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(config.getAbilityID("Unknown").has_value());
		}

		THEN("the Ability ID is retrieved by valid Ability name")
		{
			std::optional<AbilityID> abilityID{config.getAbilityID(ABILITY_NAME_NONE)};

			REQUIRE(abilityID.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((abilityID.value() == toAbilityID(BuiltinAbilityID::None)));
		}
	}

	GIVEN("getAbilityName")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(config.getAbilityName(AbilityID{200}).has_value());
		}

		THEN("a registered ability name is returned by stable ID")
		{
			std::optional<std::string_view> abilityName{config.getAbilityName(toAbilityID(BuiltinAbilityID::None))};

			REQUIRE(abilityName.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((abilityName.value() == ABILITY_NAME_NONE));
		}
	}

	GIVEN("getRegisteredAbilities")
	{
		THEN("the amount of abilities returned matches the amount that are built-in")
		{
			CHECK((config.getRegisteredAbilities().size() == finalAbilityUnderlyingValue));
		}
	}

	GIVEN("hasAbility")
	{
		WHEN("calling the string_view overload")
		{
			THEN("an unknown ability name has no entry")
			{
				CHECK_FALSE(config.hasAbility("Unknown"));
			}

			THEN("a known ability name has an entry")
			{
				CHECK(config.hasAbility(ABILITY_NAME_NONE));
			}
		}

		WHEN("calling the AbilityID overload")
		{
			THEN("an unknown ability ID has no entry")
			{
				CHECK_FALSE(config.hasAbility(AbilityID{200}));
			}

			THEN("a known ability ID has an entry")
			{
				CHECK(config.hasAbility(NO_ABILITY_ID));
			}
		}
	}

	GIVEN("addAbility")
	{
		WHEN("trying to add an ability past the capacity")
		{
			us newAbilityCount{finalAbilityUnderlyingValue};

			for (us i{0}; i < MAX_ABILITIES - finalAbilityUnderlyingValue; ++i)
			{
				std::string name{std::format("String_{:04}", i)};
				AbilityMeta definition{.mName = name, .mTriggers = {}};
				std::expected<AbilityID, RegistryErrorInfo> result{config.addAbility(definition)};

				REQUIRE(result.has_value());

				AbilityID assignedID{result.value()};
				CHECK((assignedID.getValue() == newAbilityCount++));
			}

			std::string name{std::format("String_{:04}", MAX_ABILITIES + 1)};

			AbilityMeta definition{.mName = name, .mTriggers = {}};
			std::expected<AbilityID, RegistryErrorInfo> result{config.addAbility(definition)};

			THEN("registration reports a max capacity and nothing is added")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::MaxCapacity));
				CHECK((config.getAmountRegistered() == MAX_ABILITIES));
			}
		}

		WHEN("an ability whose name is already in use is added")
		{
			AbilityMeta definition{.mName = std::string(ABILITY_NAME_DRIZZLE), .mTriggers = {}};
			std::expected<AbilityID, RegistryErrorInfo> result{config.addAbility(definition)};

			THEN("registration reports a duplicate ability and nothing is added")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::DuplicateAbility));
				CHECK((config.getAmountRegistered() == finalAbilityUnderlyingValue));
			}
		}

		WHEN("a unique ability definition is added")
		{
			std::vector<EffectTrigger> triggers{
				{
					.mEffects = {toEffectID(BuiltinEffectID::Recoil), toEffectID(BuiltinEffectID::StatusTick)},
					.mTrigger = BattleEventID::TurnEnd,
				},
			};

			AbilityMeta definition{.mName = "TestAbilityName", .mTriggers = triggers};

			std::expected<AbilityID, RegistryErrorInfo> result{config.addAbility(definition)};

			THEN("it receives the first custom stable ID and owns its trigger data")
			{
				REQUIRE(result.has_value());
				AbilityID assignedID{result.value()};
				CHECK((assignedID.getValue() == finalAbilityUnderlyingValue));

				triggers.clear();

				const AbilityMeta *metadata{config.getAbilityMetadata(assignedID)};

				REQUIRE((metadata != nullptr));
				CHECK((metadata->mName == "TestAbilityName"));
				REQUIRE((metadata->mTriggers.size() == 1U));
				CHECK((metadata->mTriggers.front().mEffects.size() == 2U));
			}
		}
	}

	GIVEN("addAbilities")
	{
		WHEN("trying to add an ability past the capacity")
		{
			std::vector<AbilityMeta> abilityMetas;

			for (us i{0}; i < MAX_ABILITIES - finalAbilityUnderlyingValue; ++i)
			{
				std::string name{std::format("String_{:04}", i)};
				abilityMetas.push_back({.mName = name, .mTriggers = {}});
			}

			std::string name{std::format("String_{:04}", MAX_ABILITIES + 1)};

			abilityMetas.push_back({.mName = name, .mTriggers = {}});
			std::expected<void, RegistryErrorInfo> result{config.addAbilities(abilityMetas)};

			THEN("registration reports a max capacity and nothing is added")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::MaxCapacity));
				CHECK((config.getAmountRegistered() == finalAbilityUnderlyingValue));
			}
		}

		WHEN("an ability whose name is already in use is added")
		{
			std::vector<AbilityMeta> abilityMetas{};

			for (us i{0}; i < 20; ++i)
			{
				std::string name{std::format("String_{:04}", i)};
				abilityMetas.push_back({.mName = name, .mTriggers = {}});
			}

			abilityMetas.push_back({.mName = std::string(ABILITY_NAME_DRIZZLE), .mTriggers = {}});
			std::expected<void, RegistryErrorInfo> result{config.addAbilities(abilityMetas)};

			THEN("registration reports a duplicate ability and the registry is rollback to the checkpoint before the erroneous addition")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::DuplicateAbility));
				CHECK((config.getAmountRegistered() == finalAbilityUnderlyingValue));
			}
		}

		WHEN("a unique ability definition is added")
		{
			std::vector<AbilityMeta> abilityMetas{};
			std::vector<EffectTrigger> triggers{
				{
					.mEffects = {toEffectID(BuiltinEffectID::Recoil), toEffectID(BuiltinEffectID::StatusTick)},
					.mTrigger = BattleEventID::TurnEnd,
				},
			};

			abilityMetas.push_back({.mName = "TestAbilityName", .mTriggers = triggers});

			std::expected<void, RegistryErrorInfo> result{config.addAbilities(abilityMetas)};

			THEN("the registry reports no error and all the ability definitions are added")
			{
				REQUIRE(result.has_value());
				CHECK((config.getAmountRegistered() == finalAbilityUnderlyingValue + 1));
			}
		}
	}

	GIVEN("setAbilityTriggers")
	{
		WHEN("calling the string_view overload")
		{
			WHEN("calling with an invalid ability name")
			{
				std::vector<EffectTrigger> triggers{
					{
						.mEffects = {toEffectID(BuiltinEffectID::Recoil), toEffectID(BuiltinEffectID::StatusTick)},
						.mTrigger = BattleEventID::TurnEnd,
					},
				};

				std::expected<void, RegistryErrorInfo> result{config.setAbilityTriggers("ThisIsInvalid", triggers)};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::AbilityNotFound));
					CHECK((config.getAmountRegistered() == finalAbilityUnderlyingValue));
				}
			}

			WHEN("updating an existing ability definition")
			{
				std::vector<EffectTrigger> triggers{
					{
						.mEffects = {toEffectID(BuiltinEffectID::Recoil), toEffectID(BuiltinEffectID::StatusTick)},
						.mTrigger = BattleEventID::TurnEnd,
					},
				};

				std::expected<void, RegistryErrorInfo> result{config.setAbilityTriggers(ABILITY_NAME_NONE, triggers)};

				THEN("the registry reports no error and the triggers are appropriately updated")
				{
					REQUIRE(result.has_value());
					CHECK((config.getAmountRegistered() == finalAbilityUnderlyingValue));
					CHECK((config.getAbilityMetadata(toAbilityID(BuiltinAbilityID::None))->mTriggers == triggers));
				}
			}
		}

		WHEN("calling the AbilityID overload")
		{
			WHEN("calling with an invalid ability name")
			{
				std::vector<EffectTrigger> triggers{
					{
						.mEffects = {toEffectID(BuiltinEffectID::Recoil), toEffectID(BuiltinEffectID::StatusTick)},
						.mTrigger = BattleEventID::TurnEnd,
					},
				};

				std::expected<void, RegistryErrorInfo> result{config.setAbilityTriggers(AbilityID{200}, triggers)};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::AbilityNotFound));
					CHECK((config.getAmountRegistered() == finalAbilityUnderlyingValue));
				}
			}

			WHEN("updating an existing ability definition")
			{
				std::vector<EffectTrigger> triggers{
					{
						.mEffects = {toEffectID(BuiltinEffectID::Recoil), toEffectID(BuiltinEffectID::StatusTick)},
						.mTrigger = BattleEventID::TurnEnd,
					},
				};

				std::expected<void, RegistryErrorInfo> result{config.setAbilityTriggers(toAbilityID(BuiltinAbilityID::None), triggers)};

				THEN("the registry reports no error and the triggers are appropriately updated")
				{
					REQUIRE(result.has_value());
					CHECK((config.getAmountRegistered() == finalAbilityUnderlyingValue));
					CHECK((config.getAbilityMetadata(toAbilityID(BuiltinAbilityID::None))->mTriggers == triggers));
				}
			}
		}
	}

	GIVEN("setAbilityTarget")
	{
		WHEN("calling the string_view overload")
		{
			WHEN("calling with an invalid ability name")
			{
				std::expected<void, RegistryErrorInfo> result{config.setAbilityTarget("ThisIsInvalid", BattleTargetID::AllAllies)};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::AbilityNotFound));
					CHECK((config.getAmountRegistered() == finalAbilityUnderlyingValue));
				}
			}

			WHEN("updating an existing ability definition")
			{
				std::expected<void, RegistryErrorInfo> result{
					config.setAbilityTarget(ABILITY_NAME_NONE, BattleTargetID::AllAllies),
				};

				THEN("the registry reports no error and the target is appropriately updated")
				{
					REQUIRE(result.has_value());
					CHECK((config.getAmountRegistered() == finalAbilityUnderlyingValue));
					CHECK((config.getAbilityMetadata(toAbilityID(BuiltinAbilityID::None))->mTargetID == BattleTargetID::AllAllies));
				}
			}
		}

		WHEN("calling the AbilityID overload")
		{
			WHEN("calling with an invalid ability name")
			{
				std::expected<void, RegistryErrorInfo> result{config.setAbilityTarget(AbilityID{200}, BattleTargetID::AllAllies)};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::AbilityNotFound));
					CHECK((config.getAmountRegistered() == finalAbilityUnderlyingValue));
				}
			}

			WHEN("updating an existing ability definition")
			{
				std::expected<void, RegistryErrorInfo> result{
					config.setAbilityTarget(toAbilityID(BuiltinAbilityID::None), BattleTargetID::AllAllies),
				};

				THEN("the registry reports no error and the target is appropriately updated")
				{
					REQUIRE(result.has_value());
					CHECK((config.getAmountRegistered() == finalAbilityUnderlyingValue));
					CHECK((config.getAbilityMetadata(toAbilityID(BuiltinAbilityID::None))->mTargetID == BattleTargetID::AllAllies));
				}
			}
		}
	}

	GIVEN("renameAbility")
	{
		WHEN("calling with an invalid ability name")
		{
			std::expected<void, RegistryErrorInfo> result{config.renameAbility("ThisIsInvalid", "NewName")};

			THEN("the registry reports an error and there is no update to the registry")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::AbilityNotFound));
				CHECK((config.getAmountRegistered() == finalAbilityUnderlyingValue));
			}
		}

		WHEN("trying to rename to a name already in the registry")
		{
			std::expected<void, RegistryErrorInfo> result{
				config.renameAbility(ABILITY_NAME_NONE, ABILITY_NAME_DRIZZLE),
			};

			THEN("the registry reports an error and there is no update to the registry")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::DuplicateAbility));
				CHECK((config.getAmountRegistered() == finalAbilityUnderlyingValue));
			}
		}

		WHEN("updating an existing ability definition")
		{
			std::expected<void, RegistryErrorInfo> result{config.renameAbility(ABILITY_NAME_NONE, "NewName")};

			THEN("the registry reports no error and the name is appropriately updated")
			{
				REQUIRE(result.has_value());
				CHECK((config.getAmountRegistered() == finalAbilityUnderlyingValue));
				CHECK((config.getAbilityMetadata(toAbilityID(BuiltinAbilityID::None))->mName == "NewName"));
			}
		}
	}

	GIVEN("updateAbility")
	{
		std::vector<EffectTrigger> triggers{
			{
				.mEffects = {toEffectID(BuiltinEffectID::Recoil), toEffectID(BuiltinEffectID::StatusTick)},
				.mTrigger = BattleEventID::TurnEnd,
			},
		};

		AbilityMeta definition{.mName = "TestAbilityName", .mTriggers = triggers};

		WHEN("calling the string_view overload")
		{
			WHEN("calling with an invalid ability name")
			{
				std::expected<void, RegistryErrorInfo> result{config.updateAbility("ThisIsInvalid", definition)};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::AbilityNotFound));
					CHECK((config.getAmountRegistered() == finalAbilityUnderlyingValue));
				}
			}

			WHEN("trying to rename to a name already in the registry")
			{
				std::expected<void, RegistryErrorInfo> result{
					config.updateAbility(ABILITY_NAME_NONE, {.mName = std::string(ABILITY_NAME_DRIZZLE), .mTriggers = triggers}),
				};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::DuplicateAbility));
					CHECK((config.getAmountRegistered() == finalAbilityUnderlyingValue));
				}
			}

			WHEN("updating an existing ability definition")
			{
				std::expected<void, RegistryErrorInfo> result{config.updateAbility(ABILITY_NAME_NONE, definition)};

				THEN("the registry reports no error and the target is appropriately updated")
				{
					REQUIRE(result.has_value());
					CHECK((config.getAmountRegistered() == finalAbilityUnderlyingValue));
					CHECK((config.getAbilityMetadata(toAbilityID(BuiltinAbilityID::None))->mName == "TestAbilityName"));
					CHECK((config.getAbilityMetadata(toAbilityID(BuiltinAbilityID::None))->mTriggers == triggers));
				}
			}
		}

		WHEN("calling the AbilityID overload")
		{
			WHEN("calling with an invalid ability name")
			{
				std::expected<void, RegistryErrorInfo> result{config.updateAbility(AbilityID{200}, definition)};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::AbilityNotFound));
					CHECK((config.getAmountRegistered() == finalAbilityUnderlyingValue));
				}
			}

			WHEN("trying to rename to a name already in the registry")
			{
				std::expected<void, RegistryErrorInfo> result{
					config.updateAbility(toAbilityID(BuiltinAbilityID::None),
										 {.mName = std::string(ABILITY_NAME_DRIZZLE), .mTriggers = triggers}),
				};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::DuplicateAbility));
					CHECK((config.getAmountRegistered() == finalAbilityUnderlyingValue));
				}
			}

			WHEN("updating an existing ability definition")
			{
				std::expected<void, RegistryErrorInfo> result{config.updateAbility(toAbilityID(BuiltinAbilityID::None), definition)};

				THEN("the registry reports no error and the target is appropriately updated")
				{
					REQUIRE(result.has_value());
					CHECK((config.getAmountRegistered() == finalAbilityUnderlyingValue));
					CHECK((config.getAbilityMetadata(toAbilityID(BuiltinAbilityID::None))->mName == "TestAbilityName"));
					CHECK((config.getAbilityMetadata(toAbilityID(BuiltinAbilityID::None))->mTriggers == triggers));
				}
			}
		}
	}

	GIVEN("removeAbility")
	{
		WHEN("calling the string_view overload")
		{
			WHEN("calling with an unknown ability name")
			{
				std::expected<AbilityID, RegistryErrorInfo> result{config.removeAbility("Unknown")};

				THEN("the result is an error and the registry is not updated")
				{
					CHECK_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::AbilityNotFound));
					CHECK((config.getAmountRegistered() == finalAbilityUnderlyingValue));
				}
			}

			WHEN("calling with a known ability name")
			{
				std::expected<AbilityID, RegistryErrorInfo> result{config.removeAbility(ABILITY_NAME_NONE)};

				THEN("the result is a success and the registry is updated")
				{
					CHECK(result.has_value());
					CHECK((result.value() == toAbilityID(BuiltinAbilityID::None)));
					CHECK((config.getAmountRegistered() == finalAbilityUnderlyingValue - 1));
				}
			}
		}

		WHEN("calling the AbilityID overload")
		{
			WHEN("calling with an unknown ability ID")
			{
				std::expected<AbilityID, RegistryErrorInfo> result{config.removeAbility(AbilityID{200})};

				THEN("the result is an error and the registry is not updated")
				{
					CHECK_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::AbilityNotFound));
					CHECK((config.getAmountRegistered() == finalAbilityUnderlyingValue));
				}
			}

			WHEN("calling with a known ability ID")
			{
				std::expected<AbilityID, RegistryErrorInfo> result{config.removeAbility(toAbilityID(BuiltinAbilityID::None))};

				THEN("the result is a success and the registry is updated")
				{
					CHECK(result.has_value());
					CHECK((result.value() == toAbilityID(BuiltinAbilityID::None)));
					CHECK((config.getAmountRegistered() == finalAbilityUnderlyingValue - 1));
				}
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
