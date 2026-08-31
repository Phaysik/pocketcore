/*! @file pokemonRegistry.h
	@brief Provides fixed-capacity storage and lookup for built-in and user-defined pokemons.
	@date 08/26/2026
	@since 0.11.6
	@version 0.12.5
	@author Matthew Moore
*/

#ifndef INCLUDE_REGISTRY_POKEMON_REGISTRY_H
#define INCLUDE_REGISTRY_POKEMON_REGISTRY_H

#include <optional>
#include <span>
#include <string_view>

#include "Ability/builtInAbilityID.h"
#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Effect/builtInEffectID.h"
#include "Item/builtInItemID.h"
#include "Pokemon/builtInPokemonID.h"
#include "Pokemon/constants.h"
#include "Pokemon/pokemonID.h"
#include "Pokemon/pokemonMeta.h"
#include "Registry/fixedMetadataRegistry.h"
#include "Types/builtInTypeID.h"

namespace PocketCore::Registry::Pokemon
{
	using PocketCore::Ability::BuiltinAbilityID;
	using PocketCore::Ability::toAbilityID;
	using PocketCore::Configuration::MAX_POKEMON;
	using PocketCore::Core::us;
	using PocketCore::Effect::BuiltinEffectID;
	using PocketCore::Item::BuiltinItemID;
	using PocketCore::Item::toItemID;
	using PocketCore::Pokemon::BuiltinPokemonID;
	using PocketCore::Pokemon::PokemonID;
	using PocketCore::Pokemon::PokemonMeta;
	using PocketCore::Pokemon::toPokemonID;
	using PocketCore::Registry::FixedMetadataRegistry;
	using PocketCore::Type::BuiltInTypeID;
	using PocketCore::Type::toTypeID;

	/*! @class PokemonRegistry Registry/pokemonRegistry.h
		@brief Stores built-in and user-defined pokemon metadata in fixed-capacity storage.
		@details Built-in pokemons are registered during construction with IDs derived from @ref BuiltinPokemonID. Configuration code may
	   append, replace, or remove entries through the low-level mutators while battle-time callers use allocation-free lookup operations.
		@note Lookup operations are O(n), where n is bounded by @ref MAX_POKEMON.
		@date 08/22/2026
		@since 0.11.6
		@version 0.12.0
		@author Matthew Moore
	*/
	class PokemonRegistry : private FixedMetadataRegistry<PokemonMeta, PokemonID, MAX_POKEMON, &PokemonMeta::mPokemonID>
	{
		private:
			using Base = FixedMetadataRegistry<PokemonMeta, PokemonID, MAX_POKEMON, &PokemonMeta::mPokemonID>;

