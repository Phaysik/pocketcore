/*! @file typeEffectivenessID.h
	@brief Defines the open identifier used for built-in and user-defined type-effectiveness entries.
	@date 07/27/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_TYPES_TYPEEFFECTIVENESSID_H
#define INCLUDE_TYPES_TYPEEFFECTIVENESSID_H

#include <limits>

#include "Core/typedefs.h"
#include "ID/idInterface.h"

namespace PocketCore::Types
{
	using PocketCore::Core::ub;

	namespace Detail
	{
		/*! @brief Distinguishes type-effectiveness identifiers from all other stable identifier domains. */
		struct typeEffectivenessID;
	} // namespace Detail

	/*! @typedef TypeEffectivenessID
		@brief A strongly typed stable identifier for any registered type-effectiveness definition.
		@details Values are assigned by the type-effectiveness registry. The type is open and represents both built-in
		@ref BuiltInTypeEffectivenessID values and user-defined entries. Its tag prevents comparison or conversion with identifiers from
		other registry domains.
	*/
	using TypeEffectivenessID = PocketCore::ID::IDInterface<Detail::typeEffectivenessID, std::numeric_limits<ub>::max()>;

	/*! @brief The identifier used for an unassigned or not-defined type-effectiveness slot. */
	inline constexpr TypeEffectivenessID NO_TYPE_EFFECTIVENESS_ID{};
} // namespace PocketCore::Types

#endif
