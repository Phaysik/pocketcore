/*! @file constants.h
	@brief Contains constexpr assert message strings for the item registry.
	@date 10/28/2026
	@since 0.4.1
	@version 0.12.53
	@author Matthew Moore
*/

#ifndef INCLUDE_ITEM_CONSTANTS_H
#define INCLUDE_ITEM_CONSTANTS_H

#include <string_view>

namespace PocketCore::Item
{
	inline constexpr std::string_view ITEM_NAME_NONE{"None"};
	inline constexpr std::string_view ITEM_NAME_ABILITY_CAPSULE{"Ability Capsule"};

	inline constexpr std::string_view ITEM_NAME_CHERI_BERRY{"Cheri Berry"};
	inline constexpr std::string_view ITEM_NAME_CHESTO_BERRY{"Chesto Berry"};
} // namespace PocketCore::Item

#endif
