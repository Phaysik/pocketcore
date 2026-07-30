/*! @file effectTypes.h
	@brief Contains the effect types
	@date 07/24/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_EFFECT_EFFECTTYPE_H
#define INCLUDE_EFFECT_EFFECTTYPE_H

#include "Core/typedefs.h"

namespace PocketCore::Effect
{
	enum class EffectTypeID : PocketCore::Core::ub
	{
		CriticalHit,
		BaseDamage,
		Targets,
		Weather,
		Terrain,
		Randomization,
		Stab,
		TypeEffectiveness,
		BurnDamageReduction,
		Flinch,
		Recoil,
		StatChange,
		StatusApply,
		StatusRemove,
		StatusTurnSkip,
		StatusTick,
		AccuracyCheck,
		SetSandstorm,
		SetSun,
		SetRain,
		PsychicTerrainPriorityBlock,
	};
} // namespace PocketCore::Effect

#endif
