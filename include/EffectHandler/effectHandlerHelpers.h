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
#include <optional>
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
	using PocketCore::Type::BuiltInTypeID;
	using PocketCore::Type::toTypeID;
	using PocketCore::Type::TypeID;

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
																 const std::string_view &expectedName)
	{
		if (battleSlot.mPokemon == nullptr || provider.abilityRegistry == nullptr)
		{
			return false;
		}

		return std::ranges::any_of(battleSlot.mPokemon->getAbilitiesArray(), [expectedName, &provider](const AbilityID pokemonAbility) {
			const std::optional<std::string_view> abilityName{provider.abilityRegistry->getAbilityName(pokemonAbility)};
			return abilityName.has_value() && abilityName.value() == expectedName;
		});
	}

	ATTR_NODISCARD static inline bool battleSlotHasAbilityByID(const BattleSlot &battleSlot, const AbilityID &abilityID)
	{
		if (battleSlot.mPokemon == nullptr)
		{
			return false;
		}
		return std::ranges::contains(battleSlot.mPokemon->getAbilitiesArray(), abilityID);
	}

	ATTR_NODISCARD static inline bool battleSlotHoldsItemByName(const BattleSlot &battleSlot, const RegistryProvider &provider,
																const std::string_view &expectedName)
	{
		if (battleSlot.mPokemon == nullptr || provider.itemRegistry == nullptr)
		{
			return false;
		}

		return std::ranges::any_of(battleSlot.mPokemon->getItemsArray(), [expectedName, &provider](const ItemID pokemonItem) {
			const std::optional<std::string_view> itemName{provider.itemRegistry->getItemName(pokemonItem)};
			return itemName.has_value() && itemName.value() == expectedName;
		});
	}

	ATTR_NODISCARD static inline bool battleSlotHoldsItemByID(const BattleSlot &battleSlot, const ItemID itemID)
	{
		if (battleSlot.mPokemon == nullptr)
		{
			return false;
		}

		return std::ranges::contains(battleSlot.mPokemon->getItemsArray(), itemID);
	}

	ATTR_NODISCARD static inline bool battleSlotHoldsNatureByName(const BattleSlot &battleSlot, const RegistryProvider &provider,
																  const std::string_view &expectedName)
	{
		if (battleSlot.mPokemon == nullptr || provider.natureRegistry == nullptr)
		{
			return false;
		}

		return std::ranges::any_of(battleSlot.mPokemon->getNatureIDsArray(), [expectedName, &provider](const NatureID natureID) {
			const std::optional<std::string_view> natureName{provider.natureRegistry->getNatureName(natureID)};
			return natureName.has_value() && natureName.value() == expectedName;
		});
	}

	ATTR_NODISCARD static inline bool battleSlotHoldsNatureByID(const BattleSlot &battleSlot, const NatureID natureID)
	{
		if (battleSlot.mPokemon == nullptr)
		{
			return false;
		}

		return std::ranges::contains(battleSlot.mPokemon->getNatureIDsArray(), natureID);
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
