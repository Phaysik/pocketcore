/*! @file battleAction.h
	@brief Defines actions for battle states.
	@date 07/27/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_BATTLE_BATTLE_ACTION_H
#define INCLUDE_BATTLE_BATTLE_ACTION_H

#include <optional>
#include <variant>

#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Effect/effectContext.h"

#include "battleTargetsAndTriggers.h"

namespace PocketCore::Battle
{
	using PocketCore::Core::ub;
	using PocketCore::Effect::Side;

	/*! @struct BattleTarget Battle/battleEngine.h
		@brief Identifies one active battle slot selected as an effect or move target.
		@date 08/05/2026
		@version x.x.x
		@since x.x.x
		@author Matthew Moore
	*/
	struct BattleTarget
	{
		public:
			// NOLINTBEGIN(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)

			/*! @brief The trainer side containing the selected slot. */
			Side mSide{Side::A};

			/*! @brief The zero-based active slot index on the selected side. */
			ub mSlotIndex{0};

			// NOLINTEND(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)

			ATTR_NODISCARD constexpr bool operator==(const BattleTarget &) const noexcept = default;
	};

	/*! @struct MoveAction Battle/battleEngine.h
		@brief Describes one active Pokemon selecting a move during a turn.
		@date 08/05/2026
		@version x.x.x
		@since x.x.x
		@author Matthew Moore
	*/
	struct MoveAction
	{
		public:
			/*! @brief The explicitly selected target for single-target moves, or no value for automatically resolved targets. */
			std::optional<BattleTarget> mSelectedTarget{};

			/*! @brief The trainer issuing the move. */
			Side mSide{Side::A};

			/*! @brief The zero-based active slot containing the move user. */
			ub mUserSlotIndex{0};

			/*! @brief The zero-based move slot selected from the user's move set. */
			ub mMoveSlotIndex{0};
	};

	/*! @struct SwitchAction Battle/battleEngine.h
		@brief Describes replacing one active Pokemon with a party member.
		@date 08/05/2026
		@version x.x.x
		@since x.x.x
		@author Matthew Moore
	*/
	struct SwitchAction
	{
		public:
			// NOLINTBEGIN(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)

			/*! @brief The trainer performing the switch. */
			Side mSide{Side::A};

			/*! @brief The zero-based active slot to replace. */
			ub mActiveSlotIndex{0};

			/*! @brief The zero-based party index of the incoming Pokemon. */
			ub mPartyIndex{0};

			// NOLINTEND(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)
	};

	using BattleAction = std::variant<MoveAction, SwitchAction>;
} // namespace PocketCore::Battle

#endif
