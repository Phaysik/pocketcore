/*! @file idInterface.h
	@brief Defines a tagged value type for creating strongly typed stable identifiers.
	@date 07/30/2026
	@since 0.4.1
	@version 0.8.1
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
		@since 0.4.1
		@version 0.4.1
		@author Matthew Moore
	*/
	template <typename Tag, us DefaultValue>
	class IDInterface
	{
		public:
			/*! @brief Constructs an identifier with the domain-specific default value.
				@since 0.4.1
				@version 0.4.1
			 */
			constexpr IDInterface() noexcept = default;

			/*! @brief Constructs an identifier from a registry-assigned value.
				@param[in] value The stable numeric value assigned by the domain registry.
				@since 0.4.1
				@version 0.4.1
			*/
			explicit constexpr IDInterface(const us value) noexcept : mValue{value} {}

			/*! @brief Returns the underlying registry-assigned value.
				@return The stable numeric identifier.
				@since 0.4.1
				@version 0.4.1
			*/
			ATTR_NODISCARD constexpr us getValue() const noexcept
			{
				return mValue;
			}

			/*! @brief Compares two identifiers from the same domain for equality.
				@since 0.4.1
				@version 0.4.1
			 */
			constexpr std::strong_ordering operator<=>(const IDInterface &) const noexcept = default;

		private:
			/*! @brief The stable numeric value assigned by the domain registry. */
			us mValue{DefaultValue};
	};
} // namespace PocketCore::ID

namespace std
{
	/*! @struct hash ID/idInterface.h
		@brief Provides a standard hash function for a tagged PocketCore identifier.
		@details The specialization hashes the identifier's underlying numeric value, allowing @ref PocketCore::ID::IDInterface to be
			used as a key in standard unordered containers.
		@tparam Tag The unique type identifying the registry domain.
		@tparam DefaultValue The numeric value used by the identifier's default constructor.
		@date 07/30/2026
		@since 0.8.1
		@version 0.8.1
		@author Matthew Moore
	*/
	template <typename Tag, PocketCore::Core::us DefaultValue>
	struct hash<PocketCore::ID::IDInterface<Tag, DefaultValue>>
	{
		public:
			/*! @brief Computes a hash value from an identifier's underlying numeric value.
				@param[in] identifier The tagged identifier to hash.
				@return The hash produced by the standard hash function for the identifier's numeric value.
				@note The operation is noexcept and produces equal hash values for equal identifiers.
				@since 0.8.1
				@version 0.8.1
			*/
			std::size_t operator()(const PocketCore::ID::IDInterface<Tag, DefaultValue> &identifier) const noexcept
			{
				return std::hash<PocketCore::Core::us>{}(identifier.getValue());
			}
	};
} // namespace std

#endif
