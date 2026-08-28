/*! @file multiplierRegistryConfiguration.h
	@brief Declares the user-facing facade for configuring multiplier metadata.
	@date 08/14/2026
	@since 0.8.1
	@version 0.10.3
	@author Matthew Moore
*/

#ifndef INCLUDE_CONFIGURATION_MULTIPLIER_REGISTRY_CONFIGURATION_H
#define INCLUDE_CONFIGURATION_MULTIPLIER_REGISTRY_CONFIGURATION_H

#include <expected>
#include <optional>
#include <span>
#include <string_view>

#include "Configuration/constants.h"
#include "Configuration/fixedMetadataRegistryConfiguration.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Multiplier/multiplierID.h"
#include "Multiplier/multiplierMeta.h"
#include "Registry/multiplierRegistry.h"
#include "Registry/registryError.h"

namespace PocketCore::Configuration
{
	using PocketCore::Core::us;
	using PocketCore::Multiplier::MultiplierID;
	using PocketCore::Multiplier::MultiplierMeta;
	using PocketCore::Registry::RegistryError;
	using PocketCore::Registry::RegistryErrorInfo;
	using Registry::Multiplier::MultiplierRegistry;

	namespace Detail
	{
		/*! @struct MultiplierRegistryConfigurationPolicy Configuration/multiplierRegistryConfiguration.h
			@brief Policy class providing error codes and display strings for multiplier registry configuration.
			@details Encapsulates the multiplier-specific error categories and display names used by the generic
			 @ref FixedMetadataRegistryConfiguration template to report validation and lookup failures with
			 domain-specific terminology.
			@date 07/30/2026
			@since 0.8.1
			@version 0.8.1
			@author Matthew Moore
		*/
		struct MultiplierRegistryConfigurationPolicy
		{
			public:
				/*! @brief The display name of the configuration system. */
				static constexpr std::string_view configurationName{"MultiplierRegistryConfiguration"};

				/*! @brief The singular entity type managed by this configuration. */
				static constexpr std::string_view entityName{"multiplier"};

				/*! @brief The error code returned when a duplicate ability name is registered. */
				static constexpr RegistryError duplicateError{RegistryError::DuplicateMultiplier};

				/*! @brief The error code returned when an ability lookup fails. */
				static constexpr RegistryError notFoundError{RegistryError::MultiplierNotFound};
		};
	} // namespace Detail

