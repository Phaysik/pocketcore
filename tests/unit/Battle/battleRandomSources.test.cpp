/*! @file battleRandomSources.test.cpp
	@brief C++ file for running tests for the BattleRandomSources.
	@date 10/19/2026
	@since 0.12.51
	@version 0.12.51
	@author Matthew Moore
*/

#include "Battle/battleRandomSources.h"

#include <cstddef>

#include "Core/typedefs.h"
#include "Utility/Random/callableRandomSource.h"
#include "Utility/Random/defaultRandomSource.h"
#include "Utility/Random/random.testHelper.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

using PocketCore::Battle::BattleRandomSources;
using PocketCore::Core::sb;
using PocketCore::Core::ub;
using PocketCore::Testing::SequenceRandom;
using PocketCore::Utility::Random::CallableRandomSource;
using PocketCore::Utility::Random::getDefaultRandomSource;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("BattleRandomSources")
{
	GIVEN("a default-constructed bundle")
	{
		BattleRandomSources sources{};

		THEN("every slot references the default source for its numeric type")
		{
			CHECK((&sources.mUnsigned.get() == &getDefaultRandomSource<ub>()));
			CHECK((&sources.mSigned.get() == &getDefaultRandomSource<sb>()));
			CHECK((&sources.mReal.get() == &getDefaultRandomSource<double>()));
			CHECK((&sources.mIndex.get() == &getDefaultRandomSource<std::size_t>()));
		}

		THEN("another default bundle references the same source instances")
		{
			BattleRandomSources other{};

			CHECK((&sources.mUnsigned.get() == &other.mUnsigned.get()));
			CHECK((&sources.mSigned.get() == &other.mSigned.get()));
			CHECK((&sources.mReal.get() == &other.mReal.get()));
			CHECK((&sources.mIndex.get() == &other.mIndex.get()));
		}
	}

	GIVEN("a custom source supplied for only the real-valued slot")
	{
		CallableRandomSource<double, SequenceRandom<double>> realSource{SequenceRandom<double>{0.25}};
		BattleRandomSources sources{.mReal = realSource};

		THEN("the custom slot is bound while omitted slots retain their defaults")
		{
			CHECK((&sources.mUnsigned.get() == &getDefaultRandomSource<ub>()));
			CHECK((&sources.mSigned.get() == &getDefaultRandomSource<sb>()));
			CHECK((&sources.mReal.get() == &realSource));
			CHECK((&sources.mIndex.get() == &getDefaultRandomSource<std::size_t>()));
			CHECK_THAT(sources.mReal(0.0, 1.0), Catch::Matchers::WithinAbs(0.25, 0.0));
		}
	}

	GIVEN("custom sources for every numeric type")
	{
		CallableRandomSource<ub, SequenceRandom<ub>> unsignedSource{SequenceRandom<ub>{2, 3}};
		CallableRandomSource<sb, SequenceRandom<sb>> signedSource{SequenceRandom<sb>{-2, -3}};
		CallableRandomSource<double, SequenceRandom<double>> realSource{SequenceRandom<double>{0.25, 0.75}};
		CallableRandomSource<std::size_t, SequenceRandom<std::size_t>> indexSource{SequenceRandom<std::size_t>{4, 5}};
		BattleRandomSources sources{.mUnsigned = unsignedSource, .mSigned = signedSource, .mReal = realSource, .mIndex = indexSource};

		THEN("each slot references the supplied source")
		{
			CHECK((&sources.mUnsigned.get() == &unsignedSource));
			CHECK((&sources.mSigned.get() == &signedSource));
			CHECK((&sources.mReal.get() == &realSource));
			CHECK((&sources.mIndex.get() == &indexSource));
		}

		THEN("each slot invokes the corresponding custom source")
		{
			CHECK((sources.mUnsigned(0, 10) == 2));
			CHECK((sources.mSigned(-10, 10) == -2));
			CHECK_THAT(sources.mReal(0.0, 1.0), Catch::Matchers::WithinAbs(0.25, 0.0));
			CHECK((sources.mIndex(0, 10) == 4));
		}

		WHEN("the bundle is copy-constructed")
		{
			BattleRandomSources copied{sources};

			THEN("the copy references the same sources and shares their state")
			{
				CHECK((&copied.mUnsigned.get() == &unsignedSource));
				CHECK((&copied.mSigned.get() == &signedSource));
				CHECK((&copied.mReal.get() == &realSource));
				CHECK((&copied.mIndex.get() == &indexSource));

				CHECK((sources.mUnsigned(0, 10) == 2));
				CHECK((copied.mUnsigned(0, 10) == 3));
				CHECK((sources.mSigned(-10, 10) == -2));
				CHECK((copied.mSigned(-10, 10) == -3));
				CHECK_THAT(sources.mReal(0.0, 1.0), Catch::Matchers::WithinAbs(0.25, 0.0));
				CHECK_THAT(copied.mReal(0.0, 1.0), Catch::Matchers::WithinAbs(0.75, 0.0));
				CHECK((sources.mIndex(0, 10) == 4));
				CHECK((copied.mIndex(0, 10) == 5));
			}
		}

		WHEN("the bundle is assigned to a default-constructed bundle")
		{
			BattleRandomSources assigned{};
			assigned = sources;

			THEN("assignment replaces every default binding with the supplied references")
			{
				CHECK((&assigned.mUnsigned.get() == &unsignedSource));
				CHECK((&assigned.mSigned.get() == &signedSource));
				CHECK((&assigned.mReal.get() == &realSource));
				CHECK((&assigned.mIndex.get() == &indexSource));
			}
		}

		WHEN("each slot of a copied bundle is rebound to its default source")
		{
			BattleRandomSources copied{sources};
			copied.mUnsigned = getDefaultRandomSource<ub>();
			copied.mSigned = getDefaultRandomSource<sb>();
			copied.mReal = getDefaultRandomSource<double>();
			copied.mIndex = getDefaultRandomSource<std::size_t>();

			THEN("the copy uses the new bindings without changing the original bundle")
			{
				CHECK((&copied.mUnsigned.get() == &getDefaultRandomSource<ub>()));
				CHECK((&copied.mSigned.get() == &getDefaultRandomSource<sb>()));
				CHECK((&copied.mReal.get() == &getDefaultRandomSource<double>()));
				CHECK((&copied.mIndex.get() == &getDefaultRandomSource<std::size_t>()));
				CHECK((&sources.mUnsigned.get() == &unsignedSource));
				CHECK((&sources.mSigned.get() == &signedSource));
				CHECK((&sources.mReal.get() == &realSource));
				CHECK((&sources.mIndex.get() == &indexSource));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
