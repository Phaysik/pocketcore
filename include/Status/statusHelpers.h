/*! @file statusHelpers.h
	@brief Defines identifiers for status aides.
	@date 07/27/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_STATUS_STATUS_HELPERS_H
#define INCLUDE_STATUS_STATUS_HELPERS_H

#include <algorithm>
#include <cstddef>

#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Registry/statusRegistry.h"
#include "Status/statusID.h"
#include "Status/statusMeta.h"

namespace PocketCore::Status
{
	using PocketCore::Registry::Status::StatusRegistry;

	ATTR_NODISCARD constexpr bool hasInteraction(const StatusID statusID, const StatusID existingStatusID,
												 const StatusInteractionAction action, const StatusRegistry &statusRegistry)
	{
		const StatusMeta *statusMeta{statusRegistry.getStatusMetadata(statusID)};

		if (statusMeta == nullptr)
		{
			return false;
		}

		return std::ranges::any_of(statusMeta->mStatusInteractions, [existingStatusID, action](const auto &interaction) {
			return interaction.mExistingStatusID == existingStatusID && interaction.mAction == action;
		});
	}

	ATTR_NODISCARD constexpr bool willBlockIncoming(const StatusID statusID, const StatusRegistry &statusRegistry,
													const std::array<StatusID, Configuration::MAX_STATUSES_PER_POKEMON> &statusIDs)
	{
		return std::ranges::any_of(statusIDs, [statusID, &statusRegistry](const StatusID existingStatusID) {
			return hasInteraction(statusID, existingStatusID, StatusInteractionAction::BlockIncoming, statusRegistry);
		});
	}

	ATTR_NODISCARD constexpr bool statusAlreadyExists(const StatusID statusID,
													  const std::array<StatusID, Configuration::MAX_STATUSES_PER_POKEMON> &statusIDs)
	{
		return std::ranges::any_of(statusIDs, [statusID](const StatusID existingStatusID) { return statusID == existingStatusID; });
	}

	ATTR_NODISCARD constexpr bool statusReplaceHandler(const StatusID statusID, const StatusRegistry &statusRegistry,
													   std::array<StatusID, Configuration::MAX_STATUSES_PER_POKEMON> &statusIDs)
	{
		bool replacedCurrentStatus{false};

		std::ranges::for_each(statusIDs, [statusID, &statusRegistry, &replacedCurrentStatus](StatusID &existingStatusID) {
			if (hasInteraction(statusID, existingStatusID, StatusInteractionAction::ReplaceCurrent, statusRegistry))
			{
				existingStatusID = replacedCurrentStatus ? NO_STATUS_ID : statusID;
				replacedCurrentStatus = true;
			}
		});

		return replacedCurrentStatus;
	}

	constexpr void statusRemoveHandler(const StatusID statusID, const StatusRegistry &statusRegistry,
									   std::array<StatusID, Configuration::MAX_STATUSES_PER_POKEMON> &statusIDs)
	{
		std::ranges::for_each(statusIDs, [statusID, &statusRegistry](StatusID &existingStatusID) {
			if (hasInteraction(statusID, existingStatusID, StatusInteractionAction::RemoveCurrent, statusRegistry))
			{
				existingStatusID = NO_STATUS_ID;
			}
		});
	}

	ATTR_NODISCARD constexpr std::size_t shiftAndGetNextAvailableStatus(
		std::array<StatusID, Configuration::MAX_STATUSES_PER_POKEMON> &statusIDs)
	{
		std::size_t nextActiveStatusIndex{};

		for (std::size_t index{}; index < statusIDs.size(); index++)
		{
			if (statusIDs.at(index) != NO_STATUS_ID)
			{
				statusIDs.at(nextActiveStatusIndex) = statusIDs.at(index);
				nextActiveStatusIndex++;
			}
		}

		return nextActiveStatusIndex;
	}
} // namespace PocketCore::Status

#endif
