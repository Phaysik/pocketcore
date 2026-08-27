/*! @file criticalHitHandler.h
	@brief Contains the critical hit effect handler
	@date 08/03/2026
	@since 0.7.2
	@version 0.8.7
	@author Matthew Moore
*/

#ifndef INCLUDE_EFFECT_CRITICALHITHANDLER_H
#define INCLUDE_EFFECT_CRITICALHITHANDLER_H

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

	/*! @class CriticalHitHandler EffectHandler/criticalHitHandler.h
		@brief Determines whether an effect produces a critical hit.
		@details The handler performs a configured random critical-hit roll. When the roll succeeds,
		it marks the damage as critical and stores the configured critical-hit multiplier in @ref EffectContext.
		@note The handler does not inspect or modify the battle state or registry provider.
		@date 08/03/2026
		@since 0.7.2
		@version 0.8.7
		@author Matthew Moore
	*/
	class CriticalHitHandler : public IEffectHandler
	{
		public:
			/*! @brief Performs the critical-hit roll and updates the effect context on success.
				@param[in,out] state The battle state, which is not inspected or modified by this handler.
				@param[in,out] context The effect context receiving the critical-hit flag and multiplier when the roll succeeds.
				@param[in] provider The registry provider, which is not used by this handler.
				@post On a successful roll, @p context marks the damage as critical and contains the configured critical-hit multiplier.
				@note A failed roll leaves the critical-hit state and multiplier unchanged.
				@since 0.7.2
				@version 0.8.7
			*/
			void apply(ATTR_MAYBE_UNUSED BattleState &state, EffectContext &context,
					   ATTR_MAYBE_UNUSED const RegistryProvider &provider) const override;
	};

	/*! @brief Applies the critical-hit roll to an effect context.
		@param[in,out] state The battle state, which is not inspected or modified by this handler.
		@param[in,out] context The effect context receiving the critical-hit flag and multiplier when the roll succeeds.
		@param[in] provider The registry provider, which is not used by this handler.
		@post On a successful roll, @p context marks the damage as critical and contains the configured critical-hit multiplier.
		@note A failed roll leaves the critical-hit state and multiplier unchanged.
	*/
	inline void applyCriticalHit(BattleState &state, EffectContext &context, const RegistryProvider &provider)
	{
		CriticalHitHandler{}.apply(state, context, provider);
	}
} // namespace PocketCore::Effect

#endif
