/*! @file effectID.h
	@brief Defines the open identifier used for built-in and user-defined abilities.
	@date 07/24/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_EFFECT_EFFECTID_H
#define INCLUDE_EFFECT_EFFECTID_H

#include "ID/idInterface.h"

namespace PocketCore::Effect
{
	namespace Detail
	{
		/*! @brief Distinguishes effect identifiers from all other stable identifier domains. */
		struct EffectIDTag;
	} // namespace Detail

	/*! @typedef EffectID
		@brief A strongly typed stable identifier for any registered effect.
		@details Values are assigned by the effect registry. Unlike @ref BuiltinEffectID, this type is open and can represent user-defined
	   abilities without extending an enum. Its tag prevents comparison or conversion with identifiers from other registry domains.
	*/
	using EffectID = PocketCore::ID::IDInterface<Detail::EffectIDTag, 0>;

	/*! @brief The stable identifier representing no effect. */
	inline constexpr EffectID NO_EFFECT_ID{};
} // namespace PocketCore::Effect

#endif
