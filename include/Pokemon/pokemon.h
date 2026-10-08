/*! @file pokemon.h
	@brief Contains the pokemon
	@date 10/08/2026
	@since 0.3.0
	@version 0.12.50
	@author Matthew Moore
*/

#ifndef INCLUDE_POKEMON_POKEMON_H
#define INCLUDE_POKEMON_POKEMON_H

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <expected>
#include <functional>
#include <ostream>
#include <string_view>

#include "Ability/abilityID.h"
#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Interaction/interactionApplicationError.h"
#include "Interaction/interactionHelpers.h"
#include "Item/itemID.h"
#include "Learnset/learnsetID.h"
#include "Move/moveID.h"
#include "Nature/natureID.h"
#include "Nature/natureMeta.h"
#include "Pokemon/pokemonID.h"
#include "Pokemon/pokemonMeta.h"
#include "Registry/natureRegistry.h"
#include "Registry/statusRegistry.h"
#include "Ruleset/rulesetPolicy.h"
#include "Status/statusID.h"
#include "Status/statusMeta.h"
#include "Types/typeID.h"
#include "Utility/random.h"
#include "Validation/Pokemon/pokemonError.h"
#include "Validation/Pokemon/pokemonValidation.h"

namespace PocketCore::Pokemon
{
	using PocketCore::Ability::AbilityID;
	using PocketCore::Configuration::CALCULATED_EV_DIVISOR;
	using PocketCore::Configuration::CALCULATED_HEALTH_OFFSET;
	using PocketCore::Configuration::CALCULATED_NUMERATOR_DIVISOR;
	using PocketCore::Configuration::CALCULATED_NUMERATOR_MULTIPLIER;
	using PocketCore::Configuration::CALCULATED_STAT_OFFSET;
	using PocketCore::Configuration::LEVEL_DAMAGE_FACTOR_DENOMINATOR;
	using PocketCore::Configuration::LEVEL_DAMAGE_FACTOR_NUMERATOR;
	using PocketCore::Configuration::LEVEL_DAMAGE_FACTOR_OFFSET;
	using PocketCore::Configuration::MAX_ABILITIES_PER_POKEMON;
	using PocketCore::Configuration::MAX_ITEMS_PER_POKEMON;
	using PocketCore::Configuration::MAX_MOVES_PER_POKEMON;
	using PocketCore::Configuration::MAX_NATURES_PER_POKEMON;
	using PocketCore::Configuration::MAX_NON_VOLATILE_STATUSES_PER_POKEMON;
	using PocketCore::Configuration::MAX_TYPES_PER_POKEMON;
	using PocketCore::Configuration::NATURE_STAT_BASE_MULTIPLIER;
	using PocketCore::Core::ub;
	using PocketCore::Core::ui;
	using PocketCore::Core::us;
	using PocketCore::Interaction::applyInteractions;
	using PocketCore::Interaction::InteractionApplicationError;
	using PocketCore::Item::ItemID;
	using PocketCore::Learnset::LearnsetID;
	using PocketCore::Move::MoveID;
	using PocketCore::Nature::NatureID;
	using PocketCore::Nature::NatureMeta;
	using PocketCore::Registry::Nature::NatureRegistry;
	using PocketCore::Registry::Status::StatusRegistry;
	using PocketCore::Ruleset::RulesetPolicy;
	using PocketCore::Status::NO_STATUS_ID;
	using PocketCore::Status::StatusClassification;
	using PocketCore::Status::StatusID;
	using PocketCore::Status::StatusMeta;
	using PocketCore::Type::TypeID;
	using PocketCore::Utility::Random;
	using PocketCore::Validation::Pokemon::isValidPokemonEV;
	using PocketCore::Validation::Pokemon::isValidPokemonEVArray;
	using PocketCore::Validation::Pokemon::isValidPokemonIV;
	using PocketCore::Validation::Pokemon::isValidPokemonIVArray;
	using PocketCore::Validation::Pokemon::PokemonError;

	/*! @class Pokemon Pokemon/pokemon.h
		@brief Stores a Pokemon's identity, battle statistics, moves, held items, abilities, types, natures, and statuses.
		@details The class owns all identifier arrays and scalar battle state. The display name is a non-owning string view whose backing
		 storage must remain valid for the lifetime of the Pokemon object. Indexed accessors and mutators require an index within the
		 corresponding fixed-size array. When a stat input changes calculated maximum health, current health changes by the same amount
		 to preserve missing health; a Pokemon with zero current health remains fainted, and a living Pokemon is never reduced below 1.
		@warning A Pokemon does not own the registry objects passed to its status operations or used by formatting helpers.
		@date 10/08/2026
		@since 0.3.0
		@version 0.12.50
		@author Matthew Moore
	*/
	class Pokemon
	{
		public:
			// Constructors

			/*! @brief Constructs a Pokemon with empty move slots and zero move PP.
				@param[in] pokemonID The ID of the Pokemon.
				@param[in] name Non-owning display-name view whose backing storage must outlive the object.
				@param[in] stats The base stats of the Pokemon.
				@param[in] level Pokemon level used to compute the level damage factor.
				@param[in] abilityIDs Fixed ability identifier slots.
				@param[in] itemIDs Fixed held-item identifier slots.
				@param[in] typeIDs Fixed type identifier slots.
				@param[in] natureIDs Fixed nature identifier slots.
				@param[in] natureRegistry The registry used to resolve the incoming nature metadata.
				@param[in] pokemonIVs Fixed individual values for the Pokemon's base stats.
				@param[in] pokemonEVs Fixed effort values for the Pokemon's base stats.
				@since 0.3.0
				@version 0.12.50
			*/
			explicit constexpr Pokemon(const PokemonID pokemonID, const LearnsetID learnsetID, const std::string_view &name,
									   const PokemonStats &stats, const us level,
									   const std::array<AbilityID, MAX_ABILITIES_PER_POKEMON> &abilityIDs,
									   const std::array<ItemID, MAX_ITEMS_PER_POKEMON> &itemIDs,
									   const std::array<TypeID, MAX_TYPES_PER_POKEMON> &typeIDs,
									   const std::array<NatureID, MAX_NATURES_PER_POKEMON> &natureIDs, const NatureRegistry &natureRegistry,
									   const std::array<us, POKEMON_STAT_COUNT> &pokemonIVs,
									   const std::array<us, POKEMON_STAT_COUNT> &pokemonEVs)
				: mName{name}, mBaseStats{stats}, mTypeIDs{typeIDs}, mAbilityIDs{abilityIDs}, mItemIDs{itemIDs}, mPokemonID(pokemonID),
				  mLearnsetID(learnsetID)
			{
				mMoveIDs.fill(PocketCore::Move::NO_MOVE_ID);
				mMaxPP.fill(0);
				mCurrentPP.fill(0);

				resolveNatureMultipliers(natureIDs, natureRegistry);

				updateLevel(level);
				updatePokemonIVsArray(pokemonIVs);
				updatePokemonEVsArray(pokemonEVs);

				recomputeStats();

				setHealth(mCalculatedStats.mMaxHealth);
			}

