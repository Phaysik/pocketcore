/*! @file rulesetPresets.h
	@brief Defines preset ruleset policies for various generations and formats.
	@date 09/14/2026
	@since 0.12.32
	@version 0.12.33
	@author Matthew Moore
*/

#ifndef INCLUDE_RULESET_RULESET_PRESETS_H
#define INCLUDE_RULESET_RULESET_PRESETS_H

#include "rulesetPolicy.h"

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
} // namespace PocketCore::Ruleset

#endif
