/*! @file effectContext.h
	@brief Contains the effect context
	@date 07/24/2026
	@version x.x.x
	@since x.x.x
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
#include "Configuration/constants.h"
#include "Configuration/multiplierRegistryConfiguration.h"
#include "Core/typedefs.h"
#include "Item/itemID.h"
#include "Move/moveID.h"
#include "Move/moveTargetsAndTriggers.h"
#include "Multiplier/builtInMultiplierID.h"
#include "Multiplier/multiplierID.h"
#include "Multiplier/multiplierMeta.h"
#include "Pokemon/pokemon.h"
#include "Status/statusID.h"
#include "Types/typeID.h"

namespace PocketCore::Effect
{
	using PocketCore::Ability::AbilityID;
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
	using PocketCore::Pokemon::Pokemon;
	using PocketCore::Status::StatusID;
	using PocketCore::Types::TypeID;

	enum class Side : ub
	{
		A,
		B,
	};

	enum class EffectSource : ub
	{
		None,
		Move,
		Ability,
		Item,
		Hazard,
	};

	struct DamageContext
	{
		public:
			float mRecoilRatio{0.0F};
			us mDamage{0};

			bool mIsCritical{false};
			bool mIsMiss{false};
			bool mShouldApplyDamage{true};
			bool mShouldContinue{true};
	};

	struct EffectContext
	{

		public:
			void setMultiplier(const MultiplierID multID, const float value);

			ATTR_NODISCARD us applyMultiplier(const us baseDamage) const;

			void resetMultipliers();

			/*! @brief Returns active multipliers in application order.
				@return A read-only view that remains valid until the next multiplier mutation.
			*/
			ATTR_NODISCARD std::span<const std::pair<MultiplierID, float>> getActiveMultipliers() const noexcept;

			// NOLINTBEGIN(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)

			DamageContext mDamage{};

			TypeID mMoveTypeID{};
			AbilityID mAbilityID{};
			MoveID mMoveID{};

			std::optional<PocketCore::Move::MoveRangeID> mRangeOverride{};

			ub mUserIndex{0};
			ub mTargetIndex{0};
			ub mMoveBasePower{0};
			ub mMoveAccuracy{0};

			EffectSource mSourceType{EffectSource::None};
			Side mUserSide{};
			Side mTargetSide{};

			bool mIsSpecial{false};

			// NOLINTEND(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)

		private:
			static constexpr std::size_t BUILTIN_MULTIPLIER_COUNT{static_cast<std::size_t>(BuiltinMultiplierID::Stab) + 1U};

			std::vector<std::pair<MultiplierID, float>> mActiveMultipliers{};
			std::array<us, BUILTIN_MULTIPLIER_COUNT> mBuiltinMultiplierPositions{};
	};
} // namespace PocketCore::Effect

#endif
