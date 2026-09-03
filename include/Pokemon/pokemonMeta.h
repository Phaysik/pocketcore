/*! @file pokemonMeta.h
	@brief Defines the metadata stored for built-in and user-defined pokemons.
	@date 09/03/2026
	@since 0.11.6
	@version 0.12.19
	@author Matthew Moore
*/

#ifndef INCLUDE_POKEMON_POKEMONMETA_H
#define INCLUDE_POKEMON_POKEMONMETA_H

#include <array>
#include <string_view>

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

	/*! @struct PokemonMeta Pokemon/pokemonMeta.h
		@brief Stores one pokemon's stable ID, display name, and owned trigger definitions.
		@details The trigger vector owns its elements and their effect vectors. The display name is a non-owning view whose backing storage
	   must remain valid while this metadata is registered.
		@date 09/03/2026
		@since 0.11.6
		@version 0.12.19
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

			/*! @brief The case-sensitive display name stored as a non-owning view. */
			std::string_view mName{};

			/*! @brief The Pokemon's move IDs. */
			std::array<MoveID, MAX_MOVES_PER_POKEMON> mMoveIDs{};

			/*! @brief The Pokemon's max PP values. */
			std::array<ub, MAX_MOVES_PER_POKEMON> mMaxPP{};

			/*! @brief The Pokemon's type IDs. */
			std::array<TypeID, MAX_TYPES_PER_POKEMON> mTypeIDs{};

			/*! @brief The Pokemon's pool of available abilities. */
			std::array<AbilityID, MAX_ABILITY_POOL_PER_POKEMON> mAbilityPool{};

			/*! @brief The Pokemon's base attack stat. */
			us mBaseAttack{};

			/*! @brief The Pokemon's base defense stat. */
			us mBaseDefense{};

			/*! @brief The Pokemon's base max health stat. */
			us mBaseMaxHealth{};

			/*! @brief The Pokemon's base speed stat. */
			us mBaseSpeed{};

			/*! @brief The Pokemon's base special attack stat. */
			us mBaseSpAttack{};

			/*! @brief The Pokemon's base special defense stat. */
			us mBaseSpDefense{};

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
