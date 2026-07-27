/*! @file pokemon.cpp
	@brief Contains the function definitions for creating a type registry configuration
	@date 03/11/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#include "Pokemon/pokemon.h"

namespace PocketCore::Pokemon
{
	void Pokemon::setName(const std::string_view name)
	{
		mName = name;
	}

	void Pokemon::setMovesArray(const std::array<MoveID, 4> &moveIDs)
	{
		mMoveIDs = moveIDs;
	}

	void Pokemon::setMove(const ub slotIndex, const MoveID moveID)
	{
		assert(slotIndex < mMoveIDs.size());

		mMoveIDs.at(slotIndex) = moveID;
	}

	void Pokemon::setMaxPPArray(const std::array<ub, 4> &maxPP)
	{
		mMaxPP = maxPP;
	}

	void Pokemon::setMaxPP(const ub slotIndex, const ub maxPP)
	{
		assert(slotIndex < mMaxPP.size());

		mMaxPP.at(slotIndex) = maxPP;
	}

	void Pokemon::setCurrentPPArray(const std::array<ub, 4> &currentPP)
	{
		mCurrentPP = currentPP;
	}

	void Pokemon::setCurrentPP(const ub slotIndex, const ub currentPP)
	{
		assert(slotIndex < mCurrentPP.size());

		mCurrentPP.at(slotIndex) = currentPP;
	}

	void Pokemon::setAttack(const us attack)
	{
		mAttack = attack;
	}

	void Pokemon::setDefense(const us defense)
	{
		mDefense = defense;
	}

	void Pokemon::setHealth(const us health)
	{
		mHealth = health;
	}

	void Pokemon::setSpeed(const us speed)
	{
		mSpeed = speed;
	}

	void Pokemon::setSpAttack(const us spAttack)
	{
		mSpAttack = spAttack;
	}

	void Pokemon::setSpDefense(const us spDefense)
	{
		mSpDefense = spDefense;
	}

	void Pokemon::setAbility(const AbilityID abilityID)
	{
		mAbilityID = abilityID;
	}

	void Pokemon::setItem(const ItemID itemID)
	{
		mItemID = itemID;
	}
} // namespace PocketCore::Pokemon
