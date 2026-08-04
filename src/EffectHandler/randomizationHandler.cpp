#include "EffectHandler/randomizationHandler.h"

#include <cassert>

#include "Battle/battleState.h"
#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Effect/effectContext.h"
#include "EffectHandler/effectHandlerHelpers.h"
#include "Multiplier/builtInMultiplierID.h"
#include "Registry/registryProvider.h"
#include "Utility/random.h"

namespace PocketCore::Effect
{
	using PocketCore::Battle::BattleState;
	using PocketCore::Configuration::RANDOMIZATION_MULTIPLIER_MAX_VALUE;
	using PocketCore::Configuration::RANDOMIZATION_MULTIPLIER_MIN_VALUE;
	using PocketCore::Effect::EffectContext;
	using PocketCore::Multiplier::BuiltinMultiplierID;
	using PocketCore::Multiplier::toMultiplierID;
	using PocketCore::Registry::RegistryProvider;
	using Utility::Random;

	void RandomizationHandler::apply(ATTR_MAYBE_UNUSED const BattleState &state, EffectContext &context,
									 ATTR_MAYBE_UNUSED const RegistryProvider &provider) const
	{
		const double randomizationMultiplier{
			static_cast<double>(Random::get<ub>(RANDOMIZATION_MULTIPLIER_MIN_VALUE, RANDOMIZATION_MULTIPLIER_MAX_VALUE)) / 100};

		context.setMultiplier(toMultiplierID(BuiltinMultiplierID::Randomization), randomizationMultiplier);
	}
} // namespace PocketCore::Effect
