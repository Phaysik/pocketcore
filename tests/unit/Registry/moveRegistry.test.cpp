#include "Registry/moveRegistry.h"

#include <optional>
#include <span>
#include <string_view>

#include "Effect/effectType.h"
#include "Move/builtinMoveID.h"
#include "Move/moveID.h"
#include "Move/moveMeta.h"
#include "Move/moveTargetsAndTriggers.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Effect::EffectTypeID;
using PocketCore::Move::BuiltinMoveID;
using PocketCore::Move::MoveID;
using PocketCore::Move::MoveMeta;
using PocketCore::Move::MoveTriggerID;
using PocketCore::Move::toMoveID;
using PocketCore::Registry::Move::MoveRegistry;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("MoveRegistry")
{
	MoveRegistry registry{};

	GIVEN("a default-constructed registry")
	{
		THEN("all built-in move metadata is registered")
		{
			CHECK((registry.getAmountRegistered() == 3));
			CHECK((registry.getNextMoveID() == 3));

			std::optional<MoveID> poundIdentifier{registry.getMoveID("Pound")};
			std::optional<MoveID> karateChopIdentifier{registry.getMoveID("Karate Chop")};
			REQUIRE(poundIdentifier.has_value());
			REQUIRE(karateChopIdentifier.has_value());
			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((poundIdentifier.value() == toMoveID(BuiltinMoveID::Pound)));
			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((karateChopIdentifier.value() == toMoveID(BuiltinMoveID::KarateChop)));
		}

		THEN("Chesto Berry retains its turn-end status removal metadata")
		{
			const MoveMeta *metadata{registry.getMoveMetadata(toMoveID(BuiltinMoveID::Pound))};
			REQUIRE((metadata != nullptr));
			REQUIRE((metadata->mTriggers.size() == 1U));
			CHECK((metadata->mTriggers.front().mTrigger == MoveTriggerID::OnUse));
			REQUIRE((metadata->mTriggers.front().mEffects.size() == 10U));
			CHECK((metadata->mTriggers.front().mEffects.front() == EffectTypeID::StatusRemove));
		}

		THEN("unknown move lookups are absent")
		{
			MoveID unknownIdentifier{200};
			CHECK_FALSE(registry.getMoveID("Unknown").has_value());
			CHECK((registry.getMoveMetadata(unknownIdentifier) == nullptr));
			CHECK_FALSE(registry.getMoveName(unknownIdentifier).has_value());
			CHECK_FALSE(registry.findIndexByMoveID(unknownIdentifier).has_value());
			CHECK_FALSE(registry.hasMove(unknownIdentifier));
		}

		THEN("registered entries are exposed through the domain API")
		{
			std::span<const MoveMeta> moves{registry.getRegisteredMoves()};
			REQUIRE((moves.size() == 3U));
			CHECK((registry.getMoveName(toMoveID(BuiltinMoveID::Pound)) == "Pound"));
			CHECK(registry.hasMove("Karate Chop"));
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
