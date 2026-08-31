/*! @file typeEffectivenessHandler.h
	@brief Contains the type effectiveness handler
	@date 08/28/2026
	@since 0.9.2
	@version 0.12.9
	@author Matthew Moore
*/

#ifndef INCLUDE_EFFECT_TYPEEFFECTIVENESSHANDLER_H
#define INCLUDE_EFFECT_TYPEEFFECTIVENESSHANDLER_H

#include "Battle/battleState.h"
#include "Effect/effectContext.h"
#include "Registry/registryProvider.h"

#include "effectHandlerInterface.h"

namespace PocketCore::Effect
{
	using PocketCore::Battle::BattleSlot;
	using PocketCore::Battle::BattleState;
	using PocketCore::Registry::RegistryProvider;

	/*! @class TypeEffectivenessHandler EffectHandler/typeEffectivenessHandler.h
		@brief Applies the type effectiveness multiplier for a damaging move.
		@details The handler resolves the move's type and each of the target's types in the type registry, multiplies the effectiveness
			values of every defined type chart cell, and stores the product as the built-in type effectiveness multiplier in
			@ref EffectContext.
		@warning The context must identify valid user and target slots.
		@note Undefined type chart cells and unregistered target types are skipped and leave the running product unchanged.
		@date 08/28/2026
		@since 0.9.2
		@version 0.12.9
		@author Matthew Moore
	*/
	class TypeEffectivenessHandler : public IEffectHandler
	{
		public:
			/*! @brief Calculates the combined type effectiveness and stores it in the effect context.
				@param[in,out] state The battle state containing the user and target slots.
				@param[in,out] context The effect context containing the move type and receiving the type effectiveness multiplier.
				@param[in] provider The registry provider used to resolve type identifiers and type chart entries.
				@pre The user and target indexes in @p context must refer to existing slots in their selected teams.
				@post On success, @p context contains the product of every defined effectiveness value against the target's types.
				@note The context remains unchanged when either slot has no Pokemon, the move base power is zero, the type registry is
				 unavailable, or the move type is not registered.
				@since 0.9.2
				@version 0.12.9
			*/
			void apply(BattleState &state, EffectContext &context, const RegistryProvider &provider) const override;
	};

	/*! @brief Applies the type effectiveness multiplier to an effect context.
		@param[in,out] state The battle state containing the user and target slots.
		@param[in,out] context The effect context containing the move type and receiving the type effectiveness multiplier.
		@param[in] provider The registry provider used to resolve type identifiers and type chart entries.
		@pre The user and target indexes in @p context must refer to existing slots in their selected teams.
		@post On success, @p context contains the product of every defined effectiveness value against the target's types.
		@note The context remains unchanged when required Pokemon, move base power, or type registry data is unavailable.
		@since 0.10.0
		@version 0.12.9
	*/
	inline void applyTypeEffectiveness(BattleState &state, EffectContext &context, const RegistryProvider &provider)
	{
		TypeEffectivenessHandler{}.apply(state, context, provider);
	}
} // namespace PocketCore::Effect

#endif
