/*! @file pokemonRegistry.h
	@brief Provides fixed-capacity storage and lookup for built-in and user-defined pokemons.
	@date 09/12/2026
	@since 0.11.6
	@version 0.12.28
	@author Matthew Moore
*/

#ifndef INCLUDE_REGISTRY_POKEMON_REGISTRY_H
#define INCLUDE_REGISTRY_POKEMON_REGISTRY_H

#include <expected>
#include <optional>
#include <span>
#include <string_view>

#include "Ability/abilityID.h"
#include "Ability/abilityMeta.h"
#include "Ability/builtInAbilityID.h"
#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Effect/builtInEffectID.h"
#include "Item/builtInItemID.h"
#include "Item/itemID.h"
#include "Item/itemMeta.h"
#include "Learnset/builtInLearnsetID.h"
#include "Move/moveID.h"
#include "Move/moveMeta.h"
#include "Nature/natureID.h"
#include "Nature/natureMeta.h"
#include "Pokemon/builtInPokemonID.h"
#include "Pokemon/constants.h"
#include "Pokemon/pokemon.h"
#include "Pokemon/pokemonID.h"
#include "Pokemon/pokemonMeta.h"
#include "Registry/abilityRegistry.h"
#include "Registry/fixedMetadataRegistry.h"
#include "Registry/itemRegistry.h"
#include "Registry/moveRegistry.h"
#include "Registry/natureRegistry.h"
#include "Registry/registryError.h"
#include "Types/builtInTypeID.h"

namespace PocketCore::Registry::Pokemon
{
	using PocketCore::Ability::AbilityID;
	using PocketCore::Ability::AbilityMeta;
	using PocketCore::Ability::BuiltinAbilityID;
	using PocketCore::Ability::toAbilityID;
	using PocketCore::Configuration::MAX_ABILITIES_PER_POKEMON;
	using PocketCore::Configuration::MAX_ITEMS_PER_POKEMON;
	using PocketCore::Configuration::MAX_MOVES_PER_POKEMON;
	using PocketCore::Configuration::MAX_NATURES_PER_POKEMON;
	using PocketCore::Configuration::MAX_POKEMON;
	using PocketCore::Configuration::MIN_EV_STAT_VALUE;
	using PocketCore::Core::us;
	using PocketCore::Effect::BuiltinEffectID;
	using PocketCore::Item::BuiltinItemID;
	using PocketCore::Item::ItemID;
	using PocketCore::Item::ItemMeta;
	using PocketCore::Item::toItemID;
	using PocketCore::Learnset::BuiltinLearnsetID;
	using PocketCore::Learnset::toLearnsetID;
	using PocketCore::Move::MoveID;
	using PocketCore::Move::MoveMeta;
	using PocketCore::Nature::NatureID;
	using PocketCore::Nature::NatureMeta;
	using PocketCore::Pokemon::BLASTOISE_BASE_STATS;
	using PocketCore::Pokemon::BuiltinPokemonID;
	using PocketCore::Pokemon::BULBASAUR_BASE_STATS;
	using PocketCore::Pokemon::CHARIZARD_BASE_STATS;
	using PocketCore::Pokemon::CHARMANDER_BASE_STATS;
	using PocketCore::Pokemon::CHARMELEON_BASE_STATS;
	using PocketCore::Pokemon::IVYSAUR_BASE_STATS;
	using PocketCore::Pokemon::Pokemon;
	using PocketCore::Pokemon::POKEMON_NAME_BLASTOISE;
	using PocketCore::Pokemon::POKEMON_NAME_BULBASAUR;
	using PocketCore::Pokemon::POKEMON_NAME_CHARIZARD;
	using PocketCore::Pokemon::POKEMON_NAME_CHARMANDER;
	using PocketCore::Pokemon::POKEMON_NAME_CHARMELEON;
	using PocketCore::Pokemon::POKEMON_NAME_IVYSAUR;
	using PocketCore::Pokemon::POKEMON_NAME_NONE;
	using PocketCore::Pokemon::POKEMON_NAME_SQUIRTLE;
	using PocketCore::Pokemon::POKEMON_NAME_VENUSAUR;
	using PocketCore::Pokemon::POKEMON_NAME_WARTORTLE;
	using PocketCore::Pokemon::POKEMON_STAT_COUNT;
	using PocketCore::Pokemon::PokemonID;
	using PocketCore::Pokemon::PokemonMeta;
	using PocketCore::Pokemon::SQUIRTLE_BASE_STATS;
	using PocketCore::Pokemon::toPokemonID;
	using PocketCore::Pokemon::VENUSAUR_BASE_STATS;
	using PocketCore::Pokemon::WARTORTLE_BASE_STATS;
	using PocketCore::Registry::Ability::AbilityRegistry;
	using PocketCore::Registry::FixedMetadataRegistry;
	using PocketCore::Registry::Item::ItemRegistry;
	using PocketCore::Registry::Move::MoveRegistry;
	using PocketCore::Registry::Nature::NatureRegistry;
	using PocketCore::Registry::RegistryErrorInfo;
	using PocketCore::Type::BuiltinTypeID;
	using PocketCore::Type::toTypeID;

