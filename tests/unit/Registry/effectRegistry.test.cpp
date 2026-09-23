/*! @file effectRegistry.test.cpp
	@brief C++ file for running tests for the EffectRegistry.
	@date 09/23/2026
	@since 0.12.20
	@version 0.12.43
	@author Matthew Moore
*/

#include "Registry/effectRegistry.h"

#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <utility>

#include "Core/typedefs.h"
#include "Effect/builtInEffectID.h"
#include "Effect/constants.h"
#include "Effect/effectID.h"
#include "Effect/effectMeta.h"
#include "EffectHandler/accuracyCheckHandler.h"
#include "EffectHandler/baseDamageHandler.h"
#include "EffectHandler/burnDamageHandler.h"
#include "EffectHandler/criticalHitHandler.h"
#include "EffectHandler/flinchHandler.h"
#include "EffectHandler/populationBombHandler.h"
#include "EffectHandler/psychicTerrainPriorityBlockHandler.h"
#include "EffectHandler/randomizationHandler.h"
#include "EffectHandler/recoilHandler.h"
#include "EffectHandler/setElectricTerrainHandler.h"
#include "EffectHandler/setGrassyTerrainHandler.h"
#include "EffectHandler/setHarshSunlightHandler.h"
#include "EffectHandler/setRainHandler.h"
#include "EffectHandler/setSandstormHandler.h"
#include "EffectHandler/stabHandler.h"
#include "EffectHandler/statusApplyHandler.h"
#include "EffectHandler/statusRemoveHandler.h"
#include "EffectHandler/statusTickHandler.h"
#include "EffectHandler/statusTurnSkipHandler.h"
#include "EffectHandler/targetsHandler.h"
#include "EffectHandler/terrainHandler.h"
#include "EffectHandler/typeEffectivenessHandler.h"
#include "EffectHandler/weatherHandler.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Core::ub;
using PocketCore::Effect::applyAccuracyCheck;
using PocketCore::Effect::applyBaseDamage;
using PocketCore::Effect::applyBurnDamage;
using PocketCore::Effect::applyCriticalHit;
using PocketCore::Effect::applyFlinch;
using PocketCore::Effect::applyPopulationBomb;
using PocketCore::Effect::applyPsychicTerrainPriorityBlock;
using PocketCore::Effect::applyRandomization;
using PocketCore::Effect::applyRecoil;
using PocketCore::Effect::applySetElectric;
using PocketCore::Effect::applySetGrassy;
using PocketCore::Effect::applySetHarshSunlight;
using PocketCore::Effect::applySetMisty;
using PocketCore::Effect::applySetPsychic;
using PocketCore::Effect::applySetRain;
using PocketCore::Effect::applySetSandstorm;
using PocketCore::Effect::applySTAB;
using PocketCore::Effect::applyStatusApply;
using PocketCore::Effect::applyStatusRemove;
using PocketCore::Effect::applyStatusTick;
using PocketCore::Effect::applyStatusTurnSkip;
using PocketCore::Effect::applyTargets;
using PocketCore::Effect::applyTerrain;
using PocketCore::Effect::applyTypeEffectiveness;
using PocketCore::Effect::applyWeather;
using PocketCore::Effect::BuiltinEffectID;
using PocketCore::Effect::EFFECT_NAME_ACCURACY_CHECK;
using PocketCore::Effect::EFFECT_NAME_BASE_DAMAGE;
using PocketCore::Effect::EFFECT_NAME_BURN_DAMAGE_REDUCTION;
using PocketCore::Effect::EFFECT_NAME_CRITICAL_HIT;
using PocketCore::Effect::EFFECT_NAME_FLINCH;
using PocketCore::Effect::EFFECT_NAME_NONE;
using PocketCore::Effect::EFFECT_NAME_POPULATION_BOMB;
using PocketCore::Effect::EFFECT_NAME_PSYCHIC_TERRAIN_PRIORITY_BLOCK;
using PocketCore::Effect::EFFECT_NAME_RANDOMIZATION;
using PocketCore::Effect::EFFECT_NAME_RECOIL;
using PocketCore::Effect::EFFECT_NAME_SET_ELECTRIC_TERRAIN;
using PocketCore::Effect::EFFECT_NAME_SET_GRASSY_TERRAIN;
using PocketCore::Effect::EFFECT_NAME_SET_HARSH_SUNLIGHT;
using PocketCore::Effect::EFFECT_NAME_SET_MISTY_TERRAIN;
using PocketCore::Effect::EFFECT_NAME_SET_PSYCHIC_TERRAIN;
using PocketCore::Effect::EFFECT_NAME_SET_RAIN;
using PocketCore::Effect::EFFECT_NAME_SET_SANDSTORM;
using PocketCore::Effect::EFFECT_NAME_STAB;
using PocketCore::Effect::EFFECT_NAME_STATUS_APPLY;
using PocketCore::Effect::EFFECT_NAME_STATUS_REMOVE;
using PocketCore::Effect::EFFECT_NAME_STATUS_TICK;
using PocketCore::Effect::EFFECT_NAME_STATUS_TURN_SKIP;
using PocketCore::Effect::EFFECT_NAME_TARGETS;
using PocketCore::Effect::EFFECT_NAME_TERRAIN;
using PocketCore::Effect::EFFECT_NAME_TYPE_EFFECTIVENESS;
using PocketCore::Effect::EFFECT_NAME_WEATHER;
using PocketCore::Effect::EffectID;
using PocketCore::Effect::EffectMeta;
using PocketCore::Effect::NO_EFFECT_ID;
using PocketCore::Effect::toEffectID;
using PocketCore::Registry::Effect::EffectRegistry;

