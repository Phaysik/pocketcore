/*! @file effectContext.h
	@brief Contains the effect context
	@date 07/27/2026
	@version 0.3.0
	@since 0.5.1
	@author Matthew Moore
*/

#ifndef INCLUDE_EFFECT_EFFECTCONTEXT_H
#define INCLUDE_EFFECT_EFFECTCONTEXT_H

#include <array>
#include <optional>
#include <span>
#include <utility>
#include <vector>

#include "Ability/abilityID.h"
#include "Battle/battleTargetsAndTriggers.h"
#include "Configuration/constants.h"
#include "Configuration/multiplierRegistryConfiguration.h"
#include "Core/typedefs.h"
#include "Effect/effectSourceAndSuppresion.h"
#include "Item/itemID.h"
#include "Move/moveID.h"
#include "Multiplier/builtInMultiplierID.h"
#include "Multiplier/multiplierID.h"
#include "Multiplier/multiplierMeta.h"
#include "Nature/natureID.h"
#include "Pokemon/pokemon.h"
#include "Registry/multiplierRegistry.h"
#include "Status/statusID.h"
#include "Types/typeID.h"

namespace PocketCore::Effect
{
	using PocketCore::Ability::AbilityID;
	using PocketCore::Battle::BattleRangeID;
	using PocketCore::Configuration::BASE_MULTIPLIER_VALUE;
	using PocketCore::Configuration::MultiplierRegistryConfiguration;
	using PocketCore::Core::ub;
	using PocketCore::Core::us;
	using PocketCore::Item::ItemID;
	using PocketCore::Move::MoveID;
	using PocketCore::Multiplier::BuiltinMultiplierID;
	using PocketCore::Multiplier::MultiplierID;
	using PocketCore::Multiplier::MultiplierMeta;
	using PocketCore::Multiplier::toMultiplierID;
	using PocketCore::Nature::NatureID;
	using PocketCore::Pokemon::Pokemon;
	using PocketCore::Registry::Multiplier::MultiplierRegistry;
	using PocketCore::Status::StatusID;
	using PocketCore::Types::TypeID;

	/*! @enum Side
		@showenumvalues
		@brief Identifies one of the two sides participating in a battle.
		@details The value selects the corresponding side's active slots and party data and identifies the user or target side in an
		 @ref EffectContext.
		@note All enum values must be handled exhaustively when selecting side-specific battle state.
		@date 07/26/2026
		@since 0.3.0
		@version 0.4.0
		@author Matthew Moore
	*/
	enum class Side : ub
	{
		/*! @brief Identifies side A. */
		A,
		/*! @brief Identifies side B. */
		B,
	};

	/*! @struct DamageContext Effect/effectContext.h
		@brief Stores damage calculation results and control flags for an effect dispatch.
		@details The context records the damage amount, recoil ratio, critical-hit and miss state, and whether damage application and
		 subsequent effect processing should continue.
		@date 07/26/2026
		@since 0.3.0
		@version 0.4.0
		@author Matthew Moore
	*/
	struct DamageContext
	{
		public:
			/*! @brief The fraction of damage returned to the source as recoil. */
			float mRecoilRatio{0.0F};

			/*! @brief The calculated damage amount. */
			us mDamage{0};

			/*! @brief Indicates whether the calculated damage is a critical hit. */
			bool mIsCritical{false};

			/*! @brief Indicates whether the attempted action missed. */
			bool mIsMiss{false};

			/*! @brief Indicates whether the calculated damage should be applied. */
			bool mShouldApplyDamage{true};

			/*! @brief Indicates whether processing of subsequent effects should continue. */
			bool mShouldContinue{true};
	};

