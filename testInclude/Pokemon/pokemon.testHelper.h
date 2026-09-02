/*! @file pokemon.testHelper.h
	@brief Test helper for dealing with Pokemon concepts.
	@date 09/02/2026
	@since 0.12.13
	@version 0.12.17
	@author Matthew Moore
*/

#ifndef TEST_INCLUDE_POKEMON_POKEMON_TEST_HELPER_H
#define TEST_INCLUDE_POKEMON_POKEMON_TEST_HELPER_H

#include <string_view>

#include "Ability/abilityID.h"
#include "Configuration/constants.h"
#include "Core/typedefs.h"
#include "Item/itemID.h"
#include "Move/moveID.h"
#include "Nature/natureID.h"
#include "Pokemon/constants.h"
#include "Pokemon/pokemon.h"
#include "Status/statusID.h"
#include "Types/typeID.h"

namespace PocketCore::Testing
{
	using PocketCore::Ability::AbilityID;
	using PocketCore::Ability::NO_ABILITY_ID;
	using PocketCore::Configuration::MAX_ABILITIES_PER_POKEMON;
	using PocketCore::Configuration::MAX_ITEMS_PER_POKEMON;
	using PocketCore::Configuration::MAX_MOVES_PER_POKEMON;
	using PocketCore::Configuration::MAX_NATURES_PER_POKEMON;
	using PocketCore::Configuration::MAX_STATUSES_PER_POKEMON;
	using PocketCore::Configuration::MAX_TYPES_PER_POKEMON;
	using PocketCore::Core::ub;
	using PocketCore::Core::us;
	using PocketCore::Item::ItemID;
	using PocketCore::Item::NO_ITEM_ID;
	using PocketCore::Move::MoveID;
	using PocketCore::Move::NO_MOVE_ID;
	using PocketCore::Nature::NatureID;
	using PocketCore::Nature::NO_NATURE_ID;
	using PocketCore::Pokemon::Pokemon;
	using PocketCore::Pokemon::POKEMON_NAME_BULBASAUR;
	using PocketCore::Status::NO_STATUS_ID;
	using PocketCore::Status::StatusID;
	using PocketCore::Type::NO_TYPE_ID;
	using PocketCore::Type::TypeID;

	struct PokemonTestData
	{
		public:
			std::string_view mName{POKEMON_NAME_BULBASAUR};

			std::array<StatusID, MAX_STATUSES_PER_POKEMON> mStatusIDs{NO_STATUS_ID};

			std::array<MoveID, MAX_MOVES_PER_POKEMON> mMoveIDs{NO_MOVE_ID};
			std::array<ub, MAX_MOVES_PER_POKEMON> mMaxPP{0};
			std::array<ub, MAX_MOVES_PER_POKEMON> mCurrentPP{0};

			std::array<TypeID, MAX_TYPES_PER_POKEMON> mTypesIDs{NO_TYPE_ID};
			std::array<AbilityID, MAX_ABILITIES_PER_POKEMON> mAbilityIDs{NO_ABILITY_ID};
			std::array<ItemID, MAX_ITEMS_PER_POKEMON> mItemIDs{NO_ITEM_ID};
			std::array<NatureID, MAX_NATURES_PER_POKEMON> mNatureIDs{NO_NATURE_ID};

			us mHealth{0};
			us mMaximumHealth{0};
			us mAttack{0};
			us mDefense{0};
			us mSpecialAttack{0};
			us mSpecialDefense{0};
			us mSpeed{0};
			
			us mLevel{0};
	};

	constexpr Pokemon makePokemon(const PokemonTestData &data)
	{
		Pokemon pokemon{data.mName,	  data.mMoveIDs,	data.mMaxPP,		 data.mCurrentPP,	   data.mHealth,
						data.mAttack, data.mDefense,	data.mSpecialAttack, data.mSpecialDefense, data.mSpeed,
						data.mLevel,  data.mAbilityIDs, data.mItemIDs,		 data.mTypesIDs};

		pokemon.setStatusIDsArray(data.mStatusIDs);
		pokemon.setNatureIDsArray(data.mNatureIDs);
		pokemon.setMaximumHealth(data.mMaximumHealth);

		return pokemon;
	}

} // namespace PocketCore::Testing

#endif
