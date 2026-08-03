/*! @file builtInTypeEffectivenessID.h
	@brief Declares built-in catalog identifiers for default type-effectiveness entries.
	@date 07/27/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_TYPES_BUILT_IN_TYPE_EFFECTIVENESS_ID_H
#define INCLUDE_TYPES_BUILT_IN_TYPE_EFFECTIVENESS_ID_H

#include "Core/typedefs.h"
#include "Types/typeEffectivenessID.h"

namespace PocketCore::Types
{
	using PocketCore::Core::ub;

	/*! @enum BuiltInTypeEffectivenessID
		@showenumvalues
		@brief Enumerates built-in type-effectiveness categories used by default type charts.
		@date 08/03/2026
		@version x.x.x
		@since x.x.x
		@author Matthew Moore
	*/
	enum class BuiltInTypeEffectivenessID : ub
	{
		NE,
		NVE,
		E,
		SE,
	};

	/*! @brief Converts a built-in type-effectiveness catalog value to its stable registry identifier.
		@param[in] builtInTypeEffectiveness The built-in type-effectiveness value to convert.
		@return The corresponding open type-effectiveness identifier.
	*/
	ATTR_NODISCARD constexpr TypeEffectivenessID toTypeEffectivenessID(const BuiltInTypeEffectivenessID builtInTypeEffectiveness) noexcept
	{
		return TypeEffectivenessID{static_cast<PocketCore::Core::ub>(builtInTypeEffectiveness)};
	}
} // namespace PocketCore::Types

#endif
