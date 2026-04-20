/*! @file constants.h
	@brief Contains constexpr assert message strings for the type registry.
	@date 04/17/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_REGISTRY_CONSTANTS_H
#define INCLUDE_REGISTRY_CONSTANTS_H

#include <string_view>

namespace Pokemon::Registry::Types
{
	constexpr std::string_view INDEX_OOB_GET_ENTRY{"Index out of bounds in getEntry"};
	constexpr std::string_view ROW_OOB_GET_TYPE_CHART_CELL{"Row index out of bounds in getTypeChartCell"};
	constexpr std::string_view COL_OOB_GET_TYPE_CHART_CELL{"Column index out of bounds in getTypeChartCell"};
	constexpr std::string_view ROW_OOB_GET_TYPE_CHART_ROW{"Row index out of bounds in getTypeChartRow"};
	constexpr std::string_view INDEX_OOB_GET_TYPE_ID{"Index out of bounds in getTypeId"};
	constexpr std::string_view INDEX_OOB_GET_TYPE_NAME{"Index out of bounds in getTypeName"};
	constexpr std::string_view INDEX_OOB_SET_ENTRY{"Index out of bounds in setEntry"};
	constexpr std::string_view ROW_OOB_SET_TYPE_CHART_CELL{"Row index out of bounds in setTypeChartCell"};
	constexpr std::string_view COL_OOB_SET_TYPE_CHART_CELL{"Column index out of bounds in setTypeChartCell"};
	constexpr std::string_view ROW_OOB_SET_TYPE_CHART_ROW{"Row index out of bounds in setTypeChartRow"};
	constexpr std::string_view REGISTERED_EXCEEDS_ENTRIES_FIND_BY_NAME{
		"The amount of registered types exceeds the amount of type entries in findEntryIndexByName"};
	constexpr std::string_view REGISTERED_EXCEEDS_ENTRIES_FIND_BY_ID{
		"The amount of registered types exceeds the amount of type entries in findEntryIndexById"};
	constexpr std::string_view REGISTERED_EXCEEDS_ENTRIES_ADD_BUILTIN{
		"The amount of registered types exceeds the amount of type entries in addBuiltin"};
	constexpr std::string_view MATCHUPS_EXCEED_COLUMNS_ADD_BUILTIN{
		"The size of the offensive matchups span exceeds the number of columns in the type chart in addBuiltin"};
} // namespace Pokemon::Registry::Types

#endif
