/*! @file abilityRegistry.h
	@brief Provides fixed-capacity storage and lookup for built-in and user-defined abilities.
	@date 07/27/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_REGISTRY_ABILITY_REGISTRY_H
#define INCLUDE_REGISTRY_ABILITY_REGISTRY_H

#include <array>
#include <cassert>
#include <functional>
#include <optional>
#include <span>
#include <string_view>
#include <utility>

#include "Ability/abilityID.h"
#include "Ability/abilityMeta.h"
#include "Ability/abilityTargetsAndTriggers.h"
#include "Ability/builtinAbilityID.h"
#include "Ability/constants.h"
#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Effect/effectType.h"

namespace PocketCore::Registry::Ability
{
	using PocketCore::Ability::AbilityEffectTrigger;
	using PocketCore::Ability::AbilityID;
	using PocketCore::Ability::AbilityMeta;
	using PocketCore::Ability::AbilityTriggerID;
	using PocketCore::Ability::BuiltinAbilityID;
	using PocketCore::Ability::toAbilityID;
	using PocketCore::Configuration::MAX_ABILITIES;
	using PocketCore::Core::us;
	using PocketCore::Effect::EffectTypeID;

	/*! @class AbilityRegistry Registry/abilityRegistry.h
		@brief Stores built-in and user-defined ability metadata in fixed-capacity storage.
		@details Built-in abilities are registered during construction with IDs derived from @ref BuiltinAbilityID. Configuration code may
	   append, replace, or remove entries through the low-level mutators while battle-time callers use allocation-free lookup operations.
		@note Lookup operations are O(n), where n is bounded by @ref MAX_ABILITIES.
		@date 07/27/2026
		@version x.x.x
		@since x.x.x
		@author Matthew Moore
	*/
	class AbilityRegistry
	{
		public:
			/*! @brief Constructs a registry populated with every @ref BuiltinAbilityID. */
			explicit constexpr AbilityRegistry() : mNextAbilityID{static_cast<us>(toAbilityID(BuiltinAbilityID::Drizzle).getValue() + 1U)}
			{
				addBuiltin(
					{.mAbilityID = toAbilityID(BuiltinAbilityID::None), .mName = PocketCore::Ability::ABILITY_NAME_NONE, .mTriggers = {}});
				addBuiltin({
					.mAbilityID = toAbilityID(BuiltinAbilityID::Stench),
					.mName = PocketCore::Ability::ABILITY_NAME_STENCH,
					.mTriggers = {{.mTrigger = AbilityTriggerID::OnMoveUse, .mEffects = {EffectTypeID::Flinch}}},
				});
				addBuiltin({
					.mAbilityID = toAbilityID(BuiltinAbilityID::Drizzle),
					.mName = PocketCore::Ability::ABILITY_NAME_DRIZZLE,
					.mTriggers = {{.mTrigger = AbilityTriggerID::OnSwitchIn, .mEffects = {EffectTypeID::SetRain}}},
				});
			}

			/*! @brief Returns the metadata stored at an internal array index.
				@pre @p index < @ref MAX_ABILITIES.
				@param[in] index The internal array index.
				@return A const reference to the stored metadata that remains valid until the registry is mutated or destroyed.
			*/
			ATTR_NODISCARD constexpr const AbilityMeta &getEntry(const us index) const
			{
				assert(index < mAbilities.size());
				return mAbilities.at(index);
			}

			/*! @brief Looks up ability metadata by stable ID.
				@param[in] abilityID The stable ability identifier.
				@return A reference to the metadata if registered, or std::nullopt otherwise. The reference remains valid until mutation or
			   destruction.
			*/
			ATTR_NODISCARD constexpr std::optional<std::reference_wrapper<const AbilityMeta>> getAbilityMetadata(
				const AbilityID abilityID) const
			{
				const us index{findEntryIndexByID(abilityID)};

				if (index == mAmountRegistered)
				{
					return std::nullopt;
				}

				return std::cref(mAbilities.at(index));
			}

			/*! @brief Looks up an ability ID by display name.
				@param[in] name The case-sensitive display name.
				@return The stable ID if registered, or std::nullopt otherwise.
			*/
			ATTR_NODISCARD constexpr std::optional<AbilityID> getAbilityID(const std::string_view name) const
			{
				const us index{findEntryIndexByName(name)};

				if (index == mAmountRegistered)
				{
					return std::nullopt;
				}

				return mAbilities.at(index).mAbilityID;
			}

			/*! @brief Looks up an ability display name by stable ID.
				@param[in] abilityID The stable ability identifier.
				@return The display name if registered, or std::nullopt otherwise.
			*/
			ATTR_NODISCARD constexpr std::optional<std::string_view> getAbilityName(const AbilityID abilityID) const
			{
				const auto metadata{getAbilityMetadata(abilityID)};

				if (!metadata.has_value())
				{
					return std::nullopt;
				}

				return metadata->get().mName;
			}

			/*! @brief Returns all currently registered ability definitions.
				@return A read-only span that remains valid until the registry is mutated or destroyed.
			*/
			ATTR_NODISCARD constexpr std::span<const AbilityMeta> getRegisteredAbilities() const noexcept
			{
				return {mAbilities.data(), mAmountRegistered};
			}

			/*! @brief Returns the number of registered abilities.
				@return The number of valid entries in the registry.
			*/
			ATTR_NODISCARD constexpr us getAmountRegistered() const noexcept
			{
				return mAmountRegistered;
			}

			/*! @brief Returns the next stable ID assigned to a custom ability.
				@return The underlying numeric value of the next ability ID.
			*/
			ATTR_NODISCARD constexpr us getNextAbilityID() const noexcept
			{
				return mNextAbilityID;
			}

			/*! @brief Finds an internal array index by stable ability ID.
				@param[in] abilityID The stable ability identifier.
				@return The internal index if registered, or std::nullopt otherwise.
			*/
			ATTR_NODISCARD constexpr std::optional<us> findIndexByAbilityID(const AbilityID abilityID) const
			{
				const us index{findEntryIndexByID(abilityID)};

				if (index == mAmountRegistered)
				{
					return std::nullopt;
				}

				return index;
			}

			/*! @brief Checks whether an ability name is registered.
				@param[in] name The case-sensitive display name.
				@return True if the name is registered, otherwise false.
			*/
			ATTR_NODISCARD constexpr bool hasAbility(const std::string_view name) const
			{
				return findEntryIndexByName(name) != mAmountRegistered;
			}

			/*! @brief Checks whether an ability ID is registered.
				@param[in] abilityID The stable ability identifier.
				@return True if the ID is registered, otherwise false.
			*/
			ATTR_NODISCARD constexpr bool hasAbility(const AbilityID abilityID) const
			{
				return findEntryIndexByID(abilityID) != mAmountRegistered;
			}

			/*! @brief Replaces the metadata at an internal array index.
				@pre @p index < @ref MAX_ABILITIES.
				@param[in] index The internal array index.
				@param[in] metadata The complete metadata to store.
			*/
			constexpr void setEntry(const us index, const AbilityMeta &metadata)
			{
				assert(index < mAbilities.size());
				mAbilities.at(index) = metadata;
			}

			/*! @brief Sets the registered entry count.
				@param[in] amount The new number of valid entries.
			*/
			constexpr void setAmountRegistered(const us amount) noexcept
			{
				mAmountRegistered = amount;
			}

			/*! @brief Sets the next custom ability ID counter.
				@param[in] nextID The next underlying ID value.
			*/
			constexpr void setNextAbilityID(const us nextID) noexcept
			{
				mNextAbilityID = nextID;
			}

			/*! @brief Increments the registered entry count. */
			constexpr void incrementAmountRegistered() noexcept
			{
				++mAmountRegistered;
			}

			/*! @brief Decrements the registered entry count. */
			constexpr void decrementAmountRegistered() noexcept
			{
				--mAmountRegistered;
			}

			/*! @brief Increments the next custom ability ID counter. */
			constexpr void incrementNextAbilityID() noexcept
			{
				++mNextAbilityID;
			}

		private:
			ATTR_NODISCARD constexpr us findEntryIndexByName(const std::string_view name) const
			{
				for (us index{0}; index < mAmountRegistered; ++index)
				{
					if (mAbilities.at(index).mName == name)
					{
						return index;
					}
				}

				return mAmountRegistered;
			}

			ATTR_NODISCARD constexpr us findEntryIndexByID(const AbilityID abilityID) const
			{
				for (us index{0}; index < mAmountRegistered; ++index)
				{
					if (mAbilities.at(index).mAbilityID == abilityID)
					{
						return index;
					}
				}

				return mAmountRegistered;
			}

			constexpr void addBuiltin(AbilityMeta metadata)
			{
				assert(mAmountRegistered < mAbilities.size());
				mAbilities.at(mAmountRegistered) = std::move(metadata);
				++mAmountRegistered;
			}

		private:
			std::array<AbilityMeta, MAX_ABILITIES> mAbilities{};
			us mAmountRegistered{0};
			us mNextAbilityID{0};
	};
} // namespace PocketCore::Registry::Ability

#endif
