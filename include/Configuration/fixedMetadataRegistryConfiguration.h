/*! @file fixedMetadataRegistryConfiguration.h
	@brief Provides shared validated lifecycle operations for fixed metadata registries.
	@date 10/14/2026
	@since 0.5.0
	@version 0.12.51
	@author Matthew Moore
*/

#ifndef INCLUDE_CONFIGURATION_FIXED_METADATA_REGISTRY_CONFIGURATION_H
#define INCLUDE_CONFIGURATION_FIXED_METADATA_REGISTRY_CONFIGURATION_H

#include <cstddef>
#include <expected>
#include <functional>
#include <limits>
#include <optional>
#include <span>
#include <string_view>
#include <utility>

#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Core/concepts.h"
#include "Core/typedefs.h"
#include "Utility/Debug/Logging/logger.h"
#include "Validation/Registry/registryError.h"

namespace PocketCore::Configuration
{
	using PocketCore::Core::InvocableWithArgs;
	using PocketCore::Core::us;
	using PocketCore::Utility::Debug::Logging::Logger;
	using PocketCore::Validation::Registry::RegistryError;

	namespace Detail
	{
		/*! @brief Returns an independent copy of a metadata record.
			@tparam Metadata The metadata record type to copy.
			@param[in] metadata The registered metadata record to copy.
			@return A copy that can be mutated without affecting the stored entry.
			@note Inlining is suppressed so the copy remains observable to sanitizers and coverage instrumentation.
			@since 0.9.8
			@version 0.12.9
		*/
		template <typename Metadata>
		ATTR_NOINLINE constexpr Metadata cloneMetadata(const Metadata &metadata)
		{
			return metadata;
		}
	} // namespace Detail

	/*! @class FixedMetadataRegistryConfiguration Configuration/fixedMetadataRegistryConfiguration.h
		@brief Implements validated lifecycle operations shared by named metadata registry facades.
		@details Inherits one registry and centralizes lookup, monotonic ID assignment, duplicate and capacity validation, atomic batch
	   addition, renaming, removal with compaction, and metadata mutation. Domain facades provide user-facing names and construct their
	   metadata.
		@tparam Registry The concrete fixed metadata registry type.
		@tparam Metadata The metadata record type, including an mName member.
		@tparam StableID The registry's strongly typed stable identifier.
		@tparam Capacity The registry's fixed maximum entry count.
		@tparam IDMember A pointer to the StableID member within Metadata.
		@tparam Policy A domain policy exposing configurationName, entityName, duplicateError, and notFoundError constants.
		@date 10/13/2026
		@since 0.5.0
		@version 0.12.51
		@author Matthew Moore
	*/
	template <typename Registry, typename Metadata, typename StableID, us Capacity, StableID Metadata::*IDMember, typename Policy>
	class FixedMetadataRegistryConfiguration : protected Registry
	{
		protected:
			/*! @brief Constructs a configuration containing the concrete registry's built-in metadata.
				@since 0.5.0
				@version 0.5.0
			 */
			constexpr FixedMetadataRegistryConfiguration() = default;

		protected:
			using Registry::addEntry;
			using Registry::createCheckpoint;
			using Registry::eraseEntry;
			using Registry::findIndexByID;
			using Registry::getAmountRegistered;
			using Registry::getEntry;
			using Registry::getNextID;
			using Registry::restoreCheckpoint;
			using Registry::setEntry;

			/*! @brief Returns mutable access to the inherited concrete registry.
				@return A mutable reference to the underlying registry instance.
				@since 0.9.0
				@version 0.12.18
			*/
			ATTR_NODISCARD constexpr Registry &getRegistry() noexcept
			{
				return *this;
			}

			/*! @brief Returns read-only access to the inherited concrete registry.
				@return A const reference to the underlying registry instance.
				@since 0.9.0
				@version 0.12.18
			*/
			ATTR_NODISCARD constexpr const Registry &getRegistry() const noexcept
			{
				return *this;
			}

