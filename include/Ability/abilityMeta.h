/*! @file abilityMeta.h
	@brief Defines the metadata stored for built-in and user-defined abilities.
	@date 08/12/2026
	@since 0.3.0
	@version 0.10.0
	@author Matthew Moore
*/

#ifndef INCLUDE_ABILITY_ABILITYMETA_H
#define INCLUDE_ABILITY_ABILITYMETA_H

#include <string_view>
#include <vector>

#include "Battle/battleTargetsAndTriggers.h"
#include "Effect/effectTrigger.h"

#include "abilityID.h"

namespace PocketCore::Ability
{
	using PocketCore::Battle::BattleEventID;
	using PocketCore::Battle::BattleEventRole;
	using PocketCore::Battle::BattleTargetID;
	using PocketCore::Effect::EffectTrigger;

	/*! @struct AbilityMeta Ability/abilityMeta.h
		@brief Stores one ability's stable ID, display name, and owned trigger definitions.
		@details The trigger vector owns its elements and their effect vectors. The display name is a non-owning view whose backing storage
	   must remain valid while this metadata is registered.
		@date 07/27/2026
		@since 0.3.0
		@version 0.5.3
		@author Matthew Moore
	*/
	struct AbilityMeta
	{
		public:
			/*! @brief The owned trigger and effect definitions for this ability. */
			std::vector<EffectTrigger> mTriggers;

			/*! @brief The case-sensitive display name stored as a non-owning view. */
			std::string_view mName{};

			/*! @brief The stable built-in or user-assigned identifier. */
			AbilityID mAbilityID{};

			/*! @brief The target ID for this ability's effects. */
			BattleTargetID mTargetID{};
	};
} // namespace PocketCore::Ability

#endif
