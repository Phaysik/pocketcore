/*! @file fixedMetadataRegistry.benchmark.cpp
	@brief Contains benchmarks for the fixed metadata registry
	@date 08/03/2026
	@since 0.8.7
	@version 0.8.7
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
} // namespace

static MoveRegistry makeFullMoveRegistry() // NOLINT(misc-use-anonymous-namespace)
{
	MoveRegistry registry{};

	while (registry.getAmountRegistered() < MAX_MOVES)
	{
		const us index{registry.getAmountRegistered()};
		const MoveID moveID{registry.getNextMoveID()};
		registry.setEntry(index, MoveMeta{.mTriggers = {}, .mName = "Benchmark Move", .mMoveID = moveID});
		registry.incrementAmountRegistered();
		registry.incrementNextMoveID();
	}

	return registry;
}

static void BM_FixedMetadataRegistry_FindMoveByID(benchmark::State &state) // NOLINT(misc-use-anonymous-namespace)
{
	const MoveRegistry registry{makeFullMoveRegistry()};
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
