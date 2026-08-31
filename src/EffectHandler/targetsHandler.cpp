/*! @file targetsHandler.cpp
	@brief Contains the targets effect handler implementation
	@date 08/26/2026
	@since 0.8.1
	@version 0.12.7
	@author Matthew Moore
*/

#include "EffectHandler/targetsHandler.h"

#include <cassert>

#include "Battle/battleState.h"
#include "Battle/battleTargetsAndTriggers.h"
#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Effect/effectContext.h"
#include "EffectHandler/effectHandlerHelpers.h"
#include "Move/moveMeta.h"
#include "Multiplier/builtInMultiplierID.h"
#include "Registry/registryProvider.h"

namespace PocketCore::Effect
{
	using PocketCore::Battle::BattleState;
	using PocketCore::Battle::BattleTargetID;
	using PocketCore::Configuration::TARGETS_HIT_MULTIPLIER_VALUE;
	using PocketCore::Effect::EffectContext;
	using PocketCore::Move::MoveMeta;
	using PocketCore::Multiplier::BuiltinMultiplierID;
	using PocketCore::Multiplier::toMultiplierID;
	using PocketCore::Registry::RegistryProvider;

	void TargetsHandler::apply(ATTR_MAYBE_UNUSED BattleState &state, EffectContext &context, const RegistryProvider &provider) const
	{
		const MoveMeta *moveMeta{provider.moveRegistry->getMoveMetadata(context.mMoveID)};

		if (moveMeta == nullptr)
		{
			return;
		}

		const BattleTargetID moveTarget{moveMeta->mTargetID};

		if (moveTarget != BattleTargetID::SingleOpponent && moveTarget != BattleTargetID::Self)
		{
			context.setMultiplier(toMultiplierID(BuiltinMultiplierID::Targets), TARGETS_HIT_MULTIPLIER_VALUE);
		}
	}
} // namespace PocketCore::Effect
