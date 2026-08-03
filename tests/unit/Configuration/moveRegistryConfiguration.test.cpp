#include "Configuration/moveRegistryConfiguration.h"

#include <algorithm>
#include <array>
#include <optional>
#include <string_view>
#include <vector>

#include "Effect/effectType.h"
#include "Move/moveID.h"
#include "Move/moveMeta.h"
#include "Move/moveTargetsAndTriggers.h"
#include "Utility/Debug/Logging/logger.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Configuration::MoveRegistryConfiguration;
using PocketCore::Configuration::RegistryError;
using PocketCore::Effect::EffectTypeID;
using PocketCore::Move::MoveEffectTrigger;
using PocketCore::Move::MoveID;
using PocketCore::Move::MoveMeta;
using PocketCore::Move::MoveTargetID;
using PocketCore::Move::MoveTriggerID;
using PocketCore::Utility::Debug::Logging::Logger;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity,llvm-prefer-static-over-anonymous-namespace)

namespace
{
	void ensureMoveLoggerInitialized() // NOLINT(llvm-prefer-static-over-anonymous-namespace)
	{
		static bool initialized{false};

		if (!initialized)
		{
			initialized = Logger::initialize("mrc_test", "moveRegistryConfiguration_test.log", true);
		}
	}

	MoveMeta makeMove(const std::string_view name)
	{
		return MoveMeta{
			.mTriggers = {{.mEffects = {EffectTypeID::AccuracyCheck, EffectTypeID::BaseDamage}, .mTrigger = MoveTriggerID::OnTarget}},
			.mName = name,
			.mTargetID = MoveTargetID::SingleOpponent,
		};
	}
} // namespace

SCENARIO("MoveRegistryConfiguration addMove")
{
	ensureMoveLoggerInitialized();
	MoveRegistryConfiguration configuration{};

	GIVEN("a unique move definition")
	{
		std::vector<MoveEffectTrigger> triggers{
			{.mEffects = {EffectTypeID::AccuracyCheck, EffectTypeID::BaseDamage}, .mTrigger = MoveTriggerID::OnTarget},
		};
		MoveMeta definition{.mTriggers = triggers, .mName = "Custom Jab", .mTargetID = MoveTargetID::SingleOpponent};

		WHEN("the move is added")
		{
			auto result{configuration.addMove(definition)};

			THEN("it receives the first custom stable ID and owns trigger data")
			{
				REQUIRE(result.has_value());
				MoveID assignedIdentifier{result.value()};
				CHECK((assignedIdentifier.getValue() == 3U));

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
		MoveMeta definition{.mTriggers = {}, .mName = "Pound", .mTargetID = MoveTargetID::SingleOpponent};

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
			std::array<MoveEffectTrigger, 1> replacement{
				{{.mEffects = {EffectTypeID::Recoil}, .mTrigger = MoveTriggerID::OnUse}},
			};
			auto setResult{configuration.setMoveTriggers("Custom Move", replacement)};

			THEN("the updated metadata is visible through stable ID")
			{
				REQUIRE(setResult.has_value());
				const MoveMeta *metadata{configuration.getMoveMetadata(customIdentifier)};
				REQUIRE((metadata != nullptr));
				CHECK((metadata->mTriggers.front().mTrigger == MoveTriggerID::OnUse));
				CHECK((metadata->mTriggers.front().mEffects.front() == EffectTypeID::Recoil));
			}
		}

		WHEN("its triggers are replaced by stable ID")
		{
			std::array<MoveEffectTrigger, 1> replacement{
				{{.mEffects = {EffectTypeID::StatusApply}, .mTrigger = MoveTriggerID::OnTarget}},
			};
			auto setResult{configuration.setMoveTriggers(customIdentifier, replacement)};

			THEN("the update remains associated with the same ID")
			{
				REQUIRE(setResult.has_value());
				const MoveMeta *metadata{configuration.getMoveMetadata(customIdentifier)};
				REQUIRE((metadata != nullptr));
				CHECK((metadata->mTriggers.front().mEffects.front() == EffectTypeID::StatusApply));
			}
		}

		WHEN("its target is replaced by name")
		{
			auto setResult{configuration.setMoveTarget("Custom Move", MoveTargetID::AllOpponents)};

			THEN("target metadata changes")
			{
				REQUIRE(setResult.has_value());
				const MoveMeta *metadata{configuration.getMoveMetadata(customIdentifier)};
				REQUIRE((metadata != nullptr));
				CHECK((metadata->mTargetID == MoveTargetID::AllOpponents));
			}
		}

		WHEN("its target is replaced by stable ID")
		{
			auto setResult{configuration.setMoveTarget(customIdentifier, MoveTargetID::Self)};

			THEN("target metadata changes")
			{
				REQUIRE(setResult.has_value());
				const MoveMeta *metadata{configuration.getMoveMetadata(customIdentifier)};
				REQUIRE((metadata != nullptr));
				CHECK((metadata->mTargetID == MoveTargetID::Self));
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
			replacement.mTargetID = MoveTargetID::AllExceptSelf;
			auto updateResult{configuration.updateMove("Custom Move", replacement)};

			THEN("stored metadata is replaced while preserving its stable ID")
			{
				REQUIRE(updateResult.has_value());
				auto replacementIdentifier{configuration.getMoveID("Replacement")};
				REQUIRE(replacementIdentifier.has_value());
				CHECK((replacementIdentifier.value() == customIdentifier));

				const auto registeredMoves{configuration.getRegisteredMoves()};
				auto foundMove = std::ranges::find_if(registeredMoves.begin(), registeredMoves.end(),
													  [](const MoveMeta &metadata) { return metadata.mName == "Replacement"; });
				REQUIRE((foundMove != registeredMoves.end()));
				CHECK((foundMove->mTargetID == MoveTargetID::AllExceptSelf));
				CHECK(configuration.hasMove(customIdentifier));
			}
		}

		WHEN("it is updated by stable ID")
		{
			MoveMeta replacement{makeMove("Replacement By ID")};
			replacement.mTargetID = MoveTargetID::AllAllies;
			auto updateResult{configuration.updateMove(customIdentifier, replacement)};

			THEN("stored metadata is replaced while preserving its stable ID")
			{
				REQUIRE(updateResult.has_value());
				auto replacementIdentifier{configuration.getMoveID("Replacement By ID")};
				REQUIRE(replacementIdentifier.has_value());
				CHECK((replacementIdentifier.value() == customIdentifier));

				const auto registeredMoves{configuration.getRegisteredMoves()};
				auto foundMove = std::ranges::find_if(registeredMoves.begin(), registeredMoves.end(),
													  [](const MoveMeta &metadata) { return metadata.mName == "Replacement By ID"; });
				REQUIRE((foundMove != registeredMoves.end()));
				CHECK((foundMove->mTargetID == MoveTargetID::AllAllies));
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
			auto targetResult{configuration.setMoveTarget("Missing", MoveTargetID::Self)};
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
