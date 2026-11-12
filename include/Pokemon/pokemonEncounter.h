/*! @file pokemonEncounter.h
	@brief Defines species-specific acquisition categories and encounter-level specifications.
	@date 11/09/2026
	@since 0.12.51
	@version 0.12.54
	@author Matthew Moore
*/

#ifndef INCLUDE_POKEMON_POKEMONENCOUNTER_H
#define INCLUDE_POKEMON_POKEMONENCOUNTER_H

#include <algorithm>
#include <array>
#include <variant>

#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Item/itemID.h"
#include "Location/locationID.h"
#include "Move/moveID.h"
#include "Nature/natureID.h"
#include "Pokemon/pokemonMetaEntries.h"
#include "Pokemon/pokemonStats.h"

namespace PocketCore::Pokemon
{
	using PocketCore::Configuration::MAX_ABILITIES_PER_POKEMON;
	using PocketCore::Configuration::MAX_ITEMS_PER_POKEMON;
	using PocketCore::Configuration::MAX_MOVES_PER_POKEMON;
	using PocketCore::Configuration::MAX_NATURES_PER_POKEMON;
	using PocketCore::Configuration::MIN_ENCOUNTER_LEVEL;
	using PocketCore::Core::ub;
	using PocketCore::Core::us;
	using PocketCore::Item::ItemID;
	using PocketCore::Location::LocationID;
	using PocketCore::Move::MoveID;
	using PocketCore::Nature::NatureID;

	/*! @enum PokemonOrigin
		@showenumvalues
		@brief Identifies the acquisition category of a species-specific encounter.
		@details Describes a possible acquisition, not an individual's recorded history or later transfers.
		@note All enum values must be handled exhaustively when processing acquisition categories.
		@date 11/09/2026
		@since 0.12.51
		@version 0.12.54
		@author Matthew Moore
	*/
	enum class PokemonOrigin : ub
	{
		Wild,	 /*! @brief Describes acquisition through a wild encounter. */
		Egg,	 /*! @brief Describes acquisition through egg hatching. */
		Trade,	 /*! @brief Describes acquisition through a defined in-game trade, not a later player transfer. */
		Gift,	 /*! @brief Describes acquisition through a gift event. */
		Starter, /*! @brief Describes acquisition by choosing a starter Pokemon. */
	};

	/*! @struct SetLevelEncounter Pokemon/pokemonEncounter.h
		@brief Carries the acquisition level of a fixed-level encounter.
		@note Callers validate that the level is nonzero and permitted by the applicable ruleset.
		@date 10/09/2026
		@since 0.12.51
		@version 0.12.51
		@author Matthew Moore
	*/
	struct SetLevelEncounter
	{
		public:
			/*! @brief Compares fixed acquisition levels exactly.
				@param[in] other The comparison reference, valid for the duration of the call.
				@return True when the levels match; otherwise false.
				@note Supports constexpr comparison, is no-throw, and marks the result nodiscard.
				@since 0.12.51
				@version 0.12.51
			*/
			ATTR_NODISCARD constexpr bool operator==(const SetLevelEncounter &other) const noexcept = default;

			// NOLINTBEGIN(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)

			/*! @brief The acquisition level, not an individual's later current level. */
			us mLevel{MIN_ENCOUNTER_LEVEL};

			// NOLINTEND(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)
	};

	/*! @struct VariableLevelEncounter Pokemon/pokemonEncounter.h
		@brief Carries the inclusive acquisition-level bounds of a variable-level encounter.
		@details Does not specify a selection distribution or perform validation.
		@note Callers validate nonzero, ruleset-permitted bounds with mMinLevel no greater than mMaxLevel.
		@date 10/09/2026
		@since 0.12.51
		@version 0.12.51
		@author Matthew Moore
	*/
	struct VariableLevelEncounter
	{
		public:
			/*! @brief Compares both inclusive level bounds exactly.
				@param[in] other The comparison reference, valid for the duration of the call.
				@return True when both bounds match; otherwise false.
				@note Supports constexpr comparison, is no-throw, and marks the result nodiscard.
				@since 0.12.51
				@version 0.12.51
			*/
			ATTR_NODISCARD constexpr bool operator==(const VariableLevelEncounter &other) const noexcept = default;

