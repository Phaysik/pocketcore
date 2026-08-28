/*! @file populationBombHandler.h
	@brief Contains the population bomb effect handler
	@date 08/12/2026
	@since 0.9.8
	@version 0.10.0
	@author Matthew Moore
*/

#ifndef INCLUDE_EFFECT_POPULATION_BOMB_HANDLER_H
#define INCLUDE_EFFECT_POPULATION_BOMB_HANDLER_H

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

	/*! @class PopulationBombHandler EffectHandler/populationBombHandler.h
		@brief Applies the Population Bomb multiplier for the current hit attempt.
		@details The handler selects the configured first-hit or consecutive-hit multiplier based on the context's one-based hit attempt
			index and stores it as the built-in Population Bomb multiplier in @ref EffectContext.
		@note The handler does not inspect or modify the battle state or registry provider.
		@date 08/04/2026
		@since 0.9.8
		@version 0.9.10
		@author Matthew Moore
	*/
	class PopulationBombHandler : public IEffectHandler
	{
		public:
			/*! @brief Stores the hit-attempt-specific Population Bomb multiplier in the effect context.
				@param[in,out] state The battle state, which is not inspected or modified by this handler.
				@param[in,out] context The effect context containing the hit attempt index and receiving the multiplier.
				@param[in] provider The registry provider, which is not used by this handler.
				@post @p context contains the first-hit multiplier for the first attempt and the consecutive-hit multiplier for later
				 attempts.
				@note A hit attempt index of zero leaves the active multipliers unchanged.
				@since 0.9.8
				@version 0.9.10
			*/
			void apply(ATTR_MAYBE_UNUSED BattleState &state, EffectContext &context,
					   ATTR_MAYBE_UNUSED const RegistryProvider &provider) const override;
	};

	/*! @brief Applies the Population Bomb multiplier to an effect context.
		@param[in,out] state The battle state, which is not inspected or modified by this handler.
		@param[in,out] context The effect context containing the hit attempt index and receiving the multiplier.
		@param[in] provider The registry provider, which is not used by this handler.
		@post @p context contains the first-hit multiplier for the first attempt and the consecutive-hit multiplier for later attempts.
		@note A hit attempt index of zero leaves the active multipliers unchanged.
		@since 0.10.0
		@version 0.10.0
	*/
	inline void applyPopulationBomb(BattleState &state, EffectContext &context, const RegistryProvider &provider)
	{
		PopulationBombHandler{}.apply(state, context, provider);
	}
} // namespace PocketCore::Effect

#endif
