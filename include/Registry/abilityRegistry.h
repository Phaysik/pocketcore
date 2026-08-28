/*! @file abilityRegistry.h
	@brief Provides fixed-capacity storage and lookup for built-in and user-defined abilities.
	@date 08/18/2026
	@since 0.4.0
	@version 0.10.8
	@author Matthew Moore
*/

#ifndef INCLUDE_REGISTRY_ABILITY_REGISTRY_H
#define INCLUDE_REGISTRY_ABILITY_REGISTRY_H

#include <optional>
#include <span>
#include <string_view>

#include "Ability/abilityID.h"
#include "Ability/abilityMeta.h"
#include "Ability/builtInAbilityID.h"
#include "Ability/constants.h"
#include "Battle/battleTargetsAndTriggers.h"
#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Effect/builtInEffectID.h"
#include "Registry/fixedMetadataRegistry.h"

namespace PocketCore::Registry::Ability
{
	using PocketCore::Ability::AbilityID;
	using PocketCore::Ability::AbilityMeta;
	using PocketCore::Ability::BuiltinAbilityID;
	using PocketCore::Ability::toAbilityID;
	using PocketCore::Battle::BattleEventID;
	using PocketCore::Battle::BattleEventRole;
	using PocketCore::Battle::BattleTargetID;
	using PocketCore::Configuration::MAX_ABILITIES;
	using PocketCore::Core::us;
	using PocketCore::Effect::BuiltinEffectID;
	using PocketCore::Effect::toEffectID;
	using PocketCore::Registry::FixedMetadataRegistry;

	/*! @class AbilityRegistry Registry/abilityRegistry.h
		@brief Stores built-in and user-defined ability metadata in fixed-capacity storage.
		@details Built-in abilities are registered during construction with IDs derived from @ref BuiltinAbilityID. Configuration code may
	   append, replace, or remove entries through the low-level mutators while battle-time callers use allocation-free lookup operations.
		@note Lookup operations are O(n), where n is bounded by @ref MAX_ABILITIES.
		@date 08/18/2026
		@since 0.4.0
		@version 0.10.8
		@author Matthew Moore
	*/
	class AbilityRegistry : private FixedMetadataRegistry<AbilityMeta, AbilityID, MAX_ABILITIES, &AbilityMeta::mAbilityID>
	{
		private:
			using Base = FixedMetadataRegistry<AbilityMeta, AbilityID, MAX_ABILITIES, &AbilityMeta::mAbilityID>;

		public:
			// LCOV_EXCL_START - If the built in additions fail, the program wouldn't work anyway
			/*! @brief Constructs a registry populated with every @ref BuiltinAbilityID.
				@since 0.4.0
				@version 0.10.8
			 */
			ATTR_NOINLINE explicit constexpr AbilityRegistry() : Base{toAbilityID(BuiltinAbilityID::FinalAbility).getValue()}
			{
				addBuiltin({
					.mTriggers = {},
					.mName = PocketCore::Ability::ABILITY_NAME_NONE,
					.mAbilityID = toAbilityID(BuiltinAbilityID::None),
				});
				addBuiltin({
					.mTriggers
					= {{.mEffects = {toEffectID(BuiltinEffectID::Flinch)}, .mTrigger = BattleEventID::Hit, .mRole = BattleEventRole::User}},
					.mName = PocketCore::Ability::ABILITY_NAME_STENCH,
					.mAbilityID = toAbilityID(BuiltinAbilityID::Stench),
					.mTargetID = BattleTargetID::SingleOpponent,
				});
				addBuiltin({
					.mTriggers = {{.mEffects = {toEffectID(BuiltinEffectID::SetRain)}, .mTrigger = BattleEventID::SwitchIn}},
					.mName = PocketCore::Ability::ABILITY_NAME_DRIZZLE,
					.mAbilityID = toAbilityID(BuiltinAbilityID::Drizzle),
					.mTargetID = BattleTargetID::Self,
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

			/*! @brief Looks up ability metadata by stable ID.
				@param[in] abilityID The stable ability identifier.
				@return A non-owning pointer to metadata if registered, or nullptr otherwise. The pointer remains valid until replacement or
			   registry destruction.
				@since 0.4.0
				@version 0.5.1
			*/
			ATTR_NODISCARD constexpr const AbilityMeta *getAbilityMetadata(const AbilityID abilityID) const
			{
				return getMetadata(abilityID);
			}

			/*! @brief Looks up an ability ID by display name.
				@param[in] name The case-sensitive display name.
				@return The stable ID if registered, or std::nullopt otherwise.
				@since 0.4.0
				@version 0.5.1
			*/
			ATTR_NODISCARD constexpr const std::optional<AbilityID> getAbilityID(const std::string_view &name) const
			{
				return getID(name);
			}

			/*! @brief Looks up an ability display name by stable ID.
				@param[in] abilityID The stable ability identifier.
				@return The display name if registered, or std::nullopt otherwise.
				@since 0.4.0
				@version 0.5.1
			*/
			ATTR_NODISCARD constexpr const std::optional<std::string_view> getAbilityName(const AbilityID abilityID) const
			{
				return getName(abilityID);
			}

			/*! @brief Returns all currently registered ability definitions.
				@return A read-only span that remains valid until the registry is mutated or destroyed.
				@since 0.4.0
				@version 0.5.1
			*/
			ATTR_NODISCARD constexpr const std::span<const AbilityMeta> getRegisteredAbilities() const noexcept
			{
				return getRegisteredEntries();
			}

			/*! @brief Returns the next stable ID assigned to a custom ability.
				@return The underlying numeric value of the next ability ID.
				@since 0.4.0
				@version 0.5.0
			*/
			ATTR_NODISCARD constexpr us getNextAbilityID() const noexcept
			{
				return getNextID();
			}

			/*! @brief Finds an internal array index by stable ability ID.
				@param[in] abilityID The stable ability identifier.
				@return The internal index if registered, or std::nullopt otherwise.
				@since 0.4.0
				@version 0.5.1
			*/
			ATTR_NODISCARD constexpr const std::optional<us> findIndexByAbilityID(const AbilityID abilityID) const
			{
				return findIndexByID(abilityID);
			}

			/*! @brief Checks whether an ability name is registered.
				@param[in] name The case-sensitive display name.
				@return True if the name is registered, otherwise false.
				@since 0.4.0
				@version 0.5.1
			*/
			ATTR_NODISCARD constexpr bool hasAbility(const std::string_view &name) const
			{
				return hasEntry(name);
			}

			/*! @brief Checks whether an ability ID is registered.
				@param[in] abilityID The stable ability identifier.
				@return True if the ID is registered, otherwise false.
				@since 0.4.0
				@version 0.5.0
			*/
			ATTR_NODISCARD constexpr bool hasAbility(const AbilityID abilityID) const
			{
				return hasEntry(abilityID);
			}

			/*! @brief Sets the next custom ability ID counter.
				@param[in] nextID The next underlying ID value.
				@since 0.4.0
				@version 0.5.0
			*/
			constexpr void setNextAbilityID(const us nextID) noexcept
			{
				setNextID(nextID);
			}

			/*! @brief Increments the next custom ability ID counter.
				@since 0.4.0
				@version 0.5.0
			 */
			constexpr void incrementNextAbilityID() noexcept
			{
				incrementNextID();
			}
	};
} // namespace PocketCore::Registry::Ability

#endif
