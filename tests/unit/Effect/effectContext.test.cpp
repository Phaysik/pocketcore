/*! @file effectContext.test.cpp
	@brief C++ file for running tests for the EffectContext.
	@date 09/02/2026
	@since 0.8.7
	@version 0.12.17
	@author Matthew Moore
*/

#include "Effect/effectContext.h"

#include <limits>
#include <span>
#include <utility>

#include "Ability/builtInAbilityID.h"
#include "Battle/battleTargetsAndTriggers.h"
#include "Effect/effectContext.testHelper.h"
#include "Effect/effectSourceAndSuppression.h"
#include "Item/builtInItemID.h"
#include "Move/builtInMoveID.h"
#include "Multiplier/builtInMultiplierID.h"
#include "Multiplier/multiplierID.h"
#include "Nature/builtInNatureID.h"
#include "Registry/multiplierRegistry.h"
#include "Types/builtInTypeID.h"
#include "Utility/Math/floatUtility.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Ability::BuiltinAbilityID;
using PocketCore::Ability::toAbilityID;
using PocketCore::Battle::BattleRangeID;
using PocketCore::Effect::DamageContext;
using PocketCore::Effect::EffectContext;
using PocketCore::Effect::EffectSource;
using PocketCore::Effect::Side;
using PocketCore::Item::BuiltinItemID;
using PocketCore::Item::toItemID;
using PocketCore::Move::BuiltinMoveID;
using PocketCore::Move::toMoveID;
using PocketCore::Multiplier::BuiltinMultiplierID;
using PocketCore::Multiplier::MultiplierID;
using PocketCore::Multiplier::toMultiplierID;
using PocketCore::Nature::BuiltinNatureID;
using PocketCore::Nature::toNatureID;
using PocketCore::Registry::Multiplier::MultiplierRegistry;
using PocketCore::Testing::makeEffectContext;
using PocketCore::Type::BuiltinTypeID;
using PocketCore::Type::toTypeID;
using PocketCore::Utility::Math::approximatelyEqualAbsRel;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

SCENARIO("DamageContext")
{
	DamageContext context{};

	WHEN("operator==")
	{
		GIVEN("two default constructed contexts")
		{
			DamageContext other{};

			THEN("they are equal")
			{
				CHECK((context == other));
			}
		}

		GIVEN("for mRecoilRatio modified in one context")
		{
			DamageContext other{.mRecoilRatio = 0.52};

			THEN("they are not equal")
			{
				CHECK((context != other));
			}
		}

		GIVEN("for mDamage modified in one context")
		{
			DamageContext other{.mDamage = 1};

			THEN("they are not equal")
			{
				CHECK((context != other));
			}
		}

		GIVEN("for mIsCritical modified in one context")
		{
			DamageContext other{.mIsCritical = true};

			THEN("they are not equal")
			{
				CHECK((context != other));
			}
		}

		GIVEN("for mIsMiss modified in one context")
		{
			DamageContext other{.mIsMiss = true};

			THEN("they are not equal")
			{
				CHECK((context != other));
			}
		}

		GIVEN("for mShouldApplyDamage modified in one context")
		{
			DamageContext other{.mShouldApplyDamage = false};

			THEN("they are not equal")
			{
				CHECK((context != other));
			}
		}

		GIVEN("for mShouldContinue modified in one context")
		{
			DamageContext other{.mShouldContinue = false};

			THEN("they are not equal")
			{
				CHECK((context != other));
			}
		}

		GIVEN("two contexts modified the same way")
		{
			DamageContext other{.mDamage = 2};
			context.mDamage = 2;

			THEN("they are equal")
			{
				CHECK((context == other));
			}
		}
	}
}

