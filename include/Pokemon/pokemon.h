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
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Item/itemID.h"
#include "Move/moveID.h"

namespace PocketCore::Pokemon
{
	using PocketCore::Ability::AbilityID;
	using PocketCore::Core::ub;
	using PocketCore::Core::us;
	using PocketCore::Item::ItemID;
	using PocketCore::Move::MoveID;

	class Pokemon
	{
		public:
			// Constructors

			explicit constexpr Pokemon(const std::string_view name, const us attack, const us defense, const us health, const us speed,
									   const us spAttack, const us spDefense, const AbilityID abilityID, const ItemID itemID)
				: mName{name}, mAttack{attack}, mDefense{defense}, mHealth{health}, mSpeed{speed}, mSpAttack{spAttack},
				  mSpDefense{spDefense}, mAbilityID{abilityID}, mItemID{itemID}
			{
				mMoveIDs.fill(MoveID::None);
				mMaxPP.fill(0);
				mCurrentPP.fill(0);
			}

			explicit constexpr Pokemon(const std::string_view name, const std::array<MoveID, 4> moveIDs, const std::array<ub, 4> maxPP,
									   const std::array<ub, 4> currentPP, const us attack, const us defense, const us health,
									   const us speed, const us spAttack, const us spDefense, const AbilityID abilityID,
									   const ItemID itemID)
				: mName{name}, mMoveIDs{moveIDs}, mMaxPP{maxPP}, mCurrentPP{currentPP}, mAttack{attack}, mDefense{defense}, mHealth{health},
				  mSpeed{speed}, mSpAttack{spAttack}, mSpDefense{spDefense}, mAbilityID{abilityID}, mItemID{itemID}
			{}

			// Getters

			ATTR_NODISCARD constexpr const std::string_view &getName() const
			{
				return mName;
			}

			ATTR_NODISCARD constexpr const std::array<MoveID, 4> &getMovesArray() const
			{
				return mMoveIDs;
			}

			ATTR_NODISCARD constexpr MoveID getMoveID(const us index) const
			{
				assert(index < mMoveIDs.size());

				return mMoveIDs.at(index);
			}

			ATTR_NODISCARD constexpr const std::array<ub, 4> &getMaxPPArray() const
			{
				return mMaxPP;
			}

			ATTR_NODISCARD constexpr ub getMaxPP(const us index) const
			{
				assert(index < mMaxPP.size());

				return mMaxPP.at(index);
			}

			ATTR_NODISCARD constexpr const std::array<ub, 4> &getCurrentPPArray() const
			{
				return mCurrentPP;
			}

			ATTR_NODISCARD constexpr ub getCurrentPP(const us index) const
			{
				assert(index < mCurrentPP.size());

				return mCurrentPP.at(index);
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

			ATTR_NODISCARD constexpr AbilityID getAbilityID() const
			{
				return mAbilityID;
			}

			ATTR_NODISCARD constexpr ItemID getItemID() const
			{
				return mItemID;
			}

			// Setters

			void setName(const std::string_view name);

			void setMovesArray(const std::array<MoveID, 4> &moveIDs);

			void setMove(const ub slotIndex, const MoveID moveID);

			void setMaxPPArray(const std::array<ub, 4> &maxPP);

			void setMaxPP(const ub slotIndex, const ub maxPP);

			void setCurrentPPArray(const std::array<ub, 4> &currentPP);

			void setCurrentPP(const ub slotIndex, const ub currentPP);

			void setAttack(const us attack);

			void setDefense(const us defense);

			void setHealth(const us health);

			void setSpeed(const us speed);

			void setSpAttack(const us spAttack);

			void setSpDefense(const us spDefense);

			void setAbility(const AbilityID abilityID);

			void setItem(const ItemID itemID);

			// Utility Functions

			constexpr void usePP(const ub slotIndex)
			{
				assert(slotIndex < mCurrentPP.size());

				if (mCurrentPP.at(slotIndex) > 0)
				{
					mCurrentPP.at(slotIndex)--;
				}
			}

		private:
			std::string_view mName{};
			std::array<MoveID, 4> mMoveIDs{};
			std::array<ub, 4> mMaxPP{};
			std::array<ub, 4> mCurrentPP{};
			us mAttack{};
			us mDefense{};
			us mHealth{};
			us mSpeed{};
			us mSpAttack{};
			us mSpDefense{};
			AbilityID mAbilityID{AbilityID::None};
			ItemID mItemID{ItemID::None};
	};
} // namespace PocketCore::Pokemon

#endif