			/*! @brief Constructs a Pokemon from complete move and PP arrays.
				@param[in] pokemonID The ID of the Pokemon.
				@param[in] name Non-owning display-name view whose backing storage must outlive the object.
				@param[in] moveIDs Fixed move identifier slots.
				@param[in] maxPP Maximum PP for each move slot.
				@param[in] currentPP Current PP for each move slot.
				@param[in] stats The base stats of the Pokemon.
				@param[in] level Pokemon level used to compute the level damage factor.
				@param[in] abilityIDs Fixed ability identifier slots.
				@param[in] itemIDs Fixed held-item identifier slots.
				@param[in] typeIDs Fixed type identifier slots.
				@param[in] natureIDs Fixed nature identifier slots.
				@param[in] natureRegistry The registry used to resolve the incoming status metadata.
				@param[in] pokemonIVs Fixed individual values for the Pokemon's base stats.
				@param[in] pokemonEVs Fixed effort values for the Pokemon's base stats.
				@since 0.3.0
				@version 0.12.50
			*/
			explicit constexpr Pokemon(const PokemonID pokemonID, const LearnsetID learnsetID, const std::string_view &name,
									   const std::array<MoveID, MAX_MOVES_PER_POKEMON> &moveIDs,
									   const std::array<ub, MAX_MOVES_PER_POKEMON> &maxPP,
									   const std::array<ub, MAX_MOVES_PER_POKEMON> &currentPP, const PokemonStats &stats, const us level,
									   const std::array<AbilityID, MAX_ABILITIES_PER_POKEMON> &abilityIDs,
									   const std::array<ItemID, MAX_ITEMS_PER_POKEMON> &itemIDs,
									   const std::array<TypeID, MAX_TYPES_PER_POKEMON> &typeIDs,
									   const std::array<NatureID, MAX_NATURES_PER_POKEMON> &natureIDs, const NatureRegistry &natureRegistry,
									   const std::array<us, POKEMON_STAT_COUNT> &pokemonIVs,
									   const std::array<us, POKEMON_STAT_COUNT> &pokemonEVs)
				: mName{name}, mBaseStats{stats}, mMoveIDs{moveIDs}, mMaxPP{maxPP}, mCurrentPP{currentPP}, mTypeIDs{typeIDs},
				  mAbilityIDs{abilityIDs}, mItemIDs{itemIDs}, mPokemonID(pokemonID), mLearnsetID(learnsetID)
			{
				resolveNatureMultipliers(natureIDs, natureRegistry);

				updateLevel(level);
				updatePokemonIVsArray(pokemonIVs);
				updatePokemonEVsArray(pokemonEVs);

				recomputeStats();

				setHealth(mCalculatedStats.mMaxHealth);
			}

			// Getters

			/*! @brief Returns the non-owning display name.
				@return A reference to the stored name view.
				@since 0.3.0
				@version 0.12.2
			*/
			ATTR_NODISCARD constexpr const std::string_view &getName() const
			{
				return mName;
			}

			/*! @brief Returns all status identifier slots.
				@return A read-only reference valid for the object's lifetime.
				@since 0.9.11
				@version 0.12.46
			*/
			ATTR_NODISCARD constexpr const std::array<StatusID, MAX_NON_VOLATILE_STATUSES_PER_POKEMON> &getStatusIDsArray() const
			{
				return mNonVolatileStatusIDs;
			}

			/*! @brief Returns all move identifier slots.
				@return A read-only reference valid for the object's lifetime.
				@since 0.3.0
				@version 0.12.17
			*/
			ATTR_NODISCARD constexpr const std::array<MoveID, MAX_MOVES_PER_POKEMON> &getMoveIDsArray() const
			{
				return mMoveIDs;
			}

			/*! @brief Returns all individual value (IV) slots for the Pokemon's base stats.
				@return A read-only reference valid for the object's lifetime.
				@since 0.12.23
				@version 0.12.23
			*/
			ATTR_NODISCARD constexpr const std::array<us, POKEMON_STAT_COUNT> &getPokemonIVsArray() const
			{
				return mPokemonIVs;
			}

			/*! @brief Returns all effort value (EV) slots for the Pokemon's base stats.
				@return A read-only reference valid for the object's lifetime.
				@since 0.12.23
				@version 0.12.23
			*/
			ATTR_NODISCARD constexpr const std::array<us, POKEMON_STAT_COUNT> &getPokemonEVsArray() const
			{
				return mPokemonEVs;
			}

			/*! @brief Returns maximum PP for every move slot.
				@return A read-only reference valid for the object's lifetime.
				@since 0.3.0
				@version 0.12.2
			*/
			ATTR_NODISCARD constexpr const std::array<ub, MAX_MOVES_PER_POKEMON> &getMaxPPArray() const
			{
				return mMaxPP;
			}

			/*! @brief Returns current PP for every move slot.
				@return A read-only reference valid for the object's lifetime.
				@since 0.3.0
				@version 0.12.2
			*/
			ATTR_NODISCARD constexpr const std::array<ub, MAX_MOVES_PER_POKEMON> &getCurrentPPArray() const
			{
				return mCurrentPP;
			}

			/*! @brief Returns all type identifier slots.
				@return A read-only reference valid for the object's lifetime.
				@since 0.4.0
				@version 0.12.17
			*/
			ATTR_NODISCARD constexpr const std::array<TypeID, MAX_TYPES_PER_POKEMON> &getTypeIDsArray() const noexcept
			{
				return mTypeIDs;
			}

			/*! @brief Returns all ability identifier slots.
				@return A read-only reference valid for the object's lifetime.
				@since 0.11.6
				@version 0.12.17
			*/
			ATTR_NODISCARD constexpr const std::array<AbilityID, MAX_ABILITIES_PER_POKEMON> &getAbilityIDsArray() const noexcept
			{
				return mAbilityIDs;
			}

			/*! @brief Returns all held-item identifier slots.
				@return A read-only reference valid for the object's lifetime.
				@since 0.11.6
				@version 0.12.17
			*/
			ATTR_NODISCARD constexpr const std::array<ItemID, MAX_ITEMS_PER_POKEMON> &getItemsIDsArray() const noexcept
			{
				return mItemIDs;
			}

