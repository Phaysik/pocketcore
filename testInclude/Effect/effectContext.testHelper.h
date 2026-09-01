/*! @file effectContext.testHelper.h
	@brief Test helper for dealing with EffectContext concepts.
	@date 08/31/2026
	@since 0.12.13
	@version 0.12.13
	@author Matthew Moore
*/

#ifndef TEST_INCLUDE_EFFECT_EFFECTCONTEXT_TEST_HELPER_H
#define TEST_INCLUDE_EFFECT_EFFECTCONTEXT_TEST_HELPER_H

#include "Ability/abilityID.h"
#include "Battle/battleTargetsAndTriggers.h"
#include "Core/typedefs.h"
#include "Effect/effectContext.h"
#include "Effect/effectSourceAndSuppression.h"
#include "Item/itemID.h"
#include "Move/moveID.h"
#include "Nature/natureID.h"
#include "Pokemon/pokemon.h"
#include "Terrain/terrainID.h"
#include "Types/typeID.h"
#include "Weather/weatherID.h"

namespace PocketCore::Testing
{
	using PocketCore::Ability::AbilityID;
	using PocketCore::Battle::BattleRangeID;
	using PocketCore::Core::ub;
	using PocketCore::Core::us;
	using PocketCore::Effect::DamageContext;
	using PocketCore::Effect::EffectContext;
	using PocketCore::Effect::EffectSource;
	using PocketCore::Effect::Side;
	using PocketCore::Item::ItemID;
	using PocketCore::Move::MoveID;
	using PocketCore::Nature::NatureID;
	using PocketCore::Pokemon::Pokemon;
	using PocketCore::Terrain::TerrainID;
	using PocketCore::Type::TypeID;
	using PocketCore::Weather::WeatherID;

	struct EffectContextTestData
	{
		public:
			DamageContext mDamage{};

			TypeID mMoveTypeID{};
			AbilityID mAbilityID{};
			ItemID mItemID{};
			MoveID mMoveID{};
			NatureID mNatureID{};

			std::optional<BattleRangeID> mRangeOverride{};

			ub mUserIndex{0};
			ub mTargetIndex{0};
			us mMoveBasePower{0};
			ub mMoveAccuracy{0};
			ub mHitAttemptIndex{0};

			EffectSource mSourceType{EffectSource::None};

			Side mUserSide{};
			Side mTargetSide{};

			bool mIsSpecial{false};
	};

	constexpr EffectContext makeEffectContext(const EffectContextTestData &data)
	{
		EffectContext context{};
		context.mDamage = data.mDamage;
		context.mMoveTypeID = data.mMoveTypeID;
		context.mAbilityID = data.mAbilityID;
		context.mItemID = data.mItemID;
		context.mMoveID = data.mMoveID;
		context.mNatureID = data.mNatureID;
		context.mRangeOverride = data.mRangeOverride;
		context.mUserIndex = data.mUserIndex;
		context.mTargetIndex = data.mTargetIndex;
		context.mMoveBasePower = data.mMoveBasePower;
		context.mMoveAccuracy = data.mMoveAccuracy;
		context.mHitAttemptIndex = data.mHitAttemptIndex;
		context.mSourceType = data.mSourceType;
		context.mUserSide = data.mUserSide;
		context.mTargetSide = data.mTargetSide;
		context.mIsSpecial = data.mIsSpecial;

		return context;
	}

} // namespace PocketCore::Testing

#endif
