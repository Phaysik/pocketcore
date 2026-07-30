/*! @file terrainMeta.h
	@brief Defines the metadata stored for built-in and user-defined terrains.
	@date 07/24/2026
	@version x.x.x
	@since x.x.x
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
		@date 07/27/2026
		@version x.x.x
		@since x.x.x
		@author Matthew Moore
	*/
	struct TerrainMeta
	{
		public:
			/*! @brief The case-sensitive display name stored as a non-owning view. */
			std::string_view mName{};

			/*! @brief The stable built-in or user-assigned identifier. */
			TerrainID mTerrainID{};
	};
} // namespace PocketCore::Terrain

#endif
