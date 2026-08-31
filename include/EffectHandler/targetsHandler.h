/*! @file targetsHandler.h
	@brief Contains the targets effect handler
	@date 08/26/2026
	@since 0.8.1
	@version 0.12.7
	@author Matthew Moore
*/

#ifndef INCLUDE_EFFECT_TARGETSHANDLER_H
#define INCLUDE_EFFECT_TARGETSHANDLER_H

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

	/*! @class TargetsHandler EffectHandler/targetsHandler.h
		@brief Applies the damage modifier associated with a move's target pattern.
		@details The handler resolves the move metadata and adds or updates the built-in targets multiplier for moves that target more
			than a single opponent or the user.
		@warning The registry provider must contain an available move registry.
		@note Missing move metadata leaves the effect context unchanged. The battle state is not inspected or modified.
		@date 08/26/2026
		@since 0.8.1
		@version 0.12.7
		@author Matthew Moore
	*/
	class TargetsHandler : public IEffectHandler
	{
		public:
			/*! @brief Applies the target-count multiplier for the move described by the effect context.
				@param[in] state The battle state, which is not inspected or modified by this handler.
				@param[in,out] context The effect context containing the move identifier and receiving the targets multiplier.
				@param[in] provider The registry provider used to resolve move metadata.
				@pre @p provider must contain an available move registry.
				@post Moves targeting more than a single opponent or the user receive the configured targets multiplier. Missing move
			   metadata leaves @p context unchanged.
				@since 0.8.1
				@version 0.12.7
			*/
			void apply(ATTR_MAYBE_UNUSED BattleState &state, EffectContext &context, const RegistryProvider &provider) const override;
	};

	/*! @brief Applies the target-count multiplier for the move in an effect context.
		@param[in,out] state The battle state, which is not inspected or modified by this handler.
		@param[in,out] context The effect context containing the move identifier and receiving the targets multiplier.
		@param[in] provider The registry provider used to resolve move metadata.
		@pre @p provider must contain an available move registry.
		@post Moves targeting more than a single opponent or the user receive the configured targets multiplier. Missing move metadata
	   leaves @p context unchanged.
		@since 0.10.0
		@version 0.12.7
	*/
	inline void applyTargets(BattleState &state, EffectContext &context, const RegistryProvider &provider)
	{
		TargetsHandler{}.apply(state, context, provider);
	}
} // namespace PocketCore::Effect

#endif
