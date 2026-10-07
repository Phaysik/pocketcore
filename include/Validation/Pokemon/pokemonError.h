/*! @file pokemonError.h
	@brief Contains the error codes for the Pokemon validation results.
	@date 10/07/2026
	@since 0.12.48
	@version 0.12.48
	@author Matthew Moore
*/

#ifndef INCLUDE_VALIDATION_POKEMON_POKEMON_ERROR_H
#define INCLUDE_VALIDATION_POKEMON_POKEMON_ERROR_H

#include "Core/typedefs.h"

namespace PocketCore::Validation::Pokemon
{
	using PocketCore::Core::ub;

	/*! @enum PokemonError
		@showenumvalues
		@brief Identifies validation failures in Pokemon creation and configuration.
		@details Serves as the error type in the validation functions' std::expected results. Each enumerator describes a rejected input
		or violated constraint. The error codes can be extended as validation expands to additional Pokemon creation parameters.
		@note No enumerator represents success; a successful validation result has a value and contains no error.
		All enum values must be handled exhaustively when dispatching on the error; callers must account for newly added values.
		@date 10/07/2026
		@since 0.12.48
		@version 0.12.48
		@author Matthew Moore
	*/
	enum class PokemonError : ub
	{
		/*! @brief Indicates the passed in IV is below the set minimum. */
		IV_BELOW_MINIMUM,
		/*! @brief Indicates the passed in IV is above the set maximum. */
		IV_ABOVE_MAXIMUM,
		/*! @brief Indicates the passed in EV is below the set minimum. */
		EV_BELOW_MINIMUM,
		/*! @brief Indicates the passed in EV is above the set maximum. */
		EV_ABOVE_STAT_MAXIMUM,
		/*! @brief Indicates the passed in EV would go above the maximum EV total for the pokemon. */
		EV_ABOVE_TOTAL_MAXIMUM,
		/*! @brief Indicates the passed in IV array has some invalid value. */
		INVALID_IV_ARRAY,
		/*! @brief Indicates the passed in EV array has some invalid value. */
		INVALID_EV_ARRAY,
	};
} // namespace PocketCore::Validation::Pokemon

#endif
