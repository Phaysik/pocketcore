/*! @file constants.h
	@brief Contains constexpr assert message strings for the status registry.
	@date 09/28/2026
	@since 0.8.1
	@version 0.12.45
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
	inline constexpr std::string_view STATUS_NAME_AUTOTOMIZE{"Autotomize"};
	inline constexpr std::string_view STATUS_NAME_AQUA_RING{"Aqua Ring"};
} // namespace PocketCore::Status

#endif
