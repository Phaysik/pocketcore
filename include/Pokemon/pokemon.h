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
									   const us spAttack, const us spDefense, const AbilityID abilityID, const ItemID itemID,
									   const std::array<TypeID, MAX_TYPES_PER_POKEMON> typeIDs = {})
				: mName{name}, mTypeIDs{typeIDs}, mAttack{attack}, mDefense{defense}, mHealth{health}, mSpeed{speed}, mSpAttack{spAttack},
				  mSpDefense{spDefense}, mAbilityID{abilityID}, mItemID{itemID}
			{
				mMoveIDs.fill(PocketCore::Move::NO_MOVE_ID);
				mMaxPP.fill(0);
				mCurrentPP.fill(0);
			}

			explicit constexpr Pokemon(const std::string_view name, const std::array<MoveID, MAX_MOVES_PER_POKEMON> moveIDs,
									   const std::array<ub, MAX_MOVES_PER_POKEMON> maxPP,
									   const std::array<ub, MAX_MOVES_PER_POKEMON> currentPP, const us attack, const us defense,
									   const us health, const us speed, const us spAttack, const us spDefense, const AbilityID abilityID,
									   const ItemID itemID, const std::array<TypeID, MAX_TYPES_PER_POKEMON> typeIDs = {})
				: mName{name}, mMoveIDs{moveIDs}, mMaxPP{maxPP}, mCurrentPP{currentPP}, mTypeIDs{typeIDs}, mAttack{attack},
				  mDefense{defense}, mHealth{health}, mSpeed{speed}, mSpAttack{spAttack}, mSpDefense{spDefense}, mAbilityID{abilityID},
				  mItemID{itemID}
			{}

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

			void setName(const std::string_view name);

			void setMovesArray(const std::array<MoveID, MAX_MOVES_PER_POKEMON> &moveIDs);

			void setMove(const ub slotIndex, const MoveID moveID);

			void setMaxPPArray(const std::array<ub, MAX_MOVES_PER_POKEMON> &maxPP);

			void setMaxPP(const ub slotIndex, const ub maxPP);

			void setCurrentPPArray(const std::array<ub, MAX_MOVES_PER_POKEMON> &currentPP);

			void setCurrentPP(const ub slotIndex, const ub currentPP);

			void setTypesArray(const std::array<TypeID, MAX_TYPES_PER_POKEMON> &typeIDs);

			void setType(ub slotIndex, TypeID typeID);

			void setAttack(const us attack);

			void setDefense(const us defense);

			void setHealth(const us health);

			void setSpeed(const us speed);

			void setSpAttack(const us spAttack);

			void setSpDefense(const us spDefense);

			void setLevel(const us level);

			void setAbility(const AbilityID abilityID);

			void setItem(const ItemID itemID);

			void setStatus(const StatusID statusID);

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

			AbilityID mAbilityID{};
			ItemID mItemID{};
			StatusID mStatusID{};
	};
} // namespace PocketCore::Pokemon

#endif
