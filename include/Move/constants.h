/*! @file constants.h
	@brief Contains constexpr assert message strings for the move registry.
	@date 10/27/2026
	@since 0.5.2
	@version 0.12.53
	@author Matthew Moore
*/

#ifndef INCLUDE_MOVE_CONSTANTS_H
#define INCLUDE_MOVE_CONSTANTS_H

#include <string_view>

namespace PocketCore::Move
{
	inline constexpr std::string_view MOVE_NAME_NONE{"None"};
	inline constexpr std::string_view MOVE_NAME_POUND{"Pound"};
	inline constexpr std::string_view MOVE_NAME_KARATE_CHOP{"Karate Chop"};

	inline constexpr std::string_view MOVE_NAME_QUICK_ATTACK{"Quick Attack"};
	inline constexpr std::string_view MOVE_NAME_EXTREME_SPEED{"Extreme Speed"};
	inline constexpr std::string_view MOVE_NAME_FACADE{"Facade"};
	inline constexpr std::string_view MOVE_NAME_HYDRO_STEAM{"Hydro Steam"};
} // namespace PocketCore::Move

#endif
