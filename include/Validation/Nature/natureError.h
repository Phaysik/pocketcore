/*! @file natureError.h
	@brief Contains the error codes for the Nature validation results.
	@date 10/27/2026
	@since 0.12.51
	@version 0.12.53
	@author Matthew Moore
*/

#ifndef INCLUDE_VALIDATION_NATURE_NATURE_ERROR_H
#define INCLUDE_VALIDATION_NATURE_NATURE_ERROR_H

#include "Core/typedefs.h"

namespace PocketCore::Validation::Nature
{
	using PocketCore::Core::ub;

	/*! @enum NatureError
		@showenumvalues
		@brief Identifies failures when validating Nature metadata.
		@details Serves as the error type in Nature validation functions' std::expected results. The error codes can be extended as
		Nature validation handles additional constraints.
		@note No enumerator represents success; a successful validation result has a value and contains no error.
		All enum values must be handled exhaustively when dispatching on the error; callers must account for newly added values.
		@date 10/27/2026
		@since 0.12.51
		@version 0.12.53
		@author Matthew Moore
	*/
	enum class NatureError : ub
	{
		/*! @brief Indicates a Nature ID has no matching registered metadata. */
		MISSING_NATURE_METADATA,
		/*! @brief Indicates that the duplicate Nature ID replacement value has no valid metadata. */
		DUPLICATE_REPLACEMENT_MISSING_METADATA,
		/*! @brief Indicates that the nature registry has no registered natures. */
		NO_REGISTERED_NATURES,
		/*! @brief Indicates that to many natures were passed in compared to the amount allowed by @ref
		 * Configuration::MAX_NATURES_PER_POKEMON. */
		TOO_MANY_NATURES,
		/*! @brief Indicates that the nature registry does not have enough entries to fill the Pokemon's natures array. */
		NOT_ENOUGH_REGISTERED_NATURES,
	};
} // namespace PocketCore::Validation::Nature

#endif
