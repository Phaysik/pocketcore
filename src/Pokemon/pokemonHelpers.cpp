/*! @file pokemonHelpers.cpp
	@brief Contains the function definitions for creating a Pokemon
	@date 10/20/2026
	@since 0.12.28
	@version 0.12.51
	@author Matthew Moore
*/

#include "Pokemon/pokemonHelpers.h"

#include <algorithm>
#include <array>
#include <cstddef>
#include <expected>
#include <functional>
#include <optional>
#include <ostream>
#include <ranges>
#include <span>
#include <string_view>
#include <unordered_set>
#include <vector>

#include "Ability/abilityID.h"
#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Item/itemID.h"
#include "Learnset/learnsetMeta.h"
#include "Move/builtInMoveID.h"
#include "Nature/natureID.h"
#include "Pokemon/pokemon.h"
#include "Pokemon/pokemonEncounter.h"
#include "Pokemon/pokemonMeta.h"
#include "Pokemon/pokemonStats.h"
#include "Registry/moveRegistry.h"
#include "Registry/natureRegistry.h"
#include "Registry/registryProvider.h"
#include "Validation/Ability/abilityError.h"
#include "Validation/Ability/abilityValidation.h"
#include "Validation/Item/itemError.h"
#include "Validation/Item/itemValidation.h"
#include "Validation/Move/moveError.h"
#include "Validation/Move/moveValidation.h"
#include "Validation/Nature/natureError.h"
#include "Validation/Nature/natureValidation.h"
#include "Validation/Pokemon/pokemonError.h"
#include "Validation/Pokemon/pokemonValidation.h"

namespace PocketCore::Pokemon
{
	using PocketCore::Ability::NO_ABILITY_ID;
	using PocketCore::Configuration::MAX_ABILITIES_PER_POKEMON;
	using PocketCore::Configuration::MAX_ITEMS_PER_POKEMON;
	using PocketCore::Configuration::MAX_MOVES_PER_POKEMON;
	using PocketCore::Configuration::MAX_NATURES_PER_POKEMON;
	using PocketCore::Core::us;
	using PocketCore::Item::NO_ITEM_ID;
	using PocketCore::Learnset::AcquisitionMethod;
	using PocketCore::Learnset::LearnsetMeta;
	using PocketCore::Learnset::MoveLearnEntry;
	using PocketCore::Move::NO_MOVE_ID;
	using PocketCore::Nature::NatureID;
	using PocketCore::Nature::NO_NATURE_ID;
	using PocketCore::Registry::Nature::NatureRegistry;
	using PocketCore::Registry::RegistryProvider;
	using PocketCore::Validation::Ability::AbilityError;
	using PocketCore::Validation::Ability::validateAbilityMetadata;
	using PocketCore::Validation::Item::ItemError;
	using PocketCore::Validation::Item::validateItemMetadata;
	using PocketCore::Validation::Move::MoveError;
	using PocketCore::Validation::Move::validateMoveMetadata;
	using PocketCore::Validation::Nature::NatureError;
	using PocketCore::Validation::Nature::validateNatureMetadata;
	using PocketCore::Validation::Pokemon::PokemonError;
	using Validation::Pokemon::isValidPokemonEVArray;
	using Validation::Pokemon::isValidPokemonIVArray;

#if ATTR_ONLY_GCC
	// GCC suggests returns_nonnull for references even though the attribute accepts only pointer returns.
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wsuggest-attribute=returns_nonnull"
#endif