			// NOLINTBEGIN(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)

			/*! @brief The inclusive minimum acquisition level. */
			us mMinLevel{MIN_ENCOUNTER_LEVEL};

			/*! @brief The inclusive maximum acquisition level; defaults to the minimum valid level. */
			us mMaxLevel{MIN_ENCOUNTER_LEVEL};

			// NOLINTEND(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)
	};

	/*! @struct PokemonEncounter Pokemon/pokemonEncounter.h
		@brief Carries one species-specific acquisition definition referencing a shared location.
		@details Owns its level specification by value. Multiple definitions may reference the same location with different levels or
	   origins.
		@note A location ID of NO_LOCATION_ID denotes an unused slot in a species' fixed-capacity encounter array.
			Active entries require registered locations and valid level specifications; this carrier does not validate them.
		@date 11/09/2026
		@since 0.12.51
		@version 0.12.54
		@author Matthew Moore
	*/
	struct PokemonEncounter
	{
		public:
			/*! @brief Compares all encounter fields exactly.
				@param[in] other The comparison reference, valid for the duration of the call.
				@return True when locations, origins, variant alternatives, and levels match; otherwise false.
				@note Supports constexpr comparison, is no-throw, and marks the result nodiscard.
				@since 0.12.51
				@version 0.12.54
			*/
			ATTR_NODISCARD constexpr bool operator==(const PokemonEncounter &other) const noexcept = default;

			// NOLINTBEGIN(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)

			/*! @brief Optional fixed-size individual values for the encounter's Pokemon.
				@details When present, supplies the IVs used instead of normal IV generation. When disengaged, normal IV generation is
			   requested. The array is owned by this encounter; its values are not validated here.
			*/
			std::optional<std::array<us, POKEMON_STAT_COUNT>> mIVs{};

			/*! @brief Optional fixed-size move identifiers defining the encounter's initial moves.
				@details When present, supplies the moves used instead of learnset-based move generation. When disengaged, normal
			   learnset-based move generation is requested. The array is owned by this encounter; its identifiers are not validated here.
			*/
			std::optional<std::array<MoveID, MAX_MOVES_PER_POKEMON>> mMoveIDs{};

			/*! @brief Optional fixed-size ability entries for the encounter's Pokemon.
				@details When present, supplies the ability entries used instead of normal ability selection. When disengaged, normal
			   ability selection is requested. The array is owned by this encounter; its entries are not validated here.
			*/
			std::optional<std::array<PokemonAbilityEntry, MAX_ABILITIES_PER_POKEMON>> mAbilityEntries{};

			/*! @brief The fixed acquisition level or inclusive range; defaults to a fixed minimum level. */
			std::variant<SetLevelEncounter, VariableLevelEncounter> mLevel{};

			/*! @brief Optional fixed-size held item identifiers for the encounter's Pokemon.
				@details When present, supplies the item identifiers used instead of normal item selection. When disengaged, normal item
				selection is requested. The array is owned by this encounter; its identifiers are not validated here.
			*/
			std::optional<std::array<ItemID, MAX_ITEMS_PER_POKEMON>> mItemIDs{};

			/*! @brief Optional fixed-size nature identifiers for the encounter's Pokemon.
				@details When present, supplies the nature identifiers used instead of normal nature selection. When disengaged, normal
			   nature selection is requested. The array is owned by this encounter; its identifiers are not validated here.
			*/
			std::optional<std::array<NatureID, MAX_NATURES_PER_POKEMON>> mNatureIDs{};

			/*! @brief The referenced location identifier; NO_LOCATION_ID denotes an unused encounter slot. */
			LocationID mLocationID{};

			/*! @brief The acquisition category, not an individual's transfer history; defaults to Wild. */
			PokemonOrigin mOrigin{};

			// NOLINTEND(misc-non-private-member-variables-in-classes,cppcoreguidelines-non-private-member-variables-in-classes)
	};
} // namespace PocketCore::Pokemon

#endif
