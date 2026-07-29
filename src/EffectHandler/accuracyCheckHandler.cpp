#include "EffectHandler/accuracyCheckHandler.h"

#include <algorithm>
#include <cassert>
#include <vector>

#include "Battle/battleState.h"
#include "Configuration/constants.h"
#include "Core/typedefs.h"
#include "Effect/effectContext.h"
#include "EffectHandler/effectHandlerHelpers.h"
#include "Utility/random.h"

namespace PocketCore::Effect
{
	using PocketCore::Battle::BattleSlot;
	using PocketCore::Battle::BattleState;
	using PocketCore::Configuration::BASE_ACCURACY_MULTIPLIER_DENOMINATOR;
	using PocketCore::Configuration::BASE_ACCURACY_MULTIPLIER_NUMERATOR;
	using PocketCore::Configuration::MAX_ACCURACY_HIT_VALUE;
	using PocketCore::Configuration::MIN_ACCURACY_HIT_VALUE;
	using PocketCore::Core::sb;
	using PocketCore::Core::us;
	using PocketCore::Utility::Random;

	void AccuracyCheckHandler::apply(const BattleState &state, EffectContext &context) const
	{
		const std::vector<BattleSlot> &userTeam{getTeamConst(state, context.mUserSide)};
		const std::vector<BattleSlot> &targetTeam{getTeamConst(state, context.mTargetSide)};

		assert(context.mUserIndex < userTeam.size());
		assert(context.mTargetIndex < targetTeam.size());

		const BattleSlot &user{userTeam.at(context.mUserIndex)};
		const BattleSlot &target{targetTeam.at(context.mTargetIndex)};

		const auto accuracyMult = [](sb stage) -> float {
			if (stage >= 0)
			{
				return (BASE_ACCURACY_MULTIPLIER_NUMERATOR + static_cast<float>(stage)) / BASE_ACCURACY_MULTIPLIER_DENOMINATOR;
			}

			return BASE_ACCURACY_MULTIPLIER_NUMERATOR / (BASE_ACCURACY_MULTIPLIER_DENOMINATOR - static_cast<float>(stage));
		};

		const auto evasionMult = [](sb stage) -> float {
			if (stage >= 0)
			{
				return BASE_ACCURACY_MULTIPLIER_DENOMINATOR / (BASE_ACCURACY_MULTIPLIER_NUMERATOR + static_cast<float>(stage));
			}

			return (BASE_ACCURACY_MULTIPLIER_DENOMINATOR - static_cast<float>(stage)) / BASE_ACCURACY_MULTIPLIER_NUMERATOR;
		};

		float accuracy{
			static_cast<float>(context.mMoveAccuracy) * accuracyMult(user.mStatStages.mAccuracy) * evasionMult(target.mStatStages.mEvasion),
		};

		accuracy = std::max(std::min(accuracy, 100.0F), 0.0F);

		if (Random::get<sb>(MIN_ACCURACY_HIT_VALUE, MAX_ACCURACY_HIT_VALUE) > static_cast<sb>(accuracy))
		{
			context.mDamage.mIsMiss = true;
			context.mDamage.mShouldApplyDamage = false;
			context.mDamage.mShouldContinue = false;
		}
	}
} // namespace PocketCore::Effect
