/*! @file statusRemoveHandler.cpp
	@brief Contains the status remove effect handler implementation
	@date 08/12/2026
	@since 0.10.0
	@version 0.10.0
	@author Matthew Moore
*/

#include "EffectHandler/statusRemoveHandler.h"

#include <cassert>

#include "Battle/battleState.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Effect/effectContext.h"
#include "EffectHandler/effectHandlerHelpers.h"
#include "EffectHandler/effectHandlerInterface.h"
#include "Registry/registryProvider.h"

namespace PocketCore::Effect
{
	using PocketCore::Battle::BattleState;
	using PocketCore::Core::sb;
	using PocketCore::Core::us;
	using PocketCore::Registry::RegistryProvider;

	void StatusRemoveHandler::apply(ATTR_MAYBE_UNUSED BattleState &state, ATTR_MAYBE_UNUSED EffectContext &context,
									ATTR_MAYBE_UNUSED const RegistryProvider &provider) const
	{}
} // namespace PocketCore::Effect
