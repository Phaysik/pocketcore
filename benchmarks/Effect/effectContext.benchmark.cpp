/*! @file effectContext.benchmark.cpp
	@brief Contains benchmarks for the effect context
	@date 08/27/2026
	@since 0.8.7
	@version 0.12.8
	@author Matthew Moore
*/

#include "Effect/effectContext.h"

#include <array>

#include "Multiplier/builtInMultiplierID.h"

#include <benchmark/benchmark.h>

namespace
{
	using PocketCore::Effect::EffectContext;
	using PocketCore::Multiplier::BuiltinMultiplierID;
	using PocketCore::Multiplier::toMultiplierID;

	constexpr std::array MULTIPLIERS{
		BuiltinMultiplierID::Ability, BuiltinMultiplierID::Item,	 BuiltinMultiplierID::Targets,
		BuiltinMultiplierID::Weather, BuiltinMultiplierID::Critical, BuiltinMultiplierID::Stab,
	};
	constexpr float INSERT_MULTIPLIER{1.5F};
	constexpr float APPLY_MULTIPLIER{1.1F};
	constexpr PocketCore::Core::us BASE_DAMAGE{100U};
} // namespace

static void BM_EffectContext_SetAllBuiltinMultipliers(benchmark::State &state) // NOLINT(misc-use-anonymous-namespace)
{
	for (auto benchmarkIteration : state)
	{
		static_cast<void>(benchmarkIteration);
		state.PauseTiming();
		EffectContext context{};
		state.ResumeTiming();

		for (const BuiltinMultiplierID multiplierID : MULTIPLIERS)
		{
			context.setMultiplier(toMultiplierID(multiplierID), INSERT_MULTIPLIER);
		}

		benchmark::DoNotOptimize(context);
		benchmark::ClobberMemory();
	}
}

static void BM_EffectContext_ApplyAllBuiltinMultipliers(benchmark::State &state) // NOLINT(misc-use-anonymous-namespace)
{
	EffectContext context{};
	for (const BuiltinMultiplierID multiplierID : MULTIPLIERS)
	{
		context.setMultiplier(toMultiplierID(multiplierID), APPLY_MULTIPLIER);
	}

	for (auto benchmarkIteration : state)
	{
		static_cast<void>(benchmarkIteration);
		auto damage{context.applyMultiplier(BASE_DAMAGE)};
		benchmark::DoNotOptimize(damage);
	}
}

static void BM_EffectContext_UpdateAllBuiltinMultipliers(benchmark::State &state) // NOLINT(misc-use-anonymous-namespace)
{
	EffectContext context{};
	for (const BuiltinMultiplierID multiplierID : MULTIPLIERS)
	{
		context.setMultiplier(toMultiplierID(multiplierID), INSERT_MULTIPLIER);
	}

	for (auto benchmarkIteration : state)
	{
		static_cast<void>(benchmarkIteration);
		for (const BuiltinMultiplierID multiplierID : MULTIPLIERS)
		{
			context.setMultiplier(toMultiplierID(multiplierID), APPLY_MULTIPLIER);
		}

		benchmark::DoNotOptimize(context);
		benchmark::ClobberMemory();
	}
}

BENCHMARK(BM_EffectContext_SetAllBuiltinMultipliers);
BENCHMARK(BM_EffectContext_ApplyAllBuiltinMultipliers);
BENCHMARK(BM_EffectContext_UpdateAllBuiltinMultipliers);

BENCHMARK_MAIN();
