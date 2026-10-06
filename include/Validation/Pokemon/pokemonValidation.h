/*! @file pokemonValidation.h
	@brief Defines validation functions that help with Pokemon.
	@date 10/06/2026
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

	ATTR_NODISCARD constexpr std::expected<void, PokemonError> isValidPokemonIVArray(
		const std::span<us, POKEMON_STAT_COUNT> &pokemonIVArray)
	{
		if (std::ranges::any_of(pokemonIVArray, [](const us pokemonIV) { return !isValidPokemonIV(pokemonIV).has_value(); }))
		{
			return std::unexpected{PokemonError::INVALID_IV_ARRAY};
		}

		return {};
	}

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

	ATTR_NODISCARD constexpr std::expected<void, PokemonError> isValidPokemonEVArray(
		const std::span<us, POKEMON_STAT_COUNT> &pokemonEVArray)
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
