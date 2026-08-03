#include "EffectHandler/typeEffectivenessHandler.h"

#include <cassert>
#include <optional>

#include "Battle/battleState.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Effect/effectContext.h"
#include "EffectHandler/effectHandlerHelpers.h"
#include "EffectHandler/effectHandlerInterface.h"
#include "Multiplier/builtInMultiplierID.h"
#include "Pokemon/pokemon.h"
#include "Registry/registryProvider.h"
#include "Types/typeEffectivenessID.h"
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
	using PocketCore::Types::NO_TYPE_ID;
	using PocketCore::Types::NO_TYPE_EFFECTIVENESS_ID;
	using PocketCore::Types::TypeEffectivenessID;
	using PocketCore::Types::TypeID;

	void TypeEffectivenessHandler::apply(const BattleState &state, EffectContext &context,
										 ATTR_MAYBE_UNUSED const RegistryProvider &provider) const
	{
		const BattleSlot &user{IEffectHandler::getUserBattleSlot(state, context)};
		const BattleSlot &target{IEffectHandler::getTargetBattleSlot(state, context)};

		const Pokemon *userPokemon{user.mPokemon};
		const Pokemon *targetPokemon{target.mPokemon};

		if (userPokemon == nullptr || targetPokemon == nullptr || context.mMoveBasePower == 0)
		{
			return;
		}

		if (provider.typeRegistry == nullptr || provider.typeEffectivenessRegistry == nullptr)
		{
			return;
		}

		const TypeID moveTypeID{context.mMoveTypeID};
		const std::optional<us> attackerIndex{provider.typeRegistry->findIndexByTypeID(moveTypeID)};

		if (!attackerIndex.has_value())
		{
			return;
		}

		float typeEffectivenessValue{1.0F};

		for (const TypeID &type : targetPokemon->getTypesArray())
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

			const TypeEffectivenessID effectivenessID{
				provider.typeRegistry->getTypeChartCell(attackerIndex.value(), defenderIndex.value()),
			};

			if (effectivenessID == NO_TYPE_EFFECTIVENESS_ID)
			{
				continue;
			}

			const auto *effectivenessMetadata{provider.typeEffectivenessRegistry->getTypeEffectivenessMetadata(effectivenessID)};

			if (effectivenessMetadata == nullptr)
			{
				continue;
			}

			typeEffectivenessValue *= effectivenessMetadata->mTypeEffectivenessValue;
		}

		context.setMultiplier(toMultiplierID(BuiltinMultiplierID::TypeEffectiveness), typeEffectivenessValue);
	}
} // namespace PocketCore::Effect
