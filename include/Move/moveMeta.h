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
#include <utility>
#include <vector>

#include "Core/typedefs.h"
#include "Effect/effectType.h"
#include "Types/typeID.h"

#include "moveID.h"
#include "moveTargetsAndTriggers.h"

namespace PocketCore::Move
{
	using PocketCore::Core::ub;
	using PocketCore::Core::us;
	using PocketCore::Effect::EffectTypeID;
	using PocketCore::Types::TypeID;

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
			/*! @brief The move's hit distribution. */
			std::vector<std::pair<ub, float>> mHitDistribution{{1, 1.0F}};

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
			MoveTargetID mTargetID{};

			/*! @brief The stable built-in or user-assigned identifier for the move's range. */
			MoveRangeID mRangeID{};

			/*! @brief The move's accuracy. */
			ub mAccuracy{};

			/*! @brief The move's priority. */
			ub mPriority{};

			/*! @brief Whether the move is a special move. */
			bool mSpecial{};
	};

	ATTR_CONST std::span<const EffectTypeID> baseAttackEffects() noexcept;

	ATTR_CONST std::span<const EffectTypeID> baseAttackWithRecoil() noexcept;
	ATTR_CONST std::span<const EffectTypeID> baseAttackWithStatus() noexcept;
	ATTR_CONST std::span<const EffectTypeID> baseAttackWithFlinch() noexcept;
	ATTR_CONST std::span<const EffectTypeID> statusChangeEffects() noexcept;
	ATTR_CONST std::span<const EffectTypeID> protectEffects() noexcept;
	ATTR_CONST std::span<const EffectTypeID> fieldEffectEffects() noexcept;
} // namespace PocketCore::Move

#endif
