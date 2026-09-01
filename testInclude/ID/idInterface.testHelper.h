/*! @file idInterface.testHelper.h
	@brief Test helper for dealing with idInterface concepts.
	@date 08/31/2026
	@since 0.12.13
	@version 0.12.13
	@author Matthew Moore
*/

#ifndef TEST_INCLUDE_ID_IDINTERFACE_TEST_HELPER_H
#define TEST_INCLUDE_ID_IDINTERFACE_TEST_HELPER_H

namespace PocketCore::Testing
{
	template <typename Left, typename Right>
	concept EqualityComparableWith = requires(Left left, Right right) { left == right; };
} // namespace PocketCore::Testing

#endif
