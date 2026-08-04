/*! @file effectHandlerHelpers.h
	@brief Contains the effect handler helpers
	@date 07/24/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#ifndef INCLUDE_EFFECT_EFFECTHANDLERHELPERS_H
#define INCLUDE_EFFECT_EFFECTHANDLERHELPERS_H

#include <algorithm>
#include <string_view>
#include <vector>

#include "Ability/abilityID.h"
#include "Ability/builtInAbilityID.h"
#include "Battle/battleState.h"
#include "Core/attributeMacros.h"
#include "Effect/effectContext.h"
#include "Item/builtInItemID.h"
#include "Item/itemID.h"
#include "Registry/registryProvider.h"
#include "Types/builtInTypeID.h"
#include "Types/typeID.h"

namespace PocketCore::Effect
{
	using PocketCore::Ability::AbilityID;
	using PocketCore::Ability::BuiltinAbilityID;
	using PocketCore::Ability::toAbilityID;
	using PocketCore::Battle::BattleSlot;
	using PocketCore::Battle::BattleState;
	using PocketCore::Item::BuiltinItemID;
	using PocketCore::Item::ItemID;
	using PocketCore::Item::toItemID;
	using PocketCore::Registry::RegistryProvider;
	using PocketCore::Types::BuiltInTypeID;
	using PocketCore::Types::toTypeID;
	using PocketCore::Types::TypeID;

	ATTR_NODISCARD static inline std::vector<BattleSlot> &getTeam(BattleState &state, Side side)
	{
		return (side == Side::A) ? state.mSideA : state.mSideB;
	}

	ATTR_NODISCARD static inline const std::vector<BattleSlot> &getTeamConst(BattleState &state, Side side)
	{

		return (side == Side::A) ? state.mSideA : state.mSideB;
	}

	ATTR_NODISCARD static inline bool battleSlotHasType(const BattleSlot &battleSlot, const TypeID queriedType)
	{
		if (battleSlot.mPokemon == nullptr)
		{
			return false;
		}

		return std::ranges::any_of(battleSlot.mPokemon->getTypesArray(),
								   [queriedType](const TypeID pokemonType) { return pokemonType == queriedType; });
	}

	ATTR_NODISCARD static inline bool battleSlotHasAbilityByName(const BattleSlot &battleSlot, const RegistryProvider &provider,
																 const std::string_view expectedName)
	{
		if (battleSlot.mPokemon == nullptr || provider.abilityRegistry == nullptr)
		{
			return false;
		}

		const auto abilityName{provider.abilityRegistry->getAbilityName(battleSlot.mPokemon->getAbilityID())};
		return abilityName.has_value() && abilityName.value() == expectedName;
	}

	ATTR_NODISCARD static inline bool battleSlotHasAbilityByID(const BattleSlot &battleSlot, const AbilityID &abilityID)
	{
		if (battleSlot.mPokemon == nullptr)
		{
			return false;
		}

		return battleSlot.mPokemon->getAbilityID() == abilityID;
	}

	ATTR_NODISCARD static inline bool battleSlotHoldsItemByName(const BattleSlot &battleSlot, const RegistryProvider &provider,
																const std::string_view expectedName)
	{
		if (battleSlot.mPokemon == nullptr || provider.itemRegistry == nullptr)
		{
			return false;
		}

		const auto itemName{provider.itemRegistry->getItemName(battleSlot.mPokemon->getItemID())};
		return itemName.has_value() && itemName.value() == expectedName;
	}

	ATTR_NODISCARD static inline bool battleSlotHoldsItemByID(const BattleSlot &battleSlot, const ItemID itemID)
	{
		if (battleSlot.mPokemon == nullptr)
		{
			return false;
		}

		return battleSlot.mPokemon->getItemID() == itemID;
	}

	ATTR_NODISCARD static inline bool isBattleSlotUngrounded(const BattleSlot &battleSlot)
	{
		if (battleSlot.mIsGrounded)
		{
			return false;
		}

		return battleSlotHasType(battleSlot, toTypeID(BuiltInTypeID::Flying));
	}

	ATTR_NODISCARD static inline bool isBattleSlotGrounded(const BattleSlot &battleSlot)
	{
		return !isBattleSlotUngrounded(battleSlot);
	}
} // namespace PocketCore::Effect

#endif
