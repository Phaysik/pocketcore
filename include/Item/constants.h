/*! @file constants.h
	@brief Contains constexpr assert message strings for the item registry.
	@date 04/17/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_REGISTRY_CONSTANTS_H
#define INCLUDE_REGISTRY_CONSTANTS_H

#include <string_view>

namespace PocketCore::Item
{
	constexpr std::string_view ITEM_NAME_NONE{"None"};
	constexpr std::string_view ITEM_NAME_CHERI_BERRY{"Cheri Berry"};
	constexpr std::string_view ITEM_NAME_CHESTO_BERRY{"Chesto Berry"};
} // namespace PocketCore::Item

#endif
