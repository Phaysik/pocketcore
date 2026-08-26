/*! @file accuracyCheckHandler.h
	@brief Contains the accuracy check effect handler
	@date 07/29/2026
	@since 0.7.5
	@version 0.7.5
	@author Matthew Moore
*/

#ifndef INCLUDE_EFFECT_ACCURACYCHECKHANDLER_H
#define INCLUDE_EFFECT_ACCURACYCHECKHANDLER_H

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

	/*! @class AccuracyCheckHandler EffectHandler/accuracyCheckHandler.h
		@brief Determines whether a move hits its selected target.
		@details The handler combines the move's accuracy with the user's accuracy stage and the target's evasion stage,
			clamps the resulting accuracy to the valid range, and performs a random hit check.
		@warning The context must identify valid user and target slots in their selected teams.
		@note The handler does not inspect or modify the registry provider.
		@date 07/29/2026
		@since 0.7.5
		@version 0.7.5
		@author Matthew Moore
	*/
	class AccuracyCheckHandler : public IEffectHandler
	{
		public:
			/*! @brief Performs an accuracy roll and updates the effect context when the move misses.
				@param[in,out] state The battle state containing the user and target slots.
				@param[in,out] context The effect context containing accuracy inputs and receiving miss and continuation flags.
				@param[in] provider The registry provider, which is not used by this handler.
				@pre The user and target indexes in @p context must refer to existing slots in their selected teams.
				@post When the roll fails, @p context marks the damage as missed, disables damage application, and stops subsequent effect processing.
				@note The effective accuracy is clamped to the range from 0 to 100. A successful roll leaves the damage flags unchanged.
				@since 0.7.5
				@version 0.7.5
			*/
			void apply(BattleState &state, EffectContext &context, ATTR_MAYBE_UNUSED const RegistryProvider &provider) const override;
	};

	/*! @brief Applies an accuracy check to an effect context.
		@param[in,out] state The battle state containing the user and target slots.
		@param[in,out] context The effect context containing accuracy inputs and receiving miss and continuation flags.
		@param[in] provider The registry provider, which is not used by this check.
		@pre The user and target indexes in @p context must refer to existing slots in their selected teams.
		@post When the roll fails, @p context marks the damage as missed, disables damage application, and stops subsequent effect processing.
		@note A successful roll leaves the damage flags unchanged.
	*/
	inline void applyAccuracyCheck(BattleState &state, EffectContext &context, const RegistryProvider &provider)
	{
		AccuracyCheckHandler{}.apply(state, context, provider);
	}
} // namespace PocketCore::Effect

#endif
