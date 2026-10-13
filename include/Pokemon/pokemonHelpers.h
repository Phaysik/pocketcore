/*! @file pokemonHelpers.h
	@brief Houses free functions that aide in handling pokemon.
	@date 10/13/2026
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
#include "Core/attributeMacros.h"
#include "Learnset/learnsetMeta.h"
#include "Nature/natureID.h"
#include "Pokemon/pokemonEncounter.h"
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
#include "pokemonStats.h"

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

	/*! @brief Validates supplied moves or generates moves from the Pokemon's learnset.
		@details When @p moveIDs is non-null, the supplied move IDs are checked against @p moveRegistry and copied to @p validMoveIDs;
		duplicate IDs after their first occurrence are replaced with @ref PocketCore::Move::NO_MOVE_ID. Supplied moves are not checked
		against @p learnsetMeta. When @p moveIDs is null, eligible level-up moves are generated from @p learnsetMeta, and egg moves are
		also included when @p encounterData describes an egg encounter. Generated moves are checked against @p moveRegistry.
		@param[in] moveIDs A non-owning span of supplied move IDs, or nullptr to generate moves from the learnset. The span is read only
		and is used only for the duration of the call.
		@param[out] validMoveIDs The fixed-size output array receiving validated or generated move IDs; duplicate supplied slots are
		set to @ref PocketCore::Move::NO_MOVE_ID.
		@param[in] moveRegistry The registry used to verify that each resulting move ID has metadata.
		@param[in] learnsetMeta The learnset used when @p moveIDs is nullptr.
		@param[in] encounterData The encounter origin, which determines whether egg moves are eligible.
		@param[in] pokemonEncounterLevel The Pokemon's encounter level; level-up and egg moves above this level are excluded.
		@return Success when all resulting move IDs have registered metadata; otherwise, @ref
	   PocketCore::Validation::Move::MoveError::MISSING_MOVE_METADATA.
		@since 0.12.51
		@version 0.12.51
	*/
	ATTR_NODISCARD std::expected<void, NatureError> getValidPokemonNatures(const std::span<const NatureID> natureIDs,
																		   std::array<NatureID, MAX_NATURES_PER_POKEMON> &validNatureIDs,
																		   const NatureRegistry &natureRegistry);

	/*!
		@since 0.12.51
		@version 0.12.51
	*/
	ATTR_NODISCARD std::expected<void, AbilityError> getValidPokemonAbilities(
		const std::span<const AbilityID> abilityIDs, std::array<AbilityID, MAX_ABILITIES_PER_POKEMON> &validAbilityIDs,
		const AbilityRegistry &abilityRegistry);

	/*!
		@since 0.12.51
		@version 0.12.51
	*/
	ATTR_NODISCARD std::expected<void, ItemError> getValidPokemonItems(const std::span<const ItemID> itemIDs,
																	   std::array<ItemID, MAX_ITEMS_PER_POKEMON> &validItemIDs,
																	   const ItemRegistry &itemRegistry);

	/*!
		@since 0.12.51
		@version 0.12.51
	*/
	ATTR_NODISCARD std::expected<void, MoveError> getValidPokemonMoves(const std::span<const MoveID> moveIDs,
																	   std::array<MoveID, MAX_MOVES_PER_POKEMON> &validMoveIDs,
																	   const MoveRegistry &moveRegistry, const LearnsetMeta &learnsetMeta,
																	   const PokemonEncounter &encounterData,
																	   const us pokemonEncounterLevel);

	/*!
		@since 0.12.51
		@version 0.12.51
	*/
	ATTR_NODISCARD std::expected<void, PokemonError> getValidPokemonIVs(const std::span<const us> ivs,
																		std::array<us, POKEMON_STAT_COUNT> &validIVs);

	/*!
		@since 0.12.51
		@version 0.12.51
	*/
	ATTR_NODISCARD std::expected<void, PokemonError> getValidPokemonEVs(const std::span<const us> evs,
																		std::array<us, POKEMON_STAT_COUNT> &validEVs);

	/*! @brief Generates the moves available to a Pokemon from its learnset and encounter.
		@details Selects level-up moves whose requirements do not exceed @p pokemonEncounterLevel. For egg encounters, eligible egg moves
		are also selected and take precedence over level-up entries with the same ID. Duplicate entries are ignored, selected moves are
		placed at the start of @p moveIDs, and unused slots are filled with @ref PocketCore::Move::NO_MOVE_ID. Every resulting ID is
		validated against @p moveRegistry.
		@param[out] moveIDs The fixed-size array receiving generated moves and empty-slot sentinels.
		@param[in] moveRegistry The registry used to verify generated move metadata.
		@param[in] learnsetMeta The learnset entries from which eligible moves are selected.
		@param[in] encounterData The encounter origin, which determines whether egg moves are eligible.
		@param[in] pokemonEncounterLevel The Pokemon's encounter level; moves requiring a higher level are excluded.
		@return Success when all generated move IDs have registered metadata; otherwise, @ref
	   PocketCore::Validation::Move::MoveError::MISSING_MOVE_METADATA.
		@note For @p n learnset entries, selection takes O(n log n) time and O(n) auxiliary space.
		@since 0.12.51
		@version 0.12.51
	*/
	ATTR_NODISCARD std::expected<void, MoveError> fillWithValidMoves(std::array<MoveID, MAX_MOVES_PER_POKEMON> &moveIDs,
																	 const MoveRegistry &moveRegistry, const LearnsetMeta &learnsetMeta,
																	 const PokemonEncounter &encounterData, const us pokemonEncounterLevel);
} // namespace PocketCore::Pokemon

#endif
