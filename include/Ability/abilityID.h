/*! @file abilityID.h
	@brief Defines the open identifier used for built-in and user-defined abilities.
	@date 07/24/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_ABILITY_ABILITYID_H
#define INCLUDE_ABILITY_ABILITYID_H

#include "ID/idInterface.h"

namespace PocketCore::Ability
{
	namespace Detail
	{
		/*! @brief Distinguishes ability identifiers from all other stable identifier domains. */
		struct AbilityIDTag;
	} // namespace Detail

	/*! @typedef AbilityID
		@brief A strongly typed stable identifier for any registered ability.
		@details Values are assigned by the ability registry. Unlike @ref BuiltinAbilityID, this type is open and can represent user-defined
	   abilities without extending an enum. Its tag prevents comparison or conversion with identifiers from other registry domains.
	*/
	using AbilityID = PocketCore::ID::IDInterface<Detail::AbilityIDTag, 0>;

	/*! @brief The stable identifier representing no ability. */
	inline constexpr AbilityID NO_ABILITY_ID{};
} // namespace PocketCore::Ability

#endif
