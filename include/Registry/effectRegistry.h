/*! @file effectRegistry.h
	@brief Provides fixed-capacity storage and lookup for built-in and user-defined effects.
	@date 08/22/2026
	@since 0.10.0
	@version 0.11.6
	@author Matthew Moore
*/

#ifndef INCLUDE_REGISTRY_EFFECT_REGISTRY_H
#define INCLUDE_REGISTRY_EFFECT_REGISTRY_H

#include <optional>
#include <span>
#include <string_view>

#include "Battle/battleTargetsAndTriggers.h"
#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Effect/builtInEffectID.h"
#include "Effect/constants.h"
#include "Effect/effectID.h"
#include "Effect/effectMeta.h"
#include "EffectHandler/accuracyCheckHandler.h"
#include "EffectHandler/baseDamageHandler.h"
#include "EffectHandler/burnDamageHandler.h"
#include "EffectHandler/criticalHitHandler.h"
#include "EffectHandler/flinchHandler.h"
#include "EffectHandler/populationBombHandler.h"
#include "EffectHandler/psychicTerrainPriorityBlockHandler.h"
#include "EffectHandler/randomizationHandler.h"
#include "EffectHandler/recoilHandler.h"
#include "EffectHandler/setRainHandler.h"
#include "EffectHandler/setSandstormHandler.h"
#include "EffectHandler/setSunHandler.h"
#include "EffectHandler/stabHandler.h"
#include "EffectHandler/statusApplyHandler.h"
#include "EffectHandler/statusRemoveHandler.h"
#include "EffectHandler/statusTickHandler.h"
#include "EffectHandler/statusTurnSkipHandler.h"
#include "EffectHandler/targetsHandler.h"
#include "EffectHandler/terrainHandler.h"
#include "EffectHandler/typeEffectivenessHandler.h"
#include "EffectHandler/weatherHandler.h"
#include "Registry/fixedMetadataRegistry.h"

namespace PocketCore::Registry::Effect
{
	using PocketCore::Battle::BattleEventID;
	using PocketCore::Battle::BattleTargetID;
	using PocketCore::Configuration::MAX_EFFECTS;
	using PocketCore::Core::us;
	using PocketCore::Effect::applyAccuracyCheck;
	using PocketCore::Effect::applyBaseDamage;
	using PocketCore::Effect::applyBurnDamage;
	using PocketCore::Effect::applyCriticalHit;
	using PocketCore::Effect::applyFlinch;
	using PocketCore::Effect::applyPopulationBomb;
	using PocketCore::Effect::applyPsychicTerrainPriorityBlock;
	using PocketCore::Effect::applyRandomization;
	using PocketCore::Effect::applyRecoil;
	using PocketCore::Effect::applySetRain;
	using PocketCore::Effect::applySetSandstorm;
	using PocketCore::Effect::applySetSun;
	using PocketCore::Effect::applySTAB;
	using PocketCore::Effect::applyStatusApply;
	using PocketCore::Effect::applyStatusRemove;
	using PocketCore::Effect::applyStatusTick;
	using PocketCore::Effect::applyStatusTurnSkip;
	using PocketCore::Effect::applyTargets;
	using PocketCore::Effect::applyTerrain;
	using PocketCore::Effect::applyTypeEffectiveness;
	using PocketCore::Effect::applyWeather;
	using PocketCore::Effect::BuiltinEffectID;
	using PocketCore::Effect::EffectID;
	using PocketCore::Effect::EffectMeta;
	using PocketCore::Effect::toEffectID;
	using PocketCore::Registry::FixedMetadataRegistry;

	/*! @class EffectRegistry Registry/effectRegistry.h
		@brief Stores built-in and user-defined effect metadata in fixed-capacity storage.
		@details Built-in effects are registered during construction with IDs derived from @ref BuiltinEffectID. Configuration code may
	   append, replace, or remove entries through the low-level mutators while battle-time callers use allocation-free lookup operations.
		@note Lookup operations are O(n), where n is bounded by @ref MAX_EFFECTS.
		@date 08/22/2026
		@since 0.10.0
		@version 0.11.6
		@author Matthew Moore
	*/
	class EffectRegistry : private FixedMetadataRegistry<EffectMeta, EffectID, MAX_EFFECTS, &EffectMeta::mEffectID>
	{
		private:
			using Base = FixedMetadataRegistry<EffectMeta, EffectID, MAX_EFFECTS, &EffectMeta::mEffectID>;

