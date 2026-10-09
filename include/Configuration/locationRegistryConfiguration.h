/*! @file locationRegistryConfiguration.h
	@brief Declares the user-facing facade for configuring location metadata.
	@date 10/09/2026
	@since 0.12.51
	@version 0.12.51
	@author Matthew Moore
*/

#ifndef INCLUDE_CONFIGURATION_LOCATION_REGISTRY_CONFIGURATION_H
#define INCLUDE_CONFIGURATION_LOCATION_REGISTRY_CONFIGURATION_H

#include <expected>
#include <optional>
#include <span>
#include <string_view>

#include "Configuration/constants.h"
#include "Configuration/fixedMetadataRegistryConfiguration.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Location/locationID.h"
#include "Location/locationMeta.h"
#include "Registry/locationRegistry.h"

namespace PocketCore::Configuration
{
	using PocketCore::Core::us;
	using PocketCore::Location::LocationID;
	using PocketCore::Location::LocationMeta;
	using PocketCore::Registry::Location::LocationRegistry;

	namespace Detail
	{
		/*! @struct LocationRegistryConfigurationPolicy Configuration/locationRegistryConfiguration.h
			@brief Policy class providing error codes and display strings for location registry configuration.
			@details Encapsulates the location-specific error categories and display names used by the generic
			 @ref FixedMetadataRegistryConfiguration template to report validation and lookup failures with
			 domain-specific terminology.
			@date 10/09/2026
			@since 0.12.51
			@version 0.12.51
			@author Matthew Moore
		*/
		struct LocationRegistryConfigurationPolicy
		{
			public:
				/*! @brief The display name of the configuration system. */
				static constexpr std::string_view configurationName{"LocationRegistryConfiguration"};

				/*! @brief The singular entity type managed by this configuration. */
				static constexpr std::string_view entityName{"location"};

				/*! @brief The error code returned when a duplicate location name is registered. */
				static constexpr RegistryError duplicateError{RegistryError::DuplicateLocation};

				/*! @brief The error code returned when a location lookup fails. */
				static constexpr RegistryError notFoundError{RegistryError::LocationNotFound};
		};
	} // namespace Detail

