/*! @file weatherHandler.h
	@brief Contains the weather effect handler
	@date 08/12/2026
	@since 0.9.8
	@version 0.10.0
	@author Matthew Moore
*/

#ifndef INCLUDE_EFFECT_WEATHERHANDLER_H
#define INCLUDE_EFFECT_WEATHERHANDLER_H

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

	/*! @class WeatherHandler EffectHandler/weatherHandler.h
		@brief Applies the weather damage multiplier for a move.
		@details The handler first scans both parties for a weather-nullifying ability and stores the neutral multiplier when one is
			present. Otherwise it selects the rain or harsh sunlight multiplier based on the move's type, treating Hydro Steam as a
			sunlight-boosted move.
		@note The handler inspects both parties rather than only the active slots, and does not use the registry provider.
		@date 08/04/2026
		@since 0.9.8
		@version 0.9.10
		@author Matthew Moore
	*/
	class WeatherHandler : public IEffectHandler
	{
		public:
			/*! @brief Stores the weather multiplier that applies to the current move.
				@param[in,out] state The battle state containing the active weather and both parties.
				@param[in,out] context The effect context containing the move data and receiving the weather multiplier.
				@param[in] provider The registry provider, which is not used by this handler.
				@post @p context contains the nullification multiplier when a party member has Air Lock or Cloud Nine, or the weather- and
				 type-specific multiplier otherwise.
				@note The active multipliers remain unchanged when no weather is active or the move's type is unaffected by the weather.
				@since 0.9.8
				@version 0.9.10
			*/
			void apply(BattleState &state, EffectContext &context, ATTR_MAYBE_UNUSED const RegistryProvider &provider) const override;
	};

	/*! @brief Applies the weather damage multiplier to an effect context.
		@param[in,out] state The battle state containing the active weather and both parties.
		@param[in,out] context The effect context containing the move data and receiving the weather multiplier.
		@param[in] provider The registry provider, which is not used by this handler.
		@post @p context contains the nullification multiplier when a party member nullifies weather, or the weather- and type-specific
		 multiplier otherwise.
		@note The active multipliers remain unchanged when the weather does not affect the move.
		@since 0.10.0
		@version 0.10.0
	*/
	inline void applyWeather(BattleState &state, EffectContext &context, const RegistryProvider &provider)
	{
		WeatherHandler{}.apply(state, context, provider);
	}
} // namespace PocketCore::Effect

#endif
