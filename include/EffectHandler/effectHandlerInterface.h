/*! @file effectHandlerInterface.h
	@brief Contains the effect handler interface
	@date 08/04/2026
	@since 0.7.2
	@version 0.9.10
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

	/*! @class IEffectHandler EffectHandler/effectHandlerInterface.h
		@brief Defines the interface and shared battle-slot accessors for effect handlers.
		@details Effect handlers implement @ref apply to mutate battle state using a shared @ref EffectContext and read-only registry
		 metadata. The static accessors resolve the user or target slot identified by the context.
		@warning The context indexes must refer to existing slots in the selected side's team.
		@note The class is not copyable or movable.
		@date 08/04/2026
		@since 0.7.2
		@version 0.9.10
		@author Matthew Moore
	*/
	class IEffectHandler
	{
		public:
			/*! @brief Constructs an effect handler interface.
				@note The default implementation performs no initialization.
				@since 0.7.2
				@version 0.7.2
			*/
			IEffectHandler() = default;

			/*! @brief Prevents copying an effect handler.
				@since 0.7.2
				@version 0.7.2
			 */
			IEffectHandler(const IEffectHandler &) = delete;

			/*! @brief Prevents moving an effect handler.
				@since 0.7.2
				@version 0.7.2
			 */
			IEffectHandler(IEffectHandler &&) = delete;

			/*! @brief Prevents copy assignment of an effect handler.
				@since 0.7.2
				@version 0.7.2
			 */
			IEffectHandler operator=(const IEffectHandler &) = delete;

			/*! @brief Prevents move assignment of an effect handler.
				@since 0.7.2
				@version 0.7.2
			 */
			IEffectHandler operator=(IEffectHandler &&) = delete;

			/*! @brief Destroys an effect handler through the interface.
				@since 0.7.2
				@version 0.7.2
			 */
			virtual ~IEffectHandler() = default; // LCOV_EXCL_LINE

			/*! @brief Applies the handler's effect to battle state.
				@param[in,out] state The battle state to inspect and mutate.
				@param[in,out] context The mutable state shared by effects in the current dispatch.
				@param[in] provider The registries used to resolve effect metadata.
				@note Concrete handlers define the effect-specific behavior.
				@since 0.7.2
				@version 0.8.2
			*/
			virtual void apply(BattleState &state, EffectContext &context, const RegistryProvider &provider) const = 0;

			/*! @brief Returns the mutable battle slot occupied by the effect user.
				@param[in,out] state The battle state containing the user's team.
				@param[in] context The context identifying the user's side and slot index.
				@return A mutable reference to the user's battle slot, valid while the selected team is not invalidated.
				@pre `context.mUserIndex` is less than the selected team's size.
				@note An invalid index triggers an assertion and is also rejected by vector bounds checking.
				@since 0.8.7
				@version 0.8.7
			*/
			static BattleSlot &getUserBattleSlot(BattleState &state, EffectContext &context)
			{
				std::vector<BattleSlot> &userTeam{getTeam(state, context.mUserSide)};
				assert(context.mUserIndex < userTeam.size());

				return userTeam.at(context.mUserIndex);
			}

			/*! @brief Returns the read-only battle slot occupied by the effect user.
				@param[in] state The battle state containing the user's team.
				@param[in] context The context identifying the user's side and slot index.
				@return A const reference to the user's battle slot, valid while the selected team is not invalidated.
				@pre `context.mUserIndex` is less than the selected team's size.
				@note An invalid index triggers an assertion and is also rejected by vector bounds checking.
				@since 0.9.10
				@version 0.9.10
			*/
			static const BattleSlot &getConstUserBattleSlot(BattleState &state, EffectContext &context)
			{
				const std::vector<BattleSlot> &userTeam{getTeamConst(state, context.mUserSide)};
				assert(context.mUserIndex < userTeam.size());

				return userTeam.at(context.mUserIndex);
			}

			/*! @brief Returns the mutable battle slot selected as the effect target.
				@param[in,out] state The battle state containing the target's team.
				@param[in] context The context identifying the target's side and slot index.
				@return A mutable reference to the target's battle slot, valid while the selected team is not invalidated.
				@pre `context.mTargetIndex` is less than the selected team's size.
				@note An invalid index triggers an assertion and is also rejected by vector bounds checking.
				@since 0.8.7
				@version 0.8.7
			*/
			static BattleSlot &getTargetBattleSlot(BattleState &state, EffectContext &context)
			{
				std::vector<BattleSlot> &targetTeam{getTeam(state, context.mTargetSide)};
				assert(context.mTargetIndex < targetTeam.size());

				return targetTeam.at(context.mUserIndex);
			}

			/*! @brief Returns the read-only battle slot selected as the effect target.
				@param[in] state The battle state containing the target's team.
				@param[in] context The context identifying the target's side and slot index.
				@return A const reference to the target's battle slot, valid while the selected team is not invalidated.
				@pre `context.mTargetIndex` is less than the selected team's size.
				@note An invalid index triggers an assertion and is also rejected by vector bounds checking.
				@since 0.9.10
				@version 0.9.10
			*/
			static const BattleSlot &getConstTargetBattleSlot(BattleState &state, EffectContext &context)
			{
				const std::vector<BattleSlot> &targetTeam{getTeamConst(state, context.mTargetSide)};
				assert(context.mTargetIndex < targetTeam.size());

				return targetTeam.at(context.mTargetIndex);
			}
	};
} // namespace PocketCore::Effect

#endif
