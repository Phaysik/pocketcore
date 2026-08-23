/*! @file builtinStatusID.h
	@brief Defines identifiers for statuses compiled into PocketCore.
	@date 07/27/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_STATUS_BUILTIN_STATUS_ID_H
#define INCLUDE_STATUS_BUILTIN_STATUS_ID_H

#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Status/statusID.h"

namespace PocketCore::Status
{
	using PocketCore::Core::ub;

	/*! @enum BuiltinStatusID
		@showenumvalues
		@brief Names the statuses provided by PocketCore itself.
		@details This closed enum is only a catalog of built-in statuses. Runtime state and user-facing APIs use the open @ref StatusID
	   type.
		@note All enum values must be handled exhaustively when registering built-in metadata.
		@date 07/27/2026
		@version x.x.x
		@since x.x.x
		@author Matthew Moore
	*/
	enum class BuiltinStatusID : ub
	{
		None,
		Paralysis,
		Burn,
		Sleep,
		Freeze,
		Poison,
		Toxic,
		FinalStatus,
	};

	/*! @brief Converts a built-in catalog value to its stable registry identifier.
		@param[in] builtinStatusID The built-in status to convert.
		@return The corresponding open status identifier.
	*/
	ATTR_NODISCARD constexpr StatusID toStatusID(const BuiltinStatusID builtinStatusID) noexcept
	{
		return StatusID{static_cast<ub>(builtinStatusID)};
	}
} // namespace PocketCore::Status

#endif
