#include "EffectHandler/accuracyCheckHandler.h"

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <vector>

#include "Battle/battleState.h"
#include "Configuration/cache.h"
#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Effect/effectContext.h"
#include "EffectHandler/effectHandlerHelpers.h"
#include "Registry/registryProvider.h"
#include "Utility/random.h"

namespace PocketCore::Effect
{
	using PocketCore::Battle::BattleSlot;
	using PocketCore::Battle::BattleState;
	using PocketCore::Configuration::CACHE_ACCURACY_STAGE_MULTIPLIERS;
	using PocketCore::Configuration::CACHE_EVASION_STAGE_MULTIPLIERS;
	using PocketCore::Configuration::MAX_ACCURACY_HIT_VALUE;
	using PocketCore::Configuration::MIN_ACCURACY_HIT_VALUE;
	using PocketCore::Core::sb;
	using PocketCore::Core::us;
	using PocketCore::Registry::RegistryProvider;
	using PocketCore::Utility::Random;

	void AccuracyCheckHandler::apply(const BattleState &state, EffectContext &context,
									 ATTR_MAYBE_UNUSED const RegistryProvider &provider) const
	{
		const std::vector<BattleSlot> &userTeam{getTeamConst(state, context.mUserSide)};
		const std::vector<BattleSlot> &targetTeam{getTeamConst(state, context.mTargetSide)};

		assert(context.mUserIndex < userTeam.size());
		assert(context.mTargetIndex < targetTeam.size());

		const BattleSlot &user{userTeam.at(context.mUserIndex)};
		const BattleSlot &target{targetTeam.at(context.mTargetIndex)};

		float accuracy{
			static_cast<float>(context.mMoveAccuracy)
				* CACHE_ACCURACY_STAGE_MULTIPLIERS.at(static_cast<std::size_t>(user.mStatStages.mAccuracy))
				* CACHE_EVASION_STAGE_MULTIPLIERS.at(static_cast<std::size_t>(target.mStatStages.mEvasion)),
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
