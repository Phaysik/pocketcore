#include "Move/moveMeta.h"

#include <benchmark/benchmark.h>

static void BM_MoveMeta_CreateBaseAttackEffects(benchmark::State &state) // NOLINT(misc-use-anonymous-namespace)
{
	for (auto benchmarkIteration : state)
	{
		static_cast<void>(benchmarkIteration);
		auto effects{PocketCore::Move::baseAttackEffects()};
		benchmark::DoNotOptimize(effects);
		benchmark::ClobberMemory();
	}
}

static void BM_MoveMeta_CreateDerivedAttackEffects(benchmark::State &state) // NOLINT(misc-use-anonymous-namespace)
{
	for (auto benchmarkIteration : state)
	{
		static_cast<void>(benchmarkIteration);
		auto recoil{PocketCore::Move::baseAttackWithRecoil()};
		auto status{PocketCore::Move::baseAttackWithStatus()};
		auto flinch{PocketCore::Move::baseAttackWithFlinch()};
		benchmark::DoNotOptimize(recoil);
		benchmark::DoNotOptimize(status);
		benchmark::DoNotOptimize(flinch);
		benchmark::ClobberMemory();
	}
}

BENCHMARK(BM_MoveMeta_CreateBaseAttackEffects);
BENCHMARK(BM_MoveMeta_CreateDerivedAttackEffects);