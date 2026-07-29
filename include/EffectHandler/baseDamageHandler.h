/*! @file baseDamageHandler.h
	@brief Contains the base damage effect handler
	@date 07/24/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_EFFECT_BASEDAMAGEHANDLER_H
#define INCLUDE_EFFECT_BASEDAMAGEHANDLER_H

#include "Battle/battleState.h"
#include "Effect/effectContext.h"

#include "effectHandlerInterface.h"

namespace PocketCore::Effect
{
	using PocketCore::Battle::BattleSlot;
	using PocketCore::Battle::BattleState;

	class BaseDamageHandler : public IEffectHandler
	{
		public:
			void apply(const BattleState &state, EffectContext &context) const override;
	};
} // namespace PocketCore::Effect

#endif
