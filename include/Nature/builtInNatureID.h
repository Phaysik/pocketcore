/*! @file builtinNatureID.h
	@brief Defines identifiers for natures compiled into PocketCore.
	@date 08/31/2026
	@since 0.11.6
	@version 0.12.13
	@author Matthew Moore
*/

#ifndef INCLUDE_NATURE_BUILTIN_NATURE_ID_H
#define INCLUDE_NATURE_BUILTIN_NATURE_ID_H

#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Nature/natureID.h"

namespace PocketCore::Nature
{
	using PocketCore::Core::ub;

	/*! @enum BuiltinNatureID
		@showenumvalues
		@brief Names the natures provided by PocketCore itself.
		@details This closed enum is only a catalog of built-in natures. Runtime state and user-facing APIs use the open @ref NatureID
	   type.
		@note All enum values must be handled exhaustively when registering built-in metadata.
		@date 08/31/2026
		@since 0.11.6
		@version 0.12.13
		@author Matthew Moore
	*/
	enum class BuiltinNatureID : ub
	{
		None,
		Hardy,
		Lonely,
		Brave,
		Adamant,
		Naughty,
		Bold,
		Docile,
		Relaxed,
		Impish,
		Lax,
		Timid,
		Hasty,
		Serious,
		Jolly,
		Naive,
		Modest,
		Mild,
		Quiet,
		Bashful,
		Rash,
		Calm,
		Gentle,
		Sassy,
		Careful,
		Quirky,
		FinalNature,
	};

	/*! @brief Converts a built-in catalog value to its stable registry identifier.
		@param[in] builtinNatureID The built-in nature to convert.
		@return The corresponding open nature identifier.
		@since 0.11.6
		@version 0.12.13
	*/
	ATTR_NODISCARD constexpr NatureID toNatureID(const BuiltinNatureID builtinNatureID) noexcept
	{
		return NatureID{static_cast<ub>(builtinNatureID)};
	}
} // namespace PocketCore::Nature

#endif
