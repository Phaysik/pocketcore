/*! @file pokemonStats.test.cpp
	@brief C++ file for running tests for the PokemonStats.
	@date 10/13/2026
	@since 0.12.51
	@version 0.12.51
	@author Matthew Moore
*/

#include "Pokemon/pokemonStats.h"

#include <cstddef>

#include <catch2/catch_test_macros.hpp>

using PocketCore::Core::us;
using PocketCore::Pokemon::POKEMON_STAT_COUNT;
using PocketCore::Pokemon::PokemonStat;
using PocketCore::Pokemon::PokemonStats;
using PocketCore::Pokemon::toIndex;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("toIndex")
{
	WHEN("Health")
	{
		std::size_t healthIndex{toIndex(PokemonStat::Health)};

		THEN("it properly resolves to 0")
		{
			CHECK((healthIndex == 0));
		}
	}

	WHEN("Attack")
	{
		std::size_t attackIndex{toIndex(PokemonStat::Attack)};

		THEN("it properly resolves to 1")
		{
			CHECK((attackIndex == 1));
		}
	}

	WHEN("Defense")
	{
		std::size_t defenseIndex{toIndex(PokemonStat::Defense)};

		THEN("it properly resolves to 2")
		{
			CHECK((defenseIndex == 2));
		}
	}

	WHEN("Special Attack")
	{
		std::size_t spAttackIndex{toIndex(PokemonStat::SpecialAttack)};

		THEN("it properly resolves to 3")
		{
			CHECK((spAttackIndex == 3));
		}
	}

	WHEN("Special Defense")
	{
		std::size_t spDefenseIndex{toIndex(PokemonStat::SpecialDefense)};

		THEN("it properly resolves to 4")
		{
			CHECK((spDefenseIndex == 4));
		}
	}

	WHEN("Speed")
	{
		std::size_t speedIndex{toIndex(PokemonStat::Speed)};

		THEN("it properly resolves to 5")
		{
			CHECK((speedIndex == 5));
		}
	}

	WHEN("Pokemon stat count")
	{
		std::size_t totalStatCount{POKEMON_STAT_COUNT};

		THEN("it properly resolves to 6")
		{
			CHECK((totalStatCount == 6));
		}
	}
}

SCENARIO("PokemonStats")
{
	PokemonStats context{};

	WHEN("operator==")
	{
		GIVEN("two default constructed Pokemon Stats")
		{
			PokemonStats other{};

			THEN("they are equal")
			{
				CHECK((context == other));
			}
		}

		GIVEN("for mMaxHealth modified in one Pokemon stats")
		{
			PokemonStats other{.mMaxHealth = 50};

			THEN("they are not equal")
			{
				CHECK((context != other));
			}
		}

		GIVEN("for mAttack modified in one Pokemon stats")
		{
			PokemonStats other{.mAttack = 1};

			THEN("they are not equal")
			{
				CHECK((context != other));
			}
		}

		GIVEN("for mDefense modified in one Pokemon stats")
		{
			PokemonStats other{.mDefense = 12};

			THEN("they are not equal")
			{
				CHECK((context != other));
			}
		}

		GIVEN("for mSpAttack modified in one Pokemon stats")
		{
			PokemonStats other{.mSpAttack = 15};

			THEN("they are not equal")
			{
				CHECK((context != other));
			}
		}

		GIVEN("for mSpDefense modified in one Pokemon stats")
		{
			PokemonStats other{.mSpDefense = 54};

			THEN("they are not equal")
			{
				CHECK((context != other));
			}
		}

		GIVEN("for mSpeed modified in one Pokemon stats")
		{
			PokemonStats other{.mSpeed = 23};

			THEN("they are not equal")
			{
				CHECK((context != other));
			}
		}

		GIVEN("two Pokemon Stats modified the same way")
		{
			PokemonStats other{.mAttack = 2};
			context.mAttack = 2;

			THEN("they are equal")
			{
				CHECK((context == other));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