	std::ostream &printPokemonWithNames(std::ostream &outStream, const Pokemon &pokemon, const RegistryProvider &registryProvider)
	{
		const auto printIDAndName = [&outStream]<typename StableID, typename NameLookup>(
										const std::string_view indentation, const StableID stableID, const NameLookup &nameLookup) {
			constexpr std::string_view unregisteredName{"<unregistered>"};
			const std::optional<std::string_view> name{nameLookup(stableID)};

			outStream << indentation << "ID: " << stableID.getValue() << '\n' << indentation << "Name: " << name.value_or(unregisteredName);
		};

		outStream << "Pokemon {\n"
				  << "  Name: " << pokemon.getName() << '\n'
				  << "  Level: " << pokemon.getLevel() << '\n'
				  << "  Level Damage Factor: " << pokemon.getLevelDamageFactor() << '\n'
				  << "  Health: " << pokemon.getHealth() << '/' << pokemon.getMaximumHealth() << '\n'
				  << "    IV: " << pokemon.getPokemonIV(toIndex(PokemonStat::Health)) << '\n'
				  << "    EV: " << pokemon.getPokemonEV(toIndex(PokemonStat::Health)) << '\n'
				  << "  Attack: " << pokemon.getAttack() << '\n'
				  << "    IV: " << pokemon.getPokemonIV(toIndex(PokemonStat::Attack)) << '\n'
				  << "    EV: " << pokemon.getPokemonEV(toIndex(PokemonStat::Attack)) << '\n'
				  << "  Defense: " << pokemon.getDefense() << '\n'
				  << "    IV: " << pokemon.getPokemonIV(toIndex(PokemonStat::Defense)) << '\n'
				  << "    EV: " << pokemon.getPokemonEV(toIndex(PokemonStat::Defense)) << '\n'
				  << "  Special Attack: " << pokemon.getSpAttack() << '\n'
				  << "    IV: " << pokemon.getPokemonIV(toIndex(PokemonStat::SpecialAttack)) << '\n'
				  << "    EV: " << pokemon.getPokemonEV(toIndex(PokemonStat::SpecialAttack)) << '\n'
				  << "  Special Defense: " << pokemon.getSpDefense() << '\n'
				  << "    IV: " << pokemon.getPokemonIV(toIndex(PokemonStat::SpecialDefense)) << '\n'
				  << "    EV: " << pokemon.getPokemonEV(toIndex(PokemonStat::SpecialDefense)) << '\n'
				  << "  Speed: " << pokemon.getSpeed() << '\n'
				  << "    IV: " << pokemon.getPokemonIV(toIndex(PokemonStat::Speed)) << '\n'
				  << "    EV: " << pokemon.getPokemonEV(toIndex(PokemonStat::Speed)) << '\n';

		outStream << "  Types:\n";

		for (const std::size_t index : std::views::iota(std::size_t{0}, pokemon.getTypeIDsArray().size()))
		{
			const TypeID typeID{pokemon.getTypeID(static_cast<ub>(index))};
			outStream << "    [" << index << "]:\n";

			printIDAndName("      ", typeID, [&registryProvider](const TypeID identifier) {
				return registryProvider.mTypeRegistry != nullptr ? registryProvider.mTypeRegistry->getTypeName(identifier) : std::nullopt;
			});

			outStream << '\n';
		}

		outStream << "  Natures:\n";

		for (const std::size_t index : std::views::iota(std::size_t{0}, pokemon.getNatureIDsArray().size()))
		{
			const NatureID natureID{pokemon.getNatureID(static_cast<ub>(index))};
			outStream << "    [" << index << "]:\n";

			printIDAndName("      ", natureID, [&registryProvider](const NatureID identifier) {
				return registryProvider.mNatureRegistry != nullptr ? registryProvider.mNatureRegistry->getNatureName(identifier)
																   : std::nullopt;
			});

			outStream << '\n';
		}

		outStream << "  Abilities:\n";

		for (const std::size_t index : std::views::iota(std::size_t{0}, pokemon.getAbilityIDsArray().size()))
		{
			const AbilityID abilityID{pokemon.getAbilityID(static_cast<ub>(index))};
			outStream << "    [" << index << "]:\n";

			printIDAndName("      ", abilityID, [&registryProvider](const AbilityID identifier) {
				return registryProvider.mAbilityRegistry != nullptr ? registryProvider.mAbilityRegistry->getAbilityName(identifier)
																	: std::nullopt;
			});

			outStream << '\n';
		}

		outStream << "  Items:\n";

		for (const std::size_t index : std::views::iota(std::size_t{0}, pokemon.getItemsIDsArray().size()))
		{
			const ItemID itemID{pokemon.getItemID(static_cast<ub>(index))};
			outStream << "    [" << index << "]:\n";

			printIDAndName("      ", itemID, [&registryProvider](const ItemID identifier) {
				return registryProvider.mItemRegistry != nullptr ? registryProvider.mItemRegistry->getItemName(identifier) : std::nullopt;
			});

			outStream << '\n';
		}

		outStream << "  Non-Volatile Statuses:\n";

		for (const std::size_t index : std::views::iota(std::size_t{0}, pokemon.getStatusIDsArray().size()))
		{
			const StatusID statusID{pokemon.getStatusID(static_cast<us>(index))};
			outStream << "    [" << index << "]:\n";

			printIDAndName("      ", statusID, [&registryProvider](const StatusID identifier) {
				return registryProvider.mStatusRegistry != nullptr ? registryProvider.mStatusRegistry->getStatusName(identifier)
																   : std::nullopt;
			});

			outStream << '\n';
		}

		outStream << "  Moves:\n";

		for (const std::size_t index : std::views::iota(std::size_t{0}, pokemon.getMoveIDsArray().size()))
		{
			const auto moveSlotIndex{static_cast<ub>(index)};
			const MoveID moveID{pokemon.getMoveID(moveSlotIndex)};
			outStream << "    [" << index << "]:\n";

			printIDAndName("      ", moveID, [&registryProvider](const MoveID identifier) {
				return registryProvider.mMoveRegistry != nullptr ? registryProvider.mMoveRegistry->getMoveName(identifier) : std::nullopt;
			});

			outStream << "\n      PP: " << static_cast<unsigned int>(pokemon.getCurrentPP(moveSlotIndex)) << '/'
					  << static_cast<unsigned int>(pokemon.getMaxPP(moveSlotIndex)) << '\n';
		}

		outStream << '}';

		return outStream;
	}
#if ATTR_ONLY_GCC
	#pragma GCC diagnostic pop
#endif

