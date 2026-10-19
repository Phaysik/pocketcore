/*! @file statusRemoveHandler.h
	@brief Contains the statusRemove effect handler
	@date 10/19/2026
	@since 0.10.0
	@version 0.12.51
	@author Matthew Moore
*/

#ifndef INCLUDE_EFFECT_STATUSREMOVEHANDLER_H
#define INCLUDE_EFFECT_STATUSREMOVEHANDLER_H

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

	/*! @class StatusRemoveHandler EffectHandler/statusRemoveHandler.h
		@date 10/19/2026
		@since 0.10.0
		@version 0.12.51
		@author Matthew Moore
	*/
	class StatusRemoveHandler : public IEffectHandler
	{
		public:
			/*! @brief Performs no status removal.
				@param[in,out] state The battle state, which is not inspected or modified.
				@param[in,out] context The effect context, which is not inspected or modified.
				@param[in] provider The registry provider, which is not used by this handler.
				@param[in] randomSources The random sources used for battle-related numeric types.
				@note This is a placeholder implementation with no observable effect.
				@since 0.10.0
				@version 0.12.51
			*/
			void apply(BattleState &state, EffectContext &context, ATTR_MAYBE_UNUSED const RegistryProvider &provider,
					   ATTR_MAYBE_UNUSED const BattleRandomSources &randomSources) const override;
	};

	/*! @brief Invokes status removal for an effect context.
		@param[in,out] state The battle state, which is not inspected or modified.
		@param[in,out] context The effect context, which is not inspected or modified.
		@param[in] provider The registry provider, which is not used by this handler.
		@param[in] randomSources The random sources used for battle-related numeric types.
		@note The call currently has no observable effect.
		@since 0.10.0
		@version 0.12.51
	*/
	inline void applyStatusRemove(BattleState &state, EffectContext &context, const RegistryProvider &provider,
								  const BattleRandomSources &randomSources = {})
	{
		StatusRemoveHandler{}.apply(state, context, provider, randomSources);
	}
} // namespace PocketCore::Effect

#endif
