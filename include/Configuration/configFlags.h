/*! @file configFlags.h
	@brief Contains the user modifiable flags for this application
	@date 03/19/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_CONFIGURATION_CONFIGFLAGS_H
#define INCLUDE_CONFIGURATION_CONFIGFLAGS_H

#include <cstddef>

namespace Pokemon::Configuration
{
	constexpr std::byte bitwiseANDEquals{0b0000'0001};

	constexpr std::byte useCustomConfig{true ? 0b0000'0001 : 0b0000'0000};
	constexpr std::byte configFlags{useCustomConfig};
} // namespace Pokemon::Configuration

#endif