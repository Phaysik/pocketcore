/*! @file setRainHandler.h
	@brief Contains the setRain effect handler
	@date 08/12/2026
	@since 0.10.0
	@version 0.10.0
	@author Matthew Moore
*/

#ifndef INCLUDE_EFFECT_SETRAINHANDLER_H
#define INCLUDE_EFFECT_SETRAINHANDLER_H

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

	/*! @class SetRainHandler EffectHandler/setRainHandler.h
		@brief Sets the battle-wide weather to rain.
		@details The handler unconditionally replaces the current weather identifier with the built-in rain identifier.
		@warning Any previously active weather is overwritten without checking for immunity or duration.
		@note The handler does not inspect the effect context or registry provider.
		@date 08/12/2026
		@since 0.10.0
		@version 0.10.0
		@author Matthew Moore
	*/
	class SetRainHandler : public IEffectHandler
	{
		public:
			/*! @brief Replaces the active weather with rain.
				@param[in,out] state The battle state whose weather identifier is updated.
				@param[in,out] context The effect context, which is not inspected or modified.
				@param[in] provider The registry provider, which is not used by this handler.
				@post @p state reports rain as the active weather.
				@since 0.10.0
				@version 0.10.0
			*/
			void apply(BattleState &state, EffectContext &context, ATTR_MAYBE_UNUSED const RegistryProvider &provider) const override;
	};

	/*! @brief Sets the battle-wide weather to rain.
		@param[in,out] state The battle state whose weather identifier is updated.
		@param[in,out] context The effect context, which is not inspected or modified.
		@param[in] provider The registry provider, which is not used by this handler.
		@post @p state reports rain as the active weather.
		@since 0.10.0
		@version 0.10.0
	*/
	inline void applySetRain(BattleState &state, EffectContext &context, const RegistryProvider &provider)
	{
		SetRainHandler{}.apply(state, context, provider);
	}
} // namespace PocketCore::Effect

#endif
