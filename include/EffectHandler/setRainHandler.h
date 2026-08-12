/*! @file setRainHandler.h
	@brief Contains the setRain effect handler
	@date 07/24/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_EFFECT_SETRAINHANDLER_H
#define INCLUDE_EFFECT_SETRAINHANDLER_H

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

	class SetRainHandler : public IEffectHandler
	{
		public:
			void apply(BattleState &state, EffectContext &context, ATTR_MAYBE_UNUSED const RegistryProvider &provider) const override;
	};

	inline void applySetRain(BattleState &state, EffectContext &context, const RegistryProvider &provider)
	{
		SetRainHandler{}.apply(state, context, provider);
	}
} // namespace PocketCore::Effect

#endif