			/*! @brief Returns all nature identifier slots.
				@return A read-only reference valid for the object's lifetime.
				@since 0.11.6
				@version 0.12.2
			*/
			ATTR_NODISCARD constexpr const std::array<NatureID, MAX_NATURES_PER_POKEMON> &getNatureIDsArray() const noexcept
			{
				return mNatureIDs;
			}

			/*! @brief Returns a status identifier by slot index.
				@param[in] index Status slot index; must be less than MAX_NON_VOLATILE_STATUSES_PER_POKEMON.
				@return The status identifier stored in the slot.
				@pre index < MAX_NON_VOLATILE_STATUSES_PER_POKEMON; violation triggers an assertion.
				@since 0.8.1
				@version 0.12.46
			*/
			ATTR_NODISCARD constexpr StatusID getStatusID(const us index) const
			{
				assert(index < mNonVolatileStatusIDs.size());

				return mNonVolatileStatusIDs.at(index);
			}

			/*! @brief Returns the move identifier at an indexed move slot.
				@param[in] index Move slot index; must be less than MAX_MOVES_PER_POKEMON.
				@return The move identifier stored in the slot.
				@pre index < MAX_MOVES_PER_POKEMON; violation triggers an assertion.
				@since 0.3.0
				@version 0.12.2
			*/
			ATTR_NODISCARD constexpr MoveID getMoveID(const us index) const
			{
				assert(index < mMoveIDs.size());

				return mMoveIDs.at(index);
			}

			/*! @brief Returns an individual value (IV) for an indexed base stat slot.
				@param[in] index Base stat slot index; must be less than POKEMON_STAT_COUNT.
				@return The individual value stored in the slot.
				@pre index < POKEMON_STAT_COUNT; violation triggers an assertion.
				@since 0.12.23
				@version 0.12.23
			*/
			ATTR_NODISCARD constexpr us getPokemonIV(const us index) const
			{
				assert(index < mPokemonIVs.size());

				return mPokemonIVs.at(index);
			}

			/*! @brief Returns an effort value (EV) for an indexed base stat slot.
				@param[in] index Base stat slot index; must be less than POKEMON_STAT_COUNT.
				@return The effort value stored in the slot.
				@pre index < POKEMON_STAT_COUNT; violation triggers an assertion.
				@since 0.12.23
				@version 0.12.23
			*/
			ATTR_NODISCARD constexpr us getPokemonEV(const us index) const
			{
				assert(index < mPokemonEVs.size());

				return mPokemonEVs.at(index);
			}

			/*! @brief Returns maximum PP for an indexed move slot.
				@param[in] index Move slot index; must be less than MAX_MOVES_PER_POKEMON.
				@return The slot's maximum PP.
				@pre index < MAX_MOVES_PER_POKEMON; violation triggers an assertion.
				@since 0.3.0
				@version 0.12.2
			*/
			ATTR_NODISCARD constexpr ub getMaxPP(const us index) const
			{
				assert(index < mMaxPP.size());

				return mMaxPP.at(index);
			}

			/*! @brief Returns current PP for an indexed move slot.
				@param[in] index Move slot index; must be less than MAX_MOVES_PER_POKEMON.
				@return The slot's current PP.
				@pre index < MAX_MOVES_PER_POKEMON; violation triggers an assertion.
				@since 0.3.0
				@version 0.12.2
			*/
			ATTR_NODISCARD constexpr ub getCurrentPP(const us index) const
			{
				assert(index < mCurrentPP.size());

				return mCurrentPP.at(index);
			}

			/*! @brief Returns a type identifier by slot index.
				@param[in] index Type slot index; must be less than MAX_TYPES_PER_POKEMON.
				@return The type identifier stored in the slot.
				@pre index < MAX_TYPES_PER_POKEMON; violation triggers an assertion.
				@since 0.4.0
				@version 0.12.2
			*/
			ATTR_NODISCARD constexpr TypeID getTypeID(const ub index) const
			{
				assert(index < mTypeIDs.size());

				return mTypeIDs.at(index);
			}

			/*! @brief Returns an ability identifier by slot index.
				@param[in] index Ability slot index; must be less than MAX_ABILITIES_PER_POKEMON.
				@return The ability identifier stored in the slot.
				@pre index < MAX_ABILITIES_PER_POKEMON; violation triggers an assertion.
				@since 0.3.0
				@version 0.12.2
			*/
			ATTR_NODISCARD constexpr AbilityID getAbilityID(const ub index) const
			{
				assert(index < mAbilityIDs.size());

				return mAbilityIDs.at(index);
			}

			/*! @brief Returns a held-item identifier by slot index.
				@param[in] index Item slot index; must be less than MAX_ITEMS_PER_POKEMON.
				@return The item identifier stored in the slot.
				@pre index < MAX_ITEMS_PER_POKEMON; violation triggers an assertion.
				@since 0.3.0
				@version 0.12.2
			*/
			ATTR_NODISCARD constexpr ItemID getItemID(const ub index) const
			{
				assert(index < mItemIDs.size());

				return mItemIDs.at(index);
			}

			/*! @brief Returns a nature identifier by slot index.
				@param[in] index Nature slot index; must be less than MAX_NATURES_PER_POKEMON.
				@return The nature identifier stored in the slot.
				@pre index < MAX_NATURES_PER_POKEMON; violation triggers an assertion.
				@since 0.11.6
				@version 0.12.2
			*/
			ATTR_NODISCARD constexpr NatureID getNatureID(const ub index) const
			{
				assert(index < mNatureIDs.size());

				return mNatureIDs.at(index);
			}

			/*! @brief Returns current health.
				@return The current health value.
				@since 0.3.0
				@version 0.12.2
			*/
			ATTR_NODISCARD constexpr us getHealth() const
			{
				return mHealth;
			}

			/*! @brief Returns the calculated maximum health.
				@return The maximum health value.
				@since 0.9.14
				@version 0.12.49
			*/
			ATTR_NODISCARD constexpr us getMaximumHealth() const
			{
				return mCalculatedStats.mMaxHealth;
			}

			/*! @brief Returns the calculated Attack statistic.
				@return The Attack value.
				@since 0.3.0
				@version 0.12.49
			*/
			ATTR_NODISCARD constexpr us getAttack() const
			{
				return mCalculatedStats.mAttack;
			}

			/*! @brief Returns the calculated Defense statistic.
				@return The Defense value.
				@since 0.3.0
				@version 0.12.49
			*/
			ATTR_NODISCARD constexpr us getDefense() const
			{
				return mCalculatedStats.mDefense;
			}

