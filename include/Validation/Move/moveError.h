/*! @file moveError.h
	@brief Contains the error codes for the Move validation results.
	@date 10/09/2026
	@since 0.12.51
	@version 0.12.51
	@author Matthew Moore
*/

#ifndef INCLUDE_VALIDATION_MOVE_MOVE_ERROR_H
#define INCLUDE_VALIDATION_MOVE_MOVE_ERROR_H

#include "Core/typedefs.h"

namespace PocketCore::Validation::Move
{
	using PocketCore::Core::ub;

	/*! @enum MoveError
		@showenumvalues
		@brief Identifies failures when validating Move metadata.
		@details Serves as the error type in Move validation functions' std::expected results. The error codes can be extended as
		Move validation handles additional constraints.
		@note No enumerator represents success; a successful validation result has a value and contains no error.
		All enum values must be handled exhaustively when dispatching on the error; callers must account for newly added values.
		@date 10/09/2026
		@since 0.12.51
		@version 0.12.51
		@author Matthew Moore
	*/
	enum class MoveError : ub
	{
		/*! @brief Indicates a Move ID has no matching registered metadata. */
		MISSING_MOVE_METADATA,
	};
} // namespace PocketCore::Validation::Move

#endif
