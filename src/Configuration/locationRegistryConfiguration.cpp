/*! @file locationRegistryConfiguration.cpp
	@brief Defines validated user customization operations for the location registry.
	@date 10/09/2026
	@since 0.12.51
	@version 0.12.12
	@author Matthew Moore
*/

#include "Configuration/locationRegistryConfiguration.h"

#include <expected>
#include <span>
#include <string_view>

#include "Core/attributeMacros.h"
#include "Location/locationID.h"
#include "Location/locationMeta.h"
#include "Registry/registryError.h"

namespace PocketCore::Configuration
{
	using PocketCore::Location::LocationID;
	using PocketCore::Location::LocationMeta;
	using PocketCore::Registry::RegistryErrorInfo;

	ATTR_NODISCARD std::expected<LocationID, RegistryErrorInfo> LocationRegistryConfiguration::addLocation(const LocationMeta &locationMeta)
	{
		return addMetadata(locationMeta);
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> LocationRegistryConfiguration::addLocations(
		const std::span<const LocationMeta> &locationMetas)
	{
		return addMetadataBatch(locationMetas, [](const LocationMeta &definition) { return LocationMeta{definition}; });
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> LocationRegistryConfiguration::renameLocation(const std::string_view &oldName,
																										const std::string_view &newName)
	{
		return renameMetadata(oldName, newName);
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> LocationRegistryConfiguration::updateLocation(
		const std::string_view &locationName, const LocationMeta &locationMeta)
	{
		return mutateMetadata(locationName, "updateLocation", [&locationMeta](LocationMeta &metadata) { metadata = locationMeta; });
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> LocationRegistryConfiguration::updateLocation(const LocationID locationID,
																										const LocationMeta &locationMeta)
	{
		return mutateMetadata(locationID, "updateLocation", [&locationMeta](LocationMeta &metadata) { metadata = locationMeta; });
	}

	ATTR_NODISCARD std::expected<LocationID, RegistryErrorInfo> LocationRegistryConfiguration::removeLocation(
		const std::string_view &locationName)
	{
		return removeMetadata(locationName);
	}

	ATTR_NODISCARD std::expected<LocationID, RegistryErrorInfo> LocationRegistryConfiguration::removeLocation(const LocationID locationID)
	{
		return removeMetadata(locationID);
	}
} // namespace PocketCore::Configuration