	/*! @struct PokemonInstantiationDependencies Configuration/pokemonRegistryConfiguration.h
		@brief Aggregates non-owning pointers to runtime metadata registries needed for Pokemon instantiation.
		@details Provides a lightweight dependency bundle passed into the Pokemon instantiation system.
		All pointers are non-owning and must refer to registry instances whose lifetime exceeds the provider usage.
		@warning Dereferencing any null member pointer is undefined behavior.
		@date 09/12/2026
		@since 0.12.24
		@version 0.12.28
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

	/*! @class PokemonRegistry Registry/pokemonRegistry.h
		@brief Stores built-in and user-defined pokemon metadata in fixed-capacity storage.
		@details Built-in pokemons are registered during construction with IDs derived from @ref BuiltinPokemonID. Configuration code may
	   append, replace, or remove entries through the low-level mutators while battle-time callers use allocation-free lookup operations.
		@note Lookup operations are O(n), where n is bounded by @ref MAX_POKEMON.
		@date 09/12/2026
		@since 0.11.6
		@version 0.12.12
		@author Matthew Moore
	*/
	class PokemonRegistry : private FixedMetadataRegistry<PokemonMeta, PokemonID, MAX_POKEMON, &PokemonMeta::mPokemonID>
	{
		private:
			using Base = FixedMetadataRegistry<PokemonMeta, PokemonID, MAX_POKEMON, &PokemonMeta::mPokemonID>;

		public:
			/*! @brief Compares two PokemonRegistry instances for equality.
				@param[in] other The other registry to compare with.
				@return true if the registries are equal, false otherwise.
				@since 0.12.20
				@version 0.12.20
			*/
			ATTR_NODISCARD constexpr bool operator==(const PokemonRegistry &other) const noexcept = default;

			// LCOV_EXCL_START - If the built in additions fail, the program wouldn't work anyway

