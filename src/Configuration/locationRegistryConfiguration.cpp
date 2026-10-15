/*! @file locationRegistryConfiguration.cpp
	@brief Defines validated user customization operations for the location registry.
	@date 10/13/2026
	@since 0.12.51
	@version 0.12.51
	@author Matthew Moore
*/

#include "Configuration/locationRegistryConfiguration.h"

#include <expected>
#include <span>
#include <string_view>

#include "Core/attributeMacros.h"
#include "Location/locationID.h"
#include "Location/locationMeta.h"
#include "Validation/Registry/registryError.h"

namespace PocketCore::Configuration
{
	using PocketCore::Location::LocationID;
	using PocketCore::Location::LocationMeta;
	using PocketCore::Validation::Registry::RegistryError;

	ATTR_NODISCARD std::expected<LocationID, RegistryError> LocationRegistryConfiguration::addLocation(const LocationMeta &locationMeta)
	{
		return addMetadata(locationMeta);
	}

	ATTR_NODISCARD std::expected<void, RegistryError> LocationRegistryConfiguration::addLocations(
		const std::span<const LocationMeta> locationMetas)
	{
		return addMetadataBatch(locationMetas, [](const LocationMeta &definition) { return LocationMeta{definition}; });
	}

	ATTR_NODISCARD std::expected<void, RegistryError> LocationRegistryConfiguration::renameLocation(const std::string_view oldName,
																									const std::string_view newName)
	{
		return renameMetadata(oldName, newName);
	}

	ATTR_NODISCARD std::expected<void, RegistryError> LocationRegistryConfiguration::updateLocation(const std::string_view locationName,
																									const LocationMeta &locationMeta)
	{
		return mutateMetadata(locationName, "updateLocation", [&locationMeta](LocationMeta &metadata) { metadata = locationMeta; });
	}

	ATTR_NODISCARD std::expected<void, RegistryError> LocationRegistryConfiguration::updateLocation(const LocationID locationID,
																									const LocationMeta &locationMeta)
	{
		return mutateMetadata(locationID, "updateLocation", [&locationMeta](LocationMeta &metadata) { metadata = locationMeta; });
	}

	ATTR_NODISCARD std::expected<LocationID, RegistryError> LocationRegistryConfiguration::removeLocation(
		const std::string_view locationName)
	{
		return removeMetadata(locationName);
	}

	ATTR_NODISCARD std::expected<LocationID, RegistryError> LocationRegistryConfiguration::removeLocation(const LocationID locationID)
	{
		return removeMetadata(locationID);
	}
} // namespace PocketCore::Configuration
