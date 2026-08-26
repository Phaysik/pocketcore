/*! @file natureMeta.h
	@brief Defines the metadata stored for built-in and user-defined natures.
	@date 07/24/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_NATURE_NATUREMETA_H
#define INCLUDE_NATURE_NATUREMETA_H

#include <array>
#include <string_view>

#include "Battle/battleTargetsAndTriggers.h"
#include "Effect/effectTrigger.h"

#include "constants.h"
#include "natureID.h"

namespace PocketCore::Nature
{
	using PocketCore::Battle::BattleEventID;
	using PocketCore::Battle::BattleEventRole;
	using PocketCore::Battle::BattleTargetID;
	using PocketCore::Effect::EffectTrigger;

	/*! @struct NatureMeta Nature/natureMeta.h
		@brief Stores one nature's stable ID, display name, and owned trigger definitions.
		@details The trigger vector owns its elements and their effect vectors. The display name is a non-owning view whose backing storage
	   must remain valid while this metadata is registered.
		@date 07/27/2026
		@version x.x.x
		@since x.x.x
		@author Matthew Moore
	*/
	struct NatureMeta
	{
		public:
			/*! @brief The stat multipliers for HP, Attack, Defense, SpAttack, SpDefense, Speed. */
			std::array<double, MAX_STAT_TYPES> mStatMultipliers{1.0, 1.0, 1.0, 1.0, 1.0, 1.0};

			/*! @brief The owned trigger and effect definitions for this move. */
			std::vector<EffectTrigger> mTriggers{};

			/*! @brief The case-sensitive display name stored as a non-owning view. */
			std::string_view mName{};

			/*! @brief The stable built-in or user-assigned identifier for the move's target. */
			BattleTargetID mTargetID{};

			/*! @brief The stable built-in or user-assigned identifier. */
			NatureID mNatureID{};
	};
} // namespace PocketCore::Nature

#endif
