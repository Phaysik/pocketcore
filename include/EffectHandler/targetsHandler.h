/*! @file targetsHandler.h
	@brief Contains the targets effect handler
	@date 07/24/2026
	@version x.x.x
	@since x.x.x
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

	class TargetsHandler : public IEffectHandler
	{
		public:
			void apply(ATTR_MAYBE_UNUSED BattleState &state, EffectContext &context, const RegistryProvider &provider) const override;
	};

	inline void applyTargets(BattleState &state, EffectContext &context, const RegistryProvider &provider)
	{
		TargetsHandler{}.apply(state, context, provider);
	}
} // namespace PocketCore::Effect

#endif
