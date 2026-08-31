/*! @file natureID.h
	@brief Contains the nature status
	@date 08/22/2026
	@since 0.11.6
	@version 0.11.6
	@author Matthew Moore
*/

#ifndef INCLUDE_NATURE_NATUREID_H
#define INCLUDE_NATURE_NATUREID_H

#include "ID/idInterface.h"

namespace PocketCore::Nature
{
	namespace Detail
	{
		/*! @brief Distinguishes nature identifiers from all other stable identifier domains. */
		struct NatureIDTag;
	} // namespace Detail

	/*! @typedef NatureID
		@brief A strongly typed stable identifier for any registered nature.
		@details Values are assigned by the nature registry. Unlike @ref BuiltinNatureID, this type is open and can represent user-defined
	   abilities without extending an enum. Its tag prevents comparison or conversion with identifiers from other registry domains.
	*/
	using NatureID = PocketCore::ID::IDInterface<Detail::NatureIDTag, 0>;

	/*! @brief The stable identifier representing no nature. */
	inline constexpr NatureID NO_NATURE_ID{};
} // namespace PocketCore::Nature

#endif
