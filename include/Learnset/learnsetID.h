/*! @file learnsetID.h
	@brief Contains the learnset status
	@date 09/11/2026
	@since 0.12.24
	@version 0.12.24
	@author Matthew Moore
*/

#ifndef INCLUDE_LEARNSET_LEARNSETID_H
#define INCLUDE_LEARNSET_LEARNSETID_H

#include "ID/idInterface.h"

namespace PocketCore::Learnset
{
	namespace Detail
	{
		/*! @brief Distinguishes learnset identifiers from all other stable identifier domains. */
		struct LearnsetIDTag;
	} // namespace Detail

	/*! @typedef LearnsetID
		@brief A strongly typed stable identifier for any registered learnset.
		@details Values are assigned by the learnset registry. Unlike @ref BuiltinLearnsetID, this type is open and can represent
	   user-defined abilities without extending an enum. Its tag prevents comparison or conversion with identifiers from other registry
	   domains.
	*/
	using LearnsetID = PocketCore::ID::IDInterface<Detail::LearnsetIDTag, 0>;

	/*! @brief The stable identifier representing no learnset. */
	inline constexpr LearnsetID NO_LEARNSET_ID{};
} // namespace PocketCore::Learnset

#endif
