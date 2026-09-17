/*! @file rulesetPolicyError.h
	@brief Defines error codes for ruleset policy violations.
	@details Provides a set of error codes corresponding to violations of the ruleset policy.
	@date 09/17/2026
	@since 0.12.36
	@version 0.12.36
	@author Matthew Moore
*/

#ifndef INCLUDE_RULESET_RULESET_POLICY_ERROR_H
#define INCLUDE_RULESET_RULESET_POLICY_ERROR_H

#include <array>
#include <expected>
#include <utility>

#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"

#include "constants.h"
#include "rulesetPolicy.h"

namespace PocketCore::Ruleset
{
	using PocketCore::Configuration::MAX_ACTIVE_SLOTS_PER_SIDE;
	using PocketCore::Configuration::MAX_ACTIVE_TERRAINS_ON_FIELD;
	using PocketCore::Configuration::MAX_ACTIVE_WEATHERS_ON_FIELD;
	using PocketCore::Configuration::MAX_NON_VOLATILE_STATUSES_PER_POKEMON;
	using PocketCore::Configuration::MAX_VOLATILE_STATUSES_PER_POKEMON;
	using PocketCore::Core::ub;
	using PocketCore::Core::us;

	/*! @enum RulesetPolicyError
		@showenumvalues
		@brief Identifies a ruleset policy violation.
		@details Each value identifies the policy limit that was exceeded when a ruleset was validated.
		@note The enumerators are stored using the unsigned byte type @ref PocketCore::Core::ub.
		@since 0.12.36
		@version 0.12.36
	*/
	enum class RulesetPolicyError : ub
	{
		/*! @brief Indicates that the maximum side size was exceeded. */
		MAX_SIDE_SIZE_ERROR,
		/*! @brief Indicates that the maximum number of non-volatile statuses was exceeded. */
		MAX_NON_VOLATIVE_STATUSES_ERROR,
		/*! @brief Indicates that the maximum number of volatile statuses was exceeded. */
		MAX_VOLATILE_STATUSES_ERROR,
		/*! @brief Indicates that the maximum number of weathers on the field was exceeded. */
		MAX_WEATHERS_ON_FIELD_ERROR,
		/*! @brief Indicates that the maximum number of terrains on the field was exceeded. */
		MAX_TERRAINS_ON_FIELD_ERROR,
		/*! @brief Indicates that a policy field could not be identified. */
		UNKNOWN_FIELD_ERROR,
	};

	/*! @struct RulesetPolicyFieldBound rulesetPolicyError.h Ruleset/rulesetPolicyError.h
		@brief Associates one bounded policy field with its active-capacity limit and validation error.
		@since 0.12.36
		@version 0.12.36
	*/
	struct RulesetPolicyFieldBound
	{
		public:
			/*! @brief Identifies the policy field validated by this bound. */
			ub RulesetPolicy::*mField;
			/*! @brief Stores the inclusive maximum accepted for the policy field. */
			us mMaximum;
			/*! @brief Identifies the error returned when the policy field exceeds its maximum. */
			RulesetPolicyError mError;
	};

	inline constexpr std::array RULESET_POLICY_FIELD_BOUNDS{
		RulesetPolicyFieldBound{
			.mField = &RulesetPolicy::mMaxSideSize,
			.mMaximum = MAX_ACTIVE_SLOTS_PER_SIDE,
			.mError = RulesetPolicyError::MAX_SIDE_SIZE_ERROR,
		},
		RulesetPolicyFieldBound{
			.mField = &RulesetPolicy::mMaxNonVolatileStatuses,
			.mMaximum = MAX_NON_VOLATILE_STATUSES_PER_POKEMON,
			.mError = RulesetPolicyError::MAX_NON_VOLATIVE_STATUSES_ERROR,
		},
		RulesetPolicyFieldBound{
			.mField = &RulesetPolicy::mMaxVolatileStatuses,
			.mMaximum = MAX_VOLATILE_STATUSES_PER_POKEMON,
			.mError = RulesetPolicyError::MAX_VOLATILE_STATUSES_ERROR,
		},
		RulesetPolicyFieldBound{
			.mField = &RulesetPolicy::mMaxWeathers,
			.mMaximum = MAX_ACTIVE_WEATHERS_ON_FIELD,
			.mError = RulesetPolicyError::MAX_WEATHERS_ON_FIELD_ERROR,
		},
		RulesetPolicyFieldBound{
			.mField = &RulesetPolicy::mMaxTerrains,
			.mMaximum = MAX_ACTIVE_TERRAINS_ON_FIELD,
			.mError = RulesetPolicyError::MAX_TERRAINS_ON_FIELD_ERROR,
		},
	};

	static_assert(RULESET_POLICY_FIELD_BOUNDS.size() == std::to_underlying(RulesetPolicyError::UNKNOWN_FIELD_ERROR),
				  RULESET_POLICY_FIELD_COUNT_ERROR_MESSAGE);

	/*! @brief Validates a ruleset against the configured policy limits.
		@details Checks the maximum side size, non-volatile statuses, volatile statuses, weathers, and terrains in that order.
			Returns the error for the first exceeded limit.
		@param[in] policy Ruleset policy whose limits are checked.
		@return An empty result when all policy limits are valid; otherwise, the corresponding @ref RulesetPolicyError.
		@note The function is @c constexpr and can be evaluated at compile time when supplied with a constant policy.
		@note The validation performs a single pass over the policy fields and has constant time and space complexity.
		@throws No exceptions; the function is @c noexcept.
		@since 0.12.36
		@version 0.12.36
	*/
	ATTR_NODISCARD constexpr std::expected<void, RulesetPolicyError> validateRulesetPolicy(const RulesetPolicy &policy) noexcept
	{
		for (const RulesetPolicyFieldBound &fieldBound : RULESET_POLICY_FIELD_BOUNDS)
		{
			if (policy.*fieldBound.mField > fieldBound.mMaximum)
			{
				return std::unexpected<RulesetPolicyError>(fieldBound.mError);
			}
		}

		return {};
	}
} // namespace PocketCore::Ruleset

#endif