			/*! @brief Looks up complete metadata by stable ID.
				@param[in] stableID The built-in or custom stable identifier.
				@return A non-owning pointer to metadata if registered, or nullptr otherwise. The pointer remains valid until replacement or
			   configuration destruction.
				@since 0.5.0
				@version 0.12.18
			*/
			ATTR_NODISCARD constexpr const Metadata *getMetadata(const StableID stableID) const
			{
				return getRegistry().getMetadata(stableID);
			}

			/*! @brief Looks up a stable ID by display name.
				@param[in] name The case-sensitive display name.
				@return The stable ID if registered, or std::nullopt otherwise.
				@since 0.5.0
				@version 0.12.51
			*/
			ATTR_NODISCARD constexpr const std::optional<StableID> getID(const std::string_view name) const
			{
				return getRegistry().getID(name);
			}

			/*! @brief Looks up a display name by stable ID.
				@param[in] stableID The built-in or custom stable identifier.
				@return The display name if registered, or std::nullopt otherwise.
				@since 0.5.0
				@version 0.12.18
			*/
			ATTR_NODISCARD constexpr const std::optional<std::string_view> getName(const StableID stableID) const
			{
				return getRegistry().getName(stableID);
			}

			/*! @brief Returns all currently registered metadata records.
				@return A read-only span that remains valid until mutation or destruction.
				@since 0.5.0
				@version 0.12.18
			*/
			ATTR_NODISCARD constexpr const std::span<const Metadata> getRegisteredEntries() const noexcept
			{
				return getRegistry().getRegisteredEntries();
			}

			/*! @brief Returns the number of registered built-in and custom records.
				@return The current registry entry count.
				@since 0.5.0
				@version 0.12.18
			*/
			ATTR_NODISCARD constexpr us getAmountRegistered() const noexcept
			{
				return getRegistry().getAmountRegistered();
			}

			/*! @brief Checks whether a display name is registered.
				@param[in] name The case-sensitive display name.
				@return True if the name is registered, otherwise false.
				@since 0.5.0
				@version 0.12.51
			*/
			ATTR_NODISCARD constexpr bool hasEntry(const std::string_view name) const
			{
				return getRegistry().hasEntry(name);
			}

			/*! @brief Checks whether a stable ID is registered.
				@param[in] stableID The built-in or custom stable identifier.
				@return True if the ID is registered, otherwise false.
				@since 0.5.0
				@version 0.12.18
			*/
			ATTR_NODISCARD constexpr bool hasEntry(const StableID stableID) const
			{
				return getRegistry().hasEntry(stableID);
			}

			/*! @brief Registers one complete metadata record and assigns its stable ID.
				@param[in] metadata The metadata record with name and domain-specific data populated.
				@return The assigned stable ID on success, or contextual registry error information.
				@since 0.5.0
				@version 0.12.51
			*/
			ATTR_NODISCARD const std::expected<StableID, RegistryError> addMetadata(Metadata metadata)
			{
				const std::string_view name{metadata.mName};

				if (getAmountRegistered() >= Capacity || getNextID() == std::numeric_limits<us>::max())
				{
					const std::optional<std::string_view> logResult{
						Logger::warn("{}::add registry capacity or ID space exhausted. Cannot add {} '{}'.", Policy::configurationName,
									 Policy::entityName, name),
					};

					return std::unexpected{RegistryError::MaxCapacity};
				}

				if (hasEntry(name))
				{
					const std::optional<std::string_view> logResult{
						Logger::warn("{}::add duplicate {} '{}'.", Policy::configurationName, Policy::entityName, name),
					};

					return std::unexpected{Policy::duplicateError};
				}

				return addEntry(std::move(metadata));
			}

