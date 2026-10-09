/*! @file pokemonHelpers.h
	@brief Houses free functions that aide in handling pokemon.
	@date 10/09/2026
	@since 0.12.28
	@version 0.12.51
	@author Matthew Moore
*/

#ifndef INCLUDE_POKEMON_POKEMON_HELPERS_H
#define INCLUDE_POKEMON_POKEMON_HELPERS_H

#include <array>
#include <expected>
#include <span>

#include "Configuration/constants.h"
#include "Learnset/learnsetMeta.h"
#include "Nature/natureID.h"
#include "Pokemon/pokemonMeta.h"
#include "Registry/abilityRegistry.h"
#include "Registry/itemRegistry.h"
#include "Registry/moveRegistry.h"
#include "Registry/natureRegistry.h"
#include "Registry/registryProvider.h"
#include "Validation/Ability/abilityError.h"
#include "Validation/Item/itemError.h"
#include "Validation/Move/moveError.h"
#include "Validation/Nature/natureError.h"
#include "Validation/Pokemon/pokemonError.h"

#include "pokemon.h"

namespace PocketCore::Pokemon
{
	using PocketCore::Configuration::MAX_ABILITIES_PER_POKEMON;
	using PocketCore::Configuration::MAX_ITEMS_PER_POKEMON;
	using PocketCore::Configuration::MAX_MOVES_PER_POKEMON;
	using PocketCore::Configuration::MAX_NATURES_PER_POKEMON;
	using PocketCore::Learnset::LearnsetMeta;
	using PocketCore::Nature::NatureID;
	using PocketCore::Registry::Ability::AbilityRegistry;
	using PocketCore::Registry::Item::ItemRegistry;
	using PocketCore::Registry::Move::MoveRegistry;
	using PocketCore::Registry::Nature::NatureRegistry;
	using PocketCore::Registry::RegistryProvider;
	using PocketCore::Validation::Ability::AbilityError;
	using PocketCore::Validation::Item::ItemError;
	using PocketCore::Validation::Move::MoveError;
	using PocketCore::Validation::Nature::NatureError;
	using PocketCore::Validation::Pokemon::PokemonError;

	/*! @brief Writes a Pokemon with stable identifier names resolved from runtime registries.
		@details Ability, item, type, status, and move identifiers are printed with their registered names. Missing registry entries are
		   printed as `<unregistered>`.
		@param[in,out] outStream The stream receiving the formatted Pokemon state.
		@param[in] pokemon The Pokemon whose state is printed.
		@param[in] registryProvider The registries used to resolve stable identifier names. Its registry pointers may be nullptr.
		@return The supplied stream after writing the complete representation.
		@since 0.11.2
		@version 0.12.50
	*/
	std::ostream &printPokemonWithNames(std::ostream &outStream, const Pokemon &pokemon, const RegistryProvider &registryProvider);

	/*!
		@since 0.12.51
		@version 0.12.51
	*/
	std::expected<void, NatureError> getValidPokemonNatures(const std::span<const NatureID, MAX_NATURES_PER_POKEMON> *natureIDs,
															std::array<NatureID, MAX_NATURES_PER_POKEMON> &validNatureIDs,
															const NatureRegistry &natureRegistry);

	/*!
		@since 0.12.51
		@version 0.12.51
	*/
	std::expected<void, AbilityError> getValidPokemonAbilities(const std::span<const AbilityID, MAX_ABILITIES_PER_POKEMON> *abilityIDs,
															   std::array<AbilityID, MAX_ABILITIES_PER_POKEMON> &validAbilityIDs,
															   const AbilityRegistry &abilityRegistry);

	/*!
		@since 0.12.51
		@version 0.12.51
	*/
	std::expected<void, ItemError> getValidPokemonItems(const std::span<const ItemID, MAX_ITEMS_PER_POKEMON> *itemIDs,
														std::array<ItemID, MAX_ITEMS_PER_POKEMON> &validItemIDs,
														const ItemRegistry &itemRegistry);

	/*!
		@since 0.12.51
		@version 0.12.51
	*/
	std::expected<void, MoveError> getValidPokemonMoves(const std::span<const MoveID, MAX_MOVES_PER_POKEMON> *moveIDs,
														std::array<MoveID, MAX_MOVES_PER_POKEMON> &validMoveIDs,
														const MoveRegistry &moveRegistry, const LearnsetMeta &learnsetMeta,
														const PokemonMeta &pokemonMeta);

	/*!
		@since 0.12.51
		@version 0.12.51
	*/
	std::expected<void, PokemonError> getValidPokemonIVs(const std::span<const us, POKEMON_STAT_COUNT> *ivs,
														 std::array<us, POKEMON_STAT_COUNT> &validIVs);

	/*!
		@since 0.12.51
		@version 0.12.51
	*/
	std::expected<void, PokemonError> getValidPokemonEVs(const std::span<const us, POKEMON_STAT_COUNT> *evs,
														 std::array<us, POKEMON_STAT_COUNT> &validEVs);
} // namespace PocketCore::Pokemon

#endif
