/*! @file registryProivder.testHelper.h
	@brief Test helper for dealing with RegistryProvider concepts.
	@date 09/17/2026
	@since 0.12.17
	@version 0.12.37
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

	inline RegistryProvider getNullAbilityRegistryProvider()
	{
		static const TypeRegistryConfiguration typeRegistryConfig{};
		static const ItemRegistryConfiguration itemRegistryConfig{};
		static const MoveRegistryConfiguration moveRegistryConfig{};
		static const StatusRegistryConfiguration statusRegistryConfig{};
		static const TerrainRegistryConfiguration terrainRegistryConfig{};
		static const WeatherRegistryConfiguration weatherRegistryConfig{};
		static const MultiplierRegistryConfiguration multiplierRegistryConfig{};
		static const NatureRegistryConfiguration natureRegistryConfig{};
		static const PokemonRegistryConfiguration pokemonRegistryConfig{};

		return {
			.abilityRegistry = nullptr,
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

	inline RegistryProvider getNullMoveRegistryProvider()
	{
		static const TypeRegistryConfiguration typeRegistryConfig{};
		static const ItemRegistryConfiguration itemRegistryConfig{};
		static const AbilityRegistryConfiguration abilityRegistryConfig{};
		static const StatusRegistryConfiguration statusRegistryConfig{};
		static const TerrainRegistryConfiguration terrainRegistryConfig{};
		static const WeatherRegistryConfiguration weatherRegistryConfig{};
		static const MultiplierRegistryConfiguration multiplierRegistryConfig{};
		static const NatureRegistryConfiguration natureRegistryConfig{};
		static const PokemonRegistryConfiguration pokemonRegistryConfig{};

		return {
			.abilityRegistry = &abilityRegistryConfig.getRuntimeRegistry(),
			.moveRegistry = nullptr,
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

	inline RegistryProvider getNullItemRegistryProvider()
	{
		static const TypeRegistryConfiguration typeRegistryConfig{};
		static const MoveRegistryConfiguration moveRegistryConfig{};
		static const AbilityRegistryConfiguration abilityRegistryConfig{};
		static const StatusRegistryConfiguration statusRegistryConfig{};
		static const TerrainRegistryConfiguration terrainRegistryConfig{};
		static const WeatherRegistryConfiguration weatherRegistryConfig{};
		static const MultiplierRegistryConfiguration multiplierRegistryConfig{};
		static const NatureRegistryConfiguration natureRegistryConfig{};
		static const PokemonRegistryConfiguration pokemonRegistryConfig{};

		return {
			.abilityRegistry = &abilityRegistryConfig.getRuntimeRegistry(),
			.moveRegistry = &moveRegistryConfig.getRuntimeRegistry(),
			.itemRegistry = nullptr,
			.typeRegistry = &typeRegistryConfig.getRuntimeRegistry(),
			.statusRegistry = &statusRegistryConfig.getRuntimeRegistry(),
			.weatherRegistry = &weatherRegistryConfig.getRuntimeRegistry(),
			.terrainRegistry = &terrainRegistryConfig.getRuntimeRegistry(),
			.multiplierRegistry = &multiplierRegistryConfig.getRuntimeRegistry(),
			.natureRegistry = &natureRegistryConfig.getRuntimeRegistry(),
			.pokemonRegistry = &pokemonRegistryConfig.getRuntimeRegistry(),
		};
	}

	inline RegistryProvider getNullTypeRegistryProvider()
	{
		static const ItemRegistryConfiguration itemRegistryConfig{};
		static const MoveRegistryConfiguration moveRegistryConfig{};
		static const AbilityRegistryConfiguration abilityRegistryConfig{};
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
			.typeRegistry = nullptr,
			.statusRegistry = &statusRegistryConfig.getRuntimeRegistry(),
			.weatherRegistry = &weatherRegistryConfig.getRuntimeRegistry(),
			.terrainRegistry = &terrainRegistryConfig.getRuntimeRegistry(),
			.multiplierRegistry = &multiplierRegistryConfig.getRuntimeRegistry(),
			.natureRegistry = &natureRegistryConfig.getRuntimeRegistry(),
			.pokemonRegistry = &pokemonRegistryConfig.getRuntimeRegistry(),
		};
	}

	inline RegistryProvider getNullStatusRegistryProvider()
	{
		static const TypeRegistryConfiguration typeRegistryConfig{};
		static const ItemRegistryConfiguration itemRegistryConfig{};
		static const MoveRegistryConfiguration moveRegistryConfig{};
		static const AbilityRegistryConfiguration abilityRegistryConfig{};
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
			.statusRegistry = nullptr,
			.weatherRegistry = &weatherRegistryConfig.getRuntimeRegistry(),
			.terrainRegistry = &terrainRegistryConfig.getRuntimeRegistry(),
			.multiplierRegistry = &multiplierRegistryConfig.getRuntimeRegistry(),
			.natureRegistry = &natureRegistryConfig.getRuntimeRegistry(),
			.pokemonRegistry = &pokemonRegistryConfig.getRuntimeRegistry(),
		};
	}

	inline RegistryProvider getNullWeatherRegistryProvider()
	{
		static const TypeRegistryConfiguration typeRegistryConfig{};
		static const ItemRegistryConfiguration itemRegistryConfig{};
		static const MoveRegistryConfiguration moveRegistryConfig{};
		static const AbilityRegistryConfiguration abilityRegistryConfig{};
		static const StatusRegistryConfiguration statusRegistryConfig{};
		static const TerrainRegistryConfiguration terrainRegistryConfig{};
		static const MultiplierRegistryConfiguration multiplierRegistryConfig{};
		static const NatureRegistryConfiguration natureRegistryConfig{};
		static const PokemonRegistryConfiguration pokemonRegistryConfig{};

		return {
			.abilityRegistry = &abilityRegistryConfig.getRuntimeRegistry(),
			.moveRegistry = &moveRegistryConfig.getRuntimeRegistry(),
			.itemRegistry = &itemRegistryConfig.getRuntimeRegistry(),
			.typeRegistry = &typeRegistryConfig.getRuntimeRegistry(),
			.statusRegistry = &statusRegistryConfig.getRuntimeRegistry(),
			.weatherRegistry = nullptr,
			.terrainRegistry = &terrainRegistryConfig.getRuntimeRegistry(),
			.multiplierRegistry = &multiplierRegistryConfig.getRuntimeRegistry(),
			.natureRegistry = &natureRegistryConfig.getRuntimeRegistry(),
			.pokemonRegistry = &pokemonRegistryConfig.getRuntimeRegistry(),
		};
	}

	inline RegistryProvider getNullTerrainRegistryProvider()
	{
		static const TypeRegistryConfiguration typeRegistryConfig{};
		static const ItemRegistryConfiguration itemRegistryConfig{};
		static const MoveRegistryConfiguration moveRegistryConfig{};
		static const AbilityRegistryConfiguration abilityRegistryConfig{};
		static const StatusRegistryConfiguration statusRegistryConfig{};
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
			.terrainRegistry = nullptr,
			.multiplierRegistry = &multiplierRegistryConfig.getRuntimeRegistry(),
			.natureRegistry = &natureRegistryConfig.getRuntimeRegistry(),
			.pokemonRegistry = &pokemonRegistryConfig.getRuntimeRegistry(),
		};
	}

	inline RegistryProvider getNullMultiplierRegistryProvider()
	{
		static const TypeRegistryConfiguration typeRegistryConfig{};
		static const ItemRegistryConfiguration itemRegistryConfig{};
		static const MoveRegistryConfiguration moveRegistryConfig{};
		static const AbilityRegistryConfiguration abilityRegistryConfig{};
		static const StatusRegistryConfiguration statusRegistryConfig{};
		static const TerrainRegistryConfiguration terrainRegistryConfig{};
		static const WeatherRegistryConfiguration weatherRegistryConfig{};
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
			.multiplierRegistry = nullptr,
			.natureRegistry = &natureRegistryConfig.getRuntimeRegistry(),
			.pokemonRegistry = &pokemonRegistryConfig.getRuntimeRegistry(),
		};
	}

	inline RegistryProvider getNullNatureRegistryProvider()
	{
		static const TypeRegistryConfiguration typeRegistryConfig{};
		static const ItemRegistryConfiguration itemRegistryConfig{};
		static const MoveRegistryConfiguration moveRegistryConfig{};
		static const AbilityRegistryConfiguration abilityRegistryConfig{};
		static const StatusRegistryConfiguration statusRegistryConfig{};
		static const TerrainRegistryConfiguration terrainRegistryConfig{};
		static const WeatherRegistryConfiguration weatherRegistryConfig{};
		static const MultiplierRegistryConfiguration multiplierRegistryConfig{};
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
			.natureRegistry = nullptr,
			.pokemonRegistry = &pokemonRegistryConfig.getRuntimeRegistry(),
		};
	}

	inline RegistryProvider getNullPokemonRegistryProvider()
	{
		static const TypeRegistryConfiguration typeRegistryConfig{};
		static const ItemRegistryConfiguration itemRegistryConfig{};
		static const MoveRegistryConfiguration moveRegistryConfig{};
		static const AbilityRegistryConfiguration abilityRegistryConfig{};
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
			.pokemonRegistry = nullptr,
		};
	}
} // namespace PocketCore::Testing

#endif
