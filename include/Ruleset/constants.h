/*! @file constants.h
	@brief Defines default constants for the ruleset policy.
	@date 09/22/2026
	@since 0.12.30
	@version 0.12.41
	@author Matthew Moore
*/

#ifndef INCLUDE_RULESET_CONSTANTS_H
#define INCLUDE_RULESET_CONSTANTS_H

#include <string_view>

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
	inline constexpr bool RULESET_DEFAULT_ALLOW_NON_VOLATILE_STATUS_REPLACEMENT{false};
	inline constexpr bool RULESET_DEFAULT_ALLOW_WEATHER_REPLACEMENT{true};
	inline constexpr bool RULESET_DEFAULT_ALLOW_TERRAIN_REPLACEMENT{true};

	inline constexpr std::string_view MAX_SIDE_SLOT_ERROR_MESSAGE{
		"RulesetPolicy default side size must match the canonical ruleset constant."};
	inline constexpr std::string_view MAX_NON_VOLATILE_PER_POKEMON_ERROR_MESSAGE{
		"RulesetPolicy default non-volatile statuses per Pokemon must match the canonical ruleset constant."};
	inline constexpr std::string_view MAX_VOLATILE_PER_POKEMON_ERROR_MESSAGE{
		"RulesetPolicy default volatile statuses per Pokemon must match the canonical ruleset constant."};
	inline constexpr std::string_view MAX_WEATHERS_ON_FIELD_ERROR_MESSAGE{
		"RulesetPolicy default weathers on field must match the canonical ruleset constant."};
	inline constexpr std::string_view MAX_TERRAINS_ON_FIELD_ERROR_MESSAGE{
		"RulesetPolicy default terrains on field must match the canonical ruleset constant."};
	inline constexpr std::string_view RULESET_POLICY_FIELD_COUNT_ERROR_MESSAGE{
		"RulesetPolicy bounded fields and field-specific errors must stay synchronized."};
	inline constexpr std::string_view GEN9_BASE_GAME_RULESET_POLICY_ERROR_MESSAGE{"GEN9_BASE_GAME must satisfy the ruleset policy bounds."};
	inline constexpr std::string_view GEN9_VGC_DOUBLES_RULESET_POLICY_ERROR_MESSAGE{
		"GEN9_VGC_DOUBLES must satisfy the ruleset policy bounds."};
	inline constexpr std::string_view GEN9_SMOGON_SINGLES_RULESET_POLICY_ERROR_MESSAGE{
		"GEN9_SMOGON_SINGLES must satisfy the ruleset policy bounds."};
	inline constexpr std::string_view GEN8_BASE_GAME_RULESET_POLICY_ERROR_MESSAGE{"GEN8_BASE_GAME must satisfy the ruleset policy bounds."};
	inline constexpr std::string_view GEN7_BASE_GAME_RULESET_POLICY_ERROR_MESSAGE{"GEN7_BASE_GAME must satisfy the ruleset policy bounds."};
	inline constexpr std::string_view GEN6_BASE_GAME_RULESET_POLICY_ERROR_MESSAGE{"GEN6_BASE_GAME must satisfy the ruleset policy bounds."};
} // namespace PocketCore::Ruleset

#endif
