/*! @file statusTickHandler.h
	@brief Contains the statusTick effect handler
	@date 08/12/2026
	@since 0.10.0
	@version 0.10.0
	@author Matthew Moore
*/

#ifndef INCLUDE_EFFECT_STATUSTICKHANDLER_H
#define INCLUDE_EFFECT_STATUSTICKHANDLER_H

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

	/*! @class StatusTickHandler EffectHandler/statusTickHandler.h
		@date 08/12/2026
		@since 0.10.0
		@version 0.10.0
		@author Matthew Moore
	*/
	class StatusTickHandler : public IEffectHandler
	{
		public:
			/*! @brief Performs no status tick processing.
				@param[in,out] state The battle state, which is not inspected or modified.
				@param[in,out] context The effect context, which is not inspected or modified.
				@param[in] provider The registry provider, which is not used by this handler.
				@note This is a placeholder implementation with no observable effect.
				@since 0.10.0
				@version 0.10.0
			*/
			void apply(BattleState &state, EffectContext &context, ATTR_MAYBE_UNUSED const RegistryProvider &provider) const override;
	};

	/*! @brief Invokes per-turn status processing for an effect context.
		@param[in,out] state The battle state, which is not inspected or modified.
		@param[in,out] context The effect context, which is not inspected or modified.
		@param[in] provider The registry provider, which is not used by this handler.
		@note The call currently has no observable effect.
		@since 0.10.0
		@version 0.10.0
	*/
	inline void applyStatusTick(BattleState &state, EffectContext &context, const RegistryProvider &provider)
	{
		StatusTickHandler{}.apply(state, context, provider);
	}
} // namespace PocketCore::Effect

#endif
