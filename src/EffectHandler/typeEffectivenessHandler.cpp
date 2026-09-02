/*! @file typeEffectivenessHandler.cpp
	@brief Contains the type effectiveness effect handler implementation
	@date 09/02/2026
	@since 0.9.2
	@version 0.12.17
	@author Matthew Moore
*/

#include "EffectHandler/typeEffectivenessHandler.h"

#include <cassert>
#include <optional>

#include "Battle/battleState.h"
#include "Core/typedefs.h"
#include "Effect/effectContext.h"
#include "EffectHandler/effectHandlerHelpers.h"
#include "EffectHandler/effectHandlerInterface.h"
#include "Multiplier/builtInMultiplierID.h"
#include "Pokemon/pokemon.h"
#include "Registry/registryProvider.h"
#include "Types/typeEffectiveness.h"
#include "Types/typeID.h"

namespace PocketCore::Effect
{
	using PocketCore::Battle::BattleSlot;
	using PocketCore::Battle::BattleState;
	using PocketCore::Core::us;
	using PocketCore::Multiplier::BuiltinMultiplierID;
	using PocketCore::Multiplier::toMultiplierID;
	using PocketCore::Pokemon::Pokemon;
	using PocketCore::Registry::RegistryProvider;
	using PocketCore::Type::getEffectivenessValue;
	using PocketCore::Type::NO_TYPE_ID;
	using PocketCore::Type::TypeID;

	void TypeEffectivenessHandler::apply(BattleState &state, EffectContext &context, const RegistryProvider &provider) const
	{
		const BattleSlot &user{IEffectHandler::getConstUserBattleSlot(state, context)};
		const BattleSlot &target{IEffectHandler::getConstTargetBattleSlot(state, context)};

		const Pokemon *userPokemon{user.mPokemon};
		const Pokemon *targetPokemon{target.mPokemon};

		if (userPokemon == nullptr || targetPokemon == nullptr || context.mMoveBasePower == 0)
		{
			return;
		}

		if (provider.typeRegistry == nullptr)
		{
			return;
		}

		const TypeID moveTypeID{context.mMoveTypeID};
		const std::optional<us> attackerIndex{provider.typeRegistry->findIndexByTypeID(moveTypeID)};

		if (!attackerIndex.has_value())
		{
			return;
		}

		double typeEffectivenessValue{1.0};

		for (const TypeID &type : targetPokemon->getTypeIDsArray())
		{
			if (type == NO_TYPE_ID)
			{
				continue;
			}

			const std::optional<us> defenderIndex{provider.typeRegistry->findIndexByTypeID(type)};

			if (!defenderIndex.has_value())
			{
				continue;
			}

			const Type::TypeEffectiveness effectiveness{
				provider.typeRegistry->getTypeChartCell(attackerIndex.value(), defenderIndex.value()),
			};

			if (effectiveness == Type::TypeEffectiveness::NOT_DEFINED)
			{
				continue;
			}

			typeEffectivenessValue *= getEffectivenessValue(effectiveness);
		}

		context.setMultiplier(toMultiplierID(BuiltinMultiplierID::TypeEffectiveness), typeEffectivenessValue);
	}
} // namespace PocketCore::Effect
