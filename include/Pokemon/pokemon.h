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
#include <string_view>

#include "Ability/abilityID.h"
#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Item/itemID.h"
#include "Move/moveID.h"
#include "Status/statusID.h"
#include "Types/typeID.h"

namespace PocketCore::Pokemon
{
	using PocketCore::Ability::AbilityID;
	using PocketCore::Configuration::LEVEL_DAMAGE_FACTOR_DENOMINATOR;
	using PocketCore::Configuration::LEVEL_DAMAGE_FACTOR_NUMERATOR;
	using PocketCore::Configuration::LEVEL_DAMAGE_FACTOR_OFFSET;
	using PocketCore::Configuration::MAX_MOVES_PER_POKEMON;
	using PocketCore::Configuration::MAX_TYPES_PER_POKEMON;
	using PocketCore::Core::ub;
	using PocketCore::Core::us;
	using PocketCore::Item::ItemID;
	using PocketCore::Move::MoveID;
	using PocketCore::Status::StatusID;
	using PocketCore::Types::TypeID;

	class Pokemon
	{
		public:
			// Constructors

			explicit constexpr Pokemon(const std::string_view name, const us attack, const us defense, const us health, const us speed,
									   const us spAttack, const us spDefense, const us level, const AbilityID abilityID,
									   const ItemID itemID, const std::array<TypeID, MAX_TYPES_PER_POKEMON> typeIDs = {})
				: mName{name}, mTypeIDs{typeIDs}, mAttack{attack}, mDefense{defense}, mHealth{health}, mSpeed{speed}, mSpAttack{spAttack},
				  mSpDefense{spDefense}, mAbilityID{abilityID}, mItemID{itemID}
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
									   const AbilityID abilityID, const ItemID itemID,
									   const std::array<TypeID, MAX_TYPES_PER_POKEMON> typeIDs = {})
				: mName{name}, mMoveIDs{moveIDs}, mMaxPP{maxPP}, mCurrentPP{currentPP}, mTypeIDs{typeIDs}, mAttack{attack},
				  mDefense{defense}, mHealth{health}, mSpeed{speed}, mSpAttack{spAttack}, mSpDefense{spDefense}, mAbilityID{abilityID},
				  mItemID{itemID}
			{
				setLevel(level);
			}

			// Getters

			ATTR_NODISCARD constexpr const std::string_view &getName() const
			{
				return mName;
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

			ATTR_NODISCARD constexpr TypeID getTypeID(const ub index) const
			{
				assert(index < mTypeIDs.size());

				return mTypeIDs.at(index);
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

			ATTR_NODISCARD constexpr AbilityID getAbilityID() const
			{
				return mAbilityID;
			}

			ATTR_NODISCARD constexpr ItemID getItemID() const
			{
				return mItemID;
			}

			ATTR_NODISCARD constexpr StatusID getStatusID() const
			{
				return mStatusID;
			}

			// Setters

			constexpr void setName(const std::string_view name)
			{
				mName = name;
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

			constexpr void setType(const ub slotIndex, const TypeID typeID)
			{
				assert(slotIndex < mTypeIDs.size());

				mTypeIDs.at(slotIndex) = typeID;
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
				mHealth = health;
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

			constexpr void setAbility(const AbilityID abilityID)
			{
				mAbilityID = abilityID;
			}

			constexpr void setItem(const ItemID itemID)
			{
				mItemID = itemID;
			}

			constexpr void setStatus(const StatusID statusID)
			{
				mStatusID = statusID;
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

		private:
			std::string_view mName{};

			std::array<MoveID, MAX_MOVES_PER_POKEMON> mMoveIDs{};
			std::array<ub, MAX_MOVES_PER_POKEMON> mMaxPP{};
			std::array<ub, MAX_MOVES_PER_POKEMON> mCurrentPP{};
			std::array<TypeID, MAX_TYPES_PER_POKEMON> mTypeIDs{};

			us mAttack{};
			us mDefense{};
			us mHealth{};
			us mSpeed{};
			us mSpAttack{};
			us mSpDefense{};
			us mLevel{};
			us mLevelDamageFactor{};

			AbilityID mAbilityID{};
			ItemID mItemID{};
			StatusID mStatusID{};
	};
} // namespace PocketCore::Pokemon

#endif
