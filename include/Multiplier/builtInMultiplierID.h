/*! @file builtinMultiplierID.h
	@brief Defines identifiers for multipliers compiled into PocketCore.
	@date 08/22/2026
	@since 0.8.1
	@version 0.11.6
	@author Matthew Moore
*/

#ifndef INCLUDE_MULTIPLIER_BUILTIN_MULTIPLIER_ID_H
#define INCLUDE_MULTIPLIER_BUILTIN_MULTIPLIER_ID_H

#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Multiplier/multiplierID.h"

namespace PocketCore::Multiplier
{
	/*! @enum BuiltinMultiplierID
		@showenumvalues
		@brief Names the multipliers provided by PocketCore itself.
		@details This closed enum is only a catalog of built-in multipliers. Runtime state and user-facing APIs use the open @ref
	   MultiplierID type.
		@note All enum values must be handled exhaustively when registering built-in metadata.
		@date 08/22/2026
		@since 0.8.1
		@version 0.11.6
		@author Matthew Moore
	*/
	enum class BuiltinMultiplierID : PocketCore::Core::ub
	{
		None,
		Ability,
		Item,
		Targets,
		PopulationBomb,
		Weather,
		Critical,
		Randomization,
		Stab,
		TypeEffectiveness,
		Burn,
		FinalMultiplier,
	};

	/*! @brief Converts a built-in catalog value to its stable registry identifier.
		@param[in] builtinMultiplierID The built-in multiplier to convert.
		@return The corresponding open multiplier identifier.
		@since 0.8.1
		@version 0.8.1
	*/
	ATTR_NODISCARD constexpr MultiplierID toMultiplierID(const BuiltinMultiplierID builtinMultiplierID) noexcept
	{
		return MultiplierID{static_cast<PocketCore::Core::ub>(builtinMultiplierID)};
	}
} // namespace PocketCore::Multiplier

#endif
