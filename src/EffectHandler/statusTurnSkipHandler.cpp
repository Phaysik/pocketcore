/*! @file statusTurnSkipHandler.cpp
	@brief Contains the status turn skip effect handler implementation
	@date 10/19/2026
	@since 0.10.0
	@version 0.12.51
	@author Matthew Moore
*/

#include "EffectHandler/statusTurnSkipHandler.h"

#include <cassert>

#include "Battle/battleRandomSources.h"
#include "Battle/battleState.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Effect/effectContext.h"
#include "EffectHandler/effectHandlerHelpers.h"
#include "EffectHandler/effectHandlerInterface.h"
#include "Registry/registryProvider.h"

namespace PocketCore::Effect
{
	using PocketCore::Battle::BattleRandomSources;
	using PocketCore::Battle::BattleState;
	using PocketCore::Core::sb;
	using PocketCore::Core::us;
	using PocketCore::Registry::RegistryProvider;

	void StatusTurnSkipHandler::apply(ATTR_MAYBE_UNUSED BattleState &state, ATTR_MAYBE_UNUSED EffectContext &context,
									  ATTR_MAYBE_UNUSED const RegistryProvider &provider,
									  ATTR_MAYBE_UNUSED const BattleRandomSources &randomSources) const
	{}
} // namespace PocketCore::Effect
