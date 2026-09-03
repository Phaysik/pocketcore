/*! @file moveMeta.h
	@brief Defines the metadata stored for built-in and user-defined moves.
	@date 09/03/2026
	@since 0.5.2
	@version 0.12.19
	@author Matthew Moore
*/

#ifndef INCLUDE_MOVE_MOVEMETA_H
#define INCLUDE_MOVE_MOVEMETA_H

#include <span>
#include <string_view>
#include <vector>

#include "Battle/battleTargetsAndTriggers.h"
#include "Core/typedefs.h"
#include "Effect/effectTrigger.h"
#include "Types/typeID.h"

#include "moveHitPolicy.h"
#include "moveID.h"

namespace PocketCore::Move
{
	using PocketCore::Battle::BattleEventID;
	using PocketCore::Battle::BattleEventRole;
	using PocketCore::Battle::BattleRangeID;
	using PocketCore::Battle::BattleTargetID;
	using PocketCore::Core::sb;
	using PocketCore::Core::ub;
	using PocketCore::Core::us;
	using PocketCore::Effect::EffectID;
	using PocketCore::Effect::EffectTrigger;
	using PocketCore::Type::TypeID;

	/*! @struct MoveMeta Move/moveMeta.h
		@brief Stores one move's stable ID, display name, and owned trigger definitions.
		@details The trigger vector owns its elements and their effect vectors. The display name is a non-owning view whose backing storage
	   must remain valid while this metadata is registered.
		@date 09/03/2026
		@since 0.5.2
		@version 0.12.19
		@author Matthew Moore
	*/
	struct MoveMeta
	{
		public:
			/*! @brief Compares two MoveMeta instances for equivalent metadata.
				@details Compares all fields exactly.
				@param[in] other The MoveMeta instance to compare.
				@return True when both instances contain equivalent metadata; otherwise false.
				@since 0.12.19
				@version 0.12.19
			*/
			ATTR_NODISCARD constexpr bool operator==(const MoveMeta &other) const noexcept = default;

			// NOLINTBEGIN(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)

			/*! @brief The strategy used to determine the move's number of hit attempts. */
			HitCountPolicy mHitCountPolicy{FixedHitCount{}};

			/*! @brief The owned trigger and effect definitions for this move. */
			std::vector<EffectTrigger> mTriggers{};

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

			// NOLINTEND(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)
	};

	/*! @brief Returns the ordered effect sequence for a standard damaging move.
		@return A non-owning span over the immutable base attack effects. The span remains valid for the lifetime of the program.
		@note No-throw. The function performs no allocation and references static storage.
		@since 0.5.3
		@version 0.12.6
		@author Matthew Moore
	*/
	ATTR_CONST std::span<const EffectID> baseAttackEffects() noexcept;

	/*! @brief Returns the ordered effect sequence for a damaging move that applies recoil.
		@details Preserves the complete @ref baseAttackEffects sequence and appends the recoil effect.
		@return A non-owning span over the immutable attack-with-recoil effects. The span remains valid for the lifetime of the program.
		@note No-throw. The function performs no allocation and references static storage.
		@since 0.5.3
		@version 0.12.6
		@author Matthew Moore
	*/
	ATTR_CONST std::span<const EffectID> baseAttackWithRecoil() noexcept;

	/*! @brief Returns the ordered effect sequence for a damaging move that applies a status condition.
		@details Preserves the complete @ref baseAttackEffects sequence and appends the status-application effect.
		@return A non-owning span over the immutable attack-with-status effects. The span remains valid for the lifetime of the program.
		@note No-throw. The function performs no allocation and references static storage.
		@since 0.5.3
		@version 0.12.6
		@author Matthew Moore
	*/
	ATTR_CONST std::span<const EffectID> baseAttackWithStatus() noexcept;

	/*! @brief Returns the ordered effect sequence for a damaging move that may cause flinching.
		@details Preserves the complete @ref baseAttackEffects sequence and appends the flinch effect.
		@return A non-owning span over the immutable attack-with-flinch effects. The span remains valid for the lifetime of the program.
		@note No-throw. The function performs no allocation and references static storage.
		@since 0.5.3
		@version 0.12.6
		@author Matthew Moore
	*/
	ATTR_CONST std::span<const EffectID> baseAttackWithFlinch() noexcept;
} // namespace PocketCore::Move

#endif
