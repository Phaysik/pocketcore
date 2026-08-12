/*! @file main.cpp
	@brief Contains the function definitions for creating a main
	@date --/--/----
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#include <cstdlib>
#include <string_view>

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
#include "Core/attributeMacros.h"
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
	}
	else
	{
		// MARK: Non-Constexpr below

		const bool initialized{Logging::Logger::initialize(Logging::LOGGING_LOGGER_NAME, Logging::LOGGING_FILE_NAME, true)};

		if (!initialized)
		{
			return EXIT_FAILURE;
		}

		ATTR_MAYBE_UNUSED const Configuration::TypeRegistryConfiguration typeRegistryConfig{};
		ATTR_MAYBE_UNUSED const Configuration::AbilityRegistryConfiguration abilityRegistryConfig{};
		ATTR_MAYBE_UNUSED const Configuration::ItemRegistryConfiguration itemRegistryConfig{};
		ATTR_MAYBE_UNUSED const Configuration::MoveRegistryConfiguration moveRegistryConfig{};
		ATTR_MAYBE_UNUSED const Configuration::StatusRegistryConfiguration statusRegistryConfig{};
		ATTR_MAYBE_UNUSED const Configuration::TerrainRegistryConfiguration terrainRegistryConfig{};
		ATTR_MAYBE_UNUSED const Configuration::WeatherRegistryConfiguration weatherRegistryConfig{};
		ATTR_MAYBE_UNUSED const Configuration::MultiplierRegistryConfiguration multiplierRegistryConfig{};
		ATTR_MAYBE_UNUSED const Configuration::EffectRegistryConfiguration effectRegistryConfig{};
	}

	return EXIT_SUCCESS;
}
