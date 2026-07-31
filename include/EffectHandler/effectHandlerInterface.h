/*! @file effectHandlerInterface.h
	@brief Contains the effect handler interface
	@date 07/24/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_EFFECT_EFFECTHANDLERINTERFACE_H
#define INCLUDE_EFFECT_EFFECTHANDLERINTERFACE_H

#include "Battle/battleState.h"
#include "Effect/effectContext.h"
#include "Registry/registryProvider.h"

namespace PocketCore::Effect
{
	using PocketCore::Registry::RegistryProvider;

	class IEffectHandler
	{
		public:
			IEffectHandler() = default;
			IEffectHandler(const IEffectHandler &) = delete;
			IEffectHandler(IEffectHandler &&) = delete;
			IEffectHandler operator=(const IEffectHandler &) = delete;
			IEffectHandler operator=(IEffectHandler &&) = delete;
			virtual ~IEffectHandler() = default;
			virtual void apply(const PocketCore::Battle::BattleState &state, EffectContext &context, const RegistryProvider &provider) const
				= 0;
	};
} // namespace PocketCore::Effect

#endif
