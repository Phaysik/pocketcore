/*! @file pokemon.test.cpp
	@brief C++ file for running tests for the PokemonRegistry.
	@date 09/02/2026
	@since 0.4.0
	@version 0.12.17
	@author Matthew Moore
*/

#include "Pokemon/pokemon.h"

#include <array>
#include <optional>
#include <sstream>
#include <string_view>

#include "Ability/abilityID.h"
#include "Ability/builtInAbilityID.h"
#include "Configuration/constants.h"
#include "Core/typedefs.h"
#include "Interaction/interaction.h"
#include "Item/builtInItemID.h"
#include "Item/itemID.h"
#include "Move/builtInMoveID.h"
#include "Move/moveID.h"
#include "Nature/builtInNatureID.h"
#include "Nature/natureID.h"
#include "Pokemon/pokemon.testHelper.h"
#include "Registry/registryProvider.h"
#include "Registry/registryProvider.testHelper.h"
#include "Registry/statusRegistry.h"
#include "Status/builtInStatusID.h"
#include "Status/statusID.h"
#include "Types/builtInTypeID.h"
#include "Types/typeID.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Ability::AbilityID;
using PocketCore::Ability::BuiltinAbilityID;
using PocketCore::Ability::NO_ABILITY_ID;
using PocketCore::Ability::toAbilityID;
using PocketCore::Configuration::MAX_ABILITIES_PER_POKEMON;
using PocketCore::Configuration::MAX_ITEMS_PER_POKEMON;
using PocketCore::Configuration::MAX_MOVES_PER_POKEMON;
using PocketCore::Configuration::MAX_NATURES_PER_POKEMON;
using PocketCore::Configuration::MAX_STATUSES_PER_POKEMON;
using PocketCore::Configuration::MAX_TYPES_PER_POKEMON;
using PocketCore::Core::ub;
using PocketCore::Core::us;
using PocketCore::Interaction::InteractionAction;
using PocketCore::Item::BuiltinItemID;
using PocketCore::Item::ItemID;
using PocketCore::Item::NO_ITEM_ID;
using PocketCore::Item::toItemID;
using PocketCore::Move::BuiltinMoveID;
using PocketCore::Move::MoveID;
using PocketCore::Move::NO_MOVE_ID;
using PocketCore::Move::toMoveID;
using PocketCore::Nature::BuiltinNatureID;
using PocketCore::Nature::NatureID;
using PocketCore::Nature::NO_NATURE_ID;
using PocketCore::Nature::toNatureID;
using PocketCore::Pokemon::Pokemon;
using PocketCore::Registry::RegistryProvider;
using PocketCore::Registry::Status::StatusRegistry;
using PocketCore::Status::BuiltinStatusID;
using PocketCore::Status::NO_STATUS_ID;
using PocketCore::Status::StatusID;
using PocketCore::Status::toStatusID;
using PocketCore::Testing::getDefaultInitializedRegistryProvider;
using PocketCore::Testing::makePokemon;
using PocketCore::Type::BuiltinTypeID;
using PocketCore::Type::NO_TYPE_ID;
using PocketCore::Type::toTypeID;
using PocketCore::Type::TypeID;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("Pokemon")
{
	GIVEN("constructors")
	{
		WHEN("creating a Pokemon with empty move slots and zero move PP")
		{
			Pokemon pokemon{"TestMon",
							100,
							100,
							100,
							100,
							100,
							100,
							100,
							{toAbilityID(BuiltinAbilityID::CloudNine)},
							{toItemID(BuiltinItemID::ChestoBerry)},
							{toTypeID(BuiltinTypeID::Dark)}};

			THEN("the fields are properly defaulted")
			{
				CHECK((std::ranges::all_of(pokemon.getMoveIDsArray(), [](const MoveID &moveID) { return moveID == NO_MOVE_ID; })));
				CHECK((std::ranges::all_of(pokemon.getMaxPPArray(), [](const ub maxPP) { return maxPP == 0; })));
				CHECK((std::ranges::all_of(pokemon.getCurrentPPArray(), [](const ub currentPP) { return currentPP == 0; })));
				CHECK((pokemon.getLevelDamageFactor() == 42));
			}
		}

		WHEN("creating a Pokemon with move slots and move PP")
		{
			Pokemon pokemon{"TestMon",
							{
								toMoveID(BuiltinMoveID::Facade),
								toMoveID(BuiltinMoveID::Facade),
								toMoveID(BuiltinMoveID::Facade),
								toMoveID(BuiltinMoveID::Facade),
							},
							{10, 10, 10, 10},
							{10, 10, 10, 10},
							100,
							100,
							100,
							100,
							100,
							100,
							100,
							{toAbilityID(BuiltinAbilityID::CloudNine)},
							{toItemID(BuiltinItemID::ChestoBerry)},
							{toTypeID(BuiltinTypeID::Dark)}};

			THEN("the fields are properly defaulted")
			{
				CHECK((std::ranges::all_of(pokemon.getMoveIDsArray(),
										   [](const MoveID &moveID) { return moveID == toMoveID(BuiltinMoveID::Facade); })));
				CHECK((std::ranges::all_of(pokemon.getMaxPPArray(), [](const ub maxPP) { return maxPP == 10; })));
				CHECK((std::ranges::all_of(pokemon.getCurrentPPArray(), [](const ub currentPP) { return currentPP == 10; })));
				CHECK((pokemon.getLevelDamageFactor() == 42));
			}
		}
	}

	GIVEN("get/set name")
	{
		Pokemon pokemon{makePokemon({.mName = "Name"})};

		WHEN("calling getName")
		{
			THEN("the name matches")
			{
				CHECK(("Name" == pokemon.getName()));
			}
		}

		WHEN("calling setName")
		{
			pokemon.setName("ChangedName");

			THEN("the pokemon's name is properly updated")
			{
				CHECK(("ChangedName" == pokemon.getName()));
			}
		}
	}

	GIVEN("get/set arrays")
	{
		GIVEN("status array")
		{
			Pokemon pokemon{makePokemon({.mStatusIDs = {toStatusID(BuiltinStatusID::Freeze)}})};

			WHEN("calling getStatusIDsArray")
			{
				THEN("the statuses match")
				{
					CHECK((std::array<StatusID, MAX_STATUSES_PER_POKEMON>{toStatusID(BuiltinStatusID::Freeze)}
						   == pokemon.getStatusIDsArray()));
				}
			}

			WHEN("calling setStatusIDsArray")
			{
				pokemon.setStatusIDsArray({toStatusID(BuiltinStatusID::Burn)});

				THEN("the pokemon's statuses are properly updated")
				{
					CHECK(
						(std::array<StatusID, MAX_STATUSES_PER_POKEMON>{toStatusID(BuiltinStatusID::Burn)} == pokemon.getStatusIDsArray()));
				}
			}
		}

		GIVEN("move array")
		{
			Pokemon pokemon{makePokemon({.mMoveIDs = {toMoveID(BuiltinMoveID::HydroSteam)}})};

			WHEN("calling getMoveIDsArray")
			{
				THEN("the moves match")
				{
					CHECK((std::array<MoveID, MAX_MOVES_PER_POKEMON>{toMoveID(BuiltinMoveID::HydroSteam)} == pokemon.getMoveIDsArray()));
				}
			}

			WHEN("calling setMoveIDsArray")
			{
				pokemon.setMoveIDsArray({toMoveID(BuiltinMoveID::Facade)});

				THEN("the pokemon's moves are properly updated")
				{
					CHECK((std::array<MoveID, MAX_MOVES_PER_POKEMON>{toMoveID(BuiltinMoveID::Facade)} == pokemon.getMoveIDsArray()));
				}
			}
		}

		GIVEN("max pp array")
		{
			Pokemon pokemon{makePokemon({.mMaxPP = {0}})};

			WHEN("calling getMaxPPArray")
			{
				THEN("the max pps match")
				{
					CHECK((std::array<ub, MAX_MOVES_PER_POKEMON>{0} == pokemon.getMaxPPArray()));
				}
			}

			WHEN("calling setMaxPPArray")
			{
				pokemon.setMaxPPArray({1});

				THEN("the pokemon's max pps are properly updated")
				{
					CHECK((std::array<ub, MAX_MOVES_PER_POKEMON>{1} == pokemon.getMaxPPArray()));
				}
			}
		}

		GIVEN("current pp array")
		{
			Pokemon pokemon{makePokemon({.mCurrentPP = {0}})};

			WHEN("calling getCurrentPPArray")
			{
				THEN("the current pps match")
				{
					CHECK((std::array<ub, MAX_MOVES_PER_POKEMON>{0} == pokemon.getCurrentPPArray()));
				}
			}

			WHEN("calling setCurrentPPArray")
			{
				pokemon.setCurrentPPArray({1});

				THEN("the pokemon's current pps are properly updated")
				{
					CHECK((std::array<ub, MAX_MOVES_PER_POKEMON>{1} == pokemon.getCurrentPPArray()));
				}
			}
		}

		GIVEN("type array")
		{
			Pokemon pokemon{makePokemon({.mTypesIDs = {toTypeID(BuiltinTypeID::Bug)}})};

			WHEN("calling getTypesIDsArray")
			{
				THEN("the types match")
				{
					CHECK((std::array<TypeID, MAX_TYPES_PER_POKEMON>{toTypeID(BuiltinTypeID::Bug)} == pokemon.getTypeIDsArray()));
				}
			}

			WHEN("calling setTypeIDsArray")
			{
				pokemon.setTypeIDsArray({toTypeID(BuiltinTypeID::Fire)});

				THEN("the pokemon's types are properly updated")
				{
					CHECK((std::array<TypeID, MAX_TYPES_PER_POKEMON>{toTypeID(BuiltinTypeID::Fire)} == pokemon.getTypeIDsArray()));
				}
			}
		}

		GIVEN("ability array")
		{
			Pokemon pokemon{makePokemon({.mAbilityIDs = {toAbilityID(BuiltinAbilityID::AirLock)}})};

			WHEN("calling getAbilityIDsArray")
			{
				THEN("the abilities match")
				{
					CHECK((std::array<AbilityID, MAX_ABILITIES_PER_POKEMON>{toAbilityID(BuiltinAbilityID::AirLock)}
						   == pokemon.getAbilityIDsArray()));
				}
			}

			WHEN("calling setAbilityIDsArray")
			{
				pokemon.setAbilityIDsArray({toAbilityID(BuiltinAbilityID::CloudNine)});

				THEN("the pokemon's abilities are properly updated")
				{
					CHECK((std::array<AbilityID, MAX_ABILITIES_PER_POKEMON>{toAbilityID(BuiltinAbilityID::CloudNine)}
						   == pokemon.getAbilityIDsArray()));
				}
			}
		}

		GIVEN("item array")
		{
			Pokemon pokemon{makePokemon({.mItemIDs = {toItemID(BuiltinItemID::CheriBerry)}})};

			WHEN("calling getItemsIDsArray")
			{
				THEN("the items match")
				{
					CHECK((std::array<ItemID, MAX_ITEMS_PER_POKEMON>{toItemID(BuiltinItemID::CheriBerry)} == pokemon.getItemsIDsArray()));
				}
			}

			WHEN("calling setItemIDsArray")
			{
				pokemon.setItemIDsArray({toItemID(BuiltinItemID::ChestoBerry)});

				THEN("the pokemon's items are properly updated")
				{
					CHECK((std::array<ItemID, MAX_ITEMS_PER_POKEMON>{toItemID(BuiltinItemID::ChestoBerry)} == pokemon.getItemsIDsArray()));
				}
			}
		}

		GIVEN("nature array")
		{
			Pokemon pokemon{makePokemon({.mNatureIDs = {toNatureID(BuiltinNatureID::Hasty)}})};

			WHEN("calling getNatureIDsArray")
			{
				THEN("the natures match")
				{
					CHECK(
						(std::array<NatureID, MAX_NATURES_PER_POKEMON>{toNatureID(BuiltinNatureID::Hasty)} == pokemon.getNatureIDsArray()));
				}
			}

			WHEN("calling setNatureIDsArray")
			{
				pokemon.setNatureIDsArray({toNatureID(BuiltinNatureID::Lonely)});

				THEN("the pokemon's natures are properly updated")
				{
					CHECK((std::array<NatureID, MAX_NATURES_PER_POKEMON>{toNatureID(BuiltinNatureID::Lonely)}
						   == pokemon.getNatureIDsArray()));
				}
			}
		}
	}

	GIVEN("get/set array index")
	{
		GIVEN("status array index")
		{
			Pokemon pokemon{makePokemon({.mStatusIDs = {toStatusID(BuiltinStatusID::Freeze)}})};

			WHEN("calling getStatusID")
			{
				THEN("the status matches")
				{
					CHECK((toStatusID(BuiltinStatusID::Freeze) == pokemon.getStatusID(0)));
				}
			}

			WHEN("calling setStatusID")
			{
				pokemon.setStatusID(0, {toStatusID(BuiltinStatusID::Burn)});

				THEN("the pokemon's status is properly updated")
				{
					CHECK((toStatusID(BuiltinStatusID::Burn) == pokemon.getStatusID(0)));
				}
			}
		}

		GIVEN("move array index")
		{
			Pokemon pokemon{makePokemon({.mMoveIDs = {toMoveID(BuiltinMoveID::HydroSteam)}})};

			WHEN("calling getMoveID")
			{
				THEN("the move matches")
				{
					CHECK((toMoveID(BuiltinMoveID::HydroSteam) == pokemon.getMoveID(0)));
				}
			}

			WHEN("calling setMoveID")
			{
				pokemon.setMoveIDsArray({toMoveID(BuiltinMoveID::Facade)});

				THEN("the pokemon's move is properly updated")
				{
					CHECK((toMoveID(BuiltinMoveID::Facade) == pokemon.getMoveID(0)));
				}
			}
		}

		GIVEN("max pp array index")
		{
			Pokemon pokemon{makePokemon({.mMaxPP = {0}})};

			WHEN("calling getMaxPP")
			{
				THEN("the max pp matches")
				{
					CHECK((0 == pokemon.getMaxPP(0)));
				}
			}

			WHEN("calling setMaxPPArray")
			{
				pokemon.setMaxPPArray({1});

				THEN("the pokemon's max pp is properly updated")
				{
					CHECK((1 == pokemon.getMaxPP(0)));
				}
			}
		}

		GIVEN("current pp array index")
		{
			Pokemon pokemon{makePokemon({.mCurrentPP = {0}})};

			WHEN("calling getCurrentPP")
			{
				THEN("the current pp matches")
				{
					CHECK((0 == pokemon.getCurrentPP(0)));
				}
			}

			WHEN("calling setCurrentPPArray")
			{
				pokemon.setCurrentPPArray({1});

				THEN("the pokemon's current pp is properly updated")
				{
					CHECK((1 == pokemon.getCurrentPP(0)));
				}
			}
		}

		GIVEN("type array index")
		{
			Pokemon pokemon{makePokemon({.mTypesIDs = {toTypeID(BuiltinTypeID::Bug)}})};

			WHEN("calling getTypeID")
			{
				THEN("the type matches")
				{
					CHECK((toTypeID(BuiltinTypeID::Bug) == pokemon.getTypeID(0)));
				}
			}

			WHEN("calling setTypeID")
			{
				pokemon.setTypeID(0, {toTypeID(BuiltinTypeID::Fire)});

				THEN("the pokemon's type is properly updated")
				{
					CHECK((toTypeID(BuiltinTypeID::Fire) == pokemon.getTypeID(0)));
				}
			}
		}

		GIVEN("ability array index")
		{
			Pokemon pokemon{makePokemon({.mAbilityIDs = {toAbilityID(BuiltinAbilityID::AirLock)}})};

			WHEN("calling getAbilityID")
			{
				THEN("the ability matches")
				{
					CHECK((toAbilityID(BuiltinAbilityID::AirLock) == pokemon.getAbilityID(0)));
				}
			}

			WHEN("calling setAbilityID")
			{
				pokemon.setAbilityID(0, {toAbilityID(BuiltinAbilityID::CloudNine)});

				THEN("the pokemon's ability is properly updated")
				{
					CHECK((toAbilityID(BuiltinAbilityID::CloudNine) == pokemon.getAbilityID(0)));
				}
			}
		}

		GIVEN("item array index")
		{
			Pokemon pokemon{makePokemon({.mItemIDs = {toItemID(BuiltinItemID::CheriBerry)}})};

			WHEN("calling getItemID")
			{
				THEN("the item matches")
				{
					CHECK((toItemID(BuiltinItemID::CheriBerry) == pokemon.getItemID(0)));
				}
			}

			WHEN("calling setItemID")
			{
				pokemon.setItemID(0, {toItemID(BuiltinItemID::ChestoBerry)});

				THEN("the pokemon's item is properly updated")
				{
					CHECK((toItemID(BuiltinItemID::ChestoBerry) == pokemon.getItemID(0)));
				}
			}
		}

		GIVEN("nature array index")
		{
			Pokemon pokemon{makePokemon({.mNatureIDs = {toNatureID(BuiltinNatureID::Hasty)}})};

			WHEN("calling getNatureID")
			{
				THEN("the nature matches")
				{
					CHECK((toNatureID(BuiltinNatureID::Hasty) == pokemon.getNatureID(0)));
				}
			}

			WHEN("calling setNatureID")
			{
				pokemon.setNatureID(0, {toNatureID(BuiltinNatureID::Lonely)});

				THEN("the pokemon's nature is properly updated")
				{
					CHECK((toNatureID(BuiltinNatureID::Lonely) == pokemon.getNatureID(0)));
				}
			}
		}
	}

	GIVEN("get/set base stats")
	{
		GIVEN("health")
		{
			Pokemon pokemon{makePokemon({.mHealth = 15, .mMaximumHealth = 30})};

			WHEN("calling getHealth")
			{
				THEN("the health matches")
				{
					CHECK((15 == pokemon.getHealth()));
				}
			}

			WHEN("calling setHealth")
			{
				pokemon.setHealth(25);

				THEN("the pokemon's health is properly updated")
				{
					CHECK((25 == pokemon.getHealth()));
				}
			}

			WHEN("calling setHealth with more health than the maximum health")
			{
				pokemon.setHealth(50);

				THEN("the pokemon's health is clamped to the maximum")
				{
					CHECK((30 == pokemon.getHealth()));
				}
			}
		}

		GIVEN("maximumHealth")
		{
			Pokemon pokemon{makePokemon({.mMaximumHealth = 5})};

			WHEN("calling getMaximumHealth")
			{
				THEN("the maximumHealth matches")
				{
					CHECK((5 == pokemon.getMaximumHealth()));
				}
			}

			WHEN("calling setMaximumHealth")
			{
				pokemon.setMaximumHealth(10);

				THEN("the pokemon's maximum health is properly updated")
				{
					CHECK((10 == pokemon.getMaximumHealth()));
				}
			}
		}

		GIVEN("attack")
		{
			Pokemon pokemon{makePokemon({.mAttack = 5})};

			WHEN("calling getAttack")
			{
				THEN("the attack matches")
				{
					CHECK((5 == pokemon.getAttack()));
				}
			}

			WHEN("calling setAttack")
			{
				pokemon.setAttack(10);

				THEN("the pokemon's attack is properly updated")
				{
					CHECK((10 == pokemon.getAttack()));
				}
			}
		}

		GIVEN("defense")
		{
			Pokemon pokemon{makePokemon({.mDefense = 10})};

			WHEN("calling getDefense")
			{
				THEN("the defense matches")
				{
					CHECK((10 == pokemon.getDefense()));
				}
			}

			WHEN("calling setDefense")
			{
				pokemon.setDefense(20);

				THEN("the pokemon's defense is properly updated")
				{
					CHECK((20 == pokemon.getDefense()));
				}
			}
		}

		GIVEN("special attack")
		{
			Pokemon pokemon{makePokemon({.mSpecialAttack = 9})};

			WHEN("calling getSpAttack")
			{
				THEN("the spAttack matches")
				{
					CHECK((9 == pokemon.getSpAttack()));
				}
			}

			WHEN("calling setSpAttack")
			{
				pokemon.setSpAttack(18);

				THEN("the pokemon's special attack is properly updated")
				{
					CHECK((18 == pokemon.getSpAttack()));
				}
			}
		}

		GIVEN("special defense")
		{
			Pokemon pokemon{makePokemon({.mSpecialDefense = 8})};

			WHEN("calling getSpDefense")
			{
				THEN("the spDefense matches")
				{
					CHECK((8 == pokemon.getSpDefense()));
				}
			}

			WHEN("calling setSpDefense")
			{
				pokemon.setSpDefense(16);

				THEN("the pokemon's special defense is properly updated")
				{
					CHECK((16 == pokemon.getSpDefense()));
				}
			}
		}

		GIVEN("speed")
		{
			Pokemon pokemon{makePokemon({.mSpeed = 12})};

			WHEN("calling getSpeed")
			{
				THEN("the speed matches")
				{
					CHECK((12 == pokemon.getSpeed()));
				}
			}

			WHEN("calling setSpeed")
			{
				pokemon.setSpeed(24);

				THEN("the pokemon's speed is properly updated")
				{
					CHECK((24 == pokemon.getSpeed()));
				}
			}
		}
	}

	GIVEN("get/set level and level damage factor")
	{
		Pokemon pokemon{makePokemon({.mLevel = 20})};

		WHEN("calling getLevel and getLevelDamageFactor")
		{
			THEN("the level and level damage factor matches")
			{
				CHECK((20 == pokemon.getLevel()));
				CHECK((10 == pokemon.getLevelDamageFactor()));
			}
		}

		WHEN("calling setLevel")
		{
			pokemon.setLevel(30);

			THEN("the pokemon's level and level damage factor are properly updated")
			{
				CHECK((30 == pokemon.getLevel()));
				CHECK((14 == pokemon.getLevelDamageFactor()));
			}
		}
	}

	GIVEN("usePP")
	{
		Pokemon pokemon{makePokemon({.mMoveIDs = {toMoveID(BuiltinMoveID::Facade)}, .mMaxPP = {1}, .mCurrentPP = {1}})};

		WHEN("using a move")
		{
			pokemon.usePP(0);

			THEN("the current PP is decreased by one")
			{
				CHECK((pokemon.getCurrentPP(0) == 0));
			}
		}

		WHEN("using a move after it already has no pp left")
		{
			pokemon.usePP(0);
			pokemon.usePP(0);

			THEN("the current pp stays at zero")
			{
				CHECK((pokemon.getCurrentPP(0) == 0));
			}
		}
	}

	GIVEN("isFainted")
	{
		Pokemon pokemon{makePokemon({.mHealth = 5, .mMaximumHealth = 10})};

		WHEN("the pokemon has health")
		{
			THEN("the pokemon is not fainted")
			{
				CHECK(!pokemon.isFainted());
			}
		}

		WHEN("the pokemon's health is 0")
		{
			pokemon.setHealth(0);

			THEN("the pokemon is fainted")
			{
				CHECK(pokemon.isFainted());
			}
		}
	}

	GIVEN("addStatus")
	{
		StatusRegistry registry{};
		Pokemon pokemon{
			makePokemon({
				.mName = "MissingNo",
				.mAbilityIDs = {AbilityID{}},
				.mItemIDs = {ItemID{}},
				.mHealth = 1,
				.mMaximumHealth = 1,
				.mAttack = 1,
				.mDefense = 1,
				.mSpecialAttack = 1,
				.mSpecialDefense = 1,
				.mLevel = 10,
			}),
		};

		std::array<StatusID, MAX_STATUSES_PER_POKEMON> statusIDs{};

		GIVEN("a current status that blocks the incoming status")
		{
			statusIDs.at(0) = toStatusID(BuiltinStatusID::Freeze);
			statusIDs.at(1) = toStatusID(BuiltinStatusID::Poison);
			pokemon.setStatusIDsArray(statusIDs);

			WHEN("the blocked status would otherwise replace another current status")
			{
				pokemon.addStatus(toStatusID(BuiltinStatusID::Toxic), registry);

				THEN("the incoming status is rejected before any current status changes")
				{
					CHECK((pokemon.getStatusIDsArray() == statusIDs));
				}
			}
		}

		GIVEN("a current status that the incoming status replaces")
		{
			statusIDs.at(0) = toStatusID(BuiltinStatusID::Poison);
			pokemon.setStatusIDsArray(statusIDs);

			WHEN("the incoming status is applied")
			{
				pokemon.addStatus(toStatusID(BuiltinStatusID::Toxic), registry);

				THEN("the incoming status occupies the replaced status slot")
				{
					CHECK((pokemon.getStatusID(0) == toStatusID(BuiltinStatusID::Toxic)));
					CHECK((pokemon.getStatusID(1) == NO_STATUS_ID));
				}
			}
		}

		GIVEN("several current statuses that the incoming status replaces")
		{
			StatusID incomingStatusID{toStatusID(BuiltinStatusID::Toxic)};
			std::optional<us> incomingStatusIndex{registry.findIndexByStatusID(incomingStatusID)};

			REQUIRE(incomingStatusIndex.has_value());

			registry.setEntry(incomingStatusIndex.value(), {
			    .mStatusInteractions = {
					{.mExistingID = toStatusID(BuiltinStatusID::Burn), .mAction = InteractionAction::ReplaceCurrent},
					{.mExistingID = toStatusID(BuiltinStatusID::Poison), .mAction = InteractionAction::ReplaceCurrent},
				},
				.mName = "Toxic",
				.mStatusID = incomingStatusID,
			});

			statusIDs.at(0) = toStatusID(BuiltinStatusID::Burn);
			statusIDs.at(1) = toStatusID(BuiltinStatusID::Sleep);
			statusIDs.at(2) = toStatusID(BuiltinStatusID::Poison);
			statusIDs.at(3) = toStatusID(BuiltinStatusID::Paralysis);

			pokemon.setStatusIDsArray(statusIDs);

			WHEN("the incoming status is applied")
			{
				pokemon.addStatus(incomingStatusID, registry);

				THEN("only the first replacement slot receives the incoming status")
				{
					std::array<StatusID, MAX_STATUSES_PER_POKEMON> expectedStatusIDs{
						incomingStatusID,
						toStatusID(BuiltinStatusID::Sleep),
						toStatusID(BuiltinStatusID::Paralysis),
					};

					CHECK((pokemon.getStatusIDsArray() == expectedStatusIDs));
				}
			}
		}

		GIVEN("several current statuses that the incoming status removes")
		{
			statusIDs.at(0) = toStatusID(BuiltinStatusID::Burn);
			statusIDs.at(1) = toStatusID(BuiltinStatusID::Sleep);
			statusIDs.at(2) = toStatusID(BuiltinStatusID::Paralysis);
			statusIDs.at(3) = toStatusID(BuiltinStatusID::Toxic);
			pokemon.setStatusIDsArray(statusIDs);

			WHEN("the incoming status is applied")
			{
				pokemon.addStatus(toStatusID(BuiltinStatusID::Freeze), registry);

				THEN("the remaining statuses shift down and the incoming status uses the first empty slot")
				{
					CHECK((pokemon.getStatusID(0) == toStatusID(BuiltinStatusID::Toxic)));
					CHECK((pokemon.getStatusID(1) == toStatusID(BuiltinStatusID::Freeze)));
					CHECK((pokemon.getStatusID(2) == NO_STATUS_ID));
					CHECK((pokemon.getStatusID(3) == NO_STATUS_ID));
				}
			}
		}

		GIVEN("a full status array with no matching interaction")
		{
			statusIDs.at(0) = toStatusID(BuiltinStatusID::Burn);
			statusIDs.at(1) = toStatusID(BuiltinStatusID::Sleep);
			statusIDs.at(2) = toStatusID(BuiltinStatusID::Poison);
			statusIDs.at(3) = toStatusID(BuiltinStatusID::Toxic);
			statusIDs.at(4) = toStatusID(BuiltinStatusID::Burn);
			pokemon.setStatusIDsArray(statusIDs);

			WHEN("another coexisting status is applied")
			{
				pokemon.addStatus(toStatusID(BuiltinStatusID::Paralysis), registry);

				THEN("the full status array remains unchanged")
				{
					CHECK((pokemon.getStatusIDsArray() == statusIDs));
				}
			}
		}
	}

	GIVEN("operator<<")
	{
		Pokemon pokemon{
			makePokemon({
				.mName = "TestMon",
				.mStatusIDs = {StatusID{20}, StatusID{21}, StatusID{22}, StatusID{23}, StatusID{24}},
				.mMoveIDs = {MoveID{10}, MoveID{11}, MoveID{12}, MoveID{13}},
				.mMaxPP = {15, 20, 25, 30},
				.mCurrentPP = {5, 10, 15, 20},
				.mTypesIDs = {TypeID{2}, TypeID{3}},
				.mAbilityIDs = {AbilityID{6}},
				.mItemIDs = {ItemID{7}},
				.mNatureIDs = {NatureID{5}},
				.mHealth = 150,
				.mMaximumHealth = 150,
				.mAttack = 101,
				.mDefense = 102,
				.mSpecialAttack = 103,
				.mSpecialDefense = 104,
				.mSpeed = 105,
				.mLevel = 50,
			}),
		};

		WHEN("the Pokemon is written to a stream")
		{
			std::ostringstream output;
			output << pokemon;

			THEN("all public state is written with numeric identifiers and PP")
			{
				std::string_view expected{
					"Pokemon {\n"
					"  Name: TestMon\n"
					"  Level: 50\n"
					"  Level Damage Factor: 22\n"
					"  Health: 150/150\n"
					"  Attack: 101\n"
					"  Defense: 102\n"
					"  Special Attack: 103\n"
					"  Special Defense: 104\n"
					"  Speed: 105\n"
					"  Type IDs: [2, 3]\n"
					"  Nature IDs: [5]\n"
					"  Ability IDs: [6]\n"
					"  Item IDs: [7]\n"
					"  Status IDs: [20, 21, 22, 23, 24]\n"
					"  Moves:\n"
					"    [0] ID: 10, PP: 5/15\n"
					"    [1] ID: 11, PP: 10/20\n"
					"    [2] ID: 12, PP: 15/25\n"
					"    [3] ID: 13, PP: 20/30\n"
					"}",
				};

				CHECK((output.str() == expected));
			}
		}
	}
}