	ATTR_NODISCARD std::expected<void, NatureError> getValidPokemonNatures(const std::span<const NatureID> natureIDs,
																		   std::array<NatureID, MAX_NATURES_PER_POKEMON> &validNatureIDs,
																		   const NatureRegistry &natureRegistry)
	{
		if (!natureIDs.empty())
		{
			if (const std::expected<void, NatureError> error{validateNatureMetadata(natureIDs, natureRegistry)}; !error.has_value())
			{
				return std::unexpected{error.error()};
			}

			std::ranges::transform(natureIDs, validNatureIDs.begin(), [](const NatureID natureID) { return natureID; });

			// Remove duplicate entries
			replaceDuplicatesWithNoID(NO_NATURE_ID, validNatureIDs);
		}
		else
		{
		}

		return {};
	}

	ATTR_NODISCARD std::expected<void, AbilityError> getValidPokemonAbilities(
		const std::span<const AbilityID> abilityIDs, std::array<AbilityID, MAX_ABILITIES_PER_POKEMON> &validAbilityIDs,
		const AbilityRegistry &abilityRegistry)
	{
		if (!abilityIDs.empty())
		{
			if (const std::expected<void, AbilityError> error{validateAbilityMetadata(abilityIDs, abilityRegistry)}; !error.has_value())
			{
				return std::unexpected{error.error()};
			}

			std::ranges::transform(abilityIDs, validAbilityIDs.begin(), [](const AbilityID abilityID) { return abilityID; });

			// Remove duplicate entries
			replaceDuplicatesWithNoID(NO_ABILITY_ID, validAbilityIDs);
		}
		else
		{
		}

		return {};
	}

	ATTR_NODISCARD std::expected<void, ItemError> getValidPokemonItems(const std::span<const ItemID> itemIDs,
																	   std::array<ItemID, MAX_ITEMS_PER_POKEMON> &validItemIDs,
																	   const ItemRegistry &itemRegistry)
	{
		if (!itemIDs.empty())
		{
			if (const std::expected<void, ItemError> error{validateItemMetadata(itemIDs, itemRegistry)}; !error.has_value())
			{
				return std::unexpected{error.error()};
			}

			std::ranges::transform(itemIDs, validItemIDs.begin(), [](const ItemID itemID) { return itemID; });

			// Remove duplicate entries
			replaceDuplicatesWithNoID(NO_ITEM_ID, validItemIDs);
		}
		else
		{
		}

		return {};
	}

	ATTR_NODISCARD std::expected<void, MoveError> getValidPokemonMoves(const std::span<const MoveID> moveIDs,
																	   std::array<MoveID, MAX_MOVES_PER_POKEMON> &validMoveIDs,
																	   const MoveRegistry &moveRegistry, const LearnsetMeta &learnsetMeta,
																	   const PokemonEncounter &encounterData,
																	   const us pokemonEncounterLevel,
																	   const MoveID duplicateEntryMoveReplacer)
	{
		if (!moveIDs.empty())
		{
			if (const std::expected<void, MoveError> error{validateMoveMetadata(moveIDs, moveRegistry)}; !error.has_value())
			{
				return std::unexpected{error.error()};
			}

			std::ranges::transform(moveIDs, validMoveIDs.begin(), [](const MoveID moveID) { return moveID; });

			if (moveRegistry.getMoveMetadata(duplicateEntryMoveReplacer) == nullptr)
			{
				return std::unexpected{MoveError::DUPLICATE_REPLACEMENT_MISSING_METADATA};
			}

			// Remove duplicate entries
			replaceDuplicatesWithNoID(duplicateEntryMoveReplacer, validMoveIDs);
		}
		else if (const std::expected<void, MoveError> error{
					 fillWithValidMoves(validMoveIDs, moveRegistry, learnsetMeta, encounterData, pokemonEncounterLevel),
				 };
				 !error.has_value())
		{
			return std::unexpected{error.error()};
		}

		return {};
	}

