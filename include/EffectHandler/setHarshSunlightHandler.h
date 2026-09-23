/*! @file setHarshSunlightHandler.h
	@brief Contains the setHarshSunlight effect handler
	@date 09/23/2026
	@since 0.10.0
	@version 0.12.43
	@author Matthew Moore
*/

#ifndef INCLUDE_EFFECT_SET_HARSH_SUNLIGHT_HANDLER_H
#define INCLUDE_EFFECT_SET_HARSH_SUNLIGHT_HANDLER_H

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

	/*! @class SetHarshSunlightHandler EffectHandler/setHarshSunlightHandler.h
		@brief Sets the battle-wide weather to harsh sunlight.
		@details The handler unconditionally replaces the current weather identifier with the built-in harsh sunlight identifier.
		@warning Any previously active weather is overwritten without checking for immunity or duration.
		@note The handler does not inspect the effect context or registry provider.
		@date 09/23/2026
		@since 0.10.0
		@version 0.12.43
		@author Matthew Moore
	*/
	class SetHarshSunlightHandler : public IEffectHandler
	{
		public:
			/*! @brief Replaces the active weather with harsh sunlight.
				@param[in,out] state The battle state whose weather identifier is updated.
				@param[in,out] context The effect context, which is not inspected or modified.
				@param[in] provider The registry provider, which is not used by this handler.
				@post @p state reports harsh sunlight as the active weather.
				@since 0.10.0
				@version 0.12.43
			*/
			void apply(BattleState &state, EffectContext &context, ATTR_MAYBE_UNUSED const RegistryProvider &provider) const override;
	};

	/*! @brief Sets the battle-wide weather to harsh sunlight.
		@param[in,out] state The battle state whose weather identifier is updated.
		@param[in,out] context The effect context, which is not inspected or modified.
		@param[in] provider The registry provider, which is not used by this handler.
		@post @p state reports harsh sunlight as the active weather.
		@since 0.10.0
		@version 0.12.43
	*/
	inline void applySetHarshSunlight(BattleState &state, EffectContext &context, const RegistryProvider &provider)
	{
		SetHarshSunlightHandler{}.apply(state, context, provider);
	}
} // namespace PocketCore::Effect

#endif
