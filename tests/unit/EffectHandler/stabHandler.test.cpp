/*! @file stabHandler.test.cpp
	@brief C++ file for running tests for the StabHandler.
	@date 08/03/2026
	@since 0.8.7
	@version 0.9.0
	@author Matthew Moore
*/

#include "EffectHandler/stabHandler.h"

#include <array>
#include <cmath>

#include "Ability/abilityID.h"
#include "Battle/battleState.h"
#include "Configuration/constants.h"
#include "Effect/effectContext.h"
#include "Item/itemID.h"
#include "Multiplier/builtInMultiplierID.h"
#include "Pokemon/pokemon.h"
#include "Registry/registryProvider.h"
#include "Types/builtInTypeID.h"
#include "Types/typeID.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Ability::NO_ABILITY_ID;
using PocketCore::Battle::BattleSlot;
using PocketCore::Battle::BattleState;
using PocketCore::Configuration::MAX_TYPES_PER_POKEMON;
using PocketCore::Configuration::STAB_HIT_MULTIPLIER_VALUE;
using PocketCore::Effect::EffectContext;
using PocketCore::Effect::Side;
using PocketCore::Effect::StabHandler;
using PocketCore::Item::NO_ITEM_ID;
using PocketCore::Multiplier::BuiltinMultiplierID;
using PocketCore::Multiplier::toMultiplierID;
using PocketCore::Pokemon::Pokemon;
using PocketCore::Registry::RegistryProvider;
using PocketCore::Type::BuiltInTypeID;
using PocketCore::Type::NO_TYPE_ID;
using PocketCore::Type::toTypeID;
using PocketCore::Type::TypeID;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,llvm-prefer-static-over-anonymous-namespace)

namespace
{
	Pokemon makePokemonWithTypes(const std::array<TypeID, MAX_TYPES_PER_POKEMON> &types)
	{
		return Pokemon{"UnitMon", 100U, 100U, 100U, 100U, 100U, 100U, 50U, {NO_ABILITY_ID}, {NO_ITEM_ID}, types};
	}

	RegistryProvider makeNullProvider()
	{
		return RegistryProvider{
			.abilityRegistry = nullptr,
			.moveRegistry = nullptr,
			.itemRegistry = nullptr,
			.typeRegistry = nullptr,
			.statusRegistry = nullptr,
			.weatherRegistry = nullptr,
			.terrainRegistry = nullptr,
			.multiplierRegistry = nullptr,
			.natureRegistry = nullptr,
			.pokemonRegistry = nullptr,
		};
	}
} // namespace

SCENARIO("StabHandler")
{
	StabHandler stabHandler{};
	RegistryProvider provider{makeNullProvider()};

	GIVEN("a user Pokemon with a matching move type")
	{
		Pokemon userPokemon{makePokemonWithTypes({toTypeID(BuiltInTypeID::Fire), NO_TYPE_ID})};

		BattleState battleState{};
		battleState.mSideA.push_back(BattleSlot{.mPokemon = &userPokemon});

		EffectContext effectContext{};
		effectContext.mUserSide = Side::A;
		effectContext.mUserIndex = 0;
		effectContext.mMoveTypeID = toTypeID(BuiltInTypeID::Fire);

		WHEN("applying STAB handling")
		{
			stabHandler.apply(battleState, effectContext, provider);

			THEN("the STAB multiplier is recorded")
			{
				const auto activeMultipliers{effectContext.getActiveMultipliers()};
				REQUIRE((activeMultipliers.size() == 1U));
				CHECK((activeMultipliers.front().first == toMultiplierID(BuiltinMultiplierID::Stab)));
				CHECK((std::fabs(activeMultipliers.front().second - STAB_HIT_MULTIPLIER_VALUE) < 0.0001));
			}
		}
	}

	GIVEN("a user Pokemon without a matching move type")
	{
		Pokemon userPokemon{makePokemonWithTypes({toTypeID(BuiltInTypeID::Water), NO_TYPE_ID})};

		BattleState battleState{};
		battleState.mSideA.push_back(BattleSlot{.mPokemon = &userPokemon});

		EffectContext effectContext{};
		effectContext.mUserSide = Side::A;
		effectContext.mUserIndex = 0;
		effectContext.mMoveTypeID = toTypeID(BuiltInTypeID::Fire);

		WHEN("applying STAB handling")
		{
			stabHandler.apply(battleState, effectContext, provider);

			THEN("no STAB multiplier is added")
			{
				CHECK(effectContext.getActiveMultipliers().empty());
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,llvm-prefer-static-over-anonymous-namespace)
