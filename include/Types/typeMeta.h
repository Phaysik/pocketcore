/*! @file typeMeta.h
	@brief Defines the metadata stored for built-in and user-defined types.
	@date 07/24/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_TYPE_TYPEMETA_H
#define INCLUDE_TYPE_TYPEMETA_H

#include <string_view>

#include "Configuration/constants.h"

#include "typeEffectiveness.h"
#include "typeID.h"

namespace PocketCore::Type
{
	using PocketCore::Configuration::MAX_TYPES;

	/*! @struct TypeMeta Type/typeMeta.h
		@brief Stores one type's stable ID, display name, and owned trigger definitions.
		@details The trigger vector owns its elements and their effect vectors. The display name is a non-owning view whose backing storage
	   must remain valid while this metadata is registered.
		@date 07/27/2026
		@version x.x.x
		@since x.x.x
		@author Matthew Moore
	*/
	struct TypeMeta
	{
		public:
			/*! @brief The effectiveness of this type when attacking each registered type by internal index. */
			std::array<TypeEffectiveness, MAX_TYPES> mOffensiveMatchups{};

			/*! @brief The display name for the type. */
			std::string_view mName{};

			/*! @brief The stable identifier for a built-in or user-defined type. */
			TypeID mTypeID{};
	};
} // namespace PocketCore::Type

#endif
