/*! @file builtinTerrainID.h
	@brief Defines identifiers for terrains compiled into PocketCore.
	@date 07/30/2026
	@since 0.8.0
	@version 0.8.0
	@author Matthew Moore
*/

#ifndef INCLUDE_TERRAIN_BUILTIN_TERRAIN_ID_H
#define INCLUDE_TERRAIN_BUILTIN_TERRAIN_ID_H

#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Terrain/terrainID.h"

namespace PocketCore::Terrain
{
	/*! @enum BuiltinTerrainID
		@showenumvalues
		@brief Names the terrains provided by PocketCore itself.
		@details This closed enum is only a catalog of built-in terrains. Runtime state and user-facing APIs use the open @ref TerrainID
	   type.
		@note All enum values must be handled exhaustively when registering built-in metadata.
		@date 07/30/2026
		@since 0.8.0
		@version 0.8.0
		@author Matthew Moore
	*/
	enum class BuiltinTerrainID : PocketCore::Core::ub
	{
		None,
		Electric,
		Grass,
		Misty,
		Psychic,
		FinalTerrain,
	};

	/*! @brief Converts a built-in catalog value to its stable registry identifier.
		@param[in] builtinTerrainID The built-in terrain to convert.
		@return The corresponding open terrain identifier.
		@since 0.8.0
		@version 0.8.0
	*/
	ATTR_NODISCARD constexpr TerrainID toTerrainID(const BuiltinTerrainID builtinTerrainID) noexcept
	{
		return TerrainID{static_cast<PocketCore::Core::ub>(builtinTerrainID)};
	}
} // namespace PocketCore::Terrain

#endif
