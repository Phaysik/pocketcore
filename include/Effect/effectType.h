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
		BaseDamage,
		CriticalHit,
		StatStage,
		BurnDamageReduction,
		Weather,
		Terrain,
		TypeEffectiveness,
		Randomization,
		Flinch,
		Recoil,
		StatChange,
		StatusApply,
		StatusTurnSkip,
		StatusTick,
		AccuracyCheck,
		SetSandstorm,
		SetSun,
		SetRain
	};
} // namespace PocketCore::Effect

#endif
