/*! @file main.cpp
	@brief Contains the function definitions for creating a main
	@date --/--/----
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#include <cstdlib>
#include <string_view>

#include "Battle/battleEngine.h"
#include "Configuration/abilityRegistryConfiguration.h"
#include "Configuration/configFlags.h"
#include "Configuration/effectRegistryConfiguration.h"
#include "Configuration/itemRegistryConfiguration.h"
#include "Configuration/moveRegistryConfiguration.h"
#include "Configuration/multiplierRegistryConfiguration.h"
#include "Configuration/statusRegistryConfiguration.h"
#include "Configuration/terrainRegistryConfiguration.h"
#include "Configuration/typeRegistryConfiguration.h"
#include "Configuration/weatherRegistryConfiguration.h"
#include "Registry/effectRegistry.h"
#include "Registry/registryProvider.h"
#include "Utility/Debug/Logging/constants.h"
#include "Utility/Debug/Logging/logger.h"

/*! @brief The entry point for the program
	@date --/--/----
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
	@return int The status code of the program
*/
int main()
{
	using PocketCore::Battle::BattleEngine;
	using PocketCore::Registry::Effect::EffectRegistry;
	using PocketCore::Registry::RegistryProvider;
	namespace Configuration = PocketCore::Configuration;
	namespace Logging = PocketCore::Utility::Debug::Logging;

	// NOLINTNEXTLINE(readability-redundant-parentheses)
	if constexpr ((Configuration::configFlags & Configuration::useCustomConfig) != Configuration::bitwiseANDEquals)
	{
		const bool initialized{Logging::Logger::initialize(Logging::LOGGING_LOGGER_NAME, Logging::LOGGING_FILE_NAME, true)};

		if (!initialized)
		{
			return EXIT_FAILURE;
		}

		const Configuration::TypeRegistryConfiguration typeRegistryConfig{};
		const Configuration::AbilityRegistryConfiguration abilityRegistryConfig{};
		const Configuration::ItemRegistryConfiguration itemRegistryConfig{};
		const Configuration::MoveRegistryConfiguration moveRegistryConfig{};
		const Configuration::StatusRegistryConfiguration statusRegistryConfig{};
		const Configuration::TerrainRegistryConfiguration terrainRegistryConfig{};
		const Configuration::WeatherRegistryConfiguration weatherRegistryConfig{};
		const Configuration::MultiplierRegistryConfiguration multiplierRegistryConfig{};
		const Configuration::EffectRegistryConfiguration effectRegistryConfig{};

		const RegistryProvider registryProvider{
			.abilityRegistry = &abilityRegistryConfig.getRuntimeRegistry(),
			.moveRegistry = &moveRegistryConfig.getRuntimeRegistry(),
			.itemRegistry = &itemRegistryConfig.getRuntimeRegistry(),
			.typeRegistry = &typeRegistryConfig.getRuntimeRegistry(),
			.statusRegistry = &statusRegistryConfig.getRuntimeRegistry(),
			.weatherRegistry = &weatherRegistryConfig.getRuntimeRegistry(),
			.terrainRegistry = &terrainRegistryConfig.getRuntimeRegistry(),
			.multiplierRegistry = &multiplierRegistryConfig.getRuntimeRegistry(),
		};
		const EffectRegistry &effectRegistry{effectRegistryConfig.getRuntimeRegistry()};

		BattleEngine engine{registryProvider, effectRegistry};
	}
	else
	{
		// MARK: Non-Constexpr below

		const bool initialized{Logging::Logger::initialize(Logging::LOGGING_LOGGER_NAME, Logging::LOGGING_FILE_NAME, true)};

		if (!initialized)
		{
			return EXIT_FAILURE;
		}

		const Configuration::TypeRegistryConfiguration typeRegistryConfig{};
		const Configuration::AbilityRegistryConfiguration abilityRegistryConfig{};
		const Configuration::ItemRegistryConfiguration itemRegistryConfig{};
		const Configuration::MoveRegistryConfiguration moveRegistryConfig{};
		const Configuration::StatusRegistryConfiguration statusRegistryConfig{};
		const Configuration::TerrainRegistryConfiguration terrainRegistryConfig{};
		const Configuration::WeatherRegistryConfiguration weatherRegistryConfig{};
		const Configuration::MultiplierRegistryConfiguration multiplierRegistryConfig{};
		const Configuration::EffectRegistryConfiguration effectRegistryConfig{};

		const RegistryProvider registryProvider{
			.abilityRegistry = &abilityRegistryConfig.getRuntimeRegistry(),
			.moveRegistry = &moveRegistryConfig.getRuntimeRegistry(),
			.itemRegistry = &itemRegistryConfig.getRuntimeRegistry(),
			.typeRegistry = &typeRegistryConfig.getRuntimeRegistry(),
			.statusRegistry = &statusRegistryConfig.getRuntimeRegistry(),
			.weatherRegistry = &weatherRegistryConfig.getRuntimeRegistry(),
			.terrainRegistry = &terrainRegistryConfig.getRuntimeRegistry(),
			.multiplierRegistry = &multiplierRegistryConfig.getRuntimeRegistry(),
		};
		const EffectRegistry &effectRegistry{effectRegistryConfig.getRuntimeRegistry()};

		BattleEngine engine{registryProvider, effectRegistry};
	}

	return EXIT_SUCCESS;
}