	/*! @class MultiplierRegistryConfiguration Configuration/multiplierRegistryConfiguration.h
		@brief Provides validated user customization over an internal multiplier registry.
		@details Supports lookup, addition, batch addition, trigger replacement, renaming, and removal. Custom IDs are assigned
	   monotonically and are not reused after removal. Batch additions provide all-or-nothing semantics.
		@date 08/14/2026
		@since 0.8.1
		@version 0.10.3
		@author Matthew Moore
	*/
	class MultiplierRegistryConfiguration
		: private FixedMetadataRegistryConfiguration<MultiplierRegistry, MultiplierMeta, MultiplierID, MAX_MULTIPLIERS,
													 &MultiplierMeta::mMultiplierID, Detail::MultiplierRegistryConfigurationPolicy>
	{
		private:
			using Base = FixedMetadataRegistryConfiguration<MultiplierRegistry, MultiplierMeta, MultiplierID, MAX_MULTIPLIERS,
															&MultiplierMeta::mMultiplierID, Detail::MultiplierRegistryConfigurationPolicy>;

		public:
			/*! @brief Constructs a configuration containing all built-in multipliers.
				@since 0.8.1
				@version 0.8.1
			 */
			constexpr MultiplierRegistryConfiguration() = default;

			/*! @brief Returns read-only access to the configured runtime multiplier registry.
				@return A reference that remains valid for the lifetime of this configuration.
				@since 0.10.3
				@version 0.10.3
			*/
			ATTR_NODISCARD constexpr const MultiplierRegistry &getRuntimeRegistry() const noexcept
			{
				return getRegistry();
			}

			/*! @brief Looks up complete metadata by stable multiplier ID.
				@param[in] multiplierID The built-in or custom stable identifier.
				@return A non-owning pointer to metadata if registered, or nullptr otherwise. The pointer remains valid until replacement or
			   configuration destruction.
				@since 0.8.1
				@version 0.8.1
			*/
			ATTR_NODISCARD constexpr const MultiplierMeta *getMultiplierMetadata(const MultiplierID multiplierID) const
			{
				return getMetadata(multiplierID);
			}

			/*! @brief Looks up a stable multiplier ID by display name.
				@param[in] name The case-sensitive display name.
				@return The stable ID if registered, or std::nullopt otherwise.
				@since 0.8.1
				@version 0.8.1
			*/
			ATTR_NODISCARD constexpr const std::optional<MultiplierID> getMultiplierID(const std::string_view &name) const
			{
				return getID(name);
			}

			/*! @brief Looks up a display name by stable multiplier ID.
				@param[in] multiplierID The built-in or custom stable identifier.
				@return The display name if registered, or std::nullopt otherwise.
				@since 0.8.1
				@version 0.8.1
			*/
			ATTR_NODISCARD constexpr const std::optional<std::string_view> getMultiplierName(const MultiplierID multiplierID) const
			{
				return getName(multiplierID);
			}

			/*! @brief Returns all currently registered multiplier definitions.
				@return A read-only span that remains valid until mutation or destruction.
				@since 0.8.1
				@version 0.8.1
			*/
			ATTR_NODISCARD constexpr const std::span<const MultiplierMeta> getRegisteredMultipliers() const noexcept
			{
				return getRegisteredEntries();
			}

			/*! @brief Checks whether an multiplier name is registered.
				@param[in] name The case-sensitive display name.
				@return True if the name is registered, otherwise false.
				@since 0.8.1
				@version 0.8.1
			*/
			ATTR_NODISCARD constexpr bool hasMultiplier(const std::string_view &name) const
			{
				return hasEntry(name);
			}

			/*! @brief Checks whether an multiplier ID is registered.
				@param[in] multiplierID The built-in or custom stable identifier.
				@return True if the ID is registered, otherwise false.
				@since 0.8.1
				@version 0.8.1
			*/
			ATTR_NODISCARD constexpr bool hasMultiplier(const MultiplierID multiplierID) const
			{
				return hasEntry(multiplierID);
			}

			/*! @brief Registers one user-defined multiplier and assigns a stable ID.
				@param[in] multiplierMeta The name and trigger metadata to copy into the registry.
				@return The assigned ID on success, or @ref RegistryErrorInfo on duplicate name or exhausted capacity.
				@since 0.8.1
				@version 0.9.0
			*/
			ATTR_NODISCARD std::expected<MultiplierID, RegistryErrorInfo> addMultiplier(const MultiplierMeta &multiplierMeta);

			/*! @brief Registers multiple multipliers atomically.
				@details Restores the complete prior registry state if any definition fails validation.
				@param[in] multiplierMetas The multiplier definitions to register in order.
				@return Void on success, or the first @ref RegistryErrorInfo on failure.
				@since 0.8.1
				@version 0.8.1
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> addMultipliers(const std::span<const MultiplierMeta> &multiplierMetas);

			/*! @brief Renames an multiplier without changing its other metadata.
				@details @p newName is stored as a non-owning view and its backing storage must remain valid while registered.
				@param[in] oldName The currently registered display name.
				@param[in] newName The unique replacement display name.
				@return Void on success, or @ref RegistryErrorInfo if the source is absent or target name is already registered.
				@since 0.8.1
				@version 0.9.0
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> renameMultiplier(const std::string_view &oldName,
																				   const std::string_view &newName);

			/*! @brief Replaces all multiplier metadata for an multiplier selected by stable ID.
				@param[in] multiplierName The registered display name.
				@param[in] multiplierMeta The metadata to copy into the registry.
				@return Void on success, or @ref RegistryErrorInfo if the multiplier is not registered.
				@since 0.8.1
				@version 0.9.0
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> updateMultiplier(const std::string_view &multiplierName,
																				   const MultiplierMeta &multiplierMeta);

			/*! @overload std::expected<void, RegistryErrorInfo> updateMultiplier(MultiplierID, const MultiplierMeta &multiplierMeta)
				@brief Replaces all multiplier metadata for an multiplier selected by stable ID.
				@param[in] multiplierID The built-in or custom stable identifier.
				@param[in] multiplierMeta The metadata to copy into the registry.
				@return Void on success, or @ref RegistryErrorInfo if the multiplier is not registered.
				@since 0.8.1
				@version 0.9.0
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> updateMultiplier(const MultiplierID multiplierID,
																				   const MultiplierMeta &multiplierMeta);

			/*! @brief Removes an multiplier by display name.
				@param[in] multiplierName The registered display name.
				@return The removed stable ID on success, or @ref RegistryErrorInfo if no matching multiplier exists.
				@since 0.8.1
				@version 0.9.0
			*/
			ATTR_NODISCARD std::expected<MultiplierID, RegistryErrorInfo> removeMultiplier(const std::string_view &multiplierName);

			/*! @overload std::expected<MultiplierID, RegistryErrorInfo> removeMultiplier(MultiplierID)
				@brief Removes an multiplier by stable ID.
				@param[in] multiplierID The built-in or custom stable identifier.
				@return The removed stable ID on success, or @ref RegistryErrorInfo if no matching multiplier exists.
				@since 0.8.1
				@version 0.9.0
			*/
			ATTR_NODISCARD std::expected<MultiplierID, RegistryErrorInfo> removeMultiplier(const MultiplierID multiplierID);
	};
} // namespace PocketCore::Configuration

#endif
