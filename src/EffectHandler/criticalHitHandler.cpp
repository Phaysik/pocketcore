/*! @file criticalHitHandler.cpp
	@brief Contains the critical hit effect handler implementation
	@date 10/19/2026
	@since 0.7.2
	@version 0.12.51
	@author Matthew Moore
*/

#include "EffectHandler/criticalHitHandler.h"

#include <cassert>

#include "Battle/battleRandomSources.h"
#include "Battle/battleState.h"
#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Effect/effectContext.h"
#include "EffectHandler/effectHandlerHelpers.h"
#include "Registry/registryProvider.h"

namespace PocketCore::Effect
{
	using PocketCore::Battle::BattleRandomSources;
	using PocketCore::Battle::BattleState;
	using PocketCore::Configuration::CRITICAL_HIT_MULTIPLIER_VALUE;
	using PocketCore::Configuration::CRITICAL_HIT_PERCENTAGE;
	using PocketCore::Configuration::MAX_CRITICAL_HIT_VALUE;
	using PocketCore::Configuration::MIN_CRITICAL_HIT_VALUE;
	using PocketCore::Core::ub;
	using PocketCore::Registry::RegistryProvider;

	void CriticalHitHandler::apply(ATTR_MAYBE_UNUSED BattleState &state, EffectContext &context,
								   ATTR_MAYBE_UNUSED const RegistryProvider &provider, const BattleRandomSources &randomSources) const
	{
		if (randomSources.mUnsigned(MIN_CRITICAL_HIT_VALUE, MAX_CRITICAL_HIT_VALUE) < CRITICAL_HIT_PERCENTAGE)
		{
			context.mDamage.mIsCritical = true;
			context.setMultiplier(toMultiplierID(BuiltinMultiplierID::Critical), CRITICAL_HIT_MULTIPLIER_VALUE);
		}
	}
} // namespace PocketCore::Effect
