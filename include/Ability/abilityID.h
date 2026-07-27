/*! @file abilityID.h
	@brief Defines the open identifier used for built-in and user-defined abilities.
	@date 07/24/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_ABILITY_ABILITYID_H
#define INCLUDE_ABILITY_ABILITYID_H

#include "Core/typedefs.h"

namespace PocketCore::Ability
{
	using PocketCore::Core::ub;

	/*! @class AbilityID Ability/abilityID.h
		@brief A strongly typed stable identifier for any registered ability.
		@details Values are assigned by the ability registry. Unlike @ref BuiltinAbilityID, this type is open and can represent user-defined
	   abilities without extending an enum.
		@date 07/27/2026
		@version x.x.x
		@since x.x.x
		@author Matthew Moore
	*/
	class AbilityID
	{
		public:
			/*! @brief Constructs the identifier reserved for no ability. */
			constexpr AbilityID() noexcept = default;

			/*! @brief Constructs an identifier from a registry-assigned value.
				@param[in] value The stable numeric value assigned by the ability registry.
			*/
			explicit constexpr AbilityID(const ub value) noexcept : mValue{value}
			{}

			/*! @brief Returns the underlying registry-assigned value.
				@return The stable numeric identifier.
			*/
			[[nodiscard]] constexpr ub getValue() const noexcept
			{
				return mValue;
			}

			/*! @brief Compares two stable ability identifiers for equality. */
			constexpr bool operator==(const AbilityID &) const noexcept = default;

		private:
			/*! @brief The stable numeric value assigned by the ability registry. */
			ub mValue{0};
	};

	/*! @brief The stable identifier representing no ability. */
	inline constexpr AbilityID NO_ABILITY_ID{};
} // namespace PocketCore::Ability

#endif
