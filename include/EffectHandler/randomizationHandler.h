/*! @file randomizationHandler.h
	@brief Contains the randomization effect handler
	@date 08/12/2026
	@since 0.9.6
	@version 0.10.0
	@author Matthew Moore
*/

#ifndef INCLUDE_EFFECT_RANDOMIZATIONHANDLER_H
#define INCLUDE_EFFECT_RANDOMIZATIONHANDLER_H

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

	/*! @class RandomizationHandler EffectHandler/randomizationHandler.h
		@brief Applies the random damage spread multiplier to a damage effect.
		@details The handler rolls a value within the configured randomization percentage range, converts it to a fractional multiplier,
			and stores it as the built-in randomization multiplier in @ref EffectContext.
		@warning The context must identify valid user and target slots.
		@note The multiplier is always applied, and the roll is not deterministic between calls.
		@date 08/04/2026
		@since 0.9.6
		@version 0.9.10
		@author Matthew Moore
	*/
	class RandomizationHandler : public IEffectHandler
	{
		public:
			/*! @brief Rolls the damage randomization multiplier and stores it in the effect context.
				@param[in,out] state The battle state containing the user and target slots.
				@param[in,out] context The effect context receiving the randomization multiplier.
				@param[in] provider The registry provider, which is not used by this handler.
				@pre The user and target indexes in @p context must refer to existing slots in their selected teams.
				@post @p context contains a randomization multiplier derived from the configured percentage range.
				@note The handler logs the participating slots, which report `<empty>` when a slot has no Pokemon.
				@since 0.9.6
				@version 0.9.10
			*/
			void apply(BattleState &state, EffectContext &context, ATTR_MAYBE_UNUSED const RegistryProvider &provider) const override;
	};

	/*! @brief Applies the random damage spread multiplier to an effect context.
		@param[in,out] state The battle state containing the user and target slots.
		@param[in,out] context The effect context receiving the randomization multiplier.
		@param[in] provider The registry provider, which is not used by this handler.
		@pre The user and target indexes in @p context must refer to existing slots in their selected teams.
		@post @p context contains a randomization multiplier derived from the configured percentage range.
		@since 0.10.0
		@version 0.10.0
	*/
	inline void applyRandomization(BattleState &state, EffectContext &context, const RegistryProvider &provider)
	{
		RandomizationHandler{}.apply(state, context, provider);
	}
} // namespace PocketCore::Effect

#endif
