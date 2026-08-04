#include "EffectHandler/burnDamageHandler.h"

#include <cassert>

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
#include "Status/builtInStatusID.h"
#include "Status/statusHelpers.h"

namespace PocketCore::Effect
{
	using PocketCore::Ability::BuiltinAbilityID;
	using PocketCore::Ability::toAbilityID;
	using PocketCore::Battle::BattleSlot;
	using PocketCore::Battle::BattleState;
	using PocketCore::Configuration::BURN_MULTIPLIER_VALUE;
	using PocketCore::Move::BuiltinMoveID;
	using PocketCore::Move::toMoveID;
	using PocketCore::Multiplier::BuiltinMultiplierID;
	using PocketCore::Multiplier::toMultiplierID;
	using PocketCore::Pokemon::Pokemon;
	using PocketCore::Registry::RegistryProvider;
	using PocketCore::Status::BuiltinStatusID;
	using PocketCore::Status::toStatusID;

	void BurnDamageHandler::apply(BattleState &state, EffectContext &context, ATTR_MAYBE_UNUSED const RegistryProvider &provider) const
	{
		const BattleSlot &user{IEffectHandler::getConstUserBattleSlot(state, context)};

		const Pokemon *userPokemon{user.mPokemon};

		if (userPokemon == nullptr)
		{
			return;
		}

		const bool isBurned{Status::statusAlreadyExists(toStatusID(BuiltinStatusID::Burn), userPokemon->getStatusesArray())};
		const bool abilityIsntGuts{userPokemon->getAbilityID() != toAbilityID(BuiltinAbilityID::Guts)};
		const bool moveIsntFacade{context.mMoveID != toMoveID(BuiltinMoveID::Facade)};

		if (isBurned && !context.mIsSpecial && abilityIsntGuts && moveIsntFacade)
		{
			context.setMultiplier(toMultiplierID(BuiltinMultiplierID::Burn), BURN_MULTIPLIER_VALUE);
		}
	}
} // namespace PocketCore::Effect