			/*! @brief Registers multiple definitions atomically after converting each to metadata.
				@tparam Definition The domain definition type accepted by the facade.
				@tparam Factory A callable returning Metadata for one Definition.
				@param[in] definitions The definitions to register in order.
				@param[in] factory The eager conversion callable used for each definition.
				@return Void on success, or the first registry error after restoring the prior state.
				@since 0.5.0
				@version 0.12.51
			*/
			template <typename Definition, typename Factory>
				requires InvocableWithArgs<Factory &, const Definition &>
			ATTR_NODISCARD const std::expected<void, RegistryError> addMetadataBatch(const std::span<const Definition> definitions,
																					 Factory &&factory)
			{
				if (definitions.size() > static_cast<std::size_t>(Capacity - getAmountRegistered()))
				{
					const std::optional<std::string_view> logResult{
						Logger::warn("{}::addBatch cannot add {} {} entries: capacity is {}.", Policy::configurationName,
									 definitions.size(), Policy::entityName, Capacity),
					};

					return std::unexpected{RegistryError::MaxCapacity};
				}

				const auto checkpoint{createCheckpoint()};

				Factory &&forwardedFactory{std::forward<Factory>(factory)};

				for (const Definition &definition : definitions)
				{
					const std::expected<StableID, RegistryError> result{
						addMetadata(std::invoke(forwardedFactory, definition)),
					};

					if (!result.has_value())
					{
						restoreCheckpoint(checkpoint);
						return std::unexpected{result.error()};
					}
				}

				return {}; // LCOV_EXCL_LINE
			}

			/*! @brief Mutates a copy of registered metadata selected by name and writes it back.
				@tparam Mutator A callable accepting Metadata&.
				@param[in] name The registered display name.
				@param[in] callerContext The domain operation name used in diagnostics.
				@param[in] mutator The eager mutation callable.
				@return Void on success, or not-found error information.
				@since 0.5.0
				@version 0.12.51
			*/
			template <typename Mutator>
				requires InvocableWithArgs<Mutator &, Metadata &>
			ATTR_NODISCARD const std::expected<void, RegistryError> mutateMetadata(const std::string_view name,
																				   const std::string_view callerContext, Mutator &&mutator)
			{
				const std::expected<us, RegistryError> index{resolveIndex(name)};

				if (!index.has_value())
				{
					return std::unexpected{index.error()};
				}

				const Metadata *currentMetadata{getEntry(index.value())};

				assert(currentMetadata != nullptr);

				Metadata metadata{Detail::cloneMetadata(*currentMetadata)};
				std::invoke(std::forward<Mutator>(mutator), metadata);
				metadata.*IDMember = currentMetadata->*IDMember;

				if (metadata.mName != currentMetadata->mName && hasEntry(metadata.mName))
				{
					const std::optional<std::string_view> logResult{
						Logger::warn("{}::{} target {} name '{}' already exists.", Policy::configurationName, callerContext,
									 Policy::entityName, metadata.mName),
					};

					return std::unexpected{Policy::duplicateError};
				}

				setEntry(index.value(), metadata);

				return {};
			}

			/*! @overload mutateMetadata(StableID, std::string_view, Mutator&&)
				@brief Mutates a copy of registered metadata selected by stable ID and writes it back.
				@since 0.5.0
				@version 0.12.51
			*/
			template <typename Mutator>
				requires InvocableWithArgs<Mutator &, Metadata &>
			ATTR_NODISCARD const std::expected<void, RegistryError> mutateMetadata(const StableID stableID,
																				   const std::string_view callerContext, Mutator &&mutator)
			{
				const std::expected<us, RegistryError> index{resolveIndex(stableID)};

				if (!index.has_value())
				{
					return std::unexpected{index.error()};
				}

				const Metadata *currentMetadata{getEntry(index.value())};

				assert(currentMetadata != nullptr);

				Metadata metadata{Detail::cloneMetadata(*currentMetadata)};
				std::invoke(std::forward<Mutator>(mutator), metadata);
				metadata.*IDMember = currentMetadata->*IDMember;

				if (metadata.mName != currentMetadata->mName && hasEntry(metadata.mName))
				{
					const std::optional<std::string_view> logResult{
						Logger::warn("{}::{} target {} name '{}' already exists.", Policy::configurationName, callerContext,
									 Policy::entityName, metadata.mName),
					};

					return std::unexpected{Policy::duplicateError};
				}

				setEntry(index.value(), metadata);

				return {};
			}

