#include "Registry/moveRegistry.h"

#include <optional>
#include <span>
#include <string_view>

#include "Battle/battleTargetsAndTriggers.h"
#include "Effect/builtInEffectID.h"
#include "Move/builtInMoveID.h"
#include "Move/constants.h"
#include "Move/moveID.h"
#include "Move/moveMeta.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Battle::BattleEventID;
using PocketCore::Effect::BuiltinEffectID;
using PocketCore::Effect::toEffectID;
using PocketCore::Move::BuiltinMoveID;
using PocketCore::Move::MOVE_NAME_KARATE_CHOP;
using PocketCore::Move::MOVE_NAME_NONE;
using PocketCore::Move::MOVE_NAME_POUND;
using PocketCore::Move::MoveID;
using PocketCore::Move::MoveMeta;
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

			std::optional<MoveID> poundIdentifier{registry.getMoveID(MOVE_NAME_POUND)};
			std::optional<MoveID> karateChopIdentifier{registry.getMoveID(MOVE_NAME_KARATE_CHOP)};
			REQUIRE(poundIdentifier.has_value());
			REQUIRE(karateChopIdentifier.has_value());
			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((poundIdentifier.value() == toMoveID(BuiltinMoveID::Pound)));
			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((karateChopIdentifier.value() == toMoveID(BuiltinMoveID::KarateChop)));
		}

		THEN("Pound retains its psychic terrain priority block metadata")
		{
			const MoveMeta *metadata{registry.getMoveMetadata(toMoveID(BuiltinMoveID::Pound))};
			REQUIRE((metadata != nullptr));
			REQUIRE((metadata->mTriggers.size() == 2U));
			CHECK((metadata->mTriggers.at(1).mTrigger == BattleEventID::Hit));
			REQUIRE((metadata->mTriggers.at(1).mEffects.size() == 10U));
			CHECK((metadata->mTriggers.at(1).mEffects.front() == toEffectID(BuiltinEffectID::PsychicTerrainPriorityBlock)));
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

		THEN("the registered span contains exactly the built-in entries")
		{
			std::span<const MoveMeta> moves{registry.getRegisteredMoves()};
			REQUIRE((moves.size() == 3U));
			CHECK((moves.front().mName == MOVE_NAME_NONE));
			CHECK((moves.back().mName == MOVE_NAME_KARATE_CHOP));
			CHECK(registry.hasMove(toMoveID(BuiltinMoveID::Pound)));
			CHECK(registry.hasMove(MOVE_NAME_KARATE_CHOP));
		}

		THEN("the registered amount can be restored directly")
		{
			registry.setAmountRegistered(2);
			CHECK((registry.getAmountRegistered() == 2));
		}

		THEN("the next stable ID can be restored directly")
		{
			registry.setNextMoveID(42);
			CHECK((registry.getNextMoveID() == 42));
		}

		THEN("the incrementNextMoveID() method increments the next stable ID")
		{
			registry.setNextMoveID(42);
			registry.incrementNextMoveID();
			CHECK((registry.getNextMoveID() == 43));
		}

		THEN("the shared registry mutators can update entries and counts")
		{
			CHECK((registry.getAmountRegistered() == 3));
			registry.incrementAmountRegistered();
			CHECK((registry.getAmountRegistered() == 4));

			const MoveMeta &entry{registry.getEntry(0)};
			CHECK((entry.mName == MOVE_NAME_NONE));

			MoveMeta replacement{entry};
			replacement.mName = "CustomMove";
			registry.setEntry(0, replacement);
			CHECK((registry.getEntry(0).mName == "CustomMove"));

			registry.decrementAmountRegistered();
			CHECK((registry.getAmountRegistered() == 3));
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
