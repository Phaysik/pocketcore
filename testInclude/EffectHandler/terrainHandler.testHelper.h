/*! @file terrainHandler.testHelper.h
	@brief Test helper for dealing with TerrainHandler concepts.
	@date 09/01/2026
	@since 0.12.14
	@version 0.12.14
	@author Matthew Moore
*/

#ifndef TEST_INCLUDE_EFFECT_HANDLER_TERRAIN_HANDLER_TEST_HELPER_H
#define TEST_INCLUDE_EFFECT_HANDLER_TERRAIN_HANDLER_TEST_HELPER_H

#include <cmath>

#include "Battle/battleState.h"
#include "Configuration/constants.h"
#include "Core/typedefs.h"
#include "Multiplier/builtInMultiplierID.h"
#include "Multiplier/multiplierID.h"
#include "Utility/Math/floatUtility.h"

namespace PocketCore::Testing
{
	using PocketCore::Battle::BattleState;
	using PocketCore::Configuration::DRAGON_DEBUFF_IN_TERRAIN_BASE_DAMAGE_VALUE;
	using PocketCore::Configuration::ELECTRIC_BUFF_IN_TERRAIN_BASE_DAMAGE_VALUE;
	using PocketCore::Configuration::GRASS_BUFF_IN_TERRAIN_BASE_DAMAGE_VALUE;
	using PocketCore::Configuration::PSYCHIC_BUFF_IN_TERRAIN_BASE_DAMAGE_VALUE;
	using PocketCore::Configuration::TARGETS_HIT_MULTIPLIER_VALUE;
	using PocketCore::Core::ub;
	using PocketCore::Multiplier::BuiltinMultiplierID;
	using PocketCore::Multiplier::MultiplierID;
	using PocketCore::Multiplier::toMultiplierID;
	using PocketCore::Utility::Math::approximatelyEqualAbsRel;

	constexpr bool hasElectricBuffInTerrain(const BattleState &state, const ub userIndex)
	{
		return approximatelyEqualAbsRel(state.mSideA.at(userIndex).mDamageFormulaModifiers.mAttackModifier,
										ELECTRIC_BUFF_IN_TERRAIN_BASE_DAMAGE_VALUE);
	}

	constexpr bool hasGrassBuffInTerrain(const BattleState &state, const ub userIndex)
	{
		return approximatelyEqualAbsRel(state.mSideA.at(userIndex).mDamageFormulaModifiers.mAttackModifier,
										GRASS_BUFF_IN_TERRAIN_BASE_DAMAGE_VALUE);
	}

	constexpr bool hasPsychicBuffInTerrain(const BattleState &state, const ub userIndex)
	{
		return approximatelyEqualAbsRel(state.mSideA.at(userIndex).mDamageFormulaModifiers.mAttackModifier,
										PSYCHIC_BUFF_IN_TERRAIN_BASE_DAMAGE_VALUE);
	}

	constexpr bool hasDragonDebuffInTerrain(const BattleState &state, const ub userIndex)
	{
		return approximatelyEqualAbsRel(state.mSideA.at(userIndex).mDamageFormulaModifiers.mAttackModifier,
										DRAGON_DEBUFF_IN_TERRAIN_BASE_DAMAGE_VALUE);
	}

} // namespace PocketCore::Testing

#endif
