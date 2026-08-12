/*! @file stabHandler.h
	@brief Contains the same type attack bonus damage effect handler
	@date 07/24/2026
	@version x.x.x
	@since x.x.x
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

	class StabHandler : public IEffectHandler
	{
		public:
			void apply(BattleState &state, EffectContext &context, ATTR_MAYBE_UNUSED const RegistryProvider &provider) const override;
	};

	inline void applySTAB(BattleState &state, EffectContext &context, const RegistryProvider &provider)
	{
		StabHandler{}.apply(state, context, provider);
	}
} // namespace PocketCore::Effect

#endif
