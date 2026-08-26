/*! @file constants.h
	@brief Contains constexpr assert message strings for the move registry.
	@date 07/27/2026
	@since 0.5.2
	@version 0.5.2
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
