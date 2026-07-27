/*! @file abilityMeta.h
	@brief Contains the ability targets and triggers
	@date 07/24/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_ABILITY_ABILITYMETA_H
#define INCLUDE_ABILITY_ABILITYMETA_H

#include <string_view>
#include <vector>

#include "Core/attributeMacros.h"
#include "Effect/effectType.h"

#include "abilityID.h"
#include "abilityTargetsAndTriggers.h"
#include "constants.h"

namespace PocketCore::Ability
{
	using Ability::AbilityID;
	using PocketCore::Effect::EffectTypeID;

	struct AbilityMeta
	{
		public:
			std::string_view mName{};
			std::vector<AbilityEffectTrigger> mTriggers;
	};

	constexpr AbilityMeta getAbilityMetadata(const AbilityID abilityID)
	{
		switch (abilityID)
		{
			case AbilityID::Drizzle:
				return {.mName = ABILITY_NAME_DRIZZLE,
						.mTriggers = {{.mTrigger = AbilityTriggerID::OnSwitchIn, .mEffects = {EffectTypeID::SetRain}}}};
			case AbilityID::Stench:
				return {.mName = ABILITY_NAME_STENCH,
						.mTriggers = {{.mTrigger = AbilityTriggerID::OnMoveUse, .mEffects = {EffectTypeID::Flinch}}}};
			case AbilityID::None:
				ATTR_FALLTHROUGH;
			default:
				return {.mName = ABILITY_NAME_NONE, .mTriggers = {}};
		}
	}
} // namespace PocketCore::Ability

#endif