			/*! @brief Renames a registered metadata record without changing its stable ID.
				@param[in] oldName The current display name.
				@param[in] newName The unique replacement display name.
				@return Void on success, or not-found/duplicate error information.
				@since 0.5.0
				@version 0.12.51
			*/
			ATTR_NODISCARD const std::expected<void, RegistryError> renameMetadata(const std::string_view oldName,
																				   const std::string_view newName)
			{
				const std::expected<us, RegistryError> index{resolveIndex(oldName)};

				if (!index.has_value())
				{
					return std::unexpected{index.error()};
				}

				if (hasEntry(newName))
				{
					const std::optional<std::string_view> logResult{
						Logger::warn("{}::rename target {} name '{}' already exists.", Policy::configurationName, Policy::entityName,
									 newName),
					};

					return std::unexpected{Policy::duplicateError};
				}

				const Metadata *currentMetadata{getEntry(index.value())};

				assert(currentMetadata != nullptr);

				Metadata metadata{Detail::cloneMetadata(*currentMetadata)};
				metadata.mName = newName;
				setEntry(index.value(), metadata);

				return {};
			}

			/*! @brief Removes a metadata record by display name.
				@param[in] name The registered display name.
				@return The removed stable ID on success, or not-found error information.
				@since 0.5.0
				@version 0.12.51
			*/
			ATTR_NODISCARD const std::expected<StableID, RegistryError> removeMetadata(const std::string_view name)
			{
				const std::optional<StableID> stableID{getID(name)};

				if (!stableID.has_value())
				{
					return std::unexpected{Policy::notFoundError};
				}

				const std::optional<us> index{findIndexByID(stableID.value())}; // LCOV_EXCL_BR - Can't fail when getID succeeded
				assert(index.has_value());
				removeEntry(index.value());

				return stableID.value();
			}

			/*! @brief Removes a metadata record by stable ID.
				@param[in] stableID The built-in or custom stable identifier.
				@return The removed stable ID on success, or not-found error information.
				@since 0.5.0
				@version 0.12.51
			*/
			ATTR_NODISCARD const std::expected<StableID, RegistryError> removeMetadata(const StableID stableID)
			{
				const std::expected<us, RegistryError> index{resolveIndex(stableID)};

				if (!index.has_value())
				{
					return std::unexpected{index.error()};
				}

				removeEntry(index.value());
				return stableID;
			}

		private:
			/*! @brief Resolves a display name to a registry index after lookup and validation.
				@details Queries the registry for the stable ID associated with the given name, then resolves that ID to its array index.
				Logs and reports not-found errors with domain-specific messages.
				@param[in] name The case-sensitive display name to resolve.
				@return The 0-based registry index on success, or a contextual registry error if name is not registered.
				@note This function is exception-safe (noexcept); errors are communicated via std::expected.
				@since 0.5.0
				@version 0.12.51
			*/
			ATTR_NODISCARD const std::expected<us, RegistryError> resolveIndex(const std::string_view name)
			{
				const std::optional<StableID> stableID{getID(name)};

				if (!stableID.has_value())
				{
					return std::unexpected{Policy::notFoundError};
				}

				assert(stableID.has_value());

				// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
				return findIndexByID(*stableID).value(); // LCOV_EXCL_BR - Can't fail when getID succeeded
			}

			/*! @brief Resolves a stable ID to its registry index after validation.
				@details Queries the registry for the array index of the entry with the given stable ID. Logs and reports not-found errors
				with domain-specific messages when the ID is not registered.
				@param[in] stableID The built-in or custom stable identifier to resolve.
				@return The 0-based registry index on success, or a contextual registry error if the stable ID is not registered.
				@since 0.5.0
				@version 0.12.51
			*/
			ATTR_NODISCARD const std::expected<us, RegistryError> resolveIndex(const StableID stableID)
			{
				const std::optional<us> index{findIndexByID(stableID)}; // LCOV_EXCL_BR (from possible .at() throw which can't happen)

				if (!index.has_value())
				{
					return std::unexpected{Policy::notFoundError};
				}

				assert(index.has_value());

				return *index;
			}

			/*! @brief Removes the metadata entry at the specified registry index.
				@param[in] index The valid 0-based index of the entry to remove.
				@pre The index must refer to a currently registered entry.
				@post The registry removes the entry and compacts any remaining entries as defined by @ref Registry::eraseEntry.
				@note The underlying registry remains responsible for maintaining its entry count and index consistency.
				@since 0.5.0
				@version 0.12.51
			*/
			void removeEntry(const us index)
			{
				eraseEntry(index);
			}
	};
} // namespace PocketCore::Configuration

#endif
