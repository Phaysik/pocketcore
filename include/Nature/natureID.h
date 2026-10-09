/*! @file natureID.h
	@brief Defines the open identifier used for built-in and user-defined natures.
	@date 10/09/2026
	@since 0.11.6
	@version 0.12.51
	@author Matthew Moore
*/

#ifndef INCLUDE_NATURE_NATURE_ID_H
#define INCLUDE_NATURE_NATURE_ID_H

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
