/*! @file types.h
	@brief Declares the canonical enumeration of Pokemon elemental types.
	@date 08/03/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_TYPES_TYPES_H
#define INCLUDE_TYPES_TYPES_H

#include "Core/typedefs.h"

namespace PocketCore::Types
{
	/*! @enum Types
		@showenumvalues
		@brief Enumerates all supported elemental type categories.
		@details Values map to compact, stable identifiers used by type metadata, move metadata, and type-effectiveness calculations.
		@note The underlying type is @ref PocketCore::Core::ub to minimize storage overhead.
		@date 08/03/2026
		@version x.x.x
		@since x.x.x
		@author Matthew Moore
	*/
	enum class Types : PocketCore::Core::ub
	{
		Normal,
		Fire,
		Fighting,
		Water,
		Flying,
		Grass,
		Poison,
		Electric,
		Ground,
		Psychic,
		Rock,
		Ice,
		Bug,
		Dragon,
		Ghost,
		Dark,
		Steel,
		Fairy,
		Stellar,
	};
} // namespace PocketCore::Types

#endif
