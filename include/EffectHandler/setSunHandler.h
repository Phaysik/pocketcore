/*! @file setSunHandler.h
	@brief Contains the setSun effect handler
	@date 07/24/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_EFFECT_SETSUNHANDLER_H
#define INCLUDE_EFFECT_SETSUNHANDLER_H

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

	class SetSunHandler : public IEffectHandler
	{
		public:
			void apply(BattleState &state, EffectContext &context, ATTR_MAYBE_UNUSED const RegistryProvider &provider) const override;
	};

	inline void applySetSun(BattleState &state, EffectContext &context, const RegistryProvider &provider)
	{
		SetSunHandler{}.apply(state, context, provider);
	}
} // namespace PocketCore::Effect

#endif
