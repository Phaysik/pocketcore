/*! @file constants.h
	@brief Contains constexpr assert message strings for the item registry.
	@date 07/27/2026
	@since 0.4.1
	@version 0.4.1
	@author Matthew Moore
*/

#ifndef INCLUDE_ITEM_CONSTANTS_H
#define INCLUDE_ITEM_CONSTANTS_H

#include <string_view>

namespace PocketCore::Item
{
	constexpr std::string_view ITEM_NAME_NONE{"None"};
	constexpr std::string_view ITEM_NAME_CHERI_BERRY{"Cheri Berry"};
	constexpr std::string_view ITEM_NAME_CHESTO_BERRY{"Chesto Berry"};
} // namespace PocketCore::Item

#endif
