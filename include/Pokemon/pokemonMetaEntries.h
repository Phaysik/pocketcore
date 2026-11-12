/*! @file pokemonMetaEntries.h
	@brief Defines data carriers for Pokemon meta entries, including abilities and items.
	@date 11/09/2026
	@since 0.12.54
	@version 0.12.54
	@author Matthew Moore
*/

#ifndef INCLUDE_POKEMON_POKEMON_META_ENTRIES_H
#define INCLUDE_POKEMON_POKEMON_META_ENTRIES_H

#include <utility>

#include "Ability/abilityID.h"
#include "Ability/abilityMeta.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Item/itemID.h"
#include "Utility/Math/floatUtility.h"

namespace PocketCore::Pokemon
{
	using PocketCore::Ability::AbilityClassification;
	using PocketCore::Ability::AbilityID;
	using PocketCore::Core::ub;
	using PocketCore::Item::ItemID;
	using PocketCore::Utility::Math::approximatelyEqualAbsRel;

	/*! @enum PokemonGender
		@showenumvalues
		@brief Identifies the gender categories available to a Pokemon.
		@details Supplies the values stored in @ref PokemonMeta::mGenderPool for a species or form.
		@date 10/23/2026
		@since 0.12.53
		@version 0.12.53
		@author Matthew Moore
	*/
	enum class PokemonGender : ub
	{
		/*! @brief Represents a male Pokemon. */
		Male,
		/*! @brief Represents a female Pokemon. */
		Female,
		/*! @brief Represents a Pokemon without a gender. */
		Genderless,
	};

	/*! @struct PokemonAbilityEntry Pokemon/pokemonMeta.h987568
		@brief Stores an ability identifier and its classification for a Pokemon species or form.
		@details Serves as a data carrier for entries in @ref PokemonMeta::mAbilityPool, pairing @ref mAbilityID with @ref mClassification.
		@date 10/26/2026
		@since 0.12.53
		@version 0.12.53
		@author Matthew Moore
	*/
	struct PokemonAbilityEntry
	{
		public:
			/*! @brief Compares the ability identifier and classification of two entries.
				@param[in] other The entry to compare; the reference is not retained.
				@return True when both fields match; otherwise false.
				@note Supports compile-time and runtime evaluation without throwing. The nodiscard result discourages ignoring the
			   comparison.
				@date 10/26/2026
				@since 0.12.53
				@version 0.12.53
				@author Matthew Moore
			*/
			ATTR_NODISCARD constexpr bool operator==(const PokemonAbilityEntry &other) const noexcept = default;

			// NOLINTBEGIN(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)

			/*! @brief The stable built-in or user-assigned Ability identifier. */
			AbilityID mAbilityID{};

			/*! @brief The ability's category, defaulting to @ref AbilityClassification::Standard. */
			AbilityClassification mClassification{AbilityClassification::Standard};

			// NOLINTEND(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)
	};

	/*! @struct PokemonItemEntry Pokemon/pokemonMeta.h
		@brief Stores an item identifier and its chance for a Pokemon species or form.
		@details Serves as a data carrier for entries in @ref PokemonMeta::mItemPool.
		@date 11/03/2026
		@since 0.12.53
		@version 0.12.53
		@author Matthew Moore
	*/
	struct PokemonItemEntry
	{
			/*! @brief Compares the item identifier and chance of the two entries.
				@param[in] other The entry to compare; the reference is not retained.
				@return True when both fields match; otherwise false.
				@note Supports compile-time and runtime evaluation without throwing. The nodiscard result discourages ignoring the
			   comparison.
				@date 10/28/2026
				@since 0.12.53
				@version 0.12.53
				@author Matthew Moore
			*/
			ATTR_NODISCARD constexpr bool operator==(const PokemonItemEntry &other) const noexcept
			{
				return mItemID == other.mItemID && approximatelyEqualAbsRel(mChance, other.mChance);
			}

			// NOLINTBEGIN(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)

			/*! @brief The stable built-in or user-assigned Item identifier. */
			ItemID mItemID{};

			/*! @brief The items's chance to spawn. Bounds should be [0.0, 1.0). Defaults to 0.0 which means it will never be selected. */
			double mChance{0.0};

			// NOLINTEND(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)
	};

	/*! @brief The number of categories defined by @ref PokemonGender.
		@details Supplies the compile-time array size for @ref PokemonMeta::mGenderPool.
		@note Assumes contiguous, zero-based enum values with @ref PokemonGender::Genderless as the final value.
	*/
	inline constexpr ub MAX_GENDERS{std::to_underlying(PokemonGender::Genderless) + 1};
} // namespace PocketCore::Pokemon
#endif
