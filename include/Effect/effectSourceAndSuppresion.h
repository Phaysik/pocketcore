/*! @file effectSourceAndSuppresion.h
	@brief Contains the effect source and suppression
	@date 08/21/2026
	@since 0.9.13
	@version 0.10.9
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

	/*! @enum EffectSource
		@showenumvalues
		@brief Identifies the category of game object that initiated an effect.
		@details Suppression matching compares this category before consulting any source-specific identifier.
		@note All enum values must be handled exhaustively when dispatching on the effect source.
		@date 08/04/2026
		@since 0.9.13
		@version 0.9.13
		@author Matthew Moore
	*/
	enum class EffectSource : ub
	{
		/*! @brief Indicates no initiating source. */
		None,
		/*! @brief Indicates a move initiated the effect. */
		Move,
		/*! @brief Indicates an ability initiated the effect. */
		Ability,
		/*! @brief Indicates a held item initiated the effect. */
		Item,
		/*! @brief Indicates an entry hazard initiated the effect. */
		Hazard,
		/*! @brief Indicates a nature initiated the effect. */
		Nature,
	};

	/*! @struct SuppressionRule Effect/effectSourceAndSuppresion.h
		@brief Describes which effect triggers an active suppression blocks.
		@details A rule matches when the dispatched effect's source category, trigger, and role align with the rule's targets. The optional
			identifiers narrow a rule to one specific ability, item, move, or nature; leaving one empty matches every entity of that source
			category.
		@note Only the optional identifier matching the rule's target source is consulted.
		@date 08/18/2026
		@since 0.9.13
		@version 0.10.8
		@author Matthew Moore
	*/
	struct SuppressionRule
	{
		public:
			/*! @brief The specific ability to suppress, or empty to suppress every ability. */
			std::optional<AbilityID> mTargetAbilityID{};
			/*! @brief The specific item to suppress, or empty to suppress every item. */
			std::optional<ItemID> mTargetItemID{};
			/*! @brief The specific move to suppress, or empty to suppress every move. */
			std::optional<MoveID> mTargetMoveID{};
			/*! @brief The specific nature to suppress, or empty to suppress every nature. */
			std::optional<NatureID> mTargetNatureID{};
			/*! @brief The battle event whose effects are suppressed. */
			BattleEventID mTargetTrigger{};
			/*! @brief The participant role the rule applies to, defaulting to every role. */
			BattleEventRole mTargetRole{BattleEventRole::Any};
			/*! @brief The source category the rule suppresses. */
			EffectSource mTargetSource{};
	};
} // namespace PocketCore::Effect

#endif
