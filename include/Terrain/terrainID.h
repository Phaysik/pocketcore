/*! @file terrainID.h
	@brief Contains the terrain status
	@date 07/30/2026
	@since 0.8.0
	@version 0.8.0
	@author Matthew Moore
*/

#ifndef INCLUDE_TERRAIN_TERRAINID_H
#define INCLUDE_TERRAIN_TERRAINID_H

#include "ID/idInterface.h"

namespace PocketCore::Terrain
{
	namespace Detail
	{
		/*! @brief Distinguishes terrain identifiers from all other stable identifier domains. */
		struct TerrainIDTag;
	} // namespace Detail

	/*! @typedef TerrainID
		@brief A strongly typed stable identifier for any registered terrain.
		@details Values are assigned by the terrain registry. Unlike @ref BuiltinTerrainID, this type is open and can represent user-defined
	   abilities without extending an enum. Its tag prevents comparison or conversion with identifiers from other registry domains.
	*/
	using TerrainID = PocketCore::ID::IDInterface<Detail::TerrainIDTag, 0>;

	/*! @brief The stable identifier representing no terrain. */
	inline constexpr TerrainID NO_TERRAIN_ID{};
} // namespace PocketCore::Terrain

#endif
