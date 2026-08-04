#include "EffectHandler/baseDamageHandler.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <limits>

#include "Battle/battleState.h"
#include "Configuration/cache.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Effect/effectContext.h"
#include "EffectHandler/effectHandlerHelpers.h"
#include "EffectHandler/effectHandlerInterface.h"
#include "Pokemon/pokemon.h"
#include "Registry/registryProvider.h"

namespace PocketCore::Effect
{
	using PocketCore::Battle::BattleSlot;
	using PocketCore::Battle::BattleState;
	using PocketCore::Configuration::CACHE_STAT_STAGE_MULTIPLIERS;
	using PocketCore::Configuration::statStageCacheIndex;
	using PocketCore::Core::sb;
	using PocketCore::Core::us;
	using PocketCore::Pokemon::Pokemon;
	using PocketCore::Registry::RegistryProvider;

	void BaseDamageHandler::apply(BattleState &state, EffectContext &context, ATTR_MAYBE_UNUSED const RegistryProvider &provider) const
	{
		const BattleSlot &user{IEffectHandler::getConstUserBattleSlot(state, context)};
		const BattleSlot &target{IEffectHandler::getConstTargetBattleSlot(state, context)};

		const Pokemon *userPokemon{user.mPokemon};
		const Pokemon *targetPokemon{target.mPokemon};

		if (userPokemon == nullptr || targetPokemon == nullptr || context.mMoveBasePower == 0)
		{
			return;
		}

		sb userAttackStage{context.mIsSpecial ? user.mStatStages.mSpAttack : user.mStatStages.mAttack};
		sb targetDefenseStage{context.mIsSpecial ? target.mStatStages.mSpDefense : target.mStatStages.mDefense};

		if (context.mDamage.mIsCritical)
		{
			userAttackStage = std::max(userAttackStage, static_cast<sb>(0));
			targetDefenseStage = std::min(targetDefenseStage, static_cast<sb>(0));
		}

		const float userAttackMult{
			CACHE_STAT_STAGE_MULTIPLIERS.at(statStageCacheIndex(userAttackStage)),
		};
		const float targetDefenseMult{
			CACHE_STAT_STAGE_MULTIPLIERS.at(statStageCacheIndex(targetDefenseStage)),
		};

		const double userAttackModifier{
			context.mIsSpecial ? user.mDamageFormulaModifiers.mSpecialAttackModifier : user.mDamageFormulaModifiers.mAttackModifier,
		};

		const double targetDefenseModifier{
			context.mIsSpecial ? target.mDamageFormulaModifiers.mSpecialDefenseModifier : target.mDamageFormulaModifiers.mDefenseModifier,
		};

		const double attackStat{
			static_cast<double>(context.mIsSpecial ? userPokemon->getSpAttack() : userPokemon->getAttack())
				* static_cast<double>(userAttackMult) * static_cast<double>(userAttackModifier),
		};

		const double defenseStat{
			static_cast<double>(context.mIsSpecial ? targetPokemon->getSpDefense() : targetPokemon->getDefense())
				* static_cast<double>(targetDefenseMult) * static_cast<double>(targetDefenseModifier),
		};

		if (!std::isfinite(attackStat) || !std::isfinite(defenseStat) || attackStat < 0.0 || defenseStat <= 0.0)
		{
			return;
		}

		const us levelDamageFactor{userPokemon->getLevelDamageFactor()};

		// Damage formula calcs

		const double numeratorMultPart{static_cast<double>(context.mMoveBasePower) * (attackStat / defenseStat)};

		const double numerator{std::floor(static_cast<double>(levelDamageFactor) * numeratorMultPart)};

		const double baseDamageCalc{std::floor(numerator / 50.0) + 2.0};

		context.mDamage.mDamage = static_cast<us>(std::clamp(baseDamageCalc, 1.0, static_cast<double>(std::numeric_limits<us>::max())));
	}
} // namespace PocketCore::Effect
