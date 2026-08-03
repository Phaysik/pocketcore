/*! @file typeEffectivenessRegistryConfiguration.h
	@brief Declares the user-facing facade for configuring type-effectiveness metadata.
	@date 07/27/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_CONFIGURATION_TYPEEFFECTIVENESS_REGISTRY_CONFIGURATION_H
#define INCLUDE_CONFIGURATION_TYPEEFFECTIVENESS_REGISTRY_CONFIGURATION_H

#include <expected>
#include <optional>
#include <span>
#include <string_view>

#include "Configuration/constants.h"
#include "Configuration/fixedMetadataRegistryConfiguration.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Registry/typeEffectivenessRegistry.h"
#include "Types/typeEffectivenessID.h"
#include "Types/typeEffectivenessMeta.h"

namespace PocketCore::Configuration
{
	using PocketCore::Configuration::TYPE_EFFECTIVENESS_MAX_AMOUNT;
	using PocketCore::Core::us;
	using PocketCore::Registry::Types::TypeEffectivenessRegistry;
	using PocketCore::Types::TypeEffectivenessID;
	using PocketCore::Types::TypeEffectivenessMeta;

	namespace Detail
	{
		struct TypeEffectivenessRegistryConfigurationPolicy
		{
			public:
				static constexpr std::string_view configurationName{"TypeEffectivenessRegistryConfiguration"};
				static constexpr std::string_view entityName{"typeEffectiveness"};
				static constexpr RegistryError duplicateError{RegistryError::DuplicateTypeEffectiveness};
				static constexpr RegistryError notFoundError{RegistryError::TypeEffectivenessNotFound};
		};
	} // namespace Detail

	/*! @class TypeEffectivenessRegistryConfiguration Configuration/typeEffectivenessRegistryConfiguration.h
		@brief Provides validated user customization over an internal type-effectiveness registry.
		@details Supports lookup, addition, batch addition, trigger replacement, renaming, and removal. Custom IDs are assigned
	   monotonically and are not reused after removal. Batch additions provide all-or-nothing semantics.
		@date 07/27/2026
		@version x.x.x
		@since x.x.x
		@author Matthew Moore
	*/
	class TypeEffectivenessRegistryConfiguration
		: private FixedMetadataRegistryConfiguration<TypeEffectivenessRegistry, TypeEffectivenessMeta, TypeEffectivenessID,
													 TYPE_EFFECTIVENESS_MAX_AMOUNT, &TypeEffectivenessMeta::mTypeEffectivenessID,
													 Detail::TypeEffectivenessRegistryConfigurationPolicy>
	{
		private:
			using Base = FixedMetadataRegistryConfiguration<TypeEffectivenessRegistry, TypeEffectivenessMeta, TypeEffectivenessID,
															TYPE_EFFECTIVENESS_MAX_AMOUNT, &TypeEffectivenessMeta::mTypeEffectivenessID,
															Detail::TypeEffectivenessRegistryConfigurationPolicy>;

		public:
			/*! @brief Constructs a configuration containing all built-in type-effectiveness entries. */
			constexpr TypeEffectivenessRegistryConfiguration() = default;

			/*! @brief Looks up complete metadata by stable type-effectiveness ID.
				@param[in] typeEffectivenessID The built-in or custom stable identifier.
				@return A non-owning pointer to metadata if registered, or nullptr otherwise. The pointer remains valid until replacement or
			   configuration destruction.
			*/
			ATTR_NODISCARD constexpr const TypeEffectivenessMeta *getTypeEffectivenessMetadata(
				const TypeEffectivenessID typeEffectivenessID) const
			{
				return getMetadata(typeEffectivenessID);
			}

			/*! @brief Looks up a stable type-effectiveness ID by display name.
				@param[in] name The case-sensitive display name.
				@return The stable ID if registered, or std::nullopt otherwise.
			*/
			ATTR_NODISCARD constexpr const std::optional<TypeEffectivenessID> getTypeEffectivenessID(const std::string_view &name) const
			{
				return getID(name);
			}

			/*! @brief Looks up a display name by stable type-effectiveness ID.
				@param[in] typeEffectivenessID The built-in or custom stable identifier.
				@return The display name if registered, or std::nullopt otherwise.
			*/
			ATTR_NODISCARD constexpr const std::optional<std::string_view> getTypeEffectivenessName(
				const TypeEffectivenessID typeEffectivenessID) const
			{
				return getName(typeEffectivenessID);
			}

			/*! @brief Returns all currently registered type-effectiveness definitions.
				@return A read-only span that remains valid until mutation or destruction.
			*/
			ATTR_NODISCARD constexpr const std::span<const TypeEffectivenessMeta> getRegisteredTypeEffectivenesss() const noexcept
			{
				return getRegisteredEntries();
			}

			/*! @brief Returns the number of registered built-in and custom type-effectiveness entries.
				@return The current registry entry count.
			*/
			ATTR_NODISCARD constexpr us getAmountRegistered() const noexcept
			{
				return Base::getAmountRegistered();
			}

			/*! @brief Checks whether a type-effectiveness name is registered.
				@param[in] name The case-sensitive display name.
				@return True if the name is registered, otherwise false.
			*/
			ATTR_NODISCARD constexpr bool hasTypeEffectiveness(const std::string_view &name) const
			{
				return hasEntry(name);
			}

			/*! @brief Checks whether a type-effectiveness ID is registered.
				@param[in] typeEffectivenessID The built-in or custom stable identifier.
				@return True if the ID is registered, otherwise false.
			*/
			ATTR_NODISCARD constexpr bool hasTypeEffectiveness(const TypeEffectivenessID typeEffectivenessID) const
			{
				return hasEntry(typeEffectivenessID);
			}

			/*! @brief Registers one user-defined type-effectiveness entry and assigns a stable ID.
				@param[in] typeEffectivenessMeta The name and trigger metadata to copy into the registry.
				@return The assigned ID on success, or @ref RegistryErrorInfo on duplicate name or exhausted capacity.
			*/
			ATTR_NODISCARD std::expected<TypeEffectivenessID, RegistryErrorInfo> addTypeEffectiveness(
				const TypeEffectivenessMeta &typeEffectivenessMeta);

			/*! @brief Registers multiple type-effectiveness entries atomically.
				@details Restores the complete prior registry state if any definition fails validation.
				@param[in] typeEffectivenessMetas The typeEffectiveness definitions to register in order.
				@return Void on success, or the first @ref RegistryErrorInfo on failure.
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> addTypeEffectivenesss(
				const std::span<const TypeEffectivenessMeta> &typeEffectivenessMetas);

			/*! @brief Renames a type-effectiveness entry without changing its other metadata.
				@details @p newName is stored as a non-owning view and its backing storage must remain valid while registered.
				@param[in] oldName The currently registered display name.
				@param[in] newName The unique replacement display name.
				@return Void on success, or @ref RegistryErrorInfo if the source is absent or target name is already registered.
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> renameTypeEffectiveness(const std::string_view &oldName,
																						  const std::string_view &newName);

			/*! @brief Replaces all metadata for a type-effectiveness entry selected by display name.
				@param[in] typeEffectivenessName The registered display name.
				@param[in] typeEffectivenessMeta The metadata to copy into the registry.
				@return Void on success, or @ref RegistryErrorInfo if the typeEffectiveness is not registered.
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> updateTypeEffectiveness(
				const std::string_view &typeEffectivenessName, const TypeEffectivenessMeta &typeEffectivenessMeta);

			/*! @overload std::expected<void, RegistryErrorInfo> updateTypeEffectiveness(TypeEffectivenessID, const TypeEffectivenessMeta
			   &typeEffectivenessMeta)
				@brief Replaces all metadata for a type-effectiveness entry selected by stable ID.
				@param[in] typeEffectivenessID The built-in or custom stable identifier.
				@param[in] typeEffectivenessMeta The metadata to copy into the registry.
				@return Void on success, or @ref RegistryErrorInfo if the typeEffectiveness is not registered.
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> updateTypeEffectiveness(
				const TypeEffectivenessID typeEffectivenessID, const TypeEffectivenessMeta &typeEffectivenessMeta);

			/*! @brief Removes a type-effectiveness entry by display name.
				@param[in] typeEffectivenessName The registered display name.
				@return The removed stable ID on success, or @ref RegistryErrorInfo if no matching typeEffectiveness exists.
			*/
			ATTR_NODISCARD std::expected<TypeEffectivenessID, RegistryErrorInfo> removeTypeEffectiveness(
				const std::string_view &typeEffectivenessName);

			/*! @overload std::expected<TypeEffectivenessID, RegistryErrorInfo> removeTypeEffectiveness(TypeEffectivenessID)
				@brief Removes a type-effectiveness entry by stable ID.
				@param[in] typeEffectivenessID The built-in or custom stable identifier.
				@return The removed stable ID on success, or @ref RegistryErrorInfo if no matching typeEffectiveness exists.
			*/
			ATTR_NODISCARD std::expected<TypeEffectivenessID, RegistryErrorInfo> removeTypeEffectiveness(
				const TypeEffectivenessID typeEffectivenessID);
	};
} // namespace PocketCore::Configuration

#endif