		public:
			// LCOV_EXCL_START - If the built in additions fail, the program wouldn't work anyway
			/*! @brief Constructs a registry populated with every @ref BuiltinPokemonID.
				@since 0.11.6
				@version 0.12.0
			 */
			ATTR_NOINLINE explicit constexpr PokemonRegistry() : Base{toPokemonID(BuiltinPokemonID::FinalPokemon).getValue()}
			{
				addBuiltin({.mName = PocketCore::Pokemon::POKEMON_NAME_NONE, .mPokemonID = toPokemonID(BuiltinPokemonID::None)});
				addBuiltin({
					.mName = PocketCore::Pokemon::POKEMON_NAME_BULBASAUR,
					.mTypeIDs = {toTypeID(BuiltInTypeID::Grass), toTypeID(BuiltInTypeID::Poison)},
					.mAbilityPool = {toAbilityID(BuiltinAbilityID::None)},
					.mPokemonID = toPokemonID(BuiltinPokemonID::Bulbasaur),
					.mAbilityPoolCount = 0,
				});
				addBuiltin({
					.mName = PocketCore::Pokemon::POKEMON_NAME_IVYSAUR,
					.mTypeIDs = {toTypeID(BuiltInTypeID::Grass), toTypeID(BuiltInTypeID::Poison)},
					.mAbilityPool = {toAbilityID(BuiltinAbilityID::None)},
					.mPokemonID = toPokemonID(BuiltinPokemonID::Ivysaur),
					.mAbilityPoolCount = 0,
				});
				addBuiltin({
					.mName = PocketCore::Pokemon::POKEMON_NAME_VENUSAUR,
					.mTypeIDs = {toTypeID(BuiltInTypeID::Grass), toTypeID(BuiltInTypeID::Poison)},
					.mAbilityPool = {toAbilityID(BuiltinAbilityID::None)},
					.mPokemonID = toPokemonID(BuiltinPokemonID::Venusaur),
					.mAbilityPoolCount = 0,
				});
				addBuiltin({
					.mName = PocketCore::Pokemon::POKEMON_NAME_CHARMANDER,
					.mTypeIDs = {toTypeID(BuiltInTypeID::Fire)},
					.mAbilityPool = {toAbilityID(BuiltinAbilityID::None)},
					.mPokemonID = toPokemonID(BuiltinPokemonID::Charmander),
					.mAbilityPoolCount = 0,
				});
				addBuiltin({
					.mName = PocketCore::Pokemon::POKEMON_NAME_CHARMELEON,
					.mTypeIDs = {toTypeID(BuiltInTypeID::Fire)},
					.mAbilityPool = {toAbilityID(BuiltinAbilityID::None)},
					.mPokemonID = toPokemonID(BuiltinPokemonID::Charmeleon),
					.mAbilityPoolCount = 0,
				});
				addBuiltin({
					.mName = PocketCore::Pokemon::POKEMON_NAME_CHARIZARD,
					.mTypeIDs = {toTypeID(BuiltInTypeID::Fire), toTypeID(BuiltInTypeID::Flying)},
					.mAbilityPool = {toAbilityID(BuiltinAbilityID::None)},
					.mPokemonID = toPokemonID(BuiltinPokemonID::Charizard),
					.mAbilityPoolCount = 0,
				});
				addBuiltin({
					.mName = PocketCore::Pokemon::POKEMON_NAME_SQUIRTLE,
					.mTypeIDs = {toTypeID(BuiltInTypeID::Water)},
					.mAbilityPool = {toAbilityID(BuiltinAbilityID::None)},
					.mPokemonID = toPokemonID(BuiltinPokemonID::Squirtle),
					.mAbilityPoolCount = 0,
				});
				addBuiltin({
					.mName = PocketCore::Pokemon::POKEMON_NAME_WARTORTLE,
					.mTypeIDs = {toTypeID(BuiltInTypeID::Water)},
					.mAbilityPool = {toAbilityID(BuiltinAbilityID::None)},
					.mPokemonID = toPokemonID(BuiltinPokemonID::Wartortle),
					.mAbilityPoolCount = 0,
				});
				addBuiltin({
					.mName = PocketCore::Pokemon::POKEMON_NAME_BLASTOISE,
					.mTypeIDs = {toTypeID(BuiltInTypeID::Water)},
					.mAbilityPool = {toAbilityID(BuiltinAbilityID::None)},
					.mPokemonID = toPokemonID(BuiltinPokemonID::Blastoise),
					.mAbilityPoolCount = 0,
				});
			}

			// LCOV_EXCL_STOP

			using Base::decrementAmountRegistered;
			using Base::eraseEntry;
			using Base::findIndexByID;
			using Base::getAmountRegistered;
			using Base::getEntry;
			using Base::getID;
			using Base::getMetadata;
			using Base::getName;
			using Base::getNextID;
			using Base::getRegisteredEntries;
			using Base::hasEntry;
			using Base::incrementAmountRegistered;
			using Base::incrementNextID;
			using Base::setAmountRegistered;
			using Base::setEntry;
			using Base::setNextID;

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

			/*! @brief Sets the next custom pokemon ID counter.
				@param[in] nextID The next underlying ID value.
				@since 0.11.6
				@version 0.11.6
			*/
			constexpr void setNextPokemonID(const us nextID) noexcept
			{
				setNextID(nextID);
			}

			/*! @brief Increments the next custom pokemon ID counter.
				@since 0.11.6
				@version 0.11.6
			 */
			constexpr void incrementNextPokemonID() noexcept
			{
				incrementNextID();
			}
	};
} // namespace PocketCore::Registry::Pokemon

#endif
