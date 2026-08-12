#include "EffectHandler/terrainHandler.h"

#include <array>
#include <cmath>

#include "Ability/abilityID.h"
#include "Battle/battleState.h"
#include "Configuration/constants.h"
#include "Effect/effectContext.h"
#include "Item/itemID.h"
#include "Pokemon/pokemon.h"
#include "Registry/registryProvider.h"
#include "Terrain/builtInTerrainID.h"
#include "Types/builtInTypeID.h"
#include "Types/typeID.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Ability::NO_ABILITY_ID;
using PocketCore::Battle::BattleSlot;
using PocketCore::Battle::BattleState;
using PocketCore::Configuration::ELECTRIC_BUFF_IN_TERRAIN_BASE_DAMAGE_VALUE;
using PocketCore::Configuration::MAX_TYPES_PER_POKEMON;
using PocketCore::Configuration::PSYCHIC_BUFF_IN_TERRAIN_BASE_DAMAGE_VALUE;
using PocketCore::Effect::EffectContext;
using PocketCore::Effect::Side;
using PocketCore::Effect::TerrainHandler;
using PocketCore::Item::NO_ITEM_ID;
using PocketCore::Pokemon::Pokemon;
using PocketCore::Registry::RegistryProvider;
using PocketCore::Terrain::BuiltinTerrainID;
using PocketCore::Terrain::toTerrainID;
using PocketCore::Types::BuiltInTypeID;
using PocketCore::Types::NO_TYPE_ID;
using PocketCore::Types::toTypeID;
using PocketCore::Types::TypeID;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,llvm-prefer-static-over-anonymous-namespace)

namespace
{
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
			.effectRegistry = nullptr,
		};
	}

	Pokemon makePokemonWithTypes(const std::array<TypeID, MAX_TYPES_PER_POKEMON> &types)
	{
		return Pokemon{"UnitMon", 100U, 90U, 100U, 80U, 100U, 90U, 50U, NO_ABILITY_ID, NO_ITEM_ID, types};
	}
} // namespace

