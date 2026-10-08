/*! @file learnsetRegistryConfiguration.h
	@brief Declares the user-facing facade for configuring learnset metadata.
	@date 10/08/2026
	@since 0.12.50
	@version 0.12.50
	@author Matthew Moore
*/

#ifndef INCLUDE_CONFIGURATION_LEARNSET_REGISTRY_CONFIGURATION_H
#define INCLUDE_CONFIGURATION_LEARNSET_REGISTRY_CONFIGURATION_H

#include <expected>
#include <optional>
#include <span>
#include <string_view>

#include "Configuration/constants.h"
#include "Configuration/fixedMetadataRegistryConfiguration.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Learnset/learnsetID.h"
#include "Learnset/learnsetMeta.h"
#include "Registry/learnsetRegistry.h"

namespace PocketCore::Configuration
{
	using PocketCore::Core::us;
	using PocketCore::Learnset::LearnsetID;
	using PocketCore::Learnset::LearnsetMeta;
	using PocketCore::Registry::Learnset::LearnsetRegistry;

	namespace Detail
	{
		/*! @struct LearnsetRegistryConfigurationPolicy Configuration/learnsetRegistryConfiguration.h
			@brief Policy class providing error codes and display strings for learnset registry configuration.
			@details Encapsulates the learnset-specific error categories and display names used by the generic
			 @ref FixedMetadataRegistryConfiguration template to report validation and lookup failures with
			 domain-specific terminology.
			@date 10/08/2026
			@since 0.12.50
			@version 0.12.50
			@author Matthew Moore
		*/
		struct LearnsetRegistryConfigurationPolicy
		{
			public:
				/*! @brief The display name of the configuration system. */
				static constexpr std::string_view configurationName{"LearnsetRegistryConfiguration"};

				/*! @brief The singular entity type managed by this configuration. */
				static constexpr std::string_view entityName{"learnset"};

				/*! @brief The error code returned when a duplicate learnset name is registered. */
				static constexpr RegistryError duplicateError{RegistryError::DuplicateLearnset};

				/*! @brief The error code returned when a learnset lookup fails. */
				static constexpr RegistryError notFoundError{RegistryError::LearnsetNotFound};
		};
	} // namespace Detail

