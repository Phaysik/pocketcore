/*! @file criticalHitHandler.h
	@brief Contains the base damage effect handler
	@date 07/24/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_EFFECT_CRITICALHITHANDLER_H
#define INCLUDE_EFFECT_CRITICALHITHANDLER_H

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

	class CriticalHitHandler : public IEffectHandler
	{
		public:
			void apply(ATTR_MAYBE_UNUSED BattleState &state, EffectContext &context,
					   ATTR_MAYBE_UNUSED const RegistryProvider &provider) const override;
	};

	inline void applyCriticalHit(BattleState &state, EffectContext &context, const RegistryProvider &provider)
	{
		CriticalHitHandler{}.apply(state, context, provider);
	}
} // namespace PocketCore::Effect

#endif
