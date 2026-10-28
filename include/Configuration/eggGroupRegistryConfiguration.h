/*! @file eggGroupRegistryConfiguration.h
	@brief Declares the user-facing facade for configuring eggGroup metadata.
	@date 10/26/2026
	@since 0.12.53
	@version 0.12.53
	@author Matthew Moore
*/

#ifndef INCLUDE_CONFIGURATION_EGGGROUP_REGISTRY_CONFIGURATION_H
#define INCLUDE_CONFIGURATION_EGGGROUP_REGISTRY_CONFIGURATION_H

#include <expected>
#include <optional>
#include <span>
#include <string_view>

#include "Configuration/constants.h"
#include "Configuration/fixedMetadataRegistryConfiguration.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "EggGroup/eggGroupID.h"
#include "EggGroup/eggGroupMeta.h"
#include "Registry/eggGroupRegistry.h"
#include "Validation/Registry/registryError.h"

namespace PocketCore::Configuration
{
	using PocketCore::Core::us;
	using PocketCore::EggGroup::EggGroupID;
	using PocketCore::EggGroup::EggGroupMeta;
	using PocketCore::Registry::EggGroup::EggGroupRegistry;
	using PocketCore::Validation::Registry::RegistryError;

	namespace Detail
	{
		/*! @struct EggGroupRegistryConfigurationPolicy Configuration/eggGroupRegistryConfiguration.h
			@brief Policy class providing error codes and display strings for eggGroup registry configuration.
			@details Encapsulates the eggGroup-specific error categories and display names used by the generic
			 @ref FixedMetadataRegistryConfiguration template to report validation and lookup failures with
			 domain-specific terminology.
			@date 10/26/2026
			@since 0.12.53
			@version 0.12.53
			@author Matthew Moore
		*/
		struct EggGroupRegistryConfigurationPolicy
		{
			public:
				/*! @brief The display name of the configuration system. */
				static constexpr std::string_view configurationName{"EggGroupRegistryConfiguration"};

				/*! @brief The singular entity type managed by this configuration. */
				static constexpr std::string_view entityName{"eggGroup"};

				/*! @brief The error code returned when a duplicate eggGroup name is registered. */
				static constexpr RegistryError duplicateError{RegistryError::DuplicateEggGroup};

				/*! @brief The error code returned when a eggGroup lookup fails. */
				static constexpr RegistryError notFoundError{RegistryError::EggGroupNotFound};
		};
	} // namespace Detail

