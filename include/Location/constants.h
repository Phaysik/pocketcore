/*! @file constants.h
	@brief Contains constexpr assert message strings for the location registry.
	@date 10/09/2026
	@since 0.12.51
	@version 0.12.51
	@author Matthew Moore
*/

#ifndef INCLUDE_LOCATION_CONSTANTS_H
#define INCLUDE_LOCATION_CONSTANTS_H

#include <string_view>

namespace PocketCore::Location
{
	inline constexpr std::string_view LOCATION_NAME_NONE{"None"};
	inline constexpr std::string_view LOCATION_NAME_ROUTE1{"Route 1"};
} // namespace PocketCore::Location

#endif
