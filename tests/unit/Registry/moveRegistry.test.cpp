/*! @file moveRegistry.test.cpp
	@brief C++ file for running tests for the MoveRegistry.
	@date 10/28/2026
	@since 0.6.0
	@version 0.12.53
	@author Matthew Moore
*/

#include "Registry/moveRegistry.h"

#include <expected>
#include <memory>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <utility>

#include "Battle/battleTargetsAndTriggers.h"
#include "Configuration/moveRegistryConfiguration.h"
#include "Core/typedefs.h"
#include "Effect/builtInEffectID.h"
#include "Move/builtInMoveID.h"
#include "Move/constants.h"
#include "Move/moveID.h"
#include "Move/moveMeta.h"
#include "Type/builtInTypeID.h"
#include "Validation/Registry/registryError.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Battle::BattleEventID;
using PocketCore::Battle::BattleEventRole;
using PocketCore::Battle::BattleRangeID;
using PocketCore::Battle::BattleTargetID;
using PocketCore::Configuration::MoveRegistryConfiguration;
using PocketCore::Core::ub;
using PocketCore::Effect::BuiltinEffectID;
using PocketCore::Effect::toEffectID;
using PocketCore::Move::baseAttackEffects;
using PocketCore::Move::BuiltinMoveID;
using PocketCore::Move::MOVE_NAME_EXTREME_SPEED;
using PocketCore::Move::MOVE_NAME_FACADE;
using PocketCore::Move::MOVE_NAME_HYDRO_STEAM;
using PocketCore::Move::MOVE_NAME_KARATE_CHOP;
using PocketCore::Move::MOVE_NAME_NONE;
using PocketCore::Move::MOVE_NAME_POUND;
using PocketCore::Move::MOVE_NAME_QUICK_ATTACK;
using PocketCore::Move::MoveID;
using PocketCore::Move::MoveMeta;
using PocketCore::Move::NO_MOVE_ID;
using PocketCore::Move::toMoveID;
using PocketCore::Registry::Move::MoveRegistry;
using PocketCore::Type::BuiltinTypeID;
using PocketCore::Type::toTypeID;
using PocketCore::Validation::Registry::RegistryError;

template <typename Registry>
concept PubliclyStructurallyMutable = requires(Registry &registry) { registry.setAmountRegistered(0); };

