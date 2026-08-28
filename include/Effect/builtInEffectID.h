/*! @file builtInEffectID.h
	@brief Contains the effect types
	@date 08/12/2026
	@since 0.10.0
	@version 0.10.0
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

	/*! @enum BuiltinEffectID
		@showenumvalues
		@brief Catalogs the effects registered by default in the effect registry.
		@details Each value maps to a stable @ref EffectID through @ref toEffectID, and the enumerator order defines the built-in
			identifier values. Existing enumerators must not be reordered because their numeric values are stable identifiers.
		@note All enum values must be handled exhaustively when dispatching on a built-in effect.
		@date 08/12/2026
		@since 0.10.0
		@version 0.10.0
		@author Matthew Moore
	*/
	enum class BuiltinEffectID : ub
	{
		/*! @brief Identifies the absence of an effect. */
		None,
		/*! @brief Rolls for a critical hit and applies its multiplier. */
		CriticalHit,
		/*! @brief Calculates the move's base damage. */
		BaseDamage,
		/*! @brief Applies the multiplier for moves affecting multiple targets. */
		Targets,
		/*! @brief Applies the active weather's damage multiplier. */
		Weather,
		/*! @brief Applies the active terrain's damage adjustment. */
		Terrain,
		/*! @brief Applies the per-hit multiplier used by Population Bomb. */
		PopulationBomb,
		/*! @brief Applies the random damage spread multiplier. */
		Randomization,
		/*! @brief Applies the same-type attack bonus. */
		Stab,
		/*! @brief Applies the combined type effectiveness multiplier. */
		TypeEffectiveness,
		/*! @brief Applies the damage reduction caused by a burn. */
		BurnDamageReduction,
		/*! @brief Prevents the target from acting after flinching. */
		Flinch,
		/*! @brief Applies recoil damage to the effect user. */
		Recoil,
		/*! @brief Applies a status condition to the target. */
		StatusApply,
		/*! @brief Removes a status condition from the target. */
		StatusRemove,
		/*! @brief Skips a turn because of an active status condition. */
		StatusTurnSkip,
		/*! @brief Advances per-turn status counters and residual damage. */
		StatusTick,
		/*! @brief Determines whether the move hits its target. */
		AccuracyCheck,
		/*! @brief Sets the battle-wide weather to sandstorm. */
		SetSandstorm,
		/*! @brief Sets the battle-wide weather to harsh sunlight. */
		SetSun,
		/*! @brief Sets the battle-wide weather to rain. */
		SetRain,
		/*! @brief Blocks increased priority moves in psychic terrain. */
		PsychicTerrainPriorityBlock,
		/*! @brief Marks the end of the built-in catalog and equals the number of built-in effects. */
		FinalEffect,
	};

	/*! @brief Converts a built-in catalog value to its stable registry identifier.
		@param[in] builtinEffectID The built-in effect to convert.
		@return The corresponding open effect identifier.
		@since 0.10.0
		@version 0.10.0
	*/
	ATTR_NODISCARD constexpr EffectID toEffectID(const BuiltinEffectID builtinEffectID) noexcept
	{
		return EffectID{static_cast<ub>(builtinEffectID)};
	}
} // namespace PocketCore::Effect

#endif
