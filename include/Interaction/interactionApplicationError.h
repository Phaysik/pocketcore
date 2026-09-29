/*! @file interactionApplicationError.h
	@brief Defines the structured error returned when an interaction cannot be applied.
	@details Provides the reasons an interaction application is rejected so callers receive a structured error rather than a silent drop.
	@date 09/29/2026
	@since 0.12.46
	@version 0.12.46
	@author Matthew Moore
*/

#ifndef INCLUDE_INTERACTION_INTERACTION_APPLICATION_ERROR_H
#define INCLUDE_INTERACTION_INTERACTION_APPLICATION_ERROR_H

#include "Core/typedefs.h"

namespace PocketCore::Interaction
{
	using PocketCore::Core::ub;

	/*! @enum InteractionApplicationError
		@showenumvalues
		@brief Identifies why an interaction could not be applied to a Pokemon or battle slot.
		@details Each value describes a distinct rejection reason surfaced when applying an interaction, allowing callers
	   to react programmatically instead of observing an unchanged interaction list. The enumerators are stored using the unsigned byte type
	   @ref PocketCore::Core::ub.
		@date 09/29/2026
		@since 0.12.46
		@version 0.12.46
		@author Matthew Moore
	*/
	enum class InteractionApplicationError : ub
	{
		/*! @brief Indicates the incoming interaction's classification does not match the target storage. */
		WrongClassification,
		/*! @brief Indicates an existing interaction blocked the incoming interaction. */
		Blocked,
		/*! @brief Indicates the interaction list is full and the incoming interaction was not permitted to replace an existing entry. */
		CapReached,
		/*! @brief Indicates the incoming interaction is already present. */
		Duplicate,
		/*! @brief Indicates the incoming interaction has missing metadata. */
		MissingMetadata,
		/*! @brief Indicates the incoming interaction ID is the same as the empty ID */
		SameAsEmptyID,
	};
} // namespace PocketCore::Interaction

#endif
