/*! @file pokemonValidation.test.cpp
	@brief C++ file for running tests for the Pokemon validation functions.
	@date 10/07/2026
	@since 0.12.48
	@version 0.12.48
	@author Matthew Moore
*/

#include "Validation/Pokemon/pokemonValidation.h"

#include <array>
#include <expected>

#include "Core/typedefs.h"
#include "Pokemon/pokemonMeta.h"
#include "Validation/Pokemon/pokemonError.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Core::us;
using PocketCore::Pokemon::POKEMON_STAT_COUNT;
using PocketCore::Validation::Pokemon::isValidPokemonEV;
using PocketCore::Validation::Pokemon::isValidPokemonEVArray;
using PocketCore::Validation::Pokemon::isValidPokemonIV;
using PocketCore::Validation::Pokemon::isValidPokemonIVArray;
using PocketCore::Validation::Pokemon::PokemonError;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("isValidPokemonIV")
{
	GIVEN("an IV of 0")
	{
		std::expected<void, PokemonError> result{isValidPokemonIV(0)};

		THEN("the IV is valid")
		{
			REQUIRE(result.has_value());
		}
	}

	GIVEN("an IV of 31")
	{
		std::expected<void, PokemonError> result{isValidPokemonIV(31)};

		THEN("the IV is valid")
		{
			REQUIRE(result.has_value());
		}
	}

	GIVEN("an IV of 32")
	{
		std::expected<void, PokemonError> result{isValidPokemonIV(32)};

		THEN("the IV isn't valid")
		{
			REQUIRE_FALSE(result.has_value());
			CHECK((result.error() == PokemonError::IV_ABOVE_MAXIMUM));
		}
	}
}

SCENARIO("isValidPokemonIVArray")
{
	GIVEN("an array of 0 IVs")
	{
		std::array<us, POKEMON_STAT_COUNT> ivs{};
		ivs.fill(0);

		std::expected<void, PokemonError> result{isValidPokemonIVArray(ivs)};

		THEN("the IV array is valid")
		{
			REQUIRE(result.has_value());
		}
	}

	GIVEN("an array of 31 IVs")
	{
		std::array<us, POKEMON_STAT_COUNT> ivs{};
		ivs.fill(31);

		std::expected<void, PokemonError> result{isValidPokemonIVArray(ivs)};

		THEN("the IV array is valid")
		{
			REQUIRE(result.has_value());
		}
	}

	GIVEN("an array of 32 IVs")
	{
		std::array<us, POKEMON_STAT_COUNT> ivs{};
		ivs.fill(32);

		std::expected<void, PokemonError> result{isValidPokemonIVArray(ivs)};

		THEN("the IV array is not valid")
		{
			REQUIRE_FALSE(result.has_value());
			CHECK((result.error() == PokemonError::INVALID_IV_ARRAY));
		}
	}
}

SCENARIO("isValidPokemonEV")
{
	GIVEN("an EV of 0")
	{
		std::expected<void, PokemonError> result{isValidPokemonEV(0, 0)};

		THEN("the EV is valid")
		{
			REQUIRE(result.has_value());
		}
	}

	GIVEN("an EV of 252")
	{
		std::expected<void, PokemonError> result{isValidPokemonEV(252, 0)};

		THEN("the EV is valid")
		{
			REQUIRE(result.has_value());
		}
	}

	GIVEN("an EV of 253")
	{
		us pokemonEV{253};
		std::expected<void, PokemonError> result{isValidPokemonEV(pokemonEV, 0)};

		THEN("the EV isn't valid")
		{
			REQUIRE_FALSE(result.has_value());
			CHECK((result.error() == PokemonError::EV_ABOVE_STAT_MAXIMUM));
		}
	}

	GIVEN("an EV of 10 with a EV stat total of 501")
	{
		std::expected<void, PokemonError> result{isValidPokemonEV(10, 501)};

		THEN("the EV isn't valid")
		{
			REQUIRE_FALSE(result.has_value());
			CHECK((result.error() == PokemonError::EV_ABOVE_TOTAL_MAXIMUM));
		}
	}
}

SCENARIO("isValidPokemonEVArray")
{
	GIVEN("an array of 0 EVs")
	{
		std::array<us, POKEMON_STAT_COUNT> evs{};
		evs.fill(0);

		std::expected<void, PokemonError> result{isValidPokemonEVArray(evs)};

		THEN("the EV is valid")
		{
			REQUIRE(result.has_value());
		}
	}

	GIVEN("an array with two 252 values and one value of 6")
	{
		std::array<us, POKEMON_STAT_COUNT> evs{};
		evs.fill(0);
		evs.at(0) = 252;
		evs.at(1) = 252;
		evs.at(2) = 6;

		std::expected<void, PokemonError> result{isValidPokemonEVArray(evs)};

		THEN("the EV is valid")
		{
			REQUIRE(result.has_value());
		}
	}

	GIVEN("an array with two 252 values and one value of 7")
	{
		std::array<us, POKEMON_STAT_COUNT> evs{};
		evs.fill(0);
		evs.at(0) = 252;
		evs.at(1) = 252;
		evs.at(2) = 7;

		std::expected<void, PokemonError> result{isValidPokemonEVArray(evs)};

		THEN("the EV isn't valid")
		{
			REQUIRE_FALSE(result.has_value());
			CHECK((result.error() == PokemonError::INVALID_EV_ARRAY));
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