	/*! @class EggGroupRegistryConfiguration Configuration/eggGroupRegistryConfiguration.h
		@brief Provides validated user customization over an internal eggGroup registry.
		@details Supports lookup, addition, batch addition, move learn entry replacement, renaming, and removal. The registry owns copies
	   of registered names and entries. Custom IDs are assigned monotonically and are preserved during updates and renaming, and are not
	   reused after removal. Batch additions provide all-or-nothing semantics for reported validation errors.
		@date 10/26/2026
		@since 0.12.53
		@version 0.12.53
		@author Matthew Moore
	*/
	class EggGroupRegistryConfiguration
		: private FixedMetadataRegistryConfiguration<EggGroupRegistry, EggGroupMeta, EggGroupID, MAX_EGG_GROUPS, &EggGroupMeta::mEggGroupID,
													 Detail::EggGroupRegistryConfigurationPolicy>
	{
		private:
			using Base = FixedMetadataRegistryConfiguration<EggGroupRegistry, EggGroupMeta, EggGroupID, MAX_EGG_GROUPS,
															&EggGroupMeta::mEggGroupID, Detail::EggGroupRegistryConfigurationPolicy>;

		public:
			/*! @brief Constructs a configuration containing all built-in eggGroups.
				@since 0.12.53
				@version 0.12.53
			 */
			constexpr EggGroupRegistryConfiguration() = default;

			using Base::getAmountRegistered;

			/*! @brief Returns read-only access to the configured runtime eggGroup registry.
				@return A reference that remains valid for the lifetime of this configuration.
				@since 0.12.53
				@version 0.12.53
			*/
			ATTR_NODISCARD constexpr const EggGroupRegistry &getRuntimeRegistry() const noexcept
			{
				return getRegistry();
			}

			/*! @brief Looks up complete metadata by stable eggGroup ID.
				@param[in] eggGroupID The built-in or custom stable identifier.
				@return A non-owning pointer to registry-owned metadata if registered, or nullptr otherwise. The pointer must not be
			   retained across configuration mutations or destruction, as mutations can replace metadata or compact storage.
				@since 0.12.53
				@version 0.12.53
			*/
			ATTR_NODISCARD constexpr const EggGroupMeta *getEggGroupMetadata(const EggGroupID eggGroupID) const
			{
				return getMetadata(eggGroupID);
			}

			/*! @brief Looks up a stable eggGroup ID by display name.
				@param[in] name The case-sensitive display name.
				@return The stable ID if registered, or std::nullopt otherwise.
				@since 0.12.53
				@version 0.12.53
			*/
			ATTR_NODISCARD constexpr const std::optional<EggGroupID> getEggGroupID(const std::string_view name) const
			{
				return getID(name);
			}

			/*! @brief Looks up a display name by stable eggGroup ID.
				@param[in] eggGroupID The built-in or custom stable identifier.
				@return A non-owning view of the registry-owned display name if registered, or std::nullopt otherwise. The view must not be
			   retained across configuration mutations or destruction.
				@since 0.12.53
				@version 0.12.53
			*/
			ATTR_NODISCARD constexpr const std::optional<std::string_view> getEggGroupName(const EggGroupID eggGroupID) const
			{
				return getName(eggGroupID);
			}

			/*! @brief Returns all currently registered eggGroup definitions.
				@return A read-only span that remains valid until mutation or destruction.
				@since 0.12.53
				@version 0.12.53
			*/
			ATTR_NODISCARD constexpr const std::span<const EggGroupMeta> getRegisteredEggGroups() const noexcept
			{
				return getRegisteredEntries();
			}

			/*! @brief Checks whether a eggGroup name is registered.
				@param[in] name The case-sensitive display name.
				@return True if the name is registered, otherwise false.
				@since 0.12.53
				@version 0.12.53
			*/
			ATTR_NODISCARD constexpr bool hasEggGroup(const std::string_view name) const
			{
				return hasEntry(name);
			}

			/*! @brief Checks whether a eggGroup ID is registered.
				@param[in] eggGroupID The built-in or custom stable identifier.
				@return True if the ID is registered, otherwise false.
				@since 0.12.53
				@version 0.12.53
			*/
			ATTR_NODISCARD constexpr bool hasEggGroup(const EggGroupID eggGroupID) const
			{
				return hasEntry(eggGroupID);
			}

			/*! @brief Registers one user-defined eggGroup and assigns a stable ID.
				@param[in] eggGroupMeta The name and move learn entries to copy into owned registry storage. The supplied ID is ignored,
			   and the source metadata does not need to outlive this call.
				@return The assigned ID on success, or @ref RegistryError on duplicate name or exhausted capacity.
				@since 0.12.53
				@version 0.12.53
			*/
			ATTR_NODISCARD std::expected<EggGroupID, RegistryError> addEggGroup(const EggGroupMeta &eggGroupMeta);

			/*! @brief Registers multiple eggGroups atomically.
				@details Restores the registered metadata and next stable ID if any definition fails validation.
				@param[in] eggGroupMetas The eggGroup definitions to copy eagerly into owned registry storage in order. Supplied IDs are
			   ignored, and the source span and definitions do not need to outlive this call.
				@return Void on success, or the first @ref RegistryError on failure.
				@note Rollback applies to reported validation errors; allocation exceptions are not caught and may leave earlier additions
			   registered.
				@since 0.12.53
				@version 0.12.53
			*/
			ATTR_NODISCARD std::expected<void, RegistryError> addEggGroups(const std::span<const EggGroupMeta> eggGroupMetas);

			/*! @brief Renames a eggGroup without changing its stable ID or move learn entries.
				@details Copies @p newName into the registry-owned string; its backing storage does not need to outlive this call.
				@param[in] oldName The currently registered display name.
				@param[in] newName The unique replacement display name.
				@return Void on success, or @ref RegistryError if the source is absent or target name is already registered.
				@since 0.12.53
				@version 0.12.53
			*/
			ATTR_NODISCARD std::expected<void, RegistryError> renameEggGroup(const std::string_view oldName,
																			 const std::string_view newName);

			/*! @brief Replaces a eggGroup's name and move learn entries selected by display name, preserving its stable ID.
				@param[in] eggGroupName The registered display name.
				@param[in] eggGroupMeta The metadata to copy into owned registry storage. The supplied ID is ignored, and the source
			   metadata does not need to outlive this call.
				@return Void on success, or @ref RegistryError if the eggGroup is not registered or the replacement name is already in
			   use by another eggGroup.
				@since 0.12.53
				@version 0.12.53
			*/
			ATTR_NODISCARD std::expected<void, RegistryError> updateEggGroup(const std::string_view eggGroupName,
																			 const EggGroupMeta &eggGroupMeta);

			/*! @overload std::expected<void, RegistryError> updateEggGroup(EggGroupID, const EggGroupMeta &eggGroupMeta)
				@brief Replaces a eggGroup's name and move learn entries selected by stable ID, preserving that ID.
				@param[in] eggGroupID The built-in or custom stable identifier.
				@param[in] eggGroupMeta The metadata to copy into owned registry storage. The supplied ID is ignored, and the source
			   metadata does not need to outlive this call.
				@return Void on success, or @ref RegistryError if the eggGroup is not registered or the replacement name is already in
			   use by another eggGroup.
				@since 0.12.53
				@version 0.12.53
			*/
			ATTR_NODISCARD std::expected<void, RegistryError> updateEggGroup(const EggGroupID eggGroupID, const EggGroupMeta &eggGroupMeta);

			/*! @brief Removes a eggGroup by display name.
				@param[in] eggGroupName The registered display name.
				@return The removed stable ID on success, or @ref RegistryError if no matching eggGroup exists.
				@since 0.12.53
				@version 0.12.53
			*/
			ATTR_NODISCARD std::expected<EggGroupID, RegistryError> removeEggGroup(const std::string_view eggGroupName);

			/*! @overload std::expected<EggGroupID, RegistryError> removeEggGroup(EggGroupID)
				@brief Removes a eggGroup by stable ID.
				@param[in] eggGroupID The built-in or custom stable identifier.
				@return The removed stable ID on success, or @ref RegistryError if no matching eggGroup exists.
				@since 0.12.53
				@version 0.12.53
			*/
			ATTR_NODISCARD std::expected<EggGroupID, RegistryError> removeEggGroup(const EggGroupID eggGroupID);
	};
} // namespace PocketCore::Configuration

#endif
