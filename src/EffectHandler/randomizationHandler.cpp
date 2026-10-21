/*! @file randomizationHandler.cpp
	@brief Contains the randomization effect handler implementation
	@date 10/19/2026
	@since 0.9.6
	@version 0.12.51
	@author Matthew Moore
*/

#include "EffectHandler/randomizationHandler.h"

#include <cassert>
#include <string_view>

#include "Battle/battleRandomSources.h"
#include "Battle/battleState.h"
#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Effect/effectContext.h"
#include "EffectHandler/effectHandlerHelpers.h"
#include "EffectHandler/effectHandlerInterface.h"
#include "Multiplier/builtInMultiplierID.h"
#include "Registry/registryProvider.h"
#include "Utility/Debug/Logging/logger.h"

namespace PocketCore::Effect
{
	using PocketCore::Battle::BattleRandomSources;
	using PocketCore::Battle::BattleState;
	using PocketCore::Configuration::RANDOMIZATION_MULTIPLIER_MAX_VALUE;
	using PocketCore::Configuration::RANDOMIZATION_MULTIPLIER_MIN_VALUE;
	using PocketCore::Effect::EffectContext;
	using PocketCore::Multiplier::BuiltinMultiplierID;
	using PocketCore::Multiplier::toMultiplierID;
	using PocketCore::Registry::RegistryProvider;
	using PocketCore::Utility::Debug::Logging::Logger;

	void RandomizationHandler::apply(BattleState &state, EffectContext &context, ATTR_MAYBE_UNUSED const RegistryProvider &provider,
									 const BattleRandomSources &randomSources) const
	{
		const double randomizationMultiplier{
			static_cast<double>(randomSources.mUnsigned(RANDOMIZATION_MULTIPLIER_MIN_VALUE, RANDOMIZATION_MULTIPLIER_MAX_VALUE)) / 100,
		};

		const BattleSlot &user{IEffectHandler::getConstUserBattleSlot(state, context)};
		const BattleSlot &target{IEffectHandler::getConstTargetBattleSlot(state, context)};

		const std::string_view userName{user.mPokemon != nullptr ? user.mPokemon->getName() : "<empty>"};
		const std::string_view targetName{target.mPokemon != nullptr ? target.mPokemon->getName() : "<empty>"};

		(void) Logger::info("{0}[{1}] {2} attacks {3}[{4}] {5} with randomization multiplier {6}", context.mUserSide == Side::A ? "A" : "B",
							context.mUserIndex, userName, context.mTargetSide == Side::A ? "A" : "B", context.mTargetIndex, targetName,
							randomizationMultiplier);

		context.setMultiplier(toMultiplierID(BuiltinMultiplierID::Randomization), randomizationMultiplier);
	}
} // namespace PocketCore::Effect
