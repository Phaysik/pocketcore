/*! @file moveRegistry.testHelper.h
	@brief Test helper for dealing with MoveRegistry concepts.
	@date 09/03/2026
	@since 0.12.14
	@version 0.12.18
	@author Matthew Moore
*/

#ifndef TEST_INCLUDE_REGISTRY_MOVE_REGISTRY_TEST_HELPER_H
#define TEST_INCLUDE_REGISTRY_MOVE_REGISTRY_TEST_HELPER_H

#include "Battle/battleTargetsAndTriggers.h"
#include "Configuration/moveRegistryConfiguration.h"
#include "Core/typedefs.h"
#include "Effect/effectTrigger.h"
#include "Move/moveHitPolicy.h"
#include "Move/moveID.h"
#include "Move/moveMeta.h"
#include "Types/typeID.h"

namespace PocketCore::Testing
{
	using PocketCore::Battle::BattleRangeID;
	using PocketCore::Battle::BattleTargetID;
	using PocketCore::Configuration::MoveRegistryConfiguration;
	using PocketCore::Core::sb;
	using PocketCore::Core::ub;
	using PocketCore::Core::us;
	using PocketCore::Effect::EffectTrigger;
	using PocketCore::Move::FixedHitCount;
	using PocketCore::Move::HitCountPolicy;
	using PocketCore::Move::MoveID;
	using PocketCore::Move::MoveMeta;
	using PocketCore::Type::TypeID;

	inline MoveID registerMove(MoveRegistryConfiguration &configuration, const MoveMeta &meta)
	{
		return configuration.addMove(meta).value();
	}

} // namespace PocketCore::Testing

#endif
