/*! @file statusRegistryConfiguration.cpp
	@brief Defines validated user customization operations for the status registry.
	@date 07/29/2026
	@since 0.6.4
	@version 0.6.4
	@author Matthew Moore
*/

#include "Configuration/statusRegistryConfiguration.h"

#include <expected>
#include <span>
#include <string_view>

#include "Core/attributeMacros.h"
#include "Status/statusID.h"
#include "Status/statusMeta.h"

namespace PocketCore::Configuration
{
	using PocketCore::Status::StatusID;
	using PocketCore::Status::StatusMeta;

	ATTR_NODISCARD std::expected<StatusID, RegistryErrorInfo> StatusRegistryConfiguration::addStatus(const StatusMeta &statusMeta)
	{
		return addMetadata(statusMeta);
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> StatusRegistryConfiguration::addStatuses(
		const std::span<const StatusMeta> &statusMetas)
	{
		return addMetadataBatch(statusMetas, [](const StatusMeta &definition) { return StatusMeta{definition}; });
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> StatusRegistryConfiguration::renameStatus(const std::string_view &oldName,
																									const std::string_view &newName)
	{
		return renameMetadata(oldName, newName);
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> StatusRegistryConfiguration::updateStatus(const std::string_view &statusName,
																									const StatusMeta &statusMeta)
	{
		return mutateMetadata(statusName, "updateStatus", [&statusMeta](StatusMeta &metadata) { metadata = statusMeta; });
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> StatusRegistryConfiguration::updateStatus(const StatusID statusID,
																									const StatusMeta &statusMeta)
	{
		return mutateMetadata(statusID, "updateStatus", [&statusMeta](StatusMeta &metadata) { metadata = statusMeta; });
	}

	ATTR_NODISCARD std::expected<StatusID, RegistryErrorInfo> StatusRegistryConfiguration::removeStatus(const std::string_view &statusName)
	{
		return removeMetadata(statusName);
	}

	ATTR_NODISCARD std::expected<StatusID, RegistryErrorInfo> StatusRegistryConfiguration::removeStatus(const StatusID statusID)
	{
		return removeMetadata(statusID);
	}
} // namespace PocketCore::Configuration
