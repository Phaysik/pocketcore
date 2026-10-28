/*! @file weatherHandler.cpp
	@brief Contains the weather effect handler implementation
	@date 10/28/2026
	@since 0.9.8
	@version 0.12.53
	@author Matthew Moore
*/

#include "EffectHandler/weatherHandler.h"

#include <algorithm>
#include <cassert>
#include <vector>

#include "Ability/builtInAbilityID.h"
#include "Battle/battleRandomSources.h"
#include "Battle/battleState.h"
#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Effect/effectContext.h"
#include "EffectHandler/effectHandlerHelpers.h"
#include "EffectHandler/effectHandlerInterface.h"
#include "Multiplier/builtInMultiplierID.h"
#include "Pokemon/pokemon.h"
#include "Pokemon/pokemonMeta.h"
#include "Registry/registryProvider.h"

namespace PocketCore::Effect
{
	using PocketCore::Ability::BuiltinAbilityID;
	using PocketCore::Ability::toAbilityID;
	using PocketCore::Battle::BattleRandomSources;
	using PocketCore::Battle::BattleState;
	using PocketCore::Configuration::WEATHER_NULLIFICATION_MULTIPLIER_VALUE;
	using PocketCore::Multiplier::BuiltinMultiplierID;
	using PocketCore::Multiplier::toMultiplierID;
	using PocketCore::Pokemon::Pokemon;
	using PocketCore::Pokemon::PokemonAbilityEntry;
	using PocketCore::Registry::RegistryProvider;

	void WeatherHandler::apply(BattleState &state, EffectContext &context, ATTR_MAYBE_UNUSED const RegistryProvider &provider,
							   ATTR_MAYBE_UNUSED const BattleRandomSources &randomSources) const
	{
		const std::vector<Pokemon *> partyA{state.mPartyA};

		constexpr auto hasWeatherNullifier{
			[](const Pokemon *pokemon) {
				return pokemon != nullptr
					&& std::ranges::any_of(pokemon->getAbilityEntriesArray(), [](const PokemonAbilityEntry &abilityEntry) {
						   return abilityEntry.mAbilityID == toAbilityID(BuiltinAbilityID::AirLock)
							   || abilityEntry.mAbilityID == toAbilityID(BuiltinAbilityID::CloudNine);
					   });
			},
		};

		for (const Pokemon *pokemon : partyA)
		{
			if (hasWeatherNullifier(pokemon))
			{
				context.setMultiplier(toMultiplierID(BuiltinMultiplierID::Weather), WEATHER_NULLIFICATION_MULTIPLIER_VALUE);
				return;
			}
		}

		const std::vector<Pokemon *> partyB{state.mPartyB};

		for (const Pokemon *pokemon : partyB)
		{
			if (hasWeatherNullifier(pokemon))
			{
				context.setMultiplier(toMultiplierID(BuiltinMultiplierID::Weather), WEATHER_NULLIFICATION_MULTIPLIER_VALUE);
				return;
			}
		}

		applyMultipliers(state, context);
	}
} // namespace PocketCore::Effect
