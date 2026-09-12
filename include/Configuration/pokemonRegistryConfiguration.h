/*! @file pokemonRegistryConfiguration.h
	@brief Declares the user-facing facade for configuring pokemon metadata.
	@date 09/11/2026
	@since 0.12.0
	@version 0.12.24
	@author Matthew Moore
*/

#ifndef INCLUDE_CONFIGURATION_POKEMON_REGISTRY_CONFIGURATION_H
#define INCLUDE_CONFIGURATION_POKEMON_REGISTRY_CONFIGURATION_H

#include <expected>
#include <optional>
#include <span>
#include <string_view>

#include "Ability/abilityID.h"
#include "Configuration/constants.h"
#include "Configuration/fixedMetadataRegistryConfiguration.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Nature/natureID.h"
#include "Pokemon/pokemon.h"
#include "Pokemon/pokemonID.h"
#include "Pokemon/pokemonMeta.h"
#include "Registry/abilityRegistry.h"
#include "Registry/itemRegistry.h"
#include "Registry/moveRegistry.h"
#include "Registry/natureRegistry.h"
#include "Registry/pokemonRegistry.h"
#include "Registry/registryError.h"

namespace PocketCore::Configuration
{
	using PocketCore::Ability::AbilityID;
	using PocketCore::Configuration::MAX_ABILITIES_PER_POKEMON;
	using PocketCore::Configuration::MAX_ITEMS_PER_POKEMON;
	using PocketCore::Configuration::MAX_MOVES_PER_POKEMON;
	using PocketCore::Configuration::MAX_NATURES_PER_POKEMON;
	using PocketCore::Configuration::MAX_POKEMON;
	using PocketCore::Configuration::MIN_EV_STAT_VALUE;
	using PocketCore::Core::us;
	using PocketCore::Item::ItemID;
	using PocketCore::Move::MoveID;
	using PocketCore::Nature::NatureID;
	using PocketCore::Pokemon::Pokemon;
	using PocketCore::Pokemon::POKEMON_STAT_COUNT;
	using PocketCore::Pokemon::PokemonID;
	using PocketCore::Pokemon::PokemonMeta;
	using PocketCore::Registry::Ability::AbilityRegistry;
	using PocketCore::Registry::Item::ItemRegistry;
	using PocketCore::Registry::Move::MoveRegistry;
	using PocketCore::Registry::Nature::NatureRegistry;
	using PocketCore::Registry::Pokemon::PokemonRegistry;
	using PocketCore::Registry::RegistryErrorInfo;

	/*! @struct PokemonInstantiationDependencies Configuration/pokemonRegistryConfiguration.h
		@brief Aggregates non-owning pointers to runtime metadata registries needed for Pokemon instantiation.
		@details Provides a lightweight dependency bundle passed into the Pokemon instantiation system.
		All pointers are non-owning and must refer to registry instances whose lifetime exceeds the provider usage.
		@warning Dereferencing any null member pointer is undefined behavior.
		@date 09/11/2026
		@since 0.12.24
		@version 0.12.24
		@author Matthew Moore
	*/
	struct PokemonInstantiationDependencies
	{
		public:
			/*! @brief Non-owning pointer to the ability metadata registry.
				@details Must point to a valid @ref Ability::AbilityRegistry instance for ability metadata queries.
			*/
			const AbilityRegistry *abilityRegistry{nullptr};

			/*! @brief Non-owning pointer to the move metadata registry.
				@details Must point to a valid @ref Move::MoveRegistry instance for move metadata queries.
			*/
			const MoveRegistry *moveRegistry{nullptr};

			/*! @brief Non-owning pointer to the item metadata registry.
				@details Must point to a valid @ref Item::ItemRegistry instance for item metadata queries.
			*/
			const ItemRegistry *itemRegistry{nullptr};

			/*! @brief Non-owning pointer to the nature metadata registry.
				@details Must point to a valid @ref Nature::NatureRegistry instance for nature metadata queries.
			*/
			const NatureRegistry *natureRegistry{nullptr};
	};

	namespace Detail
	{
		/*! @struct PokemonRegistryConfigurationPolicy Configuration/pokemonRegistryConfiguration.h
			@brief Policy class providing error codes and display strings for pokemon registry configuration.
			@details Encapsulates the pokemon-specific error categories and display names used by the generic
			 @ref FixedMetadataRegistryConfiguration template to report validation and lookup failures with
			 domain-specific terminology.
			@date 08/30/2026
			@since 0.12.0
			@version 0.12.11
			@author Matthew Moore
		*/
		struct PokemonRegistryConfigurationPolicy
		{
			public:
				/*! @brief The display name of the configuration system. */
				static constexpr std::string_view configurationName{"PokemonRegistryConfiguration"};

