/*! @file natureRegistryConfiguration.cpp
	@brief Defines validated user customization operations for the nature registry.
	@date 07/27/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#include "Configuration/natureRegistryConfiguration.h"

#include <expected>
#include <span>
#include <string_view>

#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Nature/natureID.h"
#include "Nature/natureMeta.h"

namespace PocketCore::Configuration
{
	using PocketCore::Nature::NatureID;
	using PocketCore::Nature::NatureMeta;

	ATTR_NODISCARD std::expected<NatureID, RegistryErrorInfo> NatureRegistryConfiguration::addNature(const NatureMeta &natureMeta)
	{
		return addMetadata(natureMeta);
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> NatureRegistryConfiguration::addNatures(
		const std::span<const NatureMeta> &natureMetas)
	{
		return addMetadataBatch(natureMetas, [](const NatureMeta &definition) { return NatureMeta{definition}; });
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> NatureRegistryConfiguration::renameNature(const std::string_view &oldName,
																									const std::string_view &newName)
	{
		return renameMetadata(oldName, newName);
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> NatureRegistryConfiguration::updateNature(const std::string_view &natureName,
																									const NatureMeta &natureMeta)
	{
		return mutateMetadata(natureName, "updateNature", [&natureMeta](NatureMeta &metadata) { metadata = natureMeta; });
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> NatureRegistryConfiguration::updateNature(const NatureID natureID,
																									const NatureMeta &natureMeta)
	{
		return mutateMetadata(natureID, "updateNature", [&natureMeta](NatureMeta &metadata) { metadata = natureMeta; });
	}

	ATTR_NODISCARD std::expected<NatureID, RegistryErrorInfo> NatureRegistryConfiguration::removeNature(const std::string_view &natureName)
	{
		return removeMetadata(natureName);
	}

	ATTR_NODISCARD std::expected<NatureID, RegistryErrorInfo> NatureRegistryConfiguration::removeNature(const NatureID natureID)
	{
		return removeMetadata(natureID);
	}
} // namespace PocketCore::Configuration