	/*! @struct EffectContext Effect/effectContext.h
		@brief Stores the mutable state shared by effects during one effect dispatch.
		@details The context carries damage results, source metadata, move parameters, target-selection data, hit-attempt state, and
		 ordered active multipliers. The active multiplier view is non-owning and remains valid until the next multiplier mutation.
		@date 07/26/2026
		@since 0.3.0
		@version 0.5.1
		@author Matthew Moore
	*/
	struct EffectContext
	{
		public:
			/*! @brief Adds or updates a multiplier in the active application sequence.
				@details Existing multipliers retain their original application order while their values are replaced. New multipliers are
			   appended to the sequence. Built-in multiplier positions are tracked for constant-time updates.
				@param[in] multID The stable identifier of the multiplier to add or update.
				@param[in] value The multiplier value to store for subsequent damage calculations.
				@throws std::out_of_range If an invalid built-in multiplier identifier indexes the built-in position table.
			*/
			void setMultiplier(const MultiplierID multID, const double value);

			/*! @brief Applies the active multipliers to a base damage value.
				@details Applies multipliers in insertion order using each registered multiplier's application policy. Non-finite and
			   negative values are treated as 1.0 and 0.0 respectively, and the resulting damage is clamped to at least 1.
				@param[in] baseDamage The unmodified damage value.
				@param[in] multiplierRegistry The registry used to resolve multiplier application policies.
				@return The damage value after all active multipliers have been applied, with a minimum value of 1.
			*/
			ATTR_NODISCARD us applyMultiplier(const us baseDamage, const MultiplierRegistry &multiplierRegistry) const;

			/*! @brief Removes all active multipliers and resets built-in multiplier indexes.
				@post The active multiplier sequence is empty and subsequent multiplier insertion starts with a clean index table.
			*/
			void resetMultipliers();

			/*! @brief Returns active multipliers in application order.
				@return A read-only view that remains valid until the next multiplier mutation.
			*/
			ATTR_NODISCARD std::span<const std::pair<MultiplierID, double>> getActiveMultipliers() const noexcept;

			// NOLINTBEGIN(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)
			/*! @brief The damage calculation state and effect-processing control flags. */
			DamageContext mDamage{};
			/*! @brief The type identifier associated with the current effect source. */
			TypeID mMoveTypeID{};

			/*! @brief The ability identifier associated with the current effect source. */
			AbilityID mAbilityID{};

			/*! @brief The item identifier associated with the current effect source. */
			ItemID mItemID{};

			/*! @brief The move identifier associated with the current effect source. */
			MoveID mMoveID{};

			/*! @brief The nature identifier associated with the current effect source. */
			NatureID mNatureID{};

			/*! @brief An optional range identifier overriding the move's default target range. */
			std::optional<BattleRangeID> mRangeOverride{};

			/*! @brief The side-local slot index of the effect user. */
			ub mUserIndex{0};
			/*! @brief The side-local slot index of the effect target. */
			ub mTargetIndex{0};
			/*! @brief The move's base power used by damage calculations. */
			us mMoveBasePower{0};
			/*! @brief The move's accuracy used by accuracy checks. */
			ub mMoveAccuracy{0};
			/*! @brief The one-based index of the current hit attempt for a multi-hit action. */
			ub mHitAttemptIndex{0};

			/*! @brief Identifies the category of source that initiated the current effect dispatch. */
			EffectSource mSourceType{EffectSource::None};

			/*! @brief Identifies the battle side containing the effect user. */
			Side mUserSide{};
			/*! @brief Identifies the battle side containing the effect target. */
			Side mTargetSide{};

			/*! @brief Indicates whether the current move uses special rather than physical calculations. */
			bool mIsSpecial{false};

			// NOLINTEND(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)

		private:
			/*! @brief The number of built-in multiplier identifiers represented by the position table. */
			static constexpr std::size_t BUILTIN_MULTIPLIER_COUNT{static_cast<std::size_t>(BuiltinMultiplierID::FinalMultiplier)};

			/*! @brief The owned active multipliers in their application order. */
			std::vector<std::pair<MultiplierID, double>> mActiveMultipliers{};
			/*! @brief Maps built-in multiplier values to their positions in mActiveMultipliers. */
			std::array<us, BUILTIN_MULTIPLIER_COUNT> mBuiltinMultiplierPositions{};
	};
} // namespace PocketCore::Effect

#endif
