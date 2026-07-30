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
#include "Configuration/moveRegistryConfiguration.h"
#include "Core/attributeMacros.h"
#include "Effect/effectContext.h"

#include "effectHandlerInterface.h"

namespace PocketCore::Effect
{
	using PocketCore::Battle::BattleSlot;
	using PocketCore::Battle::BattleState;
	using PocketCore::Configuration::MoveRegistryConfiguration;

	class TargetsHandler : public IEffectHandler
	{
		public:
			explicit TargetsHandler(const MoveRegistryConfiguration &moveRegistry) : mMoveRegistry{moveRegistry} {}

			void apply(ATTR_MAYBE_UNUSED const BattleState &state, EffectContext &context) const override;

		private:
			const MoveRegistryConfiguration &mMoveRegistry;
	};
} // namespace PocketCore::Effect

#endif
