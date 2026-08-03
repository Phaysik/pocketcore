/*! @file builtinMoveID.h
	@brief Defines identifiers for moves compiled into PocketCore.
	@date 07/27/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_MOVE_BUILTIN_MOVE_ID_H
#define INCLUDE_MOVE_BUILTIN_MOVE_ID_H

#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Move/moveID.h"

namespace PocketCore::Move
{
	/*! @enum BuiltinMoveID
		@showenumvalues
		@brief Names the moves provided by PocketCore itself.
		@details This closed enum is only a catalog of built-in moves. Runtime state and user-facing APIs use the open @ref MoveID
	   type.
		@note All enum values must be handled exhaustively when registering built-in metadata.
		@date 07/27/2026
		@version x.x.x
		@since x.x.x
		@author Matthew Moore
	*/
	enum class BuiltinMoveID : PocketCore::Core::ub
	{
		None,
		Pound,
		KarateChop,
	};

	/*! @brief Converts a built-in catalog value to its stable registry identifier.
		@param[in] builtinMoveID The built-in move to convert.
		@return The corresponding open move identifier.
	*/
	ATTR_NODISCARD constexpr MoveID toMoveID(const BuiltinMoveID builtinMoveID) noexcept
	{
		return MoveID{static_cast<PocketCore::Core::ub>(builtinMoveID)};
	}
} // namespace PocketCore::Move

#endif
