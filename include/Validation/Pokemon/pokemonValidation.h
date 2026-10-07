/*! @file pokemonValidation.h
	@brief Defines validation functions that help with Pokemon.
	@date 10/07/2026
	@since 0.12.48
	@version 0.12.48
	@author Matthew Moore
*/

#ifndef INCLUDE_VALIDATION_POKEMON_POKEMON_VALIDATION_H
#define INCLUDE_VALIDATION_POKEMON_POKEMON_VALIDATION_H

#include <algorithm>
#include <expected>
#include <functional>
#include <span>

#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Pokemon/pokemonMeta.h"

#include "pokemonError.h"

namespace PocketCore::Validation::Pokemon
{
	using PocketCore::Configuration::MAX_EV_STAT_VALUE;
	using PocketCore::Configuration::MAX_IV_STAT_VALUE;
	using PocketCore::Configuration::MAX_TOTAL_EV_STAT_VALUE;
	using PocketCore::Configuration::MIN_EV_STAT_VALUE;
	using PocketCore::Configuration::MIN_IV_STAT_VALUE;
	using PocketCore::Core::us;
	using PocketCore::Pokemon::POKEMON_STAT_COUNT;

	/*! @brief Checks whether an individual value is within the configured IV bounds.
		@details Accepts values from @ref MIN_IV_STAT_VALUE through @ref MAX_IV_STAT_VALUE, inclusive. This constexpr function can be
		evaluated at compile time or runtime.
		@param[in] pokemonIV The individual value to validate.
		@return An empty result when the value is valid; otherwise, @ref PokemonError::IV_BELOW_MINIMUM or
		@ref PokemonError::IV_ABOVE_MAXIMUM.
		@since 0.12.48
		@version 0.12.48
	*/
	ATTR_NODISCARD constexpr std::expected<void, PokemonError> isValidPokemonIV(const us pokemonIV)
	{
		if (pokemonIV > MAX_IV_STAT_VALUE)
		{
			return std::unexpected{PokemonError::IV_ABOVE_MAXIMUM};
		}

		if (pokemonIV < MIN_IV_STAT_VALUE)
		{
			return std::unexpected{PokemonError::IV_BELOW_MINIMUM};
		}

		return {};
	}

	/*! @brief Checks whether every individual value in a Pokemon stat array is valid.
		@details Validates each element against the configured IV bounds. This constexpr function can be evaluated at compile time or
		runtime.
		@param[in] pokemonIVArray The fixed-size array view to validate; its extent is @ref POKEMON_STAT_COUNT.
		@return An empty result when every IV is valid; otherwise, @ref PokemonError::INVALID_IV_ARRAY.
		@since 0.12.48
		@version 0.12.48
	*/
	ATTR_NODISCARD constexpr std::expected<void, PokemonError> isValidPokemonIVArray(
		const std::span<const us, POKEMON_STAT_COUNT> pokemonIVArray)
	{
		if (std::ranges::any_of(pokemonIVArray, [](const us pokemonIV) { return !isValidPokemonIV(pokemonIV).has_value(); }))
		{
			return std::unexpected{PokemonError::INVALID_IV_ARRAY};
		}

		return {};
	}

	/*! @brief Checks whether an effort value and its aggregate are within the configured EV limits.
		@details Validates the value against the per-stat range from @ref MIN_EV_STAT_VALUE through @ref MAX_EV_STAT_VALUE, inclusive,
		and checks that adding it to @p statTotal does not exceed @ref MAX_TOTAL_EV_STAT_VALUE. This constexpr function can be evaluated
		at compile time or runtime.
		@param[in] pokemonEV The effort value to validate.
		@param[in] statTotal The sum of the other stat EVs to include in the aggregate limit check.
		@return An empty result when both limits are satisfied; otherwise, @ref PokemonError::EV_BELOW_MINIMUM,
		@ref PokemonError::EV_ABOVE_STAT_MAXIMUM, or @ref PokemonError::EV_ABOVE_TOTAL_MAXIMUM.
		@since 0.12.48
		@version 0.12.48
	*/
	ATTR_NODISCARD constexpr std::expected<void, PokemonError> isValidPokemonEV(const us pokemonEV, const us statTotal)
	{
		if (pokemonEV > MAX_EV_STAT_VALUE)
		{
			return std::unexpected{PokemonError::EV_ABOVE_STAT_MAXIMUM};
		}

		if (pokemonEV < MIN_EV_STAT_VALUE)
		{
			return std::unexpected{PokemonError::EV_BELOW_MINIMUM};
		}

		if (statTotal + pokemonEV > MAX_TOTAL_EV_STAT_VALUE)
		{
			return std::unexpected{PokemonError::EV_ABOVE_TOTAL_MAXIMUM};
		}

		return {};
	}

	/*! @brief Checks whether every effort value in a Pokemon stat array is valid.
		@details Validates each element against the configured per-stat EV range and checks that the array's total does not exceed
		@ref MAX_TOTAL_EV_STAT_VALUE. This constexpr function can be evaluated at compile time or runtime.
		@param[in] pokemonEVArray The fixed-size array view to validate; its extent is @ref POKEMON_STAT_COUNT.
		@return An empty result when every EV and the aggregate total are valid; otherwise, @ref PokemonError::INVALID_EV_ARRAY.
		@since 0.12.48
		@version 0.12.48
	*/
	ATTR_NODISCARD constexpr std::expected<void, PokemonError> isValidPokemonEVArray(
		const std::span<const us, POKEMON_STAT_COUNT> pokemonEVArray)
	{
		const us sum{static_cast<us>(std::ranges::fold_left(pokemonEVArray, 0, std::plus<>()))};

		if (std::ranges::any_of(pokemonEVArray,
								[sum](const us pokemonEV) { return !isValidPokemonEV(pokemonEV, sum - pokemonEV).has_value(); }))
		{
			return std::unexpected{PokemonError::INVALID_EV_ARRAY};
		}

		return {};
	}
} // namespace PocketCore::Validation::Pokemon

#endif
