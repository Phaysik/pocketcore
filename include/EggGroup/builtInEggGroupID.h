/*! @file builtInEggGroupID.h
	@brief Defines identifiers for egg groups compiled into PocketCore.
	@date 10/27/2026
	@since 0.12.53
	@version 0.12.53
	@author Matthew Moore
*/

#ifndef INCLUDE_EGGGROUP_BUILTIN_EGGGROUP_ID_H
#define INCLUDE_EGGGROUP_BUILTIN_EGGGROUP_ID_H

#include <utility>

#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"

#include "eggGroupID.h"

namespace PocketCore::EggGroup
{
	using PocketCore::Configuration::MAX_EGG_GROUPS_PER_POKEMON;
	using PocketCore::Core::ub;

	/*! @enum BuiltinEggGroupID
		@showenumvalues
		@brief Names the egg groups provided by PocketCore itself.
		@details This closed enum is only a catalog of built-in egg groups. Runtime state and user-facing APIs use the open @ref EggGroupID
	   type.
		@note All enum values must be handled exhaustively when registering built-in metadata.
		@date 10/26/2026
		@since 0.12.53
		@version 0.12.53
		@author Matthew Moore
	*/
	enum class BuiltinEggGroupID : ub
	{
		None,
		Amorphous,
		Bug,
		Dragon,
		Fairy,
		Field,
		Flying,
		Grass,
		HumanLike,
		Mineral,
		Monster,
		Water1,
		Water2,
		Water3,
		Ditto,
		NoEggs,
		FinalEggGroup,
	};

	static_assert(MAX_EGG_GROUPS_PER_POKEMON < std::to_underlying(BuiltinEggGroupID::FinalEggGroup));

	/*! @brief Converts a built-in catalog value to its stable registry identifier.
		@param[in] builtinEggGroupID The built-in egg group to convert.
		@return The corresponding open egg group identifier.
		@since 0.12.53
		@version 0.12.53
	*/
	ATTR_NODISCARD constexpr EggGroupID toEggGroupID(const BuiltinEggGroupID builtinEggGroupID) noexcept
	{
		return EggGroupID{std::to_underlying(builtinEggGroupID)};
	}

	/*! @brief The stable identifier representing no egg group. */
	inline constexpr EggGroupID NO_EGG_GROUP_ID{toEggGroupID(BuiltinEggGroupID::None)};
} // namespace PocketCore::EggGroup

#endif
