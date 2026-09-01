/*! @file moveRegistry.testHelper.h
	@brief Test helper for dealing with MoveRegistry concepts.
	@date 09/01/2026
	@since 0.12.14
	@version 0.12.14
	@author Matthew Moore
*/

#ifndef TEST_INCLUDE_REGISTRY_MOVE_REGISTRY_TEST_HELPER_H
#define TEST_INCLUDE_REGISTRY_MOVE_REGISTRY_TEST_HELPER_H

#include <vector>

#include "Battle/battleTargetsAndTriggers.h"
#include "Core/typedefs.h"
#include "Effect/effectTrigger.h"
#include "Move/moveHitPolicy.h"
#include "Move/moveID.h"
#include "Move/moveMeta.h"
#include "Registry/moveRegistry.h"
#include "Types/typeID.h"

namespace PocketCore::Testing
{
	using PocketCore::Battle::BattleRangeID;
	using PocketCore::Battle::BattleTargetID;
	using PocketCore::Core::sb;
	using PocketCore::Core::ub;
	using PocketCore::Core::us;
	using PocketCore::Effect::EffectTrigger;
	using PocketCore::Move::FixedHitCount;
	using PocketCore::Move::HitCountPolicy;
	using PocketCore::Move::MoveID;
	using PocketCore::Move::MoveMeta;
	using PocketCore::Registry::Move::MoveRegistry;
	using PocketCore::Type::TypeID;

	struct RegisterMoveTestData
	{
		public:
			HitCountPolicy mHitCountPolicy{FixedHitCount{}};
			std::vector<EffectTrigger> mTriggers{};
			std::string_view mName{};
			TypeID mTypeID{};
			us mPower{};
			BattleTargetID mTargetID{};
			BattleRangeID mRangeID{};
			ub mAccuracy{};
			sb mPriority{};
			bool mSpecial{};
	};

	constexpr MoveID registerMove(MoveRegistry &registry, const MoveMeta &meta)
	{
		const MoveID customMoveID{registry.getNextMoveID()};
		const us customMoveIndex{registry.getAmountRegistered()};

		registry.setEntry(customMoveIndex, MoveMeta{
											   .mHitCountPolicy = meta.mHitCountPolicy,
											   .mTriggers = meta.mTriggers,
											   .mName = meta.mName,
											   .mMoveID = customMoveID,
											   .mTypeID = meta.mTypeID,
											   .mPower = meta.mPower,
											   .mTargetID = meta.mTargetID,
											   .mRangeID = meta.mRangeID,
											   .mAccuracy = meta.mAccuracy,
											   .mPriority = meta.mPriority,
											   .mSpecial = meta.mSpecial,
										   });

		registry.incrementAmountRegistered();
		registry.incrementNextMoveID();

		return customMoveID;
	}

} // namespace PocketCore::Testing

#endif
