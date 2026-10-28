/*! @file builtinAbilityID.h
	@brief Defines identifiers for abilities compiled into PocketCore.
	@date 10/27/2026
	@since 0.4.0
	@version 0.12.53
	@author Matthew Moore
*/

#ifndef INCLUDE_ABILITY_BUILTIN_ABILITY_ID_H
#define INCLUDE_ABILITY_BUILTIN_ABILITY_ID_H

#include <utility>

#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"

#include "abilityID.h"

namespace PocketCore::Ability
{
	using PocketCore::Configuration::MAX_ABILITIES_PER_POKEMON;
	using PocketCore::Configuration::MAX_ABILITY_POOL_PER_POKEMON;
	using PocketCore::Core::ub;

	/*! @enum BuiltinAbilityID
		@showenumvalues
		@brief Names the abilities provided by PocketCore itself.
		@details This closed enum is only a catalog of built-in abilities. Runtime state and user-facing APIs use the open @ref AbilityID
	   type.
		@note All enum values must be handled exhaustively when registering built-in metadata.
		@date 10/27/2026
		@since 0.4.0
		@version 0.12.53
		@author Matthew Moore
	*/
	enum class BuiltinAbilityID : ub
	{
		None,
		Stench,
		Drizzle,
		SpeedBoost,
		BattleArmor,
		Sturdy,

		CloudNine, // 13
		Levitate,  // 26
		Guts,	   // 62
		AirLock,   // 76
		Elevate,   // 313
		FinalAbility,
	};

	static_assert(MAX_ABILITIES_PER_POKEMON < std::to_underlying(BuiltinAbilityID::FinalAbility));
	static_assert(MAX_ABILITY_POOL_PER_POKEMON < std::to_underlying(BuiltinAbilityID::FinalAbility));

	/*! @brief Converts a built-in catalog value to its stable registry identifier.
		@param[in] builtinAbilityID The built-in ability to convert.
		@return The corresponding open ability identifier.
		@since 0.4.0
		@version 0.12.51
	*/
	ATTR_NODISCARD constexpr AbilityID toAbilityID(const BuiltinAbilityID builtinAbilityID) noexcept
	{
		return AbilityID{std::to_underlying(builtinAbilityID)};
	}
} // namespace PocketCore::Ability

#endif
