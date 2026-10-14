/*! @file pokemonIO.test.cpp
	@brief C++ file for running tests pertaining to the input/output operations on Pokemon.
	@date 10/14/2026
	@since 0.12.51
	@version 0.12.51
	@author Matthew Moore
*/

#include "Utility/IO/Pokemon/pokemonIO.h"

#include <cstddef>
#include <sstream>

#include "Core/typedefs.h"
#include "Nature/builtInNatureID.h"
#include "Pokemon/pokemon.h"
#include "Pokemon/pokemon.testHelper.h"
#include "Type/builtInTypeID.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Core::ub;
using PocketCore::Nature::BuiltinNatureID;
using PocketCore::Nature::toNatureID;
using PocketCore::Pokemon::Pokemon;
using PocketCore::Testing::makePokemon;
using PocketCore::Type::BuiltinTypeID;
using PocketCore::Type::toTypeID;
using PocketCore::Utility::IO::Pokemon::outputPokemonDataID;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("Pokemon IO functions")
{
	GIVEN("outputPokemonDataID")
	{
		GIVEN("data with two values")
		{
			Pokemon pokemon{
				makePokemon({.mTypesIDs = {toTypeID(BuiltinTypeID::Fire), toTypeID(BuiltinTypeID::Bug)}}),
			};

			WHEN("the data is properly written to the stream")
			{
				std::ostringstream output;
				for (std::size_t i{0}; i < pokemon.getTypeIDsArray().size(); ++i)
				{
					outputPokemonDataID(
						output, [&pokemon](const ub typeIndex) { return pokemon.getTypeID(typeIndex); }, static_cast<ub>(i));
				}

				THEN("all public state is written with numeric identifiers and PP")
				{
					std::string_view expected{"2, 12"};

					CHECK((output.str() == expected));
				}
			}
		}

		GIVEN("data with one value")
		{
			Pokemon pokemon{
				makePokemon({.mNatureIDs = {toNatureID(BuiltinNatureID::Hasty)}}),
			};

			WHEN("the data is properly written to the stream")
			{
				std::ostringstream output;
				for (std::size_t i{0}; i < pokemon.getNatureIDsArray().size(); ++i)
				{
					outputPokemonDataID(
						output, [&pokemon](const ub natureID) { return pokemon.getNatureID(natureID); }, static_cast<ub>(i));
				}

				THEN("all public state is written with numeric identifiers and PP")
				{
					std::string_view expected{"12"};

					CHECK((output.str() == expected));
				}
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
