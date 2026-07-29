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
#include "Item/itemID.h"
#include "Move/moveID.h"
#include "Pokemon/pokemon.h"
#include "Status/statusID.h"

namespace PocketCore::Battle
{
	using PocketCore::Core::sb;
	using PocketCore::Core::ub;
	using PocketCore::Core::us;
	using PocketCore::Item::ItemID;
	using PocketCore::Move::MoveID;
	using PocketCore::Pokemon::Pokemon;
	using PocketCore::Status::StatusID;

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

	struct BattleSlot
	{
		public:
			StatStages mStatStages{};
			const Pokemon *mPokemon{nullptr};

			float mSpeedBoost{1.0F};

			us mCurrentHp{0};

			ItemID mItemID{};
			MoveID mChoiceLockedMove{};
			StatusID mStatus{};

			ub mSleepCounter{0};
			ub mToxicCounter{0};
			ub mProtectionCounter{0};
			ub mPosition{0};

			bool mIsProtected{false};
			bool mIsFainted{false};
			bool mIsFlinched{false};
			bool mItemConsumed{false};
	};

	struct BattleState
	{
		public:
			std::vector<BattleSlot> mSideA{};
			std::vector<BattleSlot> mSideB{};

			std::vector<const Pokemon *> mPartyA{};
			std::vector<const Pokemon *> mPartyB{};

			// Spikes can have 0-3 layers

			ub mSpikesPartyA{0};
			ub mSpikesPartyB{0};

			// Spikes can have 0-2 layers

			ub mToxicSpikesPartyA{0};
			ub mToxicSpikesPartyB{0};

			bool mStealthRockPartyA{false};
			bool mStealthRockPartyB{false};

			bool mBattleStarted{false};

			bool mWeatherRain{false};
			bool mWeatherSun{false};
			bool mWeatherSandstorm{false};

			bool mTerrainElectric{false};
			bool mTerrainGrassy{false};
			bool mTerrainMisty{false};
			bool mTerrainPsychic{false};
	};
} // namespace PocketCore::Battle

#endif
