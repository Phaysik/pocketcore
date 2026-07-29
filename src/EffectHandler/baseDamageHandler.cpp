#include "EffectHandler/baseDamageHandler.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <vector>

#include "Battle/battleState.h"
#include "Configuration/constants.h"
#include "EffectHandler/effectHandlerHelpers.h"
#include "Pokemon/pokemon.h"

namespace PocketCore::Effect
{
	using PocketCore::Battle::BattleSlot;
	using PocketCore::Battle::BattleState;
	using PocketCore::Configuration::BASE_STAGE_MULTIPLIER_DENOMINATOR;
	using PocketCore::Configuration::BASE_STAGE_MULTIPLIER_NUMERATOR;
	using PocketCore::Core::sb;
	using PocketCore::Core::us;
	using PocketCore::Pokemon::Pokemon;

	void BaseDamageHandler::apply(const BattleState &state, EffectContext &context) const
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

		const auto stageToMult = [](sb stage) -> float {
			if (stage >= 0)
			{
				return (BASE_STAGE_MULTIPLIER_NUMERATOR + static_cast<float>(stage)) / BASE_STAGE_MULTIPLIER_DENOMINATOR;
			}

			return BASE_STAGE_MULTIPLIER_NUMERATOR / (BASE_STAGE_MULTIPLIER_DENOMINATOR - static_cast<float>(stage));
		};

		sb userAttackStage{context.mIsSpecial ? user.mStatStages.mSpAttack : user.mStatStages.mAttack};
		sb targetDefenseStage{context.mIsSpecial ? target.mStatStages.mSpDefense : target.mStatStages.mDefense};

		if (context.mDamage.mIsCritical)
		{
			userAttackStage = std::max(userAttackStage, static_cast<sb>(0));
			targetDefenseStage = std::min(targetDefenseStage, static_cast<sb>(0));
		}

		const float userAttackMult{stageToMult(userAttackStage)};
		const float targetDefenseMult{stageToMult(userAttackStage)};

		const float attackStat{
			static_cast<float>(context.mIsSpecial ? userPokemon->getSpAttack() : userPokemon->getAttack()) * userAttackMult,
		};
		const float defenseStat{
			static_cast<float>(context.mIsSpecial ? targetPokemon->getSpDefense() : targetPokemon->getDefense()) * targetDefenseMult,
		};
		const us level{userPokemon->getLevel()};

		// Damage formula calcs

		const us numeratorPart1{static_cast<us>(std::floor((2 * level) / 5) + 2)};
		const us numeratorPart2{static_cast<us>(std::round(static_cast<float>(context.mMoveBasePower) * (attackStat / defenseStat)))};
		const us numerator{static_cast<us>(std::floor(numeratorPart1 * numeratorPart2))};
		const us baseDamageCalc{static_cast<us>(std::floor(numerator / 50) + 2)};

		context.mDamage.mDamage = std::max(static_cast<us>(1), baseDamageCalc);
	}
} // namespace PocketCore::Effect