		public:
			// LCOV_EXCL_START - If the built in additions fail, the program wouldn't work anyway
			/*! @brief Constructs a registry populated with every @ref BuiltinEffectID.
				@since 0.10.0
				@version 0.11.6
			 */
			ATTR_NOINLINE explicit constexpr EffectRegistry() : Base{toEffectID(BuiltinEffectID::FinalEffect).getValue()}
			{
				addBuiltin({.mName = PocketCore::Effect::EFFECT_NAME_NONE, .mEffectID = toEffectID(BuiltinEffectID::None)});
				addBuiltin({
					.mName = PocketCore::Effect::EFFECT_NAME_CRITICAL_HIT,
					.mApply = applyCriticalHit,
					.mEffectID = toEffectID(BuiltinEffectID::CriticalHit),
				});
				addBuiltin({
					.mName = PocketCore::Effect::EFFECT_NAME_BASE_DAMAGE,
					.mApply = applyBaseDamage,
					.mEffectID = toEffectID(BuiltinEffectID::BaseDamage),
				});
				addBuiltin({
					.mName = PocketCore::Effect::EFFECT_NAME_TARGETS,
					.mApply = applyTargets,
					.mEffectID = toEffectID(BuiltinEffectID::Targets),
				});
				addBuiltin({
					.mName = PocketCore::Effect::EFFECT_NAME_WEATHER,
					.mApply = applyWeather,
					.mEffectID = toEffectID(BuiltinEffectID::Weather),
				});
				addBuiltin({
					.mName = PocketCore::Effect::EFFECT_NAME_TERRAIN,
					.mApply = applyTerrain,
					.mEffectID = toEffectID(BuiltinEffectID::Terrain),
				});
				addBuiltin({
					.mName = PocketCore::Effect::EFFECT_NAME_POPULATION_BOMB,
					.mApply = applyPopulationBomb,
					.mEffectID = toEffectID(BuiltinEffectID::PopulationBomb),
				});
				addBuiltin({
					.mName = PocketCore::Effect::EFFECT_NAME_RANDOMIZATION,
					.mApply = applyRandomization,
					.mEffectID = toEffectID(BuiltinEffectID::Randomization),
				});
				addBuiltin({
					.mName = PocketCore::Effect::EFFECT_NAME_STAB,
					.mApply = applySTAB,
					.mEffectID = toEffectID(BuiltinEffectID::Stab),
				});
				addBuiltin({
					.mName = PocketCore::Effect::EFFECT_NAME_TYPE_EFFECTIVENESS,
					.mApply = applyTypeEffectiveness,
					.mEffectID = toEffectID(BuiltinEffectID::TypeEffectiveness),
				});
				addBuiltin({
					.mName = PocketCore::Effect::EFFECT_NAME_BURN_DAMAGE_REDUCTION,
					.mApply = applyBurnDamage,
					.mEffectID = toEffectID(BuiltinEffectID::BurnDamageReduction),
				});
				addBuiltin({
					.mName = PocketCore::Effect::EFFECT_NAME_FLINCH,
					.mApply = applyFlinch,
					.mEffectID = toEffectID(BuiltinEffectID::Flinch),
				});
				addBuiltin({
					.mName = PocketCore::Effect::EFFECT_NAME_RECOIL,
					.mApply = applyRecoil,
					.mEffectID = toEffectID(BuiltinEffectID::Recoil),
				});
				addBuiltin({
					.mName = PocketCore::Effect::EFFECT_NAME_STATUS_APPLY,
					.mApply = applyStatusApply,
					.mEffectID = toEffectID(BuiltinEffectID::StatusApply),
					.mMayChangeStatus = true,
				});
				addBuiltin({
					.mName = PocketCore::Effect::EFFECT_NAME_STATUS_REMOVE,
					.mApply = applyStatusRemove,
					.mEffectID = toEffectID(BuiltinEffectID::StatusRemove),
					.mMayChangeStatus = true,
				});
				addBuiltin({
					.mName = PocketCore::Effect::EFFECT_NAME_STATUS_TURN_SKIP,
					.mApply = applyStatusTurnSkip,
					.mEffectID = toEffectID(BuiltinEffectID::StatusTurnSkip),
				});
				addBuiltin({
					.mName = PocketCore::Effect::EFFECT_NAME_STATUS_TICK,
					.mApply = applyStatusTick,
					.mEffectID = toEffectID(BuiltinEffectID::StatusTick),
				});
				addBuiltin({
					.mName = PocketCore::Effect::EFFECT_NAME_ACCURACY_CHECK,
					.mApply = applyAccuracyCheck,
					.mEffectID = toEffectID(BuiltinEffectID::AccuracyCheck),
				});
				addBuiltin({
					.mName = PocketCore::Effect::EFFECT_NAME_SET_SANDSTORM,
					.mApply = applySetSandstorm,
					.mEffectID = toEffectID(BuiltinEffectID::SetSandstorm),
					.mMayChangeWeather = true,
				});
				addBuiltin({
					.mName = PocketCore::Effect::EFFECT_NAME_SET_SUN,
					.mApply = applySetSun,
					.mEffectID = toEffectID(BuiltinEffectID::SetSun),
					.mMayChangeWeather = true,
				});
				addBuiltin({
					.mName = PocketCore::Effect::EFFECT_NAME_SET_RAIN,
					.mApply = applySetRain,
					.mEffectID = toEffectID(BuiltinEffectID::SetRain),
					.mMayChangeWeather = true,
				});
				addBuiltin({
					.mName = PocketCore::Effect::EFFECT_NAME_PSYCHIC_TERRAIN_PRIORITY_BLOCK,
					.mApply = applyPsychicTerrainPriorityBlock,
					.mEffectID = toEffectID(BuiltinEffectID::PsychicTerrainPriorityBlock),
				});
			}

