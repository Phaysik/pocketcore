/*! @file burnDamageHandler.cpp
	@brief Contains the burn damage effect handler implementation
	@date 10/27/2026
	@since 0.9.5
	@version 0.12.53
	@author Matthew Moore
*/

#include "EffectHandler/burnDamageHandler.h"

#include <algorithm>
#include <cassert>

#include "Ability/builtInAbilityID.h"
#include "Battle/battleRandomSources.h"
#include "Battle/battleState.h"
#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Effect/effectContext.h"
#include "EffectHandler/effectHandlerHelpers.h"
#include "EffectHandler/effectHandlerInterface.h"
#include "Move/builtInMoveID.h"
#include "Multiplier/builtInMultiplierID.h"
#include "Pokemon/pokemon.h"
#include "Pokemon/pokemonMeta.h"
#include "Registry/registryProvider.h"
#include "Status/builtInStatusID.h"

namespace PocketCore::Effect
{
	using PocketCore::Ability::BuiltinAbilityID;
	using PocketCore::Ability::toAbilityID;
	using PocketCore::Battle::BattleRandomSources;
	using PocketCore::Battle::BattleSlot;
	using PocketCore::Battle::BattleState;
	using PocketCore::Configuration::BURN_MULTIPLIER_VALUE;
	using PocketCore::Move::BuiltinMoveID;
	using PocketCore::Move::toMoveID;
	using PocketCore::Multiplier::BuiltinMultiplierID;
	using PocketCore::Multiplier::toMultiplierID;
	using PocketCore::Pokemon::Pokemon;
	using PocketCore::Pokemon::PokemonAbilityEntry;
	using PocketCore::Registry::RegistryProvider;
	using PocketCore::Status::BuiltinStatusID;
	using PocketCore::Status::toStatusID;

	void BurnDamageHandler::apply(BattleState &state, EffectContext &context, ATTR_MAYBE_UNUSED const RegistryProvider &provider,
								  ATTR_MAYBE_UNUSED const BattleRandomSources &randomSources) const
	{
		const BattleSlot &user{IEffectHandler::getConstUserBattleSlot(state, context)};

		const Pokemon *userPokemon{user.mPokemon};

		if (userPokemon == nullptr)
		{
			return;
		}

		const bool isBurned{std::ranges::contains(userPokemon->getStatusIDsArray(), toStatusID(BuiltinStatusID::Burn))};
		const bool abilityIsntGuts{
			!std::ranges::contains(userPokemon->getAbilityEntriesArray(), toAbilityID(BuiltinAbilityID::Guts),
								   &PokemonAbilityEntry::mAbilityID),
		};
		const bool moveIsntFacade{context.mMoveID != toMoveID(BuiltinMoveID::Facade)};

		if (isBurned && !context.mIsSpecial && abilityIsntGuts && moveIsntFacade)
		{
			context.setMultiplier(toMultiplierID(BuiltinMultiplierID::Burn), BURN_MULTIPLIER_VALUE);
		}
	}
} // namespace PocketCore::Effect
