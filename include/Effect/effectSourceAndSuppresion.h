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

#include "Ability/abilityID.h"
#include "Battle/battleTargetsAndTriggers.h"
#include "Core/typedefs.h"
#include "Item/itemID.h"
#include "Move/moveID.h"
#include "Nature/natureID.h"

namespace PocketCore::Effect
{
	using PocketCore::Ability::AbilityID;
	using PocketCore::Battle::BattleEventID;
	using PocketCore::Battle::BattleEventRole;
	using PocketCore::Core::ub;
	using PocketCore::Item::ItemID;
	using PocketCore::Move::MoveID;
	using PocketCore::Nature::NatureID;

	enum class EffectSource : ub
	{
		None,
		Move,
		Ability,
		Item,
		Hazard,
		Nature,
	};

	struct SuppressionRule
	{
		public:
			std::optional<AbilityID> mTargetAbilityID{};
			std::optional<ItemID> mTargetItemID{};
			std::optional<MoveID> mTargetMoveID{};
			std::optional<NatureID> mTargetNatureID{};
			BattleEventID mTargetTrigger{};
			BattleEventRole mTargetRole{BattleEventRole::Any};
			EffectSource mTargetSource{};
	};
} // namespace PocketCore::Effect

#endif
