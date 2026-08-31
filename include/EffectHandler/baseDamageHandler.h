/*! @file baseDamageHandler.h
	@brief Contains the base damage effect handler
	@date 08/26/2026
	@since 0.7.2
	@version 0.12.7
	@author Matthew Moore
*/

#ifndef INCLUDE_EFFECT_BASEDAMAGEHANDLER_H
#define INCLUDE_EFFECT_BASEDAMAGEHANDLER_H

#include "Battle/battleState.h"
#include "Core/attributeMacros.h"
#include "Effect/effectContext.h"
#include "Registry/registryProvider.h"

#include "effectHandlerInterface.h"

namespace PocketCore::Effect
{
	using PocketCore::Battle::BattleSlot;
	using PocketCore::Battle::BattleState;
	using PocketCore::Registry::RegistryProvider;

	/*! @class BaseDamageHandler EffectHandler/baseDamageHandler.h
		@brief Calculates and stores the base damage for a damage effect.
		@details The handler resolves the user's and target's battle slots from @ref EffectContext,
			then applies the selected physical or special attack and defense values, stat-stage modifiers,
			formula modifiers, move base power, and the user's level damage factor. Critical hits use the
			minimum applicable attack and defense stages before calculating damage.
		@warning The context must identify valid user and target slots. A missing Pokemon, zero move base
			power, invalid effective stat, or non-positive effective defense leaves the existing damage unchanged.
		@note The calculated damage is clamped to the range representable by the damage field.
		@date 08/26/2026
		@since 0.7.2
		@version 0.12.7
		@author Matthew Moore
	*/
	class BaseDamageHandler : public IEffectHandler
	{
		public:
			/*! @brief Calculates base damage and stores it in the effect context.
				@param[in,out] state The battle state containing the user and target slots.
				@param[in,out] context The effect context containing calculation inputs and receiving the damage result.
				@param[in] provider The registry provider retained for the common effect-handler interface; this handler does not use it.
				@pre The user and target indexes in @p context must refer to existing slots in their selected teams.
				@post On successful calculation, @p context contains damage clamped to the representable damage range.
				@note The existing damage value remains unchanged when required Pokemon data or valid effective statistics are unavailable.
				@since 0.7.2
				@version 0.12.7
			*/
			void apply(BattleState &state, EffectContext &context, ATTR_MAYBE_UNUSED const RegistryProvider &provider) const override;
	};

	/*! @brief Applies the base damage calculation to an effect context.
		@param[in,out] state The battle state containing the user and target slots.
		@param[in,out] context The effect context containing calculation inputs and receiving the damage result.
		@param[in] provider The registry provider passed to the common effect-handler interface; this calculation does not use it.
		@pre The user and target indexes in @p context must refer to existing slots in their selected teams.
		@post On successful calculation, @p context contains the calculated base damage.
		@note The existing damage value remains unchanged when the calculation cannot be performed.
		@since 0.10.0
		@version 0.12.7
	*/
	inline void applyBaseDamage(BattleState &state, EffectContext &context, const RegistryProvider &provider)
	{
		BaseDamageHandler{}.apply(state, context, provider);
	}
} // namespace PocketCore::Effect

#endif
