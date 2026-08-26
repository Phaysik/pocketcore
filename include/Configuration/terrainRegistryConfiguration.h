/*! @file terrainRegistryConfiguration.h
	@brief Declares the user-facing facade for configuring terrain metadata.
	@date 07/30/2026
	@since 0.8.0
	@version 0.8.0
	@author Matthew Moore
*/

#ifndef INCLUDE_CONFIGURATION_TERRAIN_REGISTRY_CONFIGURATION_H
#define INCLUDE_CONFIGURATION_TERRAIN_REGISTRY_CONFIGURATION_H

#include <expected>
#include <optional>
#include <span>
#include <string_view>

#include "Configuration/constants.h"
#include "Configuration/fixedMetadataRegistryConfiguration.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Registry/terrainRegistry.h"
#include "Terrain/terrainID.h"
#include "Terrain/terrainMeta.h"

namespace PocketCore::Configuration
{
	using PocketCore::Core::us;
	using PocketCore::Registry::Terrain::TerrainRegistry;
	using PocketCore::Terrain::TerrainID;
	using PocketCore::Terrain::TerrainMeta;

	namespace Detail
	{
		/*! @struct TerrainRegistryConfigurationPolicy Configuration/terrainRegistryConfiguration.h
			@brief Policy class providing error codes and display strings for terrain registry configuration.
			@details Encapsulates the terrain-specific error categories and display names used by the generic
			 @ref FixedMetadataRegistryConfiguration template to report validation and lookup failures with
			 domain-specific terminology.
			@date 07/27/2026
			@since 0.8.0
			@version 0.8.0
			@author Matthew Moore
		*/
		struct TerrainRegistryConfigurationPolicy
		{
			public:
				/*! @brief The display name of the configuration system. */
				static constexpr std::string_view configurationName{"TerrainRegistryConfiguration"};
				
				/*! @brief The singular entity type managed by this configuration. */
				static constexpr std::string_view entityName{"terrain"};
				
				/*! @brief The error code returned when a duplicate ability name is registered. */
				static constexpr RegistryError duplicateError{RegistryError::DuplicateTerrain};
				
				/*! @brief The error code returned when an ability lookup fails. */
				static constexpr RegistryError notFoundError{RegistryError::TerrainNotFound};
		};
	} // namespace Detail

