/*! @file itemMeta.h
	@brief Defines the metadata stored for built-in and user-defined items.
	@date 07/24/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_ITEM_ITEMMETA_H
#define INCLUDE_ITEM_ITEMMETA_H

#include <string_view>
#include <vector>

#include "Battle/battleTargetsAndTriggers.h"
#include "Configuration/constants.h"
#include "Core/typedefs.h"
#include "Effect/effectID.h"
#include "Effect/effectSourceAndSuppresion.h"

#include "itemID.h"

namespace PocketCore::Item
{
	using PocketCore::Battle::BattleEventID;
	using PocketCore::Battle::BattleEventRole;
	using PocketCore::Battle::BattleTargetID;
	using PocketCore::Configuration::MAX_SUPPRESSION_RULES_PER_TRIGGER;
	using PocketCore::Core::ub;
	using PocketCore::Effect::EffectID;
	using PocketCore::Effect::SuppressionRule;

	struct ItemEffectTrigger
	{
		public:
			std::array<SuppressionRule, MAX_SUPPRESSION_RULES_PER_TRIGGER> mSuppressionRules{};
			std::vector<EffectID> mEffects;
			BattleEventID mTrigger;
			BattleEventRole mRole{BattleEventRole::Any};
			ub mSuppresionRuleCount{0};
	};

	/*! @struct ItemMeta Item/itemMeta.h
		@brief Stores one item's stable ID, display name, and owned trigger definitions.
		@details The trigger vector owns its elements and their effect vectors. The display name is a non-owning view whose backing storage
	   must remain valid while this metadata is registered.
		@date 07/27/2026
		@version x.x.x
		@since x.x.x
		@author Matthew Moore
	*/
	struct ItemMeta
	{
		public:
			/*! @brief The owned trigger and effect definitions for this item. */
			std::vector<ItemEffectTrigger> mTriggers;

			/*! @brief The case-sensitive display name stored as a non-owning view. */
			std::string_view mName{};

			/*! @brief The stable built-in or user-assigned identifier. */
			ItemID mItemID{};

			/*! @brief The target ID for this item's effects. */
			BattleTargetID mTargetID{};

			/*! @brief If the item can be consumed in battle. If it gets consumed, the item is removed from the holder. */
			bool mIsConsumable{};
	};
} // namespace PocketCore::Item

#endif
