/*! @file pokemon.testHelper.h
	@brief Test helper for dealing with Pokemon concepts.
	@date 08/31/2026
	@since 0.12.13
	@version 0.12.13
	@author Matthew Moore
*/

#ifndef TEST_INCLUDE_POKEMON_POKEMON_TEST_HELPER_H
#define TEST_INCLUDE_POKEMON_POKEMON_TEST_HELPER_H

#include <string_view>

#include "Ability/abilityID.h"
#include "Configuration/constants.h"
#include "Core/typedefs.h"
#include "Item/itemID.h"
#include "Pokemon/constants.h"
#include "Pokemon/pokemon.h"
#include "Types/typeID.h"

namespace PocketCore::Testing
{
	using PocketCore::Ability::AbilityID;
	using PocketCore::Ability::NO_ABILITY_ID;
	using PocketCore::Configuration::MAX_ABILITIES_PER_POKEMON;
	using PocketCore::Configuration::MAX_ITEMS_PER_POKEMON;
	using PocketCore::Configuration::MAX_TYPES_PER_POKEMON;
	using PocketCore::Core::us;
	using PocketCore::Item::ItemID;
	using PocketCore::Item::NO_ITEM_ID;
	using PocketCore::Pokemon::Pokemon;
	using PocketCore::Pokemon::POKEMON_NAME_BULBASAUR;
	using PocketCore::Type::NO_TYPE_ID;
	using PocketCore::Type::TypeID;

	struct PokemonTestData
	{
		public:
			std::string_view mName{POKEMON_NAME_BULBASAUR};

			std::array<TypeID, MAX_TYPES_PER_POKEMON> mTypes{NO_TYPE_ID};

			us mAttack{0};
			us mDefense{0};
			us mHealth{0};
			us mSpeed{0};
			us mSpecialAttack{0};
			us mSpecialDefense{0};
			us mLevel{0};

			std::array<AbilityID, MAX_ABILITIES_PER_POKEMON> mAbilities{NO_ABILITY_ID};
			std::array<ItemID, MAX_ITEMS_PER_POKEMON> mItems{NO_ITEM_ID};
	};

	constexpr Pokemon makePokemon(const PokemonTestData &data)
	{
		return Pokemon{data.mName,			data.mAttack,		  data.mDefense, data.mHealth,	  data.mSpeed,
					   data.mSpecialAttack, data.mSpecialDefense, data.mLevel,	 data.mAbilities, data.mItems};
	}

} // namespace PocketCore::Testing

#endif
