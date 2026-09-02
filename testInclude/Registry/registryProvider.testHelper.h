/*! @file registryProivder.testHelper.h
	@brief Test helper for dealing with RegistryProvider concepts.
	@date 09/02/2026
	@since 0.12.17
	@version 0.12.17
	@author Matthew Moore
*/

#ifndef TEST_INCLUDE_REGISTRY_REGISTRY_PROVIDER_TEST_HELPER_H
#define TEST_INCLUDE_REGISTRY_REGISTRY_PROVIDER_TEST_HELPER_H

#include "Configuration/abilityRegistryConfiguration.h"
#include "Configuration/itemRegistryConfiguration.h"
#include "Configuration/moveRegistryConfiguration.h"
#include "Configuration/multiplierRegistryConfiguration.h"
#include "Configuration/natureRegistryConfiguration.h"
#include "Configuration/pokemonRegistryConfiguration.h"
#include "Configuration/statusRegistryConfiguration.h"
#include "Configuration/terrainRegistryConfiguration.h"
#include "Configuration/typeRegistryConfiguration.h"
#include "Configuration/weatherRegistryConfiguration.h"
#include "Registry/effectRegistry.h"
#include "Registry/registryProvider.h"

namespace PocketCore::Testing
{
	using PocketCore::Configuration::AbilityRegistryConfiguration;
	using PocketCore::Configuration::ItemRegistryConfiguration;
	using PocketCore::Configuration::MoveRegistryConfiguration;
	using PocketCore::Configuration::MultiplierRegistryConfiguration;
	using PocketCore::Configuration::NatureRegistryConfiguration;
	using PocketCore::Configuration::PokemonRegistryConfiguration;
	using PocketCore::Configuration::StatusRegistryConfiguration;
	using PocketCore::Configuration::TerrainRegistryConfiguration;
	using PocketCore::Configuration::TypeRegistryConfiguration;
	using PocketCore::Configuration::WeatherRegistryConfiguration;
	using PocketCore::Registry::RegistryProvider;
	using Registry::Effect::EffectRegistry;

	inline RegistryProvider getDefaultInitializedRegistryProvider()
	{
		static const TypeRegistryConfiguration typeRegistryConfig{};
		static const AbilityRegistryConfiguration abilityRegistryConfig{};
		static const ItemRegistryConfiguration itemRegistryConfig{};
		static const MoveRegistryConfiguration moveRegistryConfig{};
		static const StatusRegistryConfiguration statusRegistryConfig{};
		static const TerrainRegistryConfiguration terrainRegistryConfig{};
		static const WeatherRegistryConfiguration weatherRegistryConfig{};
		static const MultiplierRegistryConfiguration multiplierRegistryConfig{};
		static const NatureRegistryConfiguration natureRegistryConfig{};
		static const PokemonRegistryConfiguration pokemonRegistryConfig{};

		return {
			.abilityRegistry = &abilityRegistryConfig.getRuntimeRegistry(),
			.moveRegistry = &moveRegistryConfig.getRuntimeRegistry(),
			.itemRegistry = &itemRegistryConfig.getRuntimeRegistry(),
			.typeRegistry = &typeRegistryConfig.getRuntimeRegistry(),
			.statusRegistry = &statusRegistryConfig.getRuntimeRegistry(),
			.weatherRegistry = &weatherRegistryConfig.getRuntimeRegistry(),
			.terrainRegistry = &terrainRegistryConfig.getRuntimeRegistry(),
			.multiplierRegistry = &multiplierRegistryConfig.getRuntimeRegistry(),
			.natureRegistry = &natureRegistryConfig.getRuntimeRegistry(),
			.pokemonRegistry = &pokemonRegistryConfig.getRuntimeRegistry(),
		};
	}
} // namespace PocketCore::Testing

#endif
