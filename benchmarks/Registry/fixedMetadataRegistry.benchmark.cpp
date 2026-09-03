/*! @file fixedMetadataRegistry.benchmark.cpp
	@brief Contains benchmarks for the fixed metadata registry
	@date 09/03/2026
	@since 0.8.7
	@version 0.12.18
	@author Matthew Moore
*/

#include <array>

#include "Configuration/constants.h"
#include "Move/moveID.h"
#include "Move/moveMeta.h"
#include "Registry/moveRegistry.h"

#include <benchmark/benchmark.h>

namespace
{
	using PocketCore::Configuration::MAX_MOVES;
	using PocketCore::Core::us;
	using PocketCore::Move::MoveID;
	using PocketCore::Move::MoveMeta;
	using PocketCore::Registry::Move::MoveRegistry;

	class BenchmarkMoveRegistry : public MoveRegistry
	{
		public:
			explicit BenchmarkMoveRegistry() = default;

			using MoveRegistry::addEntry;
	};
} // namespace

static BenchmarkMoveRegistry makeFullMoveRegistry() // NOLINT(misc-use-anonymous-namespace)
{
	BenchmarkMoveRegistry registry{};

	while (registry.getAmountRegistered() < MAX_MOVES)
	{
		benchmark::DoNotOptimize(registry.addEntry(MoveMeta{.mTriggers = {}, .mName = "Benchmark Move"}));
	}

	return registry;
}

static void BM_FixedMetadataRegistry_FindMoveByID(benchmark::State &state) // NOLINT(misc-use-anonymous-namespace)
{
	const BenchmarkMoveRegistry registry{makeFullMoveRegistry()};
	const std::array lookupIDs{
		MoveID{0U},
		MoveID{static_cast<us>(MAX_MOVES / 2U)},
		MoveID{static_cast<us>(MAX_MOVES - 1U)},
	};
	const MoveID lookupID{lookupIDs.at(static_cast<std::size_t>(state.range(0)))};

	for (auto benchmarkIteration : state)
	{
		static_cast<void>(benchmarkIteration);
		const MoveMeta *metadata{registry.getMoveMetadata(lookupID)};
		benchmark::DoNotOptimize(metadata);
	}
}

BENCHMARK(BM_FixedMetadataRegistry_FindMoveByID)->DenseRange(0, 2);
