/*! @file battleState.test.cpp
	@brief C++ file for running tests for the BattleState.
	@date 10/21/2026
	@since 0.12.45
	@version 0.12.52
	@author Matthew Moore
*/

#include "Battle/battleState.h"

#include <expected>
#include <sstream>

#include "Ability/abilityID.h"
#include "Configuration/constants.h"
#include "Configuration/statusRegistryConfiguration.h"
#include "Core/typedefs.h"
#include "Interaction/interaction.h"
#include "Interaction/interactionApplicationError.h"
#include "Item/itemID.h"
#include "Move/builtInMoveID.h"
#include "Move/moveID.h"
#include "Nature/natureID.h"
#include "Pokemon/pokemon.h"
#include "Pokemon/pokemon.testHelper.h"
#include "Registry/statusRegistry.h"
#include "Ruleset/rulesetPolicy.h"
#include "Status/builtInStatusID.h"
#include "Status/statusID.h"
#include "Status/statusMeta.h"
#include "Status/volatileStatus.h"
#include "Type/typeID.h"
#include "Validation/Registry/registryError.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Ability::AbilityID;
using PocketCore::Battle::BattleSlot;
using PocketCore::Battle::DamageFormulaModifiers;
using PocketCore::Battle::StatStages;
using PocketCore::Configuration::MAX_VOLATILE_STATUSES_PER_POKEMON;
using PocketCore::Configuration::StatusRegistryConfiguration;
using PocketCore::Core::ub;
using PocketCore::Interaction::InteractionAction;
using PocketCore::Interaction::InteractionApplicationError;
using PocketCore::Item::ItemID;
using PocketCore::Move::BuiltinMoveID;
using PocketCore::Move::MoveID;
using PocketCore::Move::toMoveID;
using PocketCore::Nature::NatureID;
using PocketCore::Pokemon::Pokemon;
using PocketCore::Registry::Status::StatusRegistry;
using PocketCore::Ruleset::RulesetPolicy;
using PocketCore::Status::BuiltinStatusID;
using PocketCore::Status::NO_STATUS_ID;
using PocketCore::Status::StatusClassification;
using PocketCore::Status::StatusID;
using PocketCore::Status::toStatusID;
using PocketCore::Status::VolatileStatus;
using PocketCore::Testing::makePokemon;
using PocketCore::Type::TypeID;
using PocketCore::Validation::Registry::RegistryError;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("StatStages")
{
	WHEN("operator==")
	{
		StatStages stages{};

		GIVEN("two default constructed stages")
		{
			StatStages other{};

			THEN("they are equal")
			{
				CHECK((stages == other));
			}
		}

		GIVEN("for mMaxHealth modified in one stage")
		{
			StatStages other{.mMaxHealth = 2};

			THEN("they are not equal")
			{
				CHECK((stages != other));
			}
		}

		GIVEN("for mAttack modified in one stage")
		{
			StatStages other{.mAttack = 10};

			THEN("they are not equal")
			{
				CHECK((stages != other));
			}
		}

		GIVEN("for mDefense modified in one stage")
		{
			StatStages other{.mDefense = 20};

			THEN("they are not equal")
			{
				CHECK((stages != other));
			}
		}

		GIVEN("for mSpAttack modified in one stage")
		{
			StatStages other{.mSpAttack = 40};

			THEN("they are not equal")
			{
				CHECK((stages != other));
			}
		}

		GIVEN("for mSpDefense modified in one stage")
		{
			StatStages other{.mSpDefense = 50};

			THEN("they are not equal")
			{
				CHECK((stages != other));
			}
		}

		GIVEN("for mSpeed modified in one stage")
		{
			StatStages other{.mSpeed = 60};

			THEN("they are not equal")
			{
				CHECK((stages != other));
			}
		}

		GIVEN("for mAccuracy modified in one stage")
		{
			StatStages other{.mAccuracy = 70};

			THEN("they are not equal")
			{
				CHECK((stages != other));
			}
		}

		GIVEN("for mEvasion modified in one stage")
		{
			StatStages other{.mEvasion = 80};

			THEN("they are not equal")
			{
				CHECK((stages != other));
			}
		}

		GIVEN("two stages modified the same way")
		{
			StatStages other{.mAttack = 5};
			stages.mAttack = 5;

			THEN("they are equal")
			{
				CHECK((stages == other));
			}
		}
	}

	GIVEN("operator<<")
	{
		StatStages stages{.mDefense = 2, .mSpAttack = 1, .mAccuracy = 4, .mEvasion = 3};

		WHEN("the StatStages are written to a stream")
		{
			std::ostringstream output;
			output << stages;

			THEN("all public state is written with numeric identifiers and PP")
			{
				std::string_view expected{
					"Stat Stages {\n"
					"  Attack: 0\n"
					"  Defense: 2\n"
					"  Special Attack: 1\n"
					"  Special Defense: 0\n"
					"  Speed: 0\n"
					"  Accuracy: 4\n"
					"  Evasion: 3\n"
					"}",
				};

				CHECK((output.str() == expected));
			}
		}
	}
}

