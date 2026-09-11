/*! @file interactionHelpers.testHelper.h
	@brief Test helper for dealing with the free functions that handle Interaction management.
	@date 09/11/2026
	@since 0.12.22
	@version 0.12.22
	@author Matthew Moore
*/

#ifndef TEST_INCLUDE_INTERACTION_INTERACTION_HELPERS_TEST_HELPER_H
#define TEST_INCLUDE_INTERACTION_INTERACTION_HELPERS_TEST_HELPER_H
#include <array>

#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Interaction/interaction.h"

namespace PocketCore::Testing
{
	using PocketCore::Core::si;
	using PocketCore::Interaction::Interaction;

	inline constexpr si TEST_INCOMING_ID{9};

	struct TestMetadata
	{
			std::array<Interaction<si>, 4> mInteractions{};
	};

	class TestRegistry
	{
		public:
			explicit constexpr TestRegistry(const TestMetadata &metadata) : mMetadata{metadata} {}

			ATTR_NODISCARD constexpr const TestMetadata *getMetadata(const si identifier) const
			{
				return identifier == TEST_INCOMING_ID ? &mMetadata : nullptr;
			}

		private:
			TestMetadata mMetadata{};
	};

} // namespace PocketCore::Testing
#endif
