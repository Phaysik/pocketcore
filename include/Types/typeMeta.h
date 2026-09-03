/*! @file typeMeta.h
	@brief Defines the metadata stored for built-in and user-defined types.
	@date 09/03/2026
	@since 0.12.5
	@version 0.12.19
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
		@date 09/03/2026
		@since 0.12.5
		@version 0.12.19
		@author Matthew Moore
	*/
	struct TypeMeta
	{
		public:
			/*! @brief Compares two TypeMeta instances for equivalent metadata.
				@details Compares all fields exactly.
				@param[in] other The TypeMeta instance to compare.
				@return True when both instances contain equivalent metadata; otherwise false.
				@since 0.12.19
				@version 0.12.19
			*/
			ATTR_NODISCARD constexpr bool operator==(const TypeMeta &other) const noexcept = default;

			// NOLINTBEGIN(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)

			/*! @brief The effectiveness of this type when attacking each registered type by internal index. */
			std::array<TypeEffectiveness, MAX_TYPES> mOffensiveMatchups{};

			/*! @brief The display name for the type. */
			std::string_view mName{};

			/*! @brief The stable identifier for a built-in or user-defined type. */
			TypeID mTypeID{};

			// NOLINTEND(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)
	};
} // namespace PocketCore::Type

#endif
