/*! @file registryProvider.h
	@brief Provides a registry provider that holds references to all registry objects.
	@date 10/09/2026
	@since 0.8.2
	@version 0.12.51
	@author Matthew Moore
*/

#ifndef INCLUDE_REGISTRY_REGISTRY_PROVIDER_H
#define INCLUDE_REGISTRY_REGISTRY_PROVIDER_H

#include "Registry/learnsetRegistry.h"
#include "Registry/locationRegistry.h"
#include "Registry/pokemonRegistry.h"

#include "abilityRegistry.h"
#include "itemRegistry.h"
#include "moveRegistry.h"
#include "multiplierRegistry.h"
#include "natureRegistry.h"
#include "pokemonRegistry.h"
#include "statusRegistry.h"
#include "terrainRegistry.h"
#include "typeRegistry.h"
#include "weatherRegistry.h"

namespace PocketCore::Registry
{
	using PocketCore::Registry::Ability::AbilityRegistry;
	using PocketCore::Registry::Item::ItemRegistry;
	using PocketCore::Registry::Learnset::LearnsetRegistry;
	using PocketCore::Registry::Location::LocationRegistry;
	using PocketCore::Registry::Move::MoveRegistry;
	using PocketCore::Registry::Multiplier::MultiplierRegistry;
	using PocketCore::Registry::Nature::NatureRegistry;
	using PocketCore::Registry::Pokemon::PokemonRegistry;
	using PocketCore::Registry::Status::StatusRegistry;
	using PocketCore::Registry::Terrain::TerrainRegistry;
	using PocketCore::Registry::Type::TypeRegistry;
	using PocketCore::Registry::Weather::WeatherRegistry;

	/*! @struct RegistryProvider Registry/registryProvider.h
		@brief Aggregates non-owning pointers to all runtime metadata registries.
		@details Provides a lightweight dependency bundle passed into systems that require cross-registry lookup access.
		All pointers are non-owning and must refer to registry instances whose lifetime exceeds the provider usage.
		@warning Dereferencing any null member pointer is undefined behavior.
		@date 10/09/2026
		@since 0.8.2
		@version 0.12.51
		@author Matthew Moore
	*/
	struct RegistryProvider
	{
		public:
			/*! @brief Non-owning pointer to the ability metadata registry.
				@details Must point to a valid @ref AbilityRegistry instance for ability metadata queries.
			*/
			const AbilityRegistry *mAbilityRegistry{nullptr};

			/*! @brief Non-owning pointer to the move metadata registry.
				@details Must point to a valid @ref MoveRegistry instance for move metadata queries.
			*/
			const MoveRegistry *mMoveRegistry{nullptr};

			/*! @brief Non-owning pointer to the item metadata registry.
				@details Must point to a valid @ref ItemRegistry instance for item metadata queries.
			*/
			const ItemRegistry *mItemRegistry{nullptr};

			/*! @brief Non-owning pointer to the type metadata registry.
				@details Must point to a valid @ref TypeRegistry instance for type metadata queries.
			*/
			const TypeRegistry *mTypeRegistry{nullptr};

			/*! @brief Non-owning pointer to the status metadata registry.
				@details Must point to a valid @ref StatusRegistry instance for status metadata queries.
			*/
			const StatusRegistry *mStatusRegistry{nullptr};

			/*! @brief Non-owning pointer to the weather metadata registry.
				@details Must point to a valid @ref WeatherRegistry instance for weather metadata queries.
			*/
			const WeatherRegistry *mWeatherRegistry{nullptr};

			/*! @brief Non-owning pointer to the terrain metadata registry.
				@details Must point to a valid @ref TerrainRegistry instance for terrain metadata queries.
			*/
			const TerrainRegistry *mTerrainRegistry{nullptr};

			/*! @brief Non-owning pointer to the multiplier metadata registry.
				@details Must point to a valid @ref MultiplierRegistry instance for multiplier metadata queries.
			*/
			const MultiplierRegistry *mMultiplierRegistry{nullptr};

			/*! @brief Non-owning pointer to the nature metadata registry.
				@details Must point to a valid @ref NatureRegistry instance for nature metadata queries.
			*/
			const NatureRegistry *mNatureRegistry{nullptr};

			/*! @brief Non-owning pointer to the pokemon metadata registry.
				@details Must point to a valid @ref PokemonRegistry instance for pokemon metadata queries.
			*/
			const PokemonRegistry *mPokemonRegistry{nullptr};

			/*! @brief Non-owning pointer to the learnset metadata registry.
				@details Must point to a valid @ref LearnsetRegistry instance for learnset metadata queries.
			*/
			const LearnsetRegistry *mLearnsetRegistry{nullptr};

			/*! @brief Non-owning pointer to the location metadata registry.
				@details Must point to a valid @ref LocationRegistry instance for location metadata queries.
			*/
			const LocationRegistry *mLocationRegistry{nullptr};
	};
} // namespace PocketCore::Registry

#endif