	/*! @class TerrainRegistryConfiguration Configuration/terrainRegistryConfiguration.h
		@brief Provides validated user customization over an internal terrain registry.
		@details Supports lookup, addition, batch addition, trigger replacement, renaming, and removal. Custom IDs are assigned
	   monotonically and are not reused after removal. Batch additions provide all-or-nothing semantics.
		@date 07/30/2026
		@since 0.8.0
		@version 0.8.0
		@author Matthew Moore
	*/
	class TerrainRegistryConfiguration
		: private FixedMetadataRegistryConfiguration<TerrainRegistry, TerrainMeta, TerrainID, MAX_TERRAINS, &TerrainMeta::mTerrainID,
													 Detail::TerrainRegistryConfigurationPolicy>
	{
		private:
			using Base = FixedMetadataRegistryConfiguration<TerrainRegistry, TerrainMeta, TerrainID, MAX_TERRAINS, &TerrainMeta::mTerrainID,
															Detail::TerrainRegistryConfigurationPolicy>;

		public:
			/*! @brief Constructs a configuration containing all built-in terrains.
				@since 0.8.0
				@version 0.8.0
			 */
			constexpr TerrainRegistryConfiguration() = default;

			/*! @brief Returns read-only access to the configured runtime terrain registry.
				@return A reference that remains valid for the lifetime of this configuration.
			*/
			ATTR_NODISCARD constexpr const TerrainRegistry &getRuntimeRegistry() const noexcept
			{
				return Base::getRegistry();
			}

			/*! @brief Looks up complete metadata by stable terrain ID.
				@param[in] terrainID The built-in or custom stable identifier.
				@return A non-owning pointer to metadata if registered, or nullptr otherwise. The pointer remains valid until replacement or
			   configuration destruction.
				@since 0.8.0
				@version 0.8.0
			*/
			ATTR_NODISCARD constexpr const TerrainMeta *getTerrainMetadata(const TerrainID terrainID) const
			{
				return getMetadata(terrainID);
			}

			/*! @brief Looks up a stable terrain ID by display name.
				@param[in] name The case-sensitive display name.
				@return The stable ID if registered, or std::nullopt otherwise.
				@since 0.8.0
				@version 0.8.0
			*/
			ATTR_NODISCARD constexpr const std::optional<TerrainID> getTerrainID(const std::string_view &name) const
			{
				return getID(name);
			}

			/*! @brief Looks up a display name by stable terrain ID.
				@param[in] terrainID The built-in or custom stable identifier.
				@return The display name if registered, or std::nullopt otherwise.
				@since 0.8.0
				@version 0.8.0
			*/
			ATTR_NODISCARD constexpr const std::optional<std::string_view> getTerrainName(const TerrainID terrainID) const
			{
				return getName(terrainID);
			}

			/*! @brief Returns all currently registered terrain definitions.
				@return A read-only span that remains valid until mutation or destruction.
				@since 0.8.0
				@version 0.8.0
			*/
			ATTR_NODISCARD constexpr const std::span<const TerrainMeta> getRegisteredTerrains() const noexcept
			{
				return getRegisteredEntries();
			}

			/*! @brief Checks whether an terrain name is registered.
				@param[in] name The case-sensitive display name.
				@return True if the name is registered, otherwise false.
				@since 0.8.0
				@version 0.8.0
			*/
			ATTR_NODISCARD constexpr bool hasTerrain(const std::string_view &name) const
			{
				return hasEntry(name);
			}

			/*! @brief Checks whether an terrain ID is registered.
				@param[in] terrainID The built-in or custom stable identifier.
				@return True if the ID is registered, otherwise false.
				@since 0.8.0
				@version 0.8.0
			*/
			ATTR_NODISCARD constexpr bool hasTerrain(const TerrainID terrainID) const
			{
				return hasEntry(terrainID);
			}

			/*! @brief Registers one user-defined terrain and assigns a stable ID.
				@param[in] terrainMeta The name and trigger metadata to copy into the registry.
				@return The assigned ID on success, or @ref RegistryErrorInfo on duplicate name or exhausted capacity.
				@since 0.8.0
				@version 0.8.0
			*/
			ATTR_NODISCARD std::expected<TerrainID, RegistryErrorInfo> addTerrain(const TerrainMeta &terrainMeta);

			/*! @brief Registers multiple terrains atomically.
				@details Restores the complete prior registry state if any definition fails validation.
				@param[in] terrainMetas The terrain definitions to register in order.
				@return Void on success, or the first @ref RegistryErrorInfo on failure.
				@since 0.8.0
				@version 0.8.0
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> addTerrains(const std::span<const TerrainMeta> &terrainMetas);

			/*! @brief Renames an terrain without changing its other metadata.
				@details @p newName is stored as a non-owning view and its backing storage must remain valid while registered.
				@param[in] oldName The currently registered display name.
				@param[in] newName The unique replacement display name.
				@return Void on success, or @ref RegistryErrorInfo if the source is absent or target name is already registered.
				@since 0.8.0
				@version 0.8.0
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> renameTerrain(const std::string_view &oldName,
																				const std::string_view &newName);

			/*! @brief Replaces all terrain metadata for an terrain selected by stable ID.
				@param[in] terrainName The registered display name.
				@param[in] terrainMeta The metadata to copy into the registry.
				@return Void on success, or @ref RegistryErrorInfo if the terrain is not registered.
				@since 0.8.0
				@version 0.8.0
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> updateTerrain(const std::string_view &terrainName,
																				const TerrainMeta &terrainMeta);

			/*! @overload std::expected<void, RegistryErrorInfo> updateTerrain(TerrainID, const TerrainMeta &terrainMeta)
				@brief Replaces all terrain metadata for an terrain selected by stable ID.
				@param[in] terrainID The built-in or custom stable identifier.
				@param[in] terrainMeta The metadata to copy into the registry.
				@return Void on success, or @ref RegistryErrorInfo if the terrain is not registered.
				@since 0.8.0
				@version 0.8.0
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> updateTerrain(const TerrainID terrainID, const TerrainMeta &terrainMeta);

			/*! @brief Removes an terrain by display name.
				@param[in] terrainName The registered display name.
				@return The removed stable ID on success, or @ref RegistryErrorInfo if no matching terrain exists.
				@since 0.8.0
				@version 0.8.0
			*/
			ATTR_NODISCARD std::expected<TerrainID, RegistryErrorInfo> removeTerrain(const std::string_view &terrainName);

			/*! @overload std::expected<TerrainID, RegistryErrorInfo> removeTerrain(TerrainID)
				@brief Removes an terrain by stable ID.
				@param[in] terrainID The built-in or custom stable identifier.
				@return The removed stable ID on success, or @ref RegistryErrorInfo if no matching terrain exists.
				@since 0.8.0
				@version 0.8.0
			*/
			ATTR_NODISCARD std::expected<TerrainID, RegistryErrorInfo> removeTerrain(const TerrainID terrainID);
	};
} // namespace PocketCore::Configuration

#endif
