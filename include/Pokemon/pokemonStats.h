/*! @file pokemonMeta.h
	@brief Defines the objects that pertain to the pokemon's base stats.
	@date 10/09/2026
	@since 0.11.6
	@version 0.12.51
	@author Matthew Moore
*/

#ifndef INCLUDE_POKEMON_POKEMON_STATS_H
#define INCLUDE_POKEMON_POKEMON_STATS_H

#include <utility>

#include "Core/attributeMacros.h"
#include "Core/typedefs.h"

namespace PocketCore::Pokemon
{
	using PocketCore::Core::ub;
	using PocketCore::Core::us;

	/*! @enum PokemonStat
		@brief Identifies each Pokemon stat and its position in stat-indexed arrays.
		@note All enum values except @ref PokemonStat::Count must be handled exhaustively when processing Pokemon stats.
		@date 10/08/2026
		@since 0.12.23
		@version 0.12.51
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
		Count,			/*! @brief Provides the number of usable stat values and is not itself a stat. */
	};

	/*! @brief Converts a Pokemon stat identifier to its zero-based array index.
		@param[in] stat The stat identifier to convert; @ref PokemonStat::Count produces the stat-array size.
		@return The zero-based index represented by @p stat.
		@note This constexpr conversion supports both compile-time and runtime stat indexing and is no-throw.
		@since 0.12.23
		@version 0.12.51
	*/
	constexpr std::size_t toIndex(const PokemonStat stat) noexcept
	{
		return std::to_underlying(stat);
	}

	/*! @brief Provides the number of elements required by arrays indexed with @ref PokemonStat. */
	inline constexpr std::size_t POKEMON_STAT_COUNT{toIndex(PokemonStat::Count)};

	/*! @struct PokemonStats Pokemon/pokemonMeta.h
		@brief Stores one pokemon's base stats.
		@details This struct contains the base stats for a pokemon, including health, attack, defense, special attack, special defense, and
	   speed.
		@date 10/09/2026
		@since 0.12.22
		@version 0.12.51
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
				@version 0.12.51
			*/
			ATTR_NODISCARD constexpr bool operator==(const PokemonStats &other) const noexcept = default;

			// NOLINTBEGIN(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)

			/*! @brief The Pokemon's max health stat. */
			us mMaxHealth{};

			/*! @brief The Pokemon's attack stat. */
			us mAttack{};

			/*! @brief The Pokemon's defense stat. */
			us mDefense{};

			/*! @brief The Pokemon's special attack stat. */
			us mSpAttack{};

			/*! @brief The Pokemon's special defense stat. */
			us mSpDefense{};

			/*! @brief The Pokemon's speed stat. */
			us mSpeed{};

			// NOLINTEND(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)
	};
} // namespace PocketCore::Pokemon

#endif
