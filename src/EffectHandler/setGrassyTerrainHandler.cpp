/*! @file setGrassyTerrain.cpp
	@brief Contains the set grassy terrain effect handler implementation
	@date 10/08/2026
	@since 0.12.43
	@version 0.12.50
	@author Matthew Moore
*/

#include "EffectHandler/setGrassyTerrainHandler.h"

#include <cassert>
#include <expected>
#include <optional>

#include "Battle/battleState.h"
#include "Core/attributeMacros.h"
#include "Effect/effectContext.h"
#include "EffectHandler/effectHandlerHelpers.h"
#include "EffectHandler/effectHandlerInterface.h"
#include "Interaction/interactionApplicationError.h"
#include "Interaction/interactionHelpers.h"
#include "Registry/registryProvider.h"
#include "Terrain/builtInTerrainID.h"
#include "Terrain/terrainID.h"
#include "Terrain/terrainMeta.h"

namespace PocketCore::Effect
{
	using PocketCore::Battle::BattleState;
	using PocketCore::Interaction::applyInteractions;
	using PocketCore::Interaction::InteractionApplicationError;
	using PocketCore::Registry::RegistryProvider;
	using Terrain::BuiltinTerrainID;
	using Terrain::NO_TERRAIN_ID;
	using Terrain::TerrainMeta;
	using Terrain::toTerrainID;

	void SetGrassyTerrainHandler::apply(BattleState &state, ATTR_MAYBE_UNUSED EffectContext &context,
										const RegistryProvider &provider) const
	{
		const std::expected<void, InteractionApplicationError> result{
			applyInteractions(toTerrainID(BuiltinTerrainID::Grassy), NO_TERRAIN_ID, *provider.mTerrainRegistry, state.mTerrainIDs,
							  &TerrainMeta::mTerrainInteractions, state.mRuleset.mMaxTerrains, state.mRuleset.mReplaceTerrainWhenFull),
		};

		context.mInteractionOutcome = {
			.mError = result ? std::nullopt : std::optional{result.error()},
			.mApplied = result.has_value(),
		};
	}
} // namespace PocketCore::Effect