			/*! @brief Returns the calculated Special Attack statistic.
				@return The Special Attack value.
				@since 0.3.0
				@version 0.12.49
			*/
			ATTR_NODISCARD constexpr us getSpAttack() const
			{
				return mCalculatedStats.mSpAttack;
			}

			/*! @brief Returns the calculated Special Defense statistic.
				@return The Special Defense value.
				@since 0.3.0
				@version 0.12.49
			*/
			ATTR_NODISCARD constexpr us getSpDefense() const
			{
				return mCalculatedStats.mSpDefense;
			}

			/*! @brief Returns the calculated Speed statistic.
				@return The Speed value.
				@since 0.3.0
				@version 0.12.49
			*/
			ATTR_NODISCARD constexpr us getSpeed() const
			{
				return mCalculatedStats.mSpeed;
			}

			/*! @brief Returns the Pokemon's level.
				@return The level value.
				@since 0.7.2
				@version 0.12.2
			*/
			ATTR_NODISCARD constexpr us getLevel() const
			{
				return mLevel;
			}

			/*! @brief Returns the precomputed level damage factor.
				@return The level damage factor used by damage calculations.
				@since 0.8.2
				@version 0.12.2
			*/
			ATTR_NODISCARD constexpr us getLevelDamageFactor() const
			{
				return mLevelDamageFactor;
			}

			/*! @brief Returns the stable identifier for the Pokemon species.
				@return The PokemonID value.
				@since 0.12.23
				@version 0.12.23
			*/
			ATTR_NODISCARD constexpr PokemonID getPokemonID() const
			{
				return mPokemonID;
			}

			/*! @brief Returns the stable identifier for the Learnset species.
				@return The LearnsetID value.
				@since 0.12.50
				@version 0.12.50
			*/
			ATTR_NODISCARD constexpr LearnsetID getLearnsetID() const
			{
				return mLearnsetID;
			}

			// Setters

			/*! @brief Replaces the non-owning display-name view.
				@param[in] name Display-name view whose backing storage must outlive the object.
				@since 0.3.0
				@version 0.12.2
			*/
			constexpr void setName(const std::string_view &name)
			{
				mName = name;
			}

			/*! @brief Replaces all status identifier slots.
				@param[in] statusIDs The status identifiers to store.
				@since 0.9.11
				@version 0.12.46
			*/
			constexpr void setStatusIDsArray(const std::array<StatusID, MAX_NON_VOLATILE_STATUSES_PER_POKEMON> &statusIDs)
			{
				mNonVolatileStatusIDs = statusIDs;
			}

			/*! @brief Replaces all move identifier slots.
				@param[in] moveIDs The move identifiers to store.
				@since 0.3.0
				@version 0.12.17
			*/
			constexpr void setMoveIDsArray(const std::array<MoveID, MAX_MOVES_PER_POKEMON> &moveIDs)
			{
				mMoveIDs = moveIDs;
			}

			/*! @brief Sets all individual values (IVs) and immediately recomputes the calculated stats.
				@details Delegates validation and storage to @ref updatePokemonIVsArray. Valid arrays are stored unchanged; if any value is
				invalid, the existing IV array is left unchanged. Invokes @ref recomputeStats after either outcome.
				@param[in] pokemonIVs The individual values to validate and copy; no reference to the input is retained.
				@post The calculated stats reflect the stored IVs after either validation outcome.
				@since 0.12.23
				@version 0.12.49
			*/
			constexpr void setPokemonIVsArray(const std::array<us, POKEMON_STAT_COUNT> &pokemonIVs)
			{
				updatePokemonIVsArray(pokemonIVs);

				recomputeStats();
			}

			/*! @brief Sets all effort values (EVs) and immediately recomputes the calculated stats.
				@details Delegates validation and storage to @ref updatePokemonEVsArray. Valid arrays are stored unchanged; an array that
				violates the configured per-stat or aggregate EV limits leaves the existing EV array unchanged. Invokes @ref recomputeStats
				after either outcome.
				@param[in] pokemonEVs The effort values to validate and copy; no reference to the input is retained.
				@post The calculated stats reflect the stored EVs after either validation outcome.
				@since 0.12.23
				@version 0.12.49
			*/
			constexpr void setPokemonEVsArray(const std::array<us, POKEMON_STAT_COUNT> &pokemonEVs)
			{
				updatePokemonEVsArray(pokemonEVs);

				recomputeStats();
			}

			/*! @brief Replaces maximum PP for all move slots.
				@param[in] maxPP The maximum PP values to store.
				@since 0.3.0
				@version 0.12.2
			*/
			constexpr void setMaxPPArray(const std::array<ub, MAX_MOVES_PER_POKEMON> &maxPP)
			{
				mMaxPP = maxPP;
			}

			/*! @brief Replaces current PP for all move slots.
				@param[in] currentPP The current PP values to store.
				@since 0.3.0
				@version 0.12.2
			*/
			constexpr void setCurrentPPArray(const std::array<ub, MAX_MOVES_PER_POKEMON> &currentPP)
			{
				mCurrentPP = currentPP;
			}

			/*! @brief Replaces all type identifier slots.
				@param[in] typeIDs The type identifiers to store.
				@since 0.4.0
				@version 0.12.17
			*/
			constexpr void setTypeIDsArray(const std::array<TypeID, MAX_TYPES_PER_POKEMON> &typeIDs)
			{
				mTypeIDs = typeIDs;
			}

			/*! @brief Replaces all ability identifier slots.
				@param[in] abilityIDs The ability identifiers to store.
				@since 0.11.6
				@version 0.12.2
			*/
			constexpr void setAbilityIDsArray(const std::array<AbilityID, MAX_ABILITIES_PER_POKEMON> &abilityIDs)
			{
				mAbilityIDs = abilityIDs;
			}

			/*! @brief Replaces all held-item identifier slots.
				@param[in] itemIDs The item identifiers to store.
				@since 0.11.6
				@version 0.12.2
			*/
			constexpr void setItemIDsArray(const std::array<ItemID, MAX_ITEMS_PER_POKEMON> &itemIDs)
			{
				mItemIDs = itemIDs;
			}

