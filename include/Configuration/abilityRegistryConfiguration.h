/*! @file abilityRegistryConfiguration.h
	@brief Declares the user-facing facade for configuring ability metadata.
	@date 08/26/2026
	@since 0.4.0
	@version 0.12.7
	@author Matthew Moore
*/

#ifndef INCLUDE_CONFIGURATION_ABILITY_REGISTRY_CONFIGURATION_H
#define INCLUDE_CONFIGURATION_ABILITY_REGISTRY_CONFIGURATION_H

#include <expected>
#include <optional>
#include <span>
#include <string_view>

#include "Ability/abilityID.h"
#include "Ability/abilityMeta.h"
#include "Battle/battleTargetsAndTriggers.h"
#include "Configuration/constants.h"
#include "Configuration/fixedMetadataRegistryConfiguration.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Effect/effectTrigger.h"
#include "Registry/abilityRegistry.h"

namespace PocketCore::Configuration
{
	using PocketCore::Ability::AbilityID;
	using PocketCore::Ability::AbilityMeta;
	using PocketCore::Battle::BattleTargetID;
	using PocketCore::Core::us;
	using PocketCore::Effect::EffectTrigger;
	using PocketCore::Registry::Ability::AbilityRegistry;
	using PocketCore::Registry::RegistryError;

	namespace Detail
	{
		/*! @struct AbilityRegistryConfigurationPolicy Configuration/abilityRegistryConfiguration.h
			@brief Policy class providing error codes and display strings for ability registry configuration.
			@details Encapsulates the ability-specific error categories and display names used by the generic
			 @ref FixedMetadataRegistryConfiguration template to report validation and lookup failures with
			 domain-specific terminology.
			@date 07/27/2026
			@since 0.5.0
			@version 0.5.1
			@author Matthew Moore
		*/
		struct AbilityRegistryConfigurationPolicy
		{
			public:
				/*! @brief The display name of the configuration system. */
				static constexpr std::string_view configurationName{"AbilityRegistryConfiguration"};

				/*! @brief The singular entity type managed by this configuration. */
				static constexpr std::string_view entityName{"ability"};

				/*! @brief The error code returned when a duplicate ability name is registered. */
				static constexpr RegistryError duplicateError{RegistryError::DuplicateAbility};

				/*! @brief The error code returned when an ability lookup fails. */
				static constexpr RegistryError notFoundError{RegistryError::AbilityNotFound};
		};
	} // namespace Detail

