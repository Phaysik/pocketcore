/*! @file setSandstormHandler.cpp
	@brief Contains the set sandstorm effect handler implementation
	@date 10/08/2026
	@since 0.10.0
	@version 0.12.50
	@author Matthew Moore
*/

#include "EffectHandler/setSandstormHandler.h"

#include <cassert>
#include <expected>
#include <optional>

#include "Battle/battleState.h"
#include "Core/attributeMacros.h"
#include "Effect/effectContext.h"
#include "EffectHandler/effectHandlerHelpers.h"
#include "EffectHandler/effectHandlerInterface.h"
#include "Interaction/interactionApplicationError.h"
#include "Interaction/interactionHelpers.h"
#include "Registry/registryProvider.h"
#include "Weather/builtInWeatherID.h"
#include "Weather/weatherID.h"
#include "Weather/weatherMeta.h"

namespace PocketCore::Effect
{
	using PocketCore::Battle::BattleState;
	using PocketCore::Interaction::applyInteractions;
	using PocketCore::Interaction::InteractionApplicationError;
	using PocketCore::Registry::RegistryProvider;
	using PocketCore::Weather::BuiltinWeatherID;
	using PocketCore::Weather::NO_WEATHER_ID;
	using PocketCore::Weather::toWeatherID;
	using PocketCore::Weather::WeatherMeta;

	void SetSandstormHandler::apply(BattleState &state, ATTR_MAYBE_UNUSED EffectContext &context, const RegistryProvider &provider) const
	{
		const std::expected<void, InteractionApplicationError> result{
			applyInteractions(toWeatherID(BuiltinWeatherID::Sandstorm), NO_WEATHER_ID, *provider.mWeatherRegistry, state.mWeatherIDs,
							  &WeatherMeta::mWeatherInteractions, state.mRuleset.mMaxWeathers, state.mRuleset.mReplaceWeatherWhenFull),
		};

		context.mInteractionOutcome = {
			.mError = result ? std::nullopt : std::optional{result.error()},
			.mApplied = result.has_value(),
		};
	}
} // namespace PocketCore::Effect
