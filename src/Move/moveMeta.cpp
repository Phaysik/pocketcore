/*! \file moveMeta.cpp
	\brief Contains the function definitions for creating a moveMeta
	\date --/--/----
	\version x.x.x
	\since x.x.x
	\author Matthew Moore
*/

#include "Move/moveMeta.h"

#include <vector>

#include "Effect/effectType.h"

namespace PocketCore::Move
{
	using PocketCore::Effect::EffectTypeID;

	std::vector<EffectTypeID> baseAttackEffects()
	{
		return {EffectTypeID::PsychicTerrainPriorityBlock,
				EffectTypeID::AccuracyCheck,
				EffectTypeID::BaseDamage,
				EffectTypeID::CriticalHit,
				EffectTypeID::StatStage,
				EffectTypeID::BurnDamageReduction,
				EffectTypeID::Weather,
				EffectTypeID::Terrain,
				EffectTypeID::TypeEffectiveness,
				EffectTypeID::Randomization};
	}

	std::vector<EffectTypeID> baseAttackWithRecoil()
	{
		std::vector<EffectTypeID> baseAttackEffectsVector{baseAttackEffects()};
		baseAttackEffectsVector.push_back(EffectTypeID::Recoil);
		return baseAttackEffectsVector;
	}

	std::vector<EffectTypeID> baseAttackWithStatus()
	{
		std::vector<EffectTypeID> baseAttackEffectsVector{baseAttackEffects()};
		baseAttackEffectsVector.push_back(EffectTypeID::StatusApply);
		return baseAttackEffectsVector;
	}

	std::vector<EffectTypeID> baseAttackWithFlinch()
	{
		std::vector<EffectTypeID> baseAttackEffectsVector{baseAttackEffects()};
		baseAttackEffectsVector.push_back(EffectTypeID::Flinch);
		return baseAttackEffectsVector;
	}

	std::vector<EffectTypeID> statusChangeEffects()
	{
		return {EffectTypeID::PsychicTerrainPriorityBlock, EffectTypeID::AccuracyCheck, EffectTypeID::StatChange};
	}

	std::vector<EffectTypeID> protectEffects()
	{
		return {EffectTypeID::PsychicTerrainPriorityBlock};
	}

	std::vector<EffectTypeID> fieldEffectEffects()
	{
		return {EffectTypeID::PsychicTerrainPriorityBlock, EffectTypeID::AccuracyCheck};
	}
} // namespace PocketCore::Move