template <typename Registry>
concept PubliclyStructurallyMutable = requires(Registry &registry) { registry.setAmountRegistered(0); };

static_assert(!PubliclyStructurallyMutable<EffectRegistry>);

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("EffectRegistry")
{
	EffectRegistry registry{};
	ub finalEffectUnderlyingValue{std::to_underlying(BuiltinEffectID::FinalEffect)};

	GIVEN("a default constructed effect registry")
	{
		THEN("Critical Hit has the appropriate properties")
		{
			EffectMeta expected{
				.mName = std::string(EFFECT_NAME_CRITICAL_HIT),
				.mApply = applyCriticalHit,
				.mEffectID = toEffectID(BuiltinEffectID::CriticalHit),
			};

			const EffectMeta *actual{registry.getEffectMetadata(toEffectID(BuiltinEffectID::CriticalHit))};

			CHECK((expected == *actual));
		}

		THEN("Base Damage has the appropriate properties")
		{
			EffectMeta expected{
				.mName = std::string(EFFECT_NAME_BASE_DAMAGE),
				.mApply = applyBaseDamage,
				.mEffectID = toEffectID(BuiltinEffectID::BaseDamage),
			};

			const EffectMeta *actual{registry.getEffectMetadata(toEffectID(BuiltinEffectID::BaseDamage))};

			CHECK((expected == *actual));
		}

		THEN("Targets has the appropriate properties")
		{
			EffectMeta expected{
				.mName = std::string(EFFECT_NAME_TARGETS),
				.mApply = applyTargets,
				.mEffectID = toEffectID(BuiltinEffectID::Targets),
			};

			const EffectMeta *actual{registry.getEffectMetadata(toEffectID(BuiltinEffectID::Targets))};

			CHECK((expected == *actual));
		}

		THEN("Weather has the appropriate properties")
		{
			EffectMeta expected{
				.mName = std::string(EFFECT_NAME_WEATHER),
				.mApply = applyWeather,
				.mEffectID = toEffectID(BuiltinEffectID::Weather),
			};

			const EffectMeta *actual{registry.getEffectMetadata(toEffectID(BuiltinEffectID::Weather))};

			CHECK((expected == *actual));
		}

		THEN("Terrain has the appropriate properties")
		{
			EffectMeta expected{
				.mName = std::string(EFFECT_NAME_TERRAIN),
				.mApply = applyTerrain,
				.mEffectID = toEffectID(BuiltinEffectID::Terrain),
			};

			const EffectMeta *actual{registry.getEffectMetadata(toEffectID(BuiltinEffectID::Terrain))};

			CHECK((expected == *actual));
		}

		THEN("Population Bomb has the appropriate properties")
		{
			EffectMeta expected{
				.mName = std::string(EFFECT_NAME_POPULATION_BOMB),
				.mApply = applyPopulationBomb,
				.mEffectID = toEffectID(BuiltinEffectID::PopulationBomb),
			};

			const EffectMeta *actual{registry.getEffectMetadata(toEffectID(BuiltinEffectID::PopulationBomb))};

			CHECK((expected == *actual));
		}

		THEN("Randomization has the appropriate properties")
		{
			EffectMeta expected{
				.mName = std::string(EFFECT_NAME_RANDOMIZATION),
				.mApply = applyRandomization,
				.mEffectID = toEffectID(BuiltinEffectID::Randomization),
			};

			const EffectMeta *actual{registry.getEffectMetadata(toEffectID(BuiltinEffectID::Randomization))};

			CHECK((expected == *actual));
		}

		THEN("STAB has the appropriate properties")
		{
			EffectMeta expected{
				.mName = std::string(EFFECT_NAME_STAB),
				.mApply = applySTAB,
				.mEffectID = toEffectID(BuiltinEffectID::Stab),
			};

			const EffectMeta *actual{registry.getEffectMetadata(toEffectID(BuiltinEffectID::Stab))};

			CHECK((expected == *actual));
		}

		THEN("Type Effectiveness has the appropriate properties")
		{
			EffectMeta expected{
				.mName = std::string(EFFECT_NAME_TYPE_EFFECTIVENESS),
				.mApply = applyTypeEffectiveness,
				.mEffectID = toEffectID(BuiltinEffectID::TypeEffectiveness),
			};

			const EffectMeta *actual{registry.getEffectMetadata(toEffectID(BuiltinEffectID::TypeEffectiveness))};

			CHECK((expected == *actual));
		}

		THEN("Burn Damage Reduction has the appropriate properties")
		{
			EffectMeta expected{
				.mName = std::string(EFFECT_NAME_BURN_DAMAGE_REDUCTION),
				.mApply = applyBurnDamage,
				.mEffectID = toEffectID(BuiltinEffectID::BurnDamageReduction),
			};

			const EffectMeta *actual{registry.getEffectMetadata(toEffectID(BuiltinEffectID::BurnDamageReduction))};

			CHECK((expected == *actual));
		}

		THEN("Flinch has the appropriate properties")
		{
			EffectMeta expected{
				.mName = std::string(EFFECT_NAME_FLINCH),
				.mApply = applyFlinch,
				.mEffectID = toEffectID(BuiltinEffectID::Flinch),
			};

			const EffectMeta *actual{registry.getEffectMetadata(toEffectID(BuiltinEffectID::Flinch))};

			CHECK((expected == *actual));
		}

		THEN("Recoil has the appropriate properties")
		{
			EffectMeta expected{
				.mName = std::string(EFFECT_NAME_RECOIL),
				.mApply = applyRecoil,
				.mEffectID = toEffectID(BuiltinEffectID::Recoil),
			};

			const EffectMeta *actual{registry.getEffectMetadata(toEffectID(BuiltinEffectID::Recoil))};

			CHECK((expected == *actual));
		}

		THEN("Status Apply has the appropriate properties")
		{
			EffectMeta expected{
				.mName = std::string(EFFECT_NAME_STATUS_APPLY),
				.mApply = applyStatusApply,
				.mEffectID = toEffectID(BuiltinEffectID::StatusApply),
				.mMayChangeStatus = true,
			};

			const EffectMeta *actual{registry.getEffectMetadata(toEffectID(BuiltinEffectID::StatusApply))};

			CHECK((expected == *actual));
		}

		THEN("Status Remove has the appropriate properties")
		{
			EffectMeta expected{
				.mName = std::string(EFFECT_NAME_STATUS_REMOVE),
				.mApply = applyStatusRemove,
				.mEffectID = toEffectID(BuiltinEffectID::StatusRemove),
				.mMayChangeStatus = true,
			};

			const EffectMeta *actual{registry.getEffectMetadata(toEffectID(BuiltinEffectID::StatusRemove))};

			CHECK((expected == *actual));
		}

		THEN("Status Turn Skip has the appropriate properties")
		{
			EffectMeta expected{
				.mName = std::string(EFFECT_NAME_STATUS_TURN_SKIP),
				.mApply = applyStatusTurnSkip,
				.mEffectID = toEffectID(BuiltinEffectID::StatusTurnSkip),
			};

			const EffectMeta *actual{registry.getEffectMetadata(toEffectID(BuiltinEffectID::StatusTurnSkip))};

			CHECK((expected == *actual));
		}

		THEN("Status Tick has the appropriate properties")
		{
			EffectMeta expected{
				.mName = std::string(EFFECT_NAME_STATUS_TICK),
				.mApply = applyStatusTick,
				.mEffectID = toEffectID(BuiltinEffectID::StatusTick),
			};

			const EffectMeta *actual{registry.getEffectMetadata(toEffectID(BuiltinEffectID::StatusTick))};

			CHECK((expected == *actual));
		}

		THEN("Accuracy Check has the appropriate properties")
		{
			EffectMeta expected{
				.mName = std::string(EFFECT_NAME_ACCURACY_CHECK),
				.mApply = applyAccuracyCheck,
				.mEffectID = toEffectID(BuiltinEffectID::AccuracyCheck),
			};

			const EffectMeta *actual{registry.getEffectMetadata(toEffectID(BuiltinEffectID::AccuracyCheck))};

			CHECK((expected == *actual));
		}

		THEN("Set Sandstorm has the appropriate properties")
		{
			EffectMeta expected{
				.mName = std::string(EFFECT_NAME_SET_SANDSTORM),
				.mApply = applySetSandstorm,
				.mEffectID = toEffectID(BuiltinEffectID::SetSandstorm),
				.mMayChangeWeather = true,
			};

			const EffectMeta *actual{registry.getEffectMetadata(toEffectID(BuiltinEffectID::SetSandstorm))};

			CHECK((expected == *actual));
		}

		THEN("Set Harsh Sunlight has the appropriate properties")
		{
			EffectMeta expected{
				.mName = std::string(EFFECT_NAME_SET_HARSH_SUNLIGHT),
				.mApply = applySetHarshSunlight,
				.mEffectID = toEffectID(BuiltinEffectID::SetSun),
				.mMayChangeWeather = true,
			};

			const EffectMeta *actual{registry.getEffectMetadata(toEffectID(BuiltinEffectID::SetSun))};

			CHECK((expected == *actual));
		}

		THEN("Set Rain has the appropriate properties")
		{
			EffectMeta expected{
				.mName = std::string(EFFECT_NAME_SET_RAIN),
				.mApply = applySetRain,
				.mEffectID = toEffectID(BuiltinEffectID::SetRain),
				.mMayChangeWeather = true,
			};

			const EffectMeta *actual{registry.getEffectMetadata(toEffectID(BuiltinEffectID::SetRain))};

			CHECK((expected == *actual));
		}

		THEN("Set Grassy Terrain has the appropriate properties")
		{
			EffectMeta expected{
				.mName = std::string(EFFECT_NAME_SET_GRASSY_TERRAIN),
				.mApply = applySetGrassy,
				.mEffectID = toEffectID(BuiltinEffectID::SetGrassyTerrain),
				.mMayChangeTerrain = true,
			};

			const EffectMeta *actual{registry.getEffectMetadata(toEffectID(BuiltinEffectID::SetGrassyTerrain))};

			CHECK((expected == *actual));
		}

		THEN("Set Misty Terrain has the appropriate properties")
		{
			EffectMeta expected{
				.mName = std::string(EFFECT_NAME_SET_MISTY_TERRAIN),
				.mApply = applySetMisty,
				.mEffectID = toEffectID(BuiltinEffectID::SetMistyTerrain),
				.mMayChangeTerrain = true,
			};

			const EffectMeta *actual{registry.getEffectMetadata(toEffectID(BuiltinEffectID::SetMistyTerrain))};

			CHECK((expected == *actual));
		}

		THEN("Set Electric Terrain has the appropriate properties")
		{
			EffectMeta expected{
				.mName = std::string(EFFECT_NAME_SET_ELECTRIC_TERRAIN),
				.mApply = applySetElectric,
				.mEffectID = toEffectID(BuiltinEffectID::SetElectricTerrain),
				.mMayChangeTerrain = true,
			};

			const EffectMeta *actual{registry.getEffectMetadata(toEffectID(BuiltinEffectID::SetElectricTerrain))};

			CHECK((expected == *actual));
		}

		THEN("Set Psychic Terrain has the appropriate properties")
		{
			EffectMeta expected{
				.mName = std::string(EFFECT_NAME_SET_PSYCHIC_TERRAIN),
				.mApply = applySetPsychic,
				.mEffectID = toEffectID(BuiltinEffectID::SetPsychicTerrain),
				.mMayChangeTerrain = true,
			};

			const EffectMeta *actual{registry.getEffectMetadata(toEffectID(BuiltinEffectID::SetPsychicTerrain))};

			CHECK((expected == *actual));
		}

		THEN("Psychic Terrain Priority Block has the appropriate properties")
		{
			EffectMeta expected{
				.mName = std::string(EFFECT_NAME_PSYCHIC_TERRAIN_PRIORITY_BLOCK),
				.mApply = applyPsychicTerrainPriorityBlock,
				.mEffectID = toEffectID(BuiltinEffectID::PsychicTerrainPriorityBlock),
			};

			const EffectMeta *actual{registry.getEffectMetadata(toEffectID(BuiltinEffectID::PsychicTerrainPriorityBlock))};

			CHECK((expected == *actual));
		}
	}

	GIVEN("getEffectMetadata")
	{
		THEN("unknown IDs are absent")
		{
			CHECK((registry.getEffectMetadata(EffectID{200}) == nullptr));
		}

		THEN("the metadata is retrieved when accessed by a valid Effect ID")
		{
			EffectMeta expected{
				.mName = std::string(EFFECT_NAME_NONE),
				.mEffectID = toEffectID(BuiltinEffectID::None),
			};

			CHECK((expected == *registry.getEffectMetadata(NO_EFFECT_ID)));
		}
	}

	GIVEN("getEffectID")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(registry.getEffectID("Unknown").has_value());
		}

		THEN("the Effect ID is retrieved by valid Effect name")
		{
			std::optional<EffectID> effectID{registry.getEffectID(EFFECT_NAME_NONE)};

			REQUIRE(effectID.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((effectID.value() == toEffectID(BuiltinEffectID::None)));
		}
	}

	GIVEN("getEffectName")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(registry.getEffectName(EffectID{200}).has_value());
		}

		THEN("a registered effect name is returned by stable ID")
		{
			std::optional<std::string_view> effectName{registry.getEffectName(toEffectID(BuiltinEffectID::None))};

			REQUIRE(effectName.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((effectName.value() == EFFECT_NAME_NONE));
		}
	}

	GIVEN("getAmountRegistered")
	{
		THEN("the registered span contains the exact amount of built-in entries")
		{
			CHECK((registry.getAmountRegistered() == finalEffectUnderlyingValue));
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
			EffectMeta expected{
				.mName = std::string(EFFECT_NAME_NONE),
				.mEffectID = toEffectID(BuiltinEffectID::None),
			};

			const EffectMeta *effectMeta{registry.getEntry(0)};

			REQUIRE((effectMeta != nullptr));
			CHECK((*effectMeta == expected));
		}
	}

	GIVEN("getRegisteredEffects")
	{
		THEN("the amount of effects returned matches the amount that are built-in")
		{
			CHECK((registry.getRegisteredEffects().size() == finalEffectUnderlyingValue));
		}
	}

	GIVEN("getNextEffectID")
	{
		THEN("the next available stable Effect ID is after all built in effect IDs")
		{
			CHECK((registry.getNextEffectID() == finalEffectUnderlyingValue));
		}
	}

	GIVEN("findIndexByEffectID")
	{
		THEN("an unknown stable ID has no internal index")
		{
			std::optional<ub> effectIndex{registry.findIndexByEffectID(EffectID{200})};
			CHECK_FALSE(effectIndex.has_value());
		}

		THEN("the internal array index is retrieved by valid Effect ID")
		{
			std::optional<ub> effectIndex{registry.findIndexByEffectID(NO_EFFECT_ID)};

			REQUIRE(effectIndex.has_value());
			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((effectIndex.value() == 0));
		}
	}

	GIVEN("hasEffect")
	{
		WHEN("calling the string_view overload")
		{
			THEN("an unknown effect name has no entry")
			{
				CHECK_FALSE(registry.hasEffect("Unknown"));
			}

			THEN("a known effect name has an entry")
			{
				CHECK(registry.hasEffect(EFFECT_NAME_NONE));
			}
		}

		WHEN("calling the EffectID overload")
		{
			THEN("an unknown effect ID has no entry")
			{
				CHECK_FALSE(registry.hasEffect(EffectID{200}));
			}

			THEN("a known effect ID has an entry")
			{
				CHECK(registry.hasEffect(NO_EFFECT_ID));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
