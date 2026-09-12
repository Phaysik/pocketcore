/*! @file moveHelpers.h
	@brief Houses free functions that aide in handling moves.
	@date 09/11/2026
	@since 0.12.26
	@version 0.12.27
	@author Matthew Moore
*/

#ifndef INCLUDE_MOVE_MOVE_HELPERS_H
#define INCLUDE_MOVE_MOVE_HELPERS_H

#include <utility>

#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Registry/moveRegistry.h"
#include "Utility/random.h"

#include "builtInMoveID.h"
#include "moveID.h"

namespace PocketCore::Move
{
	using PocketCore::Core::us;
	using PocketCore::Registry::Move::MoveRegistry;
	using PocketCore::Utility::Random;

	/*! @brief Gets a random move ID from the built-in moves.
		@return The random move ID.
		@since 0.12.26
		@version 0.12.26
	*/
	ATTR_NODISCARD constexpr MoveID getRandomMoveIDFromBuiltIns()
	{
		return MoveID(Random::get<us>(std::to_underlying(BuiltinMoveID::None) + 1, std::to_underlying(BuiltinMoveID::FinalMove) - 1));
	}

	/*! @brief Gets a random move ID from the given configuration.
		@param[in] config The configuration to get the random move ID from.
		@return The random move ID.
		@since 0.12.26
		@version 0.12.27
	*/
	ATTR_NODISCARD constexpr MoveID getRandomMoveIDFromConfiguration(const MoveRegistry &config)
	{
		return MoveID(Random::get<us>(std::to_underlying(BuiltinMoveID::None) + 1, config.getAmountRegistered()));
	}
} // namespace PocketCore::Move

#endif
