/*! @file statusRegistryConfiguration.h
	@brief Declares the user-facing facade for configuring status metadata.
	@date 07/27/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_CONFIGURATION_STATUS_REGISTRY_CONFIGURATION_H
#define INCLUDE_CONFIGURATION_STATUS_REGISTRY_CONFIGURATION_H

#include <expected>
#include <optional>
#include <span>
#include <string_view>

#include "Configuration/constants.h"
#include "Configuration/fixedMetadataRegistryConfiguration.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Registry/statusRegistry.h"
#include "Status/statusID.h"
#include "Status/statusMeta.h"

namespace PocketCore::Configuration
{
	using PocketCore::Core::us;
	using PocketCore::Registry::Status::StatusRegistry;
	using PocketCore::Status::StatusID;
	using PocketCore::Status::StatusMeta;

	namespace Detail
	{
		struct StatusRegistryConfigurationPolicy
		{
			public:
				static constexpr std::string_view configurationName{"StatusRegistryConfiguration"};
				static constexpr std::string_view entityName{"status"};
				static constexpr RegistryError duplicateError{RegistryError::DuplicateStatus};
				static constexpr RegistryError notFoundError{RegistryError::StatusNotFound};
		};
	} // namespace Detail

	/*! @class StatusRegistryConfiguration Configuration/statusRegistryConfiguration.h
		@brief Provides validated user customization over an internal status registry.
		@details Supports lookup, addition, batch addition, trigger replacement, renaming, and removal. Custom IDs are assigned
	   monotonically and are not reused after removal. Batch additions provide all-or-nothing semantics.
		@date 07/27/2026
		@version x.x.x
		@since x.x.x
		@author Matthew Moore
	*/
	class StatusRegistryConfiguration
		: private FixedMetadataRegistryConfiguration<StatusRegistry, StatusMeta, StatusID, MAX_STATUSES, &StatusMeta::mStatusID,
													 Detail::StatusRegistryConfigurationPolicy>
	{
		private:
			using Base = FixedMetadataRegistryConfiguration<StatusRegistry, StatusMeta, StatusID, MAX_STATUSES, &StatusMeta::mStatusID,
															Detail::StatusRegistryConfigurationPolicy>;

		public:
			/*! @brief Constructs a configuration containing all built-in statuses. */
			constexpr StatusRegistryConfiguration() = default;

			/*! @brief Returns read-only access to the configured runtime status registry.
				@return A reference that remains valid for the lifetime of this configuration.
			*/
			ATTR_NODISCARD constexpr const StatusRegistry &getRuntimeRegistry() const noexcept
			{
				return Base::getRegistry();
			}

			/*! @brief Looks up complete metadata by stable status ID.
				@param[in] statusID The built-in or custom stable identifier.
				@return A non-owning pointer to metadata if registered, or nullptr otherwise. The pointer remains valid until replacement or
			   configuration destruction.
			*/
			ATTR_NODISCARD constexpr const StatusMeta *getStatusMetadata(const StatusID statusID) const
			{
				return getMetadata(statusID);
			}

			/*! @brief Looks up a stable status ID by display name.
				@param[in] name The case-sensitive display name.
				@return The stable ID if registered, or std::nullopt otherwise.
			*/
			ATTR_NODISCARD constexpr const std::optional<StatusID> getStatusID(const std::string_view &name) const
			{
				return getID(name);
			}

			/*! @brief Looks up a display name by stable status ID.
				@param[in] statusID The built-in or custom stable identifier.
				@return The display name if registered, or std::nullopt otherwise.
			*/
			ATTR_NODISCARD constexpr const std::optional<std::string_view> getStatusName(const StatusID statusID) const
			{
				return getName(statusID);
			}

			/*! @brief Returns all currently registered status definitions.
				@return A read-only span that remains valid until mutation or destruction.
			*/
			ATTR_NODISCARD constexpr const std::span<const StatusMeta> getRegisteredStatuses() const noexcept
			{
				return getRegisteredEntries();
			}

			/*! @brief Returns the number of registered built-in and custom statuses.
				@return The current registry entry count.
			*/
			ATTR_NODISCARD constexpr us getAmountRegistered() const noexcept
			{
				return Base::getAmountRegistered();
			}

			/*! @brief Checks whether an status name is registered.
				@param[in] name The case-sensitive display name.
				@return True if the name is registered, otherwise false.
			*/
			ATTR_NODISCARD constexpr bool hasStatus(const std::string_view &name) const
			{
				return hasEntry(name);
			}

			/*! @brief Checks whether an status ID is registered.
				@param[in] statusID The built-in or custom stable identifier.
				@return True if the ID is registered, otherwise false.
			*/
			ATTR_NODISCARD constexpr bool hasStatus(const StatusID statusID) const
			{
				return hasEntry(statusID);
			}

			/*! @brief Registers one user-defined status and assigns a stable ID.
				@param[in] statusMeta The name and trigger metadata to copy into the registry.
				@return The assigned ID on success, or @ref RegistryErrorInfo on duplicate name or exhausted capacity.
			*/
			ATTR_NODISCARD std::expected<StatusID, RegistryErrorInfo> addStatus(const StatusMeta &statusMeta);

			/*! @brief Registers multiple statuses atomically.
				@details Restores the complete prior registry state if any definition fails validation.
				@param[in] statusMetas The status definitions to register in order.
				@return Void on success, or the first @ref RegistryErrorInfo on failure.
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> addStatuses(const std::span<const StatusMeta> &statusMetas);

			/*! @brief Renames an status without changing its other metadata.
				@details @p newName is stored as a non-owning view and its backing storage must remain valid while registered.
				@param[in] oldName The currently registered display name.
				@param[in] newName The unique replacement display name.
				@return Void on success, or @ref RegistryErrorInfo if the source is absent or target name is already registered.
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> renameStatus(const std::string_view &oldName,
																			   const std::string_view &newName);

			/*! @brief Replaces all status metadata for an status selected by stable ID.
				@param[in] statusName The registered display name.
				@param[in] statusMeta The metadata to copy into the registry.
				@return Void on success, or @ref RegistryErrorInfo if the status is not registered.
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> updateStatus(const std::string_view &statusName,
																			   const StatusMeta &statusMeta);

			/*! @overload std::expected<void, RegistryErrorInfo> updateStatus(StatusID, const StatusMeta &statusMeta)
				@brief Replaces all status metadata for an status selected by stable ID.
				@param[in] statusID The built-in or custom stable identifier.
				@param[in] statusMeta The metadata to copy into the registry.
				@return Void on success, or @ref RegistryErrorInfo if the status is not registered.
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> updateStatus(const StatusID statusID, const StatusMeta &statusMeta);

			/*! @brief Removes an status by display name.
				@param[in] statusName The registered display name.
				@return The removed stable ID on success, or @ref RegistryErrorInfo if no matching status exists.
			*/
			ATTR_NODISCARD std::expected<StatusID, RegistryErrorInfo> removeStatus(const std::string_view &statusName);

			/*! @overload std::expected<StatusID, RegistryErrorInfo> removeStatus(StatusID)
				@brief Removes an status by stable ID.
				@param[in] statusID The built-in or custom stable identifier.
				@return The removed stable ID on success, or @ref RegistryErrorInfo if no matching status exists.
			*/
			ATTR_NODISCARD std::expected<StatusID, RegistryErrorInfo> removeStatus(const StatusID statusID);
	};
} // namespace PocketCore::Configuration

#endif