SCENARIO("DamageFormulaModifiers")
{
	WHEN("operator==")
	{
		DamageFormulaModifiers modifiers{};

		GIVEN("two default constructed damage formula modifiers")
		{
			DamageFormulaModifiers other{};

			THEN("they are equal")
			{
				CHECK((modifiers == other));
			}
		}

		GIVEN("for mHealthModifier modified in one damage formula modifier")
		{
			DamageFormulaModifiers other{.mHealthModifier = 2.0};

			THEN("they are not equal")
			{
				CHECK((modifiers != other));
			}
		}

		GIVEN("for mAttackModifier modified in one damage formula modifier")
		{
			DamageFormulaModifiers other{.mAttackModifier = 3.0};

			THEN("they are not equal")
			{
				CHECK((modifiers != other));
			}
		}

		GIVEN("for mDefenseModifier modified in one damage formula modifier")
		{
			DamageFormulaModifiers other{.mDefenseModifier = 4.0};

			THEN("they are not equal")
			{
				CHECK((modifiers != other));
			}
		}

		GIVEN("for mSpecialAttackModifier modified in one damage formula modifier")
		{
			DamageFormulaModifiers other{.mSpecialAttackModifier = 5.0};

			THEN("they are not equal")
			{
				CHECK((modifiers != other));
			}
		}

		GIVEN("for mSpecialDefenseModifier modified in one damage formula modifier")
		{
			DamageFormulaModifiers other{.mSpecialDefenseModifier = 6.0};

			THEN("they are not equal")
			{
				CHECK((modifiers != other));
			}
		}

		GIVEN("for mSpeedModifier modified in one damage formula modifier")
		{
			DamageFormulaModifiers other{.mSpeedModifier = 7.0};

			THEN("they are not equal")
			{
				CHECK((modifiers != other));
			}
		}

		GIVEN("two stages modified the same way")
		{
			DamageFormulaModifiers other{.mSpecialAttackModifier = 5.0};
			modifiers.mSpecialAttackModifier = 5.0;

			THEN("they are equal")
			{
				CHECK((modifiers == other));
			}
		}
	}

	GIVEN("operator<<")
	{
		DamageFormulaModifiers modifiers{.mHealthModifier = 2.3, .mDefenseModifier = 1.2, .mSpeedModifier = 0.85};

		WHEN("the DamageFormulaModifiers are written to a stream")
		{
			std::ostringstream output;
			output << modifiers;

			THEN("all public state is written with numeric identifiers and PP")
			{
				std::string_view expected{
					"Damage Formula Modifiers {\n"
					"  Health: 2.3\n"
					"  Attack: 1\n"
					"  Defense: 1.2\n"
					"  Special Attack: 1\n"
					"  Special Defense: 1\n"
					"  Speed: 0.85\n"
					"}",
				};

				CHECK((output.str() == expected));
			}
		}
	}
}

