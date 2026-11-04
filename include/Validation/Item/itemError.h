/*! @file itemError.h
	@brief Contains the error codes for the Item validation results.
	@date 11/04/2026
	@since 0.12.51
	@version 0.12.53
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
		@date 11/04/2026
		@since 0.12.51
		@version 0.12.53
		@author Matthew Moore
	*/
	enum class ItemError : ub
	{
		/*! @brief Indicates a Item ID has no matching registered metadata. */
		MISSING_ITEM_METADATA,
		/*! @brief Indicates that the duplicate Item ID replacement value has no valid metadata. */
		DUPLICATE_REPLACEMENT_MISSING_METADATA,
		/*! @brief Indicates that to many items were passed in compared to the amount allowed by @ref
		 * Configuration::MAX_ITEMS_PER_POKEMON. */
		TOO_MANY_ITEMS,
		/*! @brief Indicates that the item registry does not have enough entries to fill the Pokemon's items array. */
		NOT_ENOUGH_REGISTERED_ITEMS,
		/*! @brief Indicates that the item pool contains no selectable (non-empty, unique) entries. */
		EMPTY_ITEM_POOL,
	};
} // namespace PocketCore::Validation::Item

#endif
