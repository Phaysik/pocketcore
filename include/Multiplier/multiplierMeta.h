/*! @file multiplierMeta.h
	@brief Defines the metadata stored for built-in and user-defined multipliers.
	@date 07/30/2026
	@since 0.8.1
	@version 0.8.1
	@author Matthew Moore
*/

#ifndef INCLUDE_MULTIPLIER_MULTIPLIERMETA_H
#define INCLUDE_MULTIPLIER_MULTIPLIERMETA_H

#include <string_view>

#include "Core/typedefs.h"

#include "multiplierID.h"

namespace PocketCore::Multiplier
{
	using PocketCore::Core::ub;

	/*! @enum MultiplierApplicationPolicy
		@brief Selects how a damage multiplier participates in modifier calculation.
	*/
	enum class MultiplierApplicationPolicy : ub
	{
		RoundHalfDown, /*!< Applies directly to damage and rounds fractional halves down. */
		Floor,		   /*!< Applies directly to damage and discards the fractional result. */
		Other,		   /*!< Combines in 4096 fixed-point space with other values before one final damage application. */
	};

	/*! @struct MultiplierMeta Multiplier/multiplierMeta.h
		@brief Stores one multiplier's stable ID, display name, and owned trigger definitions.
		@details The trigger vector owns its elements and their effect vectors. The display name is a non-owning view whose backing storage
	   must remain valid while this metadata is registered.
		@date 07/30/2026
		@since 0.8.1
		@version 0.8.1
		@author Matthew Moore
	*/
	struct MultiplierMeta
	{
		public:
			/*! @brief The case-sensitive display name stored as a non-owning view. */
			std::string_view mName{};

			/*! @brief The stable built-in or user-assigned identifier. */
			MultiplierID mMultiplierID{};

			/*! @brief The calculation policy used when applying this multiplier to damage. */
			MultiplierApplicationPolicy mApplicationPolicy{MultiplierApplicationPolicy::RoundHalfDown};
	};
} // namespace PocketCore::Multiplier

#endif
