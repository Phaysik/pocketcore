/*! @file typeEffectivenessMeta.h
	@brief Defines metadata stored for built-in and user-defined type-effectiveness entries.
	@date 07/24/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_TYPES_TYPEEFFECTIVENESSMETA_H
#define INCLUDE_TYPES_TYPEEFFECTIVENESSMETA_H

#include <string_view>

#include "typeEffectivenessID.h"

namespace PocketCore::Types
{
	/*! @struct TypeEffectivenessMeta Types/typeEffectivenessMeta.h
		@brief Stores one type-effectiveness definition with stable ID, display name, and numeric multiplier value.
		@details The display name is a non-owning view whose backing storage must remain valid while this metadata is registered.
		@date 07/27/2026
		@version x.x.x
		@since x.x.x
		@author Matthew Moore
	*/
	struct TypeEffectivenessMeta
	{
		public:
			/*! @brief The case-sensitive display name stored as a non-owning view. */
			std::string_view mName{};

			/*! @brief The numeric multiplier value applied to damage calculations for this effectiveness entry. */
			float mTypeEffectivenessValue{};

			/*! @brief The stable built-in or user-assigned identifier. */
			TypeEffectivenessID mTypeEffectivenessID{};
	};
} // namespace PocketCore::Types

#endif
