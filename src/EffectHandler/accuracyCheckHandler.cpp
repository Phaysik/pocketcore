#include "EffectHandler/accuracyCheckHandler.h"

#include <algorithm>
#include <cassert>

#include "Battle/battleState.h"
#include "Configuration/cache.h"
#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Effect/effectContext.h"
#include "EffectHandler/effectHandlerHelpers.h"
#include "EffectHandler/effectHandlerInterface.h"
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
	using PocketCore::Configuration::statStageCacheIndex;
	using PocketCore::Core::sb;
	using PocketCore::Core::us;
	using PocketCore::Registry::RegistryProvider;
	using PocketCore::Utility::Random;

	void AccuracyCheckHandler::apply(BattleState &state, EffectContext &context, ATTR_MAYBE_UNUSED const RegistryProvider &provider) const
	{
		const BattleSlot &user{IEffectHandler::getConstUserBattleSlot(state, context)};
		const BattleSlot &target{IEffectHandler::getConstTargetBattleSlot(state, context)};

		double accuracy{
			static_cast<double>(context.mMoveAccuracy)
				* CACHE_ACCURACY_STAGE_MULTIPLIERS.at(statStageCacheIndex(user.mStatStages.mAccuracy))
				* CACHE_EVASION_STAGE_MULTIPLIERS.at(statStageCacheIndex(target.mStatStages.mEvasion)),
		};

		accuracy = std::max(std::min(accuracy, 100.0), 0.0);

		if (Random::get<sb>(MIN_ACCURACY_HIT_VALUE, MAX_ACCURACY_HIT_VALUE) > static_cast<sb>(accuracy))
		{
			context.mDamage.mIsMiss = true;
			context.mDamage.mShouldApplyDamage = false;
			context.mDamage.mShouldContinue = false;
		}
	}
} // namespace PocketCore::Effect
