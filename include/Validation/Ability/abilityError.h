/*! @file abilityError.h
	@brief Contains the error codes for the Ability validation results.
	@date 10/26/2026
	@since 0.12.51
	@version 0.12.53
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
		@date 10/26/2026
		@since 0.12.51
		@version 0.12.53
		@author Matthew Moore
	*/
	enum class AbilityError : ub
	{
		/*! @brief Indicates a Ability ID has no matching registered metadata. */
		MISSING_ABILITY_METADATA,
		/*! @brief Indicates that the duplicate Ability ID replacement value has no valid metadata. */
		DUPLICATE_REPLACEMENT_MISSING_METADATA,
		/*! @brief Indicates that the nature registry has no registered abilities. */
		NO_REGISTERED_ABILITIES,
		/*! @brief Indicates that the nature registry does not have enough entries to fill the Pokemon's abilities array. */
		NOT_ENOUGH_REGISTERED_ABILITIES,
		/*! @brief Indicates that the ability cannot be registered since one or both of the parents are not valid. */
		INVALID_BREEDING_CONTEXT,
		/*! @brief Indicates that the ability cannot be registered since both parents are the same sex. */
		SAME_SEX_PARENTS_CANT_REPRODUCE,
		/*! @brief Indicates that the ability cannot be registered since one or both of the parents cannot produce eggs. */
		PARENT_CANNOT_REPRODUCE,
		/*! @brief Indicates that the ability cannot be registered since both parents are dittos. */
		BOTH_PARENTS_ARE_DITTO,
		/*! @brief Indicates that the ability cannot be registered since the parents do not share a common egg group. */
		NO_COMMON_EGG_GROUP,
		/*! @brief Indicates that the ability cannot be registered since the a parent has no valid egg groups. */
		PARENT_HAS_NO_VALID_EGG_GROUP,
	};
} // namespace PocketCore::Validation::Ability

#endif
