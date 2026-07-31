#include "EffectHandler/baseDamageHandler.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <vector>

#include "Battle/battleState.h"
#include "Configuration/cache.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Effect/effectContext.h"
#include "EffectHandler/effectHandlerHelpers.h"
#include "Pokemon/pokemon.h"
#include "Registry/registryProvider.h"

namespace PocketCore::Effect
{
	using PocketCore::Battle::BattleSlot;
	using PocketCore::Battle::BattleState;
	using PocketCore::Configuration::CACHE_STAT_STAGE_MULTIPLIERS;
	using PocketCore::Core::sb;
	using PocketCore::Core::us;
	using PocketCore::Pokemon::Pokemon;
	using PocketCore::Registry::RegistryProvider;

	void BaseDamageHandler::apply(const BattleState &state, EffectContext &context,
								  ATTR_MAYBE_UNUSED const RegistryProvider &provider) const
	{
		const std::vector<BattleSlot> &userTeam{getTeamConst(state, context.mUserSide)};
		const std::vector<BattleSlot> &targetTeam{getTeamConst(state, context.mTargetSide)};

		assert(context.mUserIndex < userTeam.size());
		assert(context.mTargetIndex < targetTeam.size());

		const BattleSlot &user{userTeam.at(context.mUserIndex)};
		const BattleSlot &target{targetTeam.at(context.mTargetIndex)};

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

		const float userAttackMult{CACHE_STAT_STAGE_MULTIPLIERS.at(static_cast<std::size_t>(userAttackStage))};
		const float targetDefenseMult{CACHE_STAT_STAGE_MULTIPLIERS.at(static_cast<std::size_t>(targetDefenseStage))};

		const float userAttackModifier{
			context.mIsSpecial ? user.mDamageFormulaModifiers.mSpecialAttackModifier : user.mDamageFormulaModifiers.mAttackModifier,
		};

		const float targetDefenseModifier{
			context.mIsSpecial ? target.mDamageFormulaModifiers.mSpecialDefenseModifier : target.mDamageFormulaModifiers.mDefenseModifier,
		};

		const float attackStat{
			static_cast<float>(context.mIsSpecial ? userPokemon->getSpAttack() : userPokemon->getAttack()) * userAttackMult
				* userAttackModifier,
		};

		const float defenseStat{
			static_cast<float>(context.mIsSpecial ? targetPokemon->getSpDefense() : targetPokemon->getDefense()) * targetDefenseMult
				* targetDefenseModifier,
		};

		const us levelDamageFactor{userPokemon->getLevelDamageFactor()};

		// Damage formula calcs

		const us numeratorMultPart{static_cast<us>(std::round(static_cast<float>(context.mMoveBasePower) * (attackStat / defenseStat)))};

		const us numerator{static_cast<us>(std::floor(levelDamageFactor * numeratorMultPart))};

		const us baseDamageCalc{static_cast<us>(std::floor(numerator / 50) + 2)};

		context.mDamage.mDamage = std::max(static_cast<us>(1), baseDamageCalc);
	}
} // namespace PocketCore::Effect
