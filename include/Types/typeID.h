/*! @file typeID.h
	@brief Defines the open identifier used for built-in and user-defined Pokemon types.
	@date 07/27/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_TYPES_TYPE_ID_H
#define INCLUDE_TYPES_TYPE_ID_H

#include <limits>

#include "Core/typedefs.h"
#include "Types/types.h"

namespace PocketCore::Types
{
	using PocketCore::Core::ub;

	/*! @class TypeID Types/typeID.h
		@brief A strongly typed stable identifier for any registered Pokemon type.
		@details Values are assigned by the type registry. The type is open and represents both built-in @ref Types values and user-defined
	   types without extending the built-in enum.
		@date 07/27/2026
		@version x.x.x
		@since x.x.x
		@author Matthew Moore
	*/
	class TypeID
	{
		public:
			/*! @brief Constructs an unassigned type identifier. */
			constexpr TypeID() noexcept = default;

			/*! @brief Constructs an identifier from a registry-assigned value.
				@param[in] value The stable numeric value assigned by the type registry.
			*/
			explicit constexpr TypeID(const ub value) noexcept : mValue{value} {}

			/*! @brief Returns the underlying registry-assigned value.
				@return The stable numeric identifier.
			*/
			[[nodiscard]] constexpr ub getValue() const noexcept
			{
				return mValue;
			}

			/*! @brief Compares two stable type identifiers for equality. */
			constexpr bool operator==(const TypeID &) const noexcept = default;

		private:
			/*! @brief The stable numeric value, or the maximum value when unassigned. */
			ub mValue{std::numeric_limits<ub>::max()};
	};

	/*! @brief The identifier used for an unassigned Pokemon type slot. */
	inline constexpr TypeID NO_TYPE_ID{};

	/*! @brief Converts a built-in type catalog value to its stable registry identifier.
		@param[in] builtinType The built-in type to convert.
		@return The corresponding open type identifier.
	*/
	[[nodiscard]] constexpr TypeID toTypeID(const Types builtinType) noexcept
	{
		return TypeID{static_cast<ub>(builtinType)};
	}
} // namespace PocketCore::Types

#endif
