/*! @file moveRegistryConfiguration.h
	@brief Declares the user-facing facade for configuring move metadata.
	@date 07/27/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_CONFIGURATION_MOVE_REGISTRY_CONFIGURATION_H
#define INCLUDE_CONFIGURATION_MOVE_REGISTRY_CONFIGURATION_H

#include <expected>
#include <optional>
#include <span>
#include <string_view>

#include "Battle/battleTargetsAndTriggers.h"
#include "Configuration/constants.h"
#include "Configuration/fixedMetadataRegistryConfiguration.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Move/moveID.h"
#include "Move/moveMeta.h"
#include "Registry/moveRegistry.h"

namespace PocketCore::Configuration
{
	using PocketCore::Battle::BattleTargetID;
	using PocketCore::Core::us;
	using PocketCore::Move::MoveEffectTrigger;
	using PocketCore::Move::MoveID;
	using PocketCore::Move::MoveMeta;
	using PocketCore::Registry::Move::MoveRegistry;

	namespace Detail
	{
		struct MoveRegistryConfigurationPolicy
		{
			public:
				static constexpr std::string_view configurationName{"MoveRegistryConfiguration"};
				static constexpr std::string_view entityName{"move"};
				static constexpr RegistryError duplicateError{RegistryError::DuplicateMove};
				static constexpr RegistryError notFoundError{RegistryError::MoveNotFound};
		};
	} // namespace Detail

	/*! @class MoveRegistryConfiguration Configuration/moveRegistryConfiguration.h
		@brief Provides validated user customization over an internal move registry.
		@details Supports lookup, addition, batch addition, trigger replacement, renaming, and removal. Custom IDs are assigned
	   monotonically and are not reused after removal. Batch additions provide all-or-nothing semantics.
		@date 07/27/2026
		@version x.x.x
		@since x.x.x
		@author Matthew Moore
	*/
	class MoveRegistryConfiguration
		: private FixedMetadataRegistryConfiguration<MoveRegistry, MoveMeta, MoveID, MAX_MOVES, &MoveMeta::mMoveID,
													 Detail::MoveRegistryConfigurationPolicy>
	{
		private:
			using Base = FixedMetadataRegistryConfiguration<MoveRegistry, MoveMeta, MoveID, MAX_MOVES, &MoveMeta::mMoveID,
															Detail::MoveRegistryConfigurationPolicy>;

		public:
			/*! @brief Constructs a configuration containing all built-in moves. */
			constexpr MoveRegistryConfiguration() = default;

			/*! @brief Looks up complete metadata by stable move ID.
				@param[in] moveID The built-in or custom stable identifier.
				@return A non-owning pointer to metadata if registered, or nullptr otherwise. The pointer remains valid until replacement or
			   configuration destruction.
			*/
			ATTR_NODISCARD constexpr const MoveMeta *getMoveMetadata(const MoveID moveID) const
			{
				return getMetadata(moveID);
			}

			/*! @brief Looks up a stable move ID by display name.
				@param[in] name The case-sensitive display name.
				@return The stable ID if registered, or std::nullopt otherwise.
			*/
			ATTR_NODISCARD constexpr const std::optional<MoveID> getMoveID(const std::string_view &name) const
			{
				return getID(name);
			}

			/*! @brief Looks up a display name by stable move ID.
				@param[in] moveID The built-in or custom stable identifier.
				@return The display name if registered, or std::nullopt otherwise.
			*/
			ATTR_NODISCARD constexpr const std::optional<std::string_view> getMoveName(const MoveID moveID) const
			{
				return getName(moveID);
			}

			/*! @brief Returns all currently registered move definitions.
				@return A read-only span that remains valid until mutation or destruction.
			*/
			ATTR_NODISCARD constexpr const std::span<const MoveMeta> getRegisteredMoves() const noexcept
			{
				return getRegisteredEntries();
			}

			/*! @brief Returns the number of registered built-in and custom moves.
				@return The current registry entry count.
			*/
			ATTR_NODISCARD constexpr us getAmountRegistered() const noexcept
			{
				return Base::getAmountRegistered();
			}

			/*! @brief Checks whether an move name is registered.
				@param[in] name The case-sensitive display name.
				@return True if the name is registered, otherwise false.
			*/
			ATTR_NODISCARD constexpr bool hasMove(const std::string_view &name) const
			{
				return hasEntry(name);
			}

			/*! @brief Checks whether an move ID is registered.
				@param[in] moveID The built-in or custom stable identifier.
				@return True if the ID is registered, otherwise false.
			*/
			ATTR_NODISCARD constexpr bool hasMove(const MoveID moveID) const
			{
				return hasEntry(moveID);
			}

			/*! @brief Registers one user-defined move and assigns a stable ID.
				@param[in] moveMeta The name and trigger metadata to copy into the registry.
				@return The assigned ID on success, or @ref RegistryErrorInfo on duplicate name or exhausted capacity.
			*/
			ATTR_NODISCARD std::expected<MoveID, RegistryErrorInfo> addMove(const MoveMeta &moveMeta);

			/*! @brief Registers multiple moves atomically.
				@details Restores the complete prior registry state if any definition fails validation.
				@param[in] moveMetas The move definitions to register in order.
				@return Void on success, or the first @ref RegistryErrorInfo on failure.
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> addMoves(const std::span<const MoveMeta> &moveMetas);

			/*! @brief Replaces all trigger metadata for an move selected by name.
				@param[in] moveName The registered display name.
				@param[in] triggers The trigger definitions to copy into the registry.
				@return Void on success, or @ref RegistryErrorInfo if the move is not registered.
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> setMoveTriggers(const std::string_view &moveName,
																				  const std::span<const MoveEffectTrigger> &triggers);

			/*! @overload std::expected<void, RegistryErrorInfo> setMoveTriggers(MoveID, std::span<const MoveEffectTrigger>)
				@brief Replaces all trigger metadata for an move selected by stable ID.
				@param[in] moveID The built-in or custom stable identifier.
				@param[in] triggers The trigger definitions to copy into the registry.
				@return Void on success, or @ref RegistryErrorInfo if the move is not registered.
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> setMoveTriggers(const MoveID moveID,
																				  const std::span<const MoveEffectTrigger> &triggers);

			/*! @brief Replaces all trigger metadata for an move selected by name.
				@param[in] moveName The registered display name.
				@param[in] target The target to copy into the registry.
				@return Void on success, or @ref RegistryErrorInfo if the move is not registered.
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> setMoveTarget(const std::string_view &moveName,
																				const BattleTargetID target);

			/*! @overload std::expected<void, RegistryErrorInfo> setMoveTarget(MoveID, const BattleTargetID target)
				@brief Replaces all trigger metadata for an move selected by stable ID.
				@param[in] moveID The built-in or custom stable identifier.
				@param[in] target The target to copy into the registry.
				@return Void on success, or @ref RegistryErrorInfo if the move is not registered.
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> setMoveTarget(const MoveID moveID, const BattleTargetID target);

			/*! @brief Renames an move without changing its other metadata.
				@details @p newName is stored as a non-owning view and its backing storage must remain valid while registered.
				@param[in] oldName The currently registered display name.
				@param[in] newName The unique replacement display name.
				@return Void on success, or @ref RegistryErrorInfo if the source is absent or target name is already registered.
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> renameMove(const std::string_view &oldName,
																			 const std::string_view &newName);

			/*! @brief Replaces all move metadata for an move selected by stable ID.
				@param[in] moveName The registered display name.
				@param[in] moveMeta The metadata to copy into the registry.
				@return Void on success, or @ref RegistryErrorInfo if the move is not registered.
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> updateMove(const std::string_view &moveName, const MoveMeta &moveMeta);

			/*! @overload std::expected<void, RegistryErrorInfo> updateMove(MoveID, const MoveMeta &moveMeta)
				@brief Replaces all move metadata for an move selected by stable ID.
				@param[in] moveID The built-in or custom stable identifier.
				@param[in] moveMeta The metadata to copy into the registry.
				@return Void on success, or @ref RegistryErrorInfo if the move is not registered.
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> updateMove(const MoveID moveID, const MoveMeta &moveMeta);

			/*! @brief Removes an move by display name.
				@param[in] moveName The registered display name.
				@return The removed stable ID on success, or @ref RegistryErrorInfo if no matching move exists.
			*/
			ATTR_NODISCARD std::expected<MoveID, RegistryErrorInfo> removeMove(const std::string_view &moveName);

			/*! @overload std::expected<MoveID, RegistryErrorInfo> removeMove(MoveID)
				@brief Removes an move by stable ID.
				@param[in] moveID The built-in or custom stable identifier.
				@return The removed stable ID on success, or @ref RegistryErrorInfo if no matching move exists.
			*/
			ATTR_NODISCARD std::expected<MoveID, RegistryErrorInfo> removeMove(const MoveID moveID);
	};
} // namespace PocketCore::Configuration

#endif
