/*! @file moveRegistryConfiguration.test.cpp
	@brief C++ file for running tests for the MoveRegistryConfiguration.
	@date 09/10/2026
	@since 0.8.7
	@version 0.12.20
	@author Matthew Moore
*/

#include "Configuration/moveRegistryConfiguration.h"

#include <expected>
#include <format>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "Battle/battleTargetsAndTriggers.h"
#include "Configuration/constants.h"
#include "Core/typedefs.h"
#include "Effect/builtInEffectID.h"
#include "Effect/effectTrigger.h"
#include "Move/builtInMoveID.h"
#include "Move/constants.h"
#include "Move/moveID.h"
#include "Move/moveMeta.h"
#include "Registry/moveRegistry.h"
#include "Registry/registryError.h"
#include "Utility/Debug/Logging/logging.testHelper.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Battle::BattleEventID;
using PocketCore::Battle::BattleEventRole;
using PocketCore::Battle::BattleTargetID;
using PocketCore::Configuration::MAX_MOVES;
using PocketCore::Configuration::MoveRegistryConfiguration;
using PocketCore::Configuration::RegistryError;
using PocketCore::Core::ub;
using PocketCore::Core::us;
using PocketCore::Effect::BuiltinEffectID;
using PocketCore::Effect::EffectTrigger;
using PocketCore::Effect::toEffectID;
using PocketCore::Move::BuiltinMoveID;
using PocketCore::Move::MOVE_NAME_FACADE;
using PocketCore::Move::MOVE_NAME_NONE;
using PocketCore::Move::MoveID;
using PocketCore::Move::MoveMeta;
using PocketCore::Move::NO_MOVE_ID;
using PocketCore::Move::toMoveID;
using PocketCore::Registry::Move::MoveRegistry;
using PocketCore::Registry::RegistryErrorInfo;
using PocketCore::Testing::ensureLoggerInitialized;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("MoveRegistryConfiguration")
{
	ensureLoggerInitialized("move registry configuration test", "moveRegistryConfiguration_test.log");

	MoveRegistryConfiguration config{};
	MoveRegistry registry{};
	ub finalMoveUnderlyingValue{std::to_underlying(BuiltinMoveID::FinalMove)};

	GIVEN("getAmountRegistered")
	{
		THEN("the default registry returns the expected amount of built-in moves")
		{
			CHECK((config.getAmountRegistered() == finalMoveUnderlyingValue));
		}
	}

	GIVEN("getRuntimeRegistry")
	{
		THEN("the runtime registry is the same as the default registry")
		{
			CHECK((config.getRuntimeRegistry() == registry));
		}
	}

	GIVEN("getMoveMetadata")
	{
		THEN("unknown IDs are absent")
		{
			CHECK((config.getMoveMetadata(MoveID{200}) == nullptr));
		}

		THEN("the metadata is retrieved when accessed by a valid Move ID")
		{
			MoveMeta expected{
				.mName = std::string(MOVE_NAME_NONE),
				.mTriggers = {},
				.mMoveID = toMoveID(BuiltinMoveID::None),
			};

			CHECK((expected == *config.getMoveMetadata(NO_MOVE_ID)));
		}
	}

	GIVEN("getMoveID")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(config.getMoveID("Unknown").has_value());
		}

		THEN("the Move ID is retrieved by valid Move name")
		{
			std::optional<MoveID> moveID{config.getMoveID(MOVE_NAME_NONE)};

			REQUIRE(moveID.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((moveID.value() == toMoveID(BuiltinMoveID::None)));
		}
	}

	GIVEN("getMoveName")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(config.getMoveName(MoveID{200}).has_value());
		}

		THEN("a registered move name is returned by stable ID")
		{
			std::optional<std::string_view> moveName{config.getMoveName(toMoveID(BuiltinMoveID::None))};

			REQUIRE(moveName.has_value());

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((moveName.value() == MOVE_NAME_NONE));
		}
	}

	GIVEN("getRegisteredMoves")
	{
		THEN("the amount of moves returned matches the amount that are built-in")
		{
			CHECK((config.getRegisteredMoves().size() == finalMoveUnderlyingValue));
		}
	}

	GIVEN("hasMove")
	{
		WHEN("calling the string_view overload")
		{
			THEN("an unknown move name has no entry")
			{
				CHECK_FALSE(config.hasMove("Unknown"));
			}

			THEN("a known move name has an entry")
			{
				CHECK(config.hasMove(MOVE_NAME_NONE));
			}
		}

		WHEN("calling the MoveID overload")
		{
			THEN("an unknown move ID has no entry")
			{
				CHECK_FALSE(config.hasMove(MoveID{200}));
			}

			THEN("a known move ID has an entry")
			{
				CHECK(config.hasMove(NO_MOVE_ID));
			}
		}
	}

	GIVEN("addMove")
	{
		WHEN("trying to add an move past the capacity")
		{
			us newMoveCount{finalMoveUnderlyingValue};

			for (us i{0}; i < MAX_MOVES - finalMoveUnderlyingValue; ++i)
			{
				std::string name{std::format("String_{:04}", i)};
				MoveMeta definition{.mName = name, .mTriggers = {}};
				std::expected<MoveID, RegistryErrorInfo> result{config.addMove(definition)};

				REQUIRE(result.has_value());

				MoveID assignedID{result.value()};
				CHECK((assignedID.getValue() == newMoveCount++));
			}

			std::string name{std::format("String_{:04}", MAX_MOVES + 1)};

			MoveMeta definition{.mName = name, .mTriggers = {}};
			std::expected<MoveID, RegistryErrorInfo> result{config.addMove(definition)};

			THEN("registration reports a max capacity and nothing is added")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::MaxCapacity));
				CHECK((config.getAmountRegistered() == MAX_MOVES));
			}
		}

		WHEN("an move whose name is already in use is added")
		{
			MoveMeta definition{.mName = std::string(MOVE_NAME_FACADE), .mTriggers = {}};
			std::expected<MoveID, RegistryErrorInfo> result{config.addMove(definition)};

			THEN("registration reports a duplicate move and nothing is added")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::DuplicateMove));
				CHECK((config.getAmountRegistered() == finalMoveUnderlyingValue));
			}
		}

		WHEN("a unique move definition is added")
		{
			std::vector<EffectTrigger> triggers{
				{
					.mEffects = {toEffectID(BuiltinEffectID::Recoil), toEffectID(BuiltinEffectID::StatusTick)},
					.mTrigger = BattleEventID::TurnEnd,
				},
			};

			MoveMeta definition{.mName = "TestMoveName", .mTriggers = triggers};

			std::expected<MoveID, RegistryErrorInfo> result{config.addMove(definition)};

			THEN("it receives the first custom stable ID and owns its trigger data")
			{
				REQUIRE(result.has_value());
				MoveID assignedID{result.value()};
				CHECK((assignedID.getValue() == finalMoveUnderlyingValue));

				triggers.clear();

				const MoveMeta *metadata{config.getMoveMetadata(assignedID)};

				REQUIRE((metadata != nullptr));
				CHECK((metadata->mName == "TestMoveName"));
				REQUIRE((metadata->mTriggers.size() == 1U));
				CHECK((metadata->mTriggers.front().mEffects.size() == 2U));
			}
		}
	}

	GIVEN("addMoves")
	{
		WHEN("trying to add an move past the capacity")
		{
			std::vector<MoveMeta> moveMetas;

			for (us i{0}; i < MAX_MOVES - finalMoveUnderlyingValue; ++i)
			{
				std::string name{std::format("String_{:04}", i)};
				moveMetas.push_back({.mName = name, .mTriggers = {}});
			}

			std::string name{std::format("String_{:04}", MAX_MOVES + 1)};

			moveMetas.push_back({.mName = name, .mTriggers = {}});
			std::expected<void, RegistryErrorInfo> result{config.addMoves(moveMetas)};

			THEN("registration reports a max capacity and nothing is added")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::MaxCapacity));
				CHECK((config.getAmountRegistered() == finalMoveUnderlyingValue));
			}
		}

		WHEN("an move whose name is already in use is added")
		{
			std::vector<MoveMeta> moveMetas{};

			for (us i{0}; i < 20; ++i)
			{
				std::string name{std::format("String_{:04}", i)};
				moveMetas.push_back({.mName = name, .mTriggers = {}});
			}

			moveMetas.push_back({.mName = std::string(MOVE_NAME_FACADE), .mTriggers = {}});
			std::expected<void, RegistryErrorInfo> result{config.addMoves(moveMetas)};

			THEN("registration reports a duplicate move and the registry is rollback to the checkpoint before the erroneous addition")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::DuplicateMove));
				CHECK((config.getAmountRegistered() == finalMoveUnderlyingValue));
			}
		}

		WHEN("a unique move definition is added")
		{
			std::vector<MoveMeta> moveMetas{};
			std::vector<EffectTrigger> triggers{
				{
					.mEffects = {toEffectID(BuiltinEffectID::Recoil), toEffectID(BuiltinEffectID::StatusTick)},
					.mTrigger = BattleEventID::TurnEnd,
				},
			};

			moveMetas.push_back({.mName = "TestMoveName", .mTriggers = triggers});

			std::expected<void, RegistryErrorInfo> result{config.addMoves(moveMetas)};

			THEN("the registry reports no error and all the move definitions are added")
			{
				REQUIRE(result.has_value());
				CHECK((config.getAmountRegistered() == finalMoveUnderlyingValue + 1));
			}
		}
	}

	GIVEN("setMoveTriggers")
	{
		WHEN("calling the string_view overload")
		{
			WHEN("calling with an invalid move name")
			{
				std::vector<EffectTrigger> triggers{
					{
						.mEffects = {toEffectID(BuiltinEffectID::Recoil), toEffectID(BuiltinEffectID::StatusTick)},
						.mTrigger = BattleEventID::TurnEnd,
					},
				};

				std::expected<void, RegistryErrorInfo> result{config.setMoveTriggers("ThisIsInvalid", triggers)};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::MoveNotFound));
					CHECK((config.getAmountRegistered() == finalMoveUnderlyingValue));
				}
			}

			WHEN("updating an existing move definition")
			{
				std::vector<EffectTrigger> triggers{
					{
						.mEffects = {toEffectID(BuiltinEffectID::Recoil), toEffectID(BuiltinEffectID::StatusTick)},
						.mTrigger = BattleEventID::TurnEnd,
					},
				};

				std::expected<void, RegistryErrorInfo> result{config.setMoveTriggers(MOVE_NAME_NONE, triggers)};

				THEN("the registry reports no error and the triggers are appropriately updated")
				{
					REQUIRE(result.has_value());
					CHECK((config.getAmountRegistered() == finalMoveUnderlyingValue));
					CHECK((config.getMoveMetadata(toMoveID(BuiltinMoveID::None))->mTriggers == triggers));
				}
			}
		}

		WHEN("calling the MoveID overload")
		{
			WHEN("calling with an invalid move name")
			{
				std::vector<EffectTrigger> triggers{
					{
						.mEffects = {toEffectID(BuiltinEffectID::Recoil), toEffectID(BuiltinEffectID::StatusTick)},
						.mTrigger = BattleEventID::TurnEnd,
					},
				};

				std::expected<void, RegistryErrorInfo> result{config.setMoveTriggers(MoveID{200}, triggers)};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::MoveNotFound));
					CHECK((config.getAmountRegistered() == finalMoveUnderlyingValue));
				}
			}

			WHEN("updating an existing move definition")
			{
				std::vector<EffectTrigger> triggers{
					{
						.mEffects = {toEffectID(BuiltinEffectID::Recoil), toEffectID(BuiltinEffectID::StatusTick)},
						.mTrigger = BattleEventID::TurnEnd,
					},
				};

				std::expected<void, RegistryErrorInfo> result{config.setMoveTriggers(toMoveID(BuiltinMoveID::None), triggers)};

				THEN("the registry reports no error and the triggers are appropriately updated")
				{
					REQUIRE(result.has_value());
					CHECK((config.getAmountRegistered() == finalMoveUnderlyingValue));
					CHECK((config.getMoveMetadata(toMoveID(BuiltinMoveID::None))->mTriggers == triggers));
				}
			}
		}
	}

	GIVEN("setMoveTarget")
	{
		WHEN("calling the string_view overload")
		{
			WHEN("calling with an invalid move name")
			{
				std::expected<void, RegistryErrorInfo> result{config.setMoveTarget("ThisIsInvalid", BattleTargetID::AllAllies)};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::MoveNotFound));
					CHECK((config.getAmountRegistered() == finalMoveUnderlyingValue));
				}
			}

			WHEN("updating an existing move definition")
			{
				std::expected<void, RegistryErrorInfo> result{config.setMoveTarget(MOVE_NAME_NONE, BattleTargetID::AllAllies)};

				THEN("the registry reports no error and the target is appropriately updated")
				{
					REQUIRE(result.has_value());
					CHECK((config.getAmountRegistered() == finalMoveUnderlyingValue));
					CHECK((config.getMoveMetadata(toMoveID(BuiltinMoveID::None))->mTargetID == BattleTargetID::AllAllies));
				}
			}
		}

		WHEN("calling the MoveID overload")
		{
			WHEN("calling with an invalid move name")
			{
				std::expected<void, RegistryErrorInfo> result{config.setMoveTarget(MoveID{200}, BattleTargetID::AllAllies)};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::MoveNotFound));
					CHECK((config.getAmountRegistered() == finalMoveUnderlyingValue));
				}
			}

			WHEN("updating an existing move definition")
			{
				std::expected<void, RegistryErrorInfo> result{
					config.setMoveTarget(toMoveID(BuiltinMoveID::None), BattleTargetID::AllAllies),
				};

				THEN("the registry reports no error and the target is appropriately updated")
				{
					REQUIRE(result.has_value());
					CHECK((config.getAmountRegistered() == finalMoveUnderlyingValue));
					CHECK((config.getMoveMetadata(toMoveID(BuiltinMoveID::None))->mTargetID == BattleTargetID::AllAllies));
				}
			}
		}
	}

	GIVEN("renameMove")
	{
		WHEN("calling with an invalid move name")
		{
			std::expected<void, RegistryErrorInfo> result{config.renameMove("ThisIsInvalid", "NewName")};

			THEN("the registry reports an error and there is no update to the registry")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::MoveNotFound));
				CHECK((config.getAmountRegistered() == finalMoveUnderlyingValue));
			}
		}

		WHEN("trying to rename to a name already in the registry")
		{
			std::expected<void, RegistryErrorInfo> result{config.renameMove(MOVE_NAME_NONE, MOVE_NAME_FACADE)};

			THEN("the registry reports an error and there is no update to the registry")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::DuplicateMove));
				CHECK((config.getAmountRegistered() == finalMoveUnderlyingValue));
			}
		}

		WHEN("updating an existing move definition")
		{
			std::expected<void, RegistryErrorInfo> result{config.renameMove(MOVE_NAME_NONE, "NewName")};

			THEN("the registry reports no error and the name is appropriately updated")
			{
				REQUIRE(result.has_value());
				CHECK((config.getAmountRegistered() == finalMoveUnderlyingValue));
				CHECK((config.getMoveMetadata(toMoveID(BuiltinMoveID::None))->mName == "NewName"));
			}
		}
	}

	GIVEN("updateMove")
	{
		std::vector<EffectTrigger> triggers{
			{
				.mEffects = {toEffectID(BuiltinEffectID::Recoil), toEffectID(BuiltinEffectID::StatusTick)},
				.mTrigger = BattleEventID::TurnEnd,
			},
		};

		MoveMeta definition{.mName = "TestMoveName", .mTriggers = triggers};

		WHEN("calling the string_view overload")
		{
			WHEN("calling with an invalid move name")
			{
				std::expected<void, RegistryErrorInfo> result{config.updateMove("ThisIsInvalid", definition)};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::MoveNotFound));
					CHECK((config.getAmountRegistered() == finalMoveUnderlyingValue));
				}
			}

			WHEN("trying to rename to a name already in the registry")
			{
				std::expected<void, RegistryErrorInfo> result{
					config.updateMove(MOVE_NAME_NONE, {.mName = std::string(MOVE_NAME_FACADE), .mTriggers = triggers}),
				};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::DuplicateMove));
					CHECK((config.getAmountRegistered() == finalMoveUnderlyingValue));
				}
			}

			WHEN("updating an existing move definition")
			{
				std::expected<void, RegistryErrorInfo> result{config.updateMove(MOVE_NAME_NONE, definition)};

				THEN("the registry reports no error and the target is appropriately updated")
				{
					REQUIRE(result.has_value());
					CHECK((config.getAmountRegistered() == finalMoveUnderlyingValue));
					CHECK((config.getMoveMetadata(toMoveID(BuiltinMoveID::None))->mName == "TestMoveName"));
					CHECK((config.getMoveMetadata(toMoveID(BuiltinMoveID::None))->mTriggers == triggers));
				}
			}
		}

		WHEN("calling the MoveID overload")
		{
			WHEN("calling with an invalid move name")
			{
				std::expected<void, RegistryErrorInfo> result{config.updateMove(MoveID{200}, definition)};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::MoveNotFound));
					CHECK((config.getAmountRegistered() == finalMoveUnderlyingValue));
				}
			}

			WHEN("trying to rename to a name already in the registry")
			{
				std::expected<void, RegistryErrorInfo> result{
					config.updateMove(toMoveID(BuiltinMoveID::None), {.mName = std::string(MOVE_NAME_FACADE), .mTriggers = triggers}),
				};

				THEN("the registry reports an error and there is no update to the registry")
				{
					REQUIRE_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::DuplicateMove));
					CHECK((config.getAmountRegistered() == finalMoveUnderlyingValue));
				}
			}

			WHEN("updating an existing move definition")
			{
				std::expected<void, RegistryErrorInfo> result{config.updateMove(toMoveID(BuiltinMoveID::None), definition)};

				THEN("the registry reports no error and the target is appropriately updated")
				{
					REQUIRE(result.has_value());
					CHECK((config.getAmountRegistered() == finalMoveUnderlyingValue));
					CHECK((config.getMoveMetadata(toMoveID(BuiltinMoveID::None))->mName == "TestMoveName"));
					CHECK((config.getMoveMetadata(toMoveID(BuiltinMoveID::None))->mTriggers == triggers));
				}
			}
		}
	}

	GIVEN("removeMove")
	{
		WHEN("calling the string_view overload")
		{
			WHEN("calling with an unknown move name")
			{
				std::expected<MoveID, RegistryErrorInfo> result{config.removeMove("Unknown")};

				THEN("the result is an error and the registry is not updated")
				{
					CHECK_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::MoveNotFound));
					CHECK((config.getAmountRegistered() == finalMoveUnderlyingValue));
				}
			}

			WHEN("calling with a known move name")
			{
				std::expected<MoveID, RegistryErrorInfo> result{config.removeMove(MOVE_NAME_NONE)};

				THEN("the result is a success and the registry is updated")
				{
					CHECK(result.has_value());
					CHECK((result.value() == toMoveID(BuiltinMoveID::None)));
					CHECK((config.getAmountRegistered() == finalMoveUnderlyingValue - 1));
				}
			}
		}

		WHEN("calling the MoveID overload")
		{
			WHEN("calling with an unknown move ID")
			{
				std::expected<MoveID, RegistryErrorInfo> result{config.removeMove(MoveID{200})};

				THEN("the result is an error and the registry is not updated")
				{
					CHECK_FALSE(result.has_value());
					CHECK((result.error().mKind == RegistryError::MoveNotFound));
					CHECK((config.getAmountRegistered() == finalMoveUnderlyingValue));
				}
			}

			WHEN("calling with a known move ID")
			{
				std::expected<MoveID, RegistryErrorInfo> result{config.removeMove(toMoveID(BuiltinMoveID::None))};

				THEN("the result is a success and the registry is updated")
				{
					CHECK(result.has_value());
					CHECK((result.value() == toMoveID(BuiltinMoveID::None)));
					CHECK((config.getAmountRegistered() == finalMoveUnderlyingValue - 1));
				}
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
