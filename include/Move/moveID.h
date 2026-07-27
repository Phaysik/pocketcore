/*! @file moveID.h
	@brief Contains the move status
	@date 07/24/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_MOVE_MOVEID_H
#define INCLUDE_MOVE_MOVEID_H

#include "Core/typedefs.h"

namespace PocketCore::Move
{
	enum class MoveID : PocketCore::Core::ub
	{
		None,
		Pound,
		KarateChop,
	};
} // namespace PocketCore::Move

#endif
