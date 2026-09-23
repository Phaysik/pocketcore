/*! @file pokemon.cpp
	@brief Contains the function definitions for creating a Pokemon
	@date 09/22/2026
	@since 0.3.0
	@version 0.12.42
	@author Matthew Moore
*/

#include "Pokemon/pokemon.h"

#include <cstddef>
#include <ostream>
#include <string_view>

#include "Pokemon/pokemonMeta.h"

namespace PocketCore::Pokemon
{
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

		for (std::size_t index{0}; index < pokemon.getTypeIDsArray().size(); ++index)
		{
			outStream << (index == 0U ? "" : ", ") << pokemon.getTypeID(static_cast<ub>(index)).getValue();
		}

		outStream << "]\n  Nature IDs: [";

		for (std::size_t index{0}; index < pokemon.getNatureIDsArray().size(); ++index)
		{
			outStream << (index == 0U ? "" : ", ") << pokemon.getNatureID(static_cast<ub>(index)).getValue();
		}

		outStream << "]\n  Ability IDs: [";

		for (std::size_t index{0}; index < pokemon.getAbilityIDsArray().size(); ++index)
		{
			outStream << (index == 0U ? "" : ", ") << pokemon.getAbilityID(static_cast<ub>(index)).getValue();
		}

		outStream << "]\n  Item IDs: [";

		for (std::size_t index{0}; index < pokemon.getItemsIDsArray().size(); ++index)
		{
			outStream << (index == 0U ? "" : ", ") << pokemon.getItemID(static_cast<ub>(index)).getValue();
		}

		outStream << "]\n  Status IDs: [";

		for (std::size_t index{0}; index < pokemon.getStatusIDsArray().size(); ++index)
		{
			outStream << (index == 0U ? "" : ", ") << pokemon.getStatusID(static_cast<us>(index)).getValue();
		}

		outStream << "]\n  Moves:\n";

		for (std::size_t index{0}; index < pokemon.getMoveIDsArray().size(); ++index)
		{
			const auto moveSlotIndex{static_cast<ub>(index)};
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
