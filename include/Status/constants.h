/*! @file constants.h
	@brief Contains constexpr assert message strings for the status registry.
	@date 09/11/2026
	@since 0.8.1
	@version 0.12.24
	@author Matthew Moore
*/

#ifndef INCLUDE_STATUS_CONSTANTS_H
#define INCLUDE_STATUS_CONSTANTS_H

#include <string_view>

namespace PocketCore::Status
{
	inline constexpr std::string_view STATUS_NAME_NONE{"None"};
	inline constexpr std::string_view STATUS_NAME_PARALYSIS{"Paralysis"};
	inline constexpr std::string_view STATUS_NAME_BURN{"Burn"};
	inline constexpr std::string_view STATUS_NAME_SLEEP{"Sleep"};
	inline constexpr std::string_view STATUS_NAME_FREEZE{"Freeze"};
	inline constexpr std::string_view STATUS_NAME_POISON{"Poison"};
	inline constexpr std::string_view STATUS_NAME_TOXIC{"Toxic"};
} // namespace PocketCore::Status

#endif
