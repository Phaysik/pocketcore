/*! @file eggGroupRegistryConfiguration.cpp
	@brief Defines validated user customization operations for the eggGroup registry.
	@date 10/26/2026
	@since 0.12.53
	@version 0.12.53
	@author Matthew Moore
*/

#include "Configuration/eggGroupRegistryConfiguration.h"

#include <expected>
#include <span>
#include <string_view>

#include "Core/attributeMacros.h"
#include "EggGroup/eggGroupID.h"
#include "EggGroup/eggGroupMeta.h"
#include "Validation/Registry/registryError.h"

namespace PocketCore::Configuration
{
	using PocketCore::EggGroup::EggGroupID;
	using PocketCore::EggGroup::EggGroupMeta;
	using PocketCore::Validation::Registry::RegistryError;

	ATTR_NODISCARD std::expected<EggGroupID, RegistryError> EggGroupRegistryConfiguration::addEggGroup(const EggGroupMeta &eggGroupMeta)
	{
		return addMetadata(eggGroupMeta);
	}

	ATTR_NODISCARD std::expected<void, RegistryError> EggGroupRegistryConfiguration::addEggGroups(
		const std::span<const EggGroupMeta> eggGroupMetas)
	{
		return addMetadataBatch(eggGroupMetas, [](const EggGroupMeta &definition) { return EggGroupMeta{definition}; });
	}

	ATTR_NODISCARD std::expected<void, RegistryError> EggGroupRegistryConfiguration::renameEggGroup(const std::string_view oldName,
																									const std::string_view newName)
	{
		return renameMetadata(oldName, newName);
	}

	ATTR_NODISCARD std::expected<void, RegistryError> EggGroupRegistryConfiguration::updateEggGroup(const std::string_view eggGroupName,
																									const EggGroupMeta &eggGroupMeta)
	{
		return mutateMetadata(eggGroupName, "updateEggGroup", [&eggGroupMeta](EggGroupMeta &metadata) { metadata = eggGroupMeta; });
	}

	ATTR_NODISCARD std::expected<void, RegistryError> EggGroupRegistryConfiguration::updateEggGroup(const EggGroupID eggGroupID,
																									const EggGroupMeta &eggGroupMeta)
	{
		return mutateMetadata(eggGroupID, "updateEggGroup", [&eggGroupMeta](EggGroupMeta &metadata) { metadata = eggGroupMeta; });
	}

	ATTR_NODISCARD std::expected<EggGroupID, RegistryError> EggGroupRegistryConfiguration::removeEggGroup(
		const std::string_view eggGroupName)
	{
		return removeMetadata(eggGroupName);
	}

	ATTR_NODISCARD std::expected<EggGroupID, RegistryError> EggGroupRegistryConfiguration::removeEggGroup(const EggGroupID eggGroupID)
	{
		return removeMetadata(eggGroupID);
	}
} // namespace PocketCore::Configuration
