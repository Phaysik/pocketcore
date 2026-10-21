/*! @file multiplierID.h
	@brief Defines the open identifier used for built-in and user-defined multipliers.
	@date 10/09/2026
	@since 0.8.1
	@version 0.12.51
	@author Matthew Moore
*/

#ifndef INCLUDE_MULTIPLIER_MULTIPLIER_ID_H
#define INCLUDE_MULTIPLIER_MULTIPLIER_ID_H

#include "ID/idInterface.h"

namespace PocketCore::Multiplier
{
	namespace Detail
	{
		/*! @brief Distinguishes multiplier identifiers from all other stable identifier domains. */
		struct MultiplierIDTag;
	} // namespace Detail

	/*! @typedef MultiplierID
		@brief A strongly typed stable identifier for any registered multiplier.
		@details Values are assigned by the multiplier registry. Unlike @ref BuiltinMultiplierID, this type is open and can represent
	   user-defined abilities without extending an enum. Its tag prevents comparison or conversion with identifiers from other registry
	   domains.
	*/
	using MultiplierID = PocketCore::ID::IDInterface<Detail::MultiplierIDTag, 0>;

	/*! @brief The stable identifier representing no multiplier. */
	inline constexpr MultiplierID NO_MULTIPLIER_ID{};
} // namespace PocketCore::Multiplier

#endif
