/*! @file constants.h
	@brief Contains constexpr assert message strings for the move registry.
	@date 04/17/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_MOVE_CONSTANTS_H
#define INCLUDE_MOVE_CONSTANTS_H

#include <string_view>

namespace PocketCore::Move
{
	constexpr std::string_view MOVE_NAME_NONE{"None"};
	constexpr std::string_view MOVE_NAME_POUND{"Pound"};
	constexpr std::string_view MOVE_NAME_KARATE_CHOP{"Karate Chop"};
} // namespace PocketCore::Move

#endif
