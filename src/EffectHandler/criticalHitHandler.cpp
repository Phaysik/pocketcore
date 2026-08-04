#include "EffectHandler/criticalHitHandler.h"

#include <cassert>

#include "Battle/battleState.h"
#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Effect/effectContext.h"
#include "EffectHandler/effectHandlerHelpers.h"
#include "Registry/registryProvider.h"
#include "Utility/random.h"

namespace PocketCore::Effect
{
	using PocketCore::Battle::BattleState;
	using PocketCore::Configuration::CRITICAL_HIT_MULTIPLIER_VALUE;
	using PocketCore::Configuration::CRITICAL_HIT_PERCENTAGE;
	using PocketCore::Configuration::MAX_CRITICAL_HIT_VALUE;
	using PocketCore::Configuration::MIN_CRITICAL_HIT_VALUE;
	using PocketCore::Core::ub;
	using PocketCore::Registry::RegistryProvider;
	using PocketCore::Utility::Random;

	void CriticalHitHandler::apply(ATTR_MAYBE_UNUSED BattleState &state, EffectContext &context,
								   ATTR_MAYBE_UNUSED const RegistryProvider &provider) const
	{
		if (Random::get<ub>(MIN_CRITICAL_HIT_VALUE, MAX_CRITICAL_HIT_VALUE) < CRITICAL_HIT_PERCENTAGE)
		{
			context.mDamage.mIsCritical = true;
			context.setMultiplier(toMultiplierID(BuiltinMultiplierID::Critical), CRITICAL_HIT_MULTIPLIER_VALUE);
		}
	}
} // namespace PocketCore::Effect