	/*! @class AbilityRegistryConfiguration Configuration/abilityRegistryConfiguration.h
		@brief Provides validated user customization over an internal ability registry.
		@details Supports lookup, addition, batch addition, trigger replacement, renaming, and removal. Custom IDs are assigned
	   monotonically and are not reused after removal. Batch additions provide all-or-nothing semantics.
		@date 08/26/2026
		@since 0.4.0
		@version 0.12.7
		@author Matthew Moore
	*/
	class AbilityRegistryConfiguration
		: private FixedMetadataRegistryConfiguration<AbilityRegistry, AbilityMeta, AbilityID, MAX_ABILITIES, &AbilityMeta::mAbilityID,
													 Detail::AbilityRegistryConfigurationPolicy>
	{
		private:
			using Base = FixedMetadataRegistryConfiguration<AbilityRegistry, AbilityMeta, AbilityID, MAX_ABILITIES,
															&AbilityMeta::mAbilityID, Detail::AbilityRegistryConfigurationPolicy>;

		public:
			/*! @brief Constructs a configuration containing all built-in abilities.
				@since 0.4.0
				@version 0.4.0
			 */
			constexpr AbilityRegistryConfiguration() = default;

			using Base::getAmountRegistered;

			/*! @brief Returns read-only access to the configured runtime ability registry.
				@return A reference that remains valid for the lifetime of this configuration.
				@since 0.10.3
				@version 0.10.3
			*/
			ATTR_NODISCARD constexpr const AbilityRegistry &getRuntimeRegistry() const noexcept
			{
				return getRegistry();
			}

			/*! @brief Looks up complete metadata by stable ability ID.
				@param[in] abilityID The built-in or custom stable identifier.
				@return A non-owning pointer to metadata if registered, or nullptr otherwise. The pointer remains valid until replacement or
			   configuration destruction.
				@since 0.4.0
				@version 0.5.1
			*/
			ATTR_NODISCARD constexpr const AbilityMeta *getAbilityMetadata(const AbilityID abilityID) const
			{
				return getMetadata(abilityID);
			}

			/*! @brief Looks up a stable ability ID by display name.
				@param[in] name The case-sensitive display name.
				@return The stable ID if registered, or std::nullopt otherwise.
				@since 0.4.0
				@version 0.5.1
			*/
			ATTR_NODISCARD constexpr const std::optional<AbilityID> getAbilityID(const std::string_view &name) const
			{
				return getID(name);
			}

			/*! @brief Looks up a display name by stable ability ID.
				@param[in] abilityID The built-in or custom stable identifier.
				@return The display name if registered, or std::nullopt otherwise.
				@since 0.4.0
				@version 0.5.1
			*/
			ATTR_NODISCARD constexpr const std::optional<std::string_view> getAbilityName(const AbilityID abilityID) const
			{
				return getName(abilityID);
			}

			/*! @brief Returns all currently registered ability definitions.
				@return A read-only span that remains valid until mutation or destruction.
				@since 0.4.0
				@version 0.5.1
			*/
			ATTR_NODISCARD constexpr const std::span<const AbilityMeta> getRegisteredAbilities() const noexcept
			{
				return getRegisteredEntries();
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
				@param[in] abilityID The built-in or custom stable identifier.
				@return True if the ID is registered, otherwise false.
				@since 0.4.0
				@version 0.5.0
			*/
			ATTR_NODISCARD constexpr bool hasAbility(const AbilityID abilityID) const
			{
				return hasEntry(abilityID);
			}

			/*! @brief Registers one user-defined ability and assigns a stable ID.
				@param[in] abilityMeta The name and trigger metadata to copy into the registry.
				@return The assigned ID on success, or @ref RegistryErrorInfo on duplicate name or exhausted capacity.
				@since 0.4.0
				@version 0.6.0
			*/
			ATTR_NODISCARD std::expected<AbilityID, RegistryErrorInfo> addAbility(const AbilityMeta &abilityMeta);

			/*! @brief Registers multiple abilities atomically.
				@details Restores the complete prior registry state if any definition fails validation.
				@param[in] abilityMetas The ability definitions to register in order.
				@return Void on success, or the first @ref RegistryErrorInfo on failure.
				@since 0.4.0
				@version 0.6.0
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> addAbilities(const std::span<const AbilityMeta> &abilityMetas);

			/*! @brief Replaces all trigger metadata for an ability selected by name.
				@param[in] abilityName The registered display name.
				@param[in] triggers The trigger definitions to copy into the registry.
				@return Void on success, or @ref RegistryErrorInfo if the ability is not registered.
				@since 0.4.0
				@version 0.12.6
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> setAbilityTriggers(const std::string_view &abilityName,
																					 const std::span<const EffectTrigger> &triggers);

			/*! @overload std::expected<void, RegistryErrorInfo> setAbilityTriggers(AbilityID, std::span<const EffectTrigger>)
				@brief Replaces all trigger metadata for an ability selected by stable ID.
				@param[in] abilityID The built-in or custom stable identifier.
				@param[in] triggers The trigger definitions to copy into the registry.
				@return Void on success, or @ref RegistryErrorInfo if the ability is not registered.
				@since 0.4.0
				@version 0.12.6
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> setAbilityTriggers(const AbilityID abilityID,
																					 const std::span<const EffectTrigger> &triggers);

			/*! @brief Replaces all trigger metadata for an ability selected by name.
				@param[in] abilityName The registered display name.
				@param[in] target The target to copy into the registry.
				@return Void on success, or @ref RegistryErrorInfo if the ability is not registered.
				@since 0.6.0
				@version 0.9.16
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> setAbilityTarget(const std::string_view &abilityName,
																				   const BattleTargetID target);

			/*! @overload std::expected<void, RegistryErrorInfo> setAbilityTarget(AbilityID, const BattleTargetID target)
				@brief Replaces all trigger metadata for an ability selected by stable ID.
				@param[in] abilityID The built-in or custom stable identifier.
				@param[in] target The target to copy into the registry.
				@return Void on success, or @ref RegistryErrorInfo if the ability is not registered.
				@since 0.6.0
				@version 0.9.16
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> setAbilityTarget(const AbilityID abilityID, const BattleTargetID target);

			/*! @brief Renames an ability without changing its other metadata.
				@details @p newName is stored as a non-owning view and its backing storage must remain valid while registered.
				@param[in] oldName The currently registered display name.
				@param[in] newName The unique replacement display name.
				@return Void on success, or @ref RegistryErrorInfo if the source is absent or target name is already registered.
				@since 0.4.0
				@version 0.5.1
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> renameAbility(const std::string_view &oldName,
																				const std::string_view &newName);

			/*! @brief Replaces all ability metadata for an ability selected by stable ID.
				@param[in] abilityName The registered display name.
				@param[in] abilityMeta The metadata to copy into the registry.
				@return Void on success, or @ref RegistryErrorInfo if the ability is not registered.
				@since 0.6.0
				@version 0.6.0
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> updateAbility(const std::string_view &abilityName,
																				const AbilityMeta &abilityMeta);

			/*! @overload std::expected<void, RegistryErrorInfo> updateAbility(AbilityID, const AbilityMeta &abilityMeta)
				@brief Replaces all ability metadata for an ability selected by stable ID.
				@param[in] abilityID The built-in or custom stable identifier.
				@param[in] abilityMeta The metadata to copy into the registry.
				@return Void on success, or @ref RegistryErrorInfo if the ability is not registered.
				@since 0.6.0
				@version 0.6.0
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> updateAbility(const AbilityID abilityID, const AbilityMeta &abilityMeta);

			/*! @brief Removes an ability by display name.
				@param[in] abilityName The registered display name.
				@return The removed stable ID on success, or @ref RegistryErrorInfo if no matching ability exists.
				@since 0.4.0
				@version 0.5.1
			*/
			ATTR_NODISCARD std::expected<AbilityID, RegistryErrorInfo> removeAbility(const std::string_view &abilityName);

			/*! @overload std::expected<AbilityID, RegistryErrorInfo> removeAbility(AbilityID)
				@brief Removes an ability by stable ID.
				@param[in] abilityID The built-in or custom stable identifier.
				@return The removed stable ID on success, or @ref RegistryErrorInfo if no matching ability exists.
				@since 0.4.0
				@version 0.5.1
			*/
			ATTR_NODISCARD std::expected<AbilityID, RegistryErrorInfo> removeAbility(const AbilityID abilityID);
	};
} // namespace PocketCore::Configuration

#endif
