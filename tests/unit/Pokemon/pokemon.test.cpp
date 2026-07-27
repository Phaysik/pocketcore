#include "Pokemon/pokemon.h"

#include <array>

#include "Ability/abilityID.h"
#include "Configuration/constants.h"
#include "Item/itemID.h"
#include "Types/typeID.h"
#include "Types/types.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Ability::AbilityID;
using PocketCore::Configuration::MAX_TYPES_PER_POKEMON;
using PocketCore::Item::ItemID;
using PocketCore::Pokemon::Pokemon;
using PocketCore::Types::NO_TYPE_ID;
using PocketCore::Types::toTypeID;
using PocketCore::Types::TypeID;
using PocketCore::Types::Types;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("Pokemon type identifiers")
{
	GIVEN("a Pokemon constructed without explicit types")
	{
		Pokemon pokemon{"MissingNo", 1, 1, 1, 1, 1, 1, AbilityID{}, ItemID{}};

		THEN("both type slots are unassigned")
		{
			CHECK((pokemon.getTypesArray().size() == MAX_TYPES_PER_POKEMON));
			CHECK((pokemon.getTypeID(0) == NO_TYPE_ID));
			CHECK((pokemon.getTypeID(1) == NO_TYPE_ID));
		}
	}

	GIVEN("built-in and custom stable type identifiers")
	{
		TypeID builtInTypeID{toTypeID(Types::Fire)};
		TypeID customTypeID{42};
		std::array<TypeID, MAX_TYPES_PER_POKEMON> typeIDs{builtInTypeID, customTypeID};
		Pokemon pokemon{"Hybrid", 1, 1, 1, 1, 1, 1, AbilityID{}, ItemID{}, typeIDs};

		THEN("the constructor preserves both identifiers")
		{
			CHECK((pokemon.getTypeID(0) == builtInTypeID));
			CHECK((pokemon.getTypeID(1) == customTypeID));
		}

		WHEN("one type slot is replaced")
		{
			TypeID replacementTypeID{toTypeID(Types::Water)};
			pokemon.setType(1, replacementTypeID);

			THEN("only that slot changes")
			{
				CHECK((pokemon.getTypeID(0) == builtInTypeID));
				CHECK((pokemon.getTypeID(1) == replacementTypeID));
			}
		}

		WHEN("the complete type array is replaced")
		{
			std::array<TypeID, MAX_TYPES_PER_POKEMON> replacementTypeIDs{toTypeID(Types::Grass), TypeID{43}};
			pokemon.setTypesArray(replacementTypeIDs);

			THEN("the replacement array is returned unchanged")
			{
				CHECK((pokemon.getTypesArray() == replacementTypeIDs));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)