/*! @file multiplierRegistryConfiguration.cpp
	@brief Defines validated user customization operations for the multiplier registry.
	@date 08/03/2026
	@since 0.8.1
	@version 0.9.0
	@author Matthew Moore
*/

#include "Configuration/multiplierRegistryConfiguration.h"

#include <expected>
#include <span>
#include <string_view>

#include "Core/attributeMacros.h"
#include "Multiplier/multiplierID.h"
#include "Multiplier/multiplierMeta.h"
#include "Registry/registryError.h"

namespace PocketCore::Configuration
{
	using PocketCore::Multiplier::MultiplierID;
	using PocketCore::Multiplier::MultiplierMeta;
	using PocketCore::Registry::RegistryErrorInfo;

	ATTR_NODISCARD std::expected<MultiplierID, RegistryErrorInfo> MultiplierRegistryConfiguration::addMultiplier(
		const MultiplierMeta &multiplierMeta)
	{
		return addMetadata(multiplierMeta);
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> MultiplierRegistryConfiguration::addMultipliers(
		const std::span<const MultiplierMeta> &multiplierMetas)
	{
		return addMetadataBatch(multiplierMetas, [](const MultiplierMeta &definition) { return MultiplierMeta{definition}; });
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> MultiplierRegistryConfiguration::renameMultiplier(const std::string_view &oldName,
																											const std::string_view &newName)
	{
		return renameMetadata(oldName, newName);
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> MultiplierRegistryConfiguration::updateMultiplier(
		const std::string_view &multiplierName, const MultiplierMeta &multiplierMeta)
	{
		return mutateMetadata(multiplierName, "updateMultiplier",
							  [&multiplierMeta](MultiplierMeta &metadata) { metadata = multiplierMeta; });
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> MultiplierRegistryConfiguration::updateMultiplier(
		const MultiplierID multiplierID, const MultiplierMeta &multiplierMeta)
	{
		return mutateMetadata(multiplierID, "updateMultiplier", [&multiplierMeta](MultiplierMeta &metadata) { metadata = multiplierMeta; });
	}

	ATTR_NODISCARD std::expected<MultiplierID, RegistryErrorInfo> MultiplierRegistryConfiguration::removeMultiplier(
		const std::string_view &multiplierName)
	{
		return removeMetadata(multiplierName);
	}

	ATTR_NODISCARD std::expected<MultiplierID, RegistryErrorInfo> MultiplierRegistryConfiguration::removeMultiplier(
		const MultiplierID multiplierID)
	{
		return removeMetadata(multiplierID);
	}
} // namespace PocketCore::Configuration