static_assert(!PubliclyStructurallyMutable<MoveRegistry>);

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("MoveRegistry")
{
	std::unique_ptr<MoveRegistry> registryStorage{std::make_unique<MoveRegistry>()};
	MoveRegistry &registry{*registryStorage};
	ub finalMoveUnderlyingValue{std::to_underlying(BuiltinMoveID::FinalMove)};

	GIVEN("a default constructed move registry")
	{
		THEN("Pound has the appropriate properties")
		{
			MoveMeta expected{
				.mName = std::string(MOVE_NAME_POUND),
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
				.mMoveID = toMoveID(BuiltinMoveID::Pound),
				.mTypeID = toTypeID(BuiltinTypeID::Normal),
				.mPower = 40,
				.mTargetID = BattleTargetID::SingleOpponent,
				.mRangeID = BattleRangeID::Adjacent,
				.mAccuracy = 100,
				.mPriority = 0,
				.mPPMaxAmount = 56,
				.mPPDefaultAmount = 35,
				.mSpecial = false,
			};

			const MoveMeta *actual{registry.getMoveMetadata(toMoveID(BuiltinMoveID::Pound))};

			CHECK((expected == *actual));
		}

		THEN("Karate Chop has the appropriate properties")
		{
			MoveMeta expected{
				.mName = std::string(MOVE_NAME_KARATE_CHOP),
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
				.mMoveID = toMoveID(BuiltinMoveID::KarateChop),
				.mTypeID = toTypeID(BuiltinTypeID::Fighting),
				.mPower = 50,
				.mTargetID = BattleTargetID::SingleOpponent,
				.mRangeID = BattleRangeID::Adjacent,
				.mAccuracy = 100,
				.mPriority = 0,
				.mPPMaxAmount = 40,
				.mPPDefaultAmount = 25,
				.mSpecial = false,
			};

			const MoveMeta *actual{registry.getMoveMetadata(toMoveID(BuiltinMoveID::KarateChop))};

			CHECK((expected == *actual));
		}

		THEN("Quick Attack has the appropriate properties")
		{
			MoveMeta expected{
				.mName = std::string(MOVE_NAME_QUICK_ATTACK),
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
				.mMoveID = toMoveID(BuiltinMoveID::QuickAttack),
				.mTypeID = toTypeID(BuiltinTypeID::Normal),
				.mPower = 40,
				.mTargetID = BattleTargetID::SingleOpponent,
				.mRangeID = BattleRangeID::Adjacent,
				.mAccuracy = 100,
				.mPriority = 1,
				.mPPMaxAmount = 48,
				.mPPDefaultAmount = 30,
				.mSpecial = false,
			};

			const MoveMeta *actual{registry.getMoveMetadata(toMoveID(BuiltinMoveID::QuickAttack))};

			CHECK((expected == *actual));
		}

		THEN("Extreme Speed has the appropriate properties")
		{
			MoveMeta expected{
				.mName = std::string(MOVE_NAME_EXTREME_SPEED),
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
				.mMoveID = toMoveID(BuiltinMoveID::ExtremeSpeed),
				.mTypeID = toTypeID(BuiltinTypeID::Normal),
				.mPower = 80,
				.mTargetID = BattleTargetID::SingleOpponent,
				.mRangeID = BattleRangeID::Adjacent,
				.mAccuracy = 100,
				.mPriority = 2,
				.mPPMaxAmount = 8,
				.mPPDefaultAmount = 5,
				.mSpecial = false,
			};

			const MoveMeta *actual{registry.getMoveMetadata(toMoveID(BuiltinMoveID::ExtremeSpeed))};

			CHECK((expected == *actual));
		}

		THEN("Facade has the appropriate properties")
		{
			MoveMeta expected{
				.mName = std::string(MOVE_NAME_FACADE),
				.mMoveID = toMoveID(BuiltinMoveID::Facade),
			};

			const MoveMeta *actual{registry.getMoveMetadata(toMoveID(BuiltinMoveID::Facade))};

			CHECK((expected == *actual));
		}

		THEN("Hydro Steam has the appropriate properties")
		{
			MoveMeta expected{
				.mName = std::string(MOVE_NAME_HYDRO_STEAM),
				.mMoveID = toMoveID(BuiltinMoveID::HydroSteam),
			};

			const MoveMeta *actual{registry.getMoveMetadata(toMoveID(BuiltinMoveID::HydroSteam))};

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
				.mName = std::string(MOVE_NAME_NONE),
				.mTriggers = {},
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

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((moveID == toMoveID(BuiltinMoveID::None)));
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

			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((moveName == MOVE_NAME_NONE));
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
				.mName = std::string(MOVE_NAME_NONE),
				.mTriggers = {},
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
			// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
			CHECK((moveIndex == 0));
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

	WHEN("operator==")
	{
		GIVEN("two default constructed registries")
		{
			std::unique_ptr<MoveRegistry> other{std::make_unique<MoveRegistry>()};

			THEN("they are equal")
			{
				CHECK((registry == *other));
			}
		}

		GIVEN("for an entry added in one registry")
		{
			std::unique_ptr<MoveRegistryConfiguration> other{std::make_unique<MoveRegistryConfiguration>()};
			std::expected<MoveID, RegistryError> result{other->addMove({.mName = "test"})};

			THEN("they are not equal")
			{
				REQUIRE(result.has_value());
				CHECK((registry != other->getRuntimeRegistry()));
			}
		}

		GIVEN("for an entry removed in one registry")
		{
			std::unique_ptr<MoveRegistryConfiguration> other{std::make_unique<MoveRegistryConfiguration>()};
			std::expected<MoveID, RegistryError> result{other->removeMove(toMoveID(BuiltinMoveID::None))};

			THEN("they are not equal")
			{
				REQUIRE(result.has_value());
				CHECK((registry != other->getRuntimeRegistry()));
			}
		}

		GIVEN("for an entry modified in one registry")
		{
			std::unique_ptr<MoveRegistryConfiguration> other{std::make_unique<MoveRegistryConfiguration>()};
			std::expected<void, RegistryError> result{
				other->updateMove(toMoveID(BuiltinMoveID::None), {.mName = "test"}),
			};

			THEN("they are not equal")
			{
				REQUIRE(result.has_value());
				CHECK((registry != other->getRuntimeRegistry()));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
