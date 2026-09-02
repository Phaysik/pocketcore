/*! @file weatherHandler.cpp
	@brief Contains the weather effect handler implementation
	@date 09/02/2026
	@since 0.9.8
	@version 0.12.16
	@author Matthew Moore
*/

#include "EffectHandler/weatherHandler.h"

#include <algorithm>
#include <cassert>
#include <vector>

#include "Ability/builtInAbilityID.h"
#include "Battle/battleState.h"
#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Effect/effectContext.h"
#include "EffectHandler/effectHandlerHelpers.h"
#include "EffectHandler/effectHandlerInterface.h"
#include "Move/builtInMoveID.h"
#include "Multiplier/builtInMultiplierID.h"
#include "Pokemon/pokemon.h"
#include "Registry/registryProvider.h"
#include "Types/builtInTypeID.h"
#include "Weather/builtInWeatherID.h"

namespace PocketCore::Effect
{
	using PocketCore::Ability::BuiltinAbilityID;
	using PocketCore::Ability::toAbilityID;
	using PocketCore::Battle::BattleState;
	using PocketCore::Configuration::FIRE_MOVE_IN_HARSH_SUNLIGHT_MULTIPLIER_VALUE;
	using PocketCore::Configuration::FIRE_MOVE_IN_RAIN_MULTIPLIER_VALUE;
	using PocketCore::Configuration::WATER_MOVE_IN_HARSH_SUNLIGHT_MULTIPLIER_VALUE;
	using PocketCore::Configuration::WATER_MOVE_IN_RAIN_MULTIPLIER_VALUE;
	using PocketCore::Configuration::WEATHER_NULLIFICATION_MULTIPLIER_VALUE;
	using PocketCore::Move::BuiltinMoveID;
	using PocketCore::Move::toMoveID;
	using PocketCore::Multiplier::BuiltinMultiplierID;
	using PocketCore::Multiplier::toMultiplierID;
	using PocketCore::Pokemon::Pokemon;
	using PocketCore::Registry::RegistryProvider;
	using PocketCore::Type::BuiltInTypeID;
	using PocketCore::Type::toTypeID;
	using PocketCore::Weather::BuiltinWeatherID;
	using PocketCore::Weather::toWeatherID;

	void WeatherHandler::apply(BattleState &state, EffectContext &context, ATTR_MAYBE_UNUSED const RegistryProvider &provider) const
	{
		const std::vector<Pokemon *> partyA{state.mPartyA};

		for (const Pokemon *pokemon : partyA)
		{
			if (pokemon != nullptr && std::ranges::any_of(pokemon->getAbilitiesArray(), [](const AbilityID ability) {
					return ability == toAbilityID(BuiltinAbilityID::AirLock) || ability == toAbilityID(BuiltinAbilityID::CloudNine);
				}))
			{
				context.setMultiplier(toMultiplierID(BuiltinMultiplierID::Weather), WEATHER_NULLIFICATION_MULTIPLIER_VALUE);
				return;
			}
		}

		const std::vector<Pokemon *> partyB{state.mPartyB};

		for (const Pokemon *pokemon : partyB)
		{

			if (pokemon != nullptr && std::ranges::any_of(pokemon->getAbilitiesArray(), [](const AbilityID ability) {
					return ability == toAbilityID(BuiltinAbilityID::AirLock) || ability == toAbilityID(BuiltinAbilityID::CloudNine);
				}))
			{
				context.setMultiplier(toMultiplierID(BuiltinMultiplierID::Weather), WEATHER_NULLIFICATION_MULTIPLIER_VALUE);
				return;
			}
		}

		const bool isRainy{std::ranges::contains(state.mWeatherIDs, toWeatherID(BuiltinWeatherID::Rain))};
		const bool isHarshSunlight{std::ranges::contains(state.mWeatherIDs, toWeatherID(BuiltinWeatherID::HarshSunlight))};

		const bool isWaterTypeMove{context.mMoveTypeID == toTypeID(BuiltInTypeID::Water)};
		const bool isFireTypeMove{context.mMoveTypeID == toTypeID(BuiltInTypeID::Fire)};

		const bool isHydroSteamMove{context.mMoveID == toMoveID(BuiltinMoveID::HydroSteam)};

		if (isRainy)
		{
			if (isWaterTypeMove)
			{
				context.setMultiplier(toMultiplierID(BuiltinMultiplierID::Weather), WATER_MOVE_IN_RAIN_MULTIPLIER_VALUE);
			}
			else if (isFireTypeMove)
			{
				context.setMultiplier(toMultiplierID(BuiltinMultiplierID::Weather), FIRE_MOVE_IN_RAIN_MULTIPLIER_VALUE);
			}
		}
		else if (isHarshSunlight)
		{
			if (isFireTypeMove || isHydroSteamMove)
			{
				context.setMultiplier(toMultiplierID(BuiltinMultiplierID::Weather), FIRE_MOVE_IN_HARSH_SUNLIGHT_MULTIPLIER_VALUE);
			}
			else if (isWaterTypeMove)
			{
				context.setMultiplier(toMultiplierID(BuiltinMultiplierID::Weather), WATER_MOVE_IN_HARSH_SUNLIGHT_MULTIPLIER_VALUE);
			}
		}
	}
} // namespace PocketCore::Effect
