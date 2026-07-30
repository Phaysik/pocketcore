/*! @file constants.h
	@brief Contains constexpr assert message strings for the status registry.
	@date 04/17/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_STATUS_CONSTANTS_H
#define INCLUDE_STATUS_CONSTANTS_H

#include <string_view>

namespace PocketCore::Status
{
	constexpr std::string_view STATUS_NAME_NONE{"None"};
	constexpr std::string_view STATUS_NAME_PARALYSIS{"Paralysis"};
	constexpr std::string_view STATUS_NAME_BURN{"Burn"};
	constexpr std::string_view STATUS_NAME_SLEEP{"Sleep"};
	constexpr std::string_view STATUS_NAME_FREEZE{"Freeze"};
	constexpr std::string_view STATUS_NAME_POISON{"Poison"};
	constexpr std::string_view STATUS_NAME_TOXIC{"Toxic"};
} // namespace PocketCore::Status

#endif
