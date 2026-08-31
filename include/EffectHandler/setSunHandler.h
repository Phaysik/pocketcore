/*! @file setSunHandler.h
	@brief Contains the setSun effect handler
	@date 08/28/2026
	@since 0.10.0
	@version 0.12.9
	@author Matthew Moore
*/

#ifndef INCLUDE_EFFECT_SETSUNHANDLER_H
#define INCLUDE_EFFECT_SETSUNHANDLER_H

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

	/*! @class SetSunHandler EffectHandler/setSunHandler.h
		@brief Sets the battle-wide weather to harsh sunlight.
		@details The handler unconditionally replaces the current weather identifier with the built-in harsh sunlight identifier.
		@warning Any previously active weather is overwritten without checking for immunity or duration.
		@note The handler does not inspect the effect context or registry provider.
		@date 08/28/2026
		@since 0.10.0
		@version 0.12.9
		@author Matthew Moore
	*/
	class SetSunHandler : public IEffectHandler
	{
		public:
			/*! @brief Replaces the active weather with harsh sunlight.
				@param[in,out] state The battle state whose weather identifier is updated.
				@param[in,out] context The effect context, which is not inspected or modified.
				@param[in] provider The registry provider, which is not used by this handler.
				@post @p state reports harsh sunlight as the active weather.
				@since 0.10.0
				@version 0.12.9
			*/
			void apply(BattleState &state, EffectContext &context, ATTR_MAYBE_UNUSED const RegistryProvider &provider) const override;
	};

	/*! @brief Sets the battle-wide weather to harsh sunlight.
		@param[in,out] state The battle state whose weather identifier is updated.
		@param[in,out] context The effect context, which is not inspected or modified.
		@param[in] provider The registry provider, which is not used by this handler.
		@post @p state reports harsh sunlight as the active weather.
		@since 0.10.0
		@version 0.12.9
	*/
	inline void applySetSun(BattleState &state, EffectContext &context, const RegistryProvider &provider)
	{
		SetSunHandler{}.apply(state, context, provider);
	}
} // namespace PocketCore::Effect

#endif
