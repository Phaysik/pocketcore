/*! @file battleHelpers.test.cpp
	@brief C++ file for running tests for the Battle helper functions.
	@date 10/15/2026
	@since 0.12.45
	@version 0.12.51
	@author Matthew Moore
*/

#include "Battle/battleHelpers.h"

#include <algorithm>
#include <limits>
#include <sstream>
#include <vector>

#include "Ability/builtInAbilityID.h"
#include "Battle/battleAction.h"
#include "Battle/battleState.h"
#include "Battle/battleState.testHelper.h"
#include "Battle/battleTargetsAndTriggers.h"
#include "Configuration/constants.h"
#include "Core/typedefs.h"
#include "Effect/effectContext.h"
#include "Effect/effectContext.testHelper.h"
#include "Effect/effectSourceAndSuppression.h"
#include "Item/builtInItemID.h"
#include "Move/builtInMoveID.h"
#include "Move/moveHitPolicy.h"
#include "Move/moveMeta.h"
#include "Nature/builtInNatureID.h"
#include "Pokemon/builtInPokemonID.h"
#include "Pokemon/pokemon.h"
#include "Pokemon/pokemon.testHelper.h"
#include "Pokemon/pokemonStats.h"
#include "Registry/registryProvider.h"
#include "Registry/registryProvider.testHelper.h"
#include "Status/builtInStatusID.h"
#include "Status/volatileStatus.h"
#include "Type/builtInTypeID.h"
#include "Utility/Math/floatUtility.h"
#include "Utility/random.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Ability::BuiltinAbilityID;
using PocketCore::Ability::toAbilityID;
using PocketCore::Battle::activeSlots;
using PocketCore::Battle::amountOfHealthyPokemonInParty;
using PocketCore::Battle::anyPartyPokemonNull;
using PocketCore::Battle::applyRecoil;
using PocketCore::Battle::BattleRangeID;
using PocketCore::Battle::BattleSlot;
using PocketCore::Battle::BattleState;
using PocketCore::Battle::BattleTarget;
using PocketCore::Battle::contextSlot;
using PocketCore::Battle::DamageFormulaModifiers;
using PocketCore::Battle::getEffectiveSpeed;
using PocketCore::Battle::getOppositeSide;
using PocketCore::Battle::getSideOrder;
using PocketCore::Battle::hasDuplicatePokemonPointers;
using PocketCore::Battle::isAdjacent;
using PocketCore::Battle::isHealthy;
using PocketCore::Battle::makeMoveContext;
using PocketCore::Battle::MoveAction;
using PocketCore::Battle::party;
using PocketCore::Battle::printBattleSlotWithNames;
using PocketCore::Battle::resolveHitCount;
using PocketCore::Battle::sideHasHealthyPokemon;
using PocketCore::Battle::StatStages;
using PocketCore::Battle::targetExists;
using PocketCore::Configuration::MAX_VOLATILE_STATUSES_PER_POKEMON;
using PocketCore::Core::sl;
using PocketCore::Core::ub;
using PocketCore::Core::us;
using PocketCore::Effect::EffectContext;
using PocketCore::Effect::EffectSource;
using PocketCore::Effect::Side;
using PocketCore::Item::BuiltinItemID;
using PocketCore::Item::toItemID;
using PocketCore::Move::BuiltinMoveID;
using PocketCore::Move::FixedHitCount;
using PocketCore::Move::MoveMeta;
using PocketCore::Move::toMoveID;
using PocketCore::Move::WeightedHitCount;
using PocketCore::Nature::BuiltinNatureID;
using PocketCore::Nature::toNatureID;
using PocketCore::Pokemon::BuiltinPokemonID;
using PocketCore::Pokemon::Pokemon;
using PocketCore::Pokemon::POKEMON_STAT_COUNT;
using PocketCore::Pokemon::toPokemonID;
using PocketCore::Registry::RegistryProvider;
using PocketCore::Status::BuiltinStatusID;
using PocketCore::Status::toStatusID;
using PocketCore::Status::VolatileStatus;
using PocketCore::Testing::getDefaultInitializedRegistryProvider;
using PocketCore::Testing::getNullStatusRegistryProvider;
using PocketCore::Testing::makeBattleState;
using PocketCore::Testing::makeEffectContext;
using PocketCore::Testing::makePokemon;
using PocketCore::Type::BuiltinTypeID;
using PocketCore::Type::toTypeID;
using PocketCore::Utility::Math::approximatelyEqualAbsRel;
using PocketCore::Utility::Random;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("Battle free function")
{
	GIVEN("getOppositeSide")
	{
		WHEN("calling with Side A")
		{
			THEN("it returns Side B")
			{
				CHECK((getOppositeSide(Side::A) == Side::B));
			}
		}

		WHEN("calling with Side B")
		{
			THEN("it returns Side A")
			{
				CHECK((getOppositeSide(Side::B) == Side::A));
			}
		}
	}

	GIVEN("getSideOrder")
	{
		WHEN("calling with Side A")
		{
			THEN("it returns 0")
			{
				CHECK((getSideOrder(Side::A) == 0));
			}
		}

		WHEN("calling with Side B")
		{
			THEN("it returns 1")
			{
				CHECK((getSideOrder(Side::B) == 1));
			}
		}
	}

	GIVEN("anyPartyPokemonNull")
	{
		WHEN("calling with no null Pokemon")
		{
			Pokemon member1{makePokemon({})};
			Pokemon member2{makePokemon({})};
			Pokemon member3{makePokemon({})};
			Pokemon member4{makePokemon({})};
			Pokemon member5{makePokemon({})};
			Pokemon member6{makePokemon({})};
			std::vector<Pokemon *> party{&member1, &member2, &member3, &member4, &member5, &member6};

			bool result{anyPartyPokemonNull(party)};

			THEN("it returns false")
			{
				CHECK((result == false));
			}
		}

		WHEN("calling with some null Pokemon")
		{
			Pokemon member1{makePokemon({})};
			Pokemon *member2{nullptr};
			Pokemon member3{makePokemon({})};
			Pokemon *member4{nullptr};
			Pokemon member5{makePokemon({})};
			Pokemon member6{makePokemon({})};
			std::vector<Pokemon *> party{&member1, member2, &member3, member4, &member5, &member6};

			bool result{anyPartyPokemonNull(party)};

			THEN("it returns true")
			{
				CHECK((result == true));
			}
		}
	}

	GIVEN("activeSlots")
	{
		BattleState state{makeBattleState({.mSideA = {{{.mPosition = 2}}}, .mSideB = {{{.mPosition = 4}}}})};

		GIVEN("non-const overload")
		{
			WHEN("calling with Side A")
			{
				std::vector<BattleSlot> &result{activeSlots(state, Side::A)};

				THEN("it returns the appropriate vector of modifiable battleSlots")
				{
					std::vector<BattleSlot> expected{{.mPosition = 2}};

					CHECK(std::ranges::equal(result, expected));

					result.at(0).mIsFlinched = true;
					expected.at(0).mIsFlinched = true;

					std::vector<BattleSlot> &proveModifiable{activeSlots(state, Side::A)};

					CHECK(std::ranges::equal(proveModifiable, expected));
				}
			}

			WHEN("calling with Side B")
			{
				std::vector<BattleSlot> &result{activeSlots(state, Side::B)};

				THEN("it returns the appropriate vector of modifiable battleSlots")
				{
					std::vector<BattleSlot> expected{{.mPosition = 4}};

					CHECK(std::ranges::equal(result, expected));

					result.at(0).mIsFlinched = true;
					expected.at(0).mIsFlinched = true;

					std::vector<BattleSlot> &proveModifiable{activeSlots(state, Side::B)};

					CHECK(std::ranges::equal(proveModifiable, expected));
				}
			}
		}

		GIVEN("const overload")
		{
			WHEN("calling with Side A")
			{
				const std::span<const BattleSlot> result{activeSlots(state, Side::A)};

				THEN("it returns the appropriate vector of modifiable battleSlots")
				{
					std::vector<BattleSlot> expected{{.mPosition = 2}};

					CHECK(std::ranges::equal(result, expected));
				}
			}

			WHEN("calling with Side B")
			{
				const std::span<const BattleSlot> result{activeSlots(state, Side::B)};

				THEN("it returns the appropriate vector of modifiable battleSlots")
				{
					std::vector<BattleSlot> expected{{.mPosition = 4}};

					CHECK(std::ranges::equal(result, expected));
				}
			}
		}
	}

	GIVEN("contextSlot")
	{
		BattleState state{makeBattleState({.mSideA = {{{.mPosition = 1}, {.mPosition = 2}, {.mPosition = 3}}}})};
		std::vector<BattleSlot> &slots{activeSlots(state, Side::A)};
		BattleSlot &slot1{slots.at(0)};

		GIVEN("calling with a slotIndex that is larger than the amount of BattleSlots")
		{
			BattleSlot *result{contextSlot(state, Side::A, 3)};

			THEN("it returns nullptr")
			{
				CHECK((result == nullptr));
			}
		}

		GIVEN("calling with a slotIndex that is within bounds")
		{
			BattleSlot *result{contextSlot(state, Side::A, 0)};

			THEN("it returns a modifiable BattleSlot")
			{
				REQUIRE((result != nullptr));
				CHECK((result == &slot1));

				result->mPosition = 3;

				CHECK((slot1.mPosition == 3));
			}
		}
	}

	GIVEN("party")
	{
		Pokemon partyA1{makePokemon({.mPokemonID = toPokemonID(BuiltinPokemonID::Charizard)})};
		Pokemon partyB1{makePokemon({.mPokemonID = toPokemonID(BuiltinPokemonID::Blastoise)})};
		BattleState state{makeBattleState({.mPartyA = {&partyA1}, .mPartyB = {&partyB1}})};

		GIVEN("non-const overload")
		{
			WHEN("calling with Side A")
			{
				std::vector<Pokemon *> &result{party(state, Side::A)};

				THEN("it returns the appropriate vector of modifiable battleSlots")
				{
					std::vector<Pokemon *> expected{&partyA1};

					REQUIRE((result.at(0) != nullptr));
					CHECK(std::ranges::equal(result, expected));

					result.at(0)->setAbilityID(0, toAbilityID(BuiltinAbilityID::CloudNine));
					expected.at(0)->setItemID(0, toItemID(BuiltinItemID::CheriBerry));

					std::vector<Pokemon *> &proveModifiable{party(state, Side::A)};

					CHECK(std::ranges::equal(proveModifiable, expected));
				}
			}

			WHEN("calling with Side B")
			{
				std::vector<Pokemon *> &result{party(state, Side::B)};

				THEN("it returns the appropriate vector of modifiable battleSlots")
				{
					std::vector<Pokemon *> expected{&partyB1};

					REQUIRE((result.at(0) != nullptr));
					CHECK(std::ranges::equal(result, expected));

					result.at(0)->setAbilityID(0, toAbilityID(BuiltinAbilityID::CloudNine));
					expected.at(0)->setItemID(0, toItemID(BuiltinItemID::CheriBerry));

					std::vector<Pokemon *> &proveModifiable{party(state, Side::B)};

					CHECK(std::ranges::equal(proveModifiable, expected));
				}
			}
		}

		GIVEN("const overload")
		{
			WHEN("calling with Side A")
			{
				const std::span<const Pokemon *const> result{party(state, Side::A)};

				THEN("it returns the appropriate vector of modifiable battleSlots")
				{
					const std::vector<const Pokemon *> expected{&partyA1};

					REQUIRE((result.at(0) != nullptr));
					CHECK(std::ranges::equal(result, expected));
				}
			}

			WHEN("calling with Side B")
			{
				const std::span<const Pokemon *const> result{party(state, Side::B)};

				THEN("it returns the appropriate vector of modifiable battleSlots")
				{
					const std::vector<const Pokemon *> expected{&partyB1};

					REQUIRE((result.at(0) != nullptr));
					CHECK(std::ranges::equal(result, expected));
				}
			}
		}
	}

	GIVEN("isHealthy")
	{
		Pokemon pokemon{makePokemon({.mHealth = 20, .mPokemonID = toPokemonID(BuiltinPokemonID::Blastoise)})};

		GIVEN("BattleSlot overload")
		{
			WHEN("calling with a Pokemon nullptr")
			{
				BattleSlot slot{.mPokemon = nullptr};

				THEN("it returns false")
				{
					CHECK_FALSE(isHealthy(slot));
				}
			}

			WHEN("calling with a non-fainted Pokemon")
			{
				BattleSlot slot{.mPokemon = &pokemon};

				THEN("it returns true")
				{
					CHECK(isHealthy(slot));
				}
			}

			WHEN("calling with a fainted Pokemon")
			{
				pokemon.setHealth(0);
				BattleSlot slot{.mPokemon = &pokemon};

				THEN("it returns false")
				{
					CHECK_FALSE(isHealthy(slot));
				}
			}
		}

		GIVEN("Pokemon overload")
		{
			WHEN("calling with a Pokemon nullptr")
			{
				Pokemon *nullPokemon{nullptr};

				THEN("it returns false")
				{
					CHECK_FALSE(isHealthy(nullPokemon));
				}
			}

			WHEN("calling with a non-fainted Pokemon")
			{
				THEN("it returns true")
				{
					CHECK(isHealthy(&pokemon));
				}
			}

			WHEN("calling with a fainted Pokemon")
			{
				pokemon.setHealth(0);

				THEN("it returns false")
				{
					CHECK_FALSE(isHealthy(&pokemon));
				}
			}
		}
	}

	GIVEN("isActive")
	{
		Pokemon member1{makePokemon({})};
		Pokemon member2{makePokemon({})};
		Pokemon member3{makePokemon({})};
		Pokemon member4{makePokemon({})};

		BattleState state{makeBattleState({.mSideA = {{.mPokemon = &member1}, {.mPokemon = &member2}, {.mPokemon = &member3}}})};

		GIVEN("calling with a pokemon that is not in the BattleSlot's active side")
		{
			THEN("it returns false")
			{
				CHECK_FALSE(isActive(state, Side::A, &member4));
			}
		}

		GIVEN("calling with a pokemon that is in the BattleSlot's active side")
		{
			THEN("it returns true")
			{
				CHECK(isActive(state, Side::A, &member2));
			}
		}
	}

	GIVEN("isAdjacent")
	{
		Pokemon member1{makePokemon({})};
		Pokemon member2{makePokemon({})};
		Pokemon member3{makePokemon({})};

		BattleSlot source{.mPokemon = &member1, .mPosition = 1};

		GIVEN("calling with a target whose position is not adjacent")
		{
			BattleSlot target{.mPokemon = &member2, .mPosition = 3};

			THEN("it returns false")
			{
				CHECK_FALSE(isAdjacent(source, target));
			}
		}

		GIVEN("calling with a target whose position is adjacent to the right")
		{
			BattleSlot target{.mPokemon = &member2, .mPosition = 2};

			THEN("it returns true")
			{
				CHECK(isAdjacent(source, target));
			}
		}

		GIVEN("calling with a target whose position is the same")
		{
			BattleSlot target{.mPokemon = &member2, .mPosition = 1};

			THEN("it returns true")
			{
				CHECK(isAdjacent(source, target));
			}
		}

		GIVEN("calling with targets that are adjacent on both sides")
		{
			source.mPosition = 2;
			BattleSlot target1{.mPokemon = &member2, .mPosition = 1};
			BattleSlot target2{.mPokemon = &member3, .mPosition = 3};

			THEN("it returns true")
			{
				CHECK(isAdjacent(source, target1));
				CHECK(isAdjacent(source, target2));
			}
		}
	}

	GIVEN("targetExists")
	{
		Pokemon member1{makePokemon({.mHealth = 20})};
		Pokemon member2{makePokemon({})};
		Pokemon member3{makePokemon({})};

		BattleState state{makeBattleState({.mSideA = {{.mPokemon = &member1}, {.mPokemon = &member2}, {.mPokemon = &member3}}})};
		BattleTarget target{.mSide = Side::A};

		GIVEN("calling with a BattleTarget which is not in the bounds of the active slots")
		{
			target.mSlotIndex = 4;

			THEN("it returns false")
			{
				CHECK_FALSE(targetExists(state, target));
			}
		}

		GIVEN("calling with a BattleTarget which is in the bounds of the active slots and is healthy")
		{
			target.mSlotIndex = 0;

			THEN("it returns true")
			{
				CHECK(targetExists(state, target));
			}
		}

		GIVEN("calling with a BattleTarget which is in the bounds of the active slots and is not healthy")
		{
			target.mSlotIndex = 1;

			THEN("it returns false")
			{
				CHECK_FALSE(targetExists(state, target));
			}
		}
	}

	GIVEN("sideHasHealthyPokemon")
	{
		Pokemon member1{makePokemon({.mHealth = 20})};
		Pokemon member2{makePokemon({})};
		Pokemon member3{makePokemon({})};

		BattleState state{makeBattleState({.mPartyA = {&member1}, .mPartyB = {&member2, &member3}})};

		GIVEN("calling with a side which has no healthy Pokemon")
		{
			THEN("it returns false")
			{
				CHECK_FALSE(sideHasHealthyPokemon(state, Side::B));
			}
		}

		GIVEN("calling with a side which does have a health Pokemon")
		{
			THEN("it returns true")
			{
				CHECK(sideHasHealthyPokemon(state, Side::A));
			}
		}
	}

	GIVEN("getEffectiveSpeed")
	{
		std::array<us, POKEMON_STAT_COUNT> ivs{0, 0, 0, 0, 0, 25};
		std::array<us, POKEMON_STAT_COUNT> evs{0, 0, 0, 0, 0, 148};

		Pokemon member1{
			makePokemon({
				.mStats = {.mSpeed = 50},
				.mPokemonIVs = ivs,
				.mPokemonEVs = evs,
				.mNatureIDs = {{toNatureID(BuiltinNatureID::Brave)}}, // -Speed
				.mLevel = 50,
			}),
		};
		Pokemon member2{
			makePokemon({
				.mStats = {.mSpeed = 50},
				.mPokemonIVs = ivs,
				.mPokemonEVs = evs,
				.mNatureIDs = {{toNatureID(BuiltinNatureID::Hardy)}}, // Base Speed
				.mLevel = 50,
			}),
		};
		Pokemon member3{
			makePokemon({
				.mStats = {.mSpeed = 50},
				.mPokemonIVs = ivs,
				.mPokemonEVs = evs,
				.mNatureIDs = {{toNatureID(BuiltinNatureID::Jolly)}}, // +Speed
				.mLevel = 50,
			}),
		};

		GIVEN("calling with a null Pokemon")
		{
			BattleSlot slot{.mPokemon = nullptr};

			THEN("it returns 0.0")
			{
				CHECK(approximatelyEqualAbsRel(getEffectiveSpeed(slot), 0.0));
			}
		}

		GIVEN("calling with a negative speed nature")
		{
			BattleSlot slot{.mPokemon = &member1};

			GIVEN("stat stages")
			{
				GIVEN("a negative stat stage")
				{
					slot.mStatStages = {.mSpeed = -3};

					THEN("it returns 30.0")
					{
						CHECK(approximatelyEqualAbsRel(getEffectiveSpeed(slot), 30.0));
					}
				}

				GIVEN("a positive stat stage")
				{
					slot.mStatStages = {.mSpeed = 3};

					THEN("it returns 192.0")
					{
						CHECK(approximatelyEqualAbsRel(getEffectiveSpeed(slot), 192.0));
					}
				}
			}

			GIVEN("damage formula modifiers")
			{
				GIVEN("a negative speed modifier")
				{
					slot.mDamageFormulaModifiers = {.mSpeedModifier = -3.0};

					THEN("it returns 0.0")
					{
						CHECK(approximatelyEqualAbsRel(getEffectiveSpeed(slot), 0.0));
					}
				}

				GIVEN("a positive fractional speed modifier")
				{
					slot.mDamageFormulaModifiers = {.mSpeedModifier = 0.5};

					THEN("it returns 38.0")
					{
						CHECK(approximatelyEqualAbsRel(getEffectiveSpeed(slot), 38.0));
					}
				}

				GIVEN("a positive speed modifier")
				{
					slot.mDamageFormulaModifiers = {.mSpeedModifier = 3.0};

					THEN("it returns 231")
					{
						CHECK(approximatelyEqualAbsRel(getEffectiveSpeed(slot), 231.0));
					}
				}
			}

			GIVEN("both stat stages and damage formula modifiers")
			{
				GIVEN("negative stat stages and negative speed modifier")
				{
					slot.mStatStages = {.mSpeed = -3};
					slot.mDamageFormulaModifiers = {.mSpeedModifier = -3.0};

					THEN("it returns 0.0")
					{
						CHECK(approximatelyEqualAbsRel(getEffectiveSpeed(slot), 0.0));
					}
				}

				GIVEN("negative stat stages and fractional positive speed modifier")
				{
					slot.mStatStages = {.mSpeed = -3};
					slot.mDamageFormulaModifiers = {.mSpeedModifier = 0.5};

					THEN("it returns 15.0")
					{
						CHECK(approximatelyEqualAbsRel(getEffectiveSpeed(slot), 15.0));
					}
				}

				GIVEN("negative stat stages and positive speed modifier")
				{
					slot.mStatStages = {.mSpeed = -3};
					slot.mDamageFormulaModifiers = {.mSpeedModifier = 2.0};

					THEN("it returns 61.0")
					{
						CHECK(approximatelyEqualAbsRel(getEffectiveSpeed(slot), 61.0));
					}
				}

				GIVEN("positive stat stages and negative speed modifier")
				{
					slot.mStatStages = {.mSpeed = 3};
					slot.mDamageFormulaModifiers = {.mSpeedModifier = -3.0};

					THEN("it returns 0.0")
					{
						CHECK(approximatelyEqualAbsRel(getEffectiveSpeed(slot), 0.0));
					}
				}

				GIVEN("positive stat stages and fractional positive speed modifier")
				{
					slot.mStatStages = {.mSpeed = 3};
					slot.mDamageFormulaModifiers = {.mSpeedModifier = 0.5};

					THEN("it returns 96.0")
					{
						CHECK(approximatelyEqualAbsRel(getEffectiveSpeed(slot), 96.0));
					}
				}

				GIVEN("positive stat stages and positive speed modifier")
				{
					slot.mStatStages = {.mSpeed = 3};
					slot.mDamageFormulaModifiers = {.mSpeedModifier = 3};

					THEN("it returns 577.0")
					{
						CHECK(approximatelyEqualAbsRel(getEffectiveSpeed(slot), 577.0));
					}
				}
			}
		}

		GIVEN("calling with a neutral speed nature")
		{
			BattleSlot slot{.mPokemon = &member2};

			GIVEN("stat stages")
			{
				GIVEN("a negative stat stage")
				{
					slot.mStatStages = {.mSpeed = -3};

					THEN("it returns 34.0")
					{
						CHECK(approximatelyEqualAbsRel(getEffectiveSpeed(slot), 34.0));
					}
				}

				GIVEN("a positive stat stage")
				{
					slot.mStatStages = {.mSpeed = 3};

					THEN("it returns 215.0")
					{
						CHECK(approximatelyEqualAbsRel(getEffectiveSpeed(slot), 215.0));
					}
				}
			}

			GIVEN("damage formula modifiers")
			{
				GIVEN("a negative speed modifier")
				{
					slot.mDamageFormulaModifiers = {.mSpeedModifier = -3.0};

					THEN("it returns 0.0")
					{
						CHECK(approximatelyEqualAbsRel(getEffectiveSpeed(slot), 0.0));
					}
				}

				GIVEN("a positive fractional speed modifier")
				{
					slot.mDamageFormulaModifiers = {.mSpeedModifier = 0.5};

					THEN("it returns 43.0")
					{
						CHECK(approximatelyEqualAbsRel(getEffectiveSpeed(slot), 43.0));
					}
				}

				GIVEN("a positive speed modifier")
				{
					slot.mDamageFormulaModifiers = {.mSpeedModifier = 3.0};

					THEN("it returns 258")
					{
						CHECK(approximatelyEqualAbsRel(getEffectiveSpeed(slot), 258.0));
					}
				}
			}

			GIVEN("both stat stages and damage formula modifiers")
			{
				GIVEN("negative stat stages and negative speed modifier")
				{
					slot.mStatStages = {.mSpeed = -3};
					slot.mDamageFormulaModifiers = {.mSpeedModifier = -3.0};

					THEN("it returns 0.0")
					{
						CHECK(approximatelyEqualAbsRel(getEffectiveSpeed(slot), 0.0));
					}
				}

				GIVEN("negative stat stages and fractional positive speed modifier")
				{
					slot.mStatStages = {.mSpeed = -3};
					slot.mDamageFormulaModifiers = {.mSpeedModifier = 0.5};

					THEN("it returns 17.0")
					{
						CHECK(approximatelyEqualAbsRel(getEffectiveSpeed(slot), 17.0));
					}
				}

				GIVEN("negative stat stages and positive speed modifier")
				{
					slot.mStatStages = {.mSpeed = -3};
					slot.mDamageFormulaModifiers = {.mSpeedModifier = 2.0};

					THEN("it returns 68.0")
					{
						CHECK(approximatelyEqualAbsRel(getEffectiveSpeed(slot), 68.0));
					}
				}

				GIVEN("positive stat stages and negative speed modifier")
				{
					slot.mStatStages = {.mSpeed = 3};
					slot.mDamageFormulaModifiers = {.mSpeedModifier = -3.0};

					THEN("it returns 0.0")
					{
						CHECK(approximatelyEqualAbsRel(getEffectiveSpeed(slot), 0.0));
					}
				}

				GIVEN("positive stat stages and fractional positive speed modifier")
				{
					slot.mStatStages = {.mSpeed = 3};
					slot.mDamageFormulaModifiers = {.mSpeedModifier = 0.5};

					THEN("it returns 107.0")
					{
						CHECK(approximatelyEqualAbsRel(getEffectiveSpeed(slot), 107.0));
					}
				}

				GIVEN("positive stat stages and positive speed modifier")
				{
					slot.mStatStages = {.mSpeed = 3};
					slot.mDamageFormulaModifiers = {.mSpeedModifier = 3};

					THEN("it returns 645.0")
					{
						CHECK(approximatelyEqualAbsRel(getEffectiveSpeed(slot), 645.0));
					}
				}
			}
		}

		GIVEN("calling with a positive speed nature")
		{
			BattleSlot slot{.mPokemon = &member3};

			GIVEN("stat stages")
			{
				GIVEN("a negative stat stage")
				{
					slot.mStatStages = {.mSpeed = -3};

					THEN("it returns 37.0")
					{
						CHECK(approximatelyEqualAbsRel(getEffectiveSpeed(slot), 37.0));
					}
				}

				GIVEN("a positive stat stage")
				{
					slot.mStatStages = {.mSpeed = 3};

					THEN("it returns 235.0")
					{
						CHECK(approximatelyEqualAbsRel(getEffectiveSpeed(slot), 235.0));
					}
				}
			}

			GIVEN("damage formula modifiers")
			{
				GIVEN("a negative speed modifier")
				{
					slot.mDamageFormulaModifiers = {.mSpeedModifier = -3.0};

					THEN("it returns 0.0")
					{
						CHECK(approximatelyEqualAbsRel(getEffectiveSpeed(slot), 0.0));
					}
				}

				GIVEN("a positive fractional speed modifier")
				{
					slot.mDamageFormulaModifiers = {.mSpeedModifier = 0.5};

					THEN("it returns 47.0")
					{
						CHECK(approximatelyEqualAbsRel(getEffectiveSpeed(slot), 47.0));
					}
				}

				GIVEN("a positive speed modifier")
				{
					slot.mDamageFormulaModifiers = {.mSpeedModifier = 3.0};

					THEN("it returns 282")
					{
						CHECK(approximatelyEqualAbsRel(getEffectiveSpeed(slot), 282.0));
					}
				}
			}

			GIVEN("both stat stages and damage formula modifiers")
			{
				GIVEN("negative stat stages and negative speed modifier")
				{
					slot.mStatStages = {.mSpeed = -3};
					slot.mDamageFormulaModifiers = {.mSpeedModifier = -3.0};

					THEN("it returns 0.0")
					{
						CHECK(approximatelyEqualAbsRel(getEffectiveSpeed(slot), 0.0));
					}
				}

				GIVEN("negative stat stages and fractional positive speed modifier")
				{
					slot.mStatStages = {.mSpeed = -3};
					slot.mDamageFormulaModifiers = {.mSpeedModifier = 0.5};

					THEN("it returns 18.0")
					{
						CHECK(approximatelyEqualAbsRel(getEffectiveSpeed(slot), 18.0));
					}
				}

				GIVEN("negative stat stages and positive speed modifier")
				{
					slot.mStatStages = {.mSpeed = -3};
					slot.mDamageFormulaModifiers = {.mSpeedModifier = 2.0};

					THEN("it returns 75.0")
					{
						CHECK(approximatelyEqualAbsRel(getEffectiveSpeed(slot), 75.0));
					}
				}

				GIVEN("positive stat stages and negative speed modifier")
				{
					slot.mStatStages = {.mSpeed = 3};
					slot.mDamageFormulaModifiers = {.mSpeedModifier = -3.0};

					THEN("it returns 0.0")
					{
						CHECK(approximatelyEqualAbsRel(getEffectiveSpeed(slot), 0.0));
					}
				}

				GIVEN("positive stat stages and fractional positive speed modifier")
				{
					slot.mStatStages = {.mSpeed = 3};
					slot.mDamageFormulaModifiers = {.mSpeedModifier = 0.5};

					THEN("it returns 117.0")
					{
						CHECK(approximatelyEqualAbsRel(getEffectiveSpeed(slot), 117.0));
					}
				}

				GIVEN("positive stat stages and positive speed modifier")
				{
					slot.mStatStages = {.mSpeed = 3};
					slot.mDamageFormulaModifiers = {.mSpeedModifier = 3};

					THEN("it returns 705.0")
					{
						CHECK(approximatelyEqualAbsRel(getEffectiveSpeed(slot), 705.0));
					}
				}
			}
		}
	}

	GIVEN("makeMoveContext")
	{
		MoveAction action{.mUserSlotIndex = 2};
		MoveMeta meta{
			.mMoveID = toMoveID(BuiltinMoveID::Facade),
			.mTypeID = toTypeID(BuiltinTypeID::Fire),
			.mRangeID = BattleRangeID::Adjacent,
		};
		BattleTarget target{.mSide = Side::B};
		ub hitAttemptIndex{1};

		WHEN("calling calling with the necessary parameters")
		{
			EffectContext result{makeMoveContext(action, meta, target, hitAttemptIndex)};

			THEN("it returns an appropraite EffectContext")
			{
				EffectContext expected{
					makeEffectContext({
						.mMoveTypeID = toTypeID(BuiltinTypeID::Fire),
						.mMoveID = toMoveID(BuiltinMoveID::Facade),
						.mRangeOverride = BattleRangeID::Adjacent,
						.mUserIndex = 2,
						.mTargetIndex = 0,
						.mMoveBasePower = 0,
						.mMoveAccuracy = 0,
						.mHitAttemptIndex = 1,
						.mSourceType = EffectSource::Move,
						.mUserSide = Side::A,
						.mTargetSide = Side::B,
						.mIsSpecial = false,
					}),
				};

				CHECK((result == expected));
			}
		}
	}

	GIVEN("applyRecoil")
	{
		Pokemon pokemon{makePokemon({.mStats = {.mMaxHealth = 100}, .mHealth = 100, .mLevel = 50})};
		BattleState state{makeBattleState({.mSideA = {{{.mPokemon = &pokemon}}}})};
		EffectContext context{makeEffectContext({.mDamage = {.mRecoilRatio = 2.0, .mDamage = 10}, .mUserIndex = 0, .mUserSide = Side::A})};

		WHEN("calling with a userIndex greater than the slot amount")
		{
			applyRecoil(state, makeEffectContext({.mUserIndex = 2}));

			THEN("the Pokemon's health is unaffected")
			{
				CHECK(pokemon.getHealth() == 100);
			}
		}

		WHEN("calling with a slot Pokemon is a nullptr")
		{
			BattleState nullPokemonState{makeBattleState({.mSideA = {{{.mPokemon = nullptr}}}})};

			applyRecoil(nullPokemonState, context);

			THEN("nothing happens")
			{
				REQUIRE((nullPokemonState.mSideA.at(0).mPokemon == nullptr));
			}
		}

		WHEN("calling with a zero damage context a nullpt")
		{
			context.mDamage.mDamage = 0;

			applyRecoil(state, context);

			THEN("the Pokemon's health is unaffected")
			{
				CHECK(pokemon.getHealth() == 100);
			}
		}

		WHEN("calling with an infinite recoil ratio context a nullpt")
		{
			context.mDamage.mRecoilRatio = 1.0 / 0.0;

			applyRecoil(state, context);

			THEN("the Pokemon's health is unaffected")
			{
				CHECK(pokemon.getHealth() == 100);
			}
		}

		WHEN("calling with a negative recoil ratio context a nullpt")
		{
			context.mDamage.mRecoilRatio = -2.0;

			applyRecoil(state, context);

			THEN("the Pokemon's health is unaffected")
			{
				CHECK(pokemon.getHealth() == 100);
			}
		}

		WHEN("calling with a valid recoil ratio context a nullpt")
		{
			applyRecoil(state, context);

			THEN("the Pokemon's health is affected")
			{
				CHECK(pokemon.getHealth() == 80);
			}
		}

		WHEN("calling with a recoil ratio that will get clamepd to 1.0")
		{
			context.mDamage.mRecoilRatio = 0.02;

			applyRecoil(state, context);

			THEN("the Pokemon's health is affected")
			{
				CHECK(pokemon.getHealth() == 99);
			}
		}

		WHEN("calling with a recoil ratio that will get clamepd to the numeric limits of unsigned short")
		{
			context.mDamage.mDamage = 32'768;
			context.mDamage.mRecoilRatio = 3.0;

			applyRecoil(state, context);

			THEN("the Pokemon's health is set to 0")
			{
				CHECK(pokemon.getHealth() == 0);
			}
		}
	}

	GIVEN("resolveHitCount")
	{
		WHEN("calling with a FixedHitCount")
		{
			MoveMeta meta{.mHitCountPolicy = FixedHitCount{2}};

			THEN("it returns appropriately")
			{
				CHECK((resolveHitCount(meta) == 2));
			}
		}

		GIVEN("calling with a WeightedHitCount")
		{
			WeightedHitCount weighted{};

			GIVEN("no outcome weights")
			{
				MoveMeta meta{.mHitCountPolicy = weighted};

				THEN("it returns 0")
				{
					CHECK((resolveHitCount(meta) == 0));
				}
			}

			GIVEN("weights that sum to infinity")
			{
				weighted.mOutcomes = {
					{.mHitCount = 2, .mWeight = std::numeric_limits<double>::max()},
					{.mHitCount = 3, .mWeight = std::numeric_limits<double>::max()},
				};

				MoveMeta meta{.mHitCountPolicy = weighted};

				THEN("it returns 0")
				{
					CHECK((resolveHitCount(meta) == 0));
				}
			}

			GIVEN("negative weights")
			{
				weighted.mOutcomes = {{.mHitCount = 1, .mWeight = -1.0}, {.mHitCount = 1, .mWeight = -2.0}};

				MoveMeta meta{.mHitCountPolicy = weighted};

				THEN("it returns 0")
				{
					CHECK((resolveHitCount(meta) == 0));
				}
			}

			GIVEN("one impossible weight, one negative weight, one small weight, and one large weight")
			{
				weighted.mOutcomes = {
					{.mHitCount = 1, .mWeight = 1.0 / 0.0},
					{.mHitCount = 5, .mWeight = 9.0},
					{.mWeight = -1.0},
					{.mHitCount = 2, .mWeight = 1.0},
				};

				MoveMeta meta{.mHitCountPolicy = weighted};

				Random::setSeed(7);

				ub lowerWeight{0};

				for (ub i{0}; i < 10; ++i)
				{
					if (resolveHitCount(meta) == 2)
					{
						++lowerWeight;
					}
				}

				THEN("the selected weight only hits the lower weight 10% of the time")
				{
					CHECK((lowerWeight == 1));
					CHECK(((10 - lowerWeight) == 9));
				}
			}
		}
	}

	GIVEN("hasDuplicatePokemonPointers")
	{
		Pokemon member1{makePokemon({.mHealth = 20})};
		Pokemon *member2{&member1};
		Pokemon member3{makePokemon({.mHealth = 20})};
		Pokemon *member4{&member3};

		WHEN("calling with a duplicate pointer in partyA")
		{
			std::vector<Pokemon *> partyA{&member1, member2};
			std::vector<Pokemon *> partyB{&member3};

			THEN("it returns true")
			{
				CHECK(hasDuplicatePokemonPointers(partyA, partyB));
			}
		}

		WHEN("calling with a duplicate pointer in partyB")
		{
			std::vector<Pokemon *> partyA{&member1};
			std::vector<Pokemon *> partyB{&member3, member4};

			THEN("it returns true")
			{
				CHECK(hasDuplicatePokemonPointers(partyA, partyB));
			}
		}

		WHEN("calling with a duplicate pointer in partyA that originates from partyB")
		{
			std::vector<Pokemon *> partyA{member4};
			std::vector<Pokemon *> partyB{&member3};

			THEN("it returns true")
			{
				CHECK(hasDuplicatePokemonPointers(partyA, partyB));
			}
		}

		WHEN("calling with a duplicate pointer in partyB that originates from partyA")
		{
			std::vector<Pokemon *> partyA{&member1};
			std::vector<Pokemon *> partyB{member2};

			THEN("it returns true")
			{
				CHECK(hasDuplicatePokemonPointers(partyA, partyB));
			}
		}

		WHEN("calling with a no duplicate pointers in either party")
		{
			std::vector<Pokemon *> partyA{&member1};
			std::vector<Pokemon *> partyB{&member3};

			THEN("it returns false")
			{
				CHECK_FALSE(hasDuplicatePokemonPointers(partyA, partyB));
			}
		}
	}

	GIVEN("amountOfHealthyPokemonInParty")
	{
		Pokemon member1{makePokemon({.mHealth = 20})};
		Pokemon member2{makePokemon({})};
		Pokemon member3{makePokemon({})};

		WHEN("calling with no healthy Pokemon in party")
		{
			std::vector<const Pokemon *> party{&member2, &member3};

			THEN("it returns 0")
			{
				CHECK((amountOfHealthyPokemonInParty(party) == 0));
			}
		}

		WHEN("calling with one healthy Pokemon in party")
		{
			std::vector<const Pokemon *> party{&member1, &member2};

			THEN("it returns 1")
			{
				CHECK((amountOfHealthyPokemonInParty(party) == 1));
			}
		}

		WHEN("calling with the whole party is healthy")
		{
			member2.setMaximumHealth(20);
			member2.setHealth(20);
			member3.setMaximumHealth(20);
			member3.setHealth(20);

			std::vector<const Pokemon *> party{&member1, &member2, &member3};

			THEN("it returns the parties size")
			{
				CHECK((amountOfHealthyPokemonInParty(party) == static_cast<sl>(party.size())));
			}
		}
	}

	GIVEN("printBattleSlotWithNames")
	{
		GIVEN("valid registries")
		{
			StatStages stages{.mDefense = 2, .mSpAttack = 1, .mAccuracy = 4, .mEvasion = 3};
			DamageFormulaModifiers modifiers{.mHealthModifier = 2.3, .mDefenseModifier = 1.2, .mSpeedModifier = 0.85};
			Pokemon pokemon{
    			makePokemon({
    				.mName = "TestMon",
    				.mStats = {
    					.mMaxHealth = 150,
    					.mAttack = 101,
    					.mDefense = 102,
    					.mSpAttack = 103,
    					.mSpDefense = 104,
    					.mSpeed = 105,
    				},
    				.mStatusIDs
    				= {toStatusID(BuiltinStatusID::Poison), toStatusID(BuiltinStatusID::Toxic), toStatusID(BuiltinStatusID::Paralysis),
    				   toStatusID(BuiltinStatusID::Sleep), toStatusID(BuiltinStatusID::Freeze),},
    				.mMoveIDs = {toMoveID(BuiltinMoveID::Pound), toMoveID(BuiltinMoveID::KarateChop), toMoveID(BuiltinMoveID::Facade), toMoveID(BuiltinMoveID::HydroSteam)},
    				.mMaxPP = {15, 20, 25, 30},
    				.mCurrentPP = {5, 10, 15, 20},
    				.mTypesIDs = {toTypeID(BuiltinTypeID::Fire), toTypeID(BuiltinTypeID::Bug)},
    				.mAbilityIDs = {toAbilityID(BuiltinAbilityID::AirLock)},
    				.mItemIDs = {toItemID(BuiltinItemID::CheriBerry)},
    				.mNatureIDs = {toNatureID(BuiltinNatureID::Hardy)},
    				.mHealth = 150,
    				.mLevel = 50,
    			}),
    		};
			std::array<VolatileStatus, MAX_VOLATILE_STATUSES_PER_POKEMON> volatileStatuses{
				{{.mStatusID{toStatusID(BuiltinStatusID::Autotomize)}}, {.mStatusID{toStatusID(BuiltinStatusID::AquaRing)}}},
			};

			BattleSlot slot{
				.mDamageFormulaModifiers = modifiers,
				.mVolatileStatuses = volatileStatuses,
				.mPokemon = &pokemon,
				.mStatStages = stages,
				.mChoiceLockedMove = toMoveID(BuiltinMoveID::Pound),
				.mPosition = 3,
				.mSleepCounter = 2,
				.mToxicCounter = 3,
				.mProtectionCounter = 1,
				.mIsProtected = true,
				.mIsFlinched = false,
				.mIsGrounded = true,
				.mFaintProcessed = false,
			};

			RegistryProvider provider{getDefaultInitializedRegistryProvider()};

			WHEN("the pokemon is a nullptr")
			{
				std::ostringstream output;
				printBattleSlotWithNames(output, {.mPokemon = nullptr}, provider);

				THEN("nothing is returned")
				{
					std::string_view expected{};

					CHECK((output.str() == expected));
				}
			}

			WHEN("the BattleSlot is written to a stream")
			{
				std::ostringstream output;
				printBattleSlotWithNames(output, slot, provider);

				THEN("all public state is written with numeric identifiers and PP")
				{
					std::string_view expected{
						"Battle Slot {\n"
						"  Damage Formula Modifiers {\n"
						"    Health: 2.3\n"
						"    Attack: 1\n"
						"    Defense: 1.2\n"
						"    Special Attack: 1\n"
						"    Special Defense: 1\n"
						"    Speed: 0.85\n"
						"  }\n"
						"  Volatile Statuses:\n"
						"    [0]:\n"
						"      ID: 7\n"
						"      Name: Autotomize\n"
						"    [1]:\n"
						"      ID: 8\n"
						"      Name: Aqua Ring\n"
						"    [2]:\n"
						"      ID: 0\n"
						"      Name: None\n"
						"    [3]:\n"
						"      ID: 0\n"
						"      Name: None\n"
						"    [4]:\n"
						"      ID: 0\n"
						"      Name: None\n"
						"    [5]:\n"
						"      ID: 0\n"
						"      Name: None\n"
						"    [6]:\n"
						"      ID: 0\n"
						"      Name: None\n"
						"    [7]:\n"
						"      ID: 0\n"
						"      Name: None\n"
						"  Pokemon {\n"
						"    Name: TestMon\n"
						"    Level: 50\n"
						"    Level Damage Factor: 22\n"
						"    Health: 150/210\n"
						"      IV: 0\n"
						"      EV: 0\n"
						"    Attack: 106\n"
						"      IV: 0\n"
						"      EV: 0\n"
						"    Defense: 107\n"
						"      IV: 0\n"
						"      EV: 0\n"
						"    Special Attack: 108\n"
						"      IV: 0\n"
						"      EV: 0\n"
						"    Special Defense: 109\n"
						"      IV: 0\n"
						"      EV: 0\n"
						"    Speed: 110\n"
						"      IV: 0\n"
						"      EV: 0\n"
						"    Types:\n"
						"      [0]:\n"
						"        ID: 2\n"
						"        Name: Fire\n"
						"      [1]:\n"
						"        ID: 12\n"
						"        Name: Bug\n"
						"    Natures:\n"
						"      [0]:\n"
						"        ID: 1\n"
						"        Name: Hardy\n"
						"    Abilities:\n"
						"      [0]:\n"
						"        ID: 6\n"
						"        Name: Air Lock\n"
						"    Items:\n"
						"      [0]:\n"
						"        ID: 1\n"
						"        Name: Cheri Berry\n"
						"    Non-Volatile Statuses:\n"
						"      [0]:\n"
						"        ID: 5\n"
						"        Name: Poison\n"
						"      [1]:\n"
						"        ID: 6\n"
						"        Name: Toxic\n"
						"      [2]:\n"
						"        ID: 1\n"
						"        Name: Paralysis\n"
						"      [3]:\n"
						"        ID: 3\n"
						"        Name: Sleep\n"
						"      [4]:\n"
						"        ID: 4\n"
						"        Name: Freeze\n"
						"    Moves:\n"
						"      [0]:\n"
						"        ID: 1\n"
						"        Name: Pound\n"
						"        PP: 5/15\n"
						"      [1]:\n"
						"        ID: 2\n"
						"        Name: Karate Chop\n"
						"        PP: 10/20\n"
						"      [2]:\n"
						"        ID: 3\n"
						"        Name: Facade\n"
						"        PP: 15/25\n"
						"      [3]:\n"
						"        ID: 4\n"
						"        Name: Hydro Steam\n"
						"        PP: 20/30\n"
						"  }\n"
						"  Stat Stages {\n"
						"    Attack: 0\n"
						"    Defense: 2\n"
						"    Special Attack: 1\n"
						"    Special Defense: 0\n"
						"    Speed: 0\n"
						"    Accuracy: 4\n"
						"    Evasion: 3\n"
						"  }\n"
						"  Choice Locked Move ID: 1\n"
						"  Position: 3\n"
						"  Sleep: 2\n"
						"  Toxic: 3\n"
						"  Protection: 1\n"
						"  Is Protected: true\n"
						"  Is Flinched: false\n"
						"  Is Grounded: true\n"
						"  Faint Processed: false\n"
						"}",
					};

					CHECK((output.str() == expected));
				}
			}
		}

		GIVEN("invalid status registry")
		{
			StatStages stages{.mDefense = 2, .mSpAttack = 1, .mAccuracy = 4, .mEvasion = 3};
			DamageFormulaModifiers modifiers{.mHealthModifier = 2.3, .mDefenseModifier = 1.2, .mSpeedModifier = 0.85};
			Pokemon pokemon{
    			makePokemon({
    				.mName = "TestMon",
    				.mStats = {
    					.mMaxHealth = 150,
    					.mAttack = 101,
    					.mDefense = 102,
    					.mSpAttack = 103,
    					.mSpDefense = 104,
    					.mSpeed = 105,
    				},
    				.mStatusIDs
    				= {toStatusID(BuiltinStatusID::Poison), toStatusID(BuiltinStatusID::Toxic), toStatusID(BuiltinStatusID::Paralysis),
    				   toStatusID(BuiltinStatusID::Sleep), toStatusID(BuiltinStatusID::Freeze),},
    				.mMoveIDs = {toMoveID(BuiltinMoveID::Pound), toMoveID(BuiltinMoveID::KarateChop), toMoveID(BuiltinMoveID::Facade), toMoveID(BuiltinMoveID::HydroSteam)},
    				.mMaxPP = {15, 20, 25, 30},
    				.mCurrentPP = {5, 10, 15, 20},
    				.mTypesIDs = {toTypeID(BuiltinTypeID::Fire), toTypeID(BuiltinTypeID::Bug)},
    				.mAbilityIDs = {toAbilityID(BuiltinAbilityID::AirLock)},
    				.mItemIDs = {toItemID(BuiltinItemID::CheriBerry)},
    				.mNatureIDs = {toNatureID(BuiltinNatureID::Hardy)},
    				.mHealth = 150,
    				.mLevel = 50,
    			}),
    		};
			std::array<VolatileStatus, MAX_VOLATILE_STATUSES_PER_POKEMON> volatileStatuses{
				{{.mStatusID{toStatusID(BuiltinStatusID::Autotomize)}}, {.mStatusID{toStatusID(BuiltinStatusID::AquaRing)}}},
			};

			BattleSlot slot{
				.mDamageFormulaModifiers = modifiers,
				.mVolatileStatuses = volatileStatuses,
				.mPokemon = &pokemon,
				.mStatStages = stages,
				.mChoiceLockedMove = toMoveID(BuiltinMoveID::Pound),
				.mPosition = 3,
				.mSleepCounter = 2,
				.mToxicCounter = 3,
				.mProtectionCounter = 1,
				.mIsProtected = true,
				.mIsFlinched = false,
				.mIsGrounded = true,
				.mFaintProcessed = false,
			};

			RegistryProvider nullProvider{getNullStatusRegistryProvider()};

			WHEN("the pokemon is a nullptr")
			{
				std::ostringstream output;
				printBattleSlotWithNames(output, {.mPokemon = nullptr}, nullProvider);

				THEN("nothing is returned")
				{
					std::string_view expected{};

					CHECK((output.str() == expected));
				}
			}

			WHEN("the BattleSlot is written to a stream")
			{
				std::ostringstream output;
				printBattleSlotWithNames(output, slot, nullProvider);

				THEN("all public state is written with numeric identifiers and PP")
				{
					std::string_view expected{
						"Battle Slot {\n"
						"  Damage Formula Modifiers {\n"
						"    Health: 2.3\n"
						"    Attack: 1\n"
						"    Defense: 1.2\n"
						"    Special Attack: 1\n"
						"    Special Defense: 1\n"
						"    Speed: 0.85\n"
						"  }\n"
						"  Volatile Statuses:\n"
						"    [0]:\n"
						"      ID: 7\n"
						"      Name: <unregistered>\n"
						"    [1]:\n"
						"      ID: 8\n"
						"      Name: <unregistered>\n"
						"    [2]:\n"
						"      ID: 0\n"
						"      Name: <unregistered>\n"
						"    [3]:\n"
						"      ID: 0\n"
						"      Name: <unregistered>\n"
						"    [4]:\n"
						"      ID: 0\n"
						"      Name: <unregistered>\n"
						"    [5]:\n"
						"      ID: 0\n"
						"      Name: <unregistered>\n"
						"    [6]:\n"
						"      ID: 0\n"
						"      Name: <unregistered>\n"
						"    [7]:\n"
						"      ID: 0\n"
						"      Name: <unregistered>\n"
						"  Pokemon {\n"
						"    Name: TestMon\n"
						"    Level: 50\n"
						"    Level Damage Factor: 22\n"
						"    Health: 150/210\n"
						"      IV: 0\n"
						"      EV: 0\n"
						"    Attack: 106\n"
						"      IV: 0\n"
						"      EV: 0\n"
						"    Defense: 107\n"
						"      IV: 0\n"
						"      EV: 0\n"
						"    Special Attack: 108\n"
						"      IV: 0\n"
						"      EV: 0\n"
						"    Special Defense: 109\n"
						"      IV: 0\n"
						"      EV: 0\n"
						"    Speed: 110\n"
						"      IV: 0\n"
						"      EV: 0\n"
						"    Types:\n"
						"      [0]:\n"
						"        ID: 2\n"
						"        Name: Fire\n"
						"      [1]:\n"
						"        ID: 12\n"
						"        Name: Bug\n"
						"    Natures:\n"
						"      [0]:\n"
						"        ID: 1\n"
						"        Name: Hardy\n"
						"    Abilities:\n"
						"      [0]:\n"
						"        ID: 6\n"
						"        Name: Air Lock\n"
						"    Items:\n"
						"      [0]:\n"
						"        ID: 1\n"
						"        Name: Cheri Berry\n"
						"    Non-Volatile Statuses:\n"
						"      [0]:\n"
						"        ID: 5\n"
						"        Name: <unregistered>\n"
						"      [1]:\n"
						"        ID: 6\n"
						"        Name: <unregistered>\n"
						"      [2]:\n"
						"        ID: 1\n"
						"        Name: <unregistered>\n"
						"      [3]:\n"
						"        ID: 3\n"
						"        Name: <unregistered>\n"
						"      [4]:\n"
						"        ID: 4\n"
						"        Name: <unregistered>\n"
						"    Moves:\n"
						"      [0]:\n"
						"        ID: 1\n"
						"        Name: Pound\n"
						"        PP: 5/15\n"
						"      [1]:\n"
						"        ID: 2\n"
						"        Name: Karate Chop\n"
						"        PP: 10/20\n"
						"      [2]:\n"
						"        ID: 3\n"
						"        Name: Facade\n"
						"        PP: 15/25\n"
						"      [3]:\n"
						"        ID: 4\n"
						"        Name: Hydro Steam\n"
						"        PP: 20/30\n"
						"  }\n"
						"  Stat Stages {\n"
						"    Attack: 0\n"
						"    Defense: 2\n"
						"    Special Attack: 1\n"
						"    Special Defense: 0\n"
						"    Speed: 0\n"
						"    Accuracy: 4\n"
						"    Evasion: 3\n"
						"  }\n"
						"  Choice Locked Move ID: 1\n"
						"  Position: 3\n"
						"  Sleep: 2\n"
						"  Toxic: 3\n"
						"  Protection: 1\n"
						"  Is Protected: true\n"
						"  Is Flinched: false\n"
						"  Is Grounded: true\n"
						"  Faint Processed: false\n"
						"}",
					};

					CHECK((output.str() == expected));
				}
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
