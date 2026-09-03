/*! @file itemMeta.h
	@brief Defines the metadata stored for built-in and user-defined items.
	@date 09/03/2026
	@since 0.4.1
	@version 0.12.19
	@author Matthew Moore
*/

#ifndef INCLUDE_ITEM_ITEMMETA_H
#define INCLUDE_ITEM_ITEMMETA_H

#include <string_view>
#include <vector>

#include "Battle/battleTargetsAndTriggers.h"
#include "Effect/effectTrigger.h"

#include "itemID.h"

namespace PocketCore::Item
{
	using PocketCore::Battle::BattleEventID;
	using PocketCore::Battle::BattleEventRole;
	using PocketCore::Battle::BattleTargetID;
	using PocketCore::Effect::EffectTrigger;

	/*! @struct ItemMeta Item/itemMeta.h
		@brief Stores one item's stable ID, display name, and owned trigger definitions.
		@details The trigger vector owns its elements and their effect vectors. The display name is a non-owning view whose backing storage
	   must remain valid while this metadata is registered.
		@date 09/03/2026
		@since 0.4.1
		@version 0.12.19
		@author Matthew Moore
	*/
	struct ItemMeta
	{
		public:
			/*! @brief Compares two ItemMeta instances for equivalent metadata.
				@details Compares all fields exactly.
				@param[in] other The ItemMeta instance to compare.
				@return True when both instances contain equivalent metadata; otherwise false.
				@since 0.12.19
				@version 0.12.19
			*/
			ATTR_NODISCARD constexpr bool operator==(const ItemMeta &other) const noexcept = default;

			// NOLINTBEGIN(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)

			/*! @brief The owned trigger and effect definitions for this item. */
			std::vector<EffectTrigger> mTriggers;

			/*! @brief The case-sensitive display name stored as a non-owning view. */
			std::string_view mName{};

			/*! @brief The stable built-in or user-assigned identifier. */
			ItemID mItemID{};

			/*! @brief The target ID for this item's effects. */
			BattleTargetID mTargetID{};

			/*! @brief If the item can be consumed in battle. If it gets consumed, the item is removed from the holder. */
			bool mIsConsumable{};

			// NOLINTEND(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)
	};
} // namespace PocketCore::Item

#endif
