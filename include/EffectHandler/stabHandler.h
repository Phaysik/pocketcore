/*! @file stabHandler.h
	@brief Contains the same type attack bonus damage effect handler
	@date 08/12/2026
	@since 0.8.1
	@version 0.10.0
	@author Matthew Moore
*/

#ifndef INCLUDE_EFFECT_STABHANDLER_H
#define INCLUDE_EFFECT_STABHANDLER_H

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

	/*! @class StabHandler EffectHandler/stabHandler.h
		@brief Applies the same-type attack bonus to a damage effect.
		@details The handler checks whether the effect user's Pokemon has the move's type and, when it does, adds or updates the
			built-in STAB multiplier in @ref EffectContext.
		@warning The context must identify a valid user slot containing a Pokemon.
		@note The handler does not inspect or modify the target slot or registry provider.
		@date 08/04/2026
		@since 0.8.1
		@version 0.9.10
		@author Matthew Moore
	*/
	class StabHandler : public IEffectHandler
	{
		public:
			/*! @brief Applies the same-type attack bonus when the user's Pokemon matches the move type.
				@param[in,out] state The battle state containing the effect user's slot.
				@param[in,out] context The effect context containing the move type and receiving the STAB multiplier.
				@param[in] provider The registry provider, which is not used by this handler.
				@pre The user index in @p context must refer to an existing slot containing a Pokemon.
				@post When the user's Pokemon has the move type, @p context contains the configured STAB multiplier. Otherwise, the active
			   multipliers remain unchanged.
				@since 0.8.1
				@version 0.9.10
			*/
			void apply(BattleState &state, EffectContext &context, ATTR_MAYBE_UNUSED const RegistryProvider &provider) const override;
	};

	/*! @brief Applies the same-type attack bonus to an effect context.
		@param[in,out] state The battle state containing the effect user's slot.
		@param[in,out] context The effect context containing the move type and receiving the STAB multiplier.
		@param[in] provider The registry provider, which is not used by this handler.
		@pre The user index in @p context must refer to an existing slot containing a Pokemon.
		@post When the user's Pokemon has the move type, @p context contains the configured STAB multiplier. Otherwise, the active
	   multipliers remain unchanged.
		@since 0.10.0
		@version 0.10.0
	*/
	inline void applySTAB(BattleState &state, EffectContext &context, const RegistryProvider &provider)
	{
		StabHandler{}.apply(state, context, provider);
	}
} // namespace PocketCore::Effect

#endif
