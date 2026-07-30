/*! @file multiplierMeta.h
	@brief Defines the metadata stored for built-in and user-defined multipliers.
	@date 07/24/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_MULTIPLIER_MULTIPLIERMETA_H
#define INCLUDE_MULTIPLIER_MULTIPLIERMETA_H

#include <string_view>

#include "multiplierID.h"

namespace PocketCore::Multiplier
{
	/*! @struct MultiplierMeta Multiplier/multiplierMeta.h
		@brief Stores one multiplier's stable ID, display name, and owned trigger definitions.
		@details The trigger vector owns its elements and their effect vectors. The display name is a non-owning view whose backing storage
	   must remain valid while this metadata is registered.
		@date 07/27/2026
		@version x.x.x
		@since x.x.x
		@author Matthew Moore
	*/
	struct MultiplierMeta
	{
		public:
			/*! @brief The case-sensitive display name stored as a non-owning view. */
			std::string_view mName{};

			/*! @brief The stable built-in or user-assigned identifier. */
			MultiplierID mMultiplierID{};
	};
} // namespace PocketCore::Multiplier

#endif