SCENARIO("BattleSlot")
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
            	.mTypeIDs = {TypeID{2}, TypeID{3}},
			},
			.mMoveMetas = {
    			{
                    {
                        .mMoveID = MoveID{10},
                        .mPPMaxAmount = 15,
                        .mPPDefaultAmount = 5,
                    },
                    {
                        .mMoveID = MoveID{11},
                        .mPPMaxAmount = 20,
                        .mPPDefaultAmount = 10,
        			},
                    {
                        .mMoveID = MoveID{12},
                        .mPPMaxAmount = 25,
                        .mPPDefaultAmount = 15,
        			},
                    {
                        .mMoveID = MoveID{13},
                        .mPPMaxAmount = 30,
                        .mPPDefaultAmount = 20,
        			},
                },
			},
			.mStatusIDs = {StatusID{20}, StatusID{21}, StatusID{22}, StatusID{23}, StatusID{24}},
			.mAbilityIDs = {AbilityID{6}},
			.mItemIDs = {ItemID{7}},
			.mNatureIDs = {NatureID{1}},
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

	WHEN("operator==")
	{
		BattleSlot battleSlot{};

		GIVEN("two default constructed battleSlot")
		{
			BattleSlot other{};

			THEN("they are equal")
			{
				CHECK((battleSlot == other));
			}
		}

		GIVEN("for mDamageFormulaModifiers modified in one battle slot")
		{
			BattleSlot other{.mDamageFormulaModifiers = {.mAttackModifier = 2.0}};

			THEN("they are not equal")
			{
				CHECK((battleSlot != other));
			}
		}

		GIVEN("for mVolatileStatuses modified in one battle slot")
		{
			BattleSlot other{.mVolatileStatuses = {{{.mStatusID = toStatusID(BuiltinStatusID::Burn)}}}};

			THEN("they are not equal")
			{
				CHECK((battleSlot != other));
			}
		}

		GIVEN("for mPokemon modified in one battle slot")
		{
			Pokemon tempPokemon{makePokemon({})};
			BattleSlot other{.mPokemon = &tempPokemon};

			THEN("they are not equal")
			{
				CHECK((battleSlot != other));
			}
		}

		GIVEN("for mStatStages modified in one battle slot")
		{
			BattleSlot other{.mStatStages = {.mAccuracy = 2}};

			THEN("they are not equal")
			{
				CHECK((battleSlot != other));
			}
		}

		GIVEN("for mChoiceLockedMove modified in one battle slot")
		{
			BattleSlot other{.mChoiceLockedMove = toMoveID(BuiltinMoveID::HydroSteam)};

			THEN("they are not equal")
			{
				CHECK((battleSlot != other));
			}
		}

		GIVEN("for mPosition modified in one battle slot")
		{
			BattleSlot other{.mPosition = 2};

			THEN("they are not equal")
			{
				CHECK((battleSlot != other));
			}
		}

		GIVEN("for mSleepCounter modified in one battle slot")
		{
			BattleSlot other{.mSleepCounter = 3};

			THEN("they are not equal")
			{
				CHECK((battleSlot != other));
			}
		}

		GIVEN("for mToxicCounter modified in one battle slot")
		{
			BattleSlot other{.mToxicCounter = 4};

			THEN("they are not equal")
			{
				CHECK((battleSlot != other));
			}
		}

		GIVEN("for mProtectionCounter modified in one battle slot")
		{
			BattleSlot other{.mProtectionCounter = 5};

			THEN("they are not equal")
			{
				CHECK((battleSlot != other));
			}
		}

		GIVEN("for mIsProtected modified in one battle slot")
		{
			BattleSlot other{.mIsProtected = true};

			THEN("they are not equal")
			{
				CHECK((battleSlot != other));
			}
		}

		GIVEN("for mIsFlinched modified in one battle slot")
		{
			BattleSlot other{.mIsFlinched = true};

			THEN("they are not equal")
			{
				CHECK((battleSlot != other));
			}
		}

		GIVEN("for mIsGrounded modified in one battle slot")
		{
			BattleSlot other{.mIsGrounded = true};

			THEN("they are not equal")
			{
				CHECK((battleSlot != other));
			}
		}

		GIVEN("for mFaintProcessed modified in one battle slot")
		{
			BattleSlot other{.mFaintProcessed = true};

			THEN("they are not equal")
			{
				CHECK((battleSlot != other));
			}
		}

		GIVEN("two stages modified the same way")
		{
			BattleSlot other{.mPosition = 5};
			battleSlot.mPosition = 5;

			THEN("they are equal")
			{
				CHECK((battleSlot == other));
			}
		}
	}

	GIVEN("addVolatileStatus")
	{
		StatusRegistryConfiguration statusConfiguration{};
		const StatusRegistry &registry{statusConfiguration.getRuntimeRegistry()};

		std::array<StatusID, MAX_VOLATILE_STATUSES_PER_POKEMON> statusIDs{};
		RulesetPolicy policy{.mMaxVolatileStatuses = 5, .mReplaceVolatileStatusWhenFull = false};

		WHEN("the active count is zero")
		{
			RulesetPolicy newPolicy{.mMaxVolatileStatuses = 0, .mReplaceVolatileStatusWhenFull = false};
			StatusID volatileStatusID{toStatusID(BuiltinStatusID::Paralysis)};
			std::expected<void, RegistryError> updateResult{
				statusConfiguration.updateStatus(volatileStatusID,
												 {
													 .mName = "Confusion",
													 .mStatusID = volatileStatusID,
													 .mStatusClassification = StatusClassification::Volatile,
												 }),
			};

			REQUIRE(updateResult.has_value());

			const std::expected<void, InteractionApplicationError> result{
				slot.addVolatileStatus(volatileStatusID, registry, newPolicy),
			};

			THEN("the incoming ID is not inserted")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error() == InteractionApplicationError::CapReached));
				CHECK((slot.mVolatileStatuses == volatileStatuses));
			}
		}

		GIVEN("an incoming status classified as non-volatile")
		{
			StatusID nonVolatileStatusID{toStatusID(BuiltinStatusID::Paralysis)};
			std::expected<void, RegistryError> updateResult{
				statusConfiguration.updateStatus(nonVolatileStatusID,
												 {
													 .mName = "Confusion",
													 .mStatusID = nonVolatileStatusID,
													 .mStatusClassification = StatusClassification::NonVolatile,
												 }),
			};

			REQUIRE(updateResult.has_value());

			statusIDs.at(0) = toStatusID(BuiltinStatusID::Burn);
			std::ranges::transform(statusIDs, slot.mVolatileStatuses.begin(),
								   [](const StatusID volatileStatusID) { return VolatileStatus{volatileStatusID}; });

			WHEN("the non-volatile status is applied to volatile storage")
			{
				const std::expected<void, InteractionApplicationError> result{
					slot.addVolatileStatus(nonVolatileStatusID, registry, policy),
				};

				THEN("the volatile status array is left unchanged")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error() == InteractionApplicationError::WrongClassification));

					for (ub i{0}; i < MAX_VOLATILE_STATUSES_PER_POKEMON; ++i)
					{
						CHECK((slot.mVolatileStatuses.at(i).mStatusID == statusIDs.at(i)));
					}
				}
			}
		}

		GIVEN("the incoming ID is the same as the empty ID")
		{
			statusIDs.at(0) = toStatusID(BuiltinStatusID::Burn);
			std::ranges::transform(statusIDs, slot.mVolatileStatuses.begin(),
								   [](const StatusID volatileStatusID) { return VolatileStatus{volatileStatusID}; });

			StatusID volatileStatusID{NO_STATUS_ID};
			std::expected<void, RegistryError> updateResult{
				statusConfiguration.updateStatus(volatileStatusID,
												 {
													 .mName = "Confusion",
													 .mStatusID = volatileStatusID,
													 .mStatusClassification = StatusClassification::Volatile,
												 }),
			};

			REQUIRE(updateResult.has_value());

			WHEN("the id is applied")
			{
				const std::expected<void, InteractionApplicationError> result{
					slot.addVolatileStatus(volatileStatusID, registry, policy),
				};

				THEN("the non-volatile status array is left unchanged")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error() == InteractionApplicationError::SameAsEmptyID));

					for (ub i{0}; i < MAX_VOLATILE_STATUSES_PER_POKEMON; ++i)
					{
						CHECK((slot.mVolatileStatuses.at(i).mStatusID == statusIDs.at(i)));
					}
				}
			}
		}

		GIVEN("the incoming ID is a duplicate ID")
		{
			statusIDs.at(0) = toStatusID(BuiltinStatusID::Burn);
			std::ranges::transform(statusIDs, slot.mVolatileStatuses.begin(),
								   [](const StatusID volatileStatusID) { return VolatileStatus{volatileStatusID}; });

			StatusID volatileStatusID{toStatusID(BuiltinStatusID::Burn)};
			std::expected<void, RegistryError> updateResult{
				statusConfiguration.updateStatus(volatileStatusID,
												 {
													 .mName = "Confusion",
													 .mStatusID = volatileStatusID,
													 .mStatusClassification = StatusClassification::Volatile,
												 }),
			};

			REQUIRE(updateResult.has_value());

			WHEN("the id is applied")
			{
				const std::expected<void, InteractionApplicationError> result{
					slot.addVolatileStatus(volatileStatusID, registry, policy),
				};

				THEN("the non-volatile status array is left unchanged")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error() == InteractionApplicationError::Duplicate));

					for (ub i{0}; i < MAX_VOLATILE_STATUSES_PER_POKEMON; ++i)
					{
						CHECK((slot.mVolatileStatuses.at(i).mStatusID == statusIDs.at(i)));
					}
				}
			}
		}

		GIVEN("an incoming status with no registered metadata")
		{
			const StatusID unregisteredStatusID{900};

			statusIDs.at(0) = toStatusID(BuiltinStatusID::Burn);
			std::ranges::transform(statusIDs, slot.mVolatileStatuses.begin(),
								   [](const StatusID volatileStatusID) { return VolatileStatus{volatileStatusID}; });

			WHEN("the unregistered status is applied")
			{
				const std::expected<void, InteractionApplicationError> result{
					slot.addVolatileStatus(unregisteredStatusID, registry, policy),
				};

				THEN("the volatile classification guard is skipped and the volatile array is unchanged")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error() == InteractionApplicationError::MissingMetadata));

					for (ub i{0}; i < MAX_VOLATILE_STATUSES_PER_POKEMON; ++i)
					{
						CHECK((slot.mVolatileStatuses.at(i).mStatusID == statusIDs.at(i)));
					}
				}
			}
		}

		GIVEN("a current status that blocks the incoming status")
		{
			statusIDs.at(0) = toStatusID(BuiltinStatusID::Freeze);
			statusIDs.at(1) = toStatusID(BuiltinStatusID::Poison);
			std::ranges::transform(statusIDs, slot.mVolatileStatuses.begin(),
								   [](const StatusID volatileStatusID) { return VolatileStatus{volatileStatusID}; });

			StatusID volatileStatusID{toStatusID(BuiltinStatusID::Toxic)};
			std::expected<void, RegistryError> updateResult{
				statusConfiguration.updateStatus(volatileStatusID,
												 {
													 .mName = "Confusion",
													 .mStatusInteractions
														= {{.mExistingID = toStatusID(BuiltinStatusID::Freeze), .mAction = InteractionAction::BlockIncoming},
														   {.mExistingID = toStatusID(BuiltinStatusID::Poison), .mAction = InteractionAction::ReplaceCurrent},},
													 .mStatusID = volatileStatusID,
													 .mStatusClassification = StatusClassification::Volatile,
												 }),
			};

			REQUIRE(updateResult.has_value());

			WHEN("the blocked status would otherwise replace another current status")
			{
				const std::expected<void, InteractionApplicationError> result{
					slot.addVolatileStatus(volatileStatusID, registry, policy),
				};

				THEN("the incoming status is rejected before any current status changes")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error() == InteractionApplicationError::Blocked));

					for (ub i{0}; i < MAX_VOLATILE_STATUSES_PER_POKEMON; ++i)
					{
						CHECK((slot.mVolatileStatuses.at(i).mStatusID == statusIDs.at(i)));
					}
				}
			}
		}

		GIVEN("a current status that the incoming status replaces")
		{
			statusIDs.at(0) = toStatusID(BuiltinStatusID::Poison);
			std::ranges::transform(statusIDs, slot.mVolatileStatuses.begin(),
								   [](const StatusID volatileStatusID) { return VolatileStatus{volatileStatusID}; });

			StatusID volatileStatusID{toStatusID(BuiltinStatusID::Toxic)};
			std::expected<void, RegistryError> updateResult{
				statusConfiguration.updateStatus(volatileStatusID,
												 {
													 .mName = "Confusion",
													 .mStatusInteractions = {{.mExistingID = toStatusID(BuiltinStatusID::Poison),
																			  .mAction = InteractionAction::ReplaceCurrent,},},
													 .mStatusID = volatileStatusID,
													 .mStatusClassification = StatusClassification::Volatile,
												 }),
			};
			REQUIRE(updateResult.has_value());

			WHEN("the incoming status is applied")
			{
				const std::expected<void, InteractionApplicationError> result{
					slot.addVolatileStatus(volatileStatusID, registry, policy),
				};

				THEN("the incoming status occupies the replaced status slot")
				{
					REQUIRE(result.has_value());
					CHECK((slot.mVolatileStatuses.at(0).mStatusID == toStatusID(BuiltinStatusID::Toxic)));
					CHECK((slot.mVolatileStatuses.at(1).mStatusID == NO_STATUS_ID));
				}
			}
		}

		GIVEN("several current statuses that the incoming status replaces")
		{
			StatusID incomingStatusID{toStatusID(BuiltinStatusID::Toxic)};
			std::expected<void, RegistryError> updateResult{statusConfiguration.updateStatus(incomingStatusID, {
				.mName = "Toxic",
			    .mStatusInteractions = {
					{.mExistingID = toStatusID(BuiltinStatusID::Burn), .mAction = InteractionAction::ReplaceCurrent},
					{.mExistingID = toStatusID(BuiltinStatusID::Poison), .mAction = InteractionAction::ReplaceCurrent},
				},
				.mStatusClassification = StatusClassification::Volatile,
			}),};
			REQUIRE(updateResult.has_value());

			statusIDs.at(0) = toStatusID(BuiltinStatusID::Burn);
			statusIDs.at(1) = toStatusID(BuiltinStatusID::Sleep);
			statusIDs.at(2) = toStatusID(BuiltinStatusID::Poison);
			statusIDs.at(3) = toStatusID(BuiltinStatusID::Paralysis);

			std::ranges::transform(statusIDs, slot.mVolatileStatuses.begin(),
								   [](const StatusID volatileStatusID) { return VolatileStatus{volatileStatusID}; });

			WHEN("the incoming status is applied")
			{
				const std::expected<void, InteractionApplicationError> result{
					slot.addVolatileStatus(incomingStatusID, registry, policy),
				};

				THEN("only the first replacement slot receives the incoming status")
				{
					REQUIRE(result.has_value());

					std::array<StatusID, MAX_VOLATILE_STATUSES_PER_POKEMON> expectedStatusIDs{
						incomingStatusID,
						toStatusID(BuiltinStatusID::Sleep),
						toStatusID(BuiltinStatusID::Paralysis),
					};

					for (ub i{0}; i < MAX_VOLATILE_STATUSES_PER_POKEMON; ++i)
					{
						CHECK((slot.mVolatileStatuses.at(i).mStatusID == expectedStatusIDs.at(i)));
					}
				}
			}
		}

		GIVEN("several current statuses that the incoming status removes")
		{
			statusIDs.at(0) = toStatusID(BuiltinStatusID::Burn);
			statusIDs.at(1) = toStatusID(BuiltinStatusID::Sleep);
			statusIDs.at(2) = toStatusID(BuiltinStatusID::Paralysis);
			statusIDs.at(3) = toStatusID(BuiltinStatusID::Toxic);
			std::ranges::transform(statusIDs, slot.mVolatileStatuses.begin(),
								   [](const StatusID volatileStatusID) { return VolatileStatus{volatileStatusID}; });

			StatusID volatileStatusID{toStatusID(BuiltinStatusID::Freeze)};
			std::expected<void, RegistryError> updateResult{
				statusConfiguration.updateStatus(volatileStatusID,
												 {
													 .mName = "Confusion",
													 .mStatusInteractions
														= {{.mExistingID = toStatusID(BuiltinStatusID::Burn), .mAction = InteractionAction::RemoveCurrent},
														   {.mExistingID = toStatusID(BuiltinStatusID::Sleep), .mAction = InteractionAction::RemoveCurrent},
														   {.mExistingID = toStatusID(BuiltinStatusID::Paralysis), .mAction = InteractionAction::RemoveCurrent},
														},
													 .mStatusID = volatileStatusID,
													 .mStatusClassification = StatusClassification::Volatile,
												 }),
			};
			REQUIRE(updateResult.has_value());

			WHEN("the incoming status is applied")
			{
				const std::expected<void, InteractionApplicationError> result{
					slot.addVolatileStatus(volatileStatusID, registry, policy),
				};

				THEN("the remaining statuses shift down and the incoming status uses the first empty slot")
				{
					REQUIRE(result.has_value());
					CHECK((slot.mVolatileStatuses.at(0).mStatusID == toStatusID(BuiltinStatusID::Toxic)));
					CHECK((slot.mVolatileStatuses.at(1).mStatusID == toStatusID(BuiltinStatusID::Freeze)));
					CHECK((slot.mVolatileStatuses.at(2).mStatusID == NO_STATUS_ID));
					CHECK((slot.mVolatileStatuses.at(3).mStatusID == NO_STATUS_ID));
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
			std::ranges::transform(statusIDs, slot.mVolatileStatuses.begin(),
								   [](const StatusID volatileStatusID) { return VolatileStatus{volatileStatusID}; });

			StatusID volatileStatusID{toStatusID(BuiltinStatusID::Paralysis)};
			std::expected<void, RegistryError> updateResult{
				statusConfiguration.updateStatus(volatileStatusID,
												 {
													 .mName = "Confusion",
													 .mStatusInteractions = {{
														 .mExistingID = toStatusID(BuiltinStatusID::Freeze),
														 .mAction = InteractionAction::BlockIncoming,
													 },},
													 .mStatusID = volatileStatusID,
													 .mStatusClassification = StatusClassification::Volatile,
												 }),
			};
			REQUIRE(updateResult.has_value());

			WHEN("another coexisting status is applied")
			{
				const std::expected<void, InteractionApplicationError> result{
					slot.addVolatileStatus(toStatusID(BuiltinStatusID::Paralysis), registry, policy),
				};

				THEN("the full status array remains unchanged")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error() == InteractionApplicationError::CapReached));
					for (ub i{0}; i < MAX_VOLATILE_STATUSES_PER_POKEMON; ++i)
					{
						CHECK((slot.mVolatileStatuses.at(i).mStatusID == statusIDs.at(i)));
					}
				}
			}
		}
	}

	GIVEN("operator<<")
	{
		WHEN("the pokemon is a nullptr")
		{
			std::ostringstream output;
			BattleSlot newSlot{.mPokemon = nullptr};

			output << newSlot;

			THEN("nothing is returned")
			{
				std::string_view expected{};

				CHECK((output.str() == expected));
			}
		}

		WHEN("the BattleSlot are written to a stream")
		{
			std::ostringstream output;
			output << slot;

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
					"  Volatile Status IDs: [7, 8, 0, 0, 0, 0, 0, 0]\n"
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
					"    Type IDs: [2, 3]\n"
					"    Nature IDs: [1]\n"
					"    Ability IDs: [6]\n"
					"    Item IDs: [7]\n"
					"    Non-Volatile Status IDs: [20, 21, 22, 23, 24]\n"
					"    Moves:\n"
					"      [0] ID: 10, PP: 5/15\n"
					"      [1] ID: 11, PP: 10/20\n"
					"      [2] ID: 12, PP: 15/25\n"
					"      [3] ID: 13, PP: 20/30\n"
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

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
