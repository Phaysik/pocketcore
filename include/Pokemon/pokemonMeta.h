/*! @file pokemonMeta.h
	@brief Defines the metadata stored for built-in and user-defined pokemons.
	@date 09/11/2026
	@since 0.11.6
	@version 0.12.23
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
#include "Move/moveID.h"
#include "Types/typeID.h"

#include "pokemonID.h"

namespace PocketCore::Pokemon
{
	using PocketCore::Ability::AbilityID;
	using PocketCore::Configuration::MAX_ABILITY_POOL_PER_POKEMON;
	using PocketCore::Configuration::MAX_MOVES_PER_POKEMON;
	using PocketCore::Configuration::MAX_TYPES_PER_POKEMON;
	using PocketCore::Core::ub;
	using PocketCore::Core::us;
	using PocketCore::Item::ItemID;
	using PocketCore::Item::NO_ITEM_ID;
	using PocketCore::Move::MoveID;
	using PocketCore::Type::TypeID;

	/*! @enum PokemonStat
		@brief Identifies each Pokemon stat and its position in stat-indexed arrays.
		@note All enum values except @ref PokemonStat::Count must be handled exhaustively when processing Pokemon stats.
		@date 09/11/2026
		@since 0.12.23
		@version 0.12.23
		@author Matthew Moore
	*/
	enum class PokemonStat : ub
	{
		Health,			/*! @brief Identifies the maximum health stat. */
		Attack,			/*! @brief Identifies the physical Attack stat. */
		Defense,		/*! @brief Identifies the physical Defense stat. */
		SpecialAttack,	/*! @brief Identifies the Special Attack stat. */
		SpecialDefense, /*! @brief Identifies the Special Defense stat. */
		Speed,			/*! @brief Identifies the Speed stat. */
		Count			/*! @brief Provides the number of usable stat values and is not itself a stat. */
	};

	/*! @brief Converts a Pokemon stat identifier to its zero-based array index.
		@param[in] stat The stat identifier to convert; @ref PokemonStat::Count produces the stat-array size.
		@return The zero-based index represented by @p stat.
		@note This constexpr conversion supports both compile-time and runtime stat indexing and is no-throw.
		@since 0.12.23
		@version 0.12.23
	*/
	constexpr std::size_t toIndex(const PokemonStat stat) noexcept
	{
		return static_cast<std::size_t>(stat);
	}

	/*! @brief Provides the number of elements required by arrays indexed with @ref PokemonStat. */
	inline constexpr std::size_t POKEMON_STAT_COUNT{toIndex(PokemonStat::Count)};

	/*! @struct PokemonStats Pokemon/pokemonMeta.h
		@brief Stores one pokemon's base stats.
		@details This struct contains the base stats for a pokemon, including health, attack, defense, special attack, special defense, and
	   speed.
		@date 09/11/2026
		@since 0.12.22
		@version 0.12.23
		@author Matthew Moore
	*/
	struct PokemonStats
	{
		public:
			/*! @brief Compares two PokemonStats instances for equivalent metadata.
				@details Compares all fields exactly.
				@param[in] other The PokemonStats instance to compare.
				@return True when both instances contain equivalent metadata; otherwise false.
				@since 0.12.22
				@version 0.12.23
			*/
			ATTR_NODISCARD constexpr bool operator==(const PokemonStats &other) const noexcept = default;

			// NOLINTBEGIN(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)

			/*! @brief The Pokemon's  max health stat. */
			us mMaxHealth{};

			/*! @brief The Pokemon's  attack stat. */
			us mAttack{};

			/*! @brief The Pokemon's  defense stat. */
			us mDefense{};

			/*! @brief The Pokemon's  special attack stat. */
			us mSpAttack{};

			/*! @brief The Pokemon's  special defense stat. */
			us mSpDefense{};

			/*! @brief The Pokemon's  speed stat. */
			us mSpeed{};

			// NOLINTEND(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)
	};

	/*! @struct PokemonMeta Pokemon/pokemonMeta.h
		@brief Stores one pokemon's stable ID, display name, and owned trigger definitions.
		@details The trigger vector owns its elements and their effect vectors. The display name is a non-owning view whose backing storage
	   must remain valid while this metadata is registered.
		@date 09/11/2026
		@since 0.11.6
		@version 0.12.22
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

			/*! @brief The Pokemon's base stats. */
			PokemonStats mBaseStats{};

			/*! @brief The Pokemon's move IDs. */
			std::array<MoveID, MAX_MOVES_PER_POKEMON> mMoveIDs{};

			/*! @brief The Pokemon's max PP values. */
			std::array<ub, MAX_MOVES_PER_POKEMON> mMaxPP{};

			/*! @brief The Pokemon's type IDs. */
			std::array<TypeID, MAX_TYPES_PER_POKEMON> mTypeIDs{};

			/*! @brief The Pokemon's level. */
			us mLevel{};

			/*! @brief The Pokemon's item ID. */
			ItemID mItemID{NO_ITEM_ID};

			/*! @brief The stable built-in or user-assigned identifier. */
			PokemonID mPokemonID{};

			/*! @brief The amount of abilities in the Pokemon's ability pool that are valid and not empty data. */
			ub mAbilityPoolCount{};

			// NOLINTEND(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)
	};
} // namespace PocketCore::Pokemon

#endif
