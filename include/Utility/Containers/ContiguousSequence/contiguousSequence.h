/*! @file contiguousSequence.h
	@brief Declares helper algorithms for contiguous sequence containers.
	@date 08/03/2026
	@since 0.1.0
	@version 0.3.0
	@author Matthew Moore
*/

#ifndef INCLUDE_UTILITY_CONTAINERS_CONTIGUOUSSEQUENCE_CONTIGUOUSSEQUENCE_H
#define INCLUDE_UTILITY_CONTAINERS_CONTIGUOUSSEQUENCE_CONTIGUOUSSEQUENCE_H

#include <span>

#include "Core/attributeMacros.h"
#include "Core/cconcepts.h"

/*! @namespace PocketCore::Utility::Containers::ContiguousSequence
	@brief Utilities for working with contiguous sequence containers
	@details
	This namespace provides small, efficient helper routines that operate on
	contiguous sequence containers (for example `std::array`, `std::vector`)
	by accepting a `std::span` referencing their storage. The functions are
	intentionally simple, constexpr when possible, and suitable for use in
	performance-sensitive code paths where minimal overhead is important.
	@note All functions take `std::span<const T>` and therefore do not modify
		  the underlying sequence; callers should use `std::span` conversions
		  to pass containers or raw arrays.
	@since 0.1.0
	@version 0.1.0
	@author Matthew Moore
*/
namespace PocketCore::Utility::Containers::ContiguousSequence
{
	using PocketCore::Core::Integral;

	/*! @brief Sum `length` elements from @p sequence starting at @p startIndex.
		@details
		Computes the sum of `length` contiguous elements beginning at
		`startIndex` within @p sequence. The function performs direct index
		access using `std::span` (bounds-checked by the caller if required).
		@tparam Concepts::Integral Integral The integer type used for indices
			   and arithmetic. Must satisfy @ref Concepts::Integral.
		@param[in] sequence A read-only span containing the elements to sum.
		@param[in] startIndex The starting index within @p sequence (0-based).
		@param[in] length The number of elements to include in the sum. The
						  caller must ensure `startIndex + length <= sequence.size()`.
		@return The sum of the specified elements as an `Integral` value if the indices are valid;
				returns zero if `startIndex >= sequence.size()` or `startIndex + length > sequence.size()`.
		@note Time complexity: O(length). Space complexity: O(1).
		@since 0.1.0
		@version 0.8.7
	*/
	template <Integral Integral>
	ATTR_NODISCARD constexpr Integral computeContiguousSequenceSum(const std::span<const Integral> &sequence, const Integral startIndex,
																   const Integral length)
	{
		if constexpr (std::is_signed_v<Integral>)
		{
			if (startIndex < 0 || length < 0)
			{
				return Integral{0};
			}
		}

		const std::size_t start{static_cast<std::size_t>(startIndex)};
		const std::size_t count{static_cast<std::size_t>(length)};

		if (start >= sequence.size() || count > sequence.size() - start)
		{
			return Integral{0};
		}

		Integral sum{0};

		for (std::size_t index{start}; index < start + count; ++index)
		{
			sum += sequence.at(index);
		}

		return sum;
	}

	/*! @overload
		@brief Sum elements from @p startIndex to the end of @p sequence.
		@details
		Convenience overload that forwards to the three-argument overload
		(@ref computeContiguousSequenceSum(const std::span<const Integral>&, Integral, Integral)).
		See that overload for full preconditions and complexity guarantees.
		@tparam Concepts::Integral Integral The integral type used for indices
			   and arithmetic. Must satisfy @ref Concepts::Integral.
		@param[in] sequence Read-only span of elements to sum.
		@param[in] startIndex Zero-based index at which summation begins. Defaults to 0.
		@return The sum of elements from `startIndex` to the end as an
				`Integral` value.
		@since 0.1.0
		@version 0.8.7
	*/
	template <Integral Integral>
	ATTR_NODISCARD constexpr Integral computeContiguousSequenceSum(const std::span<const Integral> &sequence, const Integral startIndex = 0)
	{
		if constexpr (std::is_signed_v<Integral>)
		{
			if (startIndex < 0)
			{
				return Integral{0};
			}
		}

		const std::size_t start{static_cast<std::size_t>(startIndex)};

		if (start >= sequence.size())
		{
			return Integral{0};
		}

		return computeContiguousSequenceSum<Integral>(sequence, startIndex, static_cast<Integral>(sequence.size() - start));
	}
} // namespace PocketCore::Utility::Containers::ContiguousSequence

#endif
