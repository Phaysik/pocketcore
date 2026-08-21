#include "Pokemon/pokemon.h"

#include <cstddef>
#include <optional>
#include <ostream>
#include <string_view>

#include "Registry/registryProvider.h"

namespace PocketCore::Pokemon
{
#if defined(ATTR_GCC) && !defined(ATTR_CLANG)
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
				  << "  Attack: " << pokemon.getAttack() << '\n'
				  << "  Defense: " << pokemon.getDefense() << '\n'
				  << "  Special Attack: " << pokemon.getSpAttack() << '\n'
				  << "  Special Defense: " << pokemon.getSpDefense() << '\n'
				  << "  Speed: " << pokemon.getSpeed() << '\n'
				  << "  Ability ID: " << pokemon.getAbilityID().getValue() << '\n'
				  << "  Item ID: " << pokemon.getItemID().getValue() << '\n'
				  << "  Type IDs: [";

		for (std::size_t index{0}; index < pokemon.getTypesArray().size(); ++index)
		{
			outStream << (index == 0U ? "" : ", ") << pokemon.getTypeID(static_cast<ub>(index)).getValue();
		}

		outStream << "]\n  Status IDs: [";

		for (std::size_t index{0}; index < pokemon.getStatusesArray().size(); ++index)
		{
			outStream << (index == 0U ? "" : ", ") << pokemon.getStatusID(static_cast<us>(index)).getValue();
		}

		outStream << "]\n  Moves:\n";

		for (std::size_t index{0}; index < pokemon.getMovesArray().size(); ++index)
		{
			const auto moveSlotIndex{static_cast<ub>(index)};
			outStream << "    [" << index << "] ID: " << pokemon.getMoveID(moveSlotIndex).getValue()
					  << ", PP: " << static_cast<unsigned int>(pokemon.getCurrentPP(moveSlotIndex)) << '/'
					  << static_cast<unsigned int>(pokemon.getMaxPP(moveSlotIndex)) << '\n';
		}

		outStream << '}';

		return outStream;
	}

	std::ostream &printPokemonWithNames(std::ostream &outStream, const Pokemon &pokemon,
										const PocketCore::Registry::RegistryProvider &registryProvider)
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
				  << "  Attack: " << pokemon.getAttack() << '\n'
				  << "  Defense: " << pokemon.getDefense() << '\n'
				  << "  Special Attack: " << pokemon.getSpAttack() << '\n'
				  << "  Special Defense: " << pokemon.getSpDefense() << '\n'
				  << "  Speed: " << pokemon.getSpeed() << "\n  Ability:\n";

		printIDAndName("    ", pokemon.getAbilityID(), [&registryProvider](const AbilityID abilityID) {
			return registryProvider.abilityRegistry != nullptr ? registryProvider.abilityRegistry->getAbilityName(abilityID) : std::nullopt;
		});

		outStream << "\n  Item:\n";
		printIDAndName("    ", pokemon.getItemID(), [&registryProvider](const ItemID itemID) {
			return registryProvider.itemRegistry != nullptr ? registryProvider.itemRegistry->getItemName(itemID) : std::nullopt;
		});

		outStream << "\n  Types:\n";
		for (std::size_t index{0}; index < pokemon.getTypesArray().size(); ++index)
		{
			const TypeID typeID{pokemon.getTypeID(static_cast<ub>(index))};
			outStream << "    [" << index << "]:\n";
			printIDAndName("      ", typeID, [&registryProvider](const TypeID identifier) {
				return registryProvider.typeRegistry != nullptr ? registryProvider.typeRegistry->getTypeName(identifier) : std::nullopt;
			});
			outStream << '\n';
		}

		outStream << "  Statuses:\n";
		for (std::size_t index{0}; index < pokemon.getStatusesArray().size(); ++index)
		{
			const StatusID statusID{pokemon.getStatusID(static_cast<us>(index))};
			outStream << "    [" << index << "]:\n";
			printIDAndName("      ", statusID, [&registryProvider](const StatusID identifier) {
				return registryProvider.statusRegistry != nullptr ? registryProvider.statusRegistry->getStatusName(identifier)
																  : std::nullopt;
			});
			outStream << '\n';
		}

		outStream << "  Moves:\n";
		for (std::size_t index{0}; index < pokemon.getMovesArray().size(); ++index)
		{
			const auto moveSlotIndex{static_cast<ub>(index)};
			const MoveID moveID{pokemon.getMoveID(moveSlotIndex)};
			outStream << "    [" << index << "]:\n";
			printIDAndName("      ", moveID, [&registryProvider](const MoveID identifier) {
				return registryProvider.moveRegistry != nullptr ? registryProvider.moveRegistry->getMoveName(identifier) : std::nullopt;
			});
			outStream << "\n      PP: " << static_cast<unsigned int>(pokemon.getCurrentPP(moveSlotIndex)) << '/'
					  << static_cast<unsigned int>(pokemon.getMaxPP(moveSlotIndex)) << '\n';
		}

		outStream << '}';
		return outStream;
	}

#if defined(ATTR_GCC) && !defined(ATTR_CLANG)
	#pragma GCC diagnostic pop
#endif
} // namespace PocketCore::Pokemon
