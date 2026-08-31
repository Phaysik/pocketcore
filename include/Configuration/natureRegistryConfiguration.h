/*! @file natureRegistryConfiguration.h
	@brief Declares the user-facing facade for configuring nature metadata.
	@date 08/27/2026
	@since 0.11.6
	@version 0.12.8
	@author Matthew Moore
*/

#ifndef INCLUDE_CONFIGURATION_NATURE_REGISTRY_CONFIGURATION_H
#define INCLUDE_CONFIGURATION_NATURE_REGISTRY_CONFIGURATION_H

#include <expected>
#include <optional>
#include <span>
#include <string_view>

#include "Configuration/constants.h"
#include "Configuration/fixedMetadataRegistryConfiguration.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Nature/natureID.h"
#include "Nature/natureMeta.h"
#include "Registry/natureRegistry.h"

namespace PocketCore::Configuration
{
	using PocketCore::Core::us;
	using PocketCore::Nature::NatureID;
	using PocketCore::Nature::NatureMeta;
	using PocketCore::Registry::Nature::NatureRegistry;

	namespace Detail
	{
		/*! @struct NatureRegistryConfigurationPolicy Configuration/natureRegistryConfiguration.h
			@brief Policy class providing error codes and display strings for nature registry configuration.
			@details Encapsulates the nature-specific error categories and display names used by the generic
			 @ref FixedMetadataRegistryConfiguration template to report validation and lookup failures with
			 domain-specific terminology.
			@date 08/22/2026
			@since 0.11.6
			@version 0.11.6
			@author Matthew Moore
		*/
		struct NatureRegistryConfigurationPolicy
		{
			public:
				/*! @brief The display name of the configuration system. */
				static constexpr std::string_view configurationName{"NatureRegistryConfiguration"};

				/*! @brief The singular entity type managed by this configuration. */
				static constexpr std::string_view entityName{"nature"};

				/*! @brief The error code returned when a duplicate ability name is registered. */
				static constexpr RegistryError duplicateError{RegistryError::DuplicateNature};

				/*! @brief The error code returned when an ability lookup fails. */
				static constexpr RegistryError notFoundError{RegistryError::NatureNotFound};
		};
	} // namespace Detail