			// LCOV_EXCL_STOP

			using Base::decrementAmountRegistered;
			using Base::eraseEntry;
			using Base::findIndexByID;
			using Base::getAmountRegistered;
			using Base::getEntry;
			using Base::getID;
			using Base::getMetadata;
			using Base::getName;
			using Base::getNextID;
			using Base::getRegisteredEntries;
			using Base::hasEntry;
			using Base::incrementAmountRegistered;
			using Base::incrementNextID;
			using Base::setAmountRegistered;
			using Base::setEntry;
			using Base::setNextID;

			/*! @brief Looks up effect metadata by stable ID.
				@param[in] effectID The stable effect identifier.
				@return A non-owning pointer to metadata if registered, or nullptr otherwise. The pointer remains valid until replacement or
			   registry destruction.
				@since 0.10.0
				@version 0.10.0
			*/
			ATTR_NODISCARD constexpr const EffectMeta *getEffectMetadata(const EffectID effectID) const
			{
				return getMetadata(effectID);
			}

			/*! @brief Looks up an effect ID by display name.
				@param[in] name The case-sensitive display name.
				@return The stable ID if registered, or std::nullopt otherwise.
				@since 0.10.0
				@version 0.10.0
			*/
			ATTR_NODISCARD constexpr const std::optional<EffectID> getEffectID(const std::string_view &name) const
			{
				return getID(name);
			}

			/*! @brief Looks up an effect display name by stable ID.
				@param[in] effectID The stable effect identifier.
				@return The display name if registered, or std::nullopt otherwise.
				@since 0.10.0
				@version 0.10.0
			*/
			ATTR_NODISCARD constexpr const std::optional<std::string_view> getEffectName(const EffectID effectID) const
			{
				return getName(effectID);
			}

			/*! @brief Returns all currently registered effect definitions.
				@return A read-only span that remains valid until the registry is mutated or destroyed.
				@since 0.10.0
				@version 0.10.0
			*/
			ATTR_NODISCARD constexpr const std::span<const EffectMeta> getRegisteredEffects() const noexcept
			{
				return getRegisteredEntries();
			}

			/*! @brief Returns the next stable ID assigned to a custom effect.
				@return The underlying numeric value of the next effect ID.
				@since 0.10.0
				@version 0.10.0
			*/
			ATTR_NODISCARD constexpr us getNextEffectID() const noexcept
			{
				return getNextID();
			}

			/*! @brief Finds an internal array index by stable effect ID.
				@param[in] effectID The stable effect identifier.
				@return The internal index if registered, or std::nullopt otherwise.
				@since 0.10.0
				@version 0.10.0
			*/
			ATTR_NODISCARD constexpr const std::optional<us> findIndexByEffectID(const EffectID effectID) const
			{
				return findIndexByID(effectID);
			}

			/*! @brief Checks whether an effect name is registered.
				@param[in] name The case-sensitive display name.
				@return True if the name is registered, otherwise false.
				@since 0.10.0
				@version 0.10.0
			*/
			ATTR_NODISCARD constexpr bool hasEffect(const std::string_view &name) const
			{
				return hasEntry(name);
			}

			/*! @brief Checks whether an effect ID is registered.
				@param[in] effectID The stable effect identifier.
				@return True if the ID is registered, otherwise false.
				@since 0.10.0
				@version 0.10.0
			*/
			ATTR_NODISCARD constexpr bool hasEffect(const EffectID effectID) const
			{
				return hasEntry(effectID);
			}

			/*! @brief Sets the next custom effect ID counter.
				@param[in] nextID The next underlying ID value.
				@since 0.10.0
				@version 0.10.0
			*/
			constexpr void setNextEffectID(const us nextID) noexcept
			{
				setNextID(nextID);
			}

			/*! @brief Increments the next custom effect ID counter.
				@since 0.10.0
				@version 0.10.0
			 */
			constexpr void incrementNextEffectID() noexcept
			{
				incrementNextID();
			}
	};
} // namespace PocketCore::Registry::Effect

#endif
