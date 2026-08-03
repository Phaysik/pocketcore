/*! @file fixedMetadataRegistryConfiguration.h
	@brief Provides shared validated lifecycle operations for fixed metadata registries.
	@date 07/27/2026
	@version x.x.x
	@since x.x.x
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
#include "Core/cconcepts.h"
#include "Core/typedefs.h"
#include "Utility/Debug/Logging/logger.h"

namespace PocketCore::Configuration
{
	using PocketCore::Core::InvocableWithArgs;
	using PocketCore::Core::us;
	using PocketCore::Utility::Debug::Logging::Logger;

	/*! @class FixedMetadataRegistryConfiguration Configuration/fixedMetadataRegistryConfiguration.h
		@brief Implements validated lifecycle operations shared by named metadata registry facades.
		@details Owns one registry and centralizes lookup, monotonic ID assignment, duplicate and capacity validation, atomic batch
	   addition, renaming, removal with compaction, and metadata mutation. Domain facades provide user-facing names and construct their
	   metadata.
		@tparam Registry The concrete fixed metadata registry type.
		@tparam Metadata The metadata record type, including an mName member.
		@tparam StableID The registry's strongly typed stable identifier.
		@tparam Capacity The registry's fixed maximum entry count.
		@tparam IDMember A pointer to the StableID member within Metadata.
		@tparam Policy A domain policy exposing configurationName, entityName, duplicateError, and notFoundError constants.
		@date 07/27/2026
		@version x.x.x
		@since x.x.x
		@author Matthew Moore
	*/
	template <typename Registry, typename Metadata, typename StableID, us Capacity, StableID Metadata::*IDMember, typename Policy>
	class FixedMetadataRegistryConfiguration
	{
		protected:
			/*! @brief Constructs a configuration containing the concrete registry's built-in metadata. */
			constexpr FixedMetadataRegistryConfiguration() = default;

			/*! @brief Looks up complete metadata by stable ID.
				@param[in] stableID The built-in or custom stable identifier.
				@return A non-owning pointer to metadata if registered, or nullptr otherwise. The pointer remains valid until replacement or
			   configuration destruction.
			*/
			ATTR_NODISCARD constexpr const Metadata *getMetadata(const StableID stableID) const
			{
				return registry.getMetadata(stableID);
			}

			/*! @brief Looks up a stable ID by display name.
				@param[in] name The case-sensitive display name.
				@return The stable ID if registered, or std::nullopt otherwise.
			*/
			ATTR_NODISCARD constexpr const std::optional<StableID> getID(const std::string_view &name) const
			{
				return registry.getID(name);
			}

			/*! @brief Looks up a display name by stable ID.
				@param[in] stableID The built-in or custom stable identifier.
				@return The display name if registered, or std::nullopt otherwise.
			*/
			ATTR_NODISCARD constexpr const std::optional<std::string_view> getName(const StableID stableID) const
			{
				return registry.getName(stableID);
			}

			/*! @brief Returns all currently registered metadata records.
				@return A read-only span that remains valid until mutation or destruction.
			*/
			ATTR_NODISCARD constexpr const std::span<const Metadata> getRegisteredEntries() const noexcept
			{
				return registry.getRegisteredEntries();
			}

			/*! @brief Returns the number of registered built-in and custom records.
				@return The current registry entry count.
			*/
			ATTR_NODISCARD constexpr us getAmountRegistered() const noexcept
			{
				return registry.getAmountRegistered();
			}

			/*! @brief Checks whether a display name is registered.
				@param[in] name The case-sensitive display name.
				@return True if the name is registered, otherwise false.
			*/
			ATTR_NODISCARD constexpr bool hasEntry(const std::string_view &name) const
			{
				return registry.hasEntry(name);
			}

			/*! @brief Checks whether a stable ID is registered.
				@param[in] stableID The built-in or custom stable identifier.
				@return True if the ID is registered, otherwise false.
			*/
			ATTR_NODISCARD constexpr bool hasEntry(const StableID stableID) const
			{
				return registry.hasEntry(stableID);
			}

			/*! @brief Registers one complete metadata record and assigns its stable ID.
				@param[in] metadata The metadata record with name and domain-specific data populated.
				@return The assigned stable ID on success, or contextual registry error information.
			*/
			ATTR_NODISCARD const std::expected<StableID, RegistryErrorInfo> addMetadata(Metadata metadata)
			{
				const std::string_view name{metadata.mName};

				if (registry.getAmountRegistered() >= Capacity || registry.getNextID() == std::numeric_limits<us>::max())
				{
					const std::optional<std::string_view> logResult{
						Logger::warn("{}::add registry capacity or ID space exhausted. Cannot add {} '{}'.", Policy::configurationName,
									 Policy::entityName, name),
					};

					return std::unexpected{RegistryErrorInfo{RegistryError::MaxCapacity, name, logResult.value_or(std::string_view{})}};
				}

				if (registry.hasEntry(name))
				{
					const std::optional<std::string_view> logResult{
						Logger::warn("{}::add duplicate {} '{}'.", Policy::configurationName, Policy::entityName, name),
					};

					return std::unexpected{RegistryErrorInfo{Policy::duplicateError, name, logResult.value_or(std::string_view{})}};
				}

				const us entryIndex{registry.getAmountRegistered()};
				const StableID assignedID{registry.getNextID()};
				metadata.*IDMember = assignedID;

				registry.setEntry(entryIndex, metadata);
				registry.incrementAmountRegistered();
				registry.incrementNextID();

				return assignedID;
			}

			/*! @brief Registers multiple definitions atomically after converting each to metadata.
				@tparam Definition The domain definition type accepted by the facade.
				@tparam Factory A callable returning Metadata for one Definition.
				@param[in] definitions The definitions to register in order.
				@param[in] factory The eager conversion callable used for each definition.
				@return Void on success, or the first registry error after restoring the prior state.
			*/
			template <typename Definition, typename Factory>
				requires InvocableWithArgs<Factory, const Definition &>
			ATTR_NODISCARD const std::expected<void, RegistryErrorInfo> addMetadataBatch(const std::span<const Definition> &definitions,
																						 Factory &&factory)
			{
				if (definitions.size() > static_cast<std::size_t>(Capacity - registry.getAmountRegistered()))
				{
					const std::optional<std::string_view> logResult{
						Logger::warn("{}::addBatch cannot add {} {} entries: capacity is {}.", Policy::configurationName,
									 definitions.size(), Policy::entityName, Capacity),
					};

					return std::unexpected{RegistryErrorInfo{RegistryError::MaxCapacity, {}, logResult.value_or(std::string_view{})}};
				}

				const us previousAmountRegistered{registry.getAmountRegistered()};
				const us previousNextID{registry.getNextID()};

				const auto forwardedFactory{std::forward<Factory>(factory)};

				for (const Definition &definition : definitions)
				{
					const std::expected<StableID, RegistryErrorInfo> result{
						addMetadata(std::invoke(forwardedFactory, definition)),
					};

					if (!result.has_value())
					{
						while (registry.getAmountRegistered() > previousAmountRegistered)
						{
							registry.eraseEntry(static_cast<us>(registry.getAmountRegistered() - 1U));
						}

						registry.setNextID(previousNextID);
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
			*/
			template <typename Mutator>
				requires InvocableWithArgs<Mutator, Metadata &>
			ATTR_NODISCARD const std::expected<void, RegistryErrorInfo> mutateMetadata(const std::string_view &name,
																					   const std::string_view &callerContext,
																					   Mutator &&mutator)
			{
				const std::expected<us, RegistryErrorInfo> index{resolveIndex(name, callerContext)};

				if (!index.has_value())
				{
					return std::unexpected{index.error()};
				}

				const Metadata &currentMetadata{registry.getEntry(index.value())};
				Metadata metadata{currentMetadata};
				std::invoke(std::forward<Mutator>(mutator), metadata);
				metadata.*IDMember = currentMetadata.*IDMember;

				if (metadata.mName != currentMetadata.mName && registry.hasEntry(metadata.mName))
				{
					const std::optional<std::string_view> logResult{
						Logger::warn("{}::{} target {} name '{}' already exists.", Policy::configurationName, callerContext,
									 Policy::entityName, metadata.mName),
					};

					return std::unexpected{
						RegistryErrorInfo{Policy::duplicateError, metadata.mName, logResult.value_or(std::string_view{})},
					};
				}

				registry.setEntry(index.value(), metadata);

				return {};
			}

			/*! @overload mutateMetadata(StableID, std::string_view, Mutator&&)
				@brief Mutates a copy of registered metadata selected by stable ID and writes it back.
			*/
			template <typename Mutator>
				requires InvocableWithArgs<Mutator, Metadata &>
			ATTR_NODISCARD const std::expected<void, RegistryErrorInfo> mutateMetadata(const StableID stableID,
																					   const std::string_view &callerContext,
																					   Mutator &&mutator)
			{
				const std::expected<us, RegistryErrorInfo> index{resolveIndex(stableID, callerContext)};

				if (!index.has_value())
				{
					return std::unexpected{index.error()};
				}

				const Metadata &currentMetadata{registry.getEntry(index.value())};
				Metadata metadata{currentMetadata};
				std::invoke(std::forward<Mutator>(mutator), metadata);
				metadata.*IDMember = currentMetadata.*IDMember;

				if (metadata.mName != currentMetadata.mName && registry.hasEntry(metadata.mName))
				{
					const std::optional<std::string_view> logResult{
						Logger::warn("{}::{} target {} name '{}' already exists.", Policy::configurationName, callerContext,
									 Policy::entityName, metadata.mName),
					};

					return std::unexpected{
						RegistryErrorInfo{Policy::duplicateError, metadata.mName, logResult.value_or(std::string_view{})},
					};
				}

				registry.setEntry(index.value(), metadata);

				return {};
			}

			/*! @brief Renames a registered metadata record without changing its stable ID.
				@param[in] oldName The current display name.
				@param[in] newName The unique replacement display name.
				@return Void on success, or not-found/duplicate error information.
			*/
			ATTR_NODISCARD const std::expected<void, RegistryErrorInfo> renameMetadata(const std::string_view &oldName,
																					   const std::string_view &newName)
			{
				const std::expected<us, RegistryErrorInfo> index{resolveIndex(oldName, "rename")};

				if (!index.has_value())
				{
					return std::unexpected{index.error()};
				}

				if (registry.hasEntry(newName))
				{
					const std::optional<std::string_view> logResult{
						Logger::warn("{}::rename target {} name '{}' already exists.", Policy::configurationName, Policy::entityName,
									 newName),
					};

					return std::unexpected{RegistryErrorInfo{Policy::duplicateError, newName, logResult.value_or(std::string_view{})}};
				}

				Metadata metadata{registry.getEntry(index.value())};
				metadata.mName = newName;
				registry.setEntry(index.value(), metadata);

				return {};
			}

			/*! @brief Removes a metadata record by display name.
				@param[in] name The registered display name.
				@return The removed stable ID on success, or not-found error information.
			*/
			ATTR_NODISCARD const std::expected<StableID, RegistryErrorInfo> removeMetadata(const std::string_view &name)
			{
				const std::optional<StableID> stableID{registry.getID(name)};

				if (!stableID.has_value())
				{
					return std::unexpected{makeNotFoundError(name, "remove")};
				}

				const std::optional<us> index{registry.findIndexByID(stableID.value())};
				assert(index.has_value());
				removeEntry(index.value());

				return stableID.value();
			}

			/*! @brief Removes a metadata record by stable ID.
				@param[in] stableID The built-in or custom stable identifier.
				@return The removed stable ID on success, or not-found error information.
			*/
			ATTR_NODISCARD const std::expected<StableID, RegistryErrorInfo> removeMetadata(const StableID stableID)
			{
				const std::expected<us, RegistryErrorInfo> index{resolveIndex(stableID, "remove")};

				if (!index.has_value())
				{
					return std::unexpected{index.error()};
				}

				removeEntry(index.value());
				return stableID;
			}

		private:
			ATTR_NODISCARD const std::expected<us, RegistryErrorInfo> resolveIndex(const std::string_view &name,
																				   const std::string_view &callerContext)
			{
				const std::optional<StableID> stableID{registry.getID(name)};

				if (!stableID.has_value())
				{
					return std::unexpected{makeNotFoundError(name, callerContext)};
				}

				// NOLINTNEXTLINE(bugprone-unchecked-optional-access)
				return registry.findIndexByID(stableID.value())
					.value(); // LCOV_EXCL_BR - Cannot fail when getID just succeeded on the same registry
			}

			ATTR_NODISCARD const std::expected<us, RegistryErrorInfo> resolveIndex(const StableID stableID,
																				   const std::string_view &callerContext)
			{
				const std::optional<us> index{registry.findIndexByID(stableID)};

				if (!index.has_value())
				{
					return std::unexpected{makeNotFoundError({}, callerContext)};
				}

				return index.value();
			}

			ATTR_NODISCARD const RegistryErrorInfo makeNotFoundError(const std::string_view &context,
																	 const std::string_view &callerContext) const
			{
				const std::optional<std::string_view> logResult{
					Logger::info("{}::{} {} '{}' not found.", Policy::configurationName, callerContext, Policy::entityName, context),
				};

				return RegistryErrorInfo{Policy::notFoundError, context, logResult.value_or(std::string_view{})};
			}

			void removeEntry(const us index)
			{
				registry.eraseEntry(index);
			}

		private:
			/*! @brief Owns the concrete metadata registry used by the domain facade. */
			Registry registry{};
	};
} // namespace PocketCore::Configuration

#endif
