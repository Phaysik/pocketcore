/*! @file typeEffectiveness.h
	@brief Contains the function declarations for creating a Detailed file description
	@date 07/26/2026
	@since 0.1.0
	@version 0.3.0
	@author Matthew Moore
*/

#ifndef INCLUDE_REGISTRY_TYPEEFFECTIVENESS_H
#define INCLUDE_REGISTRY_TYPEEFFECTIVENESS_H

#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"

namespace PocketCore::Types
{
	using PocketCore::Configuration::EFFECTIVE_VALUE;
	using PocketCore::Configuration::NO_EFFECTIVE_VALUE;
	using PocketCore::Configuration::NOT_VERY_EFFECTIVE_VALUE;
	using PocketCore::Configuration::SUPER_EFFECTIVE_VALUE;
	using PocketCore::Core::ub;

	/*! @enum TypeEffectiveness
		@showenumvalues
		@brief Describes the effectiveness multiplier of one type attacking another.
		@details Used within the type chart to encode damage multipliers. @ref NOT_DEFINED serves as a sentinel for uninitialized matchup
	   slots.
		@since 0.1.0
		@version 0.1.0
	*/
	enum class TypeEffectiveness : ub
	{
		NOT_DEFINED, /*!< Sentinel value for uninitialized matchups. */
		NE,			 /*!< No effect (0x multiplier). */
		NVE,		 /*!< Not very effective (0.5x multiplier). */
		E,			 /*!< Effective / neutral (1x multiplier). */
		SE,			 /*!< Super effective (1.5x multiplier). */
	};

	/*! @brief Returns the effectiveness value for a given type effectiveness.
		@param effectiveNess The type effectiveness to get the value for.
		@return The effectiveness value as a double.
	*/
	ATTR_NODISCARD constexpr double getEffectivenessValue(const TypeEffectiveness effectiveNess)
	{
		using enum TypeEffectiveness;

		switch (effectiveNess)
		{
			case NVE:
				return NOT_VERY_EFFECTIVE_VALUE;
			case E:
				return EFFECTIVE_VALUE;
			case SE:
				return SUPER_EFFECTIVE_VALUE;
			case NE:
				ATTR_FALLTHROUGH;
			case NOT_DEFINED:
				ATTR_FALLTHROUGH;
			default:
				return NO_EFFECTIVE_VALUE;
		}
	}
} // namespace PocketCore::Types

#endif
