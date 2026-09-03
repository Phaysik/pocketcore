/*! @file effectTrigger.h
	@brief Defines shared event-driven effect trigger metadata.
	@date 09/03/2026
	@since 0.12.6
	@version 0.12.19
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
#include "Effect/effectSourceAndSuppression.h"

namespace PocketCore::Effect
{
	using PocketCore::Battle::BattleEventID;
	using PocketCore::Battle::BattleEventRole;
	using PocketCore::Configuration::MAX_SUPPRESSION_RULES_PER_TRIGGER;
	using PocketCore::Core::ub;

	/*! @struct EffectTrigger Effect/effectTrigger.h
		@brief Stores the effects and suppression rules activated by one battle event.
		@details The effect vector owns its ordered effect identifiers. The suppression-rule array has fixed physical storage, while
		 @p mSuppressionRuleCount identifies how many entries are active. The trigger is eligible when its event and role match the
		 current battle event.
		@note @p mSuppressionRuleCount must not exceed @ref MAX_SUPPRESSION_RULES_PER_TRIGGER.
		@date 09/03/2026
		@since 0.12.6
		@version 0.12.19
		@author Matthew Moore
	*/
	struct EffectTrigger
	{
		public:
			/*! @brief Compares two EffectTrigger instances for equivalent metadata.
				@details Compares all fields exactly.
				@param[in] other The EffectTrigger instance to compare.
				@return True when both EffectTrigger instances contain equivalent metadata; otherwise false.
				@since 0.12.19
				@version 0.12.19
			*/
			ATTR_NODISCARD constexpr bool operator==(const EffectTrigger &) const noexcept = default;

			// NOLINTBEGIN(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)

			/*! @brief The fixed-capacity suppression rules associated with this trigger. */
			std::array<SuppressionRule, MAX_SUPPRESSION_RULES_PER_TRIGGER> mSuppressionRules{};

			/*! @brief The owned effect identifiers executed in declaration order when this trigger matches. */
			std::vector<EffectID> mEffects{};

			/*! @brief The battle event that activates this trigger. */
			BattleEventID mTrigger{};

			/*! @brief The battle-event role required for activation, or Any when the role is unrestricted. */
			BattleEventRole mRole{BattleEventRole::Any};

			/*! @brief The number of entries in @ref mSuppressionRules that contain active suppression rules. */
			ub mSuppressionRuleCount{0};

			// NOLINTEND(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)
	};
} // namespace PocketCore::Effect

#endif
