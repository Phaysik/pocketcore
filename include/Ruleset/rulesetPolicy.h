/*! @file rulesetPolicy.h
	@brief Defines the runtime value type for symmetric, battle-wide rules.
	@details The policy describes legal battle mechanics and semantic limits independently of physical storage capacity.
	@date 09/22/2026
	@since 0.12.30
	@version 0.12.41
	@author Matthew Moore
*/

#ifndef INCLUDE_RULESET_RULESET_POLICY_H
#define INCLUDE_RULESET_RULESET_POLICY_H

#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"

#include "constants.h"

namespace PocketCore::Ruleset
{
	using PocketCore::Configuration::MAX_ACTIVE_SLOTS_PER_SIDE;
	using PocketCore::Configuration::MAX_ACTIVE_TERRAINS_ON_FIELD;
	using PocketCore::Configuration::MAX_ACTIVE_WEATHERS_ON_FIELD;
	using PocketCore::Configuration::MAX_NON_VOLATILE_STATUSES_PER_POKEMON;
	using PocketCore::Configuration::MAX_VOLATILE_STATUSES_PER_POKEMON;
	using PocketCore::Core::ub;

	/*! @struct RulesetPolicy rulesetPolicy.h Ruleset/rulesetPolicy.h
		@brief Stores symmetric, battle-wide rules for one battle.
		@details Owns only value-based semantic limits and mechanic permissions; it owns no registry entries, Pokemon, battle state, or
	   storage. Its limits do not control array extents in \c Configuration/constants.h.
		@note Per-side permissions are intentionally excluded. @ref SideConstraints is reserved for that responsibility in version 0.14.0.
		@date 09/22/2026
		@since 0.12.30
		@version 0.12.41
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

			/*! @brief Stores the maximum number of Pokemon permitted on either side.
				@details Accepts values from 0 through @ref PocketCore::Configuration::MAX_ACTIVE_SLOTS_PER_SIDE (6), inclusive.
			*/
			ub mMaxSideSize{RULESET_DEFAULT_MAX_SIDE_SIZE};
			/*! @brief Stores the maximum number of simultaneous non-volatile statuses on one Pokemon.
				@details Accepts values from 0 through @ref PocketCore::Configuration::MAX_NON_VOLATILE_STATUSES_PER_POKEMON (5), inclusive.
			*/
			ub mMaxNonVolatileStatuses{RULESET_DEFAULT_MAX_NON_VOLATILE_STATUSES};
			/*! @brief Stores the maximum number of simultaneous volatile statuses on one Pokemon.
				@details Accepts values from 0 through @ref PocketCore::Configuration::MAX_VOLATILE_STATUSES_PER_POKEMON (8), inclusive.
			*/
			ub mMaxVolatileStatuses{RULESET_DEFAULT_MAX_VOLATILE_STATUSES};
			/*! @brief Stores the maximum number of simultaneous weather conditions on the field.
				@details Accepts values from 0 through @ref PocketCore::Configuration::MAX_ACTIVE_WEATHERS_ON_FIELD (5), inclusive.
			*/
			ub mMaxWeathers{RULESET_DEFAULT_MAX_WEATHERS};
			/*! @brief Stores the maximum number of simultaneous terrain conditions on the field.
				@details Accepts values from 0 through @ref PocketCore::Configuration::MAX_ACTIVE_TERRAINS_ON_FIELD (5), inclusive.
			*/
			ub mMaxTerrains{RULESET_DEFAULT_MAX_TERRAINS};

			/*! @brief Determines whether Terastallization is legal. */
			bool mAllowTera{RULESET_DEFAULT_ALLOW_TERA};
			/*! @brief Determines whether Gigantamax is legal. */
			bool mAllowGmax{RULESET_DEFAULT_ALLOW_GMAX};
			/*! @brief Determines whether Z-Moves are legal. */
			bool mAllowZMoves{RULESET_DEFAULT_ALLOW_Z_MOVES};
			/*! @brief Determines whether Mega Evolution is legal. */
			bool mAllowMega{RULESET_DEFAULT_ALLOW_MEGA};

			/*! @brief Determines whether non-volatile statuses should be replaced when the status list is full. */
			bool mReplaceNonVolatileStatusWhenFull{RULESET_DEFAULT_ALLOW_NON_VOLATILE_STATUS_REPLACEMENT};

			/*! @brief Determines whether weather should be replaced when the weather list is full. */
			bool mReplaceWeatherWhenFull{RULESET_DEFAULT_ALLOW_WEATHER_REPLACEMENT};

			/*! @brief Determines whether terrain should be replaced when the terrain list is full. */
			bool mReplaceTerrainWhenFull{RULESET_DEFAULT_ALLOW_TERRAIN_REPLACEMENT};

			// NOLINTEND(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)
	};

	static_assert(RulesetPolicy{}.mMaxSideSize <= MAX_ACTIVE_SLOTS_PER_SIDE, MAX_SIDE_SLOT_ERROR_MESSAGE);
	static_assert(RulesetPolicy{}.mMaxNonVolatileStatuses <= MAX_NON_VOLATILE_STATUSES_PER_POKEMON,
				  MAX_NON_VOLATILE_PER_POKEMON_ERROR_MESSAGE);
	static_assert(RulesetPolicy{}.mMaxVolatileStatuses <= MAX_VOLATILE_STATUSES_PER_POKEMON, MAX_VOLATILE_PER_POKEMON_ERROR_MESSAGE);
	static_assert(RulesetPolicy{}.mMaxWeathers <= MAX_ACTIVE_WEATHERS_ON_FIELD, MAX_WEATHERS_ON_FIELD_ERROR_MESSAGE);
	static_assert(RulesetPolicy{}.mMaxTerrains <= MAX_ACTIVE_TERRAINS_ON_FIELD, MAX_TERRAINS_ON_FIELD_ERROR_MESSAGE);
} // namespace PocketCore::Ruleset

#endif