				/*! @brief The singular entity type managed by this configuration. */
				static constexpr std::string_view entityName{"pokemon"};

				/*! @brief The error code returned when a duplicate ability name is registered. */
				static constexpr RegistryError duplicateError{RegistryError::DuplicatePokemon};

				/*! @brief The error code returned when an ability lookup fails. */
				static constexpr RegistryError notFoundError{RegistryError::PokemonNotFound};
		};
	} // namespace Detail

	/*! @class PokemonRegistryConfiguration Configuration/pokemonRegistryConfiguration.h
		@brief Provides validated user customization over an internal pokemon registry.
		@details Supports lookup, addition, batch addition, trigger replacement, renaming, and removal. Custom IDs are assigned
	   monotonically and are not reused after removal. Batch additions provide all-or-nothing semantics.
		@date 09/11/2026
		@since 0.12.0
		@version 0.12.24
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
			/*! @brief Constructs a configuration containing all built-in pokemons.
				@since 0.12.0
				@version 0.12.0
			 */
			constexpr PokemonRegistryConfiguration() = default;

			using Base::getAmountRegistered;

			/*! @brief Returns read-only access to the configured runtime pokemon registry.
				@return A reference that remains valid for the lifetime of this configuration.
				@since 0.12.0
				@version 0.12.8
			*/
			ATTR_NODISCARD constexpr const PokemonRegistry &getRuntimeRegistry() const noexcept
			{
				return getRegistry();
			}

			/*! @brief Looks up complete metadata by stable pokemon ID.
				@param[in] pokemonID The built-in or custom stable identifier.
				@return A non-owning pointer to metadata if registered, or nullptr otherwise. The pointer remains valid until replacement or
			   configuration destruction.
				@since 0.12.0
				@version 0.12.0
			*/
			ATTR_NODISCARD constexpr const PokemonMeta *getPokemonMetadata(const PokemonID pokemonID) const
			{
				return getMetadata(pokemonID);
			}

			/*! @brief Looks up a stable pokemon ID by display name.
				@param[in] name The case-sensitive display name.
				@return The stable ID if registered, or std::nullopt otherwise.
				@since 0.12.0
				@version 0.12.0
			*/
			ATTR_NODISCARD constexpr const std::optional<PokemonID> getPokemonID(const std::string_view &name) const
			{
				return getID(name);
			}

			/*! @brief Looks up a display name by stable pokemon ID.
				@param[in] pokemonID The built-in or custom stable identifier.
				@return The display name if registered, or std::nullopt otherwise.
				@since 0.12.0
				@version 0.12.0
			*/
			ATTR_NODISCARD constexpr const std::optional<std::string_view> getPokemonName(const PokemonID pokemonID) const
			{
				return getName(pokemonID);
			}

			/*! @brief Returns all currently registered pokemon definitions.
				@return A read-only span that remains valid until mutation or destruction.
				@since 0.12.0
				@version 0.12.0
			*/
			ATTR_NODISCARD constexpr const std::span<const PokemonMeta> getRegisteredPokemons() const noexcept
			{
				return getRegisteredEntries();
			}

			/*! @brief Checks whether an pokemon name is registered.
				@param[in] name The case-sensitive display name.
				@return True if the name is registered, otherwise false.
				@since 0.12.0
				@version 0.12.0
			*/
			ATTR_NODISCARD constexpr bool hasPokemon(const std::string_view &name) const
			{
				return hasEntry(name);
			}

			/*! @brief Checks whether an pokemon ID is registered.
				@param[in] pokemonID The built-in or custom stable identifier.
				@return True if the ID is registered, otherwise false.
				@since 0.12.0
				@version 0.12.0
			*/
			ATTR_NODISCARD constexpr bool hasPokemon(const PokemonID pokemonID) const
			{
				return hasEntry(pokemonID);
			}

			/*! @brief Registers one user-defined pokemon and assigns a stable ID.
				@param[in] pokemonMeta The name and trigger metadata to copy into the registry.
				@return The assigned ID on success, or @ref RegistryErrorInfo on duplicate name or exhausted capacity.
				@since 0.12.0
				@version 0.12.0
			*/
			ATTR_NODISCARD std::expected<PokemonID, RegistryErrorInfo> addPokemon(const PokemonMeta &pokemonMeta);

