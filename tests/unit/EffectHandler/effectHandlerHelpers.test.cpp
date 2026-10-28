/*! @file effectHandlerHelpers.test.cpp
	@brief C++ file for running tests for the effect handler helper free functions.
	@date 10/28/2026
	@since 0.12.53
	@version 0.12.53
	@author Matthew Moore
*/

#include "EffectHandler/effectHandlerHelpers.h"

#include <memory>
#include <span>
#include <vector>

#include "Ability/abilityID.h"
#include "Ability/builtInAbilityID.h"
#include "Battle/battleState.h"
#include "Battle/battleState.testHelper.h"
#include "Configuration/abilityRegistryConfiguration.h"
#include "Configuration/itemRegistryConfiguration.h"
#include "Configuration/natureRegistryConfiguration.h"
#include "Configuration/typeRegistryConfiguration.h"
#include "Effect/effectContext.h"
#include "Item/builtInItemID.h"
#include "Item/itemID.h"
#include "Nature/builtInNatureID.h"
#include "Nature/natureID.h"
#include "Pokemon/pokemon.h"
#include "Pokemon/pokemon.testHelper.h"
#include "Registry/registryProvider.h"
#include "Type/builtInTypeID.h"
#include "Type/typeID.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Ability::AbilityID;
using PocketCore::Ability::BuiltinAbilityID;
using PocketCore::Ability::toAbilityID;
using PocketCore::Battle::BattleSlot;
using PocketCore::Battle::BattleState;
using PocketCore::Configuration::AbilityRegistryConfiguration;
using PocketCore::Configuration::ItemRegistryConfiguration;
using PocketCore::Configuration::NatureRegistryConfiguration;
using PocketCore::Configuration::TypeRegistryConfiguration;
using PocketCore::Effect::battleSlotHasAbilityByID;
using PocketCore::Effect::battleSlotHasAbilityByName;
using PocketCore::Effect::battleSlotHasItemByID;
using PocketCore::Effect::battleSlotHasItemByName;
using PocketCore::Effect::battleSlotHasNatureByID;
using PocketCore::Effect::battleSlotHasNatureByName;
using PocketCore::Effect::battleSlotHasTypeByID;
using PocketCore::Effect::battleSlotHasTypeByName;
using PocketCore::Effect::getTeam;
using PocketCore::Effect::isBattleSlotGrounded;
using PocketCore::Effect::isBattleSlotUngrounded;
using PocketCore::Effect::Side;
using PocketCore::Item::BuiltinItemID;
using PocketCore::Item::ItemID;
using PocketCore::Item::toItemID;
using PocketCore::Nature::BuiltinNatureID;
using PocketCore::Nature::NatureID;
using PocketCore::Nature::toNatureID;
using PocketCore::Pokemon::Pokemon;
using PocketCore::Registry::RegistryProvider;
using PocketCore::Testing::makeBattleState;
using PocketCore::Testing::makePokemon;
using PocketCore::Type::BuiltinTypeID;
using PocketCore::Type::toTypeID;
using PocketCore::Type::TypeID;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

SCENARIO("getTeam")
{
	Pokemon sideA{makePokemon({})};
	Pokemon sideB{makePokemon({})};

	BattleSlot slotSideA{.mPokemon = &sideA, .mPosition = 0};
	BattleSlot slotSideB{.mPokemon = &sideB, .mPosition = 1};

	GIVEN("mutable caller")
	{
		BattleState state{
			makeBattleState({.mSideA = {slotSideA}, .mSideB = {slotSideB}}),
		};

		WHEN("calling with Side A")
		{
			std::vector<BattleSlot> &aTeam{getTeam(state, Side::A)};

			THEN("the resulting team is returned and mutable")
			{
				CHECK((slotSideA == aTeam.at(0)));

				BattleSlot &slot{aTeam.at(0)};

				slot.mPosition = 2;

				CHECK((aTeam.at(0).mPosition == 2));
			}
		}

		WHEN("calling with Side B")
		{
			std::vector<BattleSlot> &bTeam{getTeam(state, Side::B)};

			THEN("the resulting team is returned and mutable")
			{
				CHECK((slotSideB == bTeam.at(0)));

				BattleSlot &slot{bTeam.at(0)};

				slot.mPosition = 5;

				CHECK((bTeam.at(0).mPosition == 5));
			}
		}
	}

	GIVEN("immutable caller")
	{
		const BattleState state{
			makeBattleState({.mSideA = {slotSideA}, .mSideB = {slotSideB}}),
		};

		WHEN("calling with Side A")
		{
			std::span<const BattleSlot> aTeam{getTeam(state, Side::A)};

			THEN("the resulting team is returned and is not mutable")
			{
				CHECK((slotSideA == aTeam.at(0)));
			}
		}

		WHEN("calling with Side B")
		{
			std::span<const BattleSlot> bTeam{getTeam(state, Side::B)};

			THEN("the resulting team is returned and is not mutable")
			{
				CHECK((slotSideB == bTeam.at(0)));
			}
		}
	}
}

