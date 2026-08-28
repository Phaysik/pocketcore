/*! @file populationBombHandler.cpp
	@brief Contains the population bomb effect handler implementation
	@date 08/04/2026
	@since 0.9.8
	@version 0.9.10
	@author Matthew Moore
*/

#include "EffectHandler/populationBombHandler.h"

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
	using PocketCore::Battle::BattleState;
	using PocketCore::Configuration::POPULATION_BOMB_CONSECUTIVE_HIT_MULTIPLIER_VALUE;
	using PocketCore::Configuration::POPULATION_BOMB_FIRST_HIT_MULTIPLIER_VALUE;
	using PocketCore::Multiplier::BuiltinMultiplierID;
	using PocketCore::Multiplier::toMultiplierID;
	using PocketCore::Registry::RegistryProvider;

	void PopulationBombHandler::apply(ATTR_MAYBE_UNUSED BattleState &state, EffectContext &context,
									  ATTR_MAYBE_UNUSED const RegistryProvider &provider) const
	{
		if (context.mHitAttemptIndex > 1)
		{
			context.setMultiplier(toMultiplierID(BuiltinMultiplierID::PopulationBomb), POPULATION_BOMB_CONSECUTIVE_HIT_MULTIPLIER_VALUE);
		}
		else if (context.mHitAttemptIndex == 1)
		{
			context.setMultiplier(toMultiplierID(BuiltinMultiplierID::PopulationBomb), POPULATION_BOMB_FIRST_HIT_MULTIPLIER_VALUE);
		}
	}
} // namespace PocketCore::Effect
