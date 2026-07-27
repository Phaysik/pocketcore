/*! @file effectContext.h
	@brief Contains the effect context
	@date 07/24/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_EFFECT_EFFECTCONTEXT_H
#define INCLUDE_EFFECT_EFFECTCONTEXT_H

#include "Ability/abilityID.h"
#include "Core/typedefs.h"
#include "Item/itemID.h"
#include "Move/moveID.h"
#include "Pokemon/pokemon.h"
#include "Status/statusID.h"
#include "Types/typeID.h"

namespace PocketCore::Effect
{
	using PocketCore::Ability::AbilityID;
	using PocketCore::Core::ub;
	using PocketCore::Core::us;
	using PocketCore::Item::ItemID;
	using PocketCore::Move::MoveID;
	using PocketCore::Pokemon::Pokemon;
	using PocketCore::Status::StatusID;
	using PocketCore::Types::TypeID;

	enum class Side : ub
	{
		A,
		B,
	};

	enum class EffectSource : ub
	{
		None,
		Move,
		Ability,
		Item,
		Hazard,
	};

	struct DamageContext
	{
		public:
			float mRecoilRatio{0.0F};
			us mDamage{0};

			bool mIsCritical{false};
			bool mIsMiss{false};
			bool mIsProtected{false};
			bool mShouldApplyDamage{true};
			bool mShouldContinue{true};
	};

	struct EffectResult
	{
		public:
			ub mStatChangeAtk{0};
			ub mStatChangeDef{0};
			ub mSleepTurns{0};

			StatusID mStatusToApply{StatusID::None};
	};

	struct EffectContext
	{
		public:
			DamageContext mDamage{};
			EffectResult mResult{};

			float mAbilityMultiplier{1.0F};
			float mItemMultiplier{1.0F};
			float mCriticalMultiplier{1.0F};
			
			TypeID mMoveTypeID{};
			AbilityID mAbilityID{};

			ub mUserIndex{0};
			ub mTargetIndex{0};
			ub mMoveBasePower{0};
			ub moveAccuracy{0};

			EffectSource mSourceType{EffectSource::None};
			MoveID mMoveID{MoveID::None};
			Side mUserSide{};
			Side mTargetSide{};

			bool mIsSpecial{false};
	};
} // namespace PocketCore::Effect

#endif