			/*! @brief Resolves all nature slots and immediately recomputes the calculated stats.
				@details Resolves nature multipliers from the registry, then invokes @ref recomputeStats. If any nature is unregistered,
			   retains both the existing identifiers and multiplier rows, then recomputes using those unchanged inputs.
				@param[in] natureIDs The nature identifiers to store.
				@param[in] natureRegistry Non-owning registry reference used during this call; no reference is retained.
				@post The calculated stats reflect the stored nature multipliers after either lookup outcome.
				@since 0.11.6
				@version 0.12.49
			*/
			constexpr void setNatureIDsArray(const std::array<NatureID, MAX_NATURES_PER_POKEMON> &natureIDs,
											 const NatureRegistry &natureRegistry)
			{
				resolveNatureMultipliers(natureIDs, natureRegistry);
				recomputeStats();
			}

			/*! @brief Sets one status slot.
				@param[in] slotIndex Status slot index; must be less than MAX_NON_VOLATILE_STATUSES_PER_POKEMON.
				@param[in] statusID The status identifier to store.
				@pre slotIndex < MAX_NON_VOLATILE_STATUSES_PER_POKEMON; violation triggers an assertion.
				@since 0.12.17
				@version 0.12.46
			*/
			constexpr void setStatusID(const ub slotIndex, const StatusID statusID)
			{
				assert(slotIndex < mNonVolatileStatusIDs.size());

				mNonVolatileStatusIDs.at(slotIndex) = statusID;
			}

			/*! @brief Sets one move slot.
				@param[in] slotIndex Move slot index; must be less than MAX_MOVES_PER_POKEMON.
				@param[in] moveID The move identifier to store.
				@pre slotIndex < MAX_MOVES_PER_POKEMON; violation triggers an assertion.
				@since 0.3.0
				@version 0.12.17
			*/
			constexpr void setMoveID(const ub slotIndex, const MoveID moveID)
			{
				assert(slotIndex < mMoveIDs.size());

				mMoveIDs.at(slotIndex) = moveID;
			}

			/*! @brief Sets one individual value (IV) slot for the Pokemon's base stats.
				@details Stores the supplied value when it is within the configured IV bounds. Otherwise, the slot is not updated.
			   Immediately invokes @ref recomputeStats after either outcome. If the calculated maximum health changes, current health is
			   adjusted by the same amount unless it is already zero.
				@param[in] slotIndex Base stat slot index; must be less than POKEMON_STAT_COUNT.
				@param[in] pokemonIV The individual value to store; must be within the configured IV bounds to be stored as supplied.
				@pre slotIndex < POKEMON_STAT_COUNT; violation triggers an assertion.
				@since 0.12.23
				@version 0.12.49
			*/
			constexpr void setPokemonIV(const ub slotIndex, const us pokemonIV)
			{
				assert(slotIndex < mPokemonIVs.size());

				const std::expected<void, PokemonError> result{isValidPokemonIV(pokemonIV)};

				if (result.has_value())
				{
					mPokemonIVs.at(slotIndex) = pokemonIV;
				}

				recomputeStats();
			}

			/*! @brief Sets one effort value (EV) slot for the Pokemon's base stats.
				@details Validates the supplied value against the configured per-stat EV bounds and the sum of the other EV slots. Stores it
				when valid; otherwise, the slot is not updated. Immediately invokes @ref recomputeStats after either outcome.
				@param[in] slotIndex Base stat slot index; must be less than POKEMON_STAT_COUNT.
				@param[in] pokemonEV The effort value to store, subject to the per-stat and aggregate EV limits.
				@pre slotIndex < POKEMON_STAT_COUNT; violation triggers an assertion.
				@since 0.12.23
				@version 0.12.49
			*/
			constexpr void setPokemonEV(const ub slotIndex, const us pokemonEV)
			{
				assert(slotIndex < mPokemonEVs.size());

				const us statTotal{static_cast<us>(std::ranges::fold_left(mPokemonEVs, 0, std::plus<>()) - mPokemonEVs.at(slotIndex))};

				const std::expected<void, PokemonError> result{isValidPokemonEV(pokemonEV, statTotal)};

				if (result.has_value())
				{
					mPokemonEVs.at(slotIndex) = pokemonEV;
				}

				recomputeStats();
			}

			/*! @brief Sets maximum PP for one move slot.
				@param[in] slotIndex Move slot index; must be less than MAX_MOVES_PER_POKEMON.
				@param[in] maxPP The maximum PP value to store.
				@pre slotIndex < MAX_MOVES_PER_POKEMON; violation triggers an assertion.
				@since 0.3.0
				@version 0.12.2
			*/
			constexpr void setMaxPP(const ub slotIndex, const ub maxPP)
			{
				assert(slotIndex < mMaxPP.size());

				mMaxPP.at(slotIndex) = maxPP;
			}

			/*! @brief Sets current PP for one move slot.
				@param[in] slotIndex Move slot index; must be less than MAX_MOVES_PER_POKEMON.
				@param[in] currentPP The current PP value to store.
				@pre slotIndex < MAX_MOVES_PER_POKEMON; violation triggers an assertion.
				@since 0.3.0
				@version 0.12.2
			*/
			constexpr void setCurrentPP(const ub slotIndex, const ub currentPP)
			{
				assert(slotIndex < mCurrentPP.size());

				mCurrentPP.at(slotIndex) = currentPP;
			}

			/*! @brief Sets one type slot.
				@param[in] slotIndex Type slot index; must be less than MAX_TYPES_PER_POKEMON.
				@param[in] typeID The type identifier to store.
				@pre slotIndex < MAX_TYPES_PER_POKEMON; violation triggers an assertion.
				@since 0.4.0
				@version 0.12.17
			*/
			constexpr void setTypeID(const ub slotIndex, const TypeID typeID)
			{
				assert(slotIndex < mTypeIDs.size());

				mTypeIDs.at(slotIndex) = typeID;
			}

			/*! @brief Sets one ability slot.
				@param[in] slotIndex Ability slot index; must be less than MAX_ABILITIES_PER_POKEMON.
				@param[in] abilityID The ability identifier to store.
				@pre slotIndex < MAX_ABILITIES_PER_POKEMON; violation triggers an assertion.
				@since 0.3.0
				@version 0.12.17
			*/
			constexpr void setAbilityID(const ub slotIndex, const AbilityID abilityID)
			{
				assert(slotIndex < mAbilityIDs.size());

				mAbilityIDs.at(slotIndex) = abilityID;
			}

			/*! @brief Sets one held-item slot.
				@param[in] slotIndex Item slot index; must be less than MAX_ITEMS_PER_POKEMON.
				@param[in] itemID The item identifier to store.
				@pre slotIndex < MAX_ITEMS_PER_POKEMON; violation triggers an assertion.
				@since 0.3.0
				@version 0.12.17
			*/
			constexpr void setItemID(const ub slotIndex, const ItemID itemID)
			{
				assert(slotIndex < mItemIDs.size());

				mItemIDs.at(slotIndex) = itemID;
			}

