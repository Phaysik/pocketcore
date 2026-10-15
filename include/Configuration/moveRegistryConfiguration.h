/*! @file moveRegistryConfiguration.h
	@brief Declares the user-facing facade for configuring move metadata.
	@date 10/13/2026
	@since 0.6.0
	@version 0.12.51
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
#include "Effect/effectTrigger.h"
#include "Move/moveID.h"
#include "Move/moveMeta.h"
#include "Registry/moveRegistry.h"
#include "Validation/Registry/registryError.h"

namespace PocketCore::Configuration
{
	using PocketCore::Battle::BattleTargetID;
	using PocketCore::Core::us;
	using PocketCore::Effect::EffectTrigger;
	using PocketCore::Move::MoveID;
	using PocketCore::Move::MoveMeta;
	using PocketCore::Registry::Move::MoveRegistry;
	using PocketCore::Validation::Registry::RegistryError;

	namespace Detail
	{
		/*! @struct MoveRegistryConfigurationPolicy Configuration/moveRegistryConfiguration.h
			@brief Policy class providing error codes and display strings for move registry configuration.
			@details Encapsulates the move-specific error categories and display names used by the generic
			 @ref FixedMetadataRegistryConfiguration template to report validation and lookup failures with
			 domain-specific terminology.
			@date 10/09/2026
			@since 0.6.0
			@version 0.12.51
			@author Matthew Moore
		*/
		struct MoveRegistryConfigurationPolicy
		{
			public:
				/*! @brief The display name of the configuration system. */
				static constexpr std::string_view configurationName{"MoveRegistryConfiguration"};

				/*! @brief The singular entity type managed by this configuration. */
				static constexpr std::string_view entityName{"move"};

				/*! @brief The error code returned when a duplicate move name is registered. */
				static constexpr RegistryError duplicateError{RegistryError::DuplicateMove};

				/*! @brief The error code returned when a move lookup fails. */
				static constexpr RegistryError notFoundError{RegistryError::MoveNotFound};
		};
	} // namespace Detail

	/*! @class MoveRegistryConfiguration Configuration/moveRegistryConfiguration.h
		@brief Provides validated user customization over an internal move registry.
		@details Supports lookup, addition, batch addition, trigger replacement, renaming, and removal. Custom IDs are assigned
	   monotonically and are not reused after removal. Batch additions provide all-or-nothing semantics.
		@date 10/13/2026
		@since 0.6.0
		@version 0.12.51
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
			/*! @brief Constructs a configuration containing all built-in moves.
				@since 0.6.0
				@version 0.6.0
			 */
			constexpr MoveRegistryConfiguration() = default;

			using Base::getAmountRegistered;

			/*! @brief Returns read-only access to the configured runtime move registry.
				@return A reference that remains valid for the lifetime of this configuration.
				@since 0.10.3
				@version 0.12.8
			*/
			ATTR_NODISCARD constexpr const MoveRegistry &getRuntimeRegistry() const noexcept
			{
				return getRegistry();
			}

			/*! @brief Looks up complete metadata by stable move ID.
				@param[in] moveID The built-in or custom stable identifier.
				@return A non-owning pointer to metadata if registered, or nullptr otherwise. The pointer remains valid until replacement or
			   configuration destruction.
				@since 0.6.0
				@version 0.6.0
			*/
			ATTR_NODISCARD constexpr const MoveMeta *getMoveMetadata(const MoveID moveID) const
			{
				return getMetadata(moveID);
			}

			/*! @brief Looks up a stable move ID by display name.
				@param[in] name The case-sensitive display name.
				@return The stable ID if registered, or std::nullopt otherwise.
				@since 0.6.0
				@version 0.12.51
			*/
			ATTR_NODISCARD constexpr const std::optional<MoveID> getMoveID(const std::string_view name) const
			{
				return getID(name);
			}

			/*! @brief Looks up a display name by stable move ID.
				@param[in] moveID The built-in or custom stable identifier.
				@return The display name if registered, or std::nullopt otherwise.
				@since 0.6.0
				@version 0.6.0
			*/
			ATTR_NODISCARD constexpr const std::optional<std::string_view> getMoveName(const MoveID moveID) const
			{
				return getName(moveID);
			}

			/*! @brief Returns all currently registered move definitions.
				@return A read-only span that remains valid until mutation or destruction.
				@since 0.6.0
				@version 0.6.0
			*/
			ATTR_NODISCARD constexpr const std::span<const MoveMeta> getRegisteredMoves() const noexcept
			{
				return getRegisteredEntries();
			}

			/*! @brief Checks whether an move name is registered.
				@param[in] name The case-sensitive display name.
				@return True if the name is registered, otherwise false.
				@since 0.6.0
				@version 0.12.51
			*/
			ATTR_NODISCARD constexpr bool hasMove(const std::string_view name) const
			{
				return hasEntry(name);
			}

			/*! @brief Checks whether an move ID is registered.
				@param[in] moveID The built-in or custom stable identifier.
				@return True if the ID is registered, otherwise false.
				@since 0.6.0
				@version 0.6.0
			*/
			ATTR_NODISCARD constexpr bool hasMove(const MoveID moveID) const
			{
				return hasEntry(moveID);
			}

			/*! @brief Registers one user-defined move and assigns a stable ID.
				@param[in] moveMeta The name and trigger metadata to copy into the registry.
				@return The assigned ID on success, or @ref RegistryError on duplicate name or exhausted capacity.
				@since 0.6.0
				@version 0.12.51
			*/
			ATTR_NODISCARD std::expected<MoveID, RegistryError> addMove(const MoveMeta &moveMeta);

			/*! @brief Registers multiple moves atomically.
				@details Restores the complete prior registry state if any definition fails validation.
				@param[in] moveMetas The move definitions to register in order.
				@return Void on success, or the first @ref RegistryError on failure.
				@since 0.6.0
				@version 0.12.51
			*/
			ATTR_NODISCARD std::expected<void, RegistryError> addMoves(const std::span<const MoveMeta> moveMetas);

			/*! @brief Replaces all trigger metadata for an move selected by name.
				@param[in] moveName The registered display name.
				@param[in] triggers The trigger definitions to copy into the registry.
				@return Void on success, or @ref RegistryError if the move is not registered.
				@since 0.6.0
				@version 0.12.51
			*/
			ATTR_NODISCARD std::expected<void, RegistryError> setMoveTriggers(const std::string_view moveName,
																			  const std::span<const EffectTrigger> triggers);

			/*! @overload std::expected<void, RegistryError> setMoveTriggers(MoveID, std::span<const EffectTrigger>)
				@brief Replaces all trigger metadata for an move selected by stable ID.
				@param[in] moveID The built-in or custom stable identifier.
				@param[in] triggers The trigger definitions to copy into the registry.
				@return Void on success, or @ref RegistryError if the move is not registered.
				@since 0.6.0
				@version 0.12.51
			*/
			ATTR_NODISCARD std::expected<void, RegistryError> setMoveTriggers(const MoveID moveID,
																			  const std::span<const EffectTrigger> triggers);

			/*! @brief Replaces all trigger metadata for an move selected by name.
				@param[in] moveName The registered display name.
				@param[in] target The target to copy into the registry.
				@return Void on success, or @ref RegistryError if the move is not registered.
				@since 0.6.0
				@version 0.12.51
			*/
			ATTR_NODISCARD std::expected<void, RegistryError> setMoveTarget(const std::string_view moveName, const BattleTargetID target);

			/*! @overload std::expected<void, RegistryError> setMoveTarget(MoveID, const BattleTargetID target)
				@brief Replaces all trigger metadata for an move selected by stable ID.
				@param[in] moveID The built-in or custom stable identifier.
				@param[in] target The target to copy into the registry.
				@return Void on success, or @ref RegistryError if the move is not registered.
				@since 0.6.0
				@version 0.12.51
			*/
			ATTR_NODISCARD std::expected<void, RegistryError> setMoveTarget(const MoveID moveID, const BattleTargetID target);

			/*! @brief Renames an move without changing its other metadata.
				@details @p newName is stored as a non-owning view and its backing storage must remain valid while registered.
				@param[in] oldName The currently registered display name.
				@param[in] newName The unique replacement display name.
				@return Void on success, or @ref RegistryError if the source is absent or target name is already registered.
				@since 0.6.0
				@version 0.12.51
			*/
			ATTR_NODISCARD std::expected<void, RegistryError> renameMove(const std::string_view oldName, const std::string_view newName);

			/*! @brief Replaces all move metadata for an move selected by stable ID.
				@param[in] moveName The registered display name.
				@param[in] moveMeta The metadata to copy into the registry.
				@return Void on success, or @ref RegistryError if the move is not registered.
				@since 0.6.0
				@version 0.12.51
			*/
			ATTR_NODISCARD std::expected<void, RegistryError> updateMove(const std::string_view moveName, const MoveMeta &moveMeta);

			/*! @overload std::expected<void, RegistryError> updateMove(MoveID, const MoveMeta &moveMeta)
				@brief Replaces all move metadata for an move selected by stable ID.
				@param[in] moveID The built-in or custom stable identifier.
				@param[in] moveMeta The metadata to copy into the registry.
				@return Void on success, or @ref RegistryError if the move is not registered.
				@since 0.6.0
				@version 0.12.51
			*/
			ATTR_NODISCARD std::expected<void, RegistryError> updateMove(const MoveID moveID, const MoveMeta &moveMeta);

			/*! @brief Removes an move by display name.
				@param[in] moveName The registered display name.
				@return The removed stable ID on success, or @ref RegistryError if no matching move exists.
				@since 0.6.0
				@version 0.12.51
			*/
			ATTR_NODISCARD std::expected<MoveID, RegistryError> removeMove(const std::string_view moveName);

			/*! @overload std::expected<MoveID, RegistryError> removeMove(MoveID)
				@brief Removes an move by stable ID.
				@param[in] moveID The built-in or custom stable identifier.
				@return The removed stable ID on success, or @ref RegistryError if no matching move exists.
				@since 0.6.0
				@version 0.12.51
			*/
			ATTR_NODISCARD std::expected<MoveID, RegistryError> removeMove(const MoveID moveID);
	};
} // namespace PocketCore::Configuration

#endif
