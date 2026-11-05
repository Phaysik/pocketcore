/*! @file pokemon.cpp
	@brief Contains the function definitions for creating a Pokemon
	@date 10/27/2026
	@since 0.3.0
	@version 0.12.53
	@author Matthew Moore
*/

#include "Pokemon/pokemon.h"

#include <cstddef>
#include <ostream>
#include <ranges>
#include <string_view>

#include "Core/concepts.h"
#include "Core/typedefs.h"
#include "Pokemon/pokemonMeta.h"
#include "Utility/IO/Pokemon/pokemonIO.h"

namespace PocketCore::Pokemon
{
	using PocketCore::Core::InvocableWithArgs;
	using PocketCore::Core::ub;
	using PocketCore::Utility::IO::Pokemon::outputPokemonDataID;

#if ATTR_ONLY_GCC
	// GCC suggests returns_nonnull for references even though the attribute accepts only pointer returns.
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wsuggest-attribute=returns_nonnull"
#endif

	std::ostream &operator<<(std::ostream &outStream, const Pokemon &pokemon)
	{
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
				  << "    EV: " << pokemon.getPokemonEV(toIndex(PokemonStat::Speed)) << '\n'
				  << "  Type IDs: [";

		for (const std::size_t index : std::views::iota(std::size_t{0}, pokemon.getTypeIDsArray().size()))
		{
			outputPokemonDataID(outStream, [&pokemon](const ub typeIndex) { return pokemon.getTypeID(typeIndex); }, static_cast<ub>(index));
		}

		outStream << "]\n  Nature IDs: [";

		for (const std::size_t index : std::views::iota(std::size_t{0}, pokemon.getNatureIDsArray().size()))
		{
			outputPokemonDataID(
				outStream, [&pokemon](const ub natureIndex) { return pokemon.getNatureID(natureIndex); }, static_cast<ub>(index));
		}

		outStream << "]\n  Ability Entries: [";

		for (const std::size_t index : std::views::iota(std::size_t{0}, pokemon.getAbilityEntriesArray().size()))
		{
			outputPokemonDataID(
				outStream, [&pokemon](const ub abilityIndex) { return pokemon.getAbilityEntry(abilityIndex).mAbilityID; },
				static_cast<ub>(index));
		}

		outStream << "]\n  Item IDs: [";

		for (const std::size_t index : std::views::iota(std::size_t{0}, pokemon.getItemsIDsArray().size()))
		{
			outputPokemonDataID(outStream, [&pokemon](const ub itemIndex) { return pokemon.getItemID(itemIndex); }, static_cast<ub>(index));
		}

		outStream << "]\n  Non-Volatile Status IDs: [";

		for (const std::size_t index : std::views::iota(std::size_t{0}, pokemon.getStatusIDsArray().size()))
		{
			outputPokemonDataID(
				outStream, [&pokemon](const ub statusIndex) { return pokemon.getStatusID(statusIndex); }, static_cast<ub>(index));
		}

		outStream << "]\n  Moves:\n";

		for (const std::size_t index : std::views::iota(std::size_t{0}, pokemon.getMoveIDsArray().size()))
		{
			const ub moveSlotIndex{static_cast<ub>(index)};
			outStream << "    [" << index << "] ID: " << pokemon.getMoveID(moveSlotIndex).getValue()
					  << ", PP: " << static_cast<unsigned int>(pokemon.getCurrentPP(moveSlotIndex)) << '/'
					  << static_cast<unsigned int>(pokemon.getMaxPP(moveSlotIndex)) << '\n';
		}

		outStream << '}';

		return outStream;
	}

#if ATTR_ONLY_GCC
	#pragma GCC diagnostic pop
#endif
} // namespace PocketCore::Pokemon
