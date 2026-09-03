/*! @file moveRegistry.test.cpp
	@brief C++ file for running tests for the MoveRegistry.
	@date 09/03/2026
	@since 0.6.0
	@version 0.12.19
	@author Matthew Moore
*/

#include "Registry/moveRegistry.h"

#include <optional>
#include <span>
#include <string_view>

#include "Battle/battleTargetsAndTriggers.h"
#include "Core/typedefs.h"
#include "Effect/builtInEffectID.h"
#include "Move/builtInMoveID.h"
#include "Move/constants.h"
#include "Move/moveID.h"
#include "Move/moveMeta.h"
#include "Types/builtInTypeID.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Battle::BattleEventID;
using PocketCore::Battle::BattleEventRole;
using PocketCore::Battle::BattleRangeID;
using PocketCore::Battle::BattleTargetID;
using PocketCore::Core::ub;
using PocketCore::Effect::BuiltinEffectID;
using PocketCore::Effect::toEffectID;
using PocketCore::Move::baseAttackEffects;
using PocketCore::Move::BuiltinMoveID;
using PocketCore::Move::MOVE_NAME_KARATE_CHOP;
using PocketCore::Move::MOVE_NAME_NONE;
using PocketCore::Move::MOVE_NAME_POUND;
using PocketCore::Move::MoveID;
using PocketCore::Move::MoveMeta;
using PocketCore::Move::NO_MOVE_ID;
using PocketCore::Move::toMoveID;
using PocketCore::Registry::Move::MoveRegistry;
using PocketCore::Type::BuiltinTypeID;
using PocketCore::Type::toTypeID;

template <typename Registry>
concept PubliclyStructurallyMutable = requires(Registry &registry) { registry.setAmountRegistered(0); };

