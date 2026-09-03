/*! @file multiplierMeta.h
	@brief Defines the metadata stored for built-in and user-defined multipliers.
	@date 09/03/2026
	@since 0.8.1
	@version 0.12.19
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
		@since 0.11.2
		@version 0.11.5
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
		@date 09/03/2026
		@since 0.8.1
		@version 0.12.19
		@author Matthew Moore
	*/
	struct MultiplierMeta
	{
		public:
			/*! @brief Compares two MultiplierMeta instances for equivalent metadata.
				@details Compares all fields exactly.
				@param[in] other The MultiplierMeta instance to compare.
				@return True when both instances contain equivalent metadata; otherwise false.
				@since 0.12.19
				@version 0.12.19
			*/
			ATTR_NODISCARD constexpr bool operator==(const MultiplierMeta &other) const noexcept = default;

			// NOLINTBEGIN(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)

			/*! @brief The case-sensitive display name stored as a non-owning view. */
			std::string_view mName{};

			/*! @brief The stable built-in or user-assigned identifier. */
			MultiplierID mMultiplierID{};

			/*! @brief The calculation policy used when applying this multiplier to damage. */
			MultiplierApplicationPolicy mApplicationPolicy{MultiplierApplicationPolicy::RoundHalfDown};

			// NOLINTEND(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)
	};
} // namespace PocketCore::Multiplier

#endif
