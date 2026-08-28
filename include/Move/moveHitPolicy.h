/*! @file moveHitPolicy.h
	@brief Defines how moves determine the number of hit attempts.
	@date 08/04/2026
	@since 0.9.8
	@version 0.9.8
	@author Matthew Moore
*/

#ifndef INCLUDE_MOVE_MOVEHITPOLICY_H
#define INCLUDE_MOVE_MOVEHITPOLICY_H

#include <variant>
#include <vector>

#include "Core/typedefs.h"

namespace PocketCore::Move
{
	using PocketCore::Core::ub;

	/*! @struct FixedHitCount Move/moveHitPolicy.h
		@brief Stores a fixed number of hit attempts for a move.
		@date 08/04/2026
		@since 0.9.8
		@version 0.9.8
		@author Matthew Moore
	*/
	struct FixedHitCount
	{
		public:
			/*! @brief The number of hit attempts made after any move-level accuracy check succeeds. */
			ub mHitCount{1};
	};

	/*! @struct WeightedHitCountOutcome Move/moveHitPolicy.h
		@brief Associates a possible hit count with its integer selection weight.
		@date 08/04/2026
		@since 0.9.8
		@version 0.9.8
		@author Matthew Moore
	*/
	struct WeightedHitCountOutcome
	{
		public:
			/*! @brief The number of hits produced when this outcome is selected. */
			ub mHitCount{};

			/*! @brief The relative probability weight for this outcome. */
			double mWeight{};
	};

	/*! @struct WeightedHitCount Move/moveHitPolicy.h
		@brief Stores weighted alternatives for selecting a move's hit count.
		@details Weights are relative integer values and do not need to sum to 100. Resolution performs no allocation.
		@date 08/04/2026
		@since 0.9.8
		@version 0.9.8
		@author Matthew Moore
	*/
	struct WeightedHitCount
	{
		public:
			/*! @brief The owned outcomes evaluated in declaration order. */
			std::vector<WeightedHitCountOutcome> mOutcomes{};
	};

	/*! @brief Selects either a fixed or weighted hit-count strategy. */
	using HitCountPolicy = std::variant<FixedHitCount, WeightedHitCount>;
} // namespace PocketCore::Move

#endif
