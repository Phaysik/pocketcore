/*! @file terrainRegistryConfiguration.cpp
	@brief Defines validated user customization operations for the terrain registry.
	@date 08/30/2026
	@since 0.8.1
	@version 0.12.12
	@author Matthew Moore
*/

#include "Configuration/terrainRegistryConfiguration.h"

#include <expected>
#include <span>
#include <string_view>

#include "Core/attributeMacros.h"
#include "Registry/registryError.h"
#include "Terrain/terrainID.h"
#include "Terrain/terrainMeta.h"

namespace PocketCore::Configuration
{
	using PocketCore::Registry::RegistryErrorInfo;
	using PocketCore::Terrain::TerrainID;
	using PocketCore::Terrain::TerrainMeta;

	ATTR_NODISCARD std::expected<TerrainID, RegistryErrorInfo> TerrainRegistryConfiguration::addTerrain(const TerrainMeta &terrainMeta)
	{
		return addMetadata(terrainMeta);
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> TerrainRegistryConfiguration::addTerrains(
		const std::span<const TerrainMeta> &terrainMetas)
	{
		return addMetadataBatch(terrainMetas, [](const TerrainMeta &definition) { return TerrainMeta{definition}; });
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> TerrainRegistryConfiguration::renameTerrain(const std::string_view &oldName,
																									  const std::string_view &newName)
	{
		return renameMetadata(oldName, newName);
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> TerrainRegistryConfiguration::updateTerrain(const std::string_view &terrainName,
																									  const TerrainMeta &terrainMeta)
	{
		return mutateMetadata(terrainName, "updateTerrain", [&terrainMeta](TerrainMeta &metadata) { metadata = terrainMeta; });
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> TerrainRegistryConfiguration::updateTerrain(const TerrainID terrainID,
																									  const TerrainMeta &terrainMeta)
	{
		return mutateMetadata(terrainID, "updateTerrain", [&terrainMeta](TerrainMeta &metadata) { metadata = terrainMeta; });
	}

	ATTR_NODISCARD std::expected<TerrainID, RegistryErrorInfo> TerrainRegistryConfiguration::removeTerrain(
		const std::string_view &terrainName)
	{
		return removeMetadata(terrainName);
	}

	ATTR_NODISCARD std::expected<TerrainID, RegistryErrorInfo> TerrainRegistryConfiguration::removeTerrain(const TerrainID terrainID)
	{
		return removeMetadata(terrainID);
	}
} // namespace PocketCore::Configuration