			/*! @brief Constructs a registry populated with every @ref BuiltinPokemonID.
				@since 0.11.6
				@version 0.12.24
			 */
			ATTR_NOINLINE explicit constexpr PokemonRegistry() : Base{toPokemonID(BuiltinPokemonID::FinalPokemon).getValue()}
			{
				addBuiltin({.mName = std::string(POKEMON_NAME_NONE), .mPokemonID = toPokemonID(BuiltinPokemonID::None)});
				addBuiltin({
					.mName = std::string(POKEMON_NAME_BULBASAUR),
					.mAbilityPool = {toAbilityID(BuiltinAbilityID::None)},
					.mBaseStats = BULBASAUR_BASE_STATS,
					.mTypeIDs = {toTypeID(BuiltinTypeID::Grass), toTypeID(BuiltinTypeID::Poison)},
					.mPokemonID = toPokemonID(BuiltinPokemonID::Bulbasaur),
				});
				addBuiltin({
					.mName = std::string(POKEMON_NAME_IVYSAUR),
					.mAbilityPool = {toAbilityID(BuiltinAbilityID::None)},
					.mBaseStats = IVYSAUR_BASE_STATS,
					.mTypeIDs = {toTypeID(BuiltinTypeID::Grass), toTypeID(BuiltinTypeID::Poison)},
					.mPokemonID = toPokemonID(BuiltinPokemonID::Ivysaur),
				});
				addBuiltin({
					.mName = std::string(POKEMON_NAME_VENUSAUR),
					.mAbilityPool = {toAbilityID(BuiltinAbilityID::None)},
					.mBaseStats = VENUSAUR_BASE_STATS,
					.mTypeIDs = {toTypeID(BuiltinTypeID::Grass), toTypeID(BuiltinTypeID::Poison)},
					.mPokemonID = toPokemonID(BuiltinPokemonID::Venusaur),
				});
				addBuiltin({
					.mName = std::string(POKEMON_NAME_CHARMANDER),
					.mAbilityPool = {toAbilityID(BuiltinAbilityID::None)},
					.mBaseStats = CHARMANDER_BASE_STATS,
					.mTypeIDs = {toTypeID(BuiltinTypeID::Fire)},
					.mPokemonID = toPokemonID(BuiltinPokemonID::Charmander),
				});
				addBuiltin({
					.mName = std::string(POKEMON_NAME_CHARMELEON),
					.mAbilityPool = {toAbilityID(BuiltinAbilityID::None)},
					.mBaseStats = CHARMELEON_BASE_STATS,
					.mTypeIDs = {toTypeID(BuiltinTypeID::Fire)},
					.mPokemonID = toPokemonID(BuiltinPokemonID::Charmeleon),
				});
				addBuiltin({
					.mName = std::string(POKEMON_NAME_CHARIZARD),
					.mAbilityPool = {toAbilityID(BuiltinAbilityID::None)},
					.mBaseStats = CHARIZARD_BASE_STATS,
					.mTypeIDs = {toTypeID(BuiltinTypeID::Fire), toTypeID(BuiltinTypeID::Flying)},
					.mPokemonID = toPokemonID(BuiltinPokemonID::Charizard),
				});
				addBuiltin({
					.mName = std::string(POKEMON_NAME_SQUIRTLE),
					.mAbilityPool = {toAbilityID(BuiltinAbilityID::None)},
					.mBaseStats = SQUIRTLE_BASE_STATS,
					.mTypeIDs = {toTypeID(BuiltinTypeID::Water)},
					.mPokemonID = toPokemonID(BuiltinPokemonID::Squirtle),
				});
				addBuiltin({
					.mName = std::string(POKEMON_NAME_WARTORTLE),
					.mAbilityPool = {toAbilityID(BuiltinAbilityID::None)},
					.mBaseStats = WARTORTLE_BASE_STATS,
					.mTypeIDs = {toTypeID(BuiltinTypeID::Water)},
					.mPokemonID = toPokemonID(BuiltinPokemonID::Wartortle),
				});
				addBuiltin({
					.mName = std::string(POKEMON_NAME_BLASTOISE),
					.mAbilityPool = {toAbilityID(BuiltinAbilityID::None)},
					.mBaseStats = BLASTOISE_BASE_STATS,
					.mTypeIDs = {toTypeID(BuiltinTypeID::Water)},
					.mPokemonID = toPokemonID(BuiltinPokemonID::Blastoise),
				});
			}

			// LCOV_EXCL_STOP

		protected:
			using Base::addEntry;
			using Base::createCheckpoint;
			using Base::decrementAmountRegistered;
			using Base::eraseEntry;
			using Base::incrementAmountRegistered;
			using Base::restoreCheckpoint;
			using Base::setAmountRegistered;
			using Base::setEntry;

		public:
			using Base::findIndexByID;
			using Base::getAmountRegistered;
			using Base::getEntry;
			using Base::getID;
			using Base::getMetadata;
			using Base::getName;
			using Base::getNextID;
			using Base::getRegisteredEntries;
			using Base::hasEntry;

			/*! @brief Looks up pokemon metadata by stable ID.
				@param[in] pokemonID The stable pokemon identifier.
				@return A non-owning pointer to metadata if registered, or nullptr otherwise. The pointer remains valid until replacement or
			   registry destruction.
				@since 0.11.6
				@version 0.11.6
			*/
			ATTR_NODISCARD constexpr const PokemonMeta *getPokemonMetadata(const PokemonID pokemonID) const
			{
				return getMetadata(pokemonID);
			}

