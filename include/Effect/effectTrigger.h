/*! @file effectTrigger.h
	@brief Defines shared event-driven effect trigger metadata.
	@date 08/26/2026
	@since 0.5.2
	@version 0.5.2
	@author Matthew Moore
*/

#ifndef INCLUDE_EFFECT_EFFECTTRIGGER_H
#define INCLUDE_EFFECT_EFFECTTRIGGER_H

#include <array>
#include <vector>

#include "Battle/battleTargetsAndTriggers.h"
#include "Configuration/constants.h"
#include "Core/typedefs.h"
#include "Effect/effectID.h"
#include "Effect/effectSourceAndSuppresion.h"

namespace PocketCore::Effect
{
	using PocketCore::Battle::BattleEventID;
	using PocketCore::Battle::BattleEventRole;
	using PocketCore::Configuration::MAX_SUPPRESSION_RULES_PER_TRIGGER;
	using PocketCore::Core::ub;

	/*! @struct EffectTrigger Effect/effectTrigger.h
		@brief Stores the effects and suppression rules activated by one battle event.
		@details The effect vector owns its ordered effect identifiers. The suppression-rule array has fixed physical storage, while
		 @p mSuppresionRuleCount identifies how many entries are active. The trigger is eligible when its event and role match the
		 current battle event.
		@note @p mSuppresionRuleCount must not exceed @ref MAX_SUPPRESSION_RULES_PER_TRIGGER.
		@date 08/26/2026
		@since 0.5.2
		@version 0.5.2
		@author Matthew Moore
	*/
	struct EffectTrigger
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

			/*! @brief The number of entries in @ref mSuppressionRules that contain active suppression rules. */
			ub mSuppresionRuleCount{0};
	};
} // namespace PocketCore::Effect

#endif