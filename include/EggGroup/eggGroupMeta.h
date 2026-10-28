/*! @file eggGroupMeta.h
	@brief Defines the metadata stored for built-in and user-defined egg groups.
	@date 10/26/2026
	@since 0.12.53
	@version 0.12.53
	@author Matthew Moore
*/

#ifndef INCLUDE_EGGGROUP_EGGGROUPMETA_H
#define INCLUDE_EGGGROUP_EGGGROUPMETA_H

#include <string>

#include "Core/typedefs.h"

#include "eggGroupID.h"

namespace PocketCore::EggGroup
{
	using PocketCore::Core::ub;

	/*! @struct EggGroupMeta EggGroup/eggGroupMeta.h
		@brief Stores one egg group's stable ID and owned display name.
		@details The string owns the display name. Copying this metadata creates an independent
	   copy, so the source name does not need to outlive the copy stored in a registry.
		@date 10/26/2026
		@since 0.12.53
		@version 0.12.53
		@author Matthew Moore
	*/
	struct EggGroupMeta
	{
		public:
			/*! @brief Compares two EggGroupMeta instances for equivalent metadata.
				@details Compares all fields exactly.
				@param[in] other The EggGroupMeta instance to compare.
				@return True when both instances contain equivalent metadata; otherwise false.
				@since 0.12.53
				@version 0.12.53
			*/
			ATTR_NODISCARD constexpr bool operator==(const EggGroupMeta &other) const = default;

			// NOLINTBEGIN(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)

			/*! @brief Owns the case-sensitive display name. */
			std::string mName{};

			/*! @brief The stable built-in or user-assigned identifier. */
			EggGroupID mEggGroupID{};

			// NOLINTEND(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)
	};
} // namespace PocketCore::EggGroup

#endif
