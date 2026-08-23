/*! @file battleEngine_copy.h
	@brief Declares battle orchestration for fights between two sides of Pokemon trainers.
	@date 08/05/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_BATTLE_BATTLEENGINE_H
#define INCLUDE_BATTLE_BATTLEENGINE_H

#include <expected>
#include <optional>
#include <span>
#include <vector>

#include "Ability/abilityMeta.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Effect/effectContext.h"
#include "Effect/effectID.h"
#include "Effect/effectSourceAndSuppresion.h"
#include "Item/itemMeta.h"
#include "Move/moveMeta.h"
#include "Nature/natureMeta.h"
#include "Pokemon/pokemon.h"
#include "Registry/effectRegistry.h"
#include "Registry/registryProvider.h"

#include "battleAction.h"
#include "battleHelpers.h"
#include "battleState.h"
#include "battleTargetsAndTriggers.h"
#include "battleValidation.h"

namespace PocketCore::Battle
{
	using PocketCore::Ability::AbilityMeta;
	using PocketCore::Core::ub;
	using PocketCore::Effect::EffectContext;
	using PocketCore::Effect::EffectID;
	using PocketCore::Effect::EffectSource;
	using PocketCore::Effect::SuppressionRule;
	using PocketCore::Item::ItemMeta;
	using PocketCore::Move::MoveMeta;
	using PocketCore::Nature::NatureMeta;
	using PocketCore::Pokemon::Pokemon;
	using PocketCore::Registry::Effect::EffectRegistry;
	using PocketCore::Registry::RegistryProvider;

	/*! @class BattleEngine Battle/battleEngine_copy.h
		@brief Orchestrates turns, switching, targeting, and metadata triggers for two sides of Pokemon trainers.
		@details The engine owns only its @ref BattleState and uses registered effect metadata to apply effects. Party Pokemon, the
	   @ref RegistryProvider, and the @ref EffectRegistry are non-owning dependencies that must outlive the engine. Switches execute before
	   moves; moves execute by priority and effective speed. Equal ordering is resolved in favor of side A and then by active slot index to
	   keep execution deterministic.
		@warning Not thread-safe. The caller is responsible for synchronizing all access to the engine and its referenced Pokemon.
		@date 08/05/2026
		@version x.x.x
		@since x.x.x
		@author Matthew Moore
	*/
	class BattleEngine
	{
		public:
			explicit BattleEngine(const RegistryProvider &provider, const EffectRegistry &effectRegistry) noexcept
				: mProvider(&provider), mEffectRegistry(&effectRegistry)
			{}

			BattleEngine(const BattleEngine &) = delete;
			BattleEngine(BattleEngine &&) = delete;
			BattleEngine &operator=(const BattleEngine &) = delete;
			BattleEngine &operator=(BattleEngine &&) = delete;
			~BattleEngine() = default;

			/*! @brief Starts a battle and assigns the first healthy party members to active slots.
				@param[in] partyA Side A's non-owning Pokemon pointers in party order.
				@param[in] partyB Side B's non-owning Pokemon pointers in party order.
				@param[in] activePokemonPerSide Number of simultaneously active Pokemon required from each party. Must be greater than zero.
				@return Void on success, or a validation error without starting the battle.
			*/
			ATTR_NODISCARD std::expected<void, BattleEngineError> startBattle(const std::span<Pokemon *const> &partyA,
																			  const std::span<Pokemon *const> &partyB,
																			  ub activePokemonPerSide = 1U);

			/*! @brief Resolves and executes one complete turn of trainer actions.
				@details Every action is validated before the first action executes. Each active slot may submit at most one action.
			   Turn-end triggers run after all executable actions, and actions belonging to Pokemon that faint earlier in the turn are
			   skipped.
				@param[in] actions The move and switch choices submitted by both trainers.
				@return Void on success, or the first validation error before the state is mutated.
			*/
			ATTR_NODISCARD std::expected<void, BattleEngineError> executeTurn(const std::span<const BattleAction> &actions);

			/*! @brief Returns the engine-owned battle state.
				@return A read-only reference valid for the lifetime of the engine.
			*/
			ATTR_NODISCARD ATTR_CONST const BattleState &getState() const noexcept;

		private:
			/*! @enum SlotTriggerTargeting
				@brief Selects how ability and item effects determine their recipients during slot-trigger dispatch.
			*/
			enum class SlotTriggerTargeting : ub
			{
				/*! @brief Resolves recipients from the ability or item's target selector. */
				ResolveMetadataTargets,

				/*! @brief Executes effects against the recipient already stored in the event context. */
				PreserveContextTarget,
			};

			/*! @struct ActiveSuppression Battle/battleEngine_copy.h
				@brief Carries one active suppression rule together with the source and slot that established it.
				@details The source identity and owner allow suppression matching to exclude a trigger from suppressing itself.
			*/
			struct ActiveSuppression
			{
					/*! @brief Stores the suppression criteria copied from effect metadata. */
					SuppressionRule mRule{};

					/*! @brief Identifies the active slot whose metadata established the rule. */
					BattleTarget mOwner{};

					/*! @brief Identifies whether an ability, item, move, or other effect source established the rule. */
					EffectSource mSource{EffectSource::None};
			};

			/*! @brief Switches one active slot immediately and runs switch-in triggers.
				@param[in] action The active slot and incoming party member selected by a trainer.
				@return Void on success, or a validation error without changing the active slot.
			*/
			ATTR_NODISCARD std::expected<void, BattleEngineError> switchPokemon(const SwitchAction &action);

			/*! @brief Determines whether an active suppression rule blocks a trigger dispatch.
				@details Matches source category, trigger, and optional metadata identifiers while excluding a rule from suppressing its own
			   source on its owning slot.
				@param[in] source The category of effect source being dispatched.
				@param[in] owner The active slot that owns the source being dispatched.
				@param[in] triggerID The trigger being dispatched.
				@param[in] context The current effect context containing source-specific metadata identifiers.
				@return True when at least one active rule suppresses the dispatch; otherwise false.
			*/
			ATTR_NODISCARD ATTR_PURE bool isSuppressed(const EffectSource source, const BattleTarget &owner, const BattleEventID eventID,
													   const BattleEventRole role, const EffectContext &context) const noexcept;

			/*! @brief Adds metadata suppression rules to the current dispatch scope.
				@details Copies at most the smaller of @p suppressionRuleCount and @p suppressionRules.size(), associating every copied rule
			   with its source and owner.
				@param[in] suppressionRules The physical span containing suppression rule metadata.
				@param[in] suppressionRuleCount The logical number of rules enabled by the metadata.
				@param[in] source The category of source that establishes the rules.
				@param[in] owner The active slot that owns the source.
			*/
			void activateSuppressions(const std::span<const SuppressionRule> &suppressionRules, const ub suppressionRuleCount,
									  const EffectSource source, const BattleTarget &owner);

			/*! @brief Activates suppression rules from ability entries matching a trigger.
				@param[in] abilityMeta The ability metadata whose matching trigger entries are inspected.
				@param[in] owner The active slot that owns the ability.
				@param[in] triggerID The trigger whose suppression rules become active.
			*/
			void activateAbilitySuppressions(const AbilityMeta &abilityMeta, const BattleTarget &owner, const BattleEventID eventID,
											 const BattleEventRole role);

			/*! @brief Activates suppression rules from item entries matching a trigger.
				@param[in] itemMeta The item metadata whose matching trigger entries are inspected.
				@param[in] owner The active slot that owns the item.
				@param[in] triggerID The trigger whose suppression rules become active.
			*/
			void activateItemSuppressions(const ItemMeta &itemMeta, const BattleTarget &owner, const BattleEventID eventID,
										  const BattleEventRole role);

			/*! @brief Activates suppression rules from nature entries matching a trigger.
				@param[in] natureMeta The nature metadata whose matching trigger entries are inspected.
				@param[in] owner The active slot that owns the nature.
				@param[in] triggerID The trigger whose suppression rules become active.
			*/
			void activateNatureSuppressions(const NatureMeta &natureMeta, const BattleTarget &owner, const BattleEventID eventID,
											const BattleEventRole role);

			/*! @brief Invokes one registered built-in effect against the mutable battle state and context.
				@details Missing registry dependencies, missing effect metadata, or a null apply function make the invocation a no-op.
				@param[in] effect The built-in effect identifier to resolve.
				@param[in,out] context The shared effect context read and modified by the registered effect function.
			*/
			void executeEffect(const EffectID effect, EffectContext &context);

			/*! @brief Executes built-in effects in metadata order through one shared context.
				@details Stops before the next effect when @ref EffectContext damage state disallows continuation and processes
			   effect-caused faints after a normally completed chain.
				@param[in] effects The ordered effect identifiers to execute.
				@param[in,out] context The context shared and mutated by every executed effect.
			*/
			void executeEffects(const std::span<const EffectID> &effects, EffectContext &context);

			/*! @brief Executes an effect sequence once for each slot selected relative to an owner.
				@details Resolves targets without formation-range restrictions, temporarily rewrites user and target coordinates for each
			   target, and restores the original event coordinates before returning.
				@param[in] owner The active slot that owns the ability or item source.
				@param[in] targetID The selector used to resolve effect recipients.
				@param[in] effects The ordered effects to execute for every resolved recipient.
				@param[in,out] context The shared context whose event coordinates are preserved across target iteration.
			*/
			void executeTargetedEffects(const BattleTarget &owner, const BattleTargetID targetID, const std::span<const EffectID> &effects,
										EffectContext &context);

			/*! @brief Executes a previously validated move action against its currently resolvable targets.
				@details Consumes PP after metadata and targets resolve, dispatches move and slot triggers in battle order, applies damage
			   and recoil per hit, processes faints, and confines suppression rules to the move, target, or hit scope that created them.
			   Missing metadata or invalidated targets cancel execution without reporting an error.
				@param[in] action The validated move action to execute.
			*/
			void executeMove(const MoveAction &action);

			/*! @brief Executes every move metadata entry matching a trigger.
				@details Temporarily marks the context source as a move, activates each entry's suppression rules before suppression checks,
			   and restores the caller's source category afterward.
				@param[in] moveMeta The move metadata containing trigger entries.
				@param[in] triggerID The move trigger to dispatch.
				@param[in,out] context The shared context supplied to matching effects.
			*/
			void executeMoveTrigger(const MoveMeta &moveMeta, BattleEventID eventID, BattleEventRole role, EffectContext &context);

			/*! @brief Executes unsuppressed ability entries matching a trigger.
				@details Stamps ability identity into @p context and either resolves the ability's declared targets or executes effects
			   directly against an already prepared event context.
				@param[in] owner The active slot that owns the ability.
				@param[in] abilityMeta The ability metadata containing trigger entries and targeting rules.
				@param[in] triggerID The ability trigger to dispatch.
				@param[in,out] context The context stamped with the ability source and passed to matching effects.
				@param[in] targetEffects True to resolve the ability target selector; false to preserve the existing event target.
			*/
			void executeAbilityTrigger(const BattleTarget &owner, const AbilityMeta &abilityMeta, const BattleEventID eventID,
									   const BattleEventRole role, EffectContext &context, const bool targetEffects);

			/*! @brief Executes unsuppressed item entries matching a trigger.
				@details Stamps item identity into @p context and either resolves the item's declared targets or executes effects directly
			   against an already prepared event context.
				@param[in] owner The active slot that owns the item.
				@param[in] itemMeta The item metadata containing trigger entries and targeting rules.
				@param[in] triggerID The item trigger to dispatch.
				@param[in,out] context The context stamped with the item source and passed to matching effects.
				@param[in] targetEffects True to resolve the item target selector; false to preserve the existing event target.
			*/
			void executeItemTrigger(const BattleTarget &owner, const ItemMeta &itemMeta, const BattleEventID eventID,
									const BattleEventRole role, EffectContext &context, const bool targetEffects);

			/*! @brief Executes unsuppressed nature entries matching a trigger.
				@details Stamps nature identity into @p context and either resolves the nature's declared targets or executes effects
			   directly against an already prepared event context.
				@param[in] owner The active slot that owns the nature.
				@param[in] natureMeta The nature metadata containing trigger entries and targeting rules.
				@param[in] triggerID The nature trigger to dispatch.
				@param[in,out] context The context stamped with the nature source and passed to matching effects.
				@param[in] targetEffects True to resolve the nature target selector; false to preserve the existing event target.
			*/
			void executeNatureTrigger(const BattleTarget &owner, const NatureMeta &natureMeta, const BattleEventID eventID,
									  const BattleEventRole role, EffectContext &context, const bool targetEffects);

			void executeEndTurnTrigger();

			ATTR_NODISCARD bool executeWeightHitCountPolicy(const bool runBeforeHitPerAttempt, EffectContext &targetContext,
															const MoveMeta *moveMeta, std::size_t &targetSuppressionCount);

			ATTR_NODISCARD bool executeFixedHitCountPolicy(const bool runBeforeHitPerAttempt, EffectContext &context,
														   const MoveMeta *moveMeta);

			void executeDamageApplication(const EffectContext &context, const BattleTarget &target);

			/*! @brief Dispatches an ability-and-item trigger for one active slot in a fresh suppression scope.
				@param[in] owner The active slot whose ability and item receive the trigger.
				@param[in] triggerID The trigger to dispatch.
				@param[in] eventTarget The optional event recipient used to initialize target coordinates; defaults to @p owner.
			*/
			void triggerSlot(const BattleTarget &owner, const BattleEventID eventID,
							 const std::optional<BattleTarget> &eventTarget = std::nullopt, BattleEventRole role = BattleEventRole::Any);

			/*! @brief Dispatches an ability-and-item trigger through an existing effect context.
				@details Activates all matching ability and item suppression rules before executing ability effects followed by item
			   effects, then restores the caller's source category.
				@param[in] owner The active slot whose current occupant receives the trigger.
				@param[in] triggerID The trigger to dispatch.
				@param[in,out] context The existing event or move context shared by the dispatched sources.
			*/
			void triggerSlotInContext(const BattleTarget &owner, const BattleEventID eventID, EffectContext &context,
									  BattleEventRole role = BattleEventRole::Any);

			/*! @brief Dispatches an occupant's ability and item sources through an existing effect context.
				@details Resolves source metadata, activates matching suppression rules, executes ability effects before item effects, and
			   restores the context's original source category. The caller validates whether the occupant is eligible for the event.
				@param[in] owner The active slot associated with the occupant's metadata sources.
				@param[in] pokemon The non-null occupant whose ability and item metadata are dispatched. The reference must remain valid for
			   the call.
				@param[in] triggerID The trigger dispatched to both metadata sources.
				@param[in,out] context The event or move context shared by the dispatched sources.
				@param[in] targeting Selects metadata target resolution or preservation of the context's existing target.
			*/
			void dispatchSlotSources(const BattleTarget &owner, const Pokemon *pokemon, const BattleEventID eventID,
									 const BattleEventRole role, EffectContext &context, const SlotTriggerTargeting targeting);

			/*! @brief Dispatches faint triggers once for each newly fainted active occupant.
				@details Marks each faint before dispatch to prevent recursive duplicate processing, then recomputes the battle phase and
			   required replacements.
			*/
			void processFaints();

			/*! @brief Recomputes the battle phase and forced replacement slots from current party health.
				@details Finished battles take precedence over replacement requests, and a fainted slot is requested only when its side has
			   a healthy inactive reserve.
			*/
			void refreshBattlePhase();

			/*! @brief Dispatches direct faint effects for the ability and item of a fainted occupant.
				@details Builds a context addressed to the fainted slot, activates both sources' suppression rules, and executes ability
			   effects before item effects without applying their target selectors.
				@param[in] faintedTarget The active slot containing the fainted occupant.
			*/
			void triggerFaint(const BattleTarget faintedTarget);

		private:
			/*! @brief Owns the parties, active slots, and mutable conditions for the current battle. */
			BattleState mState{};

			/*! @brief Stores suppression rules active in the current standalone trigger, move, target, or hit scope. */
			std::vector<ActiveSuppression> mActiveSuppressions{};

			/*! @brief Stores fainted active slots that must receive legal replacements before normal actions resume. */
			std::vector<BattleTarget> mRequiredReplacements{};

			/*! @brief Observes the registry bundle used for metadata lookup and effect execution.
				@details This non-owning pointer is initialized by the constructor and must remain valid for the engine's lifetime.
			*/
			const RegistryProvider *mProvider;

			/*! @brief Observes the registry that maps built-in effect identifiers to runtime effect metadata.
				@details This non-owning pointer is initialized by the constructor and must remain valid for the engine's lifetime.
			*/
			const EffectRegistry *mEffectRegistry;

			/*! @brief Records which category of external input the engine currently accepts. */
			BattlePhase mPhase{BattlePhase::NotStarted};
	};
} // namespace PocketCore::Battle

#endif
