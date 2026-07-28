/*! @file moveRegistryConfiguration.cpp
	@brief Defines validated user customization operations for the move registry.
	@date 07/27/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#include "Configuration/moveRegistryConfiguration.h"

#include <expected>
#include <span>
#include <string_view>
#include <vector>

#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Move/moveID.h"
#include "Move/moveMeta.h"
#include "Move/moveTargetsAndTriggers.h"

namespace PocketCore::Configuration
{
	using PocketCore::Move::MoveID;
	using PocketCore::Move::MoveMeta;
	using PocketCore::Move::MoveTargetID;

	ATTR_NODISCARD std::expected<MoveID, RegistryErrorInfo> MoveRegistryConfiguration::addMove(const MoveMeta &moveMeta)
	{
		return addMetadata(moveMeta);
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> MoveRegistryConfiguration::addMoves(const std::span<const MoveMeta> &moveMetas)
	{
		return addMetadataBatch(moveMetas, [](const MoveMeta &definition) { return MoveMeta{definition}; });
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> MoveRegistryConfiguration::setMoveTriggers(
		const std::string_view &moveName, const std::span<const MoveEffectTrigger> &triggers)
	{
		return mutateMetadata(moveName, "setMoveTriggers",
							  [&triggers](MoveMeta &metadata) { metadata.mTriggers.assign(triggers.begin(), triggers.end()); });
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> MoveRegistryConfiguration::setMoveTriggers(
		const MoveID moveID, const std::span<const MoveEffectTrigger> &triggers)
	{
		return mutateMetadata(moveID, "setMoveTriggers",
							  [&triggers](MoveMeta &metadata) { metadata.mTriggers.assign(triggers.begin(), triggers.end()); });
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> MoveRegistryConfiguration::setMoveTarget(const std::string_view &moveName,
																								   const MoveTargetID target)
	{
		return mutateMetadata(moveName, "setMoveTarget", [&target](MoveMeta &metadata) { metadata.mTargetID = target; });
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> MoveRegistryConfiguration::setMoveTarget(const MoveID moveID,
																								   const MoveTargetID target)
	{
		return mutateMetadata(moveID, "setMoveTarget", [&target](MoveMeta &metadata) { metadata.mTargetID = target; });
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> MoveRegistryConfiguration::renameMove(const std::string_view &oldName,
																								const std::string_view &newName)
	{
		return renameMetadata(oldName, newName);
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> MoveRegistryConfiguration::updateMove(const std::string_view &moveName,
																								const MoveMeta &moveMeta)
	{
		return mutateMetadata(moveName, "updateMove", [&moveMeta](MoveMeta &metadata) { metadata = moveMeta; });
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> MoveRegistryConfiguration::updateMove(const MoveID moveID,
																								const MoveMeta &moveMeta)
	{
		return mutateMetadata(moveID, "updateMove", [&moveMeta](MoveMeta &metadata) { metadata = moveMeta; });
	}

	ATTR_NODISCARD std::expected<MoveID, RegistryErrorInfo> MoveRegistryConfiguration::removeMove(const std::string_view &moveName)
	{
		return removeMetadata(moveName);
	}

	ATTR_NODISCARD std::expected<MoveID, RegistryErrorInfo> MoveRegistryConfiguration::removeMove(const MoveID moveID)
	{
		return removeMetadata(moveID);
	}
} // namespace PocketCore::Configuration
