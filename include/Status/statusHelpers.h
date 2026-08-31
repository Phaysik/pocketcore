/*! @file statusHelpers.h
	@brief Defines identifiers for status aides.
	@date 08/28/2026
	@since 0.9.11
	@version 0.12.9
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

	/*! @brief Determines whether a status declares a specific interaction with an existing status.
		@param[in] statusID The incoming status whose metadata is inspected.
		@param[in] existingStatusID The status already present on the Pokemon.
		@param[in] action The interaction action to search for.
		@param[in] statusRegistry The registry used to resolve the incoming status metadata.
		@return `true` when the incoming status declares @p action against @p existingStatusID; otherwise, `false`.
		@note Returns `false` when the incoming status has no registered metadata.
		@since 0.9.11
		@version 0.12.9
	*/
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

	/*! @brief Determines whether any existing status prevents an incoming status from being applied.
		@param[in] statusID The incoming status to test.
		@param[in] statusRegistry The registry used to resolve the incoming status metadata.
		@param[in] statusIDs The Pokemon's current statuses, including empty slots.
		@return `true` when the incoming status declares a blocking interaction against any current status; otherwise, `false`.
		@since 0.9.11
		@version 0.12.9
	*/
	ATTR_NODISCARD constexpr bool willBlockIncoming(const StatusID statusID, const StatusRegistry &statusRegistry,
													const std::array<StatusID, Configuration::MAX_STATUSES_PER_POKEMON> &statusIDs)
	{
		return std::ranges::any_of(statusIDs, [statusID, &statusRegistry](const StatusID existingStatusID) {
			return hasInteraction(statusID, existingStatusID, StatusInteractionAction::BlockIncoming, statusRegistry);
		});
	}

	/*! @brief Determines whether a status is already present on a Pokemon.
		@param[in] statusID The status to search for.
		@param[in] statusIDs The Pokemon's current statuses, including empty slots.
		@return `true` when @p statusID occupies one of the slots; otherwise, `false`.
		@since 0.9.11
		@version 0.12.9
	*/
	ATTR_NODISCARD constexpr bool statusAlreadyExists(const StatusID statusID,
													  const std::array<StatusID, Configuration::MAX_STATUSES_PER_POKEMON> &statusIDs)
	{
		return std::ranges::any_of(statusIDs, [statusID](const StatusID existingStatusID) { return statusID == existingStatusID; });
	}

	/*! @brief Replaces every status that the incoming status supersedes.
		@param[in] statusID The incoming status to write into the first replaced slot.
		@param[in] statusRegistry The registry used to resolve the incoming status metadata.
		@param[in,out] statusIDs The Pokemon's current statuses, updated in place.
		@return `true` when at least one status was replaced; otherwise, `false`.
		@post The first superseded slot holds @p statusID and any further superseded slots are cleared, which prevents duplicates.
		@since 0.9.11
		@version 0.12.9
	*/
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

	/*! @brief Clears every status that the incoming status removes.
		@param[in] statusID The incoming status whose removal interactions are applied.
		@param[in] statusRegistry The registry used to resolve the incoming status metadata.
		@param[in,out] statusIDs The Pokemon's current statuses, updated in place.
		@post Each removed slot holds the empty status identifier, leaving gaps that require compaction.
		@note @ref shiftAndGetNextAvailableStatus compacts the resulting gaps.
		@since 0.9.11
		@version 0.12.9
	*/
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

	/*! @brief Compacts active statuses to the front of the array and reports the first free slot.
		@param[in,out] statusIDs The Pokemon's current statuses, reordered in place.
		@return The index of the first free slot, which equals the number of active statuses.
		@post Active statuses occupy the leading slots in their original relative order.
		@warning Slots at or after the returned index retain stale values and must be overwritten before use.
		@note Runs in linear time over the fixed status capacity.
		@since 0.9.11
		@version 0.12.9
	*/
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
