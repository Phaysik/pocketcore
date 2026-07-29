/*! @file pokemon.cpp
	@brief Contains the function definitions for creating a type registry configuration
	@date 03/11/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#include "Pokemon/pokemon.h"

#include <array>
#include <cassert>
#include <string_view>

#include "Configuration/constants.h"

namespace PocketCore::Pokemon
{
	using PocketCore::Configuration::MAX_MOVES_PER_POKEMON;
	using PocketCore::Configuration::MAX_TYPES_PER_POKEMON;

	void Pokemon::setName(const std::string_view name)
	{
		mName = name;
	}

	void Pokemon::setMovesArray(const std::array<MoveID, MAX_MOVES_PER_POKEMON> &moveIDs)
	{
		mMoveIDs = moveIDs;
	}

	void Pokemon::setMove(const ub slotIndex, const MoveID moveID)
	{
		assert(slotIndex < mMoveIDs.size());

		mMoveIDs.at(slotIndex) = moveID;
	}

	void Pokemon::setMaxPPArray(const std::array<ub, MAX_MOVES_PER_POKEMON> &maxPP)
	{
		mMaxPP = maxPP;
	}

	void Pokemon::setMaxPP(const ub slotIndex, const ub maxPP)
	{
		assert(slotIndex < mMaxPP.size());

		mMaxPP.at(slotIndex) = maxPP;
	}

	void Pokemon::setCurrentPPArray(const std::array<ub, MAX_MOVES_PER_POKEMON> &currentPP)
	{
		mCurrentPP = currentPP;
	}

	void Pokemon::setCurrentPP(const ub slotIndex, const ub currentPP)
	{
		assert(slotIndex < mCurrentPP.size());

		mCurrentPP.at(slotIndex) = currentPP;
	}

	void Pokemon::setTypesArray(const std::array<TypeID, MAX_TYPES_PER_POKEMON> &typeIDs)
	{
		mTypeIDs = typeIDs;
	}

	void Pokemon::setType(const ub slotIndex, const TypeID typeID)
	{
		assert(slotIndex < mTypeIDs.size());

		mTypeIDs.at(slotIndex) = typeID;
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

	void Pokemon::setLevel(const us level)
	{
		mLevel = level;
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