	/*! @class LocationRegistryConfiguration Configuration/locationRegistryConfiguration.h
		@brief Provides validated user customization over an internal location registry.
		@details Supports lookup, addition, batch addition, trigger replacement, renaming, and removal. Custom IDs are assigned
	   monotonically and are not reused after removal. Batch additions provide all-or-nothing semantics.
		@date 10/09/2026
		@since 0.12.51
		@version 0.12.51
		@author Matthew Moore
	*/
	class LocationRegistryConfiguration
		: private FixedMetadataRegistryConfiguration<LocationRegistry, LocationMeta, LocationID, MAX_LOCATIONS, &LocationMeta::mLocationID,
													 Detail::LocationRegistryConfigurationPolicy>
	{
		private:
			using Base = FixedMetadataRegistryConfiguration<LocationRegistry, LocationMeta, LocationID, MAX_LOCATIONS,
															&LocationMeta::mLocationID, Detail::LocationRegistryConfigurationPolicy>;

		public:
			/*! @brief Constructs a configuration containing all built-in locations.
				@since 0.12.51
				@version 0.12.51
			 */
			constexpr LocationRegistryConfiguration() = default;

			using Base::getAmountRegistered;

			/*! @brief Returns read-only access to the configured runtime location registry.
				@return A reference that remains valid for the lifetime of this configuration.
				@since 0.12.51
				@version 0.12.51
			*/
			ATTR_NODISCARD constexpr const LocationRegistry &getRuntimeRegistry() const noexcept
			{
				return getRegistry();
			}

			/*! @brief Looks up complete metadata by stable location ID.
				@param[in] locationID The built-in or custom stable identifier.
				@return A non-owning pointer to metadata if registered, or nullptr otherwise. The pointer remains valid until replacement or
			   configuration destruction.
				@since 0.12.51
				@version 0.12.51
			*/
			ATTR_NODISCARD constexpr const LocationMeta *getLocationMetadata(const LocationID locationID) const
			{
				return getMetadata(locationID);
			}

			/*! @brief Looks up a stable location ID by display name.
				@param[in] name The case-sensitive display name.
				@return The stable ID if registered, or std::nullopt otherwise.
				@since 0.12.51
				@version 0.12.51
			*/
			ATTR_NODISCARD constexpr const std::optional<LocationID> getLocationID(const std::string_view &name) const
			{
				return getID(name);
			}

			/*! @brief Looks up a display name by stable location ID.
				@param[in] locationID The built-in or custom stable identifier.
				@return The display name if registered, or std::nullopt otherwise.
				@since 0.12.51
				@version 0.12.51
			*/
			ATTR_NODISCARD constexpr const std::optional<std::string_view> getLocationName(const LocationID locationID) const
			{
				return getName(locationID);
			}

			/*! @brief Returns all currently registered location definitions.
				@return A read-only span that remains valid until mutation or destruction.
				@since 0.12.51
				@version 0.12.51
			*/
			ATTR_NODISCARD constexpr const std::span<const LocationMeta> getRegisteredLocations() const noexcept
			{
				return getRegisteredEntries();
			}

			/*! @brief Checks whether an location name is registered.
				@param[in] name The case-sensitive display name.
				@return True if the name is registered, otherwise false.
				@since 0.12.51
				@version 0.12.51
			*/
			ATTR_NODISCARD constexpr bool hasLocation(const std::string_view &name) const
			{
				return hasEntry(name);
			}

			/*! @brief Checks whether an location ID is registered.
				@param[in] locationID The built-in or custom stable identifier.
				@return True if the ID is registered, otherwise false.
				@since 0.12.51
				@version 0.12.51
			*/
			ATTR_NODISCARD constexpr bool hasLocation(const LocationID locationID) const
			{
				return hasEntry(locationID);
			}

			/*! @brief Registers one user-defined location and assigns a stable ID.
				@param[in] locationMeta The name and trigger metadata to copy into the registry.
				@return The assigned ID on success, or @ref RegistryErrorInfo on duplicate name or exhausted capacity.
				@since 0.12.51
				@version 0.12.51
			*/
			ATTR_NODISCARD std::expected<LocationID, RegistryErrorInfo> addLocation(const LocationMeta &locationMeta);

			/*! @brief Registers multiple locations atomically.
				@details Restores the complete prior registry state if any definition fails validation.
				@param[in] locationMetas The location definitions to register in order.
				@return Void on success, or the first @ref RegistryErrorInfo on failure.
				@since 0.12.51
				@version 0.12.51
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> addLocations(const std::span<const LocationMeta> &locationMetas);

			/*! @brief Renames an location without changing its other metadata.
				@details @p newName is stored as a non-owning view and its backing storage must remain valid while registered.
				@param[in] oldName The currently registered display name.
				@param[in] newName The unique replacement display name.
				@return Void on success, or @ref RegistryErrorInfo if the source is absent or target name is already registered.
				@since 0.12.51
				@version 0.12.51
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> renameLocation(const std::string_view &oldName,
																				 const std::string_view &newName);

			/*! @brief Replaces all location metadata for an location selected by stable ID.
				@param[in] locationName The registered display name.
				@param[in] locationMeta The metadata to copy into the registry.
				@return Void on success, or @ref RegistryErrorInfo if the location is not registered.
				@since 0.12.51
				@version 0.12.51
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> updateLocation(const std::string_view &locationName,
																				 const LocationMeta &locationMeta);

			/*! @overload std::expected<void, RegistryErrorInfo> updateLocation(LocationID, const LocationMeta &locationMeta)
				@brief Replaces all location metadata for an location selected by stable ID.
				@param[in] locationID The built-in or custom stable identifier.
				@param[in] locationMeta The metadata to copy into the registry.
				@return Void on success, or @ref RegistryErrorInfo if the location is not registered.
				@since 0.12.51
				@version 0.12.51
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> updateLocation(const LocationID locationID,
																				 const LocationMeta &locationMeta);

			/*! @brief Removes an location by display name.
				@param[in] locationName The registered display name.
				@return The removed stable ID on success, or @ref RegistryErrorInfo if no matching location exists.
				@since 0.12.51
				@version 0.12.51
			*/
			ATTR_NODISCARD std::expected<LocationID, RegistryErrorInfo> removeLocation(const std::string_view &locationName);

			/*! @overload std::expected<LocationID, RegistryErrorInfo> removeLocation(LocationID)
				@brief Removes an location by stable ID.
				@param[in] locationID The built-in or custom stable identifier.
				@return The removed stable ID on success, or @ref RegistryErrorInfo if no matching location exists.
				@since 0.12.51
				@version 0.12.51
			*/
			ATTR_NODISCARD std::expected<LocationID, RegistryErrorInfo> removeLocation(const LocationID locationID);
	};
} // namespace PocketCore::Configuration

#endif
