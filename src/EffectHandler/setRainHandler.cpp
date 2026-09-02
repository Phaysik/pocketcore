/*! @file setRainHandler.cpp
	@brief Contains the set rain effect handler implementation
	@date 09/02/2026
	@since 0.10.0
	@version 0.12.16
	@author Matthew Moore
*/

#include "EffectHandler/setRainHandler.h"

#include <cassert>

#include "Battle/battleState.h"
#include "Core/attributeMacros.h"
#include "Effect/effectContext.h"
#include "EffectHandler/effectHandlerHelpers.h"
#include "EffectHandler/effectHandlerInterface.h"
#include "Interaction/interactionHelpers.h"
#include "Registry/registryProvider.h"
#include "Weather/builtInWeatherID.h"
#include "Weather/weatherID.h"
#include "Weather/weatherMeta.h"

namespace PocketCore::Effect
{
	using PocketCore::Battle::BattleState;
	using PocketCore::Interaction::applyInteractions;
	using PocketCore::Registry::RegistryProvider;
	using PocketCore::Weather::BuiltinWeatherID;
	using PocketCore::Weather::NO_WEATHER_ID;
	using PocketCore::Weather::toWeatherID;
	using PocketCore::Weather::WeatherMeta;

	void SetRainHandler::apply(BattleState &state, ATTR_MAYBE_UNUSED EffectContext &context, const RegistryProvider &provider) const
	{
		applyInteractions(toWeatherID(BuiltinWeatherID::Rain), NO_WEATHER_ID, *provider.weatherRegistry, state.mWeatherIDs,
						  &WeatherMeta::mWeatherInteractions);
	}
} // namespace PocketCore::Effect
