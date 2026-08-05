/*! @file effectSourceAndSuppresion.h
	@brief Contains the effect source and suppression
	@date 07/24/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_EFFECT_EFFECTSOURCEANDSUPPRESSION_H
#define INCLUDE_EFFECT_EFFECTSOURCEANDSUPPRESSION_H

#include <optional>
#include <variant>

#include "Ability/abilityID.h"
#include "Ability/abilityTargetsAndTriggers.h"
#include "Core/typedefs.h"
#include "Item/itemID.h"
#include "Item/itemTargetsAndTriggers.h"
#include "Move/moveID.h"
#include "Move/moveTargetsAndTriggers.h"

namespace PocketCore::Effect
{
	using PocketCore::Ability::AbilityID;
	using PocketCore::Ability::AbilityTriggerID;
	using PocketCore::Core::ub;
	using PocketCore::Item::ItemID;
	using PocketCore::Item::ItemTriggerID;
	using PocketCore::Move::MoveID;
	using PocketCore::Move::MoveTriggerID;

	enum class EffectSource : ub
	{
		None,
		Move,
		Ability,
		Item,
		Hazard,
	};

	struct SuppressionRule
	{
		public:
			std::optional<AbilityID> mTargetAbilityID{};
			std::optional<ItemID> mTargetItemID{};
			std::optional<MoveID> mTargetMoveID{};
			std::variant<AbilityTriggerID, ItemTriggerID, MoveTriggerID> mTargetTrigger{};
			EffectSource mTargetSource{};
	};
} // namespace PocketCore::Effect

#endif
