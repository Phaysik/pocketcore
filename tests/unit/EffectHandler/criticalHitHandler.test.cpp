#include "EffectHandler/criticalHitHandler.h"

#include <cmath>
#include <optional>

#include "Configuration/constants.h"
#include "Effect/effectContext.h"
#include "Multiplier/builtinMultiplierID.h"
#include "Registry/registryProvider.h"
#include "Utility/random.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Battle::BattleState;
using PocketCore::Configuration::CRITICAL_HIT_MULTIPLIER;
using PocketCore::Configuration::CRITICAL_HIT_PERCENTAGE;
using PocketCore::Configuration::MAX_CRITICAL_HIT_VALUE;
using PocketCore::Configuration::MIN_CRITICAL_HIT_VALUE;
using PocketCore::Core::ub;
using PocketCore::Effect::CriticalHitHandler;
using PocketCore::Effect::EffectContext;
using PocketCore::Multiplier::BuiltinMultiplierID;
using PocketCore::Multiplier::toMultiplierID;
using PocketCore::Registry::RegistryProvider;
using PocketCore::Utility::Random;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,llvm-prefer-static-over-anonymous-namespace)

namespace
{
	RegistryProvider makeNullProvider()
	{
		return RegistryProvider{
			.abilityRegistry = nullptr,
			.moveRegistry = nullptr,
			.itemRegistry = nullptr,
			.typeRegistry = nullptr,
			.statusRegistry = nullptr,
			.weatherRegistry = nullptr,
			.terrainRegistry = nullptr,
			.multiplierRegistry = nullptr,
		};
	}

	bool hasCriticalMultiplier(const EffectContext &effectContext)
	{
		return std::ranges::any_of(effectContext.getActiveMultipliers(), [](const auto &multiplierPair) {
			return multiplierPair.first == toMultiplierID(BuiltinMultiplierID::Critical)
				&& std::fabs(multiplierPair.second - CRITICAL_HIT_MULTIPLIER) < 0.0001F;
		});
	}
} // namespace

SCENARIO("CriticalHitHandler")
{
	CriticalHitHandler criticalHitHandler{};
	BattleState battleState{};
	RegistryProvider provider{makeNullProvider()};

	GIVEN("a deterministic random roll around the critical threshold")
	{
		EffectContext effectContext{};

		WHEN("critical hit handling is applied")
		{
			Random::getTwister().seed(17U);
			const ub roll{Random::get<ub>(MIN_CRITICAL_HIT_VALUE, MAX_CRITICAL_HIT_VALUE)};
			Random::getTwister().seed(17U);

			criticalHitHandler.apply(battleState, effectContext, provider);

			THEN("critical flags match the random threshold decision")
			{
				const bool shouldCrit{roll < CRITICAL_HIT_PERCENTAGE};
				CHECK((effectContext.mDamage.mIsCritical == shouldCrit));
				CHECK((hasCriticalMultiplier(effectContext) == shouldCrit));
			}
		}
	}

	GIVEN("a seed that deterministically produces a critical roll")
	{
		EffectContext effectContext{};

		WHEN("critical hit handling is applied")
		{
			std::optional<unsigned int> forcedCriticalSeed{};

			for (unsigned int seed{0U}; seed < 200'000U; ++seed)
			{
				Random::getTwister().seed(seed);
				const ub roll{Random::get<ub>(MIN_CRITICAL_HIT_VALUE, MAX_CRITICAL_HIT_VALUE)};

				if (roll < CRITICAL_HIT_PERCENTAGE)
				{
					forcedCriticalSeed = seed;
					break;
				}
			}

			REQUIRE(forcedCriticalSeed.has_value());
			Random::getTwister().seed(forcedCriticalSeed.value());

			criticalHitHandler.apply(battleState, effectContext, provider);

			THEN("critical state and multiplier are set")
			{
				CHECK(effectContext.mDamage.mIsCritical);
				CHECK(hasCriticalMultiplier(effectContext));
			}
		}
	}

	GIVEN("a random roll that does not crit")
	{
		EffectContext effectContext{};

		WHEN("critical hit handling is applied")
		{
			Random::getTwister().seed(9'999U);
			const ub roll{Random::get<ub>(MIN_CRITICAL_HIT_VALUE, MAX_CRITICAL_HIT_VALUE)};
			REQUIRE((roll >= CRITICAL_HIT_PERCENTAGE));
			Random::getTwister().seed(9'999U);

			criticalHitHandler.apply(battleState, effectContext, provider);

			THEN("critical state remains unset")
			{
				CHECK_FALSE(effectContext.mDamage.mIsCritical);
				CHECK_FALSE(hasCriticalMultiplier(effectContext));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,llvm-prefer-static-over-anonymous-namespace)