			/*! @brief Sets one registered nature slot and immediately recomputes the calculated stats.
				@details Stores the identifier and its registry-resolved multipliers, then invokes @ref recomputeStats. An unregistered
			   nature leaves all state unchanged and returns without recalculating.
				@param[in] slotIndex Nature slot index; must be less than MAX_NATURES_PER_POKEMON.
				@param[in] natureID The nature identifier to store.
				@param[in] natureRegistry Non-owning registry reference used during this call; no reference is retained.
				@pre slotIndex < MAX_NATURES_PER_POKEMON; violation triggers an assertion.
				@since 0.11.6
				@version 0.12.49
			*/
			constexpr void setNatureID(const ub slotIndex, const NatureID natureID, const NatureRegistry &natureRegistry)
			{
				assert(slotIndex < mNatureIDs.size());

				const NatureMeta *metadata{natureRegistry.getNatureMetadata(natureID)};

				if (metadata == nullptr)
				{
					return;
				}

				mNatureIDs.at(slotIndex) = natureID;
				mNatureMultipliers.at(slotIndex) = metadata->mStatMultipliers;
				recomputeStats();
			}

			/*! @brief Sets current health, clamped to maximum health.
				@details Uses the existing calculated maximum; does not invoke @ref recomputeStats or change stat inputs.
				@param[in] health The requested current health value.
				@since 0.3.0
				@version 0.12.49
			*/
			constexpr void setHealth(const us health)
			{
				mHealth = std::min(health, mCalculatedStats.mMaxHealth);
			}

			/*! @brief Replaces base HP and immediately recomputes all calculated stats.
				@details Invokes @ref recomputeStats, which adjusts current health by the change in calculated maximum health while
			   preserving the amount of missing health. A Pokemon with zero current health remains fainted; a living Pokemon is never
			   reduced below 1.
				@param[in] maximumHealth The new base HP value used by the stat calculation.
				@post Current health does not exceed @ref getMaximumHealth; zero current health remains zero and non-zero health stays at
			   least 1.
				@since 0.9.14
				@version 0.12.49
			*/
			constexpr void setMaximumHealth(const us maximumHealth)
			{
				mBaseStats.mMaxHealth = maximumHealth;
				recomputeStats();
			}

			/*! @brief Replaces base Attack and immediately recomputes all calculated stats.
				@details Invokes @ref recomputeStats after storing the base stat.
				@param[in] attack The new base Attack value.
				@since 0.3.0
				@version 0.12.49
			*/
			constexpr void setAttack(const us attack)
			{
				mBaseStats.mAttack = attack;
				recomputeStats();
			}

			/*! @brief Replaces base Defense and immediately recomputes all calculated stats.
				@details Invokes @ref recomputeStats after storing the base stat.
				@param[in] defense The new base Defense value.
				@since 0.3.0
				@version 0.12.49
			*/
			constexpr void setDefense(const us defense)
			{
				mBaseStats.mDefense = defense;
				recomputeStats();
			}

			/*! @brief Replaces base Special Attack and immediately recomputes all calculated stats.
				@details Invokes @ref recomputeStats after storing the base stat.
				@param[in] spAttack The new base Special Attack value.
				@since 0.3.0
				@version 0.12.49
			*/
			constexpr void setSpAttack(const us spAttack)
			{
				mBaseStats.mSpAttack = spAttack;
				recomputeStats();
			}

			/*! @brief Replaces base Special Defense and immediately recomputes all calculated stats.
				@details Invokes @ref recomputeStats after storing the base stat.
				@param[in] spDefense The new base Special Defense value.
				@since 0.3.0
				@version 0.12.49
			*/
			constexpr void setSpDefense(const us spDefense)
			{
				mBaseStats.mSpDefense = spDefense;
				recomputeStats();
			}

			/*! @brief Replaces base Speed and immediately recomputes all calculated stats.
				@details Invokes @ref recomputeStats after storing the base stat.
				@param[in] speed The new base Speed value.
				@since 0.3.0
				@version 0.12.49
			*/
			constexpr void setSpeed(const us speed)
			{
				mBaseStats.mSpeed = speed;
				recomputeStats();
			}

			/*! @brief Sets the level and immediately recomputes the damage factor and calculated stats.
				@details Updates the level and damage factor through @ref updateLevel, then invokes @ref recomputeStats.
				@param[in] level The new Pokemon level.
				@post The damage factor and calculated stats reflect the new level.
				@since 0.7.2
				@version 0.12.48
			*/
			constexpr void setLevel(const us level)
			{
				updateLevel(level);
				recomputeStats();
			}

			/*! @brief Replaces species identity and base stats, then immediately recomputes all calculated stats.
				@details Copies the supplied base-stat record and invokes @ref recomputeStats; does not resolve species metadata.
				@param[in] meta The registry-resolved metadata for the new Pokemon.
				@since 0.12.23
				@version 0.12.50
			*/
			constexpr void setSpecies(const PokemonMeta &meta)
			{
				mPokemonID = meta.mPokemonID;
				mLearnsetID = meta.mLearnsetID;
				mBaseStats = meta.mBaseStats;
				recomputeStats();
			}

			// Utility Functions

			/*! @brief Consumes one PP from a move slot when PP remains.
				@param[in] slotIndex Move slot index; must be less than MAX_MOVES_PER_POKEMON.
				@pre slotIndex < MAX_MOVES_PER_POKEMON; violation triggers an assertion.
				@post The slot's current PP decreases by one when it was greater than zero; otherwise it is unchanged.
				@since 0.3.0
				@version 0.12.2
			*/
			constexpr void usePP(const ub slotIndex)
			{
				assert(slotIndex < mCurrentPP.size());

				if (mCurrentPP.at(slotIndex) > 0)
				{
					mCurrentPP.at(slotIndex)--;
				}
			}

			/*! @brief Determines whether current health is zero.
				@return true when the Pokemon has no health remaining; otherwise false.
				@since 0.8.1
				@version 0.12.2
			*/
			ATTR_NODISCARD constexpr bool isFainted() const
			{
				return mHealth == 0;
			}

