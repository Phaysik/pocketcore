/*! @file moveHitPolicy.h
	@brief Defines how moves determine the number of hit attempts.
	@date 09/03/2026
	@since 0.9.8
	@version 0.12.19
	@author Matthew Moore
*/

#ifndef INCLUDE_MOVE_MOVEHITPOLICY_H
#define INCLUDE_MOVE_MOVEHITPOLICY_H

#include <variant>
#include <vector>

#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Utility/Math/floatUtility.h"

namespace PocketCore::Move
{
	using PocketCore::Core::ub;
	using Utility::Math::approximatelyEqualAbsRel;

	/*! @struct FixedHitCount Move/moveHitPolicy.h
		@brief Stores a fixed number of hit attempts for a move.
		@date 09/03/2026
		@since 0.9.8
		@version 0.12.19
		@author Matthew Moore
	*/
	struct FixedHitCount
	{
		public:
			/*! @brief Compares fixed hit-count policies by their configured hit count.
				@param[in] other The fixed hit-count policy to compare.
				@return True when both policies specify the same number of hit attempts; otherwise false.
				@since 0.12.19
				@version 0.12.19
			*/
			ATTR_NODISCARD constexpr bool operator==(const FixedHitCount &other) const noexcept = default;

			// NOLINTBEGIN(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)

			/*! @brief The number of hit attempts made after any move-level accuracy check succeeds. */
			ub mHitCount{1};

			// NOLINTEND(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)
	};

	/*! @struct WeightedHitCountOutcome Move/moveHitPolicy.h
		@brief Associates a possible hit count with its integer selection weight.
		@date 09/03/2026
		@since 0.9.8
		@version 0.12.19
		@author Matthew Moore
	*/
	struct WeightedHitCountOutcome
	{
		public:
			/*! @brief Compares weighted hit-count outcomes by hit count and relative weight.
				@param[in] other The weighted outcome to compare.
				@return True when both outcomes contain the same hit count and weight; otherwise false.
				@since 0.12.19
				@version 0.12.19
			*/
			ATTR_NODISCARD constexpr bool operator==(const WeightedHitCountOutcome &other) const noexcept
			{
				return mHitCount == other.mHitCount && approximatelyEqualAbsRel(mWeight, other.mWeight);
			}

			// NOLINTBEGIN(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)

			/*! @brief The number of hits produced when this outcome is selected. */
			ub mHitCount{};

			/*! @brief The relative probability weight for this outcome. */
			double mWeight{};

			// NOLINTEND(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)
	};

	/*! @struct WeightedHitCount Move/moveHitPolicy.h
		@brief Stores weighted alternatives for selecting a move's hit count.
		@details Weights are relative integer values and do not need to sum to 100. Resolution performs no allocation.
		@date 09/03/2026
		@since 0.9.8
		@version 0.12.19
		@author Matthew Moore
	*/
	struct WeightedHitCount
	{
		public:
			/*! @brief Compares weighted hit-count policies by their ordered outcomes.
				@param[in] other The weighted hit-count policy to compare.
				@return True when both policies contain equivalent outcomes in the same order; otherwise false.
				@since 0.12.19
				@version 0.12.19
			*/
			ATTR_NODISCARD constexpr bool operator==(const WeightedHitCount &other) const noexcept = default;

			// NOLINTBEGIN(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)

			/*! @brief The owned outcomes evaluated in declaration order. */
			std::vector<WeightedHitCountOutcome> mOutcomes{};

			// NOLINTEND(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)
	};

	/*! @brief Selects either a fixed or weighted hit-count strategy. */
	using HitCountPolicy = std::variant<FixedHitCount, WeightedHitCount>;
} // namespace PocketCore::Move

#endif
