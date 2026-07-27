/*! @file abilityRegistryConfiguration.h
	@brief Declares the user-facing facade for configuring ability metadata.
	@date 07/27/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_CONFIGURATION_ABILITY_REGISTRY_CONFIGURATION_H
#define INCLUDE_CONFIGURATION_ABILITY_REGISTRY_CONFIGURATION_H

#include <expected>
#include <functional>
#include <optional>
#include <span>
#include <string_view>

#include "Ability/abilityID.h"
#include "Ability/abilityMeta.h"
#include "Ability/abilityTargetsAndTriggers.h"
#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Registry/abilityRegistry.h"

namespace PocketCore::Configuration
{
	using PocketCore::Ability::AbilityEffectTrigger;
	using PocketCore::Ability::AbilityID;
	using PocketCore::Ability::AbilityMeta;
	using PocketCore::Core::us;

	/*! @struct AbilityDefinition Configuration/abilityRegistryConfiguration.h
		@brief Describes a user-defined ability before the registry assigns its stable ID.
		@details The trigger span is copied into registry-owned metadata during registration. The name is a non-owning view and its backing
	   storage must outlive the configuration object or remain valid until the ability is removed.
		@date 07/27/2026
		@version x.x.x
		@since x.x.x
		@author Matthew Moore
	*/
	struct AbilityDefinition
	{
			/*! @brief The unique, case-sensitive display name with storage that outlives the registered ability. */
			std::string_view name{};

			/*! @brief The trigger and effect definitions copied during registration. */
			std::span<const AbilityEffectTrigger> triggers{};
	};

	/*! @class AbilityRegistryConfiguration Configuration/abilityRegistryConfiguration.h
		@brief Provides validated user customization over an internal ability registry.
		@details Supports lookup, addition, batch addition, trigger replacement, renaming, and removal. Custom IDs are assigned
	   monotonically and are not reused after removal. Batch additions provide all-or-nothing semantics.
		@date 07/27/2026
		@version x.x.x
		@since x.x.x
		@author Matthew Moore
	*/
	class AbilityRegistryConfiguration
	{
		public:
			/*! @brief Constructs a configuration containing all built-in abilities. */
			constexpr AbilityRegistryConfiguration() = default;

			/*! @brief Looks up complete metadata by stable ability ID.
				@param[in] abilityID The built-in or custom stable identifier.
				@return A reference to metadata if registered, or std::nullopt otherwise. The reference remains valid until mutation or
			   destruction.
			*/
			ATTR_NODISCARD constexpr std::optional<std::reference_wrapper<const AbilityMeta>> getAbilityMetadata(
				const AbilityID abilityID) const
			{
				return registry.getAbilityMetadata(abilityID);
			}

			/*! @brief Looks up a stable ability ID by display name.
				@param[in] name The case-sensitive display name.
				@return The stable ID if registered, or std::nullopt otherwise.
			*/
			ATTR_NODISCARD constexpr std::optional<AbilityID> getAbilityID(const std::string_view name) const
			{
				return registry.getAbilityID(name);
			}

			/*! @brief Looks up a display name by stable ability ID.
				@param[in] abilityID The built-in or custom stable identifier.
				@return The display name if registered, or std::nullopt otherwise.
			*/
			ATTR_NODISCARD constexpr std::optional<std::string_view> getAbilityName(const AbilityID abilityID) const
			{
				return registry.getAbilityName(abilityID);
			}

			/*! @brief Returns all currently registered ability definitions.
				@return A read-only span that remains valid until mutation or destruction.
			*/
			ATTR_NODISCARD constexpr std::span<const AbilityMeta> getRegisteredAbilities() const noexcept
			{
				return registry.getRegisteredAbilities();
			}

			/*! @brief Returns the number of registered built-in and custom abilities.
				@return The current registry entry count.
			*/
			ATTR_NODISCARD constexpr us getAmountRegistered() const noexcept
			{
				return registry.getAmountRegistered();
			}

			/*! @brief Checks whether an ability name is registered.
				@param[in] name The case-sensitive display name.
				@return True if the name is registered, otherwise false.
			*/
			ATTR_NODISCARD constexpr bool hasAbility(const std::string_view name) const
			{
				return registry.hasAbility(name);
			}

			/*! @brief Checks whether an ability ID is registered.
				@param[in] abilityID The built-in or custom stable identifier.
				@return True if the ID is registered, otherwise false.
			*/
			ATTR_NODISCARD constexpr bool hasAbility(const AbilityID abilityID) const
			{
				return registry.hasAbility(abilityID);
			}

			/*! @brief Registers one user-defined ability and assigns a stable ID.
				@param[in] definition The name and trigger metadata to copy into the registry.
				@return The assigned ID on success, or @ref RegistryErrorInfo on duplicate name or exhausted capacity.
			*/
			ATTR_NODISCARD std::expected<AbilityID, RegistryErrorInfo> addAbility(const AbilityDefinition &definition);

			/*! @brief Registers multiple abilities atomically.
				@details Restores the complete prior registry state if any definition fails validation.
				@param[in] definitions The ability definitions to register in order.
				@return Void on success, or the first @ref RegistryErrorInfo on failure.
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> addAbilities(std::span<const AbilityDefinition> definitions);

			/*! @brief Replaces all trigger metadata for an ability selected by name.
				@param[in] abilityName The registered display name.
				@param[in] triggers The trigger definitions to copy into the registry.
				@return Void on success, or @ref RegistryErrorInfo if the ability is not registered.
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> setAbilityTriggers(std::string_view abilityName,
																					 std::span<const AbilityEffectTrigger> triggers);

			/*! @overload std::expected<void, RegistryErrorInfo> setAbilityTriggers(AbilityID, std::span<const AbilityEffectTrigger>)
				@brief Replaces all trigger metadata for an ability selected by stable ID.
				@param[in] abilityID The built-in or custom stable identifier.
				@param[in] triggers The trigger definitions to copy into the registry.
				@return Void on success, or @ref RegistryErrorInfo if the ability is not registered.
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> setAbilityTriggers(AbilityID abilityID,
																					 std::span<const AbilityEffectTrigger> triggers);

			/*! @brief Renames an ability without changing its stable ID or trigger metadata.
				@details @p newName is stored as a non-owning view and its backing storage must remain valid while registered.
				@param[in] oldName The currently registered display name.
				@param[in] newName The unique replacement display name.
				@return Void on success, or @ref RegistryErrorInfo if the source is absent or target name is already registered.
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> renameAbility(std::string_view oldName, std::string_view newName);

			/*! @brief Removes an ability by display name.
				@param[in] abilityName The registered display name.
				@return The removed stable ID on success, or @ref RegistryErrorInfo if no matching ability exists.
			*/
			ATTR_NODISCARD std::expected<AbilityID, RegistryErrorInfo> removeAbility(std::string_view abilityName);

			/*! @overload std::expected<AbilityID, RegistryErrorInfo> removeAbility(AbilityID)
				@brief Removes an ability by stable ID.
				@param[in] abilityID The built-in or custom stable identifier.
				@return The removed stable ID on success, or @ref RegistryErrorInfo if no matching ability exists.
			*/
			ATTR_NODISCARD std::expected<AbilityID, RegistryErrorInfo> removeAbility(AbilityID abilityID);

		private:
			/*! @brief Resolves a registered name to its internal array index.
				@param[in] name The display name to resolve.
				@param[in] callerContext The calling operation used in diagnostics.
				@return The internal index on success, or @ref RegistryErrorInfo if absent.
			*/
			ATTR_NODISCARD std::expected<us, RegistryErrorInfo> resolveIndex(std::string_view name, std::string_view callerContext);

			/*! @brief Resolves a registered stable ID to its internal array index.
				@param[in] abilityID The stable ID to resolve.
				@param[in] callerContext The calling operation used in diagnostics.
				@return The internal index on success, or @ref RegistryErrorInfo if absent.
			*/
			ATTR_NODISCARD std::expected<us, RegistryErrorInfo> resolveIndex(AbilityID abilityID, std::string_view callerContext);

			/*! @brief Removes the entry at an already validated internal index.
				@param[in] index The internal array index to erase and compact.
			*/
			void removeEntry(us index);

		private:
			/*! @brief Owns built-in and user-defined ability metadata behind the facade. */
			Registry::Ability::AbilityRegistry registry{};
	};
} // namespace PocketCore::Configuration

#endif