	/*! @class LearnsetRegistryConfiguration Configuration/learnsetRegistryConfiguration.h
		@brief Provides validated user customization over an internal learnset registry.
		@details Supports lookup, addition, batch addition, move learn entry replacement, renaming, and removal. The registry owns copies
	   of registered names and entries. Custom IDs are assigned monotonically and are preserved during updates and renaming, and are not
	   reused after removal. Batch additions provide all-or-nothing semantics for reported validation errors.
		@date 10/08/2026
		@since 0.12.50
		@version 0.12.50
		@author Matthew Moore
	*/
	class LearnsetRegistryConfiguration
		: private FixedMetadataRegistryConfiguration<LearnsetRegistry, LearnsetMeta, LearnsetID, MAX_LEARNSETS, &LearnsetMeta::mLearnsetID,
													 Detail::LearnsetRegistryConfigurationPolicy>
	{
		private:
			using Base = FixedMetadataRegistryConfiguration<LearnsetRegistry, LearnsetMeta, LearnsetID, MAX_LEARNSETS,
															&LearnsetMeta::mLearnsetID, Detail::LearnsetRegistryConfigurationPolicy>;

		public:
			/*! @brief Constructs a configuration containing all built-in learnsets.
				@since 0.12.50
				@version 0.12.50
			 */
			constexpr LearnsetRegistryConfiguration() = default;

			using Base::getAmountRegistered;

			/*! @brief Returns read-only access to the configured runtime learnset registry.
				@return A reference that remains valid for the lifetime of this configuration.
				@since 0.12.50
				@version 0.12.50
			*/
			ATTR_NODISCARD constexpr const LearnsetRegistry &getRuntimeRegistry() const noexcept
			{
				return getRegistry();
			}

			/*! @brief Looks up complete metadata by stable learnset ID.
				@param[in] learnsetID The built-in or custom stable identifier.
				@return A non-owning pointer to registry-owned metadata if registered, or nullptr otherwise. The pointer must not be retained
			   across configuration mutations or destruction, as mutations can replace metadata or compact storage.
				@since 0.12.50
				@version 0.12.50
			*/
			ATTR_NODISCARD constexpr const LearnsetMeta *getLearnsetMetadata(const LearnsetID learnsetID) const
			{
				return getMetadata(learnsetID);
			}

			/*! @brief Looks up a stable learnset ID by display name.
				@param[in] name The case-sensitive display name.
				@return The stable ID if registered, or std::nullopt otherwise.
				@since 0.12.50
				@version 0.12.50
			*/
			ATTR_NODISCARD constexpr const std::optional<LearnsetID> getLearnsetID(const std::string_view &name) const
			{
				return getID(name);
			}

			/*! @brief Looks up a display name by stable learnset ID.
				@param[in] learnsetID The built-in or custom stable identifier.
				@return A non-owning view of the registry-owned display name if registered, or std::nullopt otherwise. The view must not be
			   retained across configuration mutations or destruction.
				@since 0.12.50
				@version 0.12.50
			*/
			ATTR_NODISCARD constexpr const std::optional<std::string_view> getLearnsetName(const LearnsetID learnsetID) const
			{
				return getName(learnsetID);
			}

			/*! @brief Returns all currently registered learnset definitions.
				@return A read-only span that remains valid until mutation or destruction.
				@since 0.12.50
				@version 0.12.50
			*/
			ATTR_NODISCARD constexpr const std::span<const LearnsetMeta> getRegisteredLearnsets() const noexcept
			{
				return getRegisteredEntries();
			}

			/*! @brief Checks whether a learnset name is registered.
				@param[in] name The case-sensitive display name.
				@return True if the name is registered, otherwise false.
				@since 0.12.50
				@version 0.12.50
			*/
			ATTR_NODISCARD constexpr bool hasLearnset(const std::string_view &name) const
			{
				return hasEntry(name);
			}

			/*! @brief Checks whether a learnset ID is registered.
				@param[in] learnsetID The built-in or custom stable identifier.
				@return True if the ID is registered, otherwise false.
				@since 0.12.50
				@version 0.12.50
			*/
			ATTR_NODISCARD constexpr bool hasLearnset(const LearnsetID learnsetID) const
			{
				return hasEntry(learnsetID);
			}

			/*! @brief Registers one user-defined learnset and assigns a stable ID.
				@param[in] learnsetMeta The name and move learn entries to copy into owned registry storage. The supplied ID is ignored,
			   and the source metadata does not need to outlive this call.
				@return The assigned ID on success, or @ref RegistryErrorInfo on duplicate name or exhausted capacity.
				@since 0.12.50
				@version 0.12.50
			*/
			ATTR_NODISCARD std::expected<LearnsetID, RegistryErrorInfo> addLearnset(const LearnsetMeta &learnsetMeta);

			/*! @brief Registers multiple learnsets atomically.
				@details Restores the registered metadata and next stable ID if any definition fails validation.
				@param[in] learnsetMetas The learnset definitions to copy eagerly into owned registry storage in order. Supplied IDs are
			   ignored, and the source span and definitions do not need to outlive this call.
				@return Void on success, or the first @ref RegistryErrorInfo on failure.
				@note Rollback applies to reported validation errors; allocation exceptions are not caught and may leave earlier additions
			   registered.
				@since 0.12.50
				@version 0.12.50
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> addLearnsets(const std::span<const LearnsetMeta> &learnsetMetas);

			/*! @brief Renames a learnset without changing its stable ID or move learn entries.
				@details Copies @p newName into the registry-owned string; its backing storage does not need to outlive this call.
				@param[in] oldName The currently registered display name.
				@param[in] newName The unique replacement display name.
				@return Void on success, or @ref RegistryErrorInfo if the source is absent or target name is already registered.
				@since 0.12.50
				@version 0.12.50
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> renameLearnset(const std::string_view &oldName,
																				 const std::string_view &newName);

			/*! @brief Replaces a learnset's name and move learn entries selected by display name, preserving its stable ID.
				@param[in] learnsetName The registered display name.
				@param[in] learnsetMeta The metadata to copy into owned registry storage. The supplied ID is ignored, and the source metadata
			   does not need to outlive this call.
				@return Void on success, or @ref RegistryErrorInfo if the learnset is not registered or the replacement name is already in use
			   by another learnset.
				@since 0.12.50
				@version 0.12.50
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> updateLearnset(const std::string_view &learnsetName,
																				 const LearnsetMeta &learnsetMeta);

			/*! @overload std::expected<void, RegistryErrorInfo> updateLearnset(LearnsetID, const LearnsetMeta &learnsetMeta)
				@brief Replaces a learnset's name and move learn entries selected by stable ID, preserving that ID.
				@param[in] learnsetID The built-in or custom stable identifier.
				@param[in] learnsetMeta The metadata to copy into owned registry storage. The supplied ID is ignored, and the source metadata
			   does not need to outlive this call.
				@return Void on success, or @ref RegistryErrorInfo if the learnset is not registered or the replacement name is already in use
			   by another learnset.
				@since 0.12.50
				@version 0.12.50
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> updateLearnset(const LearnsetID learnsetID,
																				 const LearnsetMeta &learnsetMeta);

			/*! @brief Removes a learnset by display name.
				@param[in] learnsetName The registered display name.
				@return The removed stable ID on success, or @ref RegistryErrorInfo if no matching learnset exists.
				@since 0.12.50
				@version 0.12.50
			*/
			ATTR_NODISCARD std::expected<LearnsetID, RegistryErrorInfo> removeLearnset(const std::string_view &learnsetName);

			/*! @overload std::expected<LearnsetID, RegistryErrorInfo> removeLearnset(LearnsetID)
				@brief Removes a learnset by stable ID.
				@param[in] learnsetID The built-in or custom stable identifier.
				@return The removed stable ID on success, or @ref RegistryErrorInfo if no matching learnset exists.
				@since 0.12.50
				@version 0.12.50
			*/
			ATTR_NODISCARD std::expected<LearnsetID, RegistryErrorInfo> removeLearnset(const LearnsetID learnsetID);
	};
} // namespace PocketCore::Configuration

#endif
