/*! @file idInterface.test.cpp
	@brief C++ file for running tests for the IDInterface.
	@date 08/31/2026
	@since 0.4.1
	@version 0.12.13
	@author Matthew Moore
*/

#include "ID/idInterface.h"

#include <type_traits>

#include "Core/typedefs.h"
#include "ID/idInterface.testHelper.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Core::us;
using PocketCore::ID::IDInterface;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

SCENARIO("IDInterface")
{
	struct FirstIDTag;
	struct SecondIDTag;

	using FirstID = IDInterface<FirstIDTag, 0>;
	using SecondID = IDInterface<SecondIDTag, 255>;
	using PocketCore::Testing::EqualityComparableWith;

	static_assert(sizeof(FirstID) == sizeof(us));
	static_assert(sizeof(SecondID) == sizeof(us));
	static_assert(std::is_trivially_copyable_v<FirstID>);
	static_assert(std::is_trivially_copyable_v<SecondID>);
	static_assert(!EqualityComparableWith<FirstID, SecondID>);

	GIVEN("two identifier domains with different defaults")
	{
		FirstID firstDefault{};
		SecondID secondDefault{};

		THEN("each domain preserves its configured default")
		{
			CHECK((firstDefault.getValue() == 0));
			CHECK((secondDefault.getValue() == 255));
		}
	}

	GIVEN("identifiers from the same domain")
	{
		FirstID firstIdentifier{42};
		FirstID matchingIdentifier{42};
		FirstID differentIdentifier{43};

		THEN("their values and equality remain strongly typed")
		{
			CHECK((firstIdentifier.getValue() == 42));
			CHECK((firstIdentifier == matchingIdentifier));
			CHECK((firstIdentifier < differentIdentifier));
		}

		THEN("check their equality by constructor")
		{
			CHECK((FirstID{42} == FirstID{42}));
			CHECK((FirstID{41} != FirstID{42}));

			CHECK((FirstID{41} < FirstID{42}));
			CHECK((FirstID{41} <= FirstID{42}));

			CHECK((FirstID{41} > FirstID{40}));
			CHECK((FirstID{41} >= FirstID{40}));
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
