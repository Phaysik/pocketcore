/*! @file pokemon.testHelper.h
	@brief Test helper for dealing with Pokemon concepts.
	@date 09/16/2026
	@since 0.12.13
	@version 0.12.36
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
#include "Pokemon/pokemonID.h"
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
	using PocketCore::Configuration::MAX_NON_VOLATILE_STATUSES_PER_POKEMON;
	using PocketCore::Configuration::MAX_TYPES_PER_POKEMON;
	using PocketCore::Configuration::MIN_IV_STAT_VALUE;
	using PocketCore::Configuration::NATURE_STAT_BASE_MULTIPLIER;
	using PocketCore::Core::ub;
	using PocketCore::Core::us;
	using PocketCore::Item::ItemID;
	using PocketCore::Item::NO_ITEM_ID;
	using PocketCore::Move::MoveID;
	using PocketCore::Move::NO_MOVE_ID;
	using PocketCore::Nature::NatureID;
	using PocketCore::Nature::NO_NATURE_ID;
	using PocketCore::Pokemon::NO_POKEMON_ID;
	using PocketCore::Pokemon::Pokemon;
	using PocketCore::Pokemon::POKEMON_NAME_BULBASAUR;
	using PocketCore::Pokemon::POKEMON_STAT_COUNT;
	using PocketCore::Pokemon::PokemonID;
	using PocketCore::Pokemon::PokemonStats;
	using PocketCore::Status::NO_STATUS_ID;
	using PocketCore::Status::StatusID;
	using PocketCore::Type::NO_TYPE_ID;
	using PocketCore::Type::TypeID;

	struct PokemonTestData
	{
		public:
			std::array<std::array<double, POKEMON_STAT_COUNT>, MAX_NATURES_PER_POKEMON> mNatureMultipliers{
				{
					{
						NATURE_STAT_BASE_MULTIPLIER,
						NATURE_STAT_BASE_MULTIPLIER,
						NATURE_STAT_BASE_MULTIPLIER,
						NATURE_STAT_BASE_MULTIPLIER,
						NATURE_STAT_BASE_MULTIPLIER,
						NATURE_STAT_BASE_MULTIPLIER,
					},
				},
			};

			std::string_view mName{POKEMON_NAME_BULBASAUR};

			PokemonStats mStats{};

			std::array<StatusID, MAX_NON_VOLATILE_STATUSES_PER_POKEMON> mStatusIDs{NO_STATUS_ID};

			std::array<MoveID, MAX_MOVES_PER_POKEMON> mMoveIDs{NO_MOVE_ID};

			std::array<us, POKEMON_STAT_COUNT> mPokemonIVs{
				MIN_IV_STAT_VALUE, MIN_IV_STAT_VALUE, MIN_IV_STAT_VALUE,
				MIN_IV_STAT_VALUE, MIN_IV_STAT_VALUE, MIN_IV_STAT_VALUE, // NOLINT(readability-trailing-comma)
			};

			std::array<us, POKEMON_STAT_COUNT> mPokemonEVs{};

			std::array<ub, MAX_MOVES_PER_POKEMON> mMaxPP{0};
			std::array<ub, MAX_MOVES_PER_POKEMON> mCurrentPP{0};

			std::array<TypeID, MAX_TYPES_PER_POKEMON> mTypesIDs{NO_TYPE_ID};
			std::array<AbilityID, MAX_ABILITIES_PER_POKEMON> mAbilityIDs{NO_ABILITY_ID};
			std::array<ItemID, MAX_ITEMS_PER_POKEMON> mItemIDs{NO_ITEM_ID};
			std::array<NatureID, MAX_NATURES_PER_POKEMON> mNatureIDs{NO_NATURE_ID};

			us mHealth{0};

			us mLevel{1};

			PokemonID mPokemonID{NO_POKEMON_ID};
	};

	constexpr Pokemon makePokemon(const PokemonTestData &data)
	{
		Pokemon pokemon{
			data.mPokemonID,  data.mName,	 data.mMoveIDs,	 data.mMaxPP,	  data.mCurrentPP,		   data.mStats,		 data.mLevel,
			data.mAbilityIDs, data.mItemIDs, data.mTypesIDs, data.mNatureIDs, data.mNatureMultipliers, data.mPokemonIVs, data.mPokemonEVs,
		};

		pokemon.setStatusIDsArray(data.mStatusIDs);
		pokemon.setHealth(data.mHealth);

		return pokemon;
	}

} // namespace PocketCore::Testing

#endif
