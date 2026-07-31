/*! @file registryProvider.h
	@brief Provides a registry provider that holds references to all registry objects.
	@date 07/27/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_REGISTRY_REGISTRY_PROVIDER_H
#define INCLUDE_REGISTRY_REGISTRY_PROVIDER_H

#include "abilityRegistry.h"
#include "itemRegistry.h"
#include "moveRegistry.h"
#include "multiplierRegistry.h"
#include "statusRegistry.h"
#include "terrainRegistry.h"
#include "typeRegistry.h"
#include "weatherRegistry.h"

namespace PocketCore::Registry
{
	struct RegistryProvider
	{
		public:
			const Ability::AbilityRegistry *abilityRegistry;
			const Move::MoveRegistry *moveRegistry;
			const Item::ItemRegistry *itemRegistry;
			const Types::TypeRegistry *typeRegistry;
			const Status::StatusRegistry *statusRegistry;
			const Weather::WeatherRegistry *weatherRegistry;
			const Terrain::TerrainRegistry *terrainRegistry;
			const Multiplier::MultiplierRegistry *multiplierRegistry;
	};
} // namespace PocketCore::Registry

#endif
