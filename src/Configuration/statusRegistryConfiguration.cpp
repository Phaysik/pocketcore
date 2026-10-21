/*! @file statusRegistryConfiguration.cpp
	@brief Defines validated user customization operations for the status registry.
	@date 10/13/2026
	@since 0.6.4
	@version 0.12.51
	@author Matthew Moore
*/

#include "Configuration/statusRegistryConfiguration.h"

#include <expected>
#include <span>
#include <string_view>

#include "Core/attributeMacros.h"
#include "Status/statusID.h"
#include "Status/statusMeta.h"
#include "Validation/Registry/registryError.h"

namespace PocketCore::Configuration
{
	using PocketCore::Status::StatusID;
	using PocketCore::Status::StatusMeta;
	using PocketCore::Validation::Registry::RegistryError;

	ATTR_NODISCARD std::expected<StatusID, RegistryError> StatusRegistryConfiguration::addStatus(const StatusMeta &statusMeta)
	{
		return addMetadata(statusMeta);
	}

	ATTR_NODISCARD std::expected<void, RegistryError> StatusRegistryConfiguration::addStatuses(
		const std::span<const StatusMeta> statusMetas)
	{
		return addMetadataBatch(statusMetas, [](const StatusMeta &definition) { return StatusMeta{definition}; });
	}

	ATTR_NODISCARD std::expected<void, RegistryError> StatusRegistryConfiguration::renameStatus(const std::string_view oldName,
																								const std::string_view newName)
	{
		return renameMetadata(oldName, newName);
	}

	ATTR_NODISCARD std::expected<void, RegistryError> StatusRegistryConfiguration::updateStatus(const std::string_view statusName,
																								const StatusMeta &statusMeta)
	{
		return mutateMetadata(statusName, "updateStatus", [&statusMeta](StatusMeta &metadata) { metadata = statusMeta; });
	}

	ATTR_NODISCARD std::expected<void, RegistryError> StatusRegistryConfiguration::updateStatus(const StatusID statusID,
																								const StatusMeta &statusMeta)
	{
		return mutateMetadata(statusID, "updateStatus", [&statusMeta](StatusMeta &metadata) { metadata = statusMeta; });
	}

	ATTR_NODISCARD std::expected<StatusID, RegistryError> StatusRegistryConfiguration::removeStatus(const std::string_view statusName)
	{
		return removeMetadata(statusName);
	}

	ATTR_NODISCARD std::expected<StatusID, RegistryError> StatusRegistryConfiguration::removeStatus(const StatusID statusID)
	{
		return removeMetadata(statusID);
	}
} // namespace PocketCore::Configuration
