/*! @file abilityMeta.h
	@brief Defines the metadata stored for built-in and user-defined abilities.
	@date 09/10/2026
	@since 0.3.0
	@version 0.12.20
	@author Matthew Moore
*/

#ifndef INCLUDE_ABILITY_ABILITYMETA_H
#define INCLUDE_ABILITY_ABILITYMETA_H

#include <string>
#include <vector>

#include "Battle/battleTargetsAndTriggers.h"
#include "Effect/effectTrigger.h"

#include "abilityID.h"

namespace PocketCore::Ability
{
	using PocketCore::Battle::BattleEventID;
	using PocketCore::Battle::BattleEventRole;
	using PocketCore::Battle::BattleTargetID;
	using PocketCore::Effect::EffectTrigger;

	/*! @struct AbilityMeta Ability/abilityMeta.h
		@brief Stores one ability's stable ID, display name, and owned trigger definitions.
		@details The trigger vector owns its elements and their effect vectors. The display name is a non-owning view whose backing storage
	   must remain valid while this metadata is registered.
		@date 09/10/2026
		@since 0.3.0
		@version 0.12.20
		@author Matthew Moore
	*/
	struct AbilityMeta
	{
		public:
			/*! @brief Compares two AbilityMeta instances for equivalent metadata.
				@details Compares all fields exactly.
				@param[in] other The AbilityMeta instance to compare.
				@return True when both instances contain equivalent metadata; otherwise false.
				@since 0.12.19
				@version 0.12.19
			*/
			ATTR_NODISCARD constexpr bool operator==(const AbilityMeta &other) const noexcept = default;

			// NOLINTBEGIN(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)

			/*! @brief The case-sensitive display name. */
			std::string mName{};

			/*! @brief The owned trigger and effect definitions for this ability. */
			std::vector<EffectTrigger> mTriggers{};

			/*! @brief The stable built-in or user-assigned identifier. */
			AbilityID mAbilityID{};

			/*! @brief The target ID for this ability's effects. */
			BattleTargetID mTargetID{};

			// NOLINTEND(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)
	};
} // namespace PocketCore::Ability

#endif
