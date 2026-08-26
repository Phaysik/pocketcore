/*! @file statusRegistry.h
	@brief Provides fixed-capacity storage and lookup for built-in and user-defined statuses.
	@date 07/29/2026
	@since 0.6.4
	@version 0.6.4
	@author Matthew Moore
*/

#ifndef INCLUDE_REGISTRY_STATUS_REGISTRY_H
#define INCLUDE_REGISTRY_STATUS_REGISTRY_H

#include <optional>
#include <span>
#include <string_view>

#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Effect/builtInEffectID.h"
#include "Registry/fixedMetadataRegistry.h"
#include "Status/builtInStatusID.h"
#include "Status/constants.h"
#include "Status/statusID.h"
#include "Status/statusMeta.h"

namespace PocketCore::Registry::Status
{
	using PocketCore::Configuration::MAX_STATUSES;
	using PocketCore::Core::us;
	using PocketCore::Effect::BuiltinEffectID;
	using PocketCore::Registry::FixedMetadataRegistry;
	using PocketCore::Status::BuiltinStatusID;
	using PocketCore::Status::StatusID;
	using PocketCore::Status::StatusInteractionAction;
	using PocketCore::Status::StatusMeta;
	using PocketCore::Status::toStatusID;

	/*! @class StatusRegistry Registry/statusRegistry.h
		@brief Stores built-in and user-defined status metadata in fixed-capacity storage.
		@details Built-in statuses are registered during construction with IDs derived from @ref BuiltinStatusID. Configuration code may
	   append, replace, or remove entries through the low-level mutators while battle-time callers use allocation-free lookup operations.
		@note Lookup operations are O(n), where n is bounded by @ref MAX_STATUSES.
		@date 07/29/2026
		@since 0.6.4
		@version 0.6.4
		@author Matthew Moore
	*/
	class StatusRegistry : private FixedMetadataRegistry<StatusMeta, StatusID, MAX_STATUSES, &StatusMeta::mStatusID>
	{
		private:
			using Base = FixedMetadataRegistry<StatusMeta, StatusID, MAX_STATUSES, &StatusMeta::mStatusID>;

		public:
			// LCOV_EXCL_START - If the built in additions fail, the program wouldn't work anyway
			/*! @brief Constructs a registry populated with every @ref BuiltinStatusID.
				@since 0.6.4
				@version 0.6.4
			 */
			ATTR_NOINLINE explicit constexpr StatusRegistry() : Base{toStatusID(BuiltinStatusID::FinalStatus).getValue()}
			{
				addBuiltin({
					.mName = PocketCore::Status::STATUS_NAME_NONE,
					.mStatusID = toStatusID(BuiltinStatusID::None),
				});
				addBuiltin({
					.mStatusInteractions
					= {{.mExistingStatusID = toStatusID(BuiltinStatusID::Freeze), .mAction = StatusInteractionAction::BlockIncoming}},
					.mName = PocketCore::Status::STATUS_NAME_PARALYSIS,
					.mStatusID = toStatusID(BuiltinStatusID::Paralysis),
				});
				addBuiltin({
					.mStatusInteractions
					= {{.mExistingStatusID = toStatusID(BuiltinStatusID::Freeze), .mAction = StatusInteractionAction::BlockIncoming}},
					.mName = PocketCore::Status::STATUS_NAME_BURN,
					.mStatusID = toStatusID(BuiltinStatusID::Burn),
				});
				addBuiltin({
					.mStatusInteractions
					= {{.mExistingStatusID = toStatusID(BuiltinStatusID::Freeze), .mAction = StatusInteractionAction::BlockIncoming}},
					.mName = PocketCore::Status::STATUS_NAME_SLEEP,
					.mStatusID = toStatusID(BuiltinStatusID::Sleep),
				});
				addBuiltin({
					.mStatusInteractions
					= {{.mExistingStatusID = toStatusID(BuiltinStatusID::Burn), .mAction = StatusInteractionAction::RemoveCurrent},
					   {.mExistingStatusID = toStatusID(BuiltinStatusID::Sleep), .mAction = StatusInteractionAction::RemoveCurrent},
					   {.mExistingStatusID = toStatusID(BuiltinStatusID::Paralysis), .mAction = StatusInteractionAction::RemoveCurrent},
					},
					.mName = PocketCore::Status::STATUS_NAME_FREEZE,
					.mStatusID = toStatusID(BuiltinStatusID::Freeze),
				});
				addBuiltin({
					.mStatusInteractions
					= {{.mExistingStatusID = toStatusID(BuiltinStatusID::Freeze), .mAction = StatusInteractionAction::BlockIncoming}},
					.mName = PocketCore::Status::STATUS_NAME_POISON,
					.mStatusID = toStatusID(BuiltinStatusID::Poison),
				});
				addBuiltin({
					.mStatusInteractions
					= {{.mExistingStatusID = toStatusID(BuiltinStatusID::Freeze), .mAction = StatusInteractionAction::BlockIncoming},
					   {.mExistingStatusID = toStatusID(BuiltinStatusID::Poison), .mAction = StatusInteractionAction::ReplaceCurrent},},
					.mName = PocketCore::Status::STATUS_NAME_TOXIC,
					.mStatusID = toStatusID(BuiltinStatusID::Toxic),
				});
			}

			// LCOV_EXCL_STOP

			using Base::decrementAmountRegistered;

			using Base::eraseEntry;
			using Base::findIndexByID;
			using Base::getAmountRegistered;
			using Base::getEntry;
			using Base::getID;
			using Base::getMetadata;
			using Base::getName;
			using Base::getNextID;
			using Base::getRegisteredEntries;
			using Base::hasEntry;
			using Base::incrementAmountRegistered;
			using Base::incrementNextID;
			using Base::setAmountRegistered;
			using Base::setEntry;
			using Base::setNextID;

			/*! @brief Looks up status metadata by stable ID.
				@param[in] statusID The stable status identifier.
				@return A non-owning pointer to metadata if registered, or nullptr otherwise. The pointer remains valid until replacement or
			   registry destruction.
				@since 0.6.4
				@version 0.6.4
			*/
			ATTR_NODISCARD constexpr const StatusMeta *getStatusMetadata(const StatusID statusID) const
			{
				return getMetadata(statusID);
			}

			/*! @brief Looks up an status ID by display name.
				@param[in] name The case-sensitive display name.
				@return The stable ID if registered, or std::nullopt otherwise.
				@since 0.6.4
				@version 0.6.4
			*/
			ATTR_NODISCARD constexpr const std::optional<StatusID> getStatusID(const std::string_view &name) const
			{
				return getID(name);
			}

			/*! @brief Looks up an status display name by stable ID.
				@param[in] statusID The stable status identifier.
				@return The display name if registered, or std::nullopt otherwise.
				@since 0.6.4
				@version 0.6.4
			*/
			ATTR_NODISCARD constexpr const std::optional<std::string_view> getStatusName(const StatusID statusID) const
			{
				return getName(statusID);
			}

			/*! @brief Returns all currently registered status definitions.
				@return A read-only span that remains valid until the registry is mutated or destroyed.
				@since 0.6.4
				@version 0.6.4
			*/
			ATTR_NODISCARD constexpr const std::span<const StatusMeta> getRegisteredStatuses() const noexcept
			{
				return getRegisteredEntries();
			}

			/*! @brief Returns the next stable ID assigned to a custom status.
				@return The underlying numeric value of the next status ID.
				@since 0.6.4
				@version 0.6.4
			*/
			ATTR_NODISCARD constexpr us getNextStatusID() const noexcept
			{
				return getNextID();
			}

			/*! @brief Finds an internal array index by stable status ID.
				@param[in] statusID The stable status identifier.
				@return The internal index if registered, or std::nullopt otherwise.
				@since 0.6.4
				@version 0.6.4
			*/
			ATTR_NODISCARD constexpr const std::optional<us> findIndexByStatusID(const StatusID statusID) const
			{
				return findIndexByID(statusID);
			}

			/*! @brief Checks whether an status name is registered.
				@param[in] name The case-sensitive display name.
				@return True if the name is registered, otherwise false.
				@since 0.6.4
				@version 0.6.4
			*/
			ATTR_NODISCARD constexpr bool hasStatus(const std::string_view &name) const
			{
				return hasEntry(name);
			}

			/*! @brief Checks whether an status ID is registered.
				@param[in] statusID The stable status identifier.
				@return True if the ID is registered, otherwise false.
				@since 0.6.4
				@version 0.6.4
			*/
			ATTR_NODISCARD constexpr bool hasStatus(const StatusID statusID) const
			{
				return hasEntry(statusID);
			}

			/*! @brief Sets the next custom status ID counter.
				@param[in] nextID The next underlying ID value.
				@since 0.6.4
				@version 0.6.4
			*/
			constexpr void setNextStatusID(const us nextID) noexcept
			{
				setNextID(nextID);
			}

			/*! @brief Increments the next custom status ID counter.
				@since 0.6.4
				@version 0.6.4
			 */
			constexpr void incrementNextStatusID() noexcept
			{
				incrementNextID();
			}
	};
} // namespace PocketCore::Registry::Status

#endif
