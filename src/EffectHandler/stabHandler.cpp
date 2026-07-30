#include "EffectHandler/stabHandler.h"

#include <algorithm>
#include <cassert>
#include <vector>

#include "Battle/battleState.h"
#include "Configuration/constants.h"
#include "Effect/effectContext.h"
#include "EffectHandler/effectHandlerHelpers.h"
#include "Multiplier/builtinMultiplierID.h"

namespace PocketCore::Effect
{
	using PocketCore::Battle::BattleSlot;
	using PocketCore::Battle::BattleState;
	using PocketCore::Configuration::STAB_HIT_MULTIPLIER;
	using PocketCore::Multiplier::BuiltinMultiplierID;
	using PocketCore::Multiplier::toMultiplierID;

	void StabHandler::apply(const BattleState &state, EffectContext &context) const
	{
		const std::vector<BattleSlot> &userTeam{getTeamConst(state, context.mUserSide)};

		assert(context.mUserIndex < userTeam.size());

		const BattleSlot &user{userTeam.at(context.mUserIndex)};

		if (std::ranges::contains(user.mPokemon->getTypesArray(), context.mMoveTypeID))
		{
			context.mMultiplierIDs[toMultiplierID(BuiltinMultiplierID::Stab)] = STAB_HIT_MULTIPLIER;
		}
	}
} // namespace PocketCore::Effect
