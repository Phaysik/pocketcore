/*! \file types.h
	\brief Contains the function declarations for creating Pokemon Types
	\date 03/09/2026
	\version x.x.x
	\since x.x.x
	\author Matthew Moore
*/

#ifndef INCLUDE_TYPES_TYPES_H
#define INCLUDE_TYPES_TYPES_H

#include "Core/typedefs.h"

namespace PocketCore::Types
{
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