static_assert(!PubliclyStructurallyMutable<MoveRegistry>);

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("MoveRegistry")
{
	MoveRegistry registry{};
	ub finalMoveUnderlyingValue{std::to_underlying(BuiltinMoveID::FinalMove)};

	GIVEN("a default constructed move registry")
	{
		THEN("Pound has the appropriate properties")
		{
			MoveMeta expected{
				.mTriggers = {{
								  .mEffects = {toEffectID(BuiltinEffectID::AccuracyCheck)},
								  .mTrigger = BattleEventID::BeforeHit,
								  .mRole = BattleEventRole::Target,
							  },
							  {
								  .mEffects = {baseAttackEffects().begin(), baseAttackEffects().end()},
								  .mTrigger = BattleEventID::Hit,
								  .mRole = BattleEventRole::Target,
							  },},
				.mName = MOVE_NAME_POUND,
				.mMoveID = toMoveID(BuiltinMoveID::Pound),
				.mTypeID = toTypeID(BuiltinTypeID::Normal),
				.mPower = 40,
				.mTargetID = BattleTargetID::SingleOpponent,
				.mRangeID = BattleRangeID::Adjacent,
				.mAccuracy = 100,
				.mPriority = 0,
				.mSpecial = false,
			};

			const MoveMeta *actual{registry.getMoveMetadata(toMoveID(BuiltinMoveID::Pound))};

			CHECK((expected == *actual));
		}

		THEN("Karate Chop has the appropriate properties")
		{
			MoveMeta expected{
				.mTriggers = {{
								  .mEffects = {toEffectID(BuiltinEffectID::AccuracyCheck)},
								  .mTrigger = BattleEventID::BeforeHit,
								  .mRole = BattleEventRole::Target,
							  },
							  {
								  .mEffects = {baseAttackEffects().begin(), baseAttackEffects().end()},
								  .mTrigger = BattleEventID::Hit,
								  .mRole = BattleEventRole::Target,
							  },},
				.mName = MOVE_NAME_KARATE_CHOP,
				.mMoveID = toMoveID(BuiltinMoveID::KarateChop),
				.mTypeID = toTypeID(BuiltinTypeID::Fighting),
				.mPower = 50,
				.mTargetID = BattleTargetID::SingleOpponent,
				.mRangeID = BattleRangeID::Adjacent,
				.mAccuracy = 100,
				.mPriority = 0,
				.mSpecial = false,
			};

			const MoveMeta *actual{registry.getMoveMetadata(toMoveID(BuiltinMoveID::KarateChop))};

			CHECK((expected == *actual));
		}
	}

	GIVEN("getMoveMetadata")
	{
		THEN("unknown IDs are absent")
		{
			CHECK((registry.getMoveMetadata(MoveID{200}) == nullptr));
		}

		THEN("the metadata is retrieved when accessed by a valid Move ID")
		{
			MoveMeta expected{
				.mTriggers = {},
				.mName = MOVE_NAME_NONE,
				.mMoveID = toMoveID(BuiltinMoveID::None),
			};

			CHECK((expected == *registry.getMoveMetadata(NO_MOVE_ID)));
		}
	}

	GIVEN("getMoveID")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(registry.getMoveID("Unknown").has_value());
		}

		THEN("the Move ID is retrieved by valid Move name")
		{
			std::optional<MoveID> moveID{registry.getMoveID(MOVE_NAME_NONE)};

			REQUIRE(moveID.has_value());

			CHECK((moveID.value() == toMoveID(BuiltinMoveID::None)));
		}
	}

	GIVEN("getMoveName")
	{
		THEN("unknown IDs are absent")
		{
			CHECK_FALSE(registry.getMoveName(MoveID{200}).has_value());
		}

		THEN("a registered move name is returned by stable ID")
		{
			std::optional<std::string_view> moveName{registry.getMoveName(toMoveID(BuiltinMoveID::None))};

			REQUIRE(moveName.has_value());

			CHECK((moveName.value() == MOVE_NAME_NONE));
		}
	}

	GIVEN("getAmountRegistered")
	{
		THEN("the registered span contains the exact amount of built-in entries")
		{
			CHECK((registry.getAmountRegistered() == finalMoveUnderlyingValue));
		}
	}

	GIVEN("getEntry")
	{
		THEN("an invalid internal array index has no metadata")
		{
			CHECK((registry.getEntry(2'000) == nullptr));
		}

		THEN("a valid internal array index has metadata")
		{
			MoveMeta expected{
				.mTriggers = {},
				.mName = MOVE_NAME_NONE,
				.mMoveID = toMoveID(BuiltinMoveID::None),
			};

			const MoveMeta *moveMeta{registry.getEntry(0)};

			REQUIRE((moveMeta != nullptr));
			CHECK((*moveMeta == expected));
		}
	}

	GIVEN("getRegisteredMoves")
	{
		THEN("the amount of moves returned matches the amount that are built-in")
		{
			CHECK((registry.getRegisteredMoves().size() == finalMoveUnderlyingValue));
		}
	}

	GIVEN("getNextMoveID")
	{
		THEN("the next available stable Move ID is after all built in move IDs")
		{
			CHECK((registry.getNextMoveID() == finalMoveUnderlyingValue));
		}
	}

	GIVEN("findIndexByMoveID")
	{
		THEN("an unknown stable ID has no internal index")
		{
			std::optional<ub> moveIndex{registry.findIndexByMoveID(MoveID{200})};
			CHECK_FALSE(moveIndex.has_value());
		}

		THEN("the internal array index is retrieved by valid Move ID")
		{
			std::optional<ub> moveIndex{registry.findIndexByMoveID(NO_MOVE_ID)};

			REQUIRE(moveIndex.has_value());
			CHECK((moveIndex.value() == 0));
		}
	}

	GIVEN("hasMove")
	{
		WHEN("calling the string_view overload")
		{
			THEN("an unknown move name has no entry")
			{
				CHECK_FALSE(registry.hasMove("Unknown"));
			}

			THEN("a known move name has an entry")
			{
				CHECK(registry.hasMove(MOVE_NAME_NONE));
			}
		}

		WHEN("calling the MoveID overload")
		{
			THEN("an unknown move ID has no entry")
			{
				CHECK_FALSE(registry.hasMove(MoveID{200}));
			}

			THEN("a known move ID has an entry")
			{
				CHECK(registry.hasMove(NO_MOVE_ID));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