	/*! @class NatureRegistryConfiguration Configuration/natureRegistryConfiguration.h
		@brief Provides validated user customization over an internal nature registry.
		@details Supports lookup, addition, batch addition, trigger replacement, renaming, and removal. Custom IDs are assigned
	   monotonically and are not reused after removal. Batch additions provide all-or-nothing semantics.
		@date 08/27/2026
		@since 0.11.6
		@version 0.12.8
		@author Matthew Moore
	*/
	class NatureRegistryConfiguration
		: private FixedMetadataRegistryConfiguration<NatureRegistry, NatureMeta, NatureID, MAX_NATURES, &NatureMeta::mNatureID,
													 Detail::NatureRegistryConfigurationPolicy>
	{
		private:
			using Base = FixedMetadataRegistryConfiguration<NatureRegistry, NatureMeta, NatureID, MAX_NATURES, &NatureMeta::mNatureID,
															Detail::NatureRegistryConfigurationPolicy>;

		public:
			/*! @brief Constructs a configuration containing all built-in natures.
				@since 0.11.6
				@version 0.11.6
			 */
			constexpr NatureRegistryConfiguration() = default;

			using Base::getAmountRegistered;

			/*! @brief Returns read-only access to the configured runtime nature registry.
				@return A reference that remains valid for the lifetime of this configuration.
				@since 0.11.6
				@version 0.12.8
			*/
			ATTR_NODISCARD constexpr const NatureRegistry &getRuntimeRegistry() const noexcept
			{
				return getRegistry();
			}

			/*! @brief Looks up complete metadata by stable nature ID.
				@param[in] natureID The built-in or custom stable identifier.
				@return A non-owning pointer to metadata if registered, or nullptr otherwise. The pointer remains valid until replacement or
			   configuration destruction.
				@since 0.11.6
				@version 0.11.6
			*/
			ATTR_NODISCARD constexpr const NatureMeta *getNatureMetadata(const NatureID natureID) const
			{
				return getMetadata(natureID);
			}

			/*! @brief Looks up a stable nature ID by display name.
				@param[in] name The case-sensitive display name.
				@return The stable ID if registered, or std::nullopt otherwise.
				@since 0.11.6
				@version 0.11.6
			*/
			ATTR_NODISCARD constexpr const std::optional<NatureID> getNatureID(const std::string_view &name) const
			{
				return getID(name);
			}

			/*! @brief Looks up a display name by stable nature ID.
				@param[in] natureID The built-in or custom stable identifier.
				@return The display name if registered, or std::nullopt otherwise.
				@since 0.11.6
				@version 0.11.6
			*/
			ATTR_NODISCARD constexpr const std::optional<std::string_view> getNatureName(const NatureID natureID) const
			{
				return getName(natureID);
			}

			/*! @brief Returns all currently registered nature definitions.
				@return A read-only span that remains valid until mutation or destruction.
				@since 0.11.6
				@version 0.11.6
			*/
			ATTR_NODISCARD constexpr const std::span<const NatureMeta> getRegisteredNatures() const noexcept
			{
				return getRegisteredEntries();
			}

			/*! @brief Checks whether an nature name is registered.
				@param[in] name The case-sensitive display name.
				@return True if the name is registered, otherwise false.
				@since 0.11.6
				@version 0.11.6
			*/
			ATTR_NODISCARD constexpr bool hasNature(const std::string_view &name) const
			{
				return hasEntry(name);
			}

			/*! @brief Checks whether an nature ID is registered.
				@param[in] natureID The built-in or custom stable identifier.
				@return True if the ID is registered, otherwise false.
				@since 0.11.6
				@version 0.11.6
			*/
			ATTR_NODISCARD constexpr bool hasNature(const NatureID natureID) const
			{
				return hasEntry(natureID);
			}

			/*! @brief Registers one user-defined nature and assigns a stable ID.
				@param[in] natureMeta The name and trigger metadata to copy into the registry.
				@return The assigned ID on success, or @ref RegistryErrorInfo on duplicate name or exhausted capacity.
				@since 0.11.6
				@version 0.11.6
			*/
			ATTR_NODISCARD std::expected<NatureID, RegistryErrorInfo> addNature(const NatureMeta &natureMeta);

			/*! @brief Registers multiple natures atomically.
				@details Restores the complete prior registry state if any definition fails validation.
				@param[in] natureMetas The nature definitions to register in order.
				@return Void on success, or the first @ref RegistryErrorInfo on failure.
				@since 0.11.6
				@version 0.11.6
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> addNatures(const std::span<const NatureMeta> &natureMetas);

			/*! @brief Renames an nature without changing its other metadata.
				@details @p newName is stored as a non-owning view and its backing storage must remain valid while registered.
				@param[in] oldName The currently registered display name.
				@param[in] newName The unique replacement display name.
				@return Void on success, or @ref RegistryErrorInfo if the source is absent or target name is already registered.
				@since 0.11.6
				@version 0.11.6
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> renameNature(const std::string_view &oldName,
																			   const std::string_view &newName);

			/*! @brief Replaces all nature metadata for an nature selected by stable ID.
				@param[in] natureName The registered display name.
				@param[in] natureMeta The metadata to copy into the registry.
				@return Void on success, or @ref RegistryErrorInfo if the nature is not registered.
				@since 0.11.6
				@version 0.11.6
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> updateNature(const std::string_view &natureName,
																			   const NatureMeta &natureMeta);

			/*! @overload std::expected<void, RegistryErrorInfo> updateNature(NatureID, const NatureMeta &natureMeta)
				@brief Replaces all nature metadata for an nature selected by stable ID.
				@param[in] natureID The built-in or custom stable identifier.
				@param[in] natureMeta The metadata to copy into the registry.
				@return Void on success, or @ref RegistryErrorInfo if the nature is not registered.
				@since 0.11.6
				@version 0.11.6
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> updateNature(const NatureID natureID, const NatureMeta &natureMeta);

			/*! @brief Removes an nature by display name.
				@param[in] natureName The registered display name.
				@return The removed stable ID on success, or @ref RegistryErrorInfo if no matching nature exists.
				@since 0.11.6
				@version 0.11.6
			*/
			ATTR_NODISCARD std::expected<NatureID, RegistryErrorInfo> removeNature(const std::string_view &natureName);

			/*! @overload std::expected<NatureID, RegistryErrorInfo> removeNature(NatureID)
				@brief Removes an nature by stable ID.
				@param[in] natureID The built-in or custom stable identifier.
				@return The removed stable ID on success, or @ref RegistryErrorInfo if no matching nature exists.
				@since 0.11.6
				@version 0.11.6
			*/
			ATTR_NODISCARD std::expected<NatureID, RegistryErrorInfo> removeNature(const NatureID natureID);
	};
} // namespace PocketCore::Configuration

#endif