			/*! @brief Applies a registered non-volatile status according to its interactions with the current statuses.
				@details Blocking interactions leave the array unchanged. Replacement interactions store the incoming status in place, while
			   removal interactions clear matching statuses and compact the remaining active statuses before insertion. A status whose
			   registry metadata classifies it as @ref PocketCore::Status::StatusClassification::Volatile is battle-slot-owned and is
			   rejected here, leaving the non-volatile array unchanged.
				@param[in] statusID The registered status identifier to apply. @ref NO_STATUS_ID is ignored.
				@param[in] statusRegistry The registry used to resolve the incoming status metadata.
				@param[in] policy The ruleset policy governing status interactions, including the maximum number of active statuses and
			   replacement behavior when full.
				@return An empty result when the status is applied or is a benign no-op; otherwise the @ref
			   PocketCore::Interaction::InteractionApplicationError describing the rejection.
				@since 0.9.11
				@version 0.12.46
			*/
			constexpr std::expected<void, InteractionApplicationError> addNonVolatileStatus(const StatusID statusID,
																							const StatusRegistry &statusRegistry,
																							const RulesetPolicy &policy)
			{
				const StatusMeta *metadata{statusRegistry.getStatusMetadata(statusID)};

				// A volatile status belongs to the battle slot; it must never occupy Pokemon-owned non-volatile storage.
				if (metadata != nullptr && metadata->mStatusClassification == StatusClassification::Volatile)
				{
					return std::unexpected(InteractionApplicationError::WrongClassification);
				}

				return applyInteractions(statusID, NO_STATUS_ID, statusRegistry, mNonVolatileStatusIDs, &StatusMeta::mStatusInteractions,
										 policy.mMaxNonVolatileStatuses, policy.mReplaceNonVolatileStatusWhenFull);
			}

			/*! @brief Writes the Pokemon's raw identifier and statistic representation to a stream.
				@param[in,out] outStream The stream receiving the representation.
				@param[in] pokemon The Pokemon to write.
				@return The supplied stream after writing the representation.
				@since 0.11.2
				@version 0.12.45
			*/
			friend std::ostream &operator<<(std::ostream &outStream, const Pokemon &pokemon);

		private:
			/*! @brief Recomputes the Pokemon's calculated stats based on its base stats, IVs, EVs, level, and nature multipliers.
				@details Owns the stat calculation formula. Constructors and public stat-input setters invoke this function after updating
			   inputs so calculated stats are immediately consistent on return. Adjusts current health by the change in calculated maximum
			   health, preserving the amount of missing health. A Pokemon with zero health stays fainted; a living Pokemon is never reduced
			   below 1 health by a stat change.
				@since 0.12.23
				@version 0.12.49
			*/
			constexpr void recomputeStats()
			{
				const auto getBaseComponent = [this](const ub index, const us baseStat) -> us {
					const ui ivAndEvCalc{
						mPokemonIVs.at(index) + static_cast<ui>(std::floor(mPokemonEVs.at(index) / CALCULATED_EV_DIVISOR)),
					};

					const ui numerator{((CALCULATED_NUMERATOR_MULTIPLIER * baseStat) + ivAndEvCalc) * mLevel};

					return static_cast<us>(std::floor(numerator / CALCULATED_NUMERATOR_DIVISOR));
				};

				constexpr std::size_t healthIndex{toIndex(PokemonStat::Health)};
				constexpr std::size_t attackIndex{toIndex(PokemonStat::Attack)};
				constexpr std::size_t defenseIndex{toIndex(PokemonStat::Defense)};
				constexpr std::size_t specialAttackIndex{toIndex(PokemonStat::SpecialAttack)};
				constexpr std::size_t specialDefenseIndex{toIndex(PokemonStat::SpecialDefense)};
				constexpr std::size_t speedIndex{toIndex(PokemonStat::Speed)};

				const us previousMaxHealth{mCalculatedStats.mMaxHealth};
				mCalculatedStats.mMaxHealth
					= static_cast<us>(getBaseComponent(healthIndex, mBaseStats.mMaxHealth) + mLevel + CALCULATED_HEALTH_OFFSET);

				mCalculatedStats.mAttack = static_cast<us>(getBaseComponent(attackIndex, mBaseStats.mAttack) + CALCULATED_STAT_OFFSET);
				mCalculatedStats.mDefense = static_cast<us>(getBaseComponent(defenseIndex, mBaseStats.mDefense) + CALCULATED_STAT_OFFSET);
				mCalculatedStats.mSpAttack
					= static_cast<us>(getBaseComponent(specialAttackIndex, mBaseStats.mSpAttack) + CALCULATED_STAT_OFFSET);
				mCalculatedStats.mSpDefense
					= static_cast<us>(getBaseComponent(specialDefenseIndex, mBaseStats.mSpDefense) + CALCULATED_STAT_OFFSET);
				mCalculatedStats.mSpeed = static_cast<us>(getBaseComponent(speedIndex, mBaseStats.mSpeed) + CALCULATED_STAT_OFFSET);

				for (const std::array<double, POKEMON_STAT_COUNT> &natureMultiplier : mNatureMultipliers)
				{
					mCalculatedStats.mMaxHealth
						= static_cast<us>(mCalculatedStats.mMaxHealth * natureMultiplier.at(toIndex(PokemonStat::Health)));
					mCalculatedStats.mAttack
						= static_cast<us>(mCalculatedStats.mAttack * natureMultiplier.at(toIndex(PokemonStat::Attack)));
					mCalculatedStats.mDefense
						= static_cast<us>(mCalculatedStats.mDefense * natureMultiplier.at(toIndex(PokemonStat::Defense)));
					mCalculatedStats.mSpAttack
						= static_cast<us>(mCalculatedStats.mSpAttack * natureMultiplier.at(toIndex(PokemonStat::SpecialAttack)));
					mCalculatedStats.mSpDefense
						= static_cast<us>(mCalculatedStats.mSpDefense * natureMultiplier.at(toIndex(PokemonStat::SpecialDefense)));
					mCalculatedStats.mSpeed = static_cast<us>(mCalculatedStats.mSpeed * natureMultiplier.at(toIndex(PokemonStat::Speed)));
				}

				if (mHealth > 0)
				{
					if (mCalculatedStats.mMaxHealth > previousMaxHealth)
					{
						const ui healthIncrease{static_cast<ui>(mCalculatedStats.mMaxHealth - previousMaxHealth)};
						const ui adjustedHealth{static_cast<ui>(mHealth) + healthIncrease};
						mHealth = static_cast<us>(std::min(adjustedHealth, static_cast<ui>(mCalculatedStats.mMaxHealth)));
					}
					else
					{
						const us healthDecrease{static_cast<us>(previousMaxHealth - mCalculatedStats.mMaxHealth)};
						mHealth = mHealth > healthDecrease ? static_cast<us>(mHealth - healthDecrease) : 1;
					}
				}
			}