SCENARIO("EffectContext")
{
	EffectContext context{};

	GIVEN("setMultiplier")
	{
		GIVEN("an empty multiplier context")
		{
			MultiplierID abilityMultiplierID{toMultiplierID(BuiltinMultiplierID::Ability)};

			WHEN("a multiplier is added")
			{
				context.setMultiplier(abilityMultiplierID, 2.0);
				std::span<const std::pair<MultiplierID, double>> activeMultipliers{context.getActiveMultipliers()};

				THEN("the multiplier list stores one entry")
				{
					REQUIRE((activeMultipliers.size() == 1U));
					CHECK((activeMultipliers.front().first == abilityMultiplierID));
					CHECK((approximatelyEqualAbsRel(activeMultipliers.front().second, 2.0)));
				}
			}

			WHEN("the same multiplier is set again")
			{
				context.setMultiplier(abilityMultiplierID, 2.0);
				context.setMultiplier(abilityMultiplierID, 3.0);
				std::span<const std::pair<MultiplierID, double>> activeMultipliers{context.getActiveMultipliers()};

				THEN("the existing entry is updated without duplication")
				{
					REQUIRE((activeMultipliers.size() == 1U));
					CHECK((approximatelyEqualAbsRel(activeMultipliers.front().second, 3.0)));
				}
			}
		}

		GIVEN("an empty multiplier context with a non-builtin ID")
		{
			MultiplierID customID{7U};

			WHEN("a non-builtin multiplier is added")
			{
				context.setMultiplier(customID, 2.0);
				std::span<const std::pair<MultiplierID, double>> activeMultipliers{context.getActiveMultipliers()};

				THEN("the multiplier list stores one entry")
				{
					REQUIRE((activeMultipliers.size() == 1U));
					CHECK((activeMultipliers.front().first == customID));
					CHECK((approximatelyEqualAbsRel(activeMultipliers.front().second, 2.0)));
				}
			}

			WHEN("the same non-builtin multiplier is set again")
			{
				context.setMultiplier(customID, 2.0);
				context.setMultiplier(customID, 3.0);
				std::span<const std::pair<MultiplierID, double>> activeMultipliers{context.getActiveMultipliers()};

				THEN("the existing entry is updated without duplication")
				{
					REQUIRE((activeMultipliers.size() == 1U));
					CHECK((approximatelyEqualAbsRel(activeMultipliers.front().second, 3.0)));
				}
			}
		}

		GIVEN("a context with multiple multipliers")
		{
			MultiplierID customID{20};

			context.setMultiplier(toMultiplierID(BuiltinMultiplierID::Ability), 2.0);
			context.setMultiplier(customID, 5.0);
			context.setMultiplier(toMultiplierID(BuiltinMultiplierID::Item), 1.0);
			context.setMultiplier(toMultiplierID(BuiltinMultiplierID::Critical), 2.5);
			context.setMultiplier(toMultiplierID(BuiltinMultiplierID::Burn), 2.3);

			WHEN("a multiplier is updated")
			{
				context.setMultiplier(customID, 2.0);
				std::span<const std::pair<MultiplierID, double>> activeMultipliers{context.getActiveMultipliers()};

				THEN("the existing entry is updated without being duplication")
				{
					REQUIRE((activeMultipliers.size() == 5U));
					CHECK((activeMultipliers.at(1).first == customID));
					CHECK((approximatelyEqualAbsRel(activeMultipliers.at(1).second, 2.0)));
				}
			}
		}
	}

	GIVEN("applyMultiplier")
	{
		MultiplierRegistry multiplierRegistry{};

		GIVEN("no active multipliers")
		{
			THEN("base damage is unchanged")
			{
				CHECK((context.applyMultiplier(37, multiplierRegistry) == 37));
			}
		}

		GIVEN("a multiplier with a fractional value over one half")
		{
			context.setMultiplier(toMultiplierID(BuiltinMultiplierID::Ability), 1.6);

			THEN("the multiplied damage rounds up")
			{
				CHECK((context.applyMultiplier(11, multiplierRegistry) == 18));
			}
		}

		GIVEN("a multiplier that produces an exact half fractional part")
		{
			context.setMultiplier(toMultiplierID(BuiltinMultiplierID::Ability), 1.5);

			THEN("the multiplied damage rounds down")
			{
				CHECK((context.applyMultiplier(11, multiplierRegistry) == 16));
			}
		}

		GIVEN("a non-finite multiplier")
		{
			context.setMultiplier(toMultiplierID(BuiltinMultiplierID::Ability), std::numeric_limits<double>::quiet_NaN());

			THEN("the multiplier is set to 1.0")
			{
				CHECK((context.applyMultiplier(10, multiplierRegistry) == 10));
			}
		}

		GIVEN("multiple multipliers")
		{
			context.setMultiplier(toMultiplierID(BuiltinMultiplierID::Ability), 1.8);
			context.setMultiplier(toMultiplierID(BuiltinMultiplierID::Critical), 2.3);
			context.setMultiplier(toMultiplierID(BuiltinMultiplierID::Targets), 0.8);
			context.setMultiplier(toMultiplierID(BuiltinMultiplierID::Item), 1.2);

			THEN("all multipliers are applied in order")
			{
				CHECK((context.applyMultiplier(14, multiplierRegistry) == 56));
			}
		}

		GIVEN("a fractional multiplier below one")
		{
			context.setMultiplier(toMultiplierID(BuiltinMultiplierID::Randomization), 0.85);

			THEN("damage is reduced by the multiplier")
			{
				CHECK((context.applyMultiplier(100, multiplierRegistry) == 85));
			}
		}

		GIVEN("a zero multiplier")
		{
			context.setMultiplier(toMultiplierID(BuiltinMultiplierID::Ability), 0.0);

			THEN("damage is clamped to at least one")
			{
				CHECK((context.applyMultiplier(0, multiplierRegistry) == 1));
			}
		}
	}

	GIVEN("resetMultipliers")
	{
		context.setMultiplier(toMultiplierID(BuiltinMultiplierID::Ability), 2.0);
		context.setMultiplier(toMultiplierID(BuiltinMultiplierID::Item), 2.0);

		WHEN("active multipliers are reset")
		{
			context.resetMultipliers();

			THEN("no multipliers remain")
			{
				CHECK(context.getActiveMultipliers().empty());
			}
		}
	}

	GIVEN("getActiveMultipliers")
	{
		context.setMultiplier(toMultiplierID(BuiltinMultiplierID::Ability), 2.0);
		context.setMultiplier(toMultiplierID(BuiltinMultiplierID::Item), 4.0);

		WHEN("the active multipliers are grabbed")
		{
			std::span<const std::pair<MultiplierID, double>> activeMultipliers{context.getActiveMultipliers()};

			THEN("the result matches whats in the context")
			{
				REQUIRE((activeMultipliers.size() == 2));
				CHECK((activeMultipliers.front().first == toMultiplierID(BuiltinMultiplierID::Ability)));
				CHECK((approximatelyEqualAbsRel(activeMultipliers.front().second, 2.0)));
				CHECK((activeMultipliers.at(1).first == toMultiplierID(BuiltinMultiplierID::Item)));
				CHECK((approximatelyEqualAbsRel(activeMultipliers.at(1).second, 4.0)));
			}
		}
	}

	WHEN("operator==")
	{
		GIVEN("two default constructed contexts")
		{
			EffectContext other{};

			THEN("they are equal")
			{
				CHECK((context == other));
			}
		}

		GIVEN("for mDamage modified in one context")
		{
			EffectContext other{makeEffectContext({.mDamage = {.mDamage = 1}})};

			THEN("they are not equal")
			{
				CHECK((context != other));
			}
		}

		GIVEN("for mMoveTypeID modified in one context")
		{
			EffectContext other{makeEffectContext({.mMoveTypeID = toTypeID(BuiltinTypeID::Fire)})};

			THEN("they are not equal")
			{
				CHECK((context != other));
			}
		}

		GIVEN("for mAbilityID modified in one context")
		{
			EffectContext other{makeEffectContext({.mAbilityID = toAbilityID(BuiltinAbilityID::Drizzle)})};

			THEN("they are not equal")
			{
				CHECK((context != other));
			}
		}

		GIVEN("for mItemID modified in one context")
		{
			EffectContext other{makeEffectContext({.mItemID = toItemID(BuiltinItemID::ChestoBerry)})};

			THEN("they are not equal")
			{
				CHECK((context != other));
			}
		}

		GIVEN("for mMoveID modified in one context")
		{
			EffectContext other{makeEffectContext({.mMoveID = toMoveID(BuiltinMoveID::HydroSteam)})};

			THEN("they are not equal")
			{
				CHECK((context != other));
			}
		}

		GIVEN("for mNatureID modified in one context")
		{
			EffectContext other{makeEffectContext({.mNatureID = toNatureID(BuiltinNatureID::Bashful)})};

			THEN("they are not equal")
			{
				CHECK((context != other));
			}
		}

		GIVEN("for mRangeOverride modified in one context")
		{
			EffectContext other{makeEffectContext({.mRangeOverride = BattleRangeID::Adjacent})};

			THEN("they are not equal")
			{
				CHECK((context != other));
			}
		}

		GIVEN("for mUserIndex modified in one context")
		{
			EffectContext other{makeEffectContext({.mUserIndex = 1})};

			THEN("they are not equal")
			{
				CHECK((context != other));
			}
		}

		GIVEN("for mTargetIndex modified in one context")
		{
			EffectContext other{makeEffectContext({.mTargetIndex = 1})};

			THEN("they are not equal")
			{
				CHECK((context != other));
			}
		}

		GIVEN("for mMoveBasePower modified in one context")
		{
			EffectContext other{makeEffectContext({.mMoveBasePower = 1})};

			THEN("they are not equal")
			{
				CHECK((context != other));
			}
		}

		GIVEN("for mMoveAccuracy modified in one context")
		{
			EffectContext other{makeEffectContext({.mMoveAccuracy = 1})};

			THEN("they are not equal")
			{
				CHECK((context != other));
			}
		}

		GIVEN("for mHitAttemptIndex modified in one context")
		{
			EffectContext other{makeEffectContext({.mHitAttemptIndex = 1})};

			THEN("they are not equal")
			{
				CHECK((context != other));
			}
		}

		GIVEN("for mSourceType modified in one context")
		{
			EffectContext other{makeEffectContext({.mSourceType = EffectSource::Hazard})};

			THEN("they are not equal")
			{
				CHECK((context != other));
			}
		}

		GIVEN("for mUserSide modified in one context")
		{
			EffectContext other{makeEffectContext({.mUserSide = Side::B})};

			THEN("they are not equal")
			{
				CHECK((context != other));
			}
		}

		GIVEN("for mTargetSide modified in one context")
		{
			EffectContext other{makeEffectContext({.mTargetSide = Side::B})};

			THEN("they are not equal")
			{
				CHECK((context != other));
			}
		}

		GIVEN("for mIsSpecial modified in one context")
		{
			EffectContext other{makeEffectContext({.mIsSpecial = true})};

			THEN("they are not equal")
			{
				CHECK((context != other));
			}
		}

		GIVEN("a multiplier is set in one context but not the other")
		{
			EffectContext other{};
			other.setMultiplier(toMultiplierID(BuiltinMultiplierID::Ability), 0.25);

			THEN("they are not equal")
			{
				CHECK((context != other));
			}
		}

		GIVEN("two contexts modified the same way")
		{
			EffectContext other{makeEffectContext({.mMoveAccuracy = 65})};
			context.mMoveAccuracy = 65;

			THEN("they are equal")
			{
				CHECK((context == other));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
