/*! @file itemError.h
	@brief Contains the error codes for the Item validation results.
	@date 10/09/2026
	@since 0.12.51
	@version 0.12.51
	@author Matthew Moore
*/

#ifndef INCLUDE_VALIDATION_ITEM_ITEM_ERROR_H
#define INCLUDE_VALIDATION_ITEM_ITEM_ERROR_H

#include "Core/typedefs.h"

namespace PocketCore::Validation::Item
{
	using PocketCore::Core::ub;

	/*! @enum ItemError
		@showenumvalues
		@brief Identifies failures when validating Item metadata.
		@details Serves as the error type in Item validation functions' std::expected results. The error codes can be extended as
		Item validation handles additional constraints.
		@note No enumerator represents success; a successful validation result has a value and contains no error.
		All enum values must be handled exhaustively when dispatching on the error; callers must account for newly added values.
		@date 10/09/2026
		@since 0.12.51
		@version 0.12.51
		@author Matthew Moore
	*/
	enum class ItemError : ub
	{
		/*! @brief Indicates a Item ID has no matching registered metadata. */
		MISSING_ITEM_METADATA,
	};
} // namespace PocketCore::Validation::Item

#endif
