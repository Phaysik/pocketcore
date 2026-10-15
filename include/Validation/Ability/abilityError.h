/*! @file abilityError.h
	@brief Contains the error codes for the Ability validation results.
	@date 10/09/2026
	@since 0.12.51
	@version 0.12.51
	@author Matthew Moore
*/

#ifndef INCLUDE_VALIDATION_ABILITY_ABILITY_ERROR_H
#define INCLUDE_VALIDATION_ABILITY_ABILITY_ERROR_H

#include "Core/typedefs.h"

namespace PocketCore::Validation::Ability
{
	using PocketCore::Core::ub;

	/*! @enum AbilityError
		@showenumvalues
		@brief Identifies failures when validating Ability metadata.
		@details Serves as the error type in Ability validation functions' std::expected results. The error codes can be extended as
		Ability validation handles additional constraints.
		@note No enumerator represents success; a successful validation result has a value and contains no error.
		All enum values must be handled exhaustively when dispatching on the error; callers must account for newly added values.
		@date 10/09/2026
		@since 0.12.51
		@version 0.12.51
		@author Matthew Moore
	*/
	enum class AbilityError : ub
	{
		/*! @brief Indicates a Ability ID has no matching registered metadata. */
		MISSING_ABILITY_METADATA,
	};
} // namespace PocketCore::Validation::Ability

#endif
