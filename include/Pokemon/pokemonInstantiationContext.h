/*! @file pokemonInstantiationContext.h
	@brief Defines the contexts needed to instantiate Pokemon
	@date 10/26/2026
	@since 0.12.53
	@version 0.12.53
	@author Matthew Moore
*/

#ifndef INCLUDE_POKEMON_POKEMON_INSTANTIATION_CONTEXT_H
#define INCLUDE_POKEMON_POKEMON_INSTANTIATION_CONTEXT_H

#include <span>

#include "Ability/abilityID.h"
#include "Item/itemID.h"
#include "Move/moveID.h"
#include "Nature/natureID.h"

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
		@date 10/26/2026
		@since 0.12.53
		@version 0.12.53
		@author Matthew Moore
	*/
	struct IDContext
	{
		public:
			/*! @brief Provides a read-only, non-owning view of the supplied nature identifiers. */
			std::span<const NatureID> natureIDs = {};

			/*! @brief Provides a read-only, non-owning view of the supplied ability identifiers. */
			std::span<const AbilityID> abilityIDs = {};

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
} // namespace PocketCore::Pokemon

#endif
