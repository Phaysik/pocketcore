/*! @file eggGroupID.h
	@brief Defines the open identifier used for built-in and user-defined egg groups.
	@date 10/26/2026
	@since 0.12.53
	@version 0.12.53
	@author Matthew Moore
*/

#ifndef INCLUDE_EGGGROUP_EGGGROUP_ID_H
#define INCLUDE_EGGGROUP_EGGGROUP_ID_H

#include "ID/idInterface.h"

namespace PocketCore::EggGroup
{
	namespace Detail
	{
		/*! @brief Distinguishes egg group identifiers from all other stable identifier domains. */
		struct EggGroupIDTag;
	} // namespace Detail

	/*! @typedef EggGroupID
		@brief A strongly typed stable identifier for any registered egg group.
		@details Values are assigned by the egg group registry. Unlike @ref BuiltinEggGroupID, this type is open and can represent
	   user-defined egg groups without extending an enum. Its tag prevents comparison or conversion with identifiers from other registry
	   domains.
	*/
	using EggGroupID = PocketCore::ID::IDInterface<Detail::EggGroupIDTag, 0>;
} // namespace PocketCore::EggGroup

#endif