SCENARIO("battleSlotHasTypeByName")
{
	Pokemon pokemon{makePokemon({.mMetadata = {.mTypeIDs = {toTypeID(BuiltinTypeID::Dragon), toTypeID(BuiltinTypeID::Water)}}})};

	BattleSlot slot{.mPokemon = &pokemon};

	std::unique_ptr<TypeRegistryConfiguration> configRegistryStorage{std::make_unique<TypeRegistryConfiguration>()};
	RegistryProvider provider{.mTypeRegistry = &configRegistryStorage->getRuntimeRegistry()};

	std::string_view expectedName{"Dragon"};

	WHEN("calling with a null pokemon")
	{
		slot.mPokemon = nullptr;

		THEN("it returns false")
		{
			CHECK_FALSE(battleSlotHasTypeByName(slot, provider, expectedName));
		}
	}

	WHEN("calling with a null type registry")
	{
		provider.mTypeRegistry = nullptr;

		THEN("it returns false")
		{
			CHECK_FALSE(battleSlotHasTypeByName(slot, provider, expectedName));
		}
	}

	WHEN("calling with an invalid type ID")
	{
		pokemon.setTypeID(0, TypeID{200});

		THEN("it returns false")
		{
			CHECK_FALSE(battleSlotHasTypeByName(slot, provider, expectedName));
		}
	}

	WHEN("calling with a name that's not in the type IDs array")
	{
		THEN("it returns false")
		{
			CHECK_FALSE(battleSlotHasTypeByName(slot, provider, "Grass"));
		}
	}

	WHEN("calling with a name that's in the type IDs array")
	{
		THEN("it returns true")
		{
			CHECK(battleSlotHasTypeByName(slot, provider, expectedName));
		}
	}
}

SCENARIO("battleSlotHasTypeByID")
{
	Pokemon pokemon{makePokemon({.mMetadata = {.mTypeIDs = {toTypeID(BuiltinTypeID::Dragon), toTypeID(BuiltinTypeID::Water)}}})};

	BattleSlot slot{.mPokemon = &pokemon};

	TypeID expectedID{toTypeID(BuiltinTypeID::Dragon)};

	WHEN("calling with a null pokemon")
	{
		slot.mPokemon = nullptr;

		THEN("it returns false")
		{
			CHECK_FALSE(battleSlotHasTypeByID(slot, expectedID));
		}
	}

	WHEN("calling with a ID that's not in the type IDs array")
	{
		THEN("it returns false")
		{
			CHECK_FALSE(battleSlotHasTypeByID(slot, toTypeID(BuiltinTypeID::Grass)));
		}
	}

	WHEN("calling with a ID that's in the type IDs array")
	{
		THEN("it returns true")
		{
			CHECK(battleSlotHasTypeByID(slot, expectedID));
		}
	}
}

SCENARIO("battleSlotHasAbilityByName")
{
	Pokemon pokemon{makePokemon({.mAbilityEntries = {{{.mAbilityID = toAbilityID(BuiltinAbilityID::Drizzle)}}}})};

	BattleSlot slot{.mPokemon = &pokemon};

	std::unique_ptr<AbilityRegistryConfiguration> configRegistryStorage{std::make_unique<AbilityRegistryConfiguration>()};
	RegistryProvider provider{.mAbilityRegistry = &configRegistryStorage->getRuntimeRegistry()};

	std::string_view expectedName{"Drizzle"};

	WHEN("calling with a null pokemon")
	{
		slot.mPokemon = nullptr;

		THEN("it returns false")
		{
			CHECK_FALSE(battleSlotHasAbilityByName(slot, provider, expectedName));
		}
	}

	WHEN("calling with a null ability registry")
	{
		provider.mAbilityRegistry = nullptr;

		THEN("it returns false")
		{
			CHECK_FALSE(battleSlotHasAbilityByName(slot, provider, expectedName));
		}
	}

	WHEN("calling with an invalid ability ID")
	{
		pokemon.setAbilityEntry(0, {.mAbilityID = AbilityID{200}});

		THEN("it returns false")
		{
			CHECK_FALSE(battleSlotHasAbilityByName(slot, provider, expectedName));
		}
	}

	WHEN("calling with a name that's not in the ability IDs array")
	{
		THEN("it returns false")
		{
			CHECK_FALSE(battleSlotHasAbilityByName(slot, provider, "Cloud Nine"));
		}
	}

	WHEN("calling with a name that's in the ability IDs array")
	{
		THEN("it returns true")
		{
			CHECK(battleSlotHasAbilityByName(slot, provider, expectedName));
		}
	}
}

