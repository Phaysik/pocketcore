/*! @file builtinLearnsetID.h
	@brief Defines identifiers for learnsets compiled into PocketCore.
	@date 09/11/2026
	@since 0.12.24
	@version 0.12.24
	@author Matthew Moore
*/

#ifndef INCLUDE_LEARNSET_BUILTIN_LEARNSET_ID_H
#define INCLUDE_LEARNSET_BUILTIN_LEARNSET_ID_H

#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Learnset/learnsetID.h"

namespace PocketCore::Learnset
{
	using PocketCore::Core::ub;

	/*! @enum BuiltinLearnsetID
		@showenumvalues
		@brief Names the learnsets provided by PocketCore itself.
		@details This closed enum is only a catalog of built-in learnsets. Runtime state and user-facing APIs use the open @ref LearnsetID
	   type.
		@note All enum values must be handled exhaustively when registering built-in metadata.
		@date 09/11/2026
		@since 0.12.24
		@version 0.12.24
		@author Matthew Moore
	*/
	enum class BuiltinLearnsetID : ub
	{
		None,
		FinalLearnset,
	};

	/*! @brief Converts a built-in catalog value to its stable registry identifier.
		@param[in] builtinLearnsetID The built-in learnset to convert.
		@return The corresponding open learnset identifier.
		@since 0.12.24
		@version 0.12.24
	*/
	ATTR_NODISCARD constexpr LearnsetID toLearnsetID(const BuiltinLearnsetID builtinLearnsetID) noexcept
	{
		return LearnsetID{static_cast<ub>(builtinLearnsetID)};
	}
} // namespace PocketCore::Learnset

#endif
