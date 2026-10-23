/*! @file abilityMeta.h
	@brief Defines the metadata stored for built-in and user-defined abilities.
	@date 10/23/2026
	@since 0.3.0
	@version 0.12.53
	@author Matthew Moore
*/

#ifndef INCLUDE_ABILITY_ABILITYMETA_H
#define INCLUDE_ABILITY_ABILITYMETA_H

#include <string>
#include <vector>

#include "Battle/battleTargetsAndTriggers.h"
#include "Core/typedefs.h"
#include "Effect/effectTrigger.h"

#include "abilityID.h"

namespace PocketCore::Ability
{
	using PocketCore::Battle::BattleEventID;
	using PocketCore::Battle::BattleEventRole;
	using PocketCore::Battle::BattleTargetID;
	using PocketCore::Core::ub;
	using PocketCore::Effect::EffectTrigger;

	/*! @enum AbilityClassification
		@showenumvalues
		@brief Distinguishes standard abilities from hidden abilities.
		@details Records an ability's category in @ref AbilityMeta independently of its trigger and effect definitions.
		@date 10/23/2026
		@since 0.12.53
		@version 0.12.53
		@author Matthew Moore
	*/
	enum class AbilityClassification : ub
	{
		/*! @brief Identifies a standard ability. */
		Standard,
		/*! @brief Identifies a hidden ability. */
		Hidden,
	};

	/*! @struct AbilityMeta Ability/abilityMeta.h
		@brief Stores one ability's stable ID, display name, and owned trigger definitions.
		@details The trigger vector owns its elements and their effect vectors. The display name is a non-owning view whose backing storage
	   must remain valid while this metadata is registered.
		@date 10/23/2026
		@since 0.3.0
		@version 0.12.53
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

			/*! @brief The ability's category, defaulting to @ref AbilityClassification::Standard. */
			AbilityClassification mClassification{AbilityClassification::Standard};

			// NOLINTEND(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)
	};
} // namespace PocketCore::Ability

#endif
