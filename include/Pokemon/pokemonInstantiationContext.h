/*! @file pokemonInstantiationContext.h
	@brief Defines the contexts needed to instantiate Pokemon
	@date 11/05/2026
	@since 0.12.53
	@version 0.12.54
	@author Matthew Moore
*/

#ifndef INCLUDE_POKEMON_POKEMON_INSTANTIATION_CONTEXT_H
#define INCLUDE_POKEMON_POKEMON_INSTANTIATION_CONTEXT_H

#include <span>

#include "Ability/abilityID.h"
#include "Item/itemID.h"
#include "Move/moveID.h"
#include "Nature/natureID.h"
#include "Pokemon/pokemonID.h"
#include "Pokemon/pokemonMeta.h"

#include "pokemon.h"

namespace PocketCore::Pokemon
{
	using PocketCore::Ability::AbilityID;
	using PocketCore::Item::ItemID;
	using PocketCore::Move::MoveID;
	using PocketCore::Nature::NatureID;

	/*! @struct PokemonBreedingContext Pokemon/pokemonInstantiationContext.h
		@brief Carries non-owning pointers to two potential breeding parents.
		@details Does not own either referenced @ref Pokemon or validate parent presence or breeding eligibility.
			Non-null pointers must refer to live objects whenever the parents are accessed.
		@warning The pointer members have no default member initializers; callers must initialize them before reading them.
		@date 10/26/2026
		@since 0.12.53
		@version 0.12.53
		@author Matthew Moore
	*/
	struct PokemonBreedingContext
	{
		public:
			/*! @brief Holds a non-owning pointer to the first parent.
				@details May be nullptr to indicate no parent; callers must check before dereferencing.
			*/
			const Pokemon *mFirstParent{nullptr};

			/*! @brief Holds a non-owning pointer to the second parent.
				@details May be nullptr to indicate no parent; callers must check before dereferencing.
			*/
			const Pokemon *mSecondParent{nullptr};
	};

	/*! @struct IDContext Pokemon/pokemonInstantiationContext.h
		@brief Carries non-owning views of identifiers supplied for Pokemon instantiation.
		@details Borrows nature, ability, item, and move identifier sequences without copying their elements.
			The backing storage must remain valid for every use of the corresponding span, including through copies of this context.
		@note All spans are empty by default. This data carrier does not validate identifiers or sequence lengths.
		@date 10/27/2026
		@since 0.12.53
		@version 0.12.53
		@author Matthew Moore
	*/
	struct IDContext
	{
		public:
			/*! @brief Provides a read-only, non-owning view of the supplied nature identifiers. */
			std::span<const NatureID> natureIDs = {};

			/*! @brief Provides a read-only, non-owning view of the supplied ability entries. */
			std::span<const PokemonAbilityEntry> abilityEntries = {};

			/*! @brief Provides a read-only, non-owning view of the supplied item identifiers. */
			std::span<const ItemID> itemIDs = {};

			/*! @brief Provides a read-only, non-owning view of the supplied move identifiers. */
			std::span<const MoveID> moveIDs = {};
	};

	/*! @struct IVEVContext Pokemon/pokemonInstantiationContext.h
		@brief Carries non-owning views of individual values (IVs) and effort values (EVs) for Pokemon instantiation.
		@details Borrows stat-value sequences without copying their elements. The backing storage must remain valid for every use
			of the corresponding span, including through copies of this context.
		@note Both spans are empty by default. This data carrier does not validate stat values or sequence lengths.
		@date 10/26/2026
		@since 0.12.53
		@version 0.12.53
		@author Matthew Moore
	*/
	struct IVEVContext
	{
		public:
			/*! @brief Provides a read-only, non-owning view of the supplied individual stat values. */
			std::span<const us> ivs = {};

			/*! @brief Provides a read-only, non-owning view of the supplied effort stat values. */
			std::span<const us> evs = {};
	};

	/*! @struct PokemonSpec Pokemon/pokemonInstantiationContext.h
		@brief Carries caller-supplied and automatic-selection inputs for Pokemon instantiation.
		@details Combines @ref IDContext, @ref IVEVContext, and @ref PokemonBreedingContext without copying their referenced data.
			Non-empty spans supply values; empty spans request automatic selection independently for each field.
			Optional level and gender values override automatic selection.
			Supplying every identifier/stat sequence, level, and gender requests construction without field generation.
		@warning Referenced span storage and breeding parents must remain valid throughout instantiation.
			Copying this spec does not extend their lifetimes. This data carrier performs no validation.
		@date 11/05/2026
		@since 0.12.54
		@version 0.12.54
		@author Matthew Moore
	*/
	struct PokemonSpec
	{
		public:
			/*! @brief Borrows supplied nature, ability, item, and move sequences.
				@details Each empty span requests automatic selection; a non-empty sentinel-only span supplies unused slots.
			*/
			IDContext mIDContext{};

			/*! @brief Borrows supplied IV and EV sequences.
				@details An empty IV span requests generation; an empty EV span requests zero EVs.
			*/
			IVEVContext mIVEVContext{};

			/*! @brief Borrows optional breeding parents for automatic egg ability selection.
				@details Parent pointers are non-owning and default to nullptr; supplied abilities do not request inheritance.
			*/
			PokemonBreedingContext mBreedingContext{};

			/*! @brief Selects the species encounter metadata used by automatic policies.
				@details Defaults to index zero; this value does not indicate whether the encounter exists or is usable.
			*/
			std::size_t mEncounterIndex{};

			/*! @brief Supplies the instance level, or requests encounter-based selection when std::nullopt.
				@details Automatic selection uses the fixed encounter level or samples its inclusive level range.
			*/
			std::optional<us> mLevel{};

			/*! @brief Supplies the instance gender, or requests selection from the species gender pool when std::nullopt. */
			std::optional<PokemonGender> mGender{};

			/*! @brief Identifies the registered species whose metadata is resolved during instantiation.
				@details Must identify the intended registered species; default initialization does not select a species randomly.
			*/
			PokemonID mPokemonID{};
	};
} // namespace PocketCore::Pokemon

#endif
