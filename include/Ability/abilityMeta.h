/*! @file abilityMeta.h
	@brief Defines the metadata stored for built-in and user-defined abilities.
	@date 07/24/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_ABILITY_ABILITYMETA_H
#define INCLUDE_ABILITY_ABILITYMETA_H

#include <string_view>
#include <vector>

#include "Battle/battleTargetsAndTriggers.h"
#include "Configuration/constants.h"
#include "Core/typedefs.h"
#include "Effect/builtInEffectID.h"
#include "Effect/effectSourceAndSuppresion.h"

#include "abilityID.h"

namespace PocketCore::Ability
{
	using PocketCore::Battle::BattleEventID;
	using PocketCore::Battle::BattleEventRole;
	using PocketCore::Battle::BattleTargetID;
	using PocketCore::Configuration::MAX_SUPPRESSION_RULES_PER_TRIGGER;
	using PocketCore::Core::ub;
	using PocketCore::Effect::BuiltinEffectID;
	using PocketCore::Effect::SuppressionRule;

	struct AbilityEffectTrigger
	{
		public:
			std::array<SuppressionRule, MAX_SUPPRESSION_RULES_PER_TRIGGER> mSuppressionRules{};
			std::vector<BuiltinEffectID> mEffects{};
			BattleEventID mTrigger{};
			BattleEventRole mRole{BattleEventRole::Any};
			ub mSuppresionRuleCount{0};
	};

	/*! @struct AbilityMeta Ability/abilityMeta.h
		@brief Stores one ability's stable ID, display name, and owned trigger definitions.
		@details The trigger vector owns its elements and their effect vectors. The display name is a non-owning view whose backing storage
	   must remain valid while this metadata is registered.
		@date 07/27/2026
		@version x.x.x
		@since x.x.x
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
