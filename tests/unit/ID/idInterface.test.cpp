#include "ID/idInterface.h"

#include <type_traits>

#include "Core/typedefs.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Core::us;
using PocketCore::ID::IDInterface;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

namespace
{
	struct FirstIDTag;
	struct SecondIDTag;

	using FirstID = IDInterface<FirstIDTag, 0>;
	using SecondID = IDInterface<SecondIDTag, 255>;

	template <typename Left, typename Right>
	concept EqualityComparableWith = requires(Left left, Right right) { left == right; };

	static_assert(sizeof(FirstID) == sizeof(us));
	static_assert(sizeof(SecondID) == sizeof(us));
	static_assert(std::is_trivially_copyable_v<FirstID>);
	static_assert(std::is_trivially_copyable_v<SecondID>);
	static_assert(!EqualityComparableWith<FirstID, SecondID>);
} // namespace

SCENARIO("IDInterface")
{
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
			CHECK_FALSE((firstIdentifier == differentIdentifier));
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)