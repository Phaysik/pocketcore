/*! @file typeEffectiveness.h
	@brief Contains the function declarations for creating a Detailed file description
	@date 03/25/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_REGISTRY_TYPEEFFECTIVENESS_H
#define INCLUDE_REGISTRY_TYPEEFFECTIVENESS_H

#include "Core/typedefs.h"

namespace Pokemon::Types
{
	using Pokemon::Core::ub;

	/*! @enum TypeEffectiveness
		@showenumvalues
		@brief Describes the effectiveness multiplier of one type attacking another.
		@details Used within the type chart to encode damage multipliers. @ref NOT_DEFINED serves as a sentinel for uninitialized matchup
	   slots.
	*/
	enum class TypeEffectiveness : ub
	{
		NOT_DEFINED, /*!< Sentinel value for uninitialized matchups. */
		NE,			 /*!< No effect (0x multiplier). */
		NVE,		 /*!< Not very effective (0.5x multiplier). */
		E,			 /*!< Effective / neutral (1x multiplier). */
		SE,			 /*!< Super effective (1.5x multiplier). */
	};
} // namespace Pokemon::Types

#endif