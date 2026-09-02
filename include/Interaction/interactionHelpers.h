/*! @file interactionHelpers.h
	@brief Defines reusable algorithms for applying metadata interactions.
	@date 09/02/2026
	@since 0.12.16
	@version 0.12.16
	@author Matthew Moore
*/

#ifndef INCLUDE_CORE_INTERACTION_HELPERS_H
#define INCLUDE_CORE_INTERACTION_HELPERS_H

#include <algorithm>
#include <cstddef>
#include <ranges>

#include "Core/attributeMacros.h"

#include "interaction.h"

namespace PocketCore::Interaction
{
	/*! @brief Determines whether an interaction range declares an action against an existing object.
		@tparam ID The stable identifier type shared by the interacting objects.
		@tparam InteractionRange The input range containing @ref Interaction objects.
		@param[in] existingID The identifier of the object that is already active.
		@param[in] action The interaction action to search for.
		@param[in] interactions The incoming object's interaction declarations.
		@return `true` when @p interactions declares @p action against @p existingID; otherwise, `false`.
		@note Runs in O(n) time and O(1) additional space, where n is the number of interactions.
		@since 0.12.16
		@version 0.12.16
	*/
	template <typename ID, std::ranges::input_range InteractionRange>
	ATTR_NODISCARD constexpr bool hasInteraction(const ID existingID, const InteractionAction action, const InteractionRange &interactions)
	{
		return std::ranges::any_of(interactions, [existingID, action](const auto &interaction) {
			return interaction.mExistingID == existingID && interaction.mAction == action;
		});
	}

	/*! @brief Determines whether any active object blocks an incoming object.
		@tparam IDRange The input range containing active object identifiers.
		@tparam InteractionRange The input range containing @ref Interaction objects.
		@param[in] existingIDs The identifiers of the objects that are already active.
		@param[in] interactions The incoming object's interaction declarations.
		@return `true` when a blocking interaction targets any identifier in @p existingIDs; otherwise, `false`.
		@note Runs in O(n * m) time and O(1) additional space, where n is the number of active identifiers and m is the number of
	interactions.
		@since 0.12.16
		@version 0.12.16
	*/
	template <std::ranges::input_range IDRange, std::ranges::input_range InteractionRange>
	ATTR_NODISCARD constexpr bool willBlockIncoming(const IDRange &existingIDs, const InteractionRange &interactions)
	{
		return std::ranges::any_of(existingIDs, [&interactions](const auto existingID) {
			return hasInteraction(existingID, InteractionAction::BlockIncoming, interactions);
		});
	}

	/*! @brief Replaces active objects targeted by an incoming object's replacement interactions.
		@tparam ID The stable identifier type shared by the interacting objects.
		@tparam IDRange The mutable forward range containing active object identifiers.
		@tparam InteractionRange The input range containing @ref Interaction objects.
		@param[in] incomingID The incoming identifier written into the first replaced position.
		@param[in] emptyID The identifier used to clear further replaced positions.
		@param[in,out] existingIDs The active identifiers updated in place.
		@param[in] interactions The incoming object's interaction declarations.
		@return `true` when at least one active object was replaced; otherwise, `false`.
		@post The first replaced position contains @p incomingID and further replaced positions contain @p emptyID.
		@since 0.12.16
		@version 0.12.16
	*/
	template <typename ID, std::ranges::forward_range IDRange, std::ranges::input_range InteractionRange>
	ATTR_NODISCARD constexpr bool replaceCurrent(const ID incomingID, const ID emptyID, IDRange &existingIDs,
												 const InteractionRange &interactions)
	{
		bool replacedCurrent{false};

		std::ranges::for_each(existingIDs, [incomingID, emptyID, &interactions, &replacedCurrent](auto &existingID) {
			if (hasInteraction(existingID, InteractionAction::ReplaceCurrent, interactions))
			{
				existingID = replacedCurrent ? emptyID : incomingID;
				replacedCurrent = true;
			}
		});

		return replacedCurrent;
	}

	/*! @brief Clears active objects targeted by an incoming object's removal interactions.
		@tparam ID The stable identifier type shared by the interacting objects.
		@tparam IDRange The mutable forward range containing active object identifiers.
		@tparam InteractionRange The input range containing @ref Interaction objects.
		@param[in] emptyID The identifier used to clear removed positions.
		@param[in,out] existingIDs The active identifiers updated in place.
		@param[in] interactions The incoming object's interaction declarations.
		@post Every position targeted by a removal interaction contains @p emptyID.
		@since 0.12.16
		@version 0.12.16
	*/
	template <typename ID, std::ranges::forward_range IDRange, std::ranges::input_range InteractionRange>
	constexpr void removeCurrent(const ID emptyID, IDRange &existingIDs, const InteractionRange &interactions)
	{
		std::ranges::for_each(existingIDs, [emptyID, &interactions](auto &existingID) {
			if (hasInteraction(existingID, InteractionAction::RemoveCurrent, interactions))
			{
				existingID = emptyID;
			}
		});
	}

	/*! @brief Compacts non-empty identifiers to the front of a mutable range.
		@tparam ID The stable identifier type stored by the range.
		@tparam IDRange The mutable forward range containing object identifiers.
		@param[in] emptyID The identifier representing an unused position.
		@param[in,out] existingIDs The identifiers compacted in place.
		@return The number of non-empty identifiers, which is also the offset of the first free position.
		@post Non-empty identifiers occupy the leading positions in their original relative order and remaining positions contain @p emptyID.
		@note Runs in O(n) time and O(1) additional space, where n is the size of @p existingIDs.
		@since 0.12.16
		@version 0.12.16
	*/
	template <typename ID, std::ranges::forward_range IDRange>
	ATTR_NODISCARD constexpr std::size_t shiftAndGetNextAvailable(const ID emptyID, IDRange &existingIDs)
	{
		auto emptyIdentifiers{std::ranges::remove(existingIDs, emptyID)};
		const std::size_t activeCount{static_cast<std::size_t>(std::ranges::distance(std::ranges::begin(existingIDs), emptyIdentifiers.begin()))};
		std::ranges::fill(emptyIdentifiers, emptyID);

		return activeCount;
	}

	/*! @brief Applies an incoming identifier according to its registered interactions with active identifiers.
		@tparam ID The stable identifier type shared by the interacting objects.
		@tparam IDRange The mutable forward range containing active object identifiers.
		@tparam Registry The registry type that resolves metadata through `getMetadata`.
		@tparam Metadata The metadata type containing the interaction range.
		@tparam InteractionRange The range type stored by the metadata interaction member.
		@param[in] incomingID The registered identifier to apply.
		@param[in] emptyID The identifier representing an unused position.
		@param[in] registry The registry used to resolve metadata for @p incomingID.
		@param[in,out] existingIDs The active identifiers updated in place.
		@param[in] interactionsMember Member pointer selecting the metadata's interaction range.
		@post Blocking interactions leave @p existingIDs unchanged. Replacement and removal interactions are applied before insertion.
		@note An unregistered non-empty identifier is inserted without applying interactions, preserving the behavior of the framework
	   adapters.
		@since 0.12.16
		@version 0.12.16
	*/
	template <typename ID, std::ranges::forward_range IDRange, typename Registry, typename Metadata,
			  std::ranges::input_range InteractionRange>
	constexpr void applyInteractions(const ID incomingID, const ID emptyID, const Registry &registry, IDRange &existingIDs,
									 InteractionRange Metadata::*interactionsMember)
	{
		if (incomingID == emptyID || std::ranges::contains(existingIDs, incomingID))
		{
			return;
		}

		const Metadata *metadata{registry.getMetadata(incomingID)};
		bool replacedCurrent{false};

		if (metadata != nullptr)
		{
			const InteractionRange &interactions{metadata->*interactionsMember};

			if (willBlockIncoming(existingIDs, interactions))
			{
				return;
			}

			replacedCurrent = replaceCurrent(incomingID, emptyID, existingIDs, interactions);
			removeCurrent(emptyID, existingIDs, interactions);
		}

		const std::size_t nextAvailableIndex{shiftAndGetNextAvailable(emptyID, existingIDs)};

		if (!replacedCurrent && nextAvailableIndex < static_cast<std::size_t>(std::ranges::distance(existingIDs)))
		{
			const auto nextAvailable{std::ranges::find(existingIDs, emptyID)};
			*nextAvailable = incomingID;
		}
	}
} // namespace PocketCore::Interaction

#endif
