/*! @file interactionHelpers.test.cpp
	@brief C++ file for running tests for the Interaction application functions.
	@date 09/22/2026
	@since 0.12.22
	@version 0.12.41
	@author Matthew Moore
*/

#include "Interaction/interactionHelpers.h"

#include <array>
#include <cstddef>

#include "Core/typedefs.h"
#include "Interaction/interactionHelpers.testHelper.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::Interaction::applyInteractions;
using PocketCore::Interaction::hasInteraction;
using PocketCore::Interaction::Interaction;
using PocketCore::Interaction::InteractionAction;
using PocketCore::Interaction::removeCurrent;
using PocketCore::Interaction::replaceCurrent;
using PocketCore::Interaction::shiftAndGetNextAvailable;
using PocketCore::Interaction::willBlockIncoming;
using PocketCore::Testing::TEST_INCOMING_ID;
using PocketCore::Testing::TestMetadata;
using PocketCore::Testing::TestRegistry;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)

SCENARIO("Interaction Helpers")
{
	using PocketCore::Core::si;

	std::array<Interaction<si>, 4> interactions{
		std::array{
			Interaction<si>{.mExistingID = 1, .mAction = InteractionAction::BlockIncoming},
			Interaction<si>{.mExistingID = 2, .mAction = InteractionAction::ReplaceCurrent},
			Interaction<si>{.mExistingID = 3, .mAction = InteractionAction::ReplaceCurrent},
			Interaction<si>{.mExistingID = 4, .mAction = InteractionAction::RemoveCurrent},
		},
	};

	GIVEN("hasInteraction")
	{
		WHEN("querying for an interaction where the ID isn't in the array")
		{
			bool result{hasInteraction(5, InteractionAction::BlockIncoming, interactions)};

			THEN("it returns false")
			{
				CHECK_FALSE(result);
			}
		}

		WHEN("querying for an interaction where the action isn't in the array")
		{
			bool result{hasInteraction(1, InteractionAction::Coexist, interactions)};

			THEN("it returns false")
			{
				CHECK_FALSE(result);
			}
		}

		WHEN("querying for an interaction where the ID and the action is in the array")
		{
			bool result{hasInteraction(1, InteractionAction::BlockIncoming, interactions)};

			THEN("it returns true")
			{
				CHECK(result);
			}
		}
	}

	GIVEN("willBlockIncoming")
	{
		WHEN("querying with an ID that isn't in the array")
		{
			std::array<si, 3> IDs{9, 7, 8};

			bool result{willBlockIncoming(IDs, interactions)};

			THEN("it returns false")
			{
				CHECK_FALSE(result);
			}
		}

		WHEN("querying with an ID that is in the array")
		{
			std::array<si, 3> IDs{1, 7, 8};

			bool result{willBlockIncoming(IDs, interactions)};

			THEN("it returns true")
			{
				CHECK(result);
			}
		}
	}

	GIVEN("replaceCurrent")
	{
		WHEN("querying with an ID that isn't in the array")
		{
			std::array<si, 3> IDs{9, 7, 8};

			bool result{replaceCurrent(5, 0, IDs, interactions)};

			THEN("it returns false and the array is not updated")
			{
				CHECK_FALSE(result);
				CHECK((IDs == std::array{9, 7, 8}));
			}
		}

		WHEN("querying with a singular ID that is in the array")
		{
			std::array<si, 3> IDs{2, 7, 8};

			bool result{replaceCurrent(5, 0, IDs, interactions)};

			THEN("it returns true and the array is updated")
			{
				CHECK(result);
				CHECK((IDs == std::array{5, 7, 8}));
			}
		}

		WHEN("querying with multiple IDs that are in the array")
		{
			std::array<si, 3> IDs{2, 7, 3};

			bool result{replaceCurrent(5, 0, IDs, interactions)};

			THEN("it returns true and the array is updated")
			{
				CHECK(result);
				CHECK((IDs == std::array{5, 7, 0}));
			}
		}
	}

	GIVEN("removeCurrent")
	{
		WHEN("querying with an ID that isn't in the array")
		{
			std::array<si, 3> IDs{9, 7, 8};

			removeCurrent(0, IDs, interactions);

			THEN("the array is not updated")
			{
				CHECK((IDs == std::array{9, 7, 8}));
			}
		}

		WHEN("querying with an ID that is in the array")
		{
			std::array<si, 3> IDs{2, 7, 4};

			removeCurrent(0, IDs, interactions);

			THEN("the array is updated")
			{
				CHECK((IDs == std::array{2, 7, 0}));
			}
		}
	}

	GIVEN("shiftAndGetNextAvailable")
	{
		WHEN("calling with an array that can't be compacted")
		{
			std::array<si, 3> IDs{2, 7, 4};
			std::size_t nextAvailable{shiftAndGetNextAvailable(0, IDs)};

			THEN("the array is unchanged and the next available index is the length of the array")
			{
				CHECK((nextAvailable == IDs.size()));
				CHECK((IDs == std::array{2, 7, 4}));
			}
		}
		WHEN("")
		{
			std::array<si, 5> IDs{1, 0, 2, 0, 3};
			std::size_t nextAvailable{shiftAndGetNextAvailable(0, IDs)};

			THEN("the array is changed and the next available index is the set the earliest empty id position")
			{
				CHECK((nextAvailable == 3));
				CHECK((IDs == std::array{1, 2, 3, 0, 0}));
			}
		}
	}

	GIVEN("applyInteractions")
	{
		TestRegistry interactionRegistry{TestRegistry{TestMetadata{
			.mInteractions = {
				Interaction<si>{.mExistingID = 1, .mAction = InteractionAction::RemoveCurrent},
				Interaction<si>{.mExistingID = 2, .mAction = InteractionAction::ReplaceCurrent},
				Interaction<si>{.mExistingID = 4, .mAction = InteractionAction::BlockIncoming},
			},
		}},};
		TestRegistry insertionRegistry{TestRegistry{TestMetadata{}}};

		WHEN("the incoming ID is empty or already active")
		{
			std::array<si, 4> IDs{1, 2, 3, 0};
			applyInteractions(0, 0, interactionRegistry, IDs, &TestMetadata::mInteractions, 4, false);
			applyInteractions(2, 0, interactionRegistry, IDs, &TestMetadata::mInteractions, 4, false);

			THEN("the active IDs are unchanged")
			{
				CHECK((IDs == std::array{1, 2, 3, 0}));
			}
		}

		WHEN("the incoming ID is unregistered")
		{
			std::array<si, 4> IDs{1, 2, 3, 0};
			applyInteractions(15, 0, interactionRegistry, IDs, &TestMetadata::mInteractions, 4, false);

			THEN("the active IDs remain unchanged")
			{
				CHECK((IDs == std::array{1, 2, 3, 0}));
			}
		}

		WHEN("an active ID blocks the incoming ID")
		{
			std::array<si, 4> IDs{1, 2, 3, 0};
			applyInteractions(4, 0, interactionRegistry, IDs, &TestMetadata::mInteractions, 4, false);

			THEN("the active IDs remain unchanged")
			{
				CHECK((IDs == std::array{1, 2, 3, 0}));
			}
		}

		WHEN("replacement and removal interactions apply")
		{
			std::array<si, 4> IDs{1, 2, 3, 0};
			applyInteractions(TEST_INCOMING_ID, 0, interactionRegistry, IDs, &TestMetadata::mInteractions, 4, false);

			THEN("the incoming ID replaces the targeted ID and removed IDs are compacted")
			{
				CHECK((IDs == std::array{TEST_INCOMING_ID, 3, 0, 0}));
			}
		}

		WHEN("the active count is zero")
		{
			std::array<si, 4> IDs{1, 0, 0, 0};
			applyInteractions(TEST_INCOMING_ID, 0, insertionRegistry, IDs, &TestMetadata::mInteractions, 0, false);

			THEN("the incoming ID is not inserted")
			{
				CHECK((IDs == std::array{1, 0, 0, 0}));
			}
		}

		WHEN("the active count reaches a one-entry cap")
		{
			std::array<si, 4> IDs{1, 0, 0, 0};
			applyInteractions(TEST_INCOMING_ID, 0, insertionRegistry, IDs, &TestMetadata::mInteractions, 1, false);

			THEN("the incoming ID is not inserted")
			{
				CHECK((IDs == std::array{1, 0, 0, 0}));
			}
		}

		WHEN("the active count is below the array-size cap")
		{
			std::array<si, 4> IDs{1, 2, 3, 0};
			applyInteractions(TEST_INCOMING_ID, 0, insertionRegistry, IDs, &TestMetadata::mInteractions, 4, false);

			THEN("the incoming ID is inserted into the next available position")
			{
				CHECK((IDs == std::array{1, 2, 3, TEST_INCOMING_ID}));
			}
		}

		WHEN("replaceWhenFull is enabled with capacity below the cap")
		{
			std::array<si, 4> IDs{1, 0, 0, 0};
			applyInteractions(TEST_INCOMING_ID, 0, insertionRegistry, IDs, &TestMetadata::mInteractions, 2, true);

			THEN("the incoming ID is inserted into the next available position")
			{
				CHECK((IDs == std::array{1, TEST_INCOMING_ID, 0, 0}));
			}
		}

		WHEN("replaceWhenFull is enabled at the cap")
		{
			std::array<si, 4> IDs{1, 2, 0, 0};
			applyInteractions(TEST_INCOMING_ID, 0, insertionRegistry, IDs, &TestMetadata::mInteractions, 2, true);

			THEN("the oldest active ID is replaced")
			{
				CHECK((IDs == std::array{TEST_INCOMING_ID, 2, 0, 0}));
			}
		}

		WHEN("replaceWhenFull is disabled at the cap")
		{
			std::array<si, 4> IDs{1, 2, 0, 0};
			applyInteractions(TEST_INCOMING_ID, 0, insertionRegistry, IDs, &TestMetadata::mInteractions, 2, false);

			THEN("the active IDs remain unchanged")
			{
				CHECK((IDs == std::array{1, 2, 0, 0}));
			}
		}

		WHEN("the cap is larger than the physical array")
		{
			std::array<si, 4> IDs{1, 2, 3, 4};
			applyInteractions(TEST_INCOMING_ID, 0, insertionRegistry, IDs, &TestMetadata::mInteractions, 5, false);

			THEN("the incoming ID is rejected without exceeding the physical array extent")
			{
				CHECK((IDs == std::array{1, 2, 3, 4}));
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,readability-function-cognitive-complexity)
