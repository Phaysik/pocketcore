/*! @file terrainHandler.cpp
	@brief Contains the terrain effect handler implementation
	@date 08/04/2026
	@since 0.9.10
	@version 0.9.10
	@author Matthew Moore
*/

#include "EffectHandler/terrainHandler.h"

#include <cassert>

#include "Battle/battleState.h"
#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Effect/effectContext.h"
#include "EffectHandler/effectHandlerHelpers.h"
#include "EffectHandler/effectHandlerInterface.h"
#include "Registry/registryProvider.h"
#include "Terrain/builtInTerrainID.h"
#include "Types/builtInTypeID.h"

namespace PocketCore::Effect
{
	using PocketCore::Battle::BattleSlot;
	using PocketCore::Battle::BattleState;
	using PocketCore::Configuration::DRAGON_DEBUFF_IN_TERRAIN_BASE_DAMAGE_VALUE;
	using PocketCore::Configuration::ELECTRIC_BUFF_IN_TERRAIN_BASE_DAMAGE_VALUE;
	using PocketCore::Configuration::GRASS_BUFF_IN_TERRAIN_BASE_DAMAGE_VALUE;
	using PocketCore::Configuration::PSYCHIC_BUFF_IN_TERRAIN_BASE_DAMAGE_VALUE;
	using PocketCore::Registry::RegistryProvider;
	using PocketCore::Terrain::BuiltinTerrainID;
	using PocketCore::Terrain::toTerrainID;
	using PocketCore::Type::BuiltInTypeID;
	using PocketCore::Type::toTypeID;

	void TerrainHandler::apply(BattleState &state, EffectContext &context, ATTR_MAYBE_UNUSED const RegistryProvider &provider) const
	{
		BattleSlot &user{IEffectHandler::getUserBattleSlot(state, context)};
		const BattleSlot &target{IEffectHandler::getConstTargetBattleSlot(state, context)};

		if (isBattleSlotUngrounded(target))
		{
			return;
		}

		const bool isElectricTerrain{state.mTerrainID == toTerrainID(BuiltinTerrainID::Electric)};
		const bool isGrassyTerrain{state.mTerrainID == toTerrainID(BuiltinTerrainID::Grass)};
		const bool isPsychicTerrain{state.mTerrainID == toTerrainID(BuiltinTerrainID::Psychic)};
		const bool isMistyTerrain{state.mTerrainID == toTerrainID(BuiltinTerrainID::Misty)};

		const bool isElectricMove{context.mMoveTypeID == toTypeID(BuiltInTypeID::Electric)};
		const bool isGrassMove{context.mMoveTypeID == toTypeID(BuiltInTypeID::Grass)};
		const bool isPsychicMove{context.mMoveTypeID == toTypeID(BuiltInTypeID::Psychic)};
		const bool isDragonMove{context.mMoveTypeID == toTypeID(BuiltInTypeID::Dragon)};

		if (isElectricTerrain && isElectricMove)
		{
			user.mDamageFormulaModifiers.mAttackModifier *= ELECTRIC_BUFF_IN_TERRAIN_BASE_DAMAGE_VALUE;
		}
		else if (isGrassyTerrain && isGrassMove)
		{
			user.mDamageFormulaModifiers.mAttackModifier *= GRASS_BUFF_IN_TERRAIN_BASE_DAMAGE_VALUE;
		}
		else if (isPsychicTerrain && isPsychicMove)
		{
			user.mDamageFormulaModifiers.mAttackModifier *= PSYCHIC_BUFF_IN_TERRAIN_BASE_DAMAGE_VALUE;
		}
		else if (isMistyTerrain && isDragonMove)
		{
			user.mDamageFormulaModifiers.mAttackModifier *= DRAGON_DEBUFF_IN_TERRAIN_BASE_DAMAGE_VALUE;
		}
	}
} // namespace PocketCore::Effect
