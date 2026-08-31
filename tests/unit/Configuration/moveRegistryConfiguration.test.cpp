/*! @file moveRegistryConfiguration.test.cpp
	@brief C++ file for running tests for the MoveRegistryConfiguration.
	@date 08/26/2026
	@since 0.8.7
	@version 0.12.6
	@author Matthew Moore
*/

#include "Configuration/moveRegistryConfiguration.h"

#include <algorithm>
#include <array>
#include <optional>
#include <string_view>
#include <vector>

#include "Battle/battleTargetsAndTriggers.h"
#include "Effect/builtInEffectID.h"
#include "Effect/effectTrigger.h"
#include "Move/moveID.h"
#include "Move/moveMeta.h"
#include "Utility/Debug/Logging/logger.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Battle::BattleEventID;
using PocketCore::Battle::BattleEventRole;
using PocketCore::Battle::BattleTargetID;
using PocketCore::Configuration::MoveRegistryConfiguration;
using PocketCore::Configuration::RegistryError;
using PocketCore::Effect::BuiltinEffectID;
using PocketCore::Effect::EffectTrigger;
using PocketCore::Effect::toEffectID;
using PocketCore::Move::MoveID;
using PocketCore::Move::MoveMeta;
using PocketCore::Utility::Debug::Logging::Logger;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity,llvm-prefer-static-over-anonymous-namespace)

namespace
{
	void ensureMoveLoggerInitialized()
	{
		static bool initialized{false};

		if (!initialized)
		{
			initialized = Logger::initialize("mrc_test", "moveRegistryConfiguration_test.log", true);
		}
	}

	MoveMeta makeMove(const std::string_view &name)
	{
		return MoveMeta{
			.mTriggers = {{
				.mEffects = {toEffectID(BuiltinEffectID::AccuracyCheck), toEffectID(BuiltinEffectID::BaseDamage)},
				.mTrigger = BattleEventID::Hit,
			},},
			.mName = name,
			.mTargetID = BattleTargetID::SingleOpponent,
		};
	}
} // namespace

SCENARIO("MoveRegistryConfiguration addMove")
{
	ensureMoveLoggerInitialized();
	MoveRegistryConfiguration configuration{};

	GIVEN("a unique move definition")
	{
		std::vector<EffectTrigger> triggers{
			{
				.mEffects = {toEffectID(BuiltinEffectID::AccuracyCheck), toEffectID(BuiltinEffectID::BaseDamage)},
				.mTrigger = BattleEventID::Hit,
			},
		};
		MoveMeta definition{.mTriggers = triggers, .mName = "Custom Jab", .mTargetID = BattleTargetID::SingleOpponent};

		WHEN("the move is added")
		{
			auto result{configuration.addMove(definition)};

			THEN("it receives the first custom stable ID and owns trigger data")
			{
				REQUIRE(result.has_value());
				MoveID assignedIdentifier{result.value()};
				CHECK((assignedIdentifier.getValue() == 5U));

				triggers.clear();
				const MoveMeta *metadata{configuration.getMoveMetadata(assignedIdentifier)};
				REQUIRE((metadata != nullptr));
				CHECK((metadata->mName == "Custom Jab"));
				REQUIRE((metadata->mTriggers.size() == 1U));
				CHECK((metadata->mTriggers.front().mEffects.size() == 2U));
			}
		}
	}

	GIVEN("a move whose name is already registered")
	{
		MoveMeta definition{.mTriggers = {}, .mName = "Pound", .mTargetID = BattleTargetID::SingleOpponent};

		THEN("registration reports DuplicateMove")
		{
			auto result{configuration.addMove(definition)};
			REQUIRE_FALSE(result.has_value());
			CHECK((result.error().mKind == RegistryError::DuplicateMove));
			CHECK((configuration.getAmountRegistered() == 3));
		}
	}
}

SCENARIO("MoveRegistryConfiguration addMoves")
{
	ensureMoveLoggerInitialized();
	MoveRegistryConfiguration configuration{};

	GIVEN("a batch containing a duplicate move name")
	{
		std::array<MoveMeta, 3> definitions{
			{
				makeMove("Batch One"),
				makeMove("Batch Two"),
				makeMove("Batch One"),
			},
		};

		WHEN("the batch is added")
		{
			auto result{configuration.addMoves(definitions)};

			THEN("the batch is rolled back atomically")
			{
				REQUIRE_FALSE(result.has_value());
				CHECK((result.error().mKind == RegistryError::DuplicateMove));
				CHECK((configuration.getAmountRegistered() == 3));
				CHECK_FALSE(configuration.hasMove("Batch One"));
				CHECK_FALSE(configuration.hasMove("Batch Two"));
			}
		}
	}
}

