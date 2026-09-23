/*! @file setHarshSunlightHandler.cpp
	@brief Contains the set harsh sunlight effect handler implementation
	@date 09/23/2026
	@since 0.10.0
	@version 0.12.43
	@author Matthew Moore
*/

#include "EffectHandler/setHarshSunlightHandler.h"

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

	void SetHarshSunlightHandler::apply(BattleState &state, ATTR_MAYBE_UNUSED EffectContext &context, const RegistryProvider &provider) const
	{
		applyInteractions(toWeatherID(BuiltinWeatherID::HarshSunlight), NO_WEATHER_ID, *provider.weatherRegistry, state.mWeatherIDs,
						  &WeatherMeta::mWeatherInteractions, state.mRuleset.mMaxWeathers, state.mRuleset.mReplaceWeatherWhenFull);
	}
} // namespace PocketCore::Effect