			/*! @brief Resolves and stores the nature multipliers for the supplied nature identifiers.
				@details Looks up each nature's metadata in @p natureRegistry and stages its stat multipliers before committing. The
			   identifiers and resolved multipliers are replaced once every lookup succeeds. An unregistered nature retains the existing
			   identifiers and multiplier rows without installing fallback values. Does not recalculate stats; callers invoke @ref
			   recomputeStats after either lookup outcome.
				@pre @p natureRegistry must outlive this call; @ref Pokemon does not own the registry.
				@post On success, @ref mNatureIDs equals @p natureIDs and @ref mNatureMultipliers holds the resolved multipliers; on
			   lookup failure, both identifiers and multipliers remain unchanged.
				@param[in] natureIDs The nature identifier slots to resolve.
				@param[in] natureRegistry The registry used to resolve each nature's stat multipliers.
				@note During construction, a lookup failure preserves the neutral defaults of @ref mNatureMultipliers.
				@since 0.12.47
				@version 0.12.49
			*/
			constexpr void resolveNatureMultipliers(const std::array<NatureID, MAX_NATURES_PER_POKEMON> &natureIDs,
													const NatureRegistry &natureRegistry)
			{
				std::array<std::array<double, POKEMON_STAT_COUNT>, MAX_NATURES_PER_POKEMON> tempValues{};

				for (std::size_t i{0}; i < natureIDs.size(); ++i)
				{
					const NatureMeta *metadata{natureRegistry.getNatureMetadata(natureIDs.at(i))};

					if (metadata == nullptr)
					{
						return;
					}

					tempValues.at(i) = metadata->mStatMultipliers;
				}

				mNatureIDs = natureIDs;
				mNatureMultipliers = tempValues;
			}

			/*! @details Stores the level and recomputes its damage factor without updating the calculated stats.
				@param[in] level The new Pokemon level.
				@note Callers must invoke @ref recomputeStats after completing updates to the stat inputs.
				@since 0.12.48
				@version 0.12.48
			*/
			constexpr void updateLevel(const us level)
			{
				mLevel = level;
				mLevelDamageFactor = static_cast<us>(std::floor((LEVEL_DAMAGE_FACTOR_NUMERATOR * level) / LEVEL_DAMAGE_FACTOR_DENOMINATOR)
													 + LEVEL_DAMAGE_FACTOR_OFFSET);
			}

			/*! @details Validates and stores all IVs without updating the calculated stats. Valid arrays are copied unchanged; if any
				value is invalid, the array is not changed.
				@param[in] pokemonIVs The individual values to validate and copy; no reference to the input is retained.
				@note Callers must invoke @ref recomputeStats after completing updates to the stat inputs.
				@since 0.12.48
				@version 0.12.48
			*/
			constexpr void updatePokemonIVsArray(const std::array<us, POKEMON_STAT_COUNT> &pokemonIVs)
			{
				const std::expected<void, PokemonError> result{isValidPokemonIVArray(pokemonIVs)};

				if (result.has_value())
				{
					mPokemonIVs = pokemonIVs;
				}
			}

			/*! @details Validates and stores all EVs without updating the calculated stats. Valid arrays are copied unchanged; an array
				that violates the configured per-stat or aggregate EV limits does not change the array.
				@param[in] pokemonEVs The effort values to validate and copy; no reference to the input is retained.
				@note Callers must invoke @ref recomputeStats after completing updates to the stat inputs.
				@since 0.12.48
				@version 0.12.48
			*/
			constexpr void updatePokemonEVsArray(const std::array<us, POKEMON_STAT_COUNT> &pokemonEVs)
			{
				const std::expected<void, PokemonError> result{isValidPokemonEVArray(pokemonEVs)};

				if (result.has_value())
				{
					mPokemonEVs = pokemonEVs;
				}
			}

		private:
			/*! @brief The nature multipliers affecting the Pokemon's stats. */
			std::array<std::array<double, POKEMON_STAT_COUNT>, MAX_NATURES_PER_POKEMON> mNatureMultipliers{
				[] {
					std::array<std::array<double, POKEMON_STAT_COUNT>, MAX_NATURES_PER_POKEMON> natureMultipliers{};

					for (auto &natureMultiplierRow : natureMultipliers)
					{
						natureMultiplierRow.fill(NATURE_STAT_BASE_MULTIPLIER);
					}
					return natureMultipliers;
				}(),
			};

			/*! @brief The non-owning display name. */
			std::string_view mName{};

			/*! @brief The base stats for the Pokemon species. */
			PokemonStats mBaseStats{};

			/*! @brief The calculated stats for the Pokemon, considering IVs, EVs, and other modifiers. */
			PokemonStats mCalculatedStats{};

			/*! @brief The individual values (IVs) for the Pokemon's stats. */
			std::array<us, POKEMON_STAT_COUNT> mPokemonIVs{};
			/*! @brief The effort values (EVs) for the Pokemon's stats. */
			std::array<us, POKEMON_STAT_COUNT> mPokemonEVs{};

			/*! @brief The owned non-volatile status identifier slots. */
			std::array<StatusID, MAX_NON_VOLATILE_STATUSES_PER_POKEMON> mNonVolatileStatusIDs{};

			/*! @brief The owned move identifier slots. */
			std::array<MoveID, MAX_MOVES_PER_POKEMON> mMoveIDs{};

			/*! @brief The maximum PP values for each move slot. */
			std::array<ub, MAX_MOVES_PER_POKEMON> mMaxPP{};
			/*! @brief The current PP values for each move slot. */
			std::array<ub, MAX_MOVES_PER_POKEMON> mCurrentPP{};
			/*! @brief The owned type identifier slots. */
			std::array<TypeID, MAX_TYPES_PER_POKEMON> mTypeIDs{};

			/*! @brief The owned ability identifier slots. */
			std::array<AbilityID, MAX_ABILITIES_PER_POKEMON> mAbilityIDs{};
			/*! @brief The owned held-item identifier slots. */
			std::array<ItemID, MAX_ITEMS_PER_POKEMON> mItemIDs{};
			/*! @brief The owned nature identifier slots. */
			std::array<NatureID, MAX_NATURES_PER_POKEMON> mNatureIDs{};

			/*! @brief The current health value for the Pokemon. */
			us mHealth{};

			/*! @brief The current level of the Pokemon. */
			us mLevel{};

			/*! @brief The derived factor used by level-scaled damage calculations. */
			us mLevelDamageFactor{};

			/*! @brief The stable identifier for the Pokemon species. */
			PokemonID mPokemonID{};

			/*! @brief The stable identifier for the Learnset species. */
			LearnsetID mLearnsetID{};
	};
} // namespace PocketCore::Pokemon

#endif
