/*! @file abilityID.h
	@brief Contains the ability status
	@date 07/24/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_ABILITY_ABILITYID_H
#define INCLUDE_ABILITY_ABILITYID_H

#include "Core/typedefs.h"

namespace PocketCore::Ability
{
	enum class AbilityID : PocketCore::Core::ub
	{
		None,
		Stench,
		Drizzle
	};
} // namespace PocketCore::Ability

#endif
