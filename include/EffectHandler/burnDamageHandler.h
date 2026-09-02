/*! @file burnDamageHandler.h
	@brief Contains the burn damage effect handler
	@date 09/02/2026
	@since 0.9.5
	@version 0.12.16
	@author Matthew Moore
*/

#ifndef INCLUDE_EFFECT_BURNDAMAGEHANDLER_H
#define INCLUDE_EFFECT_BURNDAMAGEHANDLER_H

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

	/*! @class BurnDamageHandler EffectHandler/burnDamageHandler.h
		@brief Applies the burn damage reduction to a physical move.
		@details The handler adds the built-in burn multiplier to @ref EffectContext when the effect user is burned, the move is
			physical, the user does not have the Guts ability, and the move is not Facade.
		@warning The context must identify a valid user slot.
		@note The handler does not inspect the target slot or the registry provider.
		@date 09/02/2026
		@since 0.9.5
		@version 0.12.16
		@author Matthew Moore
	*/
	class BurnDamageHandler : public IEffectHandler
	{
		public:
			/*! @brief Applies the burn multiplier when the user's burn reduces the move's damage.
				@param[in,out] state The battle state containing the effect user's slot.
				@param[in,out] context The effect context containing the move data and receiving the burn multiplier.
				@param[in] provider The registry provider, which is not used by this handler.
				@pre The user index in @p context must refer to an existing slot in the selected team.
				@post @p context contains the configured burn multiplier only when every burn reduction condition is satisfied.
				@note The context remains unchanged when the user slot has no Pokemon, the move is special, the user has Guts, or the move
				 is Facade.
				@since 0.9.5
				@version 0.12.16
			*/
			void apply(BattleState &state, EffectContext &context, ATTR_MAYBE_UNUSED const RegistryProvider &provider) const override;
	};

	/*! @brief Applies the burn damage reduction to an effect context.
		@param[in,out] state The battle state containing the effect user's slot.
		@param[in,out] context The effect context containing the move data and receiving the burn multiplier.
		@param[in] provider The registry provider, which is not used by this handler.
		@pre The user index in @p context must refer to an existing slot in the selected team.
		@post @p context contains the configured burn multiplier only when every burn reduction condition is satisfied.
		@note The context remains unchanged when the user is not burned or the burn reduction does not apply to the move.
		@since 0.10.0
		@version 0.12.9
	*/
	inline void applyBurnDamage(BattleState &state, EffectContext &context, const RegistryProvider &provider)
	{
		BurnDamageHandler{}.apply(state, context, provider);
	}
} // namespace PocketCore::Effect

#endif
