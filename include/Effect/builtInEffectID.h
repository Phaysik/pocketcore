/*! @file builtInEffectID.h
	@brief Contains the effect types
	@date 07/24/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_EFFECT_BUILT_IN_EFFECT_ID_H
#define INCLUDE_EFFECT_BUILT_IN_EFFECT_ID_H

#include "Core/attributeMacros.h"
#include "Core/typedefs.h"

#include "effectID.h"

namespace PocketCore::Effect
{
	using PocketCore::Core::ub;

	enum class BuiltinEffectID : ub
	{
		None,
		CriticalHit,
		BaseDamage,
		Targets,
		Weather,
		Terrain,
		PopulationBomb,
		Randomization,
		Stab,
		TypeEffectiveness,
		BurnDamageReduction,
		Flinch,
		Recoil,
		StatusApply,
		StatusRemove,
		StatusTurnSkip,
		StatusTick,
		AccuracyCheck,
		SetSandstorm,
		SetSun,
		SetRain,
		PsychicTerrainPriorityBlock,
		FinalEffect,
	};

	/*! @brief Converts a built-in catalog value to its stable registry identifier.
		@param[in] builtinEffectID The built-in effect to convert.
		@return The corresponding open effect identifier.
	*/
	ATTR_NODISCARD constexpr EffectID toEffectID(const BuiltinEffectID builtinEffectID) noexcept
	{
		return EffectID{static_cast<ub>(builtinEffectID)};
	}
} // namespace PocketCore::Effect

#endif
