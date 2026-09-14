/*! @file rulesetPolicy.h
	@brief Defines the runtime value type for symmetric, battle-wide rules.
	@details The policy describes legal battle mechanics and semantic limits independently of physical storage capacity.
	@date 09/14/2026
	@since 0.12.30
	@version 0.12.30
	@author Matthew Moore
*/

#ifndef INCLUDE_RULESET_RULESET_POLICY_H
#define INCLUDE_RULESET_RULESET_POLICY_H

#include "Core/attributeMacros.h"
#include "Core/typedefs.h"

#include "constants.h"

namespace PocketCore::Ruleset
{
	using PocketCore::Core::ub;

	/*! @struct RulesetPolicy rulesetPolicy.h Ruleset/rulesetPolicy.h
		@brief Stores symmetric, battle-wide rules for one battle.
		@details Owns only value-based semantic limits and mechanic permissions; it owns no registry entries, Pokemon, battle state, or
	   storage. Its limits do not control array extents in @ref PocketCore::Configuration::constants.h.
		@note Per-side permissions are intentionally excluded. @ref SideConstraints is reserved for that responsibility in version 0.14.0.
		@date 09/14/2026
		@since 0.12.30
		@version 0.12.30
		@author Matthew Moore
	*/
	struct RulesetPolicy
	{
		public:
			/*! @brief Compares all policy limits and mechanic permissions.
				@details Is defaulted so the value type compares each of its owned fields. This operation is no-throw.
				@param[in] other The policy to compare.
				@return True when every owned field equals its counterpart in @p other.
				@date 09/14/2026
				@since 0.12.30
				@version 0.12.30
				@author Matthew Moore
			*/
			ATTR_NODISCARD constexpr bool operator==(const RulesetPolicy &other) const noexcept = default;

			// NOLINTBEGIN(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)

			/*! @brief Stores the maximum number of Pokemon permitted on either side. */
			ub mMaxSideSize{RULESET_DEFAULT_MAX_SIZE_SIZE};
			/*! @brief Stores the maximum number of simultaneous non-volatile statuses on one Pokemon. */
			ub mMaxNonVolatileStatuses{RULESET_DEFAULT_MAX_NON_VOLATILE_STATUSES};
			/*! @brief Stores the maximum number of simultaneous volatile statuses on one Pokemon. */
			ub mMaxVolatileStatuses{RULESET_DEFAULT_MAX_VOLATILE_STATUSES};
			/*! @brief Stores the maximum number of simultaneous weather conditions on the field. */
			ub mMaxWeathers{RULESET_DEFAULT_MAX_WEATHERS};
			/*! @brief Stores the maximum number of simultaneous terrain conditions on the field. */
			ub mMaxTerrains{RULESET_DEFAULT_MAX_TERRAINS};

			/*! @brief Determines whether Terastallization is legal. */
			bool mAllowTera{RULESET_DEFAULT_ALLOW_TERA};
			/*! @brief Determines whether Mega Evolution is legal. */
			bool mAllowMega{RULESET_DEFAULT_ALLOW_MEGA};
			/*! @brief Determines whether Gigantamax is legal. */
			bool mAllowGmax{RULESET_DEFAULT_ALLOW_GMAX};
			/*! @brief Determines whether Z-Moves are legal. */
			bool mAllowZ{RULESET_DEFAULT_ALLOW_Z_MOVES};

			// NOLINTEND(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)
	};
} // namespace PocketCore::Ruleset

#endif
