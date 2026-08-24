/*! @file testMain.cpp
	@brief C++ file for running all tests.
	@date 04/22/2026
	@since 0.1.0
	@version 0.1.4
	@author Matthew Moore
*/

#ifdef catch2
	#include <catch2/catch_session.hpp>
#elifdef googletest
	#include <gtest/gtest.h>
#else
	#error "No testing framework defined. Please define either catch2 or googletest."
#endif

int main(int argc, char *argv[])
{
#ifdef catch2
	return Catch::Session().run(argc, argv);
#elifdef googletest
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
#else
	#error "No testing framework defined. Please define either catch2 or googletest."
#endif
}
