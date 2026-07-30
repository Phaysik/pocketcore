/*! @file terrainRegistry.h
	@brief Provides fixed-capacity storage and lookup for built-in and user-defined terrains.
	@date 07/27/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_REGISTRY_TERRAIN_REGISTRY_H
#define INCLUDE_REGISTRY_TERRAIN_REGISTRY_H

#include <optional>
#include <span>
#include <string_view>

#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Effect/effectType.h"
#include "Registry/fixedMetadataRegistry.h"
#include "Terrain/builtinTerrainID.h"
#include "Terrain/constants.h"
#include "Terrain/terrainID.h"
#include "Terrain/terrainMeta.h"

namespace PocketCore::Registry::Terrain
{
	using PocketCore::Configuration::MAX_TERRAINS;
	using PocketCore::Core::us;
	using PocketCore::Effect::EffectTypeID;
	using PocketCore::Terrain::BuiltinTerrainID;
	using PocketCore::Terrain::TerrainID;
	using PocketCore::Terrain::TerrainMeta;
	using PocketCore::Terrain::toTerrainID;

	/*! @class TerrainRegistry Registry/terrainRegistry.h
		@brief Stores built-in and user-defined terrain metadata in fixed-capacity storage.
		@details Built-in terrains are registered during construction with IDs derived from @ref BuiltinTerrainID. Configuration code may
	   append, replace, or remove entries through the low-level mutators while battle-time callers use allocation-free lookup operations.
		@note Lookup operations are O(n), where n is bounded by @ref MAX_TERRAINS.
		@date 07/27/2026
		@version x.x.x
		@since x.x.x
		@author Matthew Moore
	*/
	class TerrainRegistry
		: private PocketCore::Registry::FixedMetadataRegistry<TerrainMeta, TerrainID, MAX_TERRAINS, &TerrainMeta::mTerrainID>
	{
		private:
			using Base = PocketCore::Registry::FixedMetadataRegistry<TerrainMeta, TerrainID, MAX_TERRAINS, &TerrainMeta::mTerrainID>;

		public:
			// LCOV_EXCL_START - If the built in additions fail, the program wouldn't work anyway
			/*! @brief Constructs a registry populated with every @ref BuiltinTerrainID. */
			ATTR_NOINLINE explicit constexpr TerrainRegistry()
				: Base{static_cast<us>(toTerrainID(BuiltinTerrainID::Psychic).getValue() + 1U)}
			{
				addBuiltin({.mName = PocketCore::Terrain::TERRAIN_NAME_NONE, .mTerrainID = toTerrainID(BuiltinTerrainID::None)});
				addBuiltin({
					.mName = PocketCore::Terrain::TERRAIN_NAME_ELECTRIC,
					.mTerrainID = toTerrainID(BuiltinTerrainID::Electric),
				});
				addBuiltin({
					.mName = PocketCore::Terrain::TERRAIN_NAME_GRASS,
					.mTerrainID = toTerrainID(BuiltinTerrainID::Grass),
				});
				addBuiltin({
					.mName = PocketCore::Terrain::TERRAIN_NAME_MISTY,
					.mTerrainID = toTerrainID(BuiltinTerrainID::Misty),
				});
				addBuiltin({
					.mName = PocketCore::Terrain::TERRAIN_NAME_PSYCHIC,
					.mTerrainID = toTerrainID(BuiltinTerrainID::Psychic),
				});
			}

			// LCOV_EXCL_STOP

			using Base::decrementAmountRegistered;
			using Base::findIndexByID;
			using Base::getAmountRegistered;
			using Base::getEntry;
			using Base::getID;
			using Base::getMetadata;
			using Base::getName;
			using Base::getNextID;
			using Base::getRegisteredEntries;
			using Base::hasEntry;
			using Base::incrementAmountRegistered;
			using Base::incrementNextID;
			using Base::setAmountRegistered;
			using Base::setEntry;
			using Base::setNextID;

			/*! @brief Looks up terrain metadata by stable ID.
				@param[in] terrainID The stable terrain identifier.
				@return A non-owning pointer to metadata if registered, or nullptr otherwise. The pointer remains valid until replacement or
			   registry destruction.
			*/
			ATTR_NODISCARD constexpr const TerrainMeta *getTerrainMetadata(const TerrainID terrainID) const
			{
				return getMetadata(terrainID);
			}

			/*! @brief Looks up an terrain ID by display name.
				@param[in] name The case-sensitive display name.
				@return The stable ID if registered, or std::nullopt otherwise.
			*/
			ATTR_NODISCARD constexpr const std::optional<TerrainID> getTerrainID(const std::string_view &name) const
			{
				return getID(name);
			}

			/*! @brief Looks up an terrain display name by stable ID.
				@param[in] terrainID The stable terrain identifier.
				@return The display name if registered, or std::nullopt otherwise.
			*/
			ATTR_NODISCARD constexpr const std::optional<std::string_view> getTerrainName(const TerrainID terrainID) const
			{
				return getName(terrainID);
			}

			/*! @brief Returns all currently registered terrain definitions.
				@return A read-only span that remains valid until the registry is mutated or destroyed.
			*/
			ATTR_NODISCARD constexpr const std::span<const TerrainMeta> getRegisteredTerrains() const noexcept
			{
				return getRegisteredEntries();
			}

			/*! @brief Returns the next stable ID assigned to a custom terrain.
				@return The underlying numeric value of the next terrain ID.
			*/
			ATTR_NODISCARD constexpr us getNextTerrainID() const noexcept
			{
				return getNextID();
			}

			/*! @brief Finds an internal array index by stable terrain ID.
				@param[in] terrainID The stable terrain identifier.
				@return The internal index if registered, or std::nullopt otherwise.
			*/
			ATTR_NODISCARD constexpr const std::optional<us> findIndexByTerrainID(const TerrainID terrainID) const
			{
				return findIndexByID(terrainID);
			}

			/*! @brief Checks whether an terrain name is registered.
				@param[in] name The case-sensitive display name.
				@return True if the name is registered, otherwise false.
			*/
			ATTR_NODISCARD constexpr bool hasTerrain(const std::string_view &name) const
			{
				return hasEntry(name);
			}

			/*! @brief Checks whether an terrain ID is registered.
				@param[in] terrainID The stable terrain identifier.
				@return True if the ID is registered, otherwise false.
			*/
			ATTR_NODISCARD constexpr bool hasTerrain(const TerrainID terrainID) const
			{
				return hasEntry(terrainID);
			}

			/*! @brief Sets the next custom terrain ID counter.
				@param[in] nextID The next underlying ID value.
			*/
			constexpr void setNextTerrainID(const us nextID) noexcept
			{
				setNextID(nextID);
			}

			/*! @brief Increments the next custom terrain ID counter. */
			constexpr void incrementNextTerrainID() noexcept
			{
				incrementNextID();
			}
	};
} // namespace PocketCore::Registry::Terrain

#endif
