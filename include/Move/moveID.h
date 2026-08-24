/*! @file moveID.h
	@brief Contains the move status
	@date 07/26/2026
	@since 0.3.0
	@version 0.3.0
	@author Matthew Moore
*/

#ifndef INCLUDE_MOVE_MOVEID_H
#define INCLUDE_MOVE_MOVEID_H

#include "ID/idInterface.h"

namespace PocketCore::Move
{
	namespace Detail
	{
		/*! @brief Distinguishes move identifiers from all other stable identifier domains. */
		struct MoveIDTag;
	} // namespace Detail

	/*! @typedef MoveID
		@brief A strongly typed stable identifier for any registered move.
		@details Values are assigned by the move registry. Unlike @ref BuiltinMoveID, this type is open and can represent user-defined
	   abilities without extending an enum. Its tag prevents comparison or conversion with identifiers from other registry domains.
	*/
	using MoveID = PocketCore::ID::IDInterface<Detail::MoveIDTag, 0>;

	/*! @brief The stable identifier representing no move. */
	inline constexpr MoveID NO_MOVE_ID{};
} // namespace PocketCore::Move

#endif
