/*! @file builtinAbilityID.h
	@brief Defines identifiers for abilities compiled into PocketCore.
	@date 07/27/2026
	@since 0.4.0
	@version 0.5.1
	@author Matthew Moore
*/

#ifndef INCLUDE_ABILITY_BUILTIN_ABILITY_ID_H
#define INCLUDE_ABILITY_BUILTIN_ABILITY_ID_H

#include "Ability/abilityID.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"

namespace PocketCore::Ability
{
	using PocketCore::Core::ub;

	/*! @enum BuiltinAbilityID
		@showenumvalues
		@brief Names the abilities provided by PocketCore itself.
		@details This closed enum is only a catalog of built-in abilities. Runtime state and user-facing APIs use the open @ref AbilityID
	   type.
		@note All enum values must be handled exhaustively when registering built-in metadata.
		@date 07/27/2026
		@since 0.4.0
		@version 0.4.0
		@author Matthew Moore
	*/
	enum class BuiltinAbilityID : ub
	{
		None,
		Stench,
		Drizzle,

		Guts,
		Levitate,
		Elevate,
		AirLock,
		CloudNine,
		FinalAbility,
	};

	/*! @brief Converts a built-in catalog value to its stable registry identifier.
		@param[in] builtinAbilityID The built-in ability to convert.
		@return The corresponding open ability identifier.
		@since 0.4.0
		@version 0.5.1
	*/
	ATTR_NODISCARD constexpr AbilityID toAbilityID(const BuiltinAbilityID builtinAbilityID) noexcept
	{
		return AbilityID{static_cast<ub>(builtinAbilityID)};
	}
} // namespace PocketCore::Ability

#endif
