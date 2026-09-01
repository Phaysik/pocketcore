/*! @file builtinMoveID.h
	@brief Defines identifiers for moves compiled into PocketCore.
	@date 08/31/2026
	@since 0.5.2
	@version 0.12.13
	@author Matthew Moore
*/

#ifndef INCLUDE_MOVE_BUILTIN_MOVE_ID_H
#define INCLUDE_MOVE_BUILTIN_MOVE_ID_H

#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Move/moveID.h"

namespace PocketCore::Move
{
	using PocketCore::Core::ub;

	/*! @enum BuiltinMoveID
		@showenumvalues
		@brief Names the moves provided by PocketCore itself.
		@details This closed enum is only a catalog of built-in moves. Runtime state and user-facing APIs use the open @ref MoveID
	   type.
		@note All enum values must be handled exhaustively when registering built-in metadata.
		@date 08/22/2026
		@version 0.5.2
		@version 0.12.13
		@author Matthew Moore
	*/
	enum class BuiltinMoveID : ub
	{
		None,
		Pound,
		KarateChop,

		Facade,
		HydroSteam,
		FinalMove,
	};

	/*! @brief Converts a built-in catalog value to its stable registry identifier.
		@param[in] builtinMoveID The built-in move to convert.
		@return The corresponding open move identifier.
		@since 0.5.2
		@version 0.12.13
	*/
	ATTR_NODISCARD constexpr MoveID toMoveID(const BuiltinMoveID builtinMoveID) noexcept
	{
		return MoveID{static_cast<ub>(builtinMoveID)};
	}
} // namespace PocketCore::Move

#endif
