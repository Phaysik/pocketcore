/*! @file interaction.h
	@brief Defines reusable metadata interactions between objects of the same framework.
	@date 09/02/2026
	@since 0.12.16
	@version 0.12.16
	@author Matthew Moore
*/

#ifndef INCLUDE_CORE_INTERACTION_H
#define INCLUDE_CORE_INTERACTION_H

#include "Core/typedefs.h"

namespace PocketCore::Interaction
{
	using PocketCore::Core::ub;

	/*! @enum InteractionAction
		@showenumvalues
		@brief Identifies how an incoming object interacts with an existing object.
		@note Callers handling this enum should account for every value.
		@date 09/02/2026
		@since 0.12.16
		@version 0.12.16
		@author Matthew Moore
	*/
	enum class InteractionAction : ub
	{
		/*! @brief Allows the incoming and existing objects to remain active together. */
		Coexist,

		/*! @brief Replaces the existing object with the incoming object. */
		ReplaceCurrent,

		/*! @brief Removes the existing object before the incoming object is applied. */
		RemoveCurrent,

		/*! @brief Prevents the incoming object from being applied while the existing object is active. */
		BlockIncoming,
	};

	/*! @struct Interaction Core/interaction.h
		@brief Associates an existing object with the action taken when another object is applied.
		@tparam ID The stable identifier type shared by the incoming and existing objects.
		@date 09/02/2026
		@since 0.12.16
		@version 0.12.16
		@author Matthew Moore
	*/
	template <typename ID>
	struct Interaction
	{
		public:
			/*! @brief The stable identifier of the object that is already active. */
			ID mExistingID{};

			/*! @brief The action taken between the incoming object and the existing object. */
			InteractionAction mAction{InteractionAction::Coexist};
	};
} // namespace PocketCore::Interaction

#endif
