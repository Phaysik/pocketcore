/*! @file terrainHandler.cpp
	@brief Contains the terrain effect handler implementation
	@date 09/23/2026
	@since 0.9.10
	@version 0.12.43
	@author Matthew Moore
*/

#include "EffectHandler/terrainHandler.h"

#include <algorithm>
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
	using PocketCore::Type::BuiltinTypeID;
	using PocketCore::Type::toTypeID;

	void TerrainHandler::apply(BattleState &state, EffectContext &context, ATTR_MAYBE_UNUSED const RegistryProvider &provider) const
	{
		BattleSlot &user{IEffectHandler::getUserBattleSlot(state, context)};
		const BattleSlot &target{IEffectHandler::getConstTargetBattleSlot(state, context)};

		if (isBattleSlotUngrounded(target))
		{
			return;
		}

		const bool hasElectricTerrain{std::ranges::contains(state.mTerrainIDs, toTerrainID(BuiltinTerrainID::Electric))};
		const bool hasGrassyTerrain{std::ranges::contains(state.mTerrainIDs, toTerrainID(BuiltinTerrainID::Grassy))};
		const bool hasPsychicTerrain{std::ranges::contains(state.mTerrainIDs, toTerrainID(BuiltinTerrainID::Psychic))};
		const bool hasMistyTerrain{std::ranges::contains(state.mTerrainIDs, toTerrainID(BuiltinTerrainID::Misty))};

		const bool isElectricMove{context.mMoveTypeID == toTypeID(BuiltinTypeID::Electric)};
		const bool isGrassMove{context.mMoveTypeID == toTypeID(BuiltinTypeID::Grass)};
		const bool isPsychicMove{context.mMoveTypeID == toTypeID(BuiltinTypeID::Psychic)};
		const bool isDragonMove{context.mMoveTypeID == toTypeID(BuiltinTypeID::Dragon)};

		if (hasElectricTerrain && isElectricMove)
		{
			user.mDamageFormulaModifiers.mAttackModifier *= ELECTRIC_BUFF_IN_TERRAIN_BASE_DAMAGE_VALUE;
		}
		else if (hasGrassyTerrain && isGrassMove)
		{
			user.mDamageFormulaModifiers.mAttackModifier *= GRASS_BUFF_IN_TERRAIN_BASE_DAMAGE_VALUE;
		}
		else if (hasPsychicTerrain && isPsychicMove)
		{
			user.mDamageFormulaModifiers.mAttackModifier *= PSYCHIC_BUFF_IN_TERRAIN_BASE_DAMAGE_VALUE;
		}
		else if (hasMistyTerrain && isDragonMove)
		{
			user.mDamageFormulaModifiers.mAttackModifier *= DRAGON_DEBUFF_IN_TERRAIN_BASE_DAMAGE_VALUE;
		}
	}
} // namespace PocketCore::Effect