			/*! @brief Registers multiple pokemons atomically.
				@details Restores the complete prior registry state if any definition fails validation.
				@param[in] pokemonMetas The pokemon definitions to register in order.
				@return Void on success, or the first @ref RegistryErrorInfo on failure.
				@since 0.12.0
				@version 0.12.0
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> addPokemons(const std::span<const PokemonMeta> &pokemonMetas);

			/*! @brief Renames an pokemon without changing its other metadata.
				@details @p newName is stored as a non-owning view and its backing storage must remain valid while registered.
				@param[in] oldName The currently registered display name.
				@param[in] newName The unique replacement display name.
				@return Void on success, or @ref RegistryErrorInfo if the source is absent or target name is already registered.
				@since 0.12.0
				@version 0.12.0
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> renamePokemon(const std::string_view &oldName,
																				const std::string_view &newName);

			/*! @brief Replaces all pokemon metadata for an pokemon selected by stable ID.
				@param[in] pokemonName The registered display name.
				@param[in] pokemonMeta The metadata to copy into the registry.
				@return Void on success, or @ref RegistryErrorInfo if the pokemon is not registered.
				@since 0.12.0
				@version 0.12.0
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> updatePokemon(const std::string_view &pokemonName,
																				const PokemonMeta &pokemonMeta);

			/*! @overload std::expected<void, RegistryErrorInfo> updatePokemon(PokemonID, const PokemonMeta &pokemonMeta)
				@brief Replaces all pokemon metadata for an pokemon selected by stable ID.
				@param[in] pokemonID The built-in or custom stable identifier.
				@param[in] pokemonMeta The metadata to copy into the registry.
				@return Void on success, or @ref RegistryErrorInfo if the pokemon is not registered.
				@since 0.12.0
				@version 0.12.0
			*/
			ATTR_NODISCARD std::expected<void, RegistryErrorInfo> updatePokemon(const PokemonID pokemonID, const PokemonMeta &pokemonMeta);

			/*! @brief Removes an pokemon by display name.
				@param[in] pokemonName The registered display name.
				@return The removed stable ID on success, or @ref RegistryErrorInfo if no matching pokemon exists.
				@since 0.12.0
				@version 0.12.0
			*/
			ATTR_NODISCARD std::expected<PokemonID, RegistryErrorInfo> removePokemon(const std::string_view &pokemonName);

			/*! @overload std::expected<PokemonID, RegistryErrorInfo> removePokemon(PokemonID)
				@brief Removes an pokemon by stable ID.
				@param[in] pokemonID The built-in or custom stable identifier.
				@return The removed stable ID on success, or @ref RegistryErrorInfo if no matching pokemon exists.
				@since 0.12.0
				@version 0.12.0
			*/
			ATTR_NODISCARD std::expected<PokemonID, RegistryErrorInfo> removePokemon(const PokemonID pokemonID);

			/*! @brief Instantiates a Pokemon from the registry.
				@param[in] pokemonID The built-in or custom stable identifier.
				@param[in] natureRegistry The nature registry to use for resolving nature IDs. Defaults to nullptr.
				@param[in] ivs The IVs of the Pokemon. Defaults to nullptr.
				@param[in] evs The EVs of the Pokemon. Defaults to @ref MIN_EV_STAT_VALUE.
				@param[in] natureIDs The nature IDs of the Pokemon. Defaults to nullptr.
				@param[in] abilityIDs The ability IDs of the Pokemon. Defaults to nullptr.
				@param[in] itemIDs The item IDs of the Pokemon. Defaults to nullptr.
				@param[in] movesIDs The move IDs of the Pokemon. Defaults to nullptr.
				@return The instantiated Pokemon on success, or @ref RegistryErrorInfo if no matching pokemon exists.
				@since 0.12.0
				@version 0.12.0
			*/
			ATTR_NODISCARD std::expected<Pokemon, RegistryErrorInfo> instantiate(
				const PokemonID pokemonID, const PokemonInstantiationDependencies *dependencyRegistries = nullptr,
				const std::array<us, POKEMON_STAT_COUNT> *ivs = nullptr,
				const std::array<us, POKEMON_STAT_COUNT> &evs
				= {MIN_EV_STAT_VALUE, MIN_EV_STAT_VALUE, MIN_EV_STAT_VALUE, MIN_EV_STAT_VALUE, MIN_EV_STAT_VALUE, MIN_EV_STAT_VALUE},
				const std::array<NatureID, MAX_NATURES_PER_POKEMON> *natureIDs = nullptr,
				const std::array<AbilityID, MAX_ABILITIES_PER_POKEMON> *abilityIDs = nullptr,
				const std::array<ItemID, MAX_ITEMS_PER_POKEMON> *itemIDs = nullptr,
				const std::array<MoveID, MAX_MOVES_PER_POKEMON> *moveIDs = nullptr) const;
	};
} // namespace PocketCore::Configuration

#endif
