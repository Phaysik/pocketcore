/*! @file battleHelpers.test.cpp
	@brief C++ file for running tests for the Battle helper functions.
	@date 10/21/2026
	@since 0.12.45
	@version 0.12.52
	@author Matthew Moore
*/

#include "Battle/battleHelpers.h"

#include <algorithm>
#include <functional>
#include <limits>
#include <sstream>
#include <vector>

#include "Ability/builtInAbilityID.h"
#include "Battle/battleAction.h"
#include "Battle/battleState.h"
#include "Battle/battleState.testHelper.h"
#include "Battle/battleTargetsAndTriggers.h"
#include "Battle/battleValidation.h"
#include "Configuration/constants.h"
#include "Core/typedefs.h"
#include "Effect/effectContext.h"
#include "Effect/effectContext.testHelper.h"
#include "Effect/effectSourceAndSuppression.h"
#include "Item/builtInItemID.h"
#include "Move/builtInMoveID.h"
#include "Move/moveHitPolicy.h"
#include "Move/moveID.h"
#include "Move/moveMeta.h"
#include "Nature/builtInNatureID.h"
#include "Pokemon/builtInPokemonID.h"
#include "Pokemon/pokemon.h"
#include "Pokemon/pokemon.testHelper.h"
#include "Pokemon/pokemonStats.h"
#include "Registry/moveRegistry.h"
#include "Registry/registryProvider.h"
#include "Registry/registryProvider.testHelper.h"
#include "Status/builtInStatusID.h"
#include "Status/volatileStatus.h"
#include "Type/builtInTypeID.h"
#include "Utility/Math/floatUtility.h"
#include "Utility/Random/callableRandomSource.h"
#include "Utility/Random/random.testHelper.h"
#include "Utility/Random/randomSourceInterface.h"
#include "Validation/Battle/battleError.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Ability::BuiltinAbilityID;
using PocketCore::Ability::toAbilityID;
using PocketCore::Battle::activeSlots;
using PocketCore::Battle::amountOfHealthyPokemonInParty;
using PocketCore::Battle::anyPartyPokemonNull;
using PocketCore::Battle::appendEligibleTargets;
using PocketCore::Battle::applyRecoil;
using PocketCore::Battle::assignActiveSlots;
using PocketCore::Battle::BattleRangeID;
using PocketCore::Battle::BattleResult;
using PocketCore::Battle::BattleSlot;
using PocketCore::Battle::BattleState;
using PocketCore::Battle::BattleTarget;
using PocketCore::Battle::BattleTargetID;
using PocketCore::Battle::canTarget;
using PocketCore::Battle::contextSlot;
using PocketCore::Battle::DamageFormulaModifiers;
using PocketCore::Battle::getBattleTarget;
using PocketCore::Battle::getEffectiveSpeed;
using PocketCore::Battle::getMoveTargets;
using PocketCore::Battle::getOppositeSide;
using PocketCore::Battle::getResult;
using PocketCore::Battle::getSideOrder;
using PocketCore::Battle::handleMovePrioritization;
using PocketCore::Battle::hasDuplicatePokemonPointers;
using PocketCore::Battle::hasReserve;
using PocketCore::Battle::isAdjacent;
using PocketCore::Battle::isHealthy;
using PocketCore::Battle::makeMoveContext;
using PocketCore::Battle::MoveAction;
using PocketCore::Battle::party;
using PocketCore::Battle::printBattleSlotWithNames;
using PocketCore::Battle::resolveHitCount;
using PocketCore::Battle::resolveTargets;
using PocketCore::Battle::sideHasHealthyPokemon;
using PocketCore::Battle::StatStages;
using PocketCore::Battle::SwitchAction;
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
using PocketCore::Move::MoveID;
using PocketCore::Move::MoveMeta;
using PocketCore::Move::toMoveID;
using PocketCore::Move::WeightedHitCount;
using PocketCore::Nature::BuiltinNatureID;
using PocketCore::Nature::toNatureID;
using PocketCore::Pokemon::BuiltinPokemonID;
using PocketCore::Pokemon::Pokemon;
using PocketCore::Pokemon::POKEMON_STAT_COUNT;
using PocketCore::Pokemon::toPokemonID;
using PocketCore::Registry::Move::MoveRegistry;
using PocketCore::Registry::RegistryProvider;
using PocketCore::Status::BuiltinStatusID;
using PocketCore::Status::toStatusID;
using PocketCore::Status::VolatileStatus;
using PocketCore::Testing::FixedRandom;
using PocketCore::Testing::getDefaultInitializedRegistryProvider;
using PocketCore::Testing::getNullStatusRegistryProvider;
using PocketCore::Testing::makeBattleState;
using PocketCore::Testing::makeEffectContext;
using PocketCore::Testing::makePokemon;
using PocketCore::Testing::SequenceRandom;
using PocketCore::Type::BuiltinTypeID;
using PocketCore::Type::toTypeID;
using PocketCore::Utility::Math::approximatelyEqualAbsRel;
using PocketCore::Utility::Random::CallableRandomSource;
using PocketCore::Utility::Random::IRandomSource;
using PocketCore::Validation::Battle::BattleEngineError;

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
		Pokemon partyA1{makePokemon({.mMetadata = {.mPokemonID = toPokemonID(BuiltinPokemonID::Charizard)}})};
		Pokemon partyB1{makePokemon({.mMetadata = {.mPokemonID = toPokemonID(BuiltinPokemonID::Blastoise)}})};
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
		Pokemon pokemon{makePokemon({.mMetadata = {.mPokemonID = toPokemonID(BuiltinPokemonID::Blastoise)}, .mHealth = 20})};

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
				.mMetadata = {.mBaseStats = {.mSpeed = 50}},
				.mPokemonIVs = ivs,
				.mPokemonEVs = evs,
				.mNatureIDs = {{toNatureID(BuiltinNatureID::Brave)}}, // -Speed
				.mLevel = 50,
			}),
		};
		Pokemon member2{
			makePokemon({
				.mMetadata = {.mBaseStats = {.mSpeed = 50}},
				.mPokemonIVs = ivs,
				.mPokemonEVs = evs,
				.mNatureIDs = {{toNatureID(BuiltinNatureID::Hardy)}}, // Base Speed
				.mLevel = 50,
			}),
		};
		Pokemon member3{
			makePokemon({
				.mMetadata = {.mBaseStats = {.mSpeed = 50}},
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
		Pokemon pokemon{makePokemon({.mMetadata = {.mBaseStats = {.mMaxHealth = 100}}, .mHealth = 100, .mLevel = 50})};
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
			context.mDamage.mRecoilRatio = std::numeric_limits<double>::infinity();

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
			SequenceRandom<double> unused{};

			THEN("it returns appropriately and does not trigger a random function call")
			{
				REQUIRE((resolveHitCount(meta, std::ref(unused)) == 2));
				CHECK((resolveHitCount(meta, FixedRandom<double>{0.0}) == 2));
				CHECK((unused.calls() == 0));
			}
		}

		GIVEN("an engine-style random source reference wrapper")
		{
			SequenceRandom<double> sequence{0.0, 9.0, 10.0};
			CallableRandomSource<double, std::reference_wrapper<SequenceRandom<double>>> source{std::ref(sequence)};
			IRandomSource<double> &interface{source};
			auto randomSource{std::ref(interface)};

			WHEN("the hit count is fixed")
			{
				MoveMeta meta{.mHitCountPolicy = FixedHitCount{2}};

				THEN("the fixed count is returned without invoking the source")
				{
					CHECK((resolveHitCount(meta, randomSource) == 2));
					CHECK((sequence.calls() == 0));
				}
			}

			WHEN("the hit count is weighted")
			{
				WeightedHitCount weighted{
					.mOutcomes = {{.mHitCount = 5, .mWeight = 9.0}, {.mHitCount = 2, .mWeight = 1.0}},
				};
				MoveMeta meta{.mHitCountPolicy = weighted};

				THEN("the source selects the expected buckets and the final outcome fallback")
				{
					CHECK((resolveHitCount(meta, randomSource) == 5));
					CHECK((resolveHitCount(meta, randomSource) == 2));
					CHECK((resolveHitCount(meta, randomSource) == 2));
					CHECK((sequence.calls() == 3));
				}
			}
		}

		GIVEN("calling with a WeightedHitCount")
		{
			WeightedHitCount weighted{};

			GIVEN("no outcome weights")
			{
				MoveMeta meta{.mHitCountPolicy = weighted};
				SequenceRandom<double> unused{};

				THEN("it returns 0 and does not trigger a random function call")
				{
					REQUIRE((resolveHitCount(meta, std::ref(unused)) == 0));
					CHECK((unused.calls() == 0));
				}
			}

			GIVEN("weights that sum to infinity")
			{
				weighted.mOutcomes = {
					{.mHitCount = 2, .mWeight = std::numeric_limits<double>::max()},
					{.mHitCount = 3, .mWeight = std::numeric_limits<double>::max()},
				};

				MoveMeta meta{.mHitCountPolicy = weighted};
				SequenceRandom<double> unused{};

				THEN("it returns 0 and does not trigger a random function call")
				{
					REQUIRE((resolveHitCount(meta, std::ref(unused)) == 0));
					CHECK((unused.calls() == 0));
				}
			}

			GIVEN("negative weights")
			{
				weighted.mOutcomes = {{.mHitCount = 1, .mWeight = -1.0}, {.mHitCount = 1, .mWeight = -2.0}};

				MoveMeta meta{.mHitCountPolicy = weighted};
				SequenceRandom<double> unused{};

				THEN("it returns 0 and does not trigger a random function call")
				{
					CHECK((resolveHitCount(meta, std::ref(unused)) == 0));
					CHECK((unused.calls() == 0));
				}
			}

			GIVEN("one impossible weight, one negative weight, one small weight, and one large weight")
			{
				weighted.mOutcomes = {
					{.mHitCount = 1, .mWeight = std::numeric_limits<double>::infinity()},
					{.mHitCount = 5, .mWeight = 9.0},
					{.mHitCount = 1, .mWeight = -1.0},
					{.mHitCount = 2, .mWeight = 1.0},
				};

				MoveMeta meta{.mHitCountPolicy = weighted};

				WHEN("the random value is within the bounds [0.0, 9)")
				{
					SequenceRandom<double> seq{0.0, 8.999};
					THEN("it returns 5")
					{
						REQUIRE((resolveHitCount(meta, FixedRandom<double>{0.0}) == 5));
						CHECK((resolveHitCount(meta, std::ref(seq)) == 5));
						REQUIRE((resolveHitCount(meta, FixedRandom<double>{8.999}) == 5));
						CHECK((resolveHitCount(meta, std::ref(seq)) == 5));
						REQUIRE((seq.calls() == 2));
					}
				}

				WHEN("the random value is within the bounds [9.0, 10.0) - the exact boundary between the two valid buckets up to the "
					 "highest value in the summed buckets")
				{
					SequenceRandom<double> seq{9.0, 9.999};
					THEN("it returns 2")
					{
						REQUIRE((resolveHitCount(meta, FixedRandom<double>{9.0}) == 2));
						CHECK((resolveHitCount(meta, std::ref(seq)) == 2));
						REQUIRE((resolveHitCount(meta, FixedRandom<double>{9.999}) == 2));
						CHECK((resolveHitCount(meta, std::ref(seq)) == 2));
						REQUIRE((seq.calls() == 2));
					}
				}

				WHEN("the random value is exactly 10.0 - the theoretically unachievable max that real-distributions cannot generate")
				{
					SequenceRandom<double> seq{10.0};
					THEN("it returns 2 - the last valid outcome")
					{
						REQUIRE((resolveHitCount(meta, FixedRandom<double>{10.0}) == 2));
						CHECK((resolveHitCount(meta, std::ref(seq)) == 2));
						REQUIRE((seq.calls() == 1));
					}
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

	GIVEN("assignActiveSlots")
	{
		Pokemon member1{makePokemon({})};
		Pokemon member2{makePokemon({})};
		Pokemon member3{makePokemon({.mHealth = 30})};
		Pokemon member4{makePokemon({.mHealth = 20})};
		Pokemon member5{makePokemon({.mHealth = 10})};
		std::vector<BattleSlot> slots{};

		WHEN("calling with no party Pokemon")
		{
			std::vector<Pokemon *> party{};

			assignActiveSlots(party, slots, 2);

			THEN("no Pokemon is added to the slots")
			{
				CHECK(slots.empty());
			}
		}

		WHEN("calling with no healthy Pokemon in party")
		{
			std::vector<Pokemon *> party{&member1, &member2};

			assignActiveSlots(party, slots, 2);

			THEN("no Pokemon is added to the slots")
			{
				CHECK(slots.empty());
			}
		}

		WHEN("calling with one healthy Pokemon in party")
		{
			std::vector<Pokemon *> party{&member1, &member3};

			assignActiveSlots(party, slots, 2);

			THEN("only the healthy Pokemon is added to the slots")
			{
				CHECK((slots.size() == 1));
				CHECK((slots.at(0).mPokemon == &member3));
			}
		}

		WHEN("calling with all healthy Pokemon in party")
		{
			std::vector<Pokemon *> party{&member3, &member4};

			assignActiveSlots(party, slots, 2);

			THEN("all Pokemon are added to slots")
			{
				CHECK((slots.size() == 2));
				CHECK((slots.at(0).mPokemon == &member3));
				CHECK((slots.at(1).mPokemon == &member4));
			}
		}

		WHEN("calling with more healthy Pokemon in party than allowed active Pokemon per side")
		{
			std::vector<Pokemon *> party{&member1, &member3, &member4, &member5};

			assignActiveSlots(party, slots, 2);

			THEN("only the first 2 healthy Pokemon are added")
			{
				CHECK((slots.size() == 2));
				CHECK((slots.at(0).mPokemon == &member3));
				CHECK((slots.at(1).mPokemon == &member4));
			}
		}
	}

	GIVEN("canTarget")
	{
		Pokemon member1{makePokemon({.mHealth = 50})};
		Pokemon member2{makePokemon({})};
		Pokemon member3{makePokemon({.mHealth = 30})};
		Pokemon member4{makePokemon({.mHealth = 20})};
		Pokemon member5{makePokemon({.mHealth = 10})};
		Pokemon member6{makePokemon({.mHealth = 40})};
		BattleState state{
			makeBattleState({
				.mSideA
				= {{.mPokemon = &member1, .mPosition = 0}, {.mPokemon = &member2, .mPosition = 1}, {.mPokemon = &member3, .mPosition = 2}},
				.mSideB
				= {{.mPokemon = &member4, .mPosition = 0}, {.mPokemon = &member5, .mPosition = 1}, {.mPokemon = &member6, .mPosition = 2}},
			}),
		};
		BattleTarget source{.mSide = Side::A};
		BattleTarget target{.mSide = Side::B};

		WHEN("calling with a target who doesn't exist")
		{
			target.mSlotIndex = 3;

			THEN("it returns false")
			{
				CHECK_FALSE((canTarget(state, source, target, BattleRangeID::Adjacent)));
			}
		}

		WHEN("calling with an unrestricted rangeID")
		{
			target.mSlotIndex = 0;

			THEN("it returns true")
			{
				CHECK((canTarget(state, source, target, BattleRangeID::Unrestricted)));
			}
		}

		WHEN("calling when the target is not adjacent")
		{
			source.mSlotIndex = 0;
			target.mSlotIndex = 2;

			THEN("it returns false")
			{
				CHECK_FALSE((canTarget(state, source, target, BattleRangeID::Adjacent)));
			}
		}

		WHEN("calling when the target is adjacent")
		{
			source.mSlotIndex = 0;
			target.mSlotIndex = 1;

			THEN("it returns true")
			{
				CHECK((canTarget(state, source, target, BattleRangeID::Adjacent)));
			}
		}
	}

	GIVEN("appendEligibleTargets")
	{
		Pokemon member1{makePokemon({.mHealth = 50})};
		Pokemon member2{makePokemon({.mHealth = 60})};
		Pokemon member3{makePokemon({.mHealth = 30})};
		Pokemon member4{makePokemon({.mHealth = 20})};
		Pokemon member5{makePokemon({.mHealth = 10})};
		Pokemon member6{makePokemon({.mHealth = 40})};
		BattleState state{
			makeBattleState({
				.mSideA = {{.mPokemon = &member1, .mPosition = 0},
						   {.mPokemon = &member2, .mPosition = 1},
						   {.mPokemon = &member3, .mPosition = 2},
						   {.mPokemon = &member6, .mPosition = 3},},
				.mSideB = {{.mPokemon = &member4, .mPosition = 0}, {.mPokemon = &member5, .mPosition = 1}},
			}),
		};
		BattleTarget source{.mSide = Side::A};
		std::vector<BattleTarget> targets{};

		WHEN("calling with a source not adjacent to any targets")
		{
			source.mSlotIndex = 3;

			appendEligibleTargets(targets, state, source, BattleRangeID::Adjacent, Side::B);

			THEN("no targets are added to the vector")
			{
				CHECK(targets.empty());
			}
		}

		WHEN("calling with a source not adjacent to any targets but is using an unrestricted range move")
		{
			source.mSlotIndex = 3;

			appendEligibleTargets(targets, state, source, BattleRangeID::Unrestricted, Side::B);

			THEN("all opposing targets are added to the vector")
			{
				CHECK((targets.size() == state.mSideB.size()));
			}
		}

		WHEN("calling with a source adjacent to one target")
		{
			source.mSlotIndex = 2;

			appendEligibleTargets(targets, state, source, BattleRangeID::Adjacent, Side::B);

			THEN("the adjacent target is added to the vector")
			{
				CHECK((targets.size() == 1));
			}
		}

		WHEN("calling with a source adjacent to multiple targets")
		{
			source.mSide = Side::B;
			source.mSlotIndex = 1;

			appendEligibleTargets(targets, state, source, BattleRangeID::Adjacent, Side::A);

			THEN("the adjacent targets are added to the vector")
			{
				CHECK((targets.size() == 3));
			}
		}
	}

	GIVEN("getResult")
	{
		Pokemon member1{makePokemon({.mHealth = 50})};
		Pokemon member2{makePokemon({.mHealth = 60})};
		BattleState state{
			makeBattleState({.mPartyA = {&member1}, .mPartyB = {&member2}, .mBattleStarted = true}),
		};

		WHEN("calling when the battle hasn't started")
		{
			state.mBattleStarted = false;

			THEN("it returns NotStarted")
			{
				CHECK((getResult(state) == BattleResult::NotStarted));
			}
		}

		WHEN("calling when both sides are healthy")
		{
			THEN("it returns InProgress")
			{
				CHECK((getResult(state) == BattleResult::InProgress));
			}
		}

		WHEN("calling when side A only is healthy")
		{
			member2.setHealth(0);

			THEN("it returns SideAWon")
			{
				CHECK((getResult(state) == BattleResult::SideAWon));
			}
		}

		WHEN("calling when side B only is healthy")
		{
			member1.setHealth(0);

			THEN("it returns SideBWon")
			{
				CHECK((getResult(state) == BattleResult::SideBWon));
			}
		}

		WHEN("calling when both sides have no healthy Pokemon")
		{
			member1.setHealth(0);
			member2.setHealth(0);

			THEN("it returns Draw")
			{
				CHECK((getResult(state) == BattleResult::Draw));
			}
		}
	}

	GIVEN("hasReserve")
	{
		Pokemon member1{makePokemon({.mHealth = 50})};
		Pokemon member2{makePokemon({.mHealth = 60})};
		Pokemon member3{makePokemon({.mHealth = 60})};
		BattleState state{
			makeBattleState({.mSideA = {{{.mPokemon = &member1}, {.mPokemon = &member2}}}, .mBattleStarted = true}),
		};

		WHEN("calling when no Pokemon in the party are healthy")
		{
			member1.setHealth(0);
			member2.setHealth(0);
			member3.setHealth(0);

			THEN("it returns false")
			{
				std::vector<Pokemon *> trainerParty{&member1, &member2, &member3};
				CHECK_FALSE((hasReserve(state, Side::A, trainerParty)));
			}
		}

		WHEN("calling when no Pokemon other than the active ones exist")
		{
			THEN("it returns false")
			{
				std::vector<Pokemon *> trainerParty{&member1, &member2};
				CHECK_FALSE((hasReserve(state, Side::A, trainerParty)));
			}
		}

		WHEN("calling when a Pokemon other than the active Pokemon exist")
		{
			THEN("it returns true")
			{
				std::vector<Pokemon *> trainerParty{&member1, &member2, &member3};
				CHECK((hasReserve(state, Side::A, trainerParty)));
			}
		}
	}

	GIVEN("resolveTargets")
	{
		Pokemon member1{makePokemon({.mHealth = 50})};
		Pokemon member2{makePokemon({.mHealth = 60})};
		Pokemon member3{makePokemon({.mHealth = 30})};
		Pokemon member4{makePokemon({.mHealth = 20})};
		Pokemon member5{makePokemon({.mHealth = 10})};
		Pokemon member6{makePokemon({.mHealth = 40})};
		Pokemon member7{makePokemon({.mHealth = 70})};
		BattleState state{
    		makeBattleState({
    			.mSideA = {{.mPokemon = &member1, .mPosition = 0},
    					   {.mPokemon = &member2, .mPosition = 1},
    					   {.mPokemon = &member3, .mPosition = 2},
    					   {.mPokemon = &member6, .mPosition = 3},},
    			.mSideB = {{.mPokemon = &member4, .mPosition = 0}, {.mPokemon = &member5, .mPosition = 1}},
    		}),
    	};
		BattleTarget source{.mSide = Side::B, .mSlotIndex = 1};

		WHEN("calling with an invalid source")
		{
			source.mSlotIndex = 5;
			std::expected<std::vector<BattleTarget>, BattleEngineError> result{
				resolveTargets(state, source, BattleTargetID::Self, BattleRangeID::Adjacent),
			};

			THEN("it returns InvalidActiveSlot")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error() == BattleEngineError::InvalidActiveSlot));
			}
		}

		WHEN("calling when the targetID is Self")
		{
			std::expected<std::vector<BattleTarget>, BattleEngineError> result{
				resolveTargets(state, source, BattleTargetID::Self, BattleRangeID::Adjacent),
			};

			THEN("it returns an array with the source only")
			{
				REQUIRE(result.has_value());

				std::vector<BattleTarget> &targets{result.value()};

				CHECK((targets.size() == 1));
				CHECK((targets.at(0) == source));
			}
		}

		WHEN("calling when the targetID is SingleAlly")
		{
			BattleTarget ally{.mSide = Side::B, .mSlotIndex = 0};

			WHEN("calling with a supplied selected target")
			{
				WHEN("the selected target is targeting the same side as the source")
				{
					ally.mSide = Side::A;
					std::expected<std::vector<BattleTarget>, BattleEngineError> result{
						resolveTargets(state, source, BattleTargetID::SingleAlly, BattleRangeID::Adjacent, ally),
					};

					THEN("it returns InvalidTarget")
					{
						REQUIRE_FALSE(result.has_value());
						CHECK((result.error() == BattleEngineError::InvalidTarget));
					}
				}

				WHEN("the selected target is not able to be hit by the BattleRangeID")
				{
					ally.mSlotIndex = 3;
					std::expected<std::vector<BattleTarget>, BattleEngineError> result{
						resolveTargets(state, source, BattleTargetID::SingleAlly, BattleRangeID::Adjacent, ally),
					};

					THEN("it returns InvalidTarget")
					{
						REQUIRE_FALSE(result.has_value());
						CHECK((result.error() == BattleEngineError::InvalidTarget));
					}
				}

				WHEN("the selected target is able to be resolved")
				{
					std::expected<std::vector<BattleTarget>, BattleEngineError> result{
						resolveTargets(state, source, BattleTargetID::SingleAlly, BattleRangeID::Adjacent, ally),
					};

					THEN("it returns the singular target")
					{
						REQUIRE(result.has_value());

						std::vector<BattleTarget> &targets{result.value()};

						CHECK((targets.size() == 1));
						CHECK((targets.at(0) == ally));
					}
				}
			}

			WHEN("calling without a supplied selected target")
			{
				WHEN("there's more than one valid target")
				{
					state.mSideB = {
						{.mPokemon = &member4, .mPosition = 0},
						{.mPokemon = &member5, .mPosition = 1},
						{.mPokemon = &member7, .mPosition = 2},
					};

					std::expected<std::vector<BattleTarget>, BattleEngineError> result{
						resolveTargets(state, source, BattleTargetID::SingleAlly, BattleRangeID::Adjacent),
					};

					THEN("it returns InvalidTarget")
					{
						REQUIRE_FALSE(result.has_value());
						CHECK((result.error() == BattleEngineError::InvalidTarget));
					}
				}

				WHEN("there's only one valid target")
				{
					std::expected<std::vector<BattleTarget>, BattleEngineError> result{
						resolveTargets(state, source, BattleTargetID::SingleAlly, BattleRangeID::Adjacent),
					};

					THEN("it returns an array with the singular opponent")
					{
						REQUIRE(result.has_value());

						std::vector<BattleTarget> &targets{result.value()};

						CHECK((targets.size() == 1));
						CHECK((targets.at(0) == ally));
					}
				}
			}
		}

		WHEN("calling when the targetID is SingleOpponent")
		{
			WHEN("calling with a supplied selected target")
			{
				BattleTarget target{.mSide = Side::A, .mSlotIndex = 1};

				WHEN("the selected target is targeting the same side as the source")
				{
					target.mSide = Side::B;
					std::expected<std::vector<BattleTarget>, BattleEngineError> result{
						resolveTargets(state, source, BattleTargetID::SingleOpponent, BattleRangeID::Adjacent, target),
					};

					THEN("it returns InvalidTarget")
					{
						REQUIRE_FALSE(result.has_value());
						CHECK((result.error() == BattleEngineError::InvalidTarget));
					}
				}

				WHEN("the selected target is not able to be hit by the BattleRangeID")
				{
					target.mSlotIndex = 3;
					std::expected<std::vector<BattleTarget>, BattleEngineError> result{
						resolveTargets(state, source, BattleTargetID::SingleOpponent, BattleRangeID::Adjacent, target),
					};

					THEN("it returns InvalidTarget")
					{
						REQUIRE_FALSE(result.has_value());
						CHECK((result.error() == BattleEngineError::InvalidTarget));
					}
				}

				WHEN("the selected target is able to be resolved")
				{
					std::expected<std::vector<BattleTarget>, BattleEngineError> result{
						resolveTargets(state, source, BattleTargetID::SingleOpponent, BattleRangeID::Adjacent, target),
					};

					THEN("it returns the singular target")
					{
						REQUIRE(result.has_value());

						std::vector<BattleTarget> &targets{result.value()};

						CHECK((targets.size() == 1));
						CHECK((targets.at(0) == target));
					}
				}
			}

			WHEN("calling without a supplied selected target")
			{
				WHEN("there's more than one valid target")
				{
					std::expected<std::vector<BattleTarget>, BattleEngineError> result{
						resolveTargets(state, source, BattleTargetID::SingleOpponent, BattleRangeID::Adjacent),
					};

					THEN("it returns InvalidTarget")
					{
						REQUIRE_FALSE(result.has_value());
						CHECK((result.error() == BattleEngineError::InvalidTarget));
					}
				}

				WHEN("there's only one valid target")
				{
					state.mSideA = {{.mPokemon = &member1, .mPosition = 0}};
					std::expected<std::vector<BattleTarget>, BattleEngineError> result{
						resolveTargets(state, source, BattleTargetID::SingleOpponent, BattleRangeID::Adjacent),
					};

					THEN("it returns an array with the singular opponent")
					{
						REQUIRE(result.has_value());

						std::vector<BattleTarget> &targets{result.value()};

						BattleTarget opp{.mSide = Side::A, .mSlotIndex = 0};

						CHECK((targets.size() == 1));
						CHECK((targets.at(0) == opp));
					}
				}
			}
		}

		WHEN("calling when the targetID is AllAlliesIncludingSelf")
		{
			std::expected<std::vector<BattleTarget>, BattleEngineError> result{
				resolveTargets(state, source, BattleTargetID::AllAlliesIncludingSelf, BattleRangeID::Adjacent),
			};

			THEN("it returns an array with the all Pokemon onsource only")
			{
				REQUIRE(result.has_value());

				std::vector<BattleTarget> &targets{result.value()};

				BattleTarget ally1{.mSide = Side::B, .mSlotIndex = 0};

				CHECK((targets.size() == 2));
				CHECK((targets.at(0) == ally1));
				CHECK((targets.at(1) == source));
			}
		}

		WHEN("calling when the targetID is AllAlliesExceptSelf")
		{
			std::expected<std::vector<BattleTarget>, BattleEngineError> result{
				resolveTargets(state, source, BattleTargetID::AllAlliesExceptSelf, BattleRangeID::Adjacent),
			};

			THEN("it returns an array with the all Pokemon onsource only")
			{
				REQUIRE(result.has_value());

				std::vector<BattleTarget> &targets{result.value()};

				BattleTarget ally1{.mSide = Side::B, .mSlotIndex = 0};

				CHECK((targets.size() == 1));
				CHECK((targets.at(0) == ally1));
			}
		}

		WHEN("calling when the targetID is AllOpponents")
		{
			std::expected<std::vector<BattleTarget>, BattleEngineError> result{
				resolveTargets(state, source, BattleTargetID::AllOpponents, BattleRangeID::Adjacent),
			};

			THEN("it returns an array with the all Pokemon onsource only")
			{
				REQUIRE(result.has_value());

				std::vector<BattleTarget> &targets{result.value()};

				BattleTarget opp1{.mSide = Side::A, .mSlotIndex = 0};
				BattleTarget opp2{.mSide = Side::A, .mSlotIndex = 1};
				BattleTarget opp3{.mSide = Side::A, .mSlotIndex = 2};

				CHECK((targets.size() == 3));
				CHECK((targets.at(0) == opp1));
				CHECK((targets.at(1) == opp2));
				CHECK((targets.at(2) == opp3));
			}
		}

		WHEN("calling when the targetID is AllExceptSelf")
		{
			std::expected<std::vector<BattleTarget>, BattleEngineError> result{
				resolveTargets(state, source, BattleTargetID::AllExceptSelf, BattleRangeID::Adjacent),
			};

			THEN("it returns an array with the all Pokemon onsource only")
			{
				REQUIRE(result.has_value());

				std::vector<BattleTarget> &targets{result.value()};

				BattleTarget opp1{.mSide = Side::A, .mSlotIndex = 0};
				BattleTarget opp2{.mSide = Side::A, .mSlotIndex = 1};
				BattleTarget opp3{.mSide = Side::A, .mSlotIndex = 2};
				BattleTarget ally1{.mSide = Side::B, .mSlotIndex = 0};

				CHECK((targets.size() == 4));
				CHECK((targets.at(0) == opp1));
				CHECK((targets.at(1) == opp2));
				CHECK((targets.at(2) == opp3));
				CHECK((targets.at(3) == ally1));
			}
		}

		WHEN("calling with an unknown BattleTargetID")
		{
			std::expected<std::vector<BattleTarget>, BattleEngineError> result{
				resolveTargets(state, source, static_cast<BattleTargetID>(200), BattleRangeID::Adjacent),
			};

			THEN("it returns InvalidTarget")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error() == BattleEngineError::InvalidTarget));
			}
		}

		WHEN("calling when there are no valid targets")
		{
			source.mSide = Side::A;
			source.mSlotIndex = 3;

			std::expected<std::vector<BattleTarget>, BattleEngineError> result{
				resolveTargets(state, source, BattleTargetID::AllOpponents, BattleRangeID::Adjacent),
			};

			THEN("it returns InvalidTarget")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error() == BattleEngineError::InvalidTarget));
			}
		}
	}

	GIVEN("getMoveTargets")
	{
		RegistryProvider provider{getDefaultInitializedRegistryProvider()};
		const MoveRegistry *moveRegistry{provider.mMoveRegistry};
		BattleTarget target{.mSide = Side::B, .mSlotIndex = 1};
		MoveAction action{.mSelectedTarget = target, .mSide = Side::A, .mUserSlotIndex = 0, .mMoveSlotIndex = 0};

		Pokemon member1{
			makePokemon({
			    .mMoveMetas = {
					{
					    {
							.mMoveID = toMoveID(BuiltinMoveID::Pound),
						},
						{
						    .mMoveID = toMoveID(BuiltinMoveID::KarateChop),
						},
						{
						    .mMoveID = MoveID{200},
						}
					},
				},
			    .mHealth = 50
			}),
		};
		Pokemon member2{makePokemon({.mHealth = 60})};
		Pokemon member3{makePokemon({.mHealth = 60})};
		BattleState state{
			makeBattleState({
				.mSideA = {{{.mPokemon = &member1}}},
				.mSideB = {{{.mPokemon = &member2}, {.mPokemon = &member3}}},
				.mBattleStarted = true,
			}),
		};

		WHEN("calling with a battle that's not already started")
		{
			state.mBattleStarted = false;
			std::expected<std::vector<BattleTarget>, BattleEngineError> result{getMoveTargets(state, action, moveRegistry)};

			THEN("it returns InvalidActiveSlot")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error() == BattleEngineError::InvalidActiveSlot));
			}
		}

		WHEN("calling with an action user slot that is larger than the active slots")
		{
			action.mUserSlotIndex = 5;
			std::expected<std::vector<BattleTarget>, BattleEngineError> result{getMoveTargets(state, action, moveRegistry)};

			THEN("it returns InvalidActiveSlot")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error() == BattleEngineError::InvalidActiveSlot));
			}
		}

		WHEN("calling with an action pointing to a fainted Pokemon")
		{
			member1.setHealth(0);
			std::expected<std::vector<BattleTarget>, BattleEngineError> result{getMoveTargets(state, action, moveRegistry)};

			THEN("it returns InvalidActiveSlot")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error() == BattleEngineError::InvalidActiveSlot));
			}
		}

		WHEN("calling with an action move slot that is larger than the move slots")
		{
			action.mMoveSlotIndex = 5;
			std::expected<std::vector<BattleTarget>, BattleEngineError> result{getMoveTargets(state, action, moveRegistry)};

			THEN("it returns InvalidMoveSlot")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error() == BattleEngineError::InvalidMoveSlot));
			}
		}

		WHEN("calling with a move action that points to invalid move metadata")
		{
			action.mMoveSlotIndex = 2;
			std::expected<std::vector<BattleTarget>, BattleEngineError> result{getMoveTargets(state, action, moveRegistry)};

			THEN("it returns MoveNotFound")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error() == BattleEngineError::MoveNotFound));
			}
		}

		WHEN("calling with a valid move on a valid target")
		{
			std::expected<std::vector<BattleTarget>, BattleEngineError> result{getMoveTargets(state, action, moveRegistry)};

			THEN("it returns the singular target")
			{
				REQUIRE(result.has_value());

				std::vector<BattleTarget> &targets{result.value()};

				CHECK((targets.size() == 1));
				CHECK((targets.at(0) == target));
			}
		}
	}

	GIVEN("getBattleTarget")
	{
		WHEN("calling with a MoveAction")
		{
			MoveAction action{.mSide = Side::B, .mUserSlotIndex = 2};
			BattleTarget result{getBattleTarget(action)};

			THEN("it returns the expected BattleTarget")
			{
				BattleTarget expected{.mSide = Side::B, .mSlotIndex = 2};
				CHECK((result == expected));
			}
		}

		WHEN("calling with a SwitchAction")
		{
			SwitchAction action{.mSide = Side::A, .mActiveSlotIndex = 1};
			BattleTarget result{getBattleTarget(action)};

			THEN("it returns the expected BattleTarget")
			{
				BattleTarget expected{.mSide = Side::A, .mSlotIndex = 1};
				CHECK((result == expected));
			}
		}
	}

	GIVEN("handleMovePrioritization")
	{
		RegistryProvider provider{getDefaultInitializedRegistryProvider()};
		const MoveRegistry *moveRegistry{provider.mMoveRegistry};
		BattleTarget target1{.mSide = Side::B, .mSlotIndex = 0};
		BattleTarget target2{.mSide = Side::B, .mSlotIndex = 1};
		BattleTarget target3{.mSide = Side::B, .mSlotIndex = 2};
		BattleTarget target4{.mSide = Side::A, .mSlotIndex = 0};
		BattleTarget target5{.mSide = Side::A, .mSlotIndex = 1};
		BattleTarget target6{.mSide = Side::A, .mSlotIndex = 2};
		MoveAction pokeAction1{.mSelectedTarget = target1, .mSide = Side::A, .mUserSlotIndex = 0, .mMoveSlotIndex = 0};
		MoveAction pokeAction2{.mSelectedTarget = target2, .mSide = Side::A, .mUserSlotIndex = 1, .mMoveSlotIndex = 0};
		MoveAction pokeAction3{.mSelectedTarget = target3, .mSide = Side::A, .mUserSlotIndex = 2, .mMoveSlotIndex = 0};
		MoveAction pokeAction4{.mSelectedTarget = target4, .mSide = Side::B, .mUserSlotIndex = 0, .mMoveSlotIndex = 0};
		MoveAction pokeAction5{.mSelectedTarget = target5, .mSide = Side::B, .mUserSlotIndex = 1, .mMoveSlotIndex = 0};
		MoveAction pokeAction6{.mSelectedTarget = target6, .mSide = Side::B, .mUserSlotIndex = 2, .mMoveSlotIndex = 0};
		std::vector<MoveAction> actions{pokeAction1, pokeAction2, pokeAction3, pokeAction4, pokeAction5, pokeAction6};

		Pokemon member1{
			makePokemon({
				.mMetadata = {.mBaseStats = {.mSpeed = 10}},
				.mMoveMetas = {
				    {
						{
						    .mMoveID = toMoveID(BuiltinMoveID::Pound),
						},
					},
				},
				.mHealth = 50,
				.mLevel = 1,
			}),
		};
		Pokemon member2{
			makePokemon({
				.mMetadata = {.mBaseStats = {.mSpeed = 20}},
				.mMoveMetas = {
				    {
						{
						    .mMoveID = toMoveID(BuiltinMoveID::ExtremeSpeed),
						},
					},
				},
				.mHealth = 60,
				.mLevel = 25,
			}),
		};
		Pokemon member3{
			makePokemon({
				.mMetadata = {.mBaseStats = {.mSpeed = 30}},
				.mMoveMetas = {
				    {
						{
						    .mMoveID = toMoveID(BuiltinMoveID::Pound),
						},
					},
				},
				.mHealth = 30,
				.mLevel = 40,
			}),
		};
		Pokemon member4{
			makePokemon({
				.mMetadata = {.mBaseStats = {.mSpeed = 40}},
				.mMoveMetas = {
				    {
						{
						    .mMoveID = toMoveID(BuiltinMoveID::Pound),
						},
					},
				},
				.mHealth = 20,
				.mLevel = 50,
			}),
		};
		Pokemon member5{
			makePokemon({
				.mMetadata = {.mBaseStats = {.mSpeed = 60}},
				.mMoveMetas = {
				    {
						{
						    .mMoveID = toMoveID(BuiltinMoveID::QuickAttack),
						},
					},
				},
				.mHealth = 10,
				.mLevel = 20,
			}),
		};
		Pokemon member6{
			makePokemon({
				.mMetadata = {.mBaseStats = {.mSpeed = 40}},
				.mMoveMetas = {
				    {
						{
						    .mMoveID = toMoveID(BuiltinMoveID::Pound),
						},
					},
				},
				.mHealth = 40,
				.mLevel = 50,
			}),
		};
		BattleState state{
			makeBattleState({
				.mSideA
				= {{.mPokemon = &member1, .mPosition = 0}, {.mPokemon = &member2, .mPosition = 1}, {.mPokemon = &member3, .mPosition = 2}},
				.mSideB
				= {{.mPokemon = &member4, .mPosition = 0}, {.mPokemon = &member5, .mPosition = 1}, {.mPokemon = &member6, .mPosition = 2}},
			}),
		};

		WHEN("a move in the stable sort is a nullptr")
		{
			member1.setMoveID(1, MoveID{200});
			MoveAction newAction2{.mSelectedTarget = target1, .mSide = Side::A, .mUserSlotIndex = 0, .mMoveSlotIndex = 1};
			actions.at(1) = newAction2;
			SequenceRandom<std::size_t> seq{2, 3, 4, 0, 1};
			handleMovePrioritization(state, actions, moveRegistry, seq);

			THEN("the set of moves is properly sorted with the null move at the end and respects priority and effective speed")
			{
				CHECK((actions.at(0) == pokeAction5));
				CHECK((actions.at(1) == pokeAction6));
				CHECK((actions.at(2) == pokeAction4));
				CHECK((actions.at(3) == pokeAction3));
				CHECK((actions.at(4) == pokeAction1));
				CHECK((actions.at(5) == newAction2));
			}
		}

		WHEN("all moves are valid")
		{
			SequenceRandom<std::size_t> seq{2, 3, 4, 0, 1};
			handleMovePrioritization(state, actions, moveRegistry, seq);

			THEN("the set of moves is properly sorted based on priority and effective speed")
			{
				CHECK((actions.at(0) == pokeAction2));
				CHECK((actions.at(1) == pokeAction5));
				CHECK((actions.at(2) == pokeAction6));
				CHECK((actions.at(3) == pokeAction4));
				CHECK((actions.at(4) == pokeAction3));
				CHECK((actions.at(5) == pokeAction1));
			}
		}

		WHEN("moves are prioritized through an engine-style random source reference wrapper")
		{
			member1.setMoveID(1, MoveID{200});
			MoveAction newAction2{.mSelectedTarget = target1, .mSide = Side::A, .mUserSlotIndex = 0, .mMoveSlotIndex = 1};
			actions.at(1) = newAction2;
			SequenceRandom<std::size_t> sequence{2, 3, 4, 0, 1};
			CallableRandomSource<std::size_t, std::reference_wrapper<SequenceRandom<std::size_t>>> source{std::ref(sequence)};
			IRandomSource<std::size_t> &interface{source};
			auto randomSource{std::ref(interface)};
			handleMovePrioritization(state, actions, moveRegistry, randomSource);

			THEN("priority and speed determine execution order while shuffled ties remain stable")
			{
				CHECK((actions.at(0) == pokeAction5));
				CHECK((actions.at(1) == pokeAction6));
				CHECK((actions.at(2) == pokeAction4));
				CHECK((actions.at(3) == pokeAction3));
				CHECK((actions.at(4) == pokeAction1));
				CHECK((actions.at(5) == newAction2));
				CHECK((sequence.calls() == 5));
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
    				.mMetadata = {
                        .mName = "TestMon",
        				.mBaseStats = {
        					.mMaxHealth = 150,
        					.mAttack = 101,
        					.mDefense = 102,
        					.mSpAttack = 103,
        					.mSpDefense = 104,
        					.mSpeed = 105,
        				},
        				.mTypeIDs = {toTypeID(BuiltinTypeID::Fire), toTypeID(BuiltinTypeID::Bug)},
                    },
                    .mMoveMetas {
                        {
                            {
                                .mMoveID = toMoveID(BuiltinMoveID::Pound),
                                .mPPMaxAmount = 15,
                                .mPPDefaultAmount = 5,
                            },
                            {
                                .mMoveID = toMoveID(BuiltinMoveID::KarateChop),
                                .mPPMaxAmount = 20,
                                .mPPDefaultAmount = 10,
                           	},
                            {
                                .mMoveID = toMoveID(BuiltinMoveID::Facade),
                                .mPPMaxAmount = 25,
                                .mPPDefaultAmount = 15,
                            },
                            {
                                .mMoveID = toMoveID(BuiltinMoveID::HydroSteam),
                                .mPPMaxAmount = 30,
                                .mPPDefaultAmount = 20,
                            },
                        },
                    },
    				.mStatusIDs
    				= {toStatusID(BuiltinStatusID::Poison), toStatusID(BuiltinStatusID::Toxic), toStatusID(BuiltinStatusID::Paralysis),
    				   toStatusID(BuiltinStatusID::Sleep), toStatusID(BuiltinStatusID::Freeze),},
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
						"        ID: 4\n"
						"        Name: Facade\n"
						"        PP: 15/25\n"
						"      [3]:\n"
						"        ID: 5\n"
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
    				.mMetadata = {
                        .mName = "TestMon",
        				.mBaseStats = {
        					.mMaxHealth = 150,
        					.mAttack = 101,
        					.mDefense = 102,
        					.mSpAttack = 103,
        					.mSpDefense = 104,
        					.mSpeed = 105,
        				},
        				.mTypeIDs = {toTypeID(BuiltinTypeID::Fire), toTypeID(BuiltinTypeID::Bug)},
                    },
                    .mMoveMetas {
                        {
                            {
                                .mMoveID = toMoveID(BuiltinMoveID::Pound),
                                .mPPMaxAmount = 15,
                                .mPPDefaultAmount = 5,
                            },
                            {
                                .mMoveID = toMoveID(BuiltinMoveID::KarateChop),
                                .mPPMaxAmount = 20,
                                .mPPDefaultAmount = 10,
                           	},
                            {
                                .mMoveID = toMoveID(BuiltinMoveID::Facade),
                                .mPPMaxAmount = 25,
                                .mPPDefaultAmount = 15,
                            },
                            {
                                .mMoveID = toMoveID(BuiltinMoveID::HydroSteam),
                                .mPPMaxAmount = 30,
                                .mPPDefaultAmount = 20,
                            },
                        },
                    },
    				.mStatusIDs
    				= {toStatusID(BuiltinStatusID::Poison), toStatusID(BuiltinStatusID::Toxic), toStatusID(BuiltinStatusID::Paralysis),
    				   toStatusID(BuiltinStatusID::Sleep), toStatusID(BuiltinStatusID::Freeze),},
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
						"        ID: 4\n"
						"        Name: Facade\n"
						"        PP: 15/25\n"
						"      [3]:\n"
						"        ID: 5\n"
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