SCENARIO("Pokemon free function")
{
	GIVEN("printPokemonWithNames")
	{
		Pokemon pokemon{
			makePokemon({
				.mName = "TestMon",
				.mStatusIDs
				= {toStatusID(BuiltinStatusID::Poison), toStatusID(BuiltinStatusID::Toxic), toStatusID(BuiltinStatusID::Paralysis),
				   toStatusID(BuiltinStatusID::Sleep), toStatusID(BuiltinStatusID::Freeze),},
				.mMoveIDs = {toMoveID(BuiltinMoveID::Pound), toMoveID(BuiltinMoveID::KarateChop), toMoveID(BuiltinMoveID::Facade), toMoveID(BuiltinMoveID::HydroSteam)},
				.mMaxPP = {15, 20, 25, 30},
				.mCurrentPP = {5, 10, 15, 20},
				.mTypesIDs = {toTypeID(BuiltinTypeID::Fire), toTypeID(BuiltinTypeID::Bug)},
				.mAbilityIDs = {toAbilityID(BuiltinAbilityID::AirLock)},
				.mItemIDs = {toItemID(BuiltinItemID::CheriBerry)},
				.mNatureIDs = {toNatureID(BuiltinNatureID::Adamant)},
				.mHealth = 150,
				.mMaximumHealth = 150,
				.mAttack = 101,
				.mDefense = 102,
				.mSpecialAttack = 103,
				.mSpecialDefense = 104,
				.mSpeed = 105,
				.mLevel = 50,
			}),
		};

		RegistryProvider provider{getDefaultInitializedRegistryProvider()};

		WHEN("the Pokemon is written to a stream")
		{
			std::ostringstream output;
			printPokemonWithNames(output, pokemon, provider);

			THEN("all public state is written with numeric identifiers and PP")
			{
				std::string_view expected{
					"Pokemon {\n"
					"  Name: TestMon\n"
					"  Level: 50\n"
					"  Level Damage Factor: 22\n"
					"  Health: 150/150\n"
					"  Attack: 101\n"
					"  Defense: 102\n"
					"  Special Attack: 103\n"
					"  Special Defense: 104\n"
					"  Speed: 105\n"
					"  Types:\n"
					"    [0]:\n"
					"      ID: 1\n"
					"      Name: Fire\n"
					"    [1]:\n"
					"      ID: 12\n"
					"      Name: Bug\n"
					"  Natures:\n"
					"    [0]:\n"
					"      ID: 4\n"
					"      Name: Adamant\n"
					"  Abilities:\n"
					"    [0]:\n"
					"      ID: 6\n"
					"      Name: Air Lock\n"
					"  Items:\n"
					"    [0]:\n"
					"      ID: 1\n"
					"      Name: Cheri Berry\n"
					"  Statuses:\n"
					"    [0]:\n"
					"      ID: 5\n"
					"      Name: Poison\n"
					"    [1]:\n"
					"      ID: 6\n"
					"      Name: Toxic\n"
					"    [2]:\n"
					"      ID: 1\n"
					"      Name: Paralysis\n"
					"    [3]:\n"
					"      ID: 3\n"
					"      Name: Sleep\n"
					"    [4]:\n"
					"      ID: 4\n"
					"      Name: Freeze\n"
					"  Moves:\n"
					"    [0]:\n"
					"      ID: 1\n"
					"      Name: Pound\n"
					"      PP: 5/15\n"
					"    [1]:\n"
					"      ID: 2\n"
					"      Name: Karate Chop\n"
					"      PP: 10/20\n"
					"    [2]:\n"
					"      ID: 3\n"
					"      Name: Facade\n"
					"      PP: 15/25\n"
					"    [3]:\n"
					"      ID: 4\n"
					"      Name: Hydro Steam\n"
					"      PP: 20/30\n"
					"}",
				};

				CHECK(output.str() == expected);
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