SCENARIO("TerrainHandler")
{
	TerrainHandler terrainHandler{};
	RegistryProvider provider{makeNullProvider()};

	GIVEN("electric terrain, an electric move, and a grounded target")
	{
		Pokemon userPokemon{makePokemonWithTypes({toTypeID(BuiltInTypeID::Electric), NO_TYPE_ID})};
		Pokemon groundedTarget{makePokemonWithTypes({toTypeID(BuiltInTypeID::Normal), NO_TYPE_ID})};

		BattleState battleState{};
		battleState.mTerrainID = toTerrainID(BuiltinTerrainID::Electric);
		battleState.mSideA.push_back(BattleSlot{.mPokemon = &userPokemon});
		battleState.mSideB.push_back(BattleSlot{.mPokemon = &groundedTarget});

		EffectContext effectContext{};
		effectContext.mUserSide = Side::A;
		effectContext.mTargetSide = Side::B;
		effectContext.mUserIndex = 0;
		effectContext.mTargetIndex = 0;
		effectContext.mMoveTypeID = toTypeID(BuiltInTypeID::Electric);

		WHEN("terrain effects are applied")
		{
			terrainHandler.apply(battleState, effectContext, provider);

			THEN("the terrain attack boost is applied")
			{
				CHECK((
					std::fabs(battleState.mSideA.at(0).mDamageFormulaModifiers.mAttackModifier - ELECTRIC_BUFF_IN_TERRAIN_BASE_DAMAGE_VALUE)
					< 0.0001));
			}
		}
	}

	GIVEN("electric terrain, an electric move, and an ungrounded flying-type target")
	{
		Pokemon userPokemon{makePokemonWithTypes({toTypeID(BuiltInTypeID::Electric), NO_TYPE_ID})};
		Pokemon flyingTarget{makePokemonWithTypes({toTypeID(BuiltInTypeID::Flying), NO_TYPE_ID})};

		BattleState battleState{};
		battleState.mTerrainID = toTerrainID(BuiltinTerrainID::Electric);
		battleState.mSideA.push_back(BattleSlot{.mPokemon = &userPokemon});
		battleState.mSideB.push_back(BattleSlot{.mPokemon = &flyingTarget});

		EffectContext effectContext{};
		effectContext.mUserSide = Side::A;
		effectContext.mTargetSide = Side::B;
		effectContext.mUserIndex = 0;
		effectContext.mTargetIndex = 0;
		effectContext.mMoveTypeID = toTypeID(BuiltInTypeID::Electric);

		WHEN("terrain effects are applied")
		{
			terrainHandler.apply(battleState, effectContext, provider);

			THEN("the terrain attack boost is not applied")
			{
				CHECK((std::fabs(battleState.mSideA.at(0).mDamageFormulaModifiers.mAttackModifier - 1.0) < 0.0001));
			}
		}
	}

	GIVEN("psychic terrain and a psychic move against a grounded target")
	{
		Pokemon userPokemon{makePokemonWithTypes({toTypeID(BuiltInTypeID::Psychic), NO_TYPE_ID})};
		Pokemon groundedTarget{makePokemonWithTypes({toTypeID(BuiltInTypeID::Normal), NO_TYPE_ID})};

		BattleState battleState{};
		battleState.mTerrainID = toTerrainID(BuiltinTerrainID::Psychic);
		battleState.mSideA.push_back(BattleSlot{.mPokemon = &userPokemon});
		battleState.mSideB.push_back(BattleSlot{.mPokemon = &groundedTarget});

		EffectContext effectContext{};
		effectContext.mUserSide = Side::A;
		effectContext.mTargetSide = Side::B;
		effectContext.mUserIndex = 0;
		effectContext.mTargetIndex = 0;
		effectContext.mMoveTypeID = toTypeID(BuiltInTypeID::Psychic);

		WHEN("terrain effects are applied")
		{
			terrainHandler.apply(battleState, effectContext, provider);

			THEN("the psychic terrain attack boost is applied")
			{
				CHECK(
					(std::fabs(battleState.mSideA.at(0).mDamageFormulaModifiers.mAttackModifier - PSYCHIC_BUFF_IN_TERRAIN_BASE_DAMAGE_VALUE)
					 < 0.0001));
			}
		}
	}

	GIVEN("electric terrain and an intrinsically ungrounded flying target")
	{
		Pokemon userPokemon{makePokemonWithTypes({toTypeID(BuiltInTypeID::Electric), NO_TYPE_ID})};
		Pokemon flyingTarget{makePokemonWithTypes({toTypeID(BuiltInTypeID::Flying), NO_TYPE_ID})};

		BattleState battleState{};
		battleState.mTerrainID = toTerrainID(BuiltinTerrainID::Electric);
		battleState.mSideA.push_back(BattleSlot{.mPokemon = &userPokemon});
		battleState.mSideB.push_back(BattleSlot{.mPokemon = &flyingTarget});

		EffectContext effectContext{};
		effectContext.mUserSide = Side::A;
		effectContext.mTargetSide = Side::B;
		effectContext.mUserIndex = 0;
		effectContext.mTargetIndex = 0;
		effectContext.mMoveTypeID = toTypeID(BuiltInTypeID::Electric);

		WHEN("terrain effects are applied")
		{
			terrainHandler.apply(battleState, effectContext, provider);

			THEN("the terrain attack boost is not applied")
			{
				CHECK((std::fabs(battleState.mSideA.at(0).mDamageFormulaModifiers.mAttackModifier - 1.0) < 0.0001));
			}
		}
	}

	GIVEN("electric terrain and a flying target explicitly forced grounded")
	{
		Pokemon userPokemon{makePokemonWithTypes({toTypeID(BuiltInTypeID::Electric), NO_TYPE_ID})};
		Pokemon flyingTarget{makePokemonWithTypes({toTypeID(BuiltInTypeID::Flying), NO_TYPE_ID})};

		BattleState battleState{};
		battleState.mTerrainID = toTerrainID(BuiltinTerrainID::Electric);
		battleState.mSideA.push_back(BattleSlot{.mPokemon = &userPokemon});
		battleState.mSideB.push_back(BattleSlot{.mPokemon = &flyingTarget, .mIsGrounded = true});

		EffectContext effectContext{};
		effectContext.mUserSide = Side::A;
		effectContext.mTargetSide = Side::B;
		effectContext.mUserIndex = 0;
		effectContext.mTargetIndex = 0;
		effectContext.mMoveTypeID = toTypeID(BuiltInTypeID::Electric);

		WHEN("terrain effects are applied")
		{
			terrainHandler.apply(battleState, effectContext, provider);

			THEN("the terrain attack boost is applied")
			{
				CHECK((
					std::fabs(battleState.mSideA.at(0).mDamageFormulaModifiers.mAttackModifier - ELECTRIC_BUFF_IN_TERRAIN_BASE_DAMAGE_VALUE)
					< 0.0001));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,llvm-prefer-static-over-anonymous-namespace)
