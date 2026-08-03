#include "Effect/effectContext.h"

#include <cmath>

#include "Multiplier/builtInMultiplierID.h"
#include "Multiplier/multiplierID.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Effect::EffectContext;
using PocketCore::Multiplier::BuiltinMultiplierID;
using PocketCore::Multiplier::MultiplierID;
using PocketCore::Multiplier::toMultiplierID;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

SCENARIO("EffectContext setMultiplier")
{
	EffectContext context{};

	GIVEN("an empty multiplier context")
	{
		auto abilityMultiplierID{toMultiplierID(BuiltinMultiplierID::Ability)};

		WHEN("a multiplier is added")
		{
			context.setMultiplier(abilityMultiplierID, 2.0);
			const auto activeMultipliers{context.getActiveMultipliers()};

			THEN("the multiplier list stores one entry")
			{
				REQUIRE((activeMultipliers.size() == 1U));
				CHECK((activeMultipliers.front().first == abilityMultiplierID));
				CHECK((std::fabs(activeMultipliers.front().second - 2.0) < 1e-6));
			}
		}

		WHEN("the same multiplier is set again")
		{
			context.setMultiplier(abilityMultiplierID, 2.0);
			context.setMultiplier(abilityMultiplierID, 3.0);
			const auto activeMultipliers{context.getActiveMultipliers()};

			THEN("the existing entry is updated without duplication")
			{
				REQUIRE((activeMultipliers.size() == 1U));
				CHECK((std::fabs(activeMultipliers.front().second - 3.0) < 1e-6));
			}
		}
	}

	GIVEN("an empty multiplier context with a non-builtin ID")
	{
		MultiplierID customID{7U};

		WHEN("a non-builtin multiplier is added")
		{
			context.setMultiplier(customID, 2.0);
			const auto activeMultipliers{context.getActiveMultipliers()};

			THEN("the multiplier list stores one entry")
			{
				REQUIRE((activeMultipliers.size() == 1U));
				CHECK((activeMultipliers.front().first == customID));
				CHECK((std::fabs(activeMultipliers.front().second - 2.0) < 1e-6));
			}
		}

		WHEN("the same non-builtin multiplier is set again")
		{
			context.setMultiplier(customID, 2.0);
			context.setMultiplier(customID, 3.0);
			const auto activeMultipliers{context.getActiveMultipliers()};

			THEN("the existing entry is updated without duplication")
			{
				REQUIRE((activeMultipliers.size() == 1U));
				CHECK((std::fabs(activeMultipliers.front().second - 3.0) < 1e-6));
			}
		}
	}
}

SCENARIO("EffectContext applyMultiplier")
{
	GIVEN("no active multipliers")
	{
		EffectContext context{};

		THEN("base damage is unchanged")
		{
			CHECK((context.applyMultiplier(37U) == 37U));
		}
	}

	GIVEN("a multiplier with a fractional value over one half")
	{
		EffectContext context{};
		context.setMultiplier(toMultiplierID(BuiltinMultiplierID::Ability), 1.6);

		THEN("the multiplied damage rounds up")
		{
			CHECK((context.applyMultiplier(11U) == 18U));
		}
	}

	GIVEN("a multiplier that produces an exact half fractional part")
	{
		EffectContext context{};
		context.setMultiplier(toMultiplierID(BuiltinMultiplierID::Ability), 1.5);

		THEN("the multiplied damage rounds down")
		{
			CHECK((context.applyMultiplier(11U) == 16U));
		}
	}

	GIVEN("multiple multipliers")
	{
		EffectContext context{};
		context.setMultiplier(toMultiplierID(BuiltinMultiplierID::Ability), 2.0);
		context.setMultiplier(toMultiplierID(BuiltinMultiplierID::Item), 2.0);

		THEN("all multipliers are applied in order")
		{
			CHECK((context.applyMultiplier(5U) == 20U));
		}
	}

	GIVEN("a zero multiplier")
	{
		EffectContext context{};
		context.setMultiplier(toMultiplierID(BuiltinMultiplierID::Ability), 0.0);

		THEN("damage is clamped to at least one")
		{
			CHECK((context.applyMultiplier(999U) == 1U));
		}
	}
}

SCENARIO("EffectContext resetMultipliers")
{
	EffectContext context{};
	context.setMultiplier(toMultiplierID(BuiltinMultiplierID::Ability), 2.0);
	context.setMultiplier(toMultiplierID(BuiltinMultiplierID::Item), 2.0);
	REQUIRE((context.getActiveMultipliers().size() == 2U));

	WHEN("active multipliers are reset")
	{
		context.resetMultipliers();

		THEN("no multipliers remain and calculations revert to base damage")
		{
			CHECK(context.getActiveMultipliers().empty());
			CHECK((context.applyMultiplier(15U) == 15U));
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
