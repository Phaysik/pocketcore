#include "EffectHandler/stabHandler.h"

#include <algorithm>
#include <cassert>

#include "Battle/battleState.h"
#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Effect/effectContext.h"
#include "EffectHandler/effectHandlerHelpers.h"
#include "EffectHandler/effectHandlerInterface.h"
#include "Multiplier/builtinMultiplierID.h"
#include "Registry/registryProvider.h"

namespace PocketCore::Effect
{
	using PocketCore::Battle::BattleSlot;
	using PocketCore::Battle::BattleState;
	using PocketCore::Configuration::STAB_HIT_MULTIPLIER;
	using PocketCore::Multiplier::BuiltinMultiplierID;
	using PocketCore::Multiplier::toMultiplierID;
	using PocketCore::Registry::RegistryProvider;

	void StabHandler::apply(const BattleState &state, EffectContext &context, ATTR_MAYBE_UNUSED const RegistryProvider &provider) const
	{
		const BattleSlot &user{IEffectHandler::getUserBattleSlot(state, context)};

		if (std::ranges::contains(user.mPokemon->getTypesArray(), context.mMoveTypeID))
		{
			context.setMultiplier(toMultiplierID(BuiltinMultiplierID::Stab), STAB_HIT_MULTIPLIER);
		}
	}
} // namespace PocketCore::Effect
