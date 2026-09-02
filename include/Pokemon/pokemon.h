/*! @file pokemon.h
	@brief Contains the pokemon
	@date 09/02/2026
	@since 0.3.0
	@version 0.12.17
	@author Matthew Moore
*/

#ifndef INCLUDE_POKEMON_POKEMON_H
#define INCLUDE_POKEMON_POKEMON_H

#include <array>
#include <cassert>
#include <cmath>
#include <ostream>
#include <string_view>

#include "Ability/abilityID.h"
#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Interaction/interactionHelpers.h"
#include "Item/itemID.h"
#include "Move/moveID.h"
#include "Nature/natureID.h"
#include "Registry/registryProvider.h"
#include "Registry/statusRegistry.h"
#include "Status/statusID.h"
#include "Status/statusMeta.h"
#include "Types/typeID.h"

namespace PocketCore::Pokemon
{
	using PocketCore::Ability::AbilityID;
	using PocketCore::Configuration::LEVEL_DAMAGE_FACTOR_DENOMINATOR;
	using PocketCore::Configuration::LEVEL_DAMAGE_FACTOR_NUMERATOR;
	using PocketCore::Configuration::LEVEL_DAMAGE_FACTOR_OFFSET;
	using PocketCore::Configuration::MAX_ABILITIES_PER_POKEMON;
	using PocketCore::Configuration::MAX_ITEMS_PER_POKEMON;
	using PocketCore::Configuration::MAX_MOVES_PER_POKEMON;
	using PocketCore::Configuration::MAX_NATURES_PER_POKEMON;
	using PocketCore::Configuration::MAX_STATUSES_PER_POKEMON;
	using PocketCore::Configuration::MAX_TYPES_PER_POKEMON;
	using PocketCore::Core::ub;
	using PocketCore::Core::us;
	using PocketCore::Interaction::applyInteractions;
	using PocketCore::Item::ItemID;
	using PocketCore::Move::MoveID;
	using PocketCore::Nature::NatureID;
	using PocketCore::Registry::RegistryProvider;
	using PocketCore::Registry::Status::StatusRegistry;
	using PocketCore::Status::NO_STATUS_ID;
	using PocketCore::Status::StatusID;
	using PocketCore::Status::StatusMeta;
	using PocketCore::Type::TypeID;

	/*! @class Pokemon Pokemon/pokemon.h
		@brief Stores a Pokemon's identity, battle statistics, moves, held items, abilities, types, natures, and statuses.
		@details The class owns all identifier arrays and scalar battle state. The display name is a non-owning string view whose backing
		 storage must remain valid for the lifetime of the Pokemon object. Indexed accessors and mutators require an index within the
		 corresponding fixed-size array.
		@warning A Pokemon does not own the registry objects passed to its status operations or used by formatting helpers.
		@date 09/02/2026
		@since 0.3.0
		@version 0.12.17
		@author Matthew Moore
	*/
	class Pokemon
	{
		public:
			// Constructors

			/*! @brief Constructs a Pokemon with empty move slots and zero move PP.
				@param[in] name Non-owning display-name view whose backing storage must outlive the object.
				@param[in] attack Base physical Attack statistic.
				@param[in] defense Base physical Defense statistic.
				@param[in] health Maximum and initial health value.
				@param[in] speed Base Speed statistic.
				@param[in] spAttack Base Special Attack statistic.
				@param[in] spDefense Base Special Defense statistic.
				@param[in] level Pokemon level used to compute the level damage factor.
				@param[in] abilityIDs Fixed ability identifier slots.
				@param[in] itemIDs Fixed held-item identifier slots.
				@param[in] typeIDs Fixed type identifier slots
				@since 0.3.0
				@version 0.12.17
			*/
			explicit constexpr Pokemon(const std::string_view &name, const us health, const us attack, const us defense, const us spAttack,
									   const us spDefense, const us speed, const us level,
									   const std::array<AbilityID, MAX_ABILITIES_PER_POKEMON> abilityIDs,
									   const std::array<ItemID, MAX_ITEMS_PER_POKEMON> itemIDs,
									   const std::array<TypeID, MAX_TYPES_PER_POKEMON> typeIDs)
				: mName{name}, mTypeIDs{typeIDs}, mAbilityIDs{abilityIDs}, mItemIDs{itemIDs}, mMaxHealth{health}, mHealth{health},
				  mAttack{attack}, mDefense{defense}, mSpAttack{spAttack}, mSpDefense{spDefense}, mSpeed{speed}
			{
				mMoveIDs.fill(PocketCore::Move::NO_MOVE_ID);
				mMaxPP.fill(0);
				mCurrentPP.fill(0);
				setLevel(level);
			}

