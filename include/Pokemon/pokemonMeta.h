/*! @file pokemonMeta.h
	@brief Defines the metadata stored for built-in and user-defined pokemons.
	@date 10/09/2026
	@since 0.11.6
	@version 0.12.51
	@author Matthew Moore
*/

#ifndef INCLUDE_POKEMON_POKEMONMETA_H
#define INCLUDE_POKEMON_POKEMONMETA_H

#include <array>
#include <string>

#include "Ability/abilityID.h"
#include "Configuration/constants.h"
#include "Core/typedefs.h"
#include "Item/itemID.h"
#include "Learnset/learnsetID.h"
#include "Location/locationID.h"
#include "Move/moveID.h"
#include "Types/typeID.h"

#include "pokemonEncounter.h"
#include "pokemonID.h"
#include "pokemonStats.h"

namespace PocketCore::Pokemon
{
	using PocketCore::Ability::AbilityID;
	using PocketCore::Configuration::MAX_ABILITY_POOL_PER_POKEMON;
	using PocketCore::Configuration::MAX_ENCOUNTERS_PER_POKEMON;
	using PocketCore::Configuration::MAX_MOVES_PER_POKEMON;
	using PocketCore::Configuration::MAX_TYPES_PER_POKEMON;
	using PocketCore::Core::ub;
	using PocketCore::Core::us;
	using PocketCore::Item::ItemID;
	using PocketCore::Item::NO_ITEM_ID;
	using PocketCore::Learnset::LearnsetID;
	using PocketCore::Location::LocationID;
	using PocketCore::Move::MoveID;
	using PocketCore::Type::TypeID;

	/*! @struct PokemonMeta Pokemon/pokemonMeta.h
		@brief Stores shared species or form metadata and species-specific encounter definitions.
		@details Owns its display name and encounter array. Encounter entries reference shared location identities while defining their own
			acquisition levels and origins. Individual current levels and acquisition histories are not stored here.
		@date 10/09/2026
		@since 0.11.6
		@version 0.12.51
		@author Matthew Moore
	*/
	struct PokemonMeta
	{
		public:
			/*! @brief Compares two PokemonMeta instances for equivalent metadata.
				@details Compares all fields exactly.
				@param[in] other The PokemonMeta instance to compare.
				@return True when both instances contain equivalent metadata; otherwise false.
				@since 0.12.19
				@version 0.12.19
			*/
			ATTR_NODISCARD constexpr bool operator==(const PokemonMeta &other) const noexcept = default;

			// NOLINTBEGIN(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)

			/*! @brief The case-sensitive display name stored. */
			std::string mName{};

			/*! @brief The Pokemon's pool of available abilities. */
			std::array<AbilityID, MAX_ABILITY_POOL_PER_POKEMON> mAbilityPool{};

			/*! @brief The species-specific encounters; entries with NO_LOCATION_ID are unused slots. */
			std::array<PokemonEncounter, MAX_ENCOUNTERS_PER_POKEMON> mEncounters{};

			/*! @brief The Pokemon's base stats. */
			PokemonStats mBaseStats{};

			/*! @brief The Pokemon's type IDs. */
			std::array<TypeID, MAX_TYPES_PER_POKEMON> mTypeIDs{};

			/*! @brief The stable built-in or user-assigned identifier. */
			PokemonID mPokemonID{};

			/*! @brief The learnset ID associated with the Pokemon. */
			LearnsetID mLearnsetID{};

			// NOLINTEND(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)
	};
} // namespace PocketCore::Pokemon

#endif
