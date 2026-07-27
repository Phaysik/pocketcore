/*! @file statusID.h
	@brief Contains the status effects
	@date 07/24/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_STATUS_STATUSID_H
#define INCLUDE_STATUS_STATUSID_H

#include "Core/typedefs.h"

namespace PocketCore::Status
{
	enum class StatusID : PocketCore::Core::ub
	{
		None,
		Paralysis,
		Burn,
		Sleep,
		Freeze,
		Poison,
		Toxic
	};
} // namespace PocketCore::Status

#endif
