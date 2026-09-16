/*! @file rulesetPresets.h
	@brief Defines preset ruleset policies for various generations and formats.
	@date 09/16/2026
	@since 0.12.32
	@version 0.12.36
	@author Matthew Moore
*/

#ifndef INCLUDE_RULESET_RULESET_PRESETS_H
#define INCLUDE_RULESET_RULESET_PRESETS_H

#include "constants.h"
#include "rulesetPolicy.h"
#include "rulesetPolicyError.h"

namespace PocketCore::Ruleset
{
	inline constexpr RulesetPolicy GEN9_BASE_GAME{
		.mMaxSideSize = 2,
		.mMaxNonVolatileStatuses = 1,
		.mMaxVolatileStatuses = 8,
		.mMaxWeathers = 1,
		.mMaxTerrains = 1,
		.mAllowTera = true,
		.mAllowGmax = false,
		.mAllowZMoves = false,
		.mAllowMega = false,
	};
	static_assert(validateRulesetPolicy(GEN9_BASE_GAME).has_value(), GEN9_BASE_GAME_RULESET_POLICY_ERROR_MESSAGE);

	inline constexpr RulesetPolicy GEN9_VGC_DOUBLES{
		.mMaxSideSize = 2,
		.mMaxNonVolatileStatuses = 1,
		.mMaxVolatileStatuses = 8,
		.mMaxWeathers = 1,
		.mMaxTerrains = 1,
		.mAllowTera = true,
		.mAllowGmax = false,
		.mAllowZMoves = false,
		.mAllowMega = false,
	};
	static_assert(validateRulesetPolicy(GEN9_VGC_DOUBLES).has_value(), GEN9_VGC_DOUBLES_RULESET_POLICY_ERROR_MESSAGE);

	inline constexpr RulesetPolicy GEN9_SMOGON_SINGLES{
		.mMaxSideSize = 1,
		.mMaxNonVolatileStatuses = 1,
		.mMaxVolatileStatuses = 8,
		.mMaxWeathers = 1,
		.mMaxTerrains = 1,
		.mAllowTera = true,
		.mAllowGmax = false,
		.mAllowZMoves = false,
		.mAllowMega = false,
	};
	static_assert(validateRulesetPolicy(GEN9_SMOGON_SINGLES).has_value(), GEN9_SMOGON_SINGLES_RULESET_POLICY_ERROR_MESSAGE);

	inline constexpr RulesetPolicy GEN8_BASE_GAME{
		.mMaxSideSize = 2,
		.mMaxNonVolatileStatuses = 1,
		.mMaxVolatileStatuses = 8,
		.mMaxWeathers = 1,
		.mMaxTerrains = 1,
		.mAllowTera = false,
		.mAllowGmax = true,
		.mAllowZMoves = false,
		.mAllowMega = false,
	};
	static_assert(validateRulesetPolicy(GEN8_BASE_GAME).has_value(), GEN8_BASE_GAME_RULESET_POLICY_ERROR_MESSAGE);

	inline constexpr RulesetPolicy GEN7_BASE_GAME{
		.mMaxSideSize = 2,
		.mMaxNonVolatileStatuses = 1,
		.mMaxVolatileStatuses = 8,
		.mMaxWeathers = 1,
		.mMaxTerrains = 1,
		.mAllowTera = false,
		.mAllowGmax = false,
		.mAllowZMoves = true,
		.mAllowMega = true,
	};
	static_assert(validateRulesetPolicy(GEN7_BASE_GAME).has_value(), GEN7_BASE_GAME_RULESET_POLICY_ERROR_MESSAGE);

	inline constexpr RulesetPolicy GEN6_BASE_GAME{
		.mMaxSideSize = 2,
		.mMaxNonVolatileStatuses = 1,
		.mMaxVolatileStatuses = 8,
		.mMaxWeathers = 1,
		.mMaxTerrains = 1,
		.mAllowTera = false,
		.mAllowGmax = false,
		.mAllowZMoves = false,
		.mAllowMega = true,
	};
	static_assert(validateRulesetPolicy(GEN6_BASE_GAME).has_value(), GEN6_BASE_GAME_RULESET_POLICY_ERROR_MESSAGE);
} // namespace PocketCore::Ruleset

#endif
