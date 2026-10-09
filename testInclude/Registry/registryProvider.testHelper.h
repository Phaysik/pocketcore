/*! @file registryProivder.testHelper.h
	@brief Test helper for dealing with RegistryProvider concepts.
	@date 10/09/2026
	@since 0.12.17
	@version 0.12.51
	@author Matthew Moore
*/

#ifndef TEST_INCLUDE_REGISTRY_REGISTRY_PROVIDER_TEST_HELPER_H
#define TEST_INCLUDE_REGISTRY_REGISTRY_PROVIDER_TEST_HELPER_H

#include "Configuration/abilityRegistryConfiguration.h"
#include "Configuration/itemRegistryConfiguration.h"
#include "Configuration/learnsetRegistryConfiguration.h"
#include "Configuration/locationRegistryConfiguration.h"
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
	using PocketCore::Configuration::LearnsetRegistryConfiguration;
	using PocketCore::Configuration::LocationRegistryConfiguration;
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
		static const LearnsetRegistryConfiguration learnsetRegistryConfig{};
		static const LocationRegistryConfiguration locationRegistryConfig{};

		return {
			.mAbilityRegistry = &abilityRegistryConfig.getRuntimeRegistry(),
			.mMoveRegistry = &moveRegistryConfig.getRuntimeRegistry(),
			.mItemRegistry = &itemRegistryConfig.getRuntimeRegistry(),
			.mTypeRegistry = &typeRegistryConfig.getRuntimeRegistry(),
			.mStatusRegistry = &statusRegistryConfig.getRuntimeRegistry(),
			.mWeatherRegistry = &weatherRegistryConfig.getRuntimeRegistry(),
			.mTerrainRegistry = &terrainRegistryConfig.getRuntimeRegistry(),
			.mMultiplierRegistry = &multiplierRegistryConfig.getRuntimeRegistry(),
			.mNatureRegistry = &natureRegistryConfig.getRuntimeRegistry(),
			.mPokemonRegistry = &pokemonRegistryConfig.getRuntimeRegistry(),
			.mLearnsetRegistry = &learnsetRegistryConfig.getRuntimeRegistry(),
			.mLocationRegistry = &locationRegistryConfig.getRuntimeRegistry(),
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
		static const LearnsetRegistryConfiguration learnsetRegistryConfig{};
		static const LocationRegistryConfiguration locationRegistryConfig{};

		return {
			.mAbilityRegistry = nullptr,
			.mMoveRegistry = &moveRegistryConfig.getRuntimeRegistry(),
			.mItemRegistry = &itemRegistryConfig.getRuntimeRegistry(),
			.mTypeRegistry = &typeRegistryConfig.getRuntimeRegistry(),
			.mStatusRegistry = &statusRegistryConfig.getRuntimeRegistry(),
			.mWeatherRegistry = &weatherRegistryConfig.getRuntimeRegistry(),
			.mTerrainRegistry = &terrainRegistryConfig.getRuntimeRegistry(),
			.mMultiplierRegistry = &multiplierRegistryConfig.getRuntimeRegistry(),
			.mNatureRegistry = &natureRegistryConfig.getRuntimeRegistry(),
			.mPokemonRegistry = &pokemonRegistryConfig.getRuntimeRegistry(),
			.mLearnsetRegistry = &learnsetRegistryConfig.getRuntimeRegistry(),
			.mLocationRegistry = &locationRegistryConfig.getRuntimeRegistry(),
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
		static const LearnsetRegistryConfiguration learnsetRegistryConfig{};
		static const LocationRegistryConfiguration locationRegistryConfig{};

		return {
			.mAbilityRegistry = &abilityRegistryConfig.getRuntimeRegistry(),
			.mMoveRegistry = nullptr,
			.mItemRegistry = &itemRegistryConfig.getRuntimeRegistry(),
			.mTypeRegistry = &typeRegistryConfig.getRuntimeRegistry(),
			.mStatusRegistry = &statusRegistryConfig.getRuntimeRegistry(),
			.mWeatherRegistry = &weatherRegistryConfig.getRuntimeRegistry(),
			.mTerrainRegistry = &terrainRegistryConfig.getRuntimeRegistry(),
			.mMultiplierRegistry = &multiplierRegistryConfig.getRuntimeRegistry(),
			.mNatureRegistry = &natureRegistryConfig.getRuntimeRegistry(),
			.mPokemonRegistry = &pokemonRegistryConfig.getRuntimeRegistry(),
			.mLearnsetRegistry = &learnsetRegistryConfig.getRuntimeRegistry(),
			.mLocationRegistry = &locationRegistryConfig.getRuntimeRegistry(),
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
		static const LearnsetRegistryConfiguration learnsetRegistryConfig{};
		static const LocationRegistryConfiguration locationRegistryConfig{};

		return {
			.mAbilityRegistry = &abilityRegistryConfig.getRuntimeRegistry(),
			.mMoveRegistry = &moveRegistryConfig.getRuntimeRegistry(),
			.mItemRegistry = nullptr,
			.mTypeRegistry = &typeRegistryConfig.getRuntimeRegistry(),
			.mStatusRegistry = &statusRegistryConfig.getRuntimeRegistry(),
			.mWeatherRegistry = &weatherRegistryConfig.getRuntimeRegistry(),
			.mTerrainRegistry = &terrainRegistryConfig.getRuntimeRegistry(),
			.mMultiplierRegistry = &multiplierRegistryConfig.getRuntimeRegistry(),
			.mNatureRegistry = &natureRegistryConfig.getRuntimeRegistry(),
			.mPokemonRegistry = &pokemonRegistryConfig.getRuntimeRegistry(),
			.mLearnsetRegistry = &learnsetRegistryConfig.getRuntimeRegistry(),
			.mLocationRegistry = &locationRegistryConfig.getRuntimeRegistry(),
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
		static const LearnsetRegistryConfiguration learnsetRegistryConfig{};
		static const LocationRegistryConfiguration locationRegistryConfig{};

		return {
			.mAbilityRegistry = &abilityRegistryConfig.getRuntimeRegistry(),
			.mMoveRegistry = &moveRegistryConfig.getRuntimeRegistry(),
			.mItemRegistry = &itemRegistryConfig.getRuntimeRegistry(),
			.mTypeRegistry = nullptr,
			.mStatusRegistry = &statusRegistryConfig.getRuntimeRegistry(),
			.mWeatherRegistry = &weatherRegistryConfig.getRuntimeRegistry(),
			.mTerrainRegistry = &terrainRegistryConfig.getRuntimeRegistry(),
			.mMultiplierRegistry = &multiplierRegistryConfig.getRuntimeRegistry(),
			.mNatureRegistry = &natureRegistryConfig.getRuntimeRegistry(),
			.mPokemonRegistry = &pokemonRegistryConfig.getRuntimeRegistry(),
			.mLearnsetRegistry = &learnsetRegistryConfig.getRuntimeRegistry(),
			.mLocationRegistry = &locationRegistryConfig.getRuntimeRegistry(),
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
		static const LearnsetRegistryConfiguration learnsetRegistryConfig{};
		static const LocationRegistryConfiguration locationRegistryConfig{};

		return {
			.mAbilityRegistry = &abilityRegistryConfig.getRuntimeRegistry(),
			.mMoveRegistry = &moveRegistryConfig.getRuntimeRegistry(),
			.mItemRegistry = &itemRegistryConfig.getRuntimeRegistry(),
			.mTypeRegistry = &typeRegistryConfig.getRuntimeRegistry(),
			.mStatusRegistry = nullptr,
			.mWeatherRegistry = &weatherRegistryConfig.getRuntimeRegistry(),
			.mTerrainRegistry = &terrainRegistryConfig.getRuntimeRegistry(),
			.mMultiplierRegistry = &multiplierRegistryConfig.getRuntimeRegistry(),
			.mNatureRegistry = &natureRegistryConfig.getRuntimeRegistry(),
			.mPokemonRegistry = &pokemonRegistryConfig.getRuntimeRegistry(),
			.mLearnsetRegistry = &learnsetRegistryConfig.getRuntimeRegistry(),
			.mLocationRegistry = &locationRegistryConfig.getRuntimeRegistry(),
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
		static const LearnsetRegistryConfiguration learnsetRegistryConfig{};
		static const LocationRegistryConfiguration locationRegistryConfig{};

		return {
			.mAbilityRegistry = &abilityRegistryConfig.getRuntimeRegistry(),
			.mMoveRegistry = &moveRegistryConfig.getRuntimeRegistry(),
			.mItemRegistry = &itemRegistryConfig.getRuntimeRegistry(),
			.mTypeRegistry = &typeRegistryConfig.getRuntimeRegistry(),
			.mStatusRegistry = &statusRegistryConfig.getRuntimeRegistry(),
			.mWeatherRegistry = nullptr,
			.mTerrainRegistry = &terrainRegistryConfig.getRuntimeRegistry(),
			.mMultiplierRegistry = &multiplierRegistryConfig.getRuntimeRegistry(),
			.mNatureRegistry = &natureRegistryConfig.getRuntimeRegistry(),
			.mPokemonRegistry = &pokemonRegistryConfig.getRuntimeRegistry(),
			.mLearnsetRegistry = &learnsetRegistryConfig.getRuntimeRegistry(),
			.mLocationRegistry = &locationRegistryConfig.getRuntimeRegistry(),
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
		static const LearnsetRegistryConfiguration learnsetRegistryConfig{};
		static const LocationRegistryConfiguration locationRegistryConfig{};

		return {
			.mAbilityRegistry = &abilityRegistryConfig.getRuntimeRegistry(),
			.mMoveRegistry = &moveRegistryConfig.getRuntimeRegistry(),
			.mItemRegistry = &itemRegistryConfig.getRuntimeRegistry(),
			.mTypeRegistry = &typeRegistryConfig.getRuntimeRegistry(),
			.mStatusRegistry = &statusRegistryConfig.getRuntimeRegistry(),
			.mWeatherRegistry = &weatherRegistryConfig.getRuntimeRegistry(),
			.mTerrainRegistry = nullptr,
			.mMultiplierRegistry = &multiplierRegistryConfig.getRuntimeRegistry(),
			.mNatureRegistry = &natureRegistryConfig.getRuntimeRegistry(),
			.mPokemonRegistry = &pokemonRegistryConfig.getRuntimeRegistry(),
			.mLearnsetRegistry = &learnsetRegistryConfig.getRuntimeRegistry(),
			.mLocationRegistry = &locationRegistryConfig.getRuntimeRegistry(),
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
		static const LearnsetRegistryConfiguration learnsetRegistryConfig{};
		static const LocationRegistryConfiguration locationRegistryConfig{};

		return {
			.mAbilityRegistry = &abilityRegistryConfig.getRuntimeRegistry(),
			.mMoveRegistry = &moveRegistryConfig.getRuntimeRegistry(),
			.mItemRegistry = &itemRegistryConfig.getRuntimeRegistry(),
			.mTypeRegistry = &typeRegistryConfig.getRuntimeRegistry(),
			.mStatusRegistry = &statusRegistryConfig.getRuntimeRegistry(),
			.mWeatherRegistry = &weatherRegistryConfig.getRuntimeRegistry(),
			.mTerrainRegistry = &terrainRegistryConfig.getRuntimeRegistry(),
			.mMultiplierRegistry = nullptr,
			.mNatureRegistry = &natureRegistryConfig.getRuntimeRegistry(),
			.mPokemonRegistry = &pokemonRegistryConfig.getRuntimeRegistry(),
			.mLearnsetRegistry = &learnsetRegistryConfig.getRuntimeRegistry(),
			.mLocationRegistry = &locationRegistryConfig.getRuntimeRegistry(),
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
		static const LearnsetRegistryConfiguration learnsetRegistryConfig{};
		static const LocationRegistryConfiguration locationRegistryConfig{};

		return {
			.mAbilityRegistry = &abilityRegistryConfig.getRuntimeRegistry(),
			.mMoveRegistry = &moveRegistryConfig.getRuntimeRegistry(),
			.mItemRegistry = &itemRegistryConfig.getRuntimeRegistry(),
			.mTypeRegistry = &typeRegistryConfig.getRuntimeRegistry(),
			.mStatusRegistry = &statusRegistryConfig.getRuntimeRegistry(),
			.mWeatherRegistry = &weatherRegistryConfig.getRuntimeRegistry(),
			.mTerrainRegistry = &terrainRegistryConfig.getRuntimeRegistry(),
			.mMultiplierRegistry = &multiplierRegistryConfig.getRuntimeRegistry(),
			.mNatureRegistry = nullptr,
			.mPokemonRegistry = &pokemonRegistryConfig.getRuntimeRegistry(),
			.mLearnsetRegistry = &learnsetRegistryConfig.getRuntimeRegistry(),
			.mLocationRegistry = &locationRegistryConfig.getRuntimeRegistry(),
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
		static const LearnsetRegistryConfiguration learnsetRegistryConfig{};
		static const LocationRegistryConfiguration locationRegistryConfig{};

		return {
			.mAbilityRegistry = &abilityRegistryConfig.getRuntimeRegistry(),
			.mMoveRegistry = &moveRegistryConfig.getRuntimeRegistry(),
			.mItemRegistry = &itemRegistryConfig.getRuntimeRegistry(),
			.mTypeRegistry = &typeRegistryConfig.getRuntimeRegistry(),
			.mStatusRegistry = &statusRegistryConfig.getRuntimeRegistry(),
			.mWeatherRegistry = &weatherRegistryConfig.getRuntimeRegistry(),
			.mTerrainRegistry = &terrainRegistryConfig.getRuntimeRegistry(),
			.mMultiplierRegistry = &multiplierRegistryConfig.getRuntimeRegistry(),
			.mNatureRegistry = &natureRegistryConfig.getRuntimeRegistry(),
			.mPokemonRegistry = nullptr,
			.mLearnsetRegistry = &learnsetRegistryConfig.getRuntimeRegistry(),
			.mLocationRegistry = &locationRegistryConfig.getRuntimeRegistry(),
		};
	}

	inline RegistryProvider getNullLearnsetRegistryProvider()
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
		static const LearnsetRegistryConfiguration learnsetRegistryConfig{};
		static const LocationRegistryConfiguration locationRegistryConfig{};

		return {
			.mAbilityRegistry = &abilityRegistryConfig.getRuntimeRegistry(),
			.mMoveRegistry = &moveRegistryConfig.getRuntimeRegistry(),
			.mItemRegistry = &itemRegistryConfig.getRuntimeRegistry(),
			.mTypeRegistry = &typeRegistryConfig.getRuntimeRegistry(),
			.mStatusRegistry = &statusRegistryConfig.getRuntimeRegistry(),
			.mWeatherRegistry = &weatherRegistryConfig.getRuntimeRegistry(),
			.mTerrainRegistry = &terrainRegistryConfig.getRuntimeRegistry(),
			.mMultiplierRegistry = &multiplierRegistryConfig.getRuntimeRegistry(),
			.mNatureRegistry = &natureRegistryConfig.getRuntimeRegistry(),
			.mPokemonRegistry = &pokemonRegistryConfig.getRuntimeRegistry(),
			.mLearnsetRegistry = nullptr,
			.mLocationRegistry = &locationRegistryConfig.getRuntimeRegistry(),
		};
	}

	inline RegistryProvider getNullLocationRegistryProvider()
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
		static const LearnsetRegistryConfiguration learnsetRegistryConfig{};
		static const LocationRegistryConfiguration locationRegistryConfig{};

		return {
			.mAbilityRegistry = &abilityRegistryConfig.getRuntimeRegistry(),
			.mMoveRegistry = &moveRegistryConfig.getRuntimeRegistry(),
			.mItemRegistry = &itemRegistryConfig.getRuntimeRegistry(),
			.mTypeRegistry = &typeRegistryConfig.getRuntimeRegistry(),
			.mStatusRegistry = &statusRegistryConfig.getRuntimeRegistry(),
			.mWeatherRegistry = &weatherRegistryConfig.getRuntimeRegistry(),
			.mTerrainRegistry = &terrainRegistryConfig.getRuntimeRegistry(),
			.mMultiplierRegistry = &multiplierRegistryConfig.getRuntimeRegistry(),
			.mNatureRegistry = &natureRegistryConfig.getRuntimeRegistry(),
			.mPokemonRegistry = &pokemonRegistryConfig.getRuntimeRegistry(),
			.mLearnsetRegistry = &learnsetRegistryConfig.getRuntimeRegistry(),
			.mLocationRegistry = nullptr,
		};
	}
} // namespace PocketCore::Testing

#endif
