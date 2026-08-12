/*! @file statusRemoveHandler.h
	@brief Contains the statusRemove effect handler
	@date 07/24/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_EFFECT_STATUSREMOVEHANDLER_H
#define INCLUDE_EFFECT_STATUSREMOVEHANDLER_H

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

	class StatusRemoveHandler : public IEffectHandler
	{
		public:
			void apply(BattleState &state, EffectContext &context, ATTR_MAYBE_UNUSED const RegistryProvider &provider) const override;
	};

	inline void applyStatusRemove(BattleState &state, EffectContext &context, const RegistryProvider &provider)
	{
		StatusRemoveHandler{}.apply(state, context, provider);
	}
} // namespace PocketCore::Effect

#endif
