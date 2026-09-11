/*! @file logging.testHelper.h
	@brief Header file for running tests for the logging.
	@date 09/10/2026
	@since 0.4.0
	@version 0.12.20
	@author Matthew Moore
*/

#ifndef TEST_INCLUDE_UTILITY_DEBUG_LOGGING_LOGGING_TEST_HELPER_H
#define TEST_INCLUDE_UTILITY_DEBUG_LOGGING_LOGGING_TEST_HELPER_H

#include <string_view>

#include "Utility/Debug/Logging/logger.h"

namespace PocketCore::Testing
{
	using PocketCore::Utility::Debug::Logging::Logger;

	static void ensureLoggerInitialized(const std::string_view &loggerName, const std::string_view &fileName,
										const bool truncateFile = true)
	{
		static bool initialized{false};

		if (!initialized)
		{
			initialized = Logger::initialize(loggerName, fileName, truncateFile);
		}
	}
} // namespace PocketCore::Testing

#endif
