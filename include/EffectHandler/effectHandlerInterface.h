/*! @file effectHandlerInterface.h
	@brief Contains the effect handler interface
	@date 07/24/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_EFFECT_EFFECTHANDLERINTERFACE_H
#define INCLUDE_EFFECT_EFFECTHANDLERINTERFACE_H

#include "Battle/battleState.h"
#include "Effect/effectContext.h"
#include "EffectHandler/effectHandlerHelpers.h"
#include "Registry/registryProvider.h"

namespace PocketCore::Effect
{
	using PocketCore::Battle::BattleSlot;
	using PocketCore::Battle::BattleState;
	using PocketCore::Registry::RegistryProvider;

	class IEffectHandler
	{
		public:
			IEffectHandler() = default;
			IEffectHandler(const IEffectHandler &) = delete;
			IEffectHandler(IEffectHandler &&) = delete;
			IEffectHandler operator=(const IEffectHandler &) = delete;
			IEffectHandler operator=(IEffectHandler &&) = delete;

			virtual ~IEffectHandler() = default; // LCOV_EXCL_LINE
			virtual void apply(BattleState &state, EffectContext &context, const RegistryProvider &provider) const = 0;

			static BattleSlot &getUserBattleSlot(BattleState &state, EffectContext &context)
			{
				std::vector<BattleSlot> &userTeam{getTeam(state, context.mUserSide)};
				assert(context.mUserIndex < userTeam.size());

				return userTeam.at(context.mUserIndex);
			}

			static const BattleSlot &getConstUserBattleSlot(BattleState &state, EffectContext &context)
			{
				const std::vector<BattleSlot> &userTeam{getTeamConst(state, context.mUserSide)};
				assert(context.mUserIndex < userTeam.size());

				return userTeam.at(context.mUserIndex);
			}

			static BattleSlot &getTargetBattleSlot(BattleState &state, EffectContext &context)
			{
				std::vector<BattleSlot> &targetTeam{getTeam(state, context.mTargetSide)};
				assert(context.mTargetIndex < targetTeam.size());

				return targetTeam.at(context.mUserIndex);
			}

			static const BattleSlot &getConstTargetBattleSlot(BattleState &state, EffectContext &context)
			{
				const std::vector<BattleSlot> &targetTeam{getTeamConst(state, context.mTargetSide)};
				assert(context.mTargetIndex < targetTeam.size());

				return targetTeam.at(context.mTargetIndex);
			}
	};
} // namespace PocketCore::Effect

#endif