SCENARIO("MoveRegistryConfiguration metadata lifecycle")
{
	ensureMoveLoggerInitialized();
	MoveRegistryConfiguration configuration{};

	GIVEN("a registered custom move")
	{
		MoveMeta definition{makeMove("Custom Move")};
		auto addResult{configuration.addMove(definition)};
		REQUIRE(addResult.has_value());
		MoveID customIdentifier{addResult.value()};

		WHEN("its triggers are replaced by name")
		{
			std::array<EffectTrigger, 1> replacement{
				{{.mEffects = {toEffectID(BuiltinEffectID::Recoil)}, .mTrigger = BattleEventID::MoveUse, .mRole = BattleEventRole::User}},
			};
			auto setResult{configuration.setMoveTriggers("Custom Move", replacement)};

			THEN("the updated metadata is visible through stable ID")
			{
				REQUIRE(setResult.has_value());
				const MoveMeta *metadata{configuration.getMoveMetadata(customIdentifier)};
				REQUIRE((metadata != nullptr));
				CHECK((metadata->mTriggers.front().mTrigger == BattleEventID::MoveUse));
				CHECK((metadata->mTriggers.front().mRole == BattleEventRole::User));
				CHECK((metadata->mTriggers.front().mEffects.front() == toEffectID(BuiltinEffectID::Recoil)));
			}
		}

		WHEN("its triggers are replaced by stable ID")
		{
			std::array<EffectTrigger, 1> replacement{
				{{.mEffects = {toEffectID(BuiltinEffectID::StatusApply)}, .mTrigger = BattleEventID::Hit}},
			};
			auto setResult{configuration.setMoveTriggers(customIdentifier, replacement)};

			THEN("the update remains associated with the same ID")
			{
				REQUIRE(setResult.has_value());
				const MoveMeta *metadata{configuration.getMoveMetadata(customIdentifier)};
				REQUIRE((metadata != nullptr));
				CHECK((metadata->mTriggers.front().mEffects.front() == toEffectID(BuiltinEffectID::StatusApply)));
			}
		}

		WHEN("its target is replaced by name")
		{
			auto setResult{configuration.setMoveTarget("Custom Move", BattleTargetID::AllOpponents)};

			THEN("target metadata changes")
			{
				REQUIRE(setResult.has_value());
				const MoveMeta *metadata{configuration.getMoveMetadata(customIdentifier)};
				REQUIRE((metadata != nullptr));
				CHECK((metadata->mTargetID == BattleTargetID::AllOpponents));
			}
		}

		WHEN("its target is replaced by stable ID")
		{
			auto setResult{configuration.setMoveTarget(customIdentifier, BattleTargetID::Self)};

			THEN("target metadata changes")
			{
				REQUIRE(setResult.has_value());
				const MoveMeta *metadata{configuration.getMoveMetadata(customIdentifier)};
				REQUIRE((metadata != nullptr));
				CHECK((metadata->mTargetID == BattleTargetID::Self));
			}
		}

		WHEN("it is renamed")
		{
			auto renameResult{configuration.renameMove("Custom Move", "Renamed Move")};

			THEN("its stable ID remains unchanged")
			{
				REQUIRE(renameResult.has_value());
				CHECK_FALSE(configuration.hasMove("Custom Move"));
				auto renamedIdentifier{configuration.getMoveID("Renamed Move")};
				REQUIRE(renamedIdentifier.has_value());
				// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
				CHECK((renamedIdentifier.value() == customIdentifier));
			}
		}

		WHEN("it is updated by name")
		{
			MoveMeta replacement{makeMove("Replacement")};
			replacement.mTargetID = BattleTargetID::AllExceptSelf;
			auto updateResult{configuration.updateMove("Custom Move", replacement)};

			THEN("stored metadata is replaced while preserving its stable ID")
			{
				REQUIRE(updateResult.has_value());
				auto replacementIdentifier{configuration.getMoveID("Replacement")};
				REQUIRE(replacementIdentifier.has_value());
				// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
				CHECK((replacementIdentifier.value() == customIdentifier));

				const auto registeredMoves{configuration.getRegisteredMoves()};
				auto foundMove = std::ranges::find_if(registeredMoves.begin(), registeredMoves.end(),
													  [](const MoveMeta &metadata) { return metadata.mName == "Replacement"; });
				REQUIRE((foundMove != registeredMoves.end()));
				CHECK((foundMove->mTargetID == BattleTargetID::AllExceptSelf));
				CHECK(configuration.hasMove(customIdentifier));
			}
		}

		WHEN("it is updated by stable ID")
		{
			MoveMeta replacement{makeMove("Replacement By ID")};
			replacement.mTargetID = BattleTargetID::AllAllies;
			auto updateResult{configuration.updateMove(customIdentifier, replacement)};

			THEN("stored metadata is replaced while preserving its stable ID")
			{
				REQUIRE(updateResult.has_value());
				auto replacementIdentifier{configuration.getMoveID("Replacement By ID")};
				REQUIRE(replacementIdentifier.has_value());
				// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
				CHECK((replacementIdentifier.value() == customIdentifier));

				const auto registeredMoves{configuration.getRegisteredMoves()};
				auto foundMove = std::ranges::find_if(registeredMoves.begin(), registeredMoves.end(),
													  [](const MoveMeta &metadata) { return metadata.mName == "Replacement By ID"; });
				REQUIRE((foundMove != registeredMoves.end()));
				CHECK((foundMove->mTargetID == BattleTargetID::AllAllies));
				CHECK(configuration.hasMove(customIdentifier));
			}
		}

		WHEN("an update uses another registered move's name")
		{
			MoveMeta replacement{makeMove("Pound")};
			auto updateResult{configuration.updateMove(customIdentifier, replacement)};

			THEN("the update is rejected without changing the original entry")
			{
				REQUIRE_FALSE(updateResult.has_value());
				CHECK((updateResult.error().mKind == RegistryError::DuplicateMove));
				CHECK(configuration.hasMove(customIdentifier));
				CHECK(configuration.hasMove("Custom Move"));
			}
		}

		WHEN("a name-based update uses another registered move's name")
		{
			MoveMeta replacement{makeMove("Pound")};
			auto updateResult{configuration.updateMove("Custom Move", replacement)};

			THEN("the update is rejected without changing the original entry")
			{
				REQUIRE_FALSE(updateResult.has_value());
				CHECK((updateResult.error().mKind == RegistryError::DuplicateMove));
				CHECK(configuration.hasMove(customIdentifier));
				CHECK(configuration.hasMove("Custom Move"));
			}
		}

		WHEN("it is removed by name")
		{
			auto removeResult{configuration.removeMove("Custom Move")};

			THEN("the removed ID is returned")
			{
				REQUIRE(removeResult.has_value());
				CHECK((removeResult.value() == customIdentifier));
				CHECK_FALSE(configuration.hasMove(customIdentifier));
			}
		}

		WHEN("it is removed by stable ID")
		{
			auto removeResult{configuration.removeMove(customIdentifier)};

			THEN("the removed ID is returned")
			{
				REQUIRE(removeResult.has_value());
				CHECK((removeResult.value() == customIdentifier));
				CHECK_FALSE(configuration.hasMove(customIdentifier));
			}
		}

		WHEN("it is removed then a later move is added")
		{
			auto removeResult{configuration.removeMove(customIdentifier)};
			REQUIRE(removeResult.has_value());

			MoveMeta laterDefinition{makeMove("Later Move")};
			auto laterResult{configuration.addMove(laterDefinition)};

			THEN("the removed stable ID is not reused")
			{
				REQUIRE(laterResult.has_value());
				CHECK((laterResult.value() != customIdentifier));
				CHECK((configuration.getMoveMetadata(customIdentifier) == nullptr));
			}
		}
	}

	GIVEN("an unknown move")
	{
		THEN("mutation rename update and remove report MoveNotFound")
		{
			auto triggersResult{configuration.setMoveTriggers("Missing", {})};
			auto targetResult{configuration.setMoveTarget("Missing", BattleTargetID::Self)};
			auto renameResult{configuration.renameMove("Missing", "Renamed")};
			auto updateResult{configuration.updateMove("Missing", makeMove("Updated"))};
			auto removeResult{configuration.removeMove("Missing")};

			REQUIRE_FALSE(triggersResult.has_value());
			REQUIRE_FALSE(targetResult.has_value());
			REQUIRE_FALSE(renameResult.has_value());
			REQUIRE_FALSE(updateResult.has_value());
			REQUIRE_FALSE(removeResult.has_value());
			CHECK((triggersResult.error().mKind == RegistryError::MoveNotFound));
			CHECK((targetResult.error().mKind == RegistryError::MoveNotFound));
			CHECK((renameResult.error().mKind == RegistryError::MoveNotFound));
			CHECK((updateResult.error().mKind == RegistryError::MoveNotFound));
			CHECK((removeResult.error().mKind == RegistryError::MoveNotFound));
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity,llvm-prefer-static-over-anonymous-namespace)
