/*! @file stabHandler.cpp
	@brief Contains the stab effect handler implementation
	@date 09/02/2026
	@since 0.8.1
	@version 0.12.17
	@author Matthew Moore
*/

#include "EffectHandler/stabHandler.h"

#include <algorithm>
#include <cassert>

#include "Battle/battleState.h"
#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Effect/effectContext.h"
#include "EffectHandler/effectHandlerHelpers.h"
#include "EffectHandler/effectHandlerInterface.h"
#include "Multiplier/builtInMultiplierID.h"
#include "Registry/registryProvider.h"

namespace PocketCore::Effect
{
	using PocketCore::Battle::BattleSlot;
	using PocketCore::Battle::BattleState;
	using PocketCore::Configuration::STAB_HIT_MULTIPLIER_VALUE;
	using PocketCore::Multiplier::BuiltinMultiplierID;
	using PocketCore::Multiplier::toMultiplierID;
	using PocketCore::Registry::RegistryProvider;

	void StabHandler::apply(BattleState &state, EffectContext &context, ATTR_MAYBE_UNUSED const RegistryProvider &provider) const
	{
		const BattleSlot &user{IEffectHandler::getConstUserBattleSlot(state, context)};

		if (std::ranges::contains(user.mPokemon->getTypeIDsArray(), context.mMoveTypeID))
		{
			context.setMultiplier(toMultiplierID(BuiltinMultiplierID::Stab), STAB_HIT_MULTIPLIER_VALUE);
		}
	}
} // namespace PocketCore::Effect
