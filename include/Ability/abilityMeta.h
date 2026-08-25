/*! @file abilityMeta.h
	@brief Defines the metadata stored for built-in and user-defined abilities.
	@date 07/27/2026
	@since 0.3.0
	@version 0.4.0
	@author Matthew Moore
*/

#ifndef INCLUDE_ABILITY_ABILITYMETA_H
#define INCLUDE_ABILITY_ABILITYMETA_H

#include <string_view>
#include <vector>

#include "Battle/battleTargetsAndTriggers.h"
#include "Configuration/constants.h"
#include "Core/typedefs.h"
#include "Effect/effectID.h"
#include "Effect/effectSourceAndSuppresion.h"

#include "abilityID.h"

namespace PocketCore::Ability
{
	using PocketCore::Battle::BattleEventID;
	using PocketCore::Battle::BattleEventRole;
	using PocketCore::Battle::BattleTargetID;
	using PocketCore::Configuration::MAX_SUPPRESSION_RULES_PER_TRIGGER;
	using PocketCore::Core::ub;
	using PocketCore::Effect::EffectID;
	using PocketCore::Effect::SuppressionRule;

	/*! @struct AbilityEffectTrigger Ability/abilityMeta.h
		@brief Stores the event-driven effects and suppression rules for one ability trigger.
		@details The effect vector owns its ordered effect identifiers. The suppression-rule array has fixed physical storage, while
		 @p mSuppresionRuleCount identifies how many entries are active. The trigger is eligible when its event and role match the
		 current battle event.
		@note @p mSuppresionRuleCount must not exceed MAX_SUPPRESSION_RULES_PER_TRIGGER.
		@date 07/26/2026
		@since 3.0.0
		@version 3.0.0
		@author Matthew Moore
	*/
	struct AbilityEffectTrigger
	{
		public:
			/*! @brief The fixed-capacity suppression rules associated with this trigger. */
			std::array<SuppressionRule, MAX_SUPPRESSION_RULES_PER_TRIGGER> mSuppressionRules{};

			/*! @brief The owned effect identifiers executed in declaration order when this trigger matches. */
			std::vector<EffectID> mEffects{};

			/*! @brief The battle event that activates this trigger. */
			BattleEventID mTrigger{};

			/*! @brief The battle-event role required for activation, or Any when the role is unrestricted. */
			BattleEventRole mRole{BattleEventRole::Any};

			/*! @brief The number of entries in mSuppressionRules that contain active suppression rules. */
			ub mSuppresionRuleCount{0};
	};

	/*! @struct AbilityMeta Ability/abilityMeta.h
		@brief Stores one ability's stable ID, display name, and owned trigger definitions.
		@details The trigger vector owns its elements and their effect vectors. The display name is a non-owning view whose backing storage
	   must remain valid while this metadata is registered.
		@date 07/27/2026
		@since 0.3.0
		@version 0.4.0
		@author Matthew Moore
	*/
	struct AbilityMeta
	{
		public:
			/*! @brief The owned trigger and effect definitions for this ability. */
			std::vector<AbilityEffectTrigger> mTriggers;

			/*! @brief The case-sensitive display name stored as a non-owning view. */
			std::string_view mName{};

			/*! @brief The stable built-in or user-assigned identifier. */
			AbilityID mAbilityID{};

			/*! @brief The target ID for this ability's effects. */
			BattleTargetID mTargetID{};
	};
} // namespace PocketCore::Ability

#endif
