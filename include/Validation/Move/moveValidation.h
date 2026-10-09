/*! @file moveValidation.h
	@brief Defines validation functions that help with Move.
	@date 10/09/2026
	@since 0.12.51
	@version 0.12.51
	@author Matthew Moore
*/

#ifndef INCLUDE_VALIDATION_MOVE_MOVE_VALIDATION_H
#define INCLUDE_VALIDATION_MOVE_MOVE_VALIDATION_H

#include <algorithm>
#include <expected>
#include <span>

#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Move/moveID.h"
#include "Move/moveMeta.h"
#include "Registry/moveRegistry.h"

#include "moveError.h"

namespace PocketCore::Validation::Move
{
	using PocketCore::Configuration::MAX_MOVES_PER_POKEMON;
	using PocketCore::Core::us;
	using PocketCore::Move::MoveID;
	using PocketCore::Move::MoveMeta;
	using PocketCore::Registry::Move::MoveRegistry;

	/*! @brief Validates the metadata for the given move IDs.
		@param[in] moveIDs The array of move IDs to validate.
		@param[in] moveRegistry The registry to validate move metadata.
		@return A @ref MoveError if any move IDs are invalid, or std::nullopt if all are valid.
		@since 0.12.26
		@version 0.12.51
	 */
	ATTR_NODISCARD constexpr std::expected<void, MoveError> validateMoveMetadata(
		const std::span<const MoveID, MAX_MOVES_PER_POKEMON> *moveIDs, const MoveRegistry &moveRegistry)
	{
		std::array<const MoveMeta *, MAX_MOVES_PER_POKEMON> moveMetas{};
		std::ranges::transform(*moveIDs, moveMetas.begin(),
							   [&moveRegistry](const MoveID moveID) { return moveRegistry.getMoveMetadata(moveID); });

		if (std::ranges::any_of(moveMetas, [](const MoveMeta *meta) { return meta == nullptr; }))
		{
			return std::unexpected{MoveError::MISSING_MOVE_METADATA};
		}

		return {};
	}

} // namespace PocketCore::Validation::Move

#endif