SCENARIO("battleSlotHasAbilityByID")
{
	Pokemon pokemon{makePokemon({.mAbilityEntries = {{{.mAbilityID = toAbilityID(BuiltinAbilityID::Drizzle)}}}})};

	BattleSlot slot{.mPokemon = &pokemon};

	AbilityID expectedID{toAbilityID(BuiltinAbilityID::Drizzle)};

	WHEN("calling with a null pokemon")
	{
		slot.mPokemon = nullptr;

		THEN("it returns false")
		{
			CHECK_FALSE(battleSlotHasAbilityByID(slot, expectedID));
		}
	}

	WHEN("calling with a ID that's not in the ability IDs array")
	{
		THEN("it returns false")
		{
			CHECK_FALSE(battleSlotHasAbilityByID(slot, toAbilityID(BuiltinAbilityID::AirLock)));
		}
	}

	WHEN("calling with a ID that's in the ability IDs array")
	{
		THEN("it returns true")
		{
			CHECK(battleSlotHasAbilityByID(slot, expectedID));
		}
	}
}

SCENARIO("battleSlotHasItemByName")
{
	Pokemon pokemon{makePokemon({.mItemIDs = {toItemID(BuiltinItemID::CheriBerry)}})};

	BattleSlot slot{.mPokemon = &pokemon};

	std::unique_ptr<ItemRegistryConfiguration> configRegistryStorage{std::make_unique<ItemRegistryConfiguration>()};
	RegistryProvider provider{.mItemRegistry = &configRegistryStorage->getRuntimeRegistry()};

	std::string_view expectedName{"Cheri Berry"};

	WHEN("calling with a null pokemon")
	{
		slot.mPokemon = nullptr;

		THEN("it returns false")
		{
			CHECK_FALSE(battleSlotHasItemByName(slot, provider, expectedName));
		}
	}

	WHEN("calling with a null item registry")
	{
		provider.mItemRegistry = nullptr;

		THEN("it returns false")
		{
			CHECK_FALSE(battleSlotHasItemByName(slot, provider, expectedName));
		}
	}

	WHEN("calling with an invalid item ID")
	{
		pokemon.setItemID(0, ItemID{200});

		THEN("it returns false")
		{
			CHECK_FALSE(battleSlotHasItemByName(slot, provider, expectedName));
		}
	}

	WHEN("calling with a name that's not in the item IDs array")
	{
		THEN("it returns false")
		{
			CHECK_FALSE(battleSlotHasItemByName(slot, provider, "Chesto Berry"));
		}
	}

	WHEN("calling with a name that's in the item IDs array")
	{
		THEN("it returns true")
		{
			CHECK(battleSlotHasItemByName(slot, provider, expectedName));
		}
	}
}

SCENARIO("battleSlotHasItemByID")
{
	Pokemon pokemon{makePokemon({.mItemIDs = {toItemID(BuiltinItemID::CheriBerry)}})};

	BattleSlot slot{.mPokemon = &pokemon};

	ItemID expectedID{toItemID(BuiltinItemID::CheriBerry)};

	WHEN("calling with a null pokemon")
	{
		slot.mPokemon = nullptr;

		THEN("it returns false")
		{
			CHECK_FALSE(battleSlotHasItemByID(slot, expectedID));
		}
	}

	WHEN("calling with a ID that's not in the item IDs array")
	{
		THEN("it returns false")
		{
			CHECK_FALSE(battleSlotHasItemByID(slot, toItemID(BuiltinItemID::ChestoBerry)));
		}
	}

	WHEN("calling with a ID that's in the item IDs array")
	{
		THEN("it returns true")
		{
			CHECK(battleSlotHasItemByID(slot, expectedID));
		}
	}
}

