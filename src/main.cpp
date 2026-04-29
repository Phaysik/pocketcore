/*! @file main.cpp
	@brief Contains the function definitions for creating a main
	@date --/--/----
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#include <cstdlib>
#include <string_view>

#include "Configuration/configFlags.h"
#include "Configuration/typeRegistryConfiguration.h"
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
	namespace Configuration = Pokemon::Configuration;
	namespace Logging = Pokemon::Utility::Debug::Logging;

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

		ATTR_MAYBE_UNUSED const Configuration::TypeRegistryConfiguration config{};
	}

	return EXIT_SUCCESS;
}