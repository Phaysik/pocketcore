/*! @file pokemon.testHelper.h
	@brief Test helper for dealing with Pokemon concepts.
	@date 10/26/2026
	@since 0.12.13
	@version 0.12.53
	@author Matthew Moore
*/

#ifndef TEST_INCLUDE_POKEMON_POKEMON_TEST_HELPER_H
#define TEST_INCLUDE_POKEMON_POKEMON_TEST_HELPER_H

#include "Ability/abilityID.h"
#include "Configuration/constants.h"
#include "Core/typedefs.h"
#include "EggGroup/builtInEggGroupID.h"
#include "Item/itemID.h"
#include "Learnset/learnsetID.h"
#include "Move/builtInMoveID.h"
#include "Move/moveID.h"
#include "Move/moveMeta.h"
#include "Nature/natureID.h"
#include "Pokemon/constants.h"
#include "Pokemon/pokemon.h"
#include "Pokemon/pokemonID.h"
#include "Pokemon/pokemonMeta.h"
#include "Pokemon/pokemonStats.h"
#include "Registry/natureRegistry.h"
#include "Status/statusID.h"
#include "Type/typeID.h"

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
	using PocketCore::EggGroup::NO_EGG_GROUP_ID;
	using PocketCore::Item::ItemID;
	using PocketCore::Item::NO_ITEM_ID;
	using PocketCore::Learnset::LearnsetID;
	using PocketCore::Learnset::NO_LEARNSET_ID;
	using PocketCore::Move::MoveID;
	using PocketCore::Move::MoveMeta;
	using PocketCore::Move::NO_MOVE_ID;
	using PocketCore::Nature::NatureID;
	using PocketCore::Nature::NO_NATURE_ID;
	using PocketCore::Pokemon::NO_POKEMON_ID;
	using PocketCore::Pokemon::Pokemon;
	using PocketCore::Pokemon::POKEMON_NAME_BULBASAUR;
	using PocketCore::Pokemon::POKEMON_STAT_COUNT;
	using PocketCore::Pokemon::PokemonGender;
	using PocketCore::Pokemon::PokemonID;
	using PocketCore::Pokemon::PokemonMeta;
	using PocketCore::Pokemon::PokemonStats;
	using PocketCore::Registry::Nature::NatureRegistry;
	using PocketCore::Status::NO_STATUS_ID;
	using PocketCore::Status::StatusID;
	using PocketCore::Type::NO_TYPE_ID;
	using PocketCore::Type::TypeID;

	struct PokemonTestData
	{
		public:
			const NatureRegistry *mNatureRegistry{nullptr};

			PokemonMeta mMetadata{
				.mName = std::string(POKEMON_NAME_BULBASAUR),
				.mBaseStats = {},
				.mEggGroupIDs = {NO_EGG_GROUP_ID},
				.mPokemonID = NO_POKEMON_ID,
				.mLearnsetID = NO_LEARNSET_ID,
			};

			std::array<MoveMeta, MAX_MOVES_PER_POKEMON> mMoveMetas{
				{
					{
						.mMoveID = NO_MOVE_ID,
						.mPPMaxAmount = 0,
						.mPPDefaultAmount = 0,
					},
				},
			};

			std::array<StatusID, MAX_NON_VOLATILE_STATUSES_PER_POKEMON> mStatusIDs{NO_STATUS_ID};

			std::array<us, POKEMON_STAT_COUNT> mPokemonIVs{
				MIN_IV_STAT_VALUE, MIN_IV_STAT_VALUE, MIN_IV_STAT_VALUE,
				MIN_IV_STAT_VALUE, MIN_IV_STAT_VALUE, MIN_IV_STAT_VALUE, // NOLINT(readability-trailing-comma)
			};

			std::array<us, POKEMON_STAT_COUNT> mPokemonEVs{};

			std::array<AbilityID, MAX_ABILITIES_PER_POKEMON> mAbilityIDs{NO_ABILITY_ID};
			std::array<ItemID, MAX_ITEMS_PER_POKEMON> mItemIDs{NO_ITEM_ID};
			std::array<NatureID, MAX_NATURES_PER_POKEMON> mNatureIDs{NO_NATURE_ID};

			us mHealth{0};

			us mLevel{1};

			PokemonGender mGender{};
	};

	inline const NatureRegistry &getDefaultTestNatureRegistry()
	{
		static const NatureRegistry registry{};
		return registry;
	}

	constexpr Pokemon makePokemon(const PokemonTestData &data)
	{
		const NatureRegistry &natureRegistry{data.mNatureRegistry != nullptr ? *data.mNatureRegistry : getDefaultTestNatureRegistry()};

		Pokemon pokemon{
			data.mMetadata,	 data.mMoveMetas, data.mLevel,		data.mAbilityIDs, data.mItemIDs,
			data.mNatureIDs, natureRegistry,  data.mPokemonIVs, data.mPokemonEVs, data.mGender,
		};

		pokemon.setStatusIDsArray(data.mStatusIDs);
		pokemon.setHealth(data.mHealth);

		return pokemon;
	}

	constexpr bool checkStats(const Pokemon &pokemon, const us expectedMaxHP, const us expectedHP, const us expectedAttack,
							  const us expectedDefense, const us expectedSpAttack, const us expectedSpDefense, const us expectedSpeed)
	{
		return pokemon.getMaximumHealth() == expectedMaxHP || pokemon.getHealth() == expectedHP || pokemon.getAttack() == expectedAttack
			|| pokemon.getDefense() == expectedDefense || pokemon.getSpAttack() == expectedSpAttack
			|| pokemon.getSpDefense() == expectedSpDefense || pokemon.getSpeed() == expectedSpeed;
	}

} // namespace PocketCore::Testing

#endif
