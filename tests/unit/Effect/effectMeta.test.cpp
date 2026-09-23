/*! @file effectMeta.test.cpp
	@brief C++ file for running tests for the EffectMeta.
	@date 09/23/2026
	@since 0.12.43
	@version 0.12.43
	@author Matthew Moore
*/

#include "Effect/effectMeta.h"

#include "Effect/builtInEffectID.h"
#include "Effect/effectID.h"
#include "EffectHandler/setRainHandler.h"
#include "Interaction/interaction.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Effect::applySetRain;
using PocketCore::Effect::BuiltinEffectID;
using PocketCore::Effect::EffectID;
using PocketCore::Effect::EffectMeta;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("EffectMeta")
{
	EffectMeta meta{};

	WHEN("operator==")
	{
		GIVEN("two default constructed metas")
		{
			EffectMeta other{};

			THEN("they are equal")
			{
				CHECK((meta == other));
			}
		}

		GIVEN("for mName modified in one meta")
		{
			EffectMeta other{.mName = "Test"};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mApply modified in one meta")
		{
			EffectMeta other{.mApply = applySetRain};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mEffectID modified in one meta")
		{
			EffectMeta other{.mEffectID = toEffectID(BuiltinEffectID::BaseDamage)};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mMayChangeWeather modified in one meta")
		{
			EffectMeta other{.mMayChangeWeather = true};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mMayChangeTerrain modified in one meta")
		{
			EffectMeta other{.mMayChangeTerrain = true};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("for mMayChangeStatus modified in one meta")
		{
			EffectMeta other{.mMayChangeStatus = true};

			THEN("they are not equal")
			{
				CHECK((meta != other));
			}
		}

		GIVEN("two metas modified the same way")
		{
			EffectMeta other{.mName = "Same Name"};
			meta.mName = "Same Name";

			THEN("they are equal")
			{
				CHECK((meta == other));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
