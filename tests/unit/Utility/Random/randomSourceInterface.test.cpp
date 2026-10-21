/*! @file randomSourceInterface.test.cpp
	@brief C++ file for running tests for the IRandomSource.
	@date 10/19/2026
	@since 0.12.51
	@version 0.12.51
	@author Matthew Moore
*/

#include "Utility/Random/randomSourceInterface.h"

#include <memory>

#include "Core/concepts.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Core::FloatingPoint;
using PocketCore::Core::Integral;
using PocketCore::Utility::Random::IRandomSource;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
namespace
{
	template <typename T>
		requires Integral<T> || FloatingPoint<T>
	class NoopRandomSource final : public IRandomSource<T>
	{
		public:
			T operator()(const T min, const T /* max */) override
			{
				return min;
			}
	};
} // namespace

SCENARIO("IRandomSource")
{
	GIVEN("a dynamically allocated concrete handler")
	{
		std::unique_ptr<IRandomSource<int>> handler{std::make_unique<NoopRandomSource<int>>()};

		THEN("it returns appropraitely when called")
		{
			CHECK((*handler)(2, 3) == 2);
		}

		THEN("deletion through interface pointer invokes virtual deleting destructor")
		{
			handler.reset();
			CHECK((handler == nullptr));
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
