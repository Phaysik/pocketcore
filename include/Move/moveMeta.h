/*! @file moveMeta.h
	@brief Defines the metadata stored for built-in and user-defined moves.
	@date 07/24/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_MOVE_MOVEMETA_H
#define INCLUDE_MOVE_MOVEMETA_H

#include <span>
#include <string_view>
#include <vector>

#include "Battle/battleTargetsAndTriggers.h"
#include "Configuration/constants.h"
#include "Core/typedefs.h"
#include "Effect/effectID.h"
#include "Effect/effectSourceAndSuppresion.h"
#include "Types/typeID.h"

#include "moveHitPolicy.h"
#include "moveID.h"

namespace PocketCore::Move
{
	using PocketCore::Battle::BattleEventID;
	using PocketCore::Battle::BattleEventRole;
	using PocketCore::Battle::BattleRangeID;
	using PocketCore::Battle::BattleTargetID;
	using PocketCore::Configuration::MAX_SUPPRESSION_RULES_PER_TRIGGER;
	using PocketCore::Core::sb;
	using PocketCore::Core::ub;
	using PocketCore::Core::us;
	using PocketCore::Effect::EffectID;
	using PocketCore::Effect::SuppressionRule;
	using PocketCore::Type::TypeID;

	struct MoveEffectTrigger
	{
		public:
			std::array<SuppressionRule, MAX_SUPPRESSION_RULES_PER_TRIGGER> mSuppressionRules{};
			std::vector<EffectID> mEffects;
			BattleEventID mTrigger;
			BattleEventRole mRole{BattleEventRole::Any};
			ub mSuppresionRuleCount{0};
	};

	/*! @struct MoveMeta Move/moveMeta.h
		@brief Stores one move's stable ID, display name, and owned trigger definitions.
		@details The trigger vector owns its elements and their effect vectors. The display name is a non-owning view whose backing storage
	   must remain valid while this metadata is registered.
		@date 07/27/2026
		@version x.x.x
		@since x.x.x
		@author Matthew Moore
	*/
	struct MoveMeta
	{
		public:
			/*! @brief The strategy used to determine the move's number of hit attempts. */
			HitCountPolicy mHitCountPolicy{FixedHitCount{}};

			/*! @brief The owned trigger and effect definitions for this move. */
			std::vector<MoveEffectTrigger> mTriggers{};

			/*! @brief The case-sensitive display name stored as a non-owning view. */
			std::string_view mName{};

			/*! @brief The stable built-in or user-assigned identifier. */
			MoveID mMoveID{};

			/*! @brief The move's type ID. */
			TypeID mTypeID{};

			/*! @brief The move's power level. */
			us mPower{};

			/*! @brief The stable built-in or user-assigned identifier for the move's target. */
			BattleTargetID mTargetID{};

			/*! @brief The stable built-in or user-assigned identifier for the move's range. */
			BattleRangeID mRangeID{};

			/*! @brief The move's accuracy. */
			ub mAccuracy{};

			/*! @brief The move's priority. */
			sb mPriority{};

			/*! @brief Whether the move is a special move. */
			bool mSpecial{};
	};

	ATTR_CONST std::span<const EffectID> baseAttackEffects() noexcept;

	ATTR_CONST std::span<const EffectID> baseAttackWithRecoil() noexcept;
	ATTR_CONST std::span<const EffectID> baseAttackWithStatus() noexcept;
	ATTR_CONST std::span<const EffectID> baseAttackWithFlinch() noexcept;
	ATTR_CONST std::span<const EffectID> protectEffects() noexcept;
	ATTR_CONST std::span<const EffectID> fieldEffectEffects() noexcept;
} // namespace PocketCore::Move

#endif
