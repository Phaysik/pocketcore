/*! @file flinchHandler.h
	@brief Contains the flinch effect handler
	@date 07/24/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_EFFECT_FLINCHHANDLER_H
#define INCLUDE_EFFECT_FLINCHHANDLER_H

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

	class FlinchHandler : public IEffectHandler
	{
		public:
			void apply(BattleState &state, EffectContext &context, ATTR_MAYBE_UNUSED const RegistryProvider &provider) const override;
	};

	inline void applyFlinch(BattleState &state, EffectContext &context, const RegistryProvider &provider)
	{
		FlinchHandler{}.apply(state, context, provider);
	}
} // namespace PocketCore::Effect

#endif