			/*! @brief Looks up an pokemon ID by display name.
				@param[in] name The case-sensitive display name.
				@return The stable ID if registered, or std::nullopt otherwise.
				@since 0.11.6
				@version 0.11.6
			*/
			ATTR_NODISCARD constexpr const std::optional<PokemonID> getPokemonID(const std::string_view &name) const
			{
				return getID(name);
			}

			/*! @brief Looks up an pokemon display name by stable ID.
				@param[in] pokemonID The stable pokemon identifier.
				@return The display name if registered, or std::nullopt otherwise.
				@since 0.11.6
				@version 0.11.6
			*/
			ATTR_NODISCARD constexpr const std::optional<std::string_view> getPokemonName(const PokemonID pokemonID) const
			{
				return getName(pokemonID);
			}

			/*! @brief Returns all currently registered pokemon definitions.
				@return A read-only span that remains valid until the registry is mutated or destroyed.
				@since 0.11.6
				@version 0.11.6
			*/
			ATTR_NODISCARD constexpr const std::span<const PokemonMeta> getRegisteredPokemons() const noexcept
			{
				return getRegisteredEntries();
			}

			/*! @brief Returns the next stable ID assigned to a custom pokemon.
				@return The underlying numeric value of the next pokemon ID.
				@since 0.11.6
				@version 0.11.6
			*/
			ATTR_NODISCARD constexpr us getNextPokemonID() const noexcept
			{
				return getNextID();
			}

			/*! @brief Finds an internal array index by stable pokemon ID.
				@param[in] pokemonID The stable pokemon identifier.
				@return The internal index if registered, or std::nullopt otherwise.
				@since 0.11.6
				@version 0.11.6
			*/
			ATTR_NODISCARD constexpr const std::optional<us> findIndexByPokemonID(const PokemonID pokemonID) const
			{
				return findIndexByID(pokemonID);
			}

			/*! @brief Checks whether an pokemon name is registered.
				@param[in] name The case-sensitive display name.
				@return True if the name is registered, otherwise false.
				@since 0.11.6
				@version 0.11.6
			*/
			ATTR_NODISCARD constexpr bool hasPokemon(const std::string_view &name) const
			{
				return hasEntry(name);
			}

			/*! @brief Checks whether an pokemon ID is registered.
				@param[in] pokemonID The stable pokemon identifier.
				@return True if the ID is registered, otherwise false.
				@since 0.11.6
				@version 0.11.6
			*/
			ATTR_NODISCARD constexpr bool hasPokemon(const PokemonID pokemonID) const
			{
				return hasEntry(pokemonID);
			}

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
				@since 0.12.24
				@version 0.12.26
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

		private:
			/*! Validates that all metas in the given array are not null.
				@tparam Meta The type of the meta to validate.
				@tparam N The number of metas in the array.
				@param[in] metas The array of metas to validate.
				@param[in] code The error code to return if any metas are null.
				@param[in] message The error message to return if any metas are null.
				@return A @ref RegistryErrorInfo if any metas are null, or std::nullopt if all metas are valid.
				@since 0.12.26
				@version 0.12.26
			 */
			template <typename Meta, std::size_t N>
			ATTR_NODISCARD constexpr std::optional<RegistryErrorInfo> validateMetas(const std::array<const Meta *, N> &metas,
																					const RegistryError code,
																					const std::string_view &message) const
			{
				const bool missing{std::ranges::any_of(metas, [](const Meta *meta) { return meta == nullptr; })};

				if (missing)
				{
					return RegistryErrorInfo{code, {}, std::string{message}};
				}

				return std::nullopt;
			}

			/*! @brief Validates the metadata for the given nature IDs.
				@param[in] natureIDs The array of nature IDs to validate.
				@param[in] dependencyRegistries The dependency registries containing the nature metadata.
				@return A @ref RegistryErrorInfo if any nature IDs are invalid, or std::nullopt if all are valid.
				@since 0.12.26
				@version 0.12.26
			 */
			ATTR_NODISCARD constexpr std::expected<void, RegistryErrorInfo> validateNatureMetadata(
				const std::array<NatureID, MAX_NATURES_PER_POKEMON> *natureIDs,
				const PokemonInstantiationDependencies &dependencyRegistries) const
			{
				std::array<const NatureMeta *, MAX_NATURES_PER_POKEMON> natureMetas{};
				std::ranges::transform(*natureIDs, natureMetas.begin(), [&dependencyRegistries](const NatureID natureID) {
					return dependencyRegistries.natureRegistry->getNatureMetadata(natureID);
				});

				if (const std::optional<RegistryErrorInfo> error{validateMetas(
						natureMetas, RegistryError::NatureNotFound, "PokemonRegistryConfiguration::instantiate: missing nature metadata")})
				{
					return std::unexpected{error.value()};
				}

				return {};
			}

