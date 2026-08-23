/*! @file pokemon.h
	@brief Contains the pokemon
	@date 07/24/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_POKEMON_POKEMON_H
#define INCLUDE_POKEMON_POKEMON_H

#include <array>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <ostream>
#include <string_view>

#include "Ability/abilityID.h"
#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Item/itemID.h"
#include "Move/moveID.h"
#include "Nature/natureID.h"
#include "Registry/registryProvider.h"
#include "Registry/statusRegistry.h"
#include "Status/statusHelpers.h"
#include "Status/statusID.h"
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
	using PocketCore::Item::ItemID;
	using PocketCore::Move::MoveID;
	using PocketCore::Nature::NatureID;
	using PocketCore::Registry::RegistryProvider;
	using PocketCore::Registry::Status::StatusRegistry;
	using PocketCore::Status::hasInteraction;
	using PocketCore::Status::NO_STATUS_ID;
	using PocketCore::Status::shiftAndGetNextAvailableStatus;
	using PocketCore::Status::statusAlreadyExists;
	using PocketCore::Status::StatusID;
	using PocketCore::Status::StatusInteractionAction;
	using PocketCore::Status::statusRemoveHandler;
	using PocketCore::Status::statusReplaceHandler;
	using PocketCore::Status::willBlockIncoming;
	using PocketCore::Types::TypeID;

	class Pokemon
	{
		public:
			// Constructors

			explicit constexpr Pokemon(const std::string_view name, const us attack, const us defense, const us health, const us speed,
									   const us spAttack, const us spDefense, const us level,
									   const std::array<AbilityID, MAX_ABILITIES_PER_POKEMON> abilityIDs,
									   const std::array<ItemID, MAX_ITEMS_PER_POKEMON> itemIDs,
									   const std::array<TypeID, MAX_TYPES_PER_POKEMON> typeIDs = {})
				: mName{name}, mTypeIDs{typeIDs}, mAbilityIDs{abilityIDs}, mItemIDs{itemIDs}, mAttack{attack}, mDefense{defense},
				  mMaxHealth{health}, mHealth{health}, mSpeed{speed}, mSpAttack{spAttack}, mSpDefense{spDefense}
			{
				mMoveIDs.fill(PocketCore::Move::NO_MOVE_ID);
				mMaxPP.fill(0);
				mCurrentPP.fill(0);
				setLevel(level);
			}

			explicit constexpr Pokemon(const std::string_view name, const std::array<MoveID, MAX_MOVES_PER_POKEMON> moveIDs,
									   const std::array<ub, MAX_MOVES_PER_POKEMON> maxPP,
									   const std::array<ub, MAX_MOVES_PER_POKEMON> currentPP, const us attack, const us defense,
									   const us health, const us speed, const us spAttack, const us spDefense, const us level,
									   const std::array<AbilityID, MAX_ABILITIES_PER_POKEMON> abilityIDs,
									   const std::array<ItemID, MAX_ITEMS_PER_POKEMON> itemIDs,
									   const std::array<TypeID, MAX_TYPES_PER_POKEMON> typeIDs = {})
				: mName{name}, mMoveIDs{moveIDs}, mMaxPP{maxPP}, mCurrentPP{currentPP}, mTypeIDs{typeIDs}, mAbilityIDs{abilityIDs},
				  mItemIDs{itemIDs}, mAttack{attack}, mDefense{defense}, mMaxHealth{health}, mHealth{health}, mSpeed{speed},
				  mSpAttack{spAttack}, mSpDefense{spDefense}
			{
				setLevel(level);
			}

			// Getters

			ATTR_NODISCARD constexpr const std::string_view &getName() const
			{
				return mName;
			}

			ATTR_NODISCARD constexpr const std::array<StatusID, MAX_STATUSES_PER_POKEMON> &getStatusesArray() const
			{
				return mStatusIDs;
			}

			ATTR_NODISCARD constexpr const std::array<MoveID, MAX_MOVES_PER_POKEMON> &getMovesArray() const
			{
				return mMoveIDs;
			}

			ATTR_NODISCARD constexpr MoveID getMoveID(const us index) const
			{
				assert(index < mMoveIDs.size());

				return mMoveIDs.at(index);
			}

			ATTR_NODISCARD constexpr const std::array<ub, MAX_MOVES_PER_POKEMON> &getMaxPPArray() const
			{
				return mMaxPP;
			}

			ATTR_NODISCARD constexpr ub getMaxPP(const us index) const
			{
				assert(index < mMaxPP.size());

				return mMaxPP.at(index);
			}

			ATTR_NODISCARD constexpr const std::array<ub, MAX_MOVES_PER_POKEMON> &getCurrentPPArray() const
			{
				return mCurrentPP;
			}

			ATTR_NODISCARD constexpr ub getCurrentPP(const us index) const
			{
				assert(index < mCurrentPP.size());

				return mCurrentPP.at(index);
			}

			ATTR_NODISCARD constexpr const std::array<TypeID, MAX_TYPES_PER_POKEMON> &getTypesArray() const noexcept
			{
				return mTypeIDs;
			}

			ATTR_NODISCARD constexpr const std::array<AbilityID, MAX_ABILITIES_PER_POKEMON> &getAbilitiesArray() const noexcept
			{
				return mAbilityIDs;
			}

			ATTR_NODISCARD constexpr const std::array<ItemID, MAX_ITEMS_PER_POKEMON> &getItemsArray() const noexcept
			{
				return mItemIDs;
			}

			ATTR_NODISCARD constexpr const std::array<NatureID, MAX_NATURES_PER_POKEMON> &getNatureIDsArray() const noexcept
			{
				return mNatureIDs;
			}

			ATTR_NODISCARD constexpr TypeID getTypeID(const ub index) const
			{
				assert(index < mTypeIDs.size());

				return mTypeIDs.at(index);
			}

			ATTR_NODISCARD constexpr AbilityID getAbilityID(const ub index) const
			{
				assert(index < mAbilityIDs.size());

				return mAbilityIDs.at(index);
			}

			ATTR_NODISCARD constexpr ItemID getItemID(const ub index) const
			{
				assert(index < mItemIDs.size());

				return mItemIDs.at(index);
			}

			ATTR_NODISCARD constexpr NatureID getNatureID(const ub index) const
			{
				assert(index < mNatureIDs.size());

				return mNatureIDs.at(index);
			}

			ATTR_NODISCARD constexpr us getAttack() const
			{
				return mAttack;
			}

			ATTR_NODISCARD constexpr us getDefense() const
			{
				return mDefense;
			}

			ATTR_NODISCARD constexpr us getHealth() const
			{
				return mHealth;
			}

			ATTR_NODISCARD constexpr us getMaximumHealth() const
			{
				return mMaxHealth;
			}

			ATTR_NODISCARD constexpr us getSpeed() const
			{
				return mSpeed;
			}

			ATTR_NODISCARD constexpr us getSpAttack() const
			{
				return mSpAttack;
			}

			ATTR_NODISCARD constexpr us getSpDefense() const
			{
				return mSpDefense;
			}

			ATTR_NODISCARD constexpr us getLevel() const
			{
				return mLevel;
			}

			ATTR_NODISCARD constexpr us getLevelDamageFactor() const
			{
				return mLevelDamageFactor;
			}

			ATTR_NODISCARD constexpr StatusID getStatusID(const us index) const
			{
				assert(index < mStatusIDs.size());

				return mStatusIDs.at(index);
			}

			// Setters

			constexpr void setName(const std::string_view name)
			{
				mName = name;
			}

			constexpr void setStatusesArray(const std::array<StatusID, MAX_STATUSES_PER_POKEMON> &statusIDs)
			{
				mStatusIDs = statusIDs;
			}

			constexpr void setMovesArray(const std::array<MoveID, MAX_MOVES_PER_POKEMON> &moveIDs)
			{
				mMoveIDs = moveIDs;
			}

			constexpr void setMove(const ub slotIndex, const MoveID moveID)
			{
				assert(slotIndex < mMoveIDs.size());

				mMoveIDs.at(slotIndex) = moveID;
			}

			constexpr void setMaxPPArray(const std::array<ub, MAX_MOVES_PER_POKEMON> &maxPP)
			{
				mMaxPP = maxPP;
			}

			constexpr void setMaxPP(const ub slotIndex, const ub maxPP)
			{
				assert(slotIndex < mMaxPP.size());

				mMaxPP.at(slotIndex) = maxPP;
			}

			constexpr void setCurrentPPArray(const std::array<ub, MAX_MOVES_PER_POKEMON> &currentPP)
			{
				mCurrentPP = currentPP;
			}

			constexpr void setCurrentPP(const ub slotIndex, const ub currentPP)
			{
				assert(slotIndex < mCurrentPP.size());

				mCurrentPP.at(slotIndex) = currentPP;
			}

			constexpr void setTypesArray(const std::array<TypeID, MAX_TYPES_PER_POKEMON> &typeIDs)
			{
				mTypeIDs = typeIDs;
			}

			constexpr void setAbilityIDsArray(const std::array<AbilityID, MAX_ABILITIES_PER_POKEMON> &abilityIDs)
			{
				mAbilityIDs = abilityIDs;
			}

			constexpr void setItemIDsArray(const std::array<ItemID, MAX_ITEMS_PER_POKEMON> &itemIDs)
			{
				mItemIDs = itemIDs;
			}

			constexpr void setNatureIDsArray(const std::array<NatureID, MAX_NATURES_PER_POKEMON> &natureIDs)
			{
				mNatureIDs = natureIDs;
			}

			constexpr void setType(const ub slotIndex, const TypeID typeID)
			{
				assert(slotIndex < mTypeIDs.size());

				mTypeIDs.at(slotIndex) = typeID;
			}

			constexpr void setAbility(const ub slotIndex, const AbilityID abilityID)
			{
				assert(slotIndex < mAbilityIDs.size());

				mAbilityIDs.at(slotIndex) = abilityID;
			}

			constexpr void setItem(const ub slotIndex, const ItemID itemID)
			{
				assert(slotIndex < mItemIDs.size());

				mItemIDs.at(slotIndex) = itemID;
			}

			constexpr void setNature(const ub slotIndex, const NatureID natureID)
			{
				assert(slotIndex < mNatureIDs.size());

				mNatureIDs.at(slotIndex) = natureID;
			}

			constexpr void setAttack(const us attack)
			{
				mAttack = attack;
			}

			constexpr void setDefense(const us defense)
			{
				mDefense = defense;
			}

			constexpr void setHealth(const us health)
			{
				mHealth = std::min(health, mMaxHealth);
			}

			constexpr void setMaximumHealth(const us maximumHealth)
			{
				mMaxHealth = maximumHealth;
				mHealth = std::min(mHealth, mMaxHealth);
			}

			constexpr void setSpeed(const us speed)
			{
				mSpeed = speed;
			}

			constexpr void setSpAttack(const us spAttack)
			{
				mSpAttack = spAttack;
			}

			constexpr void setSpDefense(const us spDefense)
			{
				mSpDefense = spDefense;
			}

			constexpr void setLevel(const us level)
			{
				mLevel = level;
				mLevelDamageFactor = static_cast<us>(std::floor((LEVEL_DAMAGE_FACTOR_NUMERATOR * level) / LEVEL_DAMAGE_FACTOR_DENOMINATOR)
													 + LEVEL_DAMAGE_FACTOR_OFFSET);
			}

			// Utility Functions

			constexpr void usePP(const ub slotIndex)
			{
				assert(slotIndex < mCurrentPP.size());

				if (mCurrentPP.at(slotIndex) > 0)
				{
					mCurrentPP.at(slotIndex)--;
				}
			}

			ATTR_NODISCARD constexpr bool isFainted() const
			{
				return mHealth == 0;
			}

			/*! @brief Applies a registered status according to its interactions with the current statuses.
				@details Blocking interactions leave the array unchanged. Replacement interactions store the incoming status in place, while
			   removal interactions clear matching statuses and compact the remaining active statuses before insertion.
				@param[in] statusID The registered status identifier to apply. @ref NO_STATUS_ID is ignored.
				@param[in] statusRegistry The registry used to resolve the incoming status metadata.
			*/
			constexpr void addStatus(const StatusID statusID, const StatusRegistry &statusRegistry)
			{
				if (statusID == NO_STATUS_ID)
				{
					return;
				}

				if (statusAlreadyExists(statusID, mStatusIDs))
				{
					return;
				}

				if (willBlockIncoming(statusID, statusRegistry, mStatusIDs))
				{
					return;
				}

				const bool replacedCurrentStatus{statusReplaceHandler(statusID, statusRegistry, mStatusIDs)};

				statusRemoveHandler(statusID, statusRegistry, mStatusIDs);

				const std::size_t nextActiveStatusIndex{shiftAndGetNextAvailableStatus(mStatusIDs)};

				for (std::size_t index{nextActiveStatusIndex}; index < mStatusIDs.size(); index++)
				{
					mStatusIDs.at(index) = NO_STATUS_ID;
				}

				if (replacedCurrentStatus)
				{
					return;
				}

				if (nextActiveStatusIndex >= mStatusIDs.size())
				{
					return;
				}

				mStatusIDs.at(nextActiveStatusIndex) = statusID;
			}

			friend std::ostream &operator<<(std::ostream &outStream, const Pokemon &pokemon);

		private:
			std::string_view mName{};

			std::array<StatusID, MAX_STATUSES_PER_POKEMON> mStatusIDs{};

			std::array<MoveID, MAX_MOVES_PER_POKEMON> mMoveIDs{};

			std::array<ub, MAX_MOVES_PER_POKEMON> mMaxPP{};
			std::array<ub, MAX_MOVES_PER_POKEMON> mCurrentPP{};
			std::array<TypeID, MAX_TYPES_PER_POKEMON> mTypeIDs{};

			std::array<AbilityID, MAX_ABILITIES_PER_POKEMON> mAbilityIDs{};
			std::array<ItemID, MAX_ITEMS_PER_POKEMON> mItemIDs{};
			std::array<NatureID, MAX_NATURES_PER_POKEMON> mNatureIDs{};

			us mAttack{};
			us mDefense{};
			us mMaxHealth{};
			us mHealth{};
			us mSpeed{};
			us mSpAttack{};
			us mSpDefense{};
			us mLevel{};
			us mLevelDamageFactor{};
	};

	/*! @brief Writes a Pokemon with stable identifier names resolved from runtime registries.
		@details Ability, item, type, status, and move identifiers are printed with their registered names. Missing registry entries are
	   printed as `<unregistered>`.
		@param[in,out] outStream The stream receiving the formatted Pokemon state.
		@param[in] pokemon The Pokemon whose state is printed.
		@param[in] registryProvider The registries used to resolve stable identifier names. Its registry pointers may be nullptr.
		@return The supplied stream after writing the complete representation.
	*/
	std::ostream &printPokemonWithNames(std::ostream &outStream, const Pokemon &pokemon, const RegistryProvider &registryProvider);
} // namespace PocketCore::Pokemon

#endif
