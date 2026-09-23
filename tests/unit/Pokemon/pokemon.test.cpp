/*! @file pokemon.test.cpp
	@brief C++ file for running tests for the PokemonRegistry.
	@date 09/22/2026
	@since 0.4.0
	@version 0.12.41
	@author Matthew Moore
*/

#include "Pokemon/pokemon.h"

#include <algorithm>
#include <array>
#include <expected>
#include <sstream>
#include <string_view>

#include "Ability/abilityID.h"
#include "Ability/builtInAbilityID.h"
#include "Configuration/constants.h"
#include "Configuration/statusRegistryConfiguration.h"
#include "Core/typedefs.h"
#include "Interaction/interaction.h"
#include "Item/builtInItemID.h"
#include "Item/itemID.h"
#include "Move/builtInMoveID.h"
#include "Move/moveID.h"
#include "Nature/builtInNatureID.h"
#include "Nature/natureID.h"
#include "Pokemon/builtInPokemonID.h"
#include "Pokemon/pokemon.testHelper.h"
#include "Registry/registryError.h"
#include "Registry/statusRegistry.h"
#include "Ruleset/rulesetPolicy.h"
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
using PocketCore::Configuration::MAX_NON_VOLATILE_STATUSES_PER_POKEMON;
using PocketCore::Configuration::MAX_TYPES_PER_POKEMON;
using PocketCore::Configuration::NATURE_STAT_BASE_MULTIPLIER;
using PocketCore::Configuration::StatusRegistryConfiguration;
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
using PocketCore::Pokemon::BuiltinPokemonID;
using PocketCore::Pokemon::Pokemon;
using PocketCore::Pokemon::toPokemonID;
using PocketCore::Registry::RegistryErrorInfo;
using PocketCore::Registry::Status::StatusRegistry;
using PocketCore::Ruleset::RulesetPolicy;
using PocketCore::Status::BuiltinStatusID;
using PocketCore::Status::NO_STATUS_ID;
using PocketCore::Status::StatusID;
using PocketCore::Status::toStatusID;
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
			Pokemon pokemon{
				toPokemonID(BuiltinPokemonID::None),
				"TestMon",
				{
					.mMaxHealth = 100,
					.mAttack = 100,
					.mDefense = 100,
					.mSpAttack = 100,
					.mSpDefense = 100,
					.mSpeed = 100,
				},
				100,
				{toAbilityID(BuiltinAbilityID::CloudNine)},
				{toItemID(BuiltinItemID::ChestoBerry)},
				{toTypeID(BuiltinTypeID::Dark)},
				{toNatureID(BuiltinNatureID::Hardy)},
				{
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
				},
				{},
				{},
			};

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
			Pokemon pokemon{toPokemonID(BuiltinPokemonID::None),
							"TestMon",
							{
								toMoveID(BuiltinMoveID::Facade),
								toMoveID(BuiltinMoveID::Facade),
								toMoveID(BuiltinMoveID::Facade),
								toMoveID(BuiltinMoveID::Facade),
							},
							{10, 10, 10, 10},
							{10, 10, 10, 10},
							{
								.mMaxHealth = 100,
								.mAttack = 100,
								.mDefense = 100,
								.mSpAttack = 100,
								.mSpDefense = 100,
								.mSpeed = 100,
							},
							100,
							{toAbilityID(BuiltinAbilityID::CloudNine)},
							{toItemID(BuiltinItemID::ChestoBerry)},
							{toTypeID(BuiltinTypeID::Dark)},
							{toNatureID(BuiltinNatureID::Hardy)},
							{
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
							},
							{},
							{}};

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
					CHECK((std::array<StatusID, MAX_NON_VOLATILE_STATUSES_PER_POKEMON>{toStatusID(BuiltinStatusID::Freeze)}
						   == pokemon.getStatusIDsArray()));
				}
			}

			WHEN("calling setStatusIDsArray")
			{
				pokemon.setStatusIDsArray({toStatusID(BuiltinStatusID::Burn)});

				THEN("the pokemon's statuses are properly updated")
				{
					CHECK((std::array<StatusID, MAX_NON_VOLATILE_STATUSES_PER_POKEMON>{toStatusID(BuiltinStatusID::Burn)}
						   == pokemon.getStatusIDsArray()));
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
				pokemon.setNatureIDsArray({toNatureID(BuiltinNatureID::Lonely)}, {{{NATURE_STAT_BASE_MULTIPLIER}}});

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
				pokemon.setStatusID(0, toStatusID(BuiltinStatusID::Burn));

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
				pokemon.setMoveID(0, toMoveID(BuiltinMoveID::Facade));

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

			WHEN("calling setMaxPP")
			{
				pokemon.setMaxPP(0, 1);

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

			WHEN("calling setCurrentPP")
			{
				pokemon.setCurrentPP(0, 1);

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
				pokemon.setTypeID(0, toTypeID(BuiltinTypeID::Fire));

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
				pokemon.setAbilityID(0, toAbilityID(BuiltinAbilityID::CloudNine));

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
				pokemon.setItemID(0, toItemID(BuiltinItemID::ChestoBerry));

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
				pokemon.setNatureID(0, toNatureID(BuiltinNatureID::Lonely), {NATURE_STAT_BASE_MULTIPLIER});

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
			Pokemon pokemon{makePokemon({.mStats = {.mMaxHealth = 30}, .mHealth = 15, .mLevel = 50})};

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
				pokemon.setHealth(100);

				THEN("the pokemon's health is clamped to the maximum calculated health")
				{
					CHECK((90 == pokemon.getHealth()));
				}
			}
		}

		GIVEN("maximumHealth")
		{
			Pokemon pokemon{makePokemon({.mStats = {.mMaxHealth = 5}})};

			WHEN("calling getMaximumHealth")
			{
				THEN("the maximumHealth matches")
				{
					CHECK((11 == pokemon.getMaximumHealth()));
				}
			}

			WHEN("calling setMaximumHealth")
			{
				pokemon.setMaximumHealth(100);

				THEN("the pokemon's maximum health is properly updated")
				{
					CHECK((13 == pokemon.getMaximumHealth()));
				}
			}
		}

		GIVEN("attack")
		{
			Pokemon pokemon{makePokemon({.mStats = {.mAttack = 5}})};

			WHEN("calling getAttack")
			{
				THEN("the attack matches")
				{
					CHECK((5 == pokemon.getAttack()));
				}
			}

			WHEN("calling setAttack")
			{
				pokemon.setAttack(100);

				THEN("the pokemon's attack is properly updated")
				{
					CHECK((7 == pokemon.getAttack()));
				}
			}
		}

		GIVEN("defense")
		{
			Pokemon pokemon{makePokemon({.mStats = {.mDefense = 10}})};

			WHEN("calling getDefense")
			{
				THEN("the defense matches")
				{
					CHECK((5 == pokemon.getDefense()));
				}
			}

			WHEN("calling setDefense")
			{
				pokemon.setDefense(200);

				THEN("the pokemon's defense is properly updated")
				{
					CHECK((9 == pokemon.getDefense()));
				}
			}
		}

		GIVEN("special attack")
		{
			Pokemon pokemon{makePokemon({.mStats = {.mSpAttack = 9}})};

			WHEN("calling getSpAttack")
			{
				THEN("the spAttack matches")
				{
					CHECK((5 == pokemon.getSpAttack()));
				}
			}

			WHEN("calling setSpAttack")
			{
				pokemon.setSpAttack(300);

				THEN("the pokemon's special attack is properly updated")
				{
					CHECK((11 == pokemon.getSpAttack()));
				}
			}
		}

		GIVEN("special defense")
		{
			Pokemon pokemon{makePokemon({.mStats = {.mSpDefense = 8}})};

			WHEN("calling getSpDefense")
			{
				THEN("the spDefense matches")
				{
					CHECK((5 == pokemon.getSpDefense()));
				}
			}

			WHEN("calling setSpDefense")
			{
				pokemon.setSpDefense(400);

				THEN("the pokemon's special defense is properly updated")
				{
					CHECK((13 == pokemon.getSpDefense()));
				}
			}
		}

		GIVEN("speed")
		{
			Pokemon pokemon{makePokemon({.mStats = {.mSpeed = 12}})};

			WHEN("calling getSpeed")
			{
				THEN("the speed matches")
				{
					CHECK((5 == pokemon.getSpeed()));
				}
			}

			WHEN("calling setSpeed")
			{
				pokemon.setSpeed(500);

				THEN("the pokemon's speed is properly updated")
				{
					CHECK((15 == pokemon.getSpeed()));
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
		Pokemon pokemon{makePokemon({.mStats = {.mMaxHealth = 10}, .mHealth = 5})};

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
		StatusRegistryConfiguration statusConfiguration{};
		const StatusRegistry &registry{statusConfiguration.getRuntimeRegistry()};

		Pokemon pokemon{
			makePokemon({
				.mName = "MissingNo",
				.mStats = {
					.mMaxHealth = 1,
					.mAttack = 1,
					.mDefense = 1,
					.mSpAttack = 1,
					.mSpDefense = 1,
				},
				.mAbilityIDs = {AbilityID{}},
				.mItemIDs = {ItemID{}},
				.mHealth = 1,
				.mLevel = 10,
			}),
		};

		std::array<StatusID, MAX_NON_VOLATILE_STATUSES_PER_POKEMON> statusIDs{};
		RulesetPolicy policy{.mMaxNonVolatileStatuses = 5, .mReplaceNonVolatileStatusWhenFull = false};

		GIVEN("a current status that blocks the incoming status")
		{
			statusIDs.at(0) = toStatusID(BuiltinStatusID::Freeze);
			statusIDs.at(1) = toStatusID(BuiltinStatusID::Poison);
			pokemon.setStatusIDsArray(statusIDs);

			WHEN("the blocked status would otherwise replace another current status")
			{
				pokemon.addStatus(toStatusID(BuiltinStatusID::Toxic), registry, policy);

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
				pokemon.addStatus(toStatusID(BuiltinStatusID::Toxic), registry, policy);

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
			std::expected<void, RegistryErrorInfo> updateResult{statusConfiguration.updateStatus(incomingStatusID, {
				.mName = "Toxic",
			    .mStatusInteractions = {
					{.mExistingID = toStatusID(BuiltinStatusID::Burn), .mAction = InteractionAction::ReplaceCurrent},
					{.mExistingID = toStatusID(BuiltinStatusID::Poison), .mAction = InteractionAction::ReplaceCurrent},
				},
			}),};
			REQUIRE(updateResult.has_value());

			statusIDs.at(0) = toStatusID(BuiltinStatusID::Burn);
			statusIDs.at(1) = toStatusID(BuiltinStatusID::Sleep);
			statusIDs.at(2) = toStatusID(BuiltinStatusID::Poison);
			statusIDs.at(3) = toStatusID(BuiltinStatusID::Paralysis);

			pokemon.setStatusIDsArray(statusIDs);

			WHEN("the incoming status is applied")
			{
				pokemon.addStatus(incomingStatusID, registry, policy);

				THEN("only the first replacement slot receives the incoming status")
				{
					std::array<StatusID, MAX_NON_VOLATILE_STATUSES_PER_POKEMON> expectedStatusIDs{
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
				pokemon.addStatus(toStatusID(BuiltinStatusID::Freeze), registry, policy);

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
				pokemon.addStatus(toStatusID(BuiltinStatusID::Paralysis), registry, policy);

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
				.mStats = {
					.mMaxHealth = 150,
					.mAttack = 101,
					.mDefense = 102,
					.mSpAttack = 103,
					.mSpDefense = 104,
					.mSpeed = 105,
				},
				.mStatusIDs = {StatusID{20}, StatusID{21}, StatusID{22}, StatusID{23}, StatusID{24}},
				.mMoveIDs = {MoveID{10}, MoveID{11}, MoveID{12}, MoveID{13}},
				.mMaxPP = {15, 20, 25, 30},
				.mCurrentPP = {5, 10, 15, 20},
				.mTypesIDs = {TypeID{2}, TypeID{3}},
				.mAbilityIDs = {AbilityID{6}},
				.mItemIDs = {ItemID{7}},
				.mNatureIDs = {NatureID{1}},
				.mHealth = 150,
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
					"  Health: 150/210\n"
					"    IV: 0\n"
					"    EV: 0\n"
					"  Attack: 106\n"
					"    IV: 0\n"
					"    EV: 0\n"
					"  Defense: 107\n"
					"    IV: 0\n"
					"    EV: 0\n"
					"  Special Attack: 108\n"
					"    IV: 0\n"
					"    EV: 0\n"
					"  Special Defense: 109\n"
					"    IV: 0\n"
					"    EV: 0\n"
					"  Speed: 110\n"
					"    IV: 0\n"
					"    EV: 0\n"
					"  Type IDs: [2, 3]\n"
					"  Nature IDs: [1]\n"
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

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
