/*! @file pokemonRegistryConfiguration.h
	@brief Declares the user-facing facade for configuring pokemon metadata.
	@date 07/27/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_CONFIGURATION_POKEMON_REGISTRY_CONFIGURATION_H
#define INCLUDE_CONFIGURATION_POKEMON_REGISTRY_CONFIGURATION_H

#include <expected>
#include <optional>
#include <span>
#include <string_view>

#include "Configuration/constants.h"
#include "Configuration/fixedMetadataRegistryConfiguration.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Pokemon/pokemonID.h"
#include "Pokemon/pokemonMeta.h"
#include "Registry/pokemonRegistry.h"

namespace PocketCore::Configuration
{
	using PocketCore::Configuration::MAX_POKEMON;
	using PocketCore::Core::us;
	using PocketCore::Pokemon::PokemonID;
	using PocketCore::Pokemon::PokemonMeta;
	using PocketCore::Registry::Pokemon::PokemonRegistry;

	namespace Detail
	{
		struct PokemonRegistryConfigurationPolicy
		{
			public:
				static constexpr std::string_view configurationName{"PokemonRegistryConfiguration"};
				static constexpr std::string_view entityName{"pokemon"};
				static constexpr RegistryError duplicateError{RegistryError::DuplicatePokemon};
				static constexpr RegistryError notFoundError{RegistryError::PokemonNotFound};
		};
	} // namespace Detail

	/*! @class PokemonRegistryConfiguration Configuration/pokemonRegistryConfiguration.h
		@brief Provides validated user customization over an internal pokemon registry.
		@details Supports lookup, addition, batch addition, trigger replacement, renaming, and removal. Custom IDs are assigned
	   monotonically and are not reused after removal. Batch additions provide all-or-nothing semantics.
		@date 07/27/2026
		@version x.x.x
		@since x.x.x
		@author Matthew Moore
	*/
	class PokemonRegistryConfiguration
		: private FixedMetadataRegistryConfiguration<PokemonRegistry, PokemonMeta, PokemonID, MAX_POKEMON, &PokemonMeta::mPokemonID,
													 Detail::PokemonRegistryConfigurationPolicy>
	{
		private:
			using Base = FixedMetadataRegistryConfiguration<PokemonRegistry, PokemonMeta, PokemonID, MAX_POKEMON, &PokemonMeta::mPokemonID,
															Detail::PokemonRegistryConfigurationPolicy>;

		public:
			/*! @brief Constructs a configuration containing all built-in pokemons. */
			constexpr PokemonRegistryConfiguration() = default;

			/*! @brief Returns read-only access to the configured runtime pokemon registry.
				@return A reference that remains valid for the lifetime of this configuration.
			*/
			ATTR_NODISCARD constexpr const PokemonRegistry &getRuntimeRegistry() const noexcept
			{
				return Base::getRegistry();
			}

			/*! @brief Looks up complete metadata by stable pokemon ID.
				@param[in] pokemonID The built-in or custom stable identifier.
				@return A non-owning pointer to metadata if registered, or nullptr otherwise. The pointer remains valid until replacement or
			   configuration destruction.
			*/
			ATTR_NODISCARD constexpr const PokemonMeta *getPokemonMetadata(const PokemonID pokemonID) const
			{
				return getMetadata(pokemonID);
			}

			/*! @brief Looks up a stable pokemon ID by display name.
				@param[in] name The case-sensitive display name.
				@return The stable ID if registered, or std::nullopt otherwise.
			*/
			ATTR_NODISCARD constexpr const std::optional<PokemonID> getPokemonID(const std::string_view &name) const
			{
				return getID(name);
			}

			/*! @brief Looks up a display name by stable pokemon ID.
				@param[in] pokemonID The built-in or custom stable identifier.
				@return The display name if registered, or std::nullopt otherwise.
			*/
			ATTR_NODISCARD constexpr const std::optional<std::string_view> getPokemonName(const PokemonID pokemonID) const
			{
				return getName(pokemonID);
			}

			/*! @brief Returns all currently registered pokemon definitions.
				@return A read-only span that remains valid until mutation or destruction.
			*/
			ATTR_NODISCARD constexpr const std::span<const PokemonMeta> getRegisteredPokemons() const noexcept
			{
				return getRegisteredEntries();
			}

			/*! @brief Returns the number of registered built-in and custom pokemons.
				@return The current registry entry count.
			*/
			ATTR_NODISCARD constexpr us getAmountRegistered() const noexcept
			{
				return Base::getAmountRegistered();
			}

			/*! @brief Checks whether an pokemon name is registered.
				@param[in] name The case-sensitive display name.
				@return True if the name is registered, otherwise false.
			*/
			ATTR_NODISCARD constexpr bool hasPokemon(const std::string_view &name) const
			{
				return hasEntry(name);
			}

			/*! @brief Checks whether an pokemon ID is registered.
				@param[in] pokemonID The built-in or custom stable identifier.
				@return True if the ID is registered, otherwise false.
			*/
			ATTR_NODISCARD constexpr bool hasPokemon(const PokemonID pokemonID) const
			{
				return hasEntry(pokemonID);
			}

			/*! @brief Registers one user-defined pokemon and assigns a stable ID.
				@param[in] pokemonMeta The name and trigger metadata to copy into the registry.
				@return The assigned ID on success, or @ref RegistryErrorInfo on duplicate name or exhausted capacity.
			*/
			ATTR_NODISCARD std::expected<PokemonID, RegistryErrorInfo> addPokemon(const PokemonMeta &pokemonMeta);

			/*! @brief Registers multiple pokemons atomically.
				@details Restores the complete prior registry state if any definition fails validation.
				@param[in] pokemonMetas The pokemon definitions to register in order.
				@return Void on success, or the first @ref RegistryErrorInfo on failure.
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> addPokemons(const std::span<const PokemonMeta> &pokemonMetas);

			/*! @brief Renames an pokemon without changing its other metadata.
				@details @p newName is stored as a non-owning view and its backing storage must remain valid while registered.
				@param[in] oldName The currently registered display name.
				@param[in] newName The unique replacement display name.
				@return Void on success, or @ref RegistryErrorInfo if the source is absent or target name is already registered.
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> renamePokemon(const std::string_view &oldName,
																				const std::string_view &newName);

			/*! @brief Replaces all pokemon metadata for an pokemon selected by stable ID.
				@param[in] pokemonName The registered display name.
				@param[in] pokemonMeta The metadata to copy into the registry.
				@return Void on success, or @ref RegistryErrorInfo if the pokemon is not registered.
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> updatePokemon(const std::string_view &pokemonName,
																				const PokemonMeta &pokemonMeta);

			/*! @overload std::expected<void, RegistryErrorInfo> updatePokemon(PokemonID, const PokemonMeta &pokemonMeta)
				@brief Replaces all pokemon metadata for an pokemon selected by stable ID.
				@param[in] pokemonID The built-in or custom stable identifier.
				@param[in] pokemonMeta The metadata to copy into the registry.
				@return Void on success, or @ref RegistryErrorInfo if the pokemon is not registered.
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> updatePokemon(const PokemonID pokemonID, const PokemonMeta &pokemonMeta);

			/*! @brief Removes an pokemon by display name.
				@param[in] pokemonName The registered display name.
				@return The removed stable ID on success, or @ref RegistryErrorInfo if no matching pokemon exists.
			*/
			ATTR_NODISCARD std::expected<PokemonID, RegistryErrorInfo> removePokemon(const std::string_view &pokemonName);

			/*! @overload std::expected<PokemonID, RegistryErrorInfo> removePokemon(PokemonID)
				@brief Removes an pokemon by stable ID.
				@param[in] pokemonID The built-in or custom stable identifier.
				@return The removed stable ID on success, or @ref RegistryErrorInfo if no matching pokemon exists.
			*/
			ATTR_NODISCARD std::expected<PokemonID, RegistryErrorInfo> removePokemon(const PokemonID pokemonID);
	};
} // namespace PocketCore::Configuration

#endif
