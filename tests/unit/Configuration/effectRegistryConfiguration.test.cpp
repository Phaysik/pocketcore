/*! @file effectRegistryConfiguration.test.cpp
	@brief C++ file for running tests for the EffectRegistryConfiguration.
	@date 09/10/2026
	@since 0.12.20
	@version 0.12.20
	@author Matthew Moore
*/

#include "Configuration/effectRegistryConfiguration.h"

#include <expected>
#include <format>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "Configuration/constants.h"
#include "Core/typedefs.h"
#include "Effect/builtInEffectID.h"
#include "Effect/constants.h"
#include "Effect/effectID.h"
#include "Effect/effectMeta.h"
#include "Registry/effectRegistry.h"
#include "Registry/registryError.h"
#include "Utility/Debug/Logging/logging.testHelper.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Configuration::EffectRegistryConfiguration;
using PocketCore::Configuration::MAX_EFFECTS;
using PocketCore::Configuration::RegistryError;
using PocketCore::Core::ub;
using PocketCore::Core::us;
using PocketCore::Effect::BuiltinEffectID;
using PocketCore::Effect::EFFECT_NAME_ACCURACY_CHECK;
using PocketCore::Effect::EFFECT_NAME_NONE;
using PocketCore::Effect::EffectID;
using PocketCore::Effect::EffectMeta;
using PocketCore::Effect::NO_EFFECT_ID;
using PocketCore::Effect::toEffectID;
using PocketCore::Registry::Effect::EffectRegistry;
using PocketCore::Registry::RegistryErrorInfo;
using PocketCore::Testing::ensureLoggerInitialized;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("EffectRegistryConfiguration")
{
	ensureLoggerInitialized("effect registry configuration test", "effectRegistryConfiguration_test.log");

	EffectRegistryConfiguration config{};
	EffectRegistry registry{};
	ub finalEffectUnderlyingValue{std::to_underlying(BuiltinEffectID::FinalEffect)};

	GIVEN("getAmountRegistered")
	{
		THEN("the default registry returns the expected amount of built-in effects")
		{
			CHECK((config.getAmountRegistered() == finalEffectUnderlyingValue));
		}
	}

	GIVEN("getRuntimeRegistry")
	{
		THEN("the runtime registry is the same as the default registry")
		{
			CHECK((config.getRuntimeRegistry() == registry));
		}
	}

	GIVEN("getEffectMetadata")
	{
		THEN("unknown IDs are absent")
		{
			CHECK((config.getEffectMetadata(EffectID{200}) == nullptr));
		}

		THEN("the metadata is retrieved when accessed by a valid Effect ID")
		{
			EffectMeta expected{
				.mName = std::string(EFFECT_NAME_NONE),
				.mEffectID = toEffectID(BuiltinEffectID::None),
			};

			CHECK((expected == *config.getEffectMetadata(NO_EFFECT_ID)));
		}
	}

	GIVEN("getEffectID")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(config.getEffectID("Unknown").has_value());
		}

		THEN("the Effect ID is retrieved by valid Effect name")
		{
			std::optional<EffectID> effectID{config.getEffectID(EFFECT_NAME_NONE)};

			REQUIRE(effectID.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((effectID.value() == toEffectID(BuiltinEffectID::None)));
		}
	}

	GIVEN("getEffectName")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(config.getEffectName(EffectID{200}).has_value());
		}

		THEN("a registered effect name is returned by stable ID")
		{
			std::optional<std::string_view> effectName{config.getEffectName(toEffectID(BuiltinEffectID::None))};

			REQUIRE(effectName.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((effectName.value() == EFFECT_NAME_NONE));
		}
	}

	GIVEN("getRegisteredEffects")
	{
		THEN("the amount of effects returned matches the amount that are built-in")
		{
			CHECK((config.getRegisteredEffects().size() == finalEffectUnderlyingValue));
		}
	}

	GIVEN("hasEffect")
	{
		WHEN("calling the string_view overload")
		{
			THEN("an unknown effect name has no entry")
			{
				CHECK_FALSE(config.hasEffect("Unknown"));
			}

			THEN("a known effect name has an entry")
			{
				CHECK(config.hasEffect(EFFECT_NAME_NONE));
			}
		}

		WHEN("calling the EffectID overload")
		{
			THEN("an unknown effect ID has no entry")
			{
				CHECK_FALSE(config.hasEffect(EffectID{200}));
			}

			THEN("a known effect ID has an entry")
			{
				CHECK(config.hasEffect(NO_EFFECT_ID));
			}
		}
	}

	GIVEN("addEffect")
	{
		WHEN("trying to add an effect past the capacity")
		{
			us newEffectCount{finalEffectUnderlyingValue};

			for (us i{0}; i < MAX_EFFECTS - finalEffectUnderlyingValue; ++i)
			{
				std::string name{std::format("String_{:04}", i)};
				EffectMeta definition{.mName = name};
				std::expected<EffectID, RegistryErrorInfo> result{config.addEffect(definition)};

				REQUIRE(result.has_value());

				EffectID assignedID{result.value()};
				CHECK((assignedID.getValue() == newEffectCount++));
			}

			std::string name{std::format("String_{:04}", MAX_EFFECTS + 1)};

			EffectMeta definition{.mName = name};
			std::expected<EffectID, RegistryErrorInfo> result{config.addEffect(definition)};

			THEN("registration reports a max capacity and nothing is added")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::MaxCapacity));
				CHECK((config.getAmountRegistered() == MAX_EFFECTS));
			}
		}

		WHEN("an effect whose name is already in use is added")
		{
			EffectMeta definition{.mName = std::string(EFFECT_NAME_ACCURACY_CHECK)};
			std::expected<EffectID, RegistryErrorInfo> result{config.addEffect(definition)};

			THEN("registration reports a duplicate effect and nothing is added")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::DuplicateEffect));
				CHECK((config.getAmountRegistered() == finalEffectUnderlyingValue));
			}
		}

		WHEN("a unique effect definition is added")
		{
			EffectMeta definition{.mName = "TestEffectName"};

			std::expected<EffectID, RegistryErrorInfo> result{config.addEffect(definition)};

			THEN("it receives the first custom stable ID and owns its trigger data")
			{
				REQUIRE(result.has_value());
				EffectID assignedID{result.value()};
				CHECK((assignedID.getValue() == finalEffectUnderlyingValue));

				const EffectMeta *metadata{config.getEffectMetadata(assignedID)};

				REQUIRE((metadata != nullptr));
				CHECK((metadata->mName == "TestEffectName"));
			}
		}
	}

	GIVEN("addEffects")
	{
		WHEN("trying to add an effect past the capacity")
		{
			std::vector<EffectMeta> effectMetas;

			for (us i{0}; i < MAX_EFFECTS - finalEffectUnderlyingValue; ++i)
			{
				std::string name{std::format("String_{:04}", i)};
				effectMetas.push_back({.mName = name});
			}

			std::string name{std::format("String_{:04}", MAX_EFFECTS + 1)};

			effectMetas.push_back({.mName = name});
			std::expected<void, RegistryErrorInfo> result{config.addEffects(effectMetas)};

			THEN("registration reports a max capacity and nothing is added")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::MaxCapacity));
				CHECK((config.getAmountRegistered() == finalEffectUnderlyingValue));
			}
		}

		WHEN("an effect whose name is already in use is added")
		{
			std::vector<EffectMeta> effectMetas{};

			for (us i{0}; i < 20; ++i)
			{
				std::string name{std::format("String_{:04}", i)};
				effectMetas.push_back({.mName = name});
			}

			effectMetas.push_back({.mName = std::string(EFFECT_NAME_ACCURACY_CHECK)});
			std::expected<void, RegistryErrorInfo> result{config.addEffects(effectMetas)};

			THEN("registration reports a duplicate effect and the registry is rollback to the checkpoint before the erroneous addition")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::DuplicateEffect));
				CHECK((config.getAmountRegistered() == finalEffectUnderlyingValue));
			}
		}

		WHEN("a unique effect definition is added")
		{
			std::vector<EffectMeta> effectMetas{};

			effectMetas.push_back({.mName = "TestEffectName"});

			std::expected<void, RegistryErrorInfo> result{config.addEffects(effectMetas)};

			THEN("the registry reports no error and all the effect definitions are added")
			{
				REQUIRE(result.has_value());
				CHECK((config.getAmountRegistered() == finalEffectUnderlyingValue + 1));
			}
		}
	}

	GIVEN("renameEffect")
	{
		WHEN("calling with an invalid effect name")
		{
			std::expected<void, RegistryErrorInfo> result{config.renameEffect("ThisIsInvalid", "NewName")};

			THEN("the registry reports an error and there is no update to the registry")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::EffectNotFound));
				CHECK((config.getAmountRegistered() == finalEffectUnderlyingValue));
			}
		}

		WHEN("trying to rename to a name already in the registry")
		{
			std::expected<void, RegistryErrorInfo> result{
				config.renameEffect(EFFECT_NAME_NONE, EFFECT_NAME_ACCURACY_CHECK),
			};

			THEN("the registry reports an error and there is no update to the registry")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::DuplicateEffect));
				CHECK((config.getAmountRegistered() == finalEffectUnderlyingValue));
			}
		}

		WHEN("updating an existing effect definition")
		{
			std::expected<void, RegistryErrorInfo> result{config.renameEffect(EFFECT_NAME_NONE, "NewName")};

			THEN("the registry reports no error and the name is appropriately updated")
			{
				REQUIRE(result.has_value());
				CHECK((config.getAmountRegistered() == finalEffectUnderlyingValue));
				CHECK((config.getEffectMetadata(toEffectID(BuiltinEffectID::None))->mName == "NewName"));
			}
		}
	}

	GIVEN("updateEffect")
	{
		EffectMeta definition{.mName = "TestEffectName"};

		WHEN("calling the string_view overload")
		{
			WHEN("calling with an invalid effect name")
			{
				std::expected<void, RegistryErrorInfo> result{config.updateEffect("ThisIsInvalid", definition)};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::EffectNotFound));
					CHECK((config.getAmountRegistered() == finalEffectUnderlyingValue));
				}
			}

			WHEN("trying to rename to a name already in the registry")
			{
				std::expected<void, RegistryErrorInfo> result{
					config.updateEffect(EFFECT_NAME_NONE, {.mName = std::string(EFFECT_NAME_ACCURACY_CHECK)}),
				};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::DuplicateEffect));
					CHECK((config.getAmountRegistered() == finalEffectUnderlyingValue));
				}
			}

			WHEN("updating an existing effect definition")
			{
				std::expected<void, RegistryErrorInfo> result{config.updateEffect(EFFECT_NAME_NONE, definition)};

				THEN("the registry reports no error and the target is appropriately updated")
				{
					REQUIRE(result.has_value());
					CHECK((config.getAmountRegistered() == finalEffectUnderlyingValue));
					CHECK((config.getEffectMetadata(toEffectID(BuiltinEffectID::None))->mName == "TestEffectName"));
				}
			}
		}

		WHEN("calling the EffectID overload")
		{
			WHEN("calling with an invalid effect name")
			{
				std::expected<void, RegistryErrorInfo> result{config.updateEffect(EffectID{200}, definition)};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::EffectNotFound));
					CHECK((config.getAmountRegistered() == finalEffectUnderlyingValue));
				}
			}

			WHEN("trying to rename to a name already in the registry")
			{
				std::expected<void, RegistryErrorInfo> result{
					config.updateEffect(toEffectID(BuiltinEffectID::None), {.mName = std::string(EFFECT_NAME_ACCURACY_CHECK)}),
				};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::DuplicateEffect));
					CHECK((config.getAmountRegistered() == finalEffectUnderlyingValue));
				}
			}

			WHEN("updating an existing effect definition")
			{
				std::expected<void, RegistryErrorInfo> result{config.updateEffect(toEffectID(BuiltinEffectID::None), definition)};

				THEN("the registry reports no error and the target is appropriately updated")
				{
					REQUIRE(result.has_value());
					CHECK((config.getAmountRegistered() == finalEffectUnderlyingValue));
					CHECK((config.getEffectMetadata(toEffectID(BuiltinEffectID::None))->mName == "TestEffectName"));
				}
			}
		}
	}

	GIVEN("removeEffect")
	{
		WHEN("calling the string_view overload")
		{
			WHEN("calling with an unknown effect name")
			{
				std::expected<EffectID, RegistryErrorInfo> result{config.removeEffect("Unknown")};

				THEN("the result is an error and the registry is not updated")
				{
					CHECK_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::EffectNotFound));
					CHECK((config.getAmountRegistered() == finalEffectUnderlyingValue));
				}
			}

			WHEN("calling with a known effect name")
			{
				std::expected<EffectID, RegistryErrorInfo> result{config.removeEffect(EFFECT_NAME_NONE)};

				THEN("the result is a success and the registry is updated")
				{
					CHECK(result.has_value());
					CHECK((result.value() == toEffectID(BuiltinEffectID::None)));
					CHECK((config.getAmountRegistered() == finalEffectUnderlyingValue - 1));
				}
			}
		}

		WHEN("calling the EffectID overload")
		{
			WHEN("calling with an unknown effect ID")
			{
				std::expected<EffectID, RegistryErrorInfo> result{config.removeEffect(EffectID{200})};

				THEN("the result is an error and the registry is not updated")
				{
					CHECK_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::EffectNotFound));
					CHECK((config.getAmountRegistered() == finalEffectUnderlyingValue));
				}
			}

			WHEN("calling with a known effect ID")
			{
				std::expected<EffectID, RegistryErrorInfo> result{config.removeEffect(toEffectID(BuiltinEffectID::None))};

				THEN("the result is a success and the registry is updated")
				{
					CHECK(result.has_value());
					CHECK((result.value() == toEffectID(BuiltinEffectID::None)));
					CHECK((config.getAmountRegistered() == finalEffectUnderlyingValue - 1));
				}
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