	ATTR_NODISCARD std::expected<void, PokemonError> getValidPokemonIVs(const std::span<const us> ivs,
																		std::array<us, POKEMON_STAT_COUNT> &validIVs)
	{
		if (!ivs.empty())
		{
			if (const std::expected<void, PokemonError> error{isValidPokemonIVArray(ivs)}; !error.has_value())
			{
				return std::unexpected{error.error()};
			}

			std::ranges::transform(ivs, validIVs.begin(), [](const us pokemonIV) { return pokemonIV; });
		}
		else
		{
		}
		return {};
	}

	ATTR_NODISCARD std::expected<void, PokemonError> getValidPokemonEVs(const std::span<const us> evs,
																		std::array<us, POKEMON_STAT_COUNT> &validEVs)
	{
		if (!evs.empty())
		{
			if (const std::expected<void, PokemonError> error{isValidPokemonEVArray(evs)}; !error.has_value())
			{
				return std::unexpected{error.error()};
			}

			std::ranges::transform(evs, validEVs.begin(), [](const us pokemonEV) { return pokemonEV; });
		}
		else
		{
		}

		return {};
	}

	ATTR_NODISCARD std::expected<void, MoveError> fillWithValidMoves(std::array<MoveID, MAX_MOVES_PER_POKEMON> &moveIDs,
																	 const MoveRegistry &moveRegistry, const LearnsetMeta &learnsetMeta,
																	 const PokemonEncounter &encounterData, const us pokemonEncounterLevel)
	{
		const auto getEntriesThatMeetLevelCapByAcquisition
			= [pokemonEncounterLevel, &learnsetMeta](const AcquisitionMethod method) -> std::vector<MoveLearnEntry> {
			std::vector<MoveLearnEntry> entriesThatMeetLevelCapFilteredByMethod{
				std::ranges::to<std::vector<MoveLearnEntry>>(
					learnsetMeta.mEntries | std::views::filter([pokemonEncounterLevel, method](const MoveLearnEntry &entry) {
						return entry.mLevelRequirement <= pokemonEncounterLevel && entry.mAcquisitionMethod == method;
					})),
			};

			// Sort to have highest level moves at the front
			std::ranges::sort(entriesThatMeetLevelCapFilteredByMethod, std::ranges::greater{}, &MoveLearnEntry::mLevelRequirement);

			// Remove duplicate entries
			std::unordered_set<MoveID> seenMoveIDs;

			std::erase_if(entriesThatMeetLevelCapFilteredByMethod,
						  [&seenMoveIDs](const MoveLearnEntry &entry) { return !seenMoveIDs.insert(entry.mMoveID).second; });

			return entriesThatMeetLevelCapFilteredByMethod;
		};

		const std::vector<MoveLearnEntry> levelUpEntriesThatMeetLevelCap{
			getEntriesThatMeetLevelCapByAcquisition(AcquisitionMethod::LevelUp),
		};

		moveIDs.fill(NO_MOVE_ID);

		// Last entry goes into index 0, and so on
		std::ranges::transform(levelUpEntriesThatMeetLevelCap | std::views::take(MAX_MOVES_PER_POKEMON) | std::views::reverse,
							   moveIDs.begin(), [](const MoveLearnEntry &moveLearnEntry) { return moveLearnEntry.mMoveID; });

		if (encounterData.mOrigin == PokemonOrigin::Egg)
		{
			std::vector<MoveLearnEntry> eggMoveEntriesThatMeetLevelCap{getEntriesThatMeetLevelCapByAcquisition(AcquisitionMethod::Egg)};

			const std::vector<MoveLearnEntry> selectedEggEntries{
				std::ranges::to<std::vector<MoveLearnEntry>>(eggMoveEntriesThatMeetLevelCap | std::views::take(MAX_MOVES_PER_POKEMON)
															 | std::views::reverse),
			};

			std::ranges::replace_if(
				moveIDs,
				[&selectedEggEntries](const MoveID &moveID) {
					return std::ranges::contains(selectedEggEntries, moveID, &MoveLearnEntry::mMoveID);
				},
				NO_MOVE_ID);

			std::ranges::stable_partition(moveIDs, [](const MoveID moveID) { return moveID == NO_MOVE_ID; });
			std::ranges::transform(selectedEggEntries, moveIDs.begin(),
								   [](const MoveLearnEntry &moveLearnEntry) { return moveLearnEntry.mMoveID; });
		}

		std::ranges::stable_partition(moveIDs, [](const MoveID moveID) { return moveID != NO_MOVE_ID; });

		if (const std::expected<void, MoveError> error{validateMoveMetadata(moveIDs, moveRegistry)}; !error.has_value())
		{
			moveIDs.fill(NO_MOVE_ID);
			return std::unexpected{error.error()};
		}

		return {};
	}
} // namespace PocketCore::Pokemon
