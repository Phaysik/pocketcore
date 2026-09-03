/*! @file terrainMeta.h
	@brief Defines the metadata stored for built-in and user-defined terrains.
	@date 09/03/2026
	@since 0.8.0
	@version 0.12.19
	@author Matthew Moore
*/

#ifndef INCLUDE_TERRAIN_TERRAINMETA_H
#define INCLUDE_TERRAIN_TERRAINMETA_H

#include <string_view>

#include "terrainID.h"

namespace PocketCore::Terrain
{
	/*! @struct TerrainMeta Terrain/terrainMeta.h
		@brief Stores one terrain's stable ID, display name, and owned trigger definitions.
		@details The trigger vector owns its elements and their effect vectors. The display name is a non-owning view whose backing storage
	   must remain valid while this metadata is registered.
		@date 09/03/2026
		@since 0.8.0
		@version 0.12.19
		@author Matthew Moore
	*/
	struct TerrainMeta
	{
		public:
			/*! @brief Compares two TerrainMeta instances for equivalent metadata.
				@details Compares all fields exactly.
				@param[in] other The TerrainMeta instance to compare.
				@return True when both instances contain equivalent metadata; otherwise false.
				@since 0.12.19
				@version 0.12.19
			*/
			ATTR_NODISCARD constexpr bool operator==(const TerrainMeta &other) const noexcept = default;

			// NOLINTBEGIN(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)

			/*! @brief The case-sensitive display name stored as a non-owning view. */
			std::string_view mName{};

			/*! @brief The stable built-in or user-assigned identifier. */
			TerrainID mTerrainID{};

			// NOLINTEND(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)
	};
} // namespace PocketCore::Terrain

#endif
