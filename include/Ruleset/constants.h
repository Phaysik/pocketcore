/*! @file constants.h
	@brief Defines default constants for the ruleset policy.
	@date 09/14/2026
	@since 0.12.30
	@version 0.12.33
	@author Matthew Moore
*/

#ifndef INCLUDE_RULESET_CONSTANTS_H
#define INCLUDE_RULESET_CONSTANTS_H

#include "Core/typedefs.h"

namespace PocketCore::Ruleset
{
	using PocketCore::Core::ub;

	inline constexpr ub RULESET_DEFAULT_MAX_SIDE_SIZE{2};
	inline constexpr ub RULESET_DEFAULT_MAX_NON_VOLATILE_STATUSES{1};
	inline constexpr ub RULESET_DEFAULT_MAX_VOLATILE_STATUSES{8};
	inline constexpr ub RULESET_DEFAULT_MAX_WEATHERS{1};
	inline constexpr ub RULESET_DEFAULT_MAX_TERRAINS{1};
	inline constexpr bool RULESET_DEFAULT_ALLOW_TERA{true};
	inline constexpr bool RULESET_DEFAULT_ALLOW_MEGA{false};
	inline constexpr bool RULESET_DEFAULT_ALLOW_GMAX{false};
	inline constexpr bool RULESET_DEFAULT_ALLOW_Z_MOVES{false};
} // namespace PocketCore::Ruleset

#endif