SCENARIO("battleSlotHasNatureByName")
{
	Pokemon pokemon{makePokemon({.mNatureIDs = {toNatureID(BuiltinNatureID::Docile)}})};

	BattleSlot slot{.mPokemon = &pokemon};

	std::unique_ptr<NatureRegistryConfiguration> configRegistryStorage{std::make_unique<NatureRegistryConfiguration>()};
	RegistryProvider provider{.mNatureRegistry = &configRegistryStorage->getRuntimeRegistry()};

	std::string_view expectedName{"Docile"};

	WHEN("calling with a null pokemon")
	{
		slot.mPokemon = nullptr;

		THEN("it returns false")
		{
			CHECK_FALSE(battleSlotHasNatureByName(slot, provider, expectedName));
		}
	}

	WHEN("calling with a null nature registry")
	{
		provider.mNatureRegistry = nullptr;

		THEN("it returns false")
		{
			CHECK_FALSE(battleSlotHasNatureByName(slot, provider, expectedName));
		}
	}

	WHEN("calling with an invalid nature ID")
	{
		Pokemon badNatureIDPokemon{makePokemon({.mNatureIDs = {NatureID{200}}})};

		slot.mPokemon = &badNatureIDPokemon;

		THEN("it returns false")
		{
			CHECK_FALSE(battleSlotHasNatureByName(slot, provider, expectedName));
		}
	}

	WHEN("calling with a name that's not in the nature IDs array")
	{
		THEN("it returns false")
		{
			CHECK_FALSE(battleSlotHasNatureByName(slot, provider, "Adamant"));
		}
	}

	WHEN("calling with a name that's in the nature IDs array")
	{
		THEN("it returns true")
		{
			CHECK(battleSlotHasNatureByName(slot, provider, expectedName));
		}
	}
}

SCENARIO("battleSlotHasNatureByID")
{
	Pokemon pokemon{makePokemon({.mNatureIDs = {toNatureID(BuiltinNatureID::Docile)}})};

	BattleSlot slot{.mPokemon = &pokemon};

	NatureID expectedID{toNatureID(BuiltinNatureID::Docile)};

	WHEN("calling with a null pokemon")
	{
		slot.mPokemon = nullptr;

		THEN("it returns false")
		{
			CHECK_FALSE(battleSlotHasNatureByID(slot, expectedID));
		}
	}

	WHEN("calling with a ID that's not in the nature IDs array")
	{
		THEN("it returns false")
		{
			CHECK_FALSE(battleSlotHasNatureByID(slot, toNatureID(BuiltinNatureID::Brave)));
		}
	}

	WHEN("calling with a ID that's in the nature IDs array")
	{
		THEN("it returns true")
		{
			CHECK(battleSlotHasNatureByID(slot, expectedID));
		}
	}
}

SCENARIO("isBattleSlotUngrounded")
{
	Pokemon pokemon{makePokemon({.mMetadata = {.mTypeIDs = {toTypeID(BuiltinTypeID::Dragon)}}})};

	BattleSlot slot{.mPokemon = &pokemon};

	WHEN("calling with a grounded slot")
	{
		slot.mIsGrounded = true;

		THEN("it returns false")
		{
			CHECK_FALSE(isBattleSlotUngrounded(slot));
		}
	}

	WHEN("calling with a Pokemon that isn't flying type")
	{
		THEN("it returns false")
		{
			CHECK_FALSE(isBattleSlotUngrounded(slot));
		}
	}

	WHEN("calling with a Pokemon that is flying type")
	{
		pokemon.setTypeID(0, toTypeID(BuiltinTypeID::Flying));

		THEN("it returns true")
		{
			CHECK(isBattleSlotUngrounded(slot));
		}
	}
}

SCENARIO("isBattleSlotGrounded")
{
	Pokemon pokemon{makePokemon({.mMetadata = {.mTypeIDs = {toTypeID(BuiltinTypeID::Dragon)}}})};

	BattleSlot slot{.mPokemon = &pokemon};

	WHEN("calling with a grounded slot")
	{
		slot.mIsGrounded = true;

		THEN("it returns true")
		{
			CHECK(isBattleSlotGrounded(slot));
		}
	}

	WHEN("calling with a Pokemon that isn't flying type")
	{
		THEN("it returns true")
		{
			CHECK(isBattleSlotGrounded(slot));
		}
	}

	WHEN("calling with a Pokemon that is flying type")
	{
		pokemon.setTypeID(0, toTypeID(BuiltinTypeID::Flying));

		THEN("it returns false")
		{
			CHECK_FALSE(isBattleSlotGrounded(slot));
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