			/*! @brief Constructs a Pokemon from complete move and PP arrays.
				@param[in] name Non-owning display-name view whose backing storage must outlive the object.
				@param[in] moveIDs Fixed move identifier slots.
				@param[in] maxPP Maximum PP for each move slot.
				@param[in] currentPP Current PP for each move slot.
				@param[in] attack Base physical Attack statistic.
				@param[in] defense Base physical Defense statistic.
				@param[in] health Maximum and initial health value.
				@param[in] speed Base Speed statistic.
				@param[in] spAttack Base Special Attack statistic.
				@param[in] spDefense Base Special Defense statistic.
				@param[in] level Pokemon level used to compute the level damage factor.
				@param[in] abilityIDs Fixed ability identifier slots.
				@param[in] itemIDs Fixed held-item identifier slots.
				@param[in] typeIDs Fixed type identifier slots
				@since 0.3.0
				@version 0.12.17
			*/
			explicit constexpr Pokemon(const std::string_view &name, const std::array<MoveID, MAX_MOVES_PER_POKEMON> moveIDs,
									   const std::array<ub, MAX_MOVES_PER_POKEMON> maxPP,
									   const std::array<ub, MAX_MOVES_PER_POKEMON> currentPP, const us health, const us attack,
									   const us defense, const us spAttack, const us spDefense, const us speed, const us level,
									   const std::array<AbilityID, MAX_ABILITIES_PER_POKEMON> abilityIDs,
									   const std::array<ItemID, MAX_ITEMS_PER_POKEMON> itemIDs,
									   const std::array<TypeID, MAX_TYPES_PER_POKEMON> typeIDs)
				: mName{name}, mMoveIDs{moveIDs}, mMaxPP{maxPP}, mCurrentPP{currentPP}, mTypeIDs{typeIDs}, mAbilityIDs{abilityIDs},
				  mItemIDs{itemIDs}, mMaxHealth{health}, mHealth{health}, mAttack{attack}, mDefense{defense}, mSpAttack{spAttack},
				  mSpDefense{spDefense}, mSpeed{speed}
			{
				setLevel(level);
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
				@version 0.12.17
			*/
			ATTR_NODISCARD constexpr const std::array<StatusID, MAX_STATUSES_PER_POKEMON> &getStatusIDsArray() const
			{
				return mStatusIDs;
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
				@param[in] index Status slot index; must be less than MAX_STATUSES_PER_POKEMON.
				@return The status identifier stored in the slot.
				@pre index < MAX_STATUSES_PER_POKEMON; violation triggers an assertion.
				@since 0.8.1
				@version 0.12.2
			*/
			ATTR_NODISCARD constexpr StatusID getStatusID(const us index) const
			{
				assert(index < mStatusIDs.size());

				return mStatusIDs.at(index);
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

			/*! @brief Returns maximum health.
				@return The maximum health value.
				@since 0.9.14
				@version 0.9.14
			*/
			ATTR_NODISCARD constexpr us getMaximumHealth() const
			{
				return mMaxHealth;
			}

			/*! @brief Returns the base Attack statistic.
				@return The Attack value.
				@since 0.3.0
				@version 0.12.2
			*/
			ATTR_NODISCARD constexpr us getAttack() const
			{
				return mAttack;
			}

			/*! @brief Returns the base Defense statistic.
				@return The Defense value.
				@since 0.3.0
				@version 0.12.2
			*/
			ATTR_NODISCARD constexpr us getDefense() const
			{
				return mDefense;
			}

			/*! @brief Returns the base Special Attack statistic.
				@return The Special Attack value.
				@since 0.3.0
				@version 0.12.2
			*/
			ATTR_NODISCARD constexpr us getSpAttack() const
			{
				return mSpAttack;
			}

			/*! @brief Returns the base Special Defense statistic.
				@return The Special Defense value.
				@since 0.3.0
				@version 0.12.2
			*/
			ATTR_NODISCARD constexpr us getSpDefense() const
			{
				return mSpDefense;
			}

			/*! @brief Returns the base Speed statistic.
				@return The Speed value.
				@since 0.3.0
				@version 0.12.2
			*/
			ATTR_NODISCARD constexpr us getSpeed() const
			{
				return mSpeed;
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
				@version 0.12.17
			*/
			constexpr void setStatusIDsArray(const std::array<StatusID, MAX_STATUSES_PER_POKEMON> &statusIDs)
			{
				mStatusIDs = statusIDs;
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

			/*! @brief Replaces all nature identifier slots.
				@param[in] natureIDs The nature identifiers to store.
				@since 0.11.6
				@version 0.12.2
			*/
			constexpr void setNatureIDsArray(const std::array<NatureID, MAX_NATURES_PER_POKEMON> &natureIDs)
			{
				mNatureIDs = natureIDs;
			}

			/*! @brief Sets one status slot.
				@param[in] slotIndex Status slot index; must be less than MAX_STATUSES_PER_POKEMON.
				@param[in] statusID The status identifier to store.
				@pre slotIndex < MAX_STATUSES_PER_POKEMON; violation triggers an assertion.
				@since 0.12.17
				@version 0.12.17
			*/
			constexpr void setStatusID(const ub slotIndex, const StatusID statusID)
			{
				assert(slotIndex < mStatusIDs.size());

				mStatusIDs.at(slotIndex) = statusID;
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

			/*! @brief Sets one nature slot.
				@param[in] slotIndex Nature slot index; must be less than MAX_NATURES_PER_POKEMON.
				@param[in] natureID The nature identifier to store.
				@pre slotIndex < MAX_NATURES_PER_POKEMON; violation triggers an assertion.
				@since 0.11.6
				@version 0.12.17
			*/
			constexpr void setNatureID(const ub slotIndex, const NatureID natureID)
			{
				assert(slotIndex < mNatureIDs.size());

				mNatureIDs.at(slotIndex) = natureID;
			}

			/*! @brief Sets current health, clamped to maximum health.
				@param[in] health The requested current health value.
				@since 0.3.0
				@version 0.12.2
			*/
			constexpr void setHealth(const us health)
			{
				mHealth = std::min(health, mMaxHealth);
			}

			/*! @brief Sets maximum health and clamps current health to the new maximum.
				@param[in] maximumHealth The new maximum health value.
				@since 0.9.14
				@version 0.12.2
			*/
			constexpr void setMaximumHealth(const us maximumHealth)
			{
				mMaxHealth = maximumHealth;
				mHealth = std::min(mHealth, mMaxHealth);
			}

			/*! @brief Replaces the base Attack statistic.
				@param[in] attack The new Attack value.
				@since 0.3.0
				@version 0.12.2
			*/
			constexpr void setAttack(const us attack)
			{
				mAttack = attack;
			}

			/*! @brief Replaces the base Defense statistic.
				@param[in] defense The new Defense value.
				@since 0.3.0
				@version 0.12.2
			*/
			constexpr void setDefense(const us defense)
			{
				mDefense = defense;
			}

			/*! @brief Replaces the base Special Attack statistic.
				@param[in] spAttack The new Special Attack value.
				@since 0.3.0
				@version 0.12.2
			*/
			constexpr void setSpAttack(const us spAttack)
			{
				mSpAttack = spAttack;
			}

			/*! @brief Replaces the base Special Defense statistic.
				@param[in] spDefense The new Special Defense value.
				@since 0.3.0
				@version 0.12.2
			*/
			constexpr void setSpDefense(const us spDefense)
			{
				mSpDefense = spDefense;
			}

			/*! @brief Replaces the base Speed statistic.
				@param[in] speed The new Speed value.
				@since 0.3.0
				@version 0.12.2
			*/
			constexpr void setSpeed(const us speed)
			{
				mSpeed = speed;
			}

			/*! @brief Sets the level and recomputes its damage factor.
				@param[in] level The new Pokemon level.
				@since 0.7.2
				@version 0.12.2
			*/
			constexpr void setLevel(const us level)
			{
				mLevel = level;
				mLevelDamageFactor = static_cast<us>(std::floor((LEVEL_DAMAGE_FACTOR_NUMERATOR * level) / LEVEL_DAMAGE_FACTOR_DENOMINATOR)
													 + LEVEL_DAMAGE_FACTOR_OFFSET);
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

			/*! @brief Applies a registered status according to its interactions with the current statuses.
				@details Blocking interactions leave the array unchanged. Replacement interactions store the incoming status in place, while
			   removal interactions clear matching statuses and compact the remaining active statuses before insertion.
				@param[in] statusID The registered status identifier to apply. @ref NO_STATUS_ID is ignored.
				@param[in] statusRegistry The registry used to resolve the incoming status metadata.
				@since 0.9.11
				@version 0.12.16
			*/
			constexpr void addStatus(const StatusID statusID, const StatusRegistry &statusRegistry)
			{
				applyInteractions(statusID, NO_STATUS_ID, statusRegistry, mStatusIDs, &StatusMeta::mStatusInteractions);
			}

			/*! @brief Writes the Pokemon's raw identifier and statistic representation to a stream.
				@param[in,out] outStream The stream receiving the representation.
				@param[in] pokemon The Pokemon to write.
				@return The supplied stream after writing the representation.
				@since 0.11.2
				@version 0.12.17
			*/
			friend std::ostream &operator<<(std::ostream &outStream, const Pokemon &pokemon);

		private:
			/*! @brief The non-owning display name. */
			std::string_view mName{};

			/*! @brief The owned status identifier slots. */
			std::array<StatusID, MAX_STATUSES_PER_POKEMON> mStatusIDs{};

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

			/*! @brief The maximum health value. */
			us mMaxHealth{};
			/*! @brief The current health value. */
			us mHealth{};
			/*! @brief The base Attack statistic. */
			us mAttack{};
			/*! @brief The base Defense statistic. */
			us mDefense{};
			/*! @brief The base Special Attack statistic. */
			us mSpAttack{};
			/*! @brief The base Special Defense statistic. */
			us mSpDefense{};
			/*! @brief The base Speed statistic. */
			us mSpeed{};
			/*! @brief The current level. */
			us mLevel{};
			/*! @brief The derived factor used by level-scaled damage calculations. */
			us mLevelDamageFactor{};
	};

	/*! @brief Writes a Pokemon with stable identifier names resolved from runtime registries.
		@details Ability, item, type, status, and move identifiers are printed with their registered names. Missing registry entries are
	   printed as `<unregistered>`.
		@param[in,out] outStream The stream receiving the formatted Pokemon state.
		@param[in] pokemon The Pokemon whose state is printed.
		@param[in] registryProvider The registries used to resolve stable identifier names. Its registry pointers may be nullptr.
		@return The supplied stream after writing the complete representation.
		@since 0.11.2
		@version 0.12.17
	*/
	std::ostream &printPokemonWithNames(std::ostream &outStream, const Pokemon &pokemon, const RegistryProvider &registryProvider);
} // namespace PocketCore::Pokemon

#endif
