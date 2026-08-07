/*! @file battleState.h
	@brief Contains the battle state
	@date 07/24/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_BATTLE_BATTLESTATE_H
#define INCLUDE_BATTLE_BATTLESTATE_H

#include <vector>

#include "Core/typedefs.h"
#include "Move/moveID.h"
#include "Pokemon/pokemon.h"
#include "Terrain/terrainID.h"
#include "Weather/weatherID.h"

namespace PocketCore::Battle
{
	using PocketCore::Core::sb;
	using PocketCore::Core::ub;
	using PocketCore::Core::us;
	using PocketCore::Move::MoveID;
	using PocketCore::Pokemon::Pokemon;
	using PocketCore::Terrain::TerrainID;
	using PocketCore::Weather::WeatherID;

	struct StatStages
	{
		public:
			sb mAttack{0};
			sb mDefense{0};
			sb mSpAttack{0};
			sb mSpDefense{0};
			sb mSpeed{0};
			sb mAccuracy{0};
			sb mEvasion{0};
	};

	struct DamageFormulaModifiers
	{
		public:
			double mHealthModifier{1.0};
			double mAttackModifier{1.0};
			double mDefenseModifier{1.0};
			double mSpecialAttackModifier{1.0};
			double mSpecialDefenseModifier{1.0};
			double mSpeedModifier{1.0};
	};

	struct BattleSlot
	{
		public:
			DamageFormulaModifiers mDamageFormulaModifiers{};

			Pokemon *mPokemon{nullptr};
			StatStages mStatStages{};

			MoveID mChoiceLockedMove{};

			ub mPosition{0};

			ub mSleepCounter{0};
			ub mToxicCounter{0};
			ub mProtectionCounter{0};

			bool mIsProtected{false};
			bool mIsFlinched{false};
			bool mIsGrounded{false};
			bool mFaintProcessed{false};
	};

	struct BattleState
	{
		public:
			std::vector<BattleSlot> mSideA{};
			std::vector<BattleSlot> mSideB{};

			std::vector<Pokemon *> mPartyA{};
			std::vector<Pokemon *> mPartyB{};

			WeatherID mWeatherID{};
			TerrainID mTerrainID{};

			// Spikes can have 0-3 layers

			ub mSpikesPartyA{0};
			ub mSpikesPartyB{0};

			// Spikes can have 0-2 layers

			ub mToxicSpikesPartyA{0};
			ub mToxicSpikesPartyB{0};

			bool mStealthRockPartyA{false};
			bool mStealthRockPartyB{false};

			bool mBattleStarted{false};
	};
} // namespace PocketCore::Battle

#endif