			/*! @brief Validates the metadata for the given item IDs.
				@param[in] itemIDs The array of item IDs to validate.
				@param[in] dependencyRegistries The dependency registries containing the item metadata.
				@return A @ref RegistryErrorInfo if any item IDs are invalid, or std::nullopt if all are valid.
				@since 0.12.26
				@version 0.12.26
			 */
			ATTR_NODISCARD constexpr std::expected<void, RegistryErrorInfo> validateItemMetadata(
				const std::array<ItemID, MAX_ITEMS_PER_POKEMON> *itemIDs,
				const PokemonInstantiationDependencies &dependencyRegistries) const
			{
				std::array<const ItemMeta *, MAX_ITEMS_PER_POKEMON> itemMetas{};
				std::ranges::transform(*itemIDs, itemMetas.begin(), [&dependencyRegistries](const ItemID itemID) {
					return dependencyRegistries.itemRegistry->getItemMetadata(itemID);
				});

				if (const std::optional<RegistryErrorInfo> error{validateMetas(
						itemMetas, RegistryError::ItemNotFound, "PokemonRegistryConfiguration::instantiate: missing item metadata")})
				{
					return std::unexpected{error.value()};
				}

				return {};
			}

			/*! @brief Validates the metadata for the given ability IDs.
				@param[in] abilityIDs The array of ability IDs to validate.
				@param[in] dependencyRegistries The dependency registries containing the ability metadata.
				@return A @ref RegistryErrorInfo if any ability IDs are invalid, or std::nullopt if all are valid.
				@since 0.12.26
				@version 0.12.26
			 */
			ATTR_NODISCARD constexpr std::expected<void, RegistryErrorInfo> validateAbilityMetadata(
				const std::array<AbilityID, MAX_ABILITIES_PER_POKEMON> *abilityIDs,
				const PokemonInstantiationDependencies &dependencyRegistries) const
			{
				std::array<const AbilityMeta *, MAX_ABILITIES_PER_POKEMON> abilityMetas{};
				std::ranges::transform(*abilityIDs, abilityMetas.begin(), [&dependencyRegistries](const AbilityID abilityID) {
					return dependencyRegistries.abilityRegistry->getAbilityMetadata(abilityID);
				});

				if (const std::optional<RegistryErrorInfo> error{
						validateMetas(abilityMetas, RegistryError::AbilityNotFound,
									  "PokemonRegistryConfiguration::instantiate: missing ability metadata")})
				{
					return std::unexpected{error.value()};
				}

				return {};
			}

			/*! @brief Validates the metadata for the given move IDs.
				@param[in] moveIDs The array of move IDs to validate.
				@param[in] dependencyRegistries The dependency registries containing the move metadata.
				@return A @ref RegistryErrorInfo if any move IDs are invalid, or std::nullopt if all are valid.
				@since 0.12.26
				@version 0.12.26
			 */
			ATTR_NODISCARD constexpr std::expected<void, RegistryErrorInfo> validateMoveMetadata(
				const std::array<MoveID, MAX_MOVES_PER_POKEMON> *moveIDs,
				const PokemonInstantiationDependencies &dependencyRegistries) const
			{
				std::array<const MoveMeta *, MAX_MOVES_PER_POKEMON> moveMetas{};
				std::ranges::transform(*moveIDs, moveMetas.begin(), [&dependencyRegistries](const MoveID moveID) {
					return dependencyRegistries.moveRegistry->getMoveMetadata(moveID);
				});

				if (const std::optional<RegistryErrorInfo> error{validateMetas(
						moveMetas, RegistryError::MoveNotFound, "PokemonRegistryConfiguration::instantiate: missing move metadata")})
				{
					return std::unexpected{error.value()};
				}

				return {};
			}
	};
} // namespace PocketCore::Registry::Pokemon

#endif
