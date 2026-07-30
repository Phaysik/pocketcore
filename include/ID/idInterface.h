/*! @file idInterface.h
	@brief Defines a tagged value type for creating strongly typed stable identifiers.
	@date 07/24/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_ID_IDINTERFACE_H
#define INCLUDE_ID_IDINTERFACE_H

#include <compare>
#include <functional>

#include "Core/attributeMacros.h"
#include "Core/typedefs.h"

namespace PocketCore::ID
{
	using PocketCore::Core::us;

	/*! @class IDInterface ID/idInterface.h
		@brief Stores a stable numeric identifier whose domain is selected by a tag type.
		@details Each Tag and DefaultValue combination creates a distinct, non-polymorphic value type. Identifiers from different domains
	   cannot be compared or implicitly converted even when their numeric values match.
		@tparam Tag The unique type identifying the registry domain.
		@tparam DefaultValue The numeric value used by the default constructor.
		@date 07/27/2026
		@version x.x.x
		@since x.x.x
		@author Matthew Moore
	*/
	template <typename Tag, us DefaultValue>
	class IDInterface
	{
		public:
			/*! @brief Constructs an identifier with the domain-specific default value. */
			constexpr IDInterface() noexcept = default;

			/*! @brief Constructs an identifier from a registry-assigned value.
				@param[in] value The stable numeric value assigned by the domain registry.
			*/
			explicit constexpr IDInterface(const us value) noexcept : mValue{value} {}

			/*! @brief Returns the underlying registry-assigned value.
				@return The stable numeric identifier.
			*/
			ATTR_NODISCARD constexpr us getValue() const noexcept
			{
				return mValue;
			}

			/*! @brief Compares two identifiers from the same domain for equality. */
			constexpr std::strong_ordering operator<=>(const IDInterface &) const noexcept = default;

		private:
			/*! @brief The stable numeric value assigned by the domain registry. */
			us mValue{DefaultValue};
	};
} // namespace PocketCore::ID

namespace std
{
	template <typename Tag, PocketCore::Core::us DefaultValue>
	struct hash<PocketCore::ID::IDInterface<Tag, DefaultValue>>
	{
		public:
			std::size_t operator()(const PocketCore::ID::IDInterface<Tag, DefaultValue> &identifier) const noexcept
			{
				return std::hash<PocketCore::Core::us>{}(identifier.getValue());
			}
	};
} // namespace std

#endif
