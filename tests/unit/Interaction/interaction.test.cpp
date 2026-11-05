/*! @file interaction.test.cpp
	@brief C++ file for running tests for the Interaction.
	@date 10/28/2026
	@since 0.12.22
	@version 0.12.53
	@author Matthew Moore
*/

#include "Interaction/interaction.h"

#include "ID/idInterface.h"
#include "Terrain/terrainID.h"

#include <catch2/catch_test_macros.hpp>

using PocketCore::ID::IDInterface;
using PocketCore::Interaction::Interaction;
using PocketCore::Interaction::InteractionAction;
using PocketCore::Terrain::TerrainID;

// NOLINTBEGIN(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

SCENARIO("Interaction")
{
	struct InteractionIDTag;
	using InteractionID = IDInterface<InteractionIDTag, 0>;

	Interaction<InteractionID> interaction{};

	WHEN("operator==")
	{
		GIVEN("two default constructed interactions")
		{
			Interaction<InteractionID> other{};

			THEN("they are equal")
			{
				CHECK((interaction == other));
			}
		}

		GIVEN("for mExistingID modified with fixed hit count in one interaction")
		{
			Interaction<InteractionID> other{.mExistingID = InteractionID{5}};

			THEN("they are not equal")
			{
				CHECK((interaction != other));
			}
		}

		GIVEN("for mAction modified with weighted hit count in one interaction")
		{
			Interaction<InteractionID> other{.mAction = InteractionAction::RemoveCurrent};

			THEN("they are not equal")
			{
				CHECK((interaction != other));
			}
		}

		GIVEN("two interactions modified the same way")
		{
			Interaction<InteractionID> other{.mAction = InteractionAction::BlockIncoming};
			interaction.mAction = InteractionAction::BlockIncoming;

			THEN("they are equal")
			{
				CHECK((interaction == other));
			}
		}

		GIVEN("interactions using terrain identifiers")
		{
			Interaction<TerrainID> terrainInteraction{.mExistingID = TerrainID{1}, .mAction = InteractionAction::ReplaceCurrent};
			Interaction<TerrainID> other{terrainInteraction};

			THEN("matching identifiers and actions compare equal")
			{
				CHECK((terrainInteraction == other));
			}

			WHEN("the terrain identifiers differ")
			{
				other.mExistingID = TerrainID{2};

				THEN("the interactions compare unequal")
				{
					CHECK((terrainInteraction != other));
				}
			}

			WHEN("the actions differ")
			{
				other.mAction = InteractionAction::RemoveCurrent;

				THEN("the interactions compare unequal")
				{
					CHECK((terrainInteraction != other));
				}
			}
		}
	}
}

// NOLINTEND(misc-const-correctness,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
