/*! @file constants.h
	@brief Contains constexpr assert message strings for the egg group registry.
	@date 10/26/2026
	@since 0.12.53
	@version 0.12.53
	@author Matthew Moore
*/

#ifndef INCLUDE_EGGGROUP_CONSTANTS_H
#define INCLUDE_EGGGROUP_CONSTANTS_H

#include <string_view>

namespace PocketCore::EggGroup
{
	inline constexpr std::string_view EGG_GROUP_NAME_NONE{"None"};
	inline constexpr std::string_view EGG_GROUP_NAME_AMORPHOUS{"Amorphous"};
	inline constexpr std::string_view EGG_GROUP_NAME_BUG{"Bug"};
	inline constexpr std::string_view EGG_GROUP_NAME_DRAGON{"Dragon"};
	inline constexpr std::string_view EGG_GROUP_NAME_FAIRY{"Fairy"};
	inline constexpr std::string_view EGG_GROUP_NAME_FIELD{"Field"};
	inline constexpr std::string_view EGG_GROUP_NAME_FLYING{"Flying"};
	inline constexpr std::string_view EGG_GROUP_NAME_GRASS{"Grass"};
	inline constexpr std::string_view EGG_GROUP_NAME_HUMAN_LIKE{"Human Like"};
	inline constexpr std::string_view EGG_GROUP_NAME_MINERAL{"Mineral"};
	inline constexpr std::string_view EGG_GROUP_NAME_MONSTER{"Monster"};
	inline constexpr std::string_view EGG_GROUP_NAME_WATER1{"Water 1"};
	inline constexpr std::string_view EGG_GROUP_NAME_WATER2{"Water 2"};
	inline constexpr std::string_view EGG_GROUP_NAME_WATER3{"Water 3"};
	inline constexpr std::string_view EGG_GROUP_NAME_DITTO{"Ditto"};
	inline constexpr std::string_view EGG_GROUP_NAME_NO_EGGS{"No Eggs"};
} // namespace PocketCore::EggGroup

#endif
