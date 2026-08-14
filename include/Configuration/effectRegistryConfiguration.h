/*! @file effectRegistryConfiguration.h
	@brief Declares the user-facing facade for configuring effect metadata.
	@date 07/27/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_CONFIGURATION_EFFECT_REGISTRY_CONFIGURATION_H
#define INCLUDE_CONFIGURATION_EFFECT_REGISTRY_CONFIGURATION_H

#include <expected>
#include <optional>
#include <span>
#include <string_view>

#include "Configuration/constants.h"
#include "Configuration/fixedMetadataRegistryConfiguration.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Effect/effectID.h"
#include "Effect/effectMeta.h"
#include "Registry/effectRegistry.h"

namespace PocketCore::Configuration
{
	using PocketCore::Core::us;
	using PocketCore::Effect::EffectID;
	using PocketCore::Effect::EffectMeta;
	using PocketCore::Registry::Effect::EffectRegistry;

	namespace Detail
	{
		struct EffectRegistryConfigurationPolicy
		{
			public:
				static constexpr std::string_view configurationName{"EffectRegistryConfiguration"};
				static constexpr std::string_view entityName{"effect"};
				static constexpr RegistryError duplicateError{RegistryError::DuplicateEffect};
				static constexpr RegistryError notFoundError{RegistryError::EffectNotFound};
		};
	} // namespace Detail

	/*! @class EffectRegistryConfiguration Configuration/effectRegistryConfiguration.h
		@brief Provides validated user customization over an internal effect registry.
		@details Supports lookup, addition, batch addition, trigger replacement, renaming, and removal. Custom IDs are assigned
	   monotonically and are not reused after removal. Batch additions provide all-or-nothing semantics.
		@date 07/27/2026
		@version x.x.x
		@since x.x.x
		@author Matthew Moore
	*/
	class EffectRegistryConfiguration
		: private FixedMetadataRegistryConfiguration<EffectRegistry, EffectMeta, EffectID, MAX_EFFECTS, &EffectMeta::mEffectID,
													 Detail::EffectRegistryConfigurationPolicy>
	{
		private:
			using Base = FixedMetadataRegistryConfiguration<EffectRegistry, EffectMeta, EffectID, MAX_EFFECTS, &EffectMeta::mEffectID,
															Detail::EffectRegistryConfigurationPolicy>;

		public:
			/*! @brief Constructs a configuration containing all built-in effects. */
			constexpr EffectRegistryConfiguration() = default;

			/*! @brief Returns read-only access to the configured runtime effect registry.
				@return A reference that remains valid for the lifetime of this configuration.
			*/
			ATTR_NODISCARD constexpr const EffectRegistry &getRuntimeRegistry() const noexcept
			{
				return Base::getRegistry();
			}

			/*! @brief Looks up complete metadata by stable effect ID.
				@param[in] effectID The built-in or custom stable identifier.
				@return A non-owning pointer to metadata if registered, or nullptr otherwise. The pointer remains valid until replacement or
			   configuration destruction.
			*/
			ATTR_NODISCARD constexpr const EffectMeta *getEffectMetadata(const EffectID effectID) const
			{
				return getMetadata(effectID);
			}

			/*! @brief Looks up a stable effect ID by display name.
				@param[in] name The case-sensitive display name.
				@return The stable ID if registered, or std::nullopt otherwise.
			*/
			ATTR_NODISCARD constexpr const std::optional<EffectID> getEffectID(const std::string_view &name) const
			{
				return getID(name);
			}

			/*! @brief Looks up a display name by stable effect ID.
				@param[in] effectID The built-in or custom stable identifier.
				@return The display name if registered, or std::nullopt otherwise.
			*/
			ATTR_NODISCARD constexpr const std::optional<std::string_view> getEffectName(const EffectID effectID) const
			{
				return getName(effectID);
			}

			/*! @brief Returns all currently registered effect definitions.
				@return A read-only span that remains valid until mutation or destruction.
			*/
			ATTR_NODISCARD constexpr const std::span<const EffectMeta> getRegisteredEffects() const noexcept
			{
				return getRegisteredEntries();
			}

			/*! @brief Returns the number of registered built-in and custom effects.
				@return The current registry entry count.
			*/
			ATTR_NODISCARD constexpr us getAmountRegistered() const noexcept
			{
				return Base::getAmountRegistered();
			}

			/*! @brief Checks whether an effect name is registered.
				@param[in] name The case-sensitive display name.
				@return True if the name is registered, otherwise false.
			*/
			ATTR_NODISCARD constexpr bool hasEffect(const std::string_view &name) const
			{
				return hasEntry(name);
			}

			/*! @brief Checks whether an effect ID is registered.
				@param[in] effectID The built-in or custom stable identifier.
				@return True if the ID is registered, otherwise false.
			*/
			ATTR_NODISCARD constexpr bool hasEffect(const EffectID effectID) const
			{
				return hasEntry(effectID);
			}

			/*! @brief Registers one user-defined effect and assigns a stable ID.
				@param[in] effectMeta The name and trigger metadata to copy into the registry.
				@return The assigned ID on success, or @ref RegistryErrorInfo on duplicate name or exhausted capacity.
			*/
			ATTR_NODISCARD std::expected<EffectID, RegistryErrorInfo> addEffect(const EffectMeta &effectMeta);

			/*! @brief Registers multiple effects atomically.
				@details Restores the complete prior registry state if any definition fails validation.
				@param[in] effectMetas The effect definitions to register in order.
				@return Void on success, or the first @ref RegistryErrorInfo on failure.
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> addEffects(const std::span<const EffectMeta> &effectMetas);

			/*! @brief Renames an effect without changing its other metadata.
				@details @p newName is stored as a non-owning view and its backing storage must remain valid while registered.
				@param[in] oldName The currently registered display name.
				@param[in] newName The unique replacement display name.
				@return Void on success, or @ref RegistryErrorInfo if the source is absent or target name is already registered.
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> renameEffect(const std::string_view &oldName,
																			   const std::string_view &newName);

			/*! @brief Replaces all effect metadata for an effect selected by stable ID.
				@param[in] effectName The registered display name.
				@param[in] effectMeta The metadata to copy into the registry.
				@return Void on success, or @ref RegistryErrorInfo if the effect is not registered.
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> updateEffect(const std::string_view &effectName,
																			   const EffectMeta &effectMeta);

			/*! @overload std::expected<void, RegistryErrorInfo> updateEffect(EffectID, const EffectMeta &effectMeta)
				@brief Replaces all effect metadata for an effect selected by stable ID.
				@param[in] effectID The built-in or custom stable identifier.
				@param[in] effectMeta The metadata to copy into the registry.
				@return Void on success, or @ref RegistryErrorInfo if the effect is not registered.
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> updateEffect(const EffectID effectID, const EffectMeta &effectMeta);

			/*! @brief Removes an effect by display name.
				@param[in] effectName The registered display name.
				@return The removed stable ID on success, or @ref RegistryErrorInfo if no matching effect exists.
			*/
			ATTR_NODISCARD std::expected<EffectID, RegistryErrorInfo> removeEffect(const std::string_view &effectName);

			/*! @overload std::expected<EffectID, RegistryErrorInfo> removeEffect(EffectID)
				@brief Removes an effect by stable ID.
				@param[in] effectID The built-in or custom stable identifier.
				@return The removed stable ID on success, or @ref RegistryErrorInfo if no matching effect exists.
			*/
			ATTR_NODISCARD std::expected<EffectID, RegistryErrorInfo> removeEffect(const EffectID effectID);
	};
} // namespace PocketCore::Configuration

#endif
