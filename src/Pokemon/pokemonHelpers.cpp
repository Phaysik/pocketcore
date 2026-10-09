/*! @file pokemonHelpers.cpp
	@brief Contains the function definitions for creating a Pokemon
	@date 10/09/2026
	@since 0.12.28
	@version 0.12.51
	@author Matthew Moore
*/

#include "Pokemon/pokemonHelpers.h"

#include <array>
#include <cstddef>
#include <expected>
#include <optional>
#include <ostream>
#include <span>
#include <string_view>

#include "Configuration/constants.h"
#include "Core/typedefs.h"
#include "Learnset/learnsetMeta.h"
#include "Nature/natureID.h"
#include "Pokemon/pokemon.h"
#include "Pokemon/pokemonMeta.h"
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
	using PocketCore::Configuration::MAX_ABILITIES_PER_POKEMON;
	using PocketCore::Configuration::MAX_ITEMS_PER_POKEMON;
	using PocketCore::Configuration::MAX_MOVES_PER_POKEMON;
	using PocketCore::Configuration::MAX_NATURES_PER_POKEMON;
	using PocketCore::Core::us;
	using PocketCore::Learnset::LearnsetMeta;
	using PocketCore::Nature::NatureID;
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
										const std::string_view &indentation, const StableID stableID, const NameLookup &nameLookup) {
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

		for (std::size_t index{0}; index < pokemon.getTypeIDsArray().size(); ++index)
		{
			const TypeID typeID{pokemon.getTypeID(static_cast<ub>(index))};
			outStream << "    [" << index << "]:\n";

			printIDAndName("      ", typeID, [&registryProvider](const TypeID identifier) {
				return registryProvider.mTypeRegistry != nullptr ? registryProvider.mTypeRegistry->getTypeName(identifier) : std::nullopt;
			});

			outStream << '\n';
		}

		outStream << "  Natures:\n";

		for (std::size_t index{0}; index < pokemon.getNatureIDsArray().size(); ++index)
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

		for (std::size_t index{0}; index < pokemon.getAbilityIDsArray().size(); ++index)
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

		for (std::size_t index{0}; index < pokemon.getItemsIDsArray().size(); ++index)
		{
			const ItemID itemID{pokemon.getItemID(static_cast<ub>(index))};
			outStream << "    [" << index << "]:\n";

			printIDAndName("      ", itemID, [&registryProvider](const ItemID identifier) {
				return registryProvider.mItemRegistry != nullptr ? registryProvider.mItemRegistry->getItemName(identifier) : std::nullopt;
			});

			outStream << '\n';
		}

		outStream << "  Non-Volatile Statuses:\n";

		for (std::size_t index{0}; index < pokemon.getStatusIDsArray().size(); ++index)
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

		for (std::size_t index{0}; index < pokemon.getMoveIDsArray().size(); ++index)
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

	std::expected<void, NatureError> getValidPokemonNatures(const std::span<const NatureID, MAX_NATURES_PER_POKEMON> *natureIDs,
															std::array<NatureID, MAX_NATURES_PER_POKEMON> &validNatureIDs,
															const NatureRegistry &natureRegistry)
	{
		if (natureIDs != nullptr)
		{
			if (const std::expected<void, NatureError> error{validateNatureMetadata(natureIDs, natureRegistry)}; !error.has_value())
			{
				return std::unexpected{error.error()};
			}

			std::ranges::transform(*natureIDs, validNatureIDs.begin(), [](const NatureID natureID) { return natureID; });
		}
		else
		{
		}

		return {};
	}

	std::expected<void, AbilityError> getValidPokemonAbilities(const std::span<const AbilityID, MAX_ABILITIES_PER_POKEMON> *abilityIDs,
															   std::array<AbilityID, MAX_ABILITIES_PER_POKEMON> &validAbilityIDs,
															   const AbilityRegistry &abilityRegistry)
	{
		if (abilityIDs != nullptr)
		{
			if (const std::expected<void, AbilityError> error{validateAbilityMetadata(abilityIDs, abilityRegistry)}; !error.has_value())
			{
				return std::unexpected{error.error()};
			}

			std::ranges::transform(*abilityIDs, validAbilityIDs.begin(), [](const AbilityID abilityID) { return abilityID; });
		}
		else
		{
		}

		return {};
	}

	std::expected<void, ItemError> getValidPokemonItems(const std::span<const ItemID, MAX_ITEMS_PER_POKEMON> *itemIDs,
														std::array<ItemID, MAX_ITEMS_PER_POKEMON> &validItemIDs,
														const ItemRegistry &itemRegistry)
	{
		if (itemIDs != nullptr)
		{
			if (const std::expected<void, ItemError> error{validateItemMetadata(itemIDs, itemRegistry)}; !error.has_value())
			{
				return std::unexpected{error.error()};
			}

			std::ranges::transform(*itemIDs, validItemIDs.begin(), [](const ItemID itemID) { return itemID; });
		}
		else
		{
		}

		return {};
	}

	std::expected<void, MoveError> getValidPokemonMoves(const std::span<const MoveID, MAX_MOVES_PER_POKEMON> *moveIDs,
														std::array<MoveID, MAX_MOVES_PER_POKEMON> &validMoveIDs,
														const MoveRegistry &moveRegistry, const LearnsetMeta &learnsetMeta,
														const PokemonMeta &pokemonMeta)
	{
		if (moveIDs != nullptr)
		{
			if (const std::expected<void, MoveError> error{validateMoveMetadata(moveIDs, moveRegistry)}; !error.has_value())
			{
				return std::unexpected{error.error()};
			}

			std::ranges::transform(*moveIDs, validMoveIDs.begin(), [](const MoveID moveID) { return moveID; });
		}
		else
		{
			if (learnsetMeta.mName == "test" && pokemonMeta.mName == "test")
			{
			}
		}

		return {};
	}

	std::expected<void, PokemonError> getValidPokemonIVs(const std::span<const us, POKEMON_STAT_COUNT> *ivs,
														 std::array<us, POKEMON_STAT_COUNT> &validIVs)
	{
		if (ivs != nullptr)
		{
			if (const std::expected<void, PokemonError> error{isValidPokemonIVArray(*ivs)}; !error.has_value())
			{
				return std::unexpected{error.error()};
			}

			std::ranges::transform(*ivs, validIVs.begin(), [](const us pokemonIV) { return pokemonIV; });
		}
		else
		{
		}
		return {};
	}

	std::expected<void, PokemonError> getValidPokemonEVs(const std::span<const us, POKEMON_STAT_COUNT> *evs,
														 std::array<us, POKEMON_STAT_COUNT> &validEVs)
	{
		if (evs != nullptr)
		{
			if (const std::expected<void, PokemonError> error{isValidPokemonEVArray(*evs)}; !error.has_value())
			{
				return std::unexpected{error.error()};
			}

			std::ranges::transform(*evs, validEVs.begin(), [](const us pokemonEV) { return pokemonEV; });
		}
		else
		{
		}

		return {};
	}
} // namespace PocketCore::Pokemon
