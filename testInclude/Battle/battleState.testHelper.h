/*! @file battleState.testHelper.h
	@brief Test helper for dealing with BattleState concepts.
	@date 08/31/2026
	@since 0.12.13
	@version 0.12.13
	@author Matthew Moore
*/

#ifndef TEST_INCLUDE_BATTLE_BATTLESTATE_TEST_HELPER_H
#define TEST_INCLUDE_BATTLE_BATTLESTATE_TEST_HELPER_H

#include <vector>

#include "Battle/battleState.h"
#include "Core/typedefs.h"
#include "Pokemon/pokemon.h"
#include "Terrain/terrainID.h"
#include "Weather/weatherID.h"

namespace PocketCore::Testing
{
	using PocketCore::Battle::BattleSlot;
	using PocketCore::Battle::BattleState;
	using PocketCore::Core::ub;
	using PocketCore::Pokemon::Pokemon;
	using PocketCore::Terrain::TerrainID;
	using PocketCore::Weather::WeatherID;

	struct BattleStateTestData
	{
		public:
			std::vector<BattleSlot> mSideA{};
			std::vector<BattleSlot> mSideB{};

			std::vector<Pokemon *> mPartyA{};
			std::vector<Pokemon *> mPartyB{};

			WeatherID mWeatherID{};
			TerrainID mTerrainID{};

			ub mSpikesPartyA{0};
			ub mSpikesPartyB{0};

			ub mToxicSpikesPartyA{0};
			ub mToxicSpikesPartyB{0};

			bool mStealthRockPartyA{false};
			bool mStealthRockPartyB{false};

			bool mBattleStarted{false};
	};

	static BattleState makeBattleState(const BattleStateTestData &data)
	{
		BattleState state{};
		state.mSideA = data.mSideA;
		state.mSideB = data.mSideB;
		state.mPartyA = data.mPartyA;
		state.mPartyB = data.mPartyB;
		state.mWeatherID = data.mWeatherID;
		state.mTerrainID = data.mTerrainID;
		state.mSpikesPartyA = data.mSpikesPartyA;
		state.mSpikesPartyB = data.mSpikesPartyB;
		state.mToxicSpikesPartyA = data.mToxicSpikesPartyA;
		state.mToxicSpikesPartyB = data.mToxicSpikesPartyB;
		state.mStealthRockPartyA = data.mStealthRockPartyA;
		state.mStealthRockPartyB = data.mStealthRockPartyB;
		state.mBattleStarted = data.mBattleStarted;

		return state;
	}

} // namespace PocketCore::Testing

#endif
