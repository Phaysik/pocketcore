/*! @file statusTurnSkipHandler.h
	@brief Contains the statusTurnSkip effect handler
	@date 07/24/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_EFFECT_STATUSTURNSKIPHANDLER_H
#define INCLUDE_EFFECT_STATUSTURNSKIPHANDLER_H

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

	class StatusTurnSkipHandler : public IEffectHandler
	{
		public:
			void apply(BattleState &state, EffectContext &context, ATTR_MAYBE_UNUSED const RegistryProvider &provider) const override;
	};

	inline void applyStatusTurnSkip(BattleState &state, EffectContext &context, const RegistryProvider &provider)
	{
		StatusTurnSkipHandler{}.apply(state, context, provider);
	}
} // namespace PocketCore::Effect

#endif
