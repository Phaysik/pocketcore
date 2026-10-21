/*! @file setSandstormHandler.h
	@brief Contains the setSandstorm effect handler
	@date 10/19/2026
	@since 0.10.0
	@version 0.12.51
	@author Matthew Moore
*/

#ifndef INCLUDE_EFFECT_SETSANDSTORMHANDLER_H
#define INCLUDE_EFFECT_SETSANDSTORMHANDLER_H

#include "Battle/battleRandomSources.h"
#include "Battle/battleState.h"
#include "Core/attributeMacros.h"
#include "Effect/effectContext.h"
#include "Registry/registryProvider.h"

#include "effectHandlerInterface.h"

namespace PocketCore::Effect
{
	using PocketCore::Battle::BattleRandomSources;
	using PocketCore::Battle::BattleSlot;
	using PocketCore::Battle::BattleState;
	using PocketCore::Registry::RegistryProvider;

	/*! @class SetSandstormHandler EffectHandler/setSandstormHandler.h
		@brief Sets the battle-wide weather to sandstorm.
		@details The handler unconditionally replaces the current weather identifier with the built-in sandstorm identifier.
		@warning Any previously active weather is overwritten without checking for immunity or duration.
		@note The handler does not inspect the effect context or registry provider.
		@date 10/19/2026
		@since 0.10.0
		@version 0.12.51
		@author Matthew Moore
	*/
	class SetSandstormHandler : public IEffectHandler
	{
		public:
			/*! @brief Replaces the active weather with sandstorm.
				@param[in,out] state The battle state whose weather identifier is updated.
				@param[in,out] context The effect context, which is not inspected or modified.
				@param[in] provider The registry provider, which is not used by this handler.
				@param[in] randomSources The random sources used for battle-related numeric types.
				@post @p state reports sandstorm as the active weather.
				@since 0.10.0
				@version 0.12.51
			*/
			void apply(BattleState &state, EffectContext &context, ATTR_MAYBE_UNUSED const RegistryProvider &provider,
					   ATTR_MAYBE_UNUSED const BattleRandomSources &randomSources) const override;
	};

	/*! @brief Sets the battle-wide weather to sandstorm.
		@param[in,out] state The battle state whose weather identifier is updated.
		@param[in,out] context The effect context, which is not inspected or modified.
		@param[in] provider The registry provider, which is not used by this handler.
		@param[in] randomSources The random sources used for battle-related numeric types.
		@post @p state reports sandstorm as the active weather.
		@since 0.10.0
		@version 0.12.51
	*/
	inline void applySetSandstorm(BattleState &state, EffectContext &context, const RegistryProvider &provider,
								  const BattleRandomSources &randomSources = {})
	{
		SetSandstormHandler{}.apply(state, context, provider, randomSources);
	}
} // namespace PocketCore::Effect

#endif
