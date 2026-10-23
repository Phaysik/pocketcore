/*! @file pokemonHelpers.h
	@brief Houses free functions that aide in handling pokemon.
	@date 10/23/2026
	@since 0.12.28
	@version 0.12.53
	@author Matthew Moore
*/

#ifndef INCLUDE_POKEMON_POKEMON_HELPERS_H
#define INCLUDE_POKEMON_POKEMON_HELPERS_H

#include <algorithm>
#include <array>
#include <concepts>
#include <expected>
#include <span>
#include <unordered_set>

#include "Ability/abilityID.h"
#include "Battle/battleRandomSources.h"
#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Item/itemID.h"
#include "Learnset/learnsetMeta.h"
#include "Move/builtInMoveID.h"
#include "Move/moveID.h"
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
	using PocketCore::Ability::AbilityID;
	using PocketCore::Battle::BattleRandomSources;
	using PocketCore::Configuration::MAX_ABILITIES_PER_POKEMON;
	using PocketCore::Configuration::MAX_ITEMS_PER_POKEMON;
	using PocketCore::Configuration::MAX_MOVES_PER_POKEMON;
	using PocketCore::Configuration::MAX_NATURES_PER_POKEMON;
	using PocketCore::Item::ItemID;
	using PocketCore::Learnset::LearnsetMeta;
	using PocketCore::Move::MoveID;
	using PocketCore::Move::NO_MOVE_ID;
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
		@version 0.12.51
	*/
	std::ostream &printPokemonWithNames(std::ostream &outStream, const Pokemon &pokemon, const RegistryProvider &registryProvider);

	/*! @brief Removes duplicate non-sentinel IDs and moves empty slots to the end of the array.
		@details Preserves the first occurrence of each non-sentinel ID, replaces later duplicates with @p noID, and compacts
		the retained IDs to the front, filling the remaining slots with @p noID. Retained IDs keep their original relative order, but their
	   positions may change. Existing sentinel entries also move to the end. Does not validate IDs against a registry.
		@tparam ID One of @ref PocketCore::Item::ItemID, @ref PocketCore::Ability::AbilityID, @ref PocketCore::Move::MoveID, or
		@ref PocketCore::Nature::NatureID.
		@tparam N The number of slots in the array; zero is supported.
		@param[in] noID The sentinel representing an empty slot for the selected ID type.
		@param[in,out] validIDs The caller-owned array modified in place to contain unique non-sentinel IDs followed by sentinel entries;
		no reference to it is retained after the call.
		@since 0.12.51
		@version 0.12.51
	*/
	template <typename ID, std::size_t N>
		requires std::same_as<ID, ItemID> || std::same_as<ID, AbilityID> || std::same_as<ID, MoveID> || std::same_as<ID, NatureID>
	void replaceDuplicatesWithNoID(const ID noID, std::array<ID, N> &validIDs)
	{
		// Remove duplicate entries
		std::unordered_set<ID> seenIDs;

		std::ranges::replace_if(
			validIDs, [&seenIDs, &noID](const ID elemID) { return elemID != noID && !seenIDs.insert(elemID).second; }, noID);

		const auto emptySlots{std::ranges::remove(validIDs, noID)};
		std::ranges::fill(emptySlots, noID);
	}

	/*! @brief Validates supplied nature IDs and removes duplicates from the destination slots.
		@details Nonempty input is validated against @p natureRegistry before being copied to the destination prefix. Remaining slots
		 retain their previous contents before @ref replaceDuplicatesWithNoID removes duplicates and compacts empty slots. Empty input
		 fills every destination slot with a distinct registered ID other than @ref PocketCore::Nature::NO_NATURE_ID using
		 @ref PocketCore::Nature::getRandomNatureIDFromConfiguration, excluding IDs selected for earlier slots.
		@pre natureIDs.size() <= MAX_NATURES_PER_POKEMON; the caller enforces this bound.
		@param[in] natureIDs Read-only, non-owning span borrowed for this call; no reference is retained.
		@param[in,out] validNatureIDs Caller-owned destination array; unused slots should initially contain
		 @ref PocketCore::Nature::NO_NATURE_ID. No reference is retained.
		@param[in] natureRegistry Registry borrowed for metadata validation; no reference is retained.
		@param[in] randomSources The random sources used for battle-related numeric types.
		@return Success after validation or random selection; otherwise, the metadata-validation error,
		 @ref NatureError::NO_REGISTERED_NATURES, or @ref NatureError::NOT_ENOUGH_REGISTERED_NATURES.
		 Validation and candidate-count failures leave the array unchanged.
		@since 0.12.51
		@version 0.12.53
	*/
	ATTR_NODISCARD std::expected<void, NatureError> getValidPokemonNatures(const std::span<const NatureID> natureIDs,
																		   std::array<NatureID, MAX_NATURES_PER_POKEMON> &validNatureIDs,
																		   const NatureRegistry &natureRegistry,
																		   const BattleRandomSources &randomSources);

	/*!
		@since 0.12.51
		@version 0.12.53
	*/
	ATTR_NODISCARD std::expected<void, AbilityError> getValidPokemonAbilities(
		const std::span<const AbilityID> abilityIDs, std::array<AbilityID, MAX_ABILITIES_PER_POKEMON> &validAbilityIDs,
		const AbilityRegistry &abilityRegistry, const BattleRandomSources &randomSources);

	/*!
		@since 0.12.51
		@version 0.12.51
	*/
	ATTR_NODISCARD std::expected<void, ItemError> getValidPokemonItems(const std::span<const ItemID> itemIDs,
																	   std::array<ItemID, MAX_ITEMS_PER_POKEMON> &validItemIDs,
																	   const ItemRegistry &itemRegistry);

	/*! @brief Validates supplied move IDs or generates moves from a learnset and encounter.
		@details Nonempty input is validated against @p moveRegistry and copied to the destination prefix without checking learnset
		 eligibility. @ref replaceDuplicatesWithNoID then preserves the first occurrence of each ID other than
		 @p duplicateEntryMoveReplacer, replaces later duplicates with that ID, and moves replacement entries to the end while preserving
		 the relative order of retained moves. The replacement ID must have registered metadata even when no duplicates occur.
		 Empty input delegates to @ref fillWithValidMoves, selecting eligible level-up moves and, for egg encounters, eligible egg moves.
		 The replacement ID is neither checked nor used when generating moves.
		@pre moveIDs.size() <= MAX_MOVES_PER_POKEMON; the caller enforces this bound.
		@param[in] moveIDs Read-only, non-owning span of supplied IDs; an empty span requests generation. Borrowed only for this call.
		@param[in,out] validMoveIDs Caller-owned destination. Supplied IDs overwrite its prefix; remaining slots retain their prior
		 contents before duplicate compaction. Unused slots should initially contain @p duplicateEntryMoveReplacer. Generation overwrites
		 the entire array and fills unused slots with @ref PocketCore::Move::NO_MOVE_ID. No reference is retained.
		@param[in] moveRegistry Registry borrowed to validate supplied or generated IDs and the replacement ID; no reference is retained.
		@param[in] learnsetMeta Learnset borrowed only for generation; no reference is retained.
		@param[in] encounterData Encounter origin borrowed only for generation; egg encounters permit egg moves. No reference is retained.
		@param[in] pokemonEncounterLevel Encounter-level cap applied to generated level-up and egg moves; ignored for supplied IDs.
		@param[in] duplicateEntryMoveReplacer Registered ID used as the duplicate-replacement and compaction sentinel for supplied moves;
		 defaults to @ref PocketCore::Move::NO_MOVE_ID and is ignored for generation.
		@return Success when validation or generation succeeds; otherwise,
		 @ref PocketCore::Validation::Move::MoveError::MISSING_MOVE_METADATA for an unregistered supplied or generated move, or
		 @ref PocketCore::Validation::Move::MoveError::DUPLICATE_REPLACEMENT_MISSING_METADATA for an unregistered replacement ID.
		@note Invalid supplied metadata leaves the destination unchanged. A missing replacement ID is reported after the supplied prefix
		 has been copied. Failed generation clears the entire destination to @ref PocketCore::Move::NO_MOVE_ID.
		@since 0.12.51
		@version 0.12.52
	*/
	ATTR_NODISCARD std::expected<void, MoveError> getValidPokemonMoves(const std::span<const MoveID> moveIDs,
																	   std::array<MoveID, MAX_MOVES_PER_POKEMON> &validMoveIDs,
																	   const MoveRegistry &moveRegistry, const LearnsetMeta &learnsetMeta,
																	   const PokemonEncounter &encounterData,
																	   const us pokemonEncounterLevel,
																	   const MoveID duplicateEntryMoveReplacer = NO_MOVE_ID);

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
