#include "EffectHandler/targetsHandler.h"

#include <cassert>

#include "Battle/battleState.h"
#include "Configuration/constants.h"
#include "Configuration/moveRegistryConfiguration.h"
#include "Core/attributeMacros.h"
#include "Effect/effectContext.h"
#include "EffectHandler/effectHandlerHelpers.h"
#include "Move/moveMeta.h"
#include "Move/moveTargetsAndTriggers.h"
#include "Multiplier/builtinMultiplierID.h"

namespace PocketCore::Effect
{
	using PocketCore::Battle::BattleState;
	using PocketCore::Configuration::TARGETS_HIT_MULTIPLIER;
	using PocketCore::Effect::EffectContext;
	using PocketCore::Move::MoveMeta;
	using PocketCore::Move::MoveTargetID;
	using PocketCore::Multiplier::BuiltinMultiplierID;
	using PocketCore::Multiplier::toMultiplierID;

	void TargetsHandler::apply(ATTR_MAYBE_UNUSED const BattleState &state, EffectContext &context) const
	{
		const MoveMeta *moveMeta{mMoveRegistry.getMoveMetadata(context.mMoveID)};
		assert(moveMeta != nullptr);

		const MoveTargetID moveTarget{moveMeta->mTargetID};

		if (moveTarget != MoveTargetID::SingleOpponent && moveTarget != MoveTargetID::Self)
		{
			context.mMultiplierIDs[toMultiplierID(BuiltinMultiplierID::Targets)] = TARGETS_HIT_MULTIPLIER;
		}
	}
} // namespace PocketCore::Effect
