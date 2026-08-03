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
#include "typeEffectivenessRegistry.h"
#include "typeRegistry.h"
#include "weatherRegistry.h"

namespace PocketCore::Registry
{
	/*! @struct RegistryProvider Registry/registryProvider.h
		@brief Aggregates non-owning pointers to all runtime metadata registries.
		@details Provides a lightweight dependency bundle passed into systems that require cross-registry lookup access.
		All pointers are non-owning and must refer to registry instances whose lifetime exceeds the provider usage.
		@warning Dereferencing any null member pointer is undefined behavior.
		@date 08/03/2026
		@version x.x.x
		@since x.x.x
		@author Matthew Moore
	*/
	struct RegistryProvider
	{
		public:
			/*! @brief Non-owning pointer to the ability metadata registry.
				@details Must point to a valid @ref Ability::AbilityRegistry instance for ability metadata queries.
			*/
			const Ability::AbilityRegistry *abilityRegistry;

			/*! @brief Non-owning pointer to the move metadata registry.
				@details Must point to a valid @ref Move::MoveRegistry instance for move metadata queries.
			*/
			const Move::MoveRegistry *moveRegistry;

			/*! @brief Non-owning pointer to the item metadata registry.
				@details Must point to a valid @ref Item::ItemRegistry instance for item metadata queries.
			*/
			const Item::ItemRegistry *itemRegistry;

			/*! @brief Non-owning pointer to the type metadata registry.
				@details Must point to a valid @ref Types::TypeRegistry instance for type metadata queries.
			*/
			const Types::TypeRegistry *typeRegistry;

			/*! @brief Non-owning pointer to the type-effectiveness metadata registry.
				@details Must point to a valid @ref Types::TypeEffectivenessRegistry instance for type-effectiveness value queries.
			*/
			const Types::TypeEffectivenessRegistry *typeEffectivenessRegistry;

			/*! @brief Non-owning pointer to the status metadata registry.
				@details Must point to a valid @ref Status::StatusRegistry instance for status metadata queries.
			*/
			const Status::StatusRegistry *statusRegistry;

			/*! @brief Non-owning pointer to the weather metadata registry.
				@details Must point to a valid @ref Weather::WeatherRegistry instance for weather metadata queries.
			*/
			const Weather::WeatherRegistry *weatherRegistry;

			/*! @brief Non-owning pointer to the terrain metadata registry.
				@details Must point to a valid @ref Terrain::TerrainRegistry instance for terrain metadata queries.
			*/
			const Terrain::TerrainRegistry *terrainRegistry;

			/*! @brief Non-owning pointer to the multiplier metadata registry.
				@details Must point to a valid @ref Multiplier::MultiplierRegistry instance for multiplier metadata queries.
			*/
			const Multiplier::MultiplierRegistry *multiplierRegistry;
	};
} // namespace PocketCore::Registry

#endif
