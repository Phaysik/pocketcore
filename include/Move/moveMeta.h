/*! @file moveMeta.h
	@brief Defines the metadata stored for built-in and user-defined moves.
	@date 07/24/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_MOVE_MOVEMETA_H
#define INCLUDE_MOVE_MOVEMETA_H

#include <string_view>
#include <vector>

#include "Core/typedefs.h"

#include "moveID.h"
#include "moveTargetsAndTriggers.h"

namespace PocketCore::Move
{
	using PocketCore::Core::us;

	/*! @struct MoveMeta Move/moveMeta.h
		@brief Stores one move's stable ID, display name, and owned trigger definitions.
		@details The trigger vector owns its elements and their effect vectors. The display name is a non-owning view whose backing storage
	   must remain valid while this metadata is registered.
		@date 07/27/2026
		@version x.x.x
		@since x.x.x
		@author Matthew Moore
	*/
	struct MoveMeta
	{
		public:
			/*! @brief The owned trigger and effect definitions for this move. */
			std::vector<MoveEffectTrigger> mTriggers;

			/*! @brief The case-sensitive display name stored as a non-owning view. */
			std::string_view mName{};

			/*! @brief The stable built-in or user-assigned identifier. */
			MoveID mMoveID{};
	};
} // namespace PocketCore::Move

#endif
