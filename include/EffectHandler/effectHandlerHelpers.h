/*! @file effectHandlerHelpers.h
	@brief Contains the effect handler helpers
	@date 09/02/2026
	@since 0.7.2
	@version 0.12.17
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
	using PocketCore::Type::BuiltinTypeID;
	using PocketCore::Type::toTypeID;
	using PocketCore::Type::TypeID;

	/*! @brief Returns the mutable team selected by a battle side.
		@param[in,out] state The battle state containing both teams.
		@param[in] side The side whose team is requested.
		@return A mutable reference to the selected team, valid for the lifetime of @p state unless the team is invalidated.
		@note Selects side A for @ref Side::A and side B for every other side value.
		@since 0.7.2
		@version 0.12.7
	*/
	ATTR_NODISCARD static inline std::vector<BattleSlot> &getTeam(BattleState &state, Side side)
	{
		return (side == Side::A) ? state.mSideA : state.mSideB;
	}

	/*! @brief Returns a read-only view of the team selected by a battle side.
		@param[in] state The battle state containing both teams.
		@param[in] side The side whose team is requested.
		@return A const reference to the selected team, valid for the lifetime of @p state unless the team is invalidated.
		@note The state parameter is non-const because the existing helper API accepts @ref BattleState by mutable reference.
		@since 0.7.2
		@version 0.12.7
	*/
	ATTR_NODISCARD static inline const std::vector<BattleSlot> &getTeamConst(BattleState &state, Side side)
	{

		return (side == Side::A) ? state.mSideA : state.mSideB;
	}

	/*! @brief Determines whether a battle slot's Pokemon has an type with a specified name.
		@param[in] battleSlot The battle slot whose Pokemon is inspected.
		@param[in] provider The registries used to resolve type identifiers to names.
		@param[in] expectedName The type name to search for.
		@return `true` when a resolved type name matches @p expectedName; otherwise, `false`.
		@note Returns `false` when the slot has no Pokemon, the type registry is unavailable, or no type matches.
		@since 0.12.17
		@version 0.12.17
	*/
	ATTR_NODISCARD static inline bool battleSlotHasTypeByName(const BattleSlot &battleSlot, const RegistryProvider &provider,
															  const std::string_view &expectedName)
	{
		if (battleSlot.mPokemon == nullptr || provider.typeRegistry == nullptr)
		{
			return false;
		}

		return std::ranges::any_of(battleSlot.mPokemon->getTypeIDsArray(), [expectedName, &provider](const TypeID pokemonAbility) {
			const std::optional<std::string_view> typeName{provider.typeRegistry->getTypeName(pokemonAbility)};
			return typeName.has_value() && typeName.value() == expectedName;
		});
	}

	/*! @brief Determines whether a battle slot's Pokemon has a specified type.
		@param[in] battleSlot The battle slot whose Pokemon is inspected.
		@param[in] queriedType The type identifier to search for.
		@return `true` when the slot contains a Pokemon with @p queriedType; otherwise, `false`.
		@note Returns `false` when the slot does not contain a Pokemon.
		@since 0.9.10
		@version 0.12.17
	*/
	ATTR_NODISCARD static inline bool battleSlotHasTypeByID(const BattleSlot &battleSlot, const TypeID queriedType)
	{
		if (battleSlot.mPokemon == nullptr)
		{
			return false;
		}

		return std::ranges::any_of(battleSlot.mPokemon->getTypeIDsArray(),
								   [queriedType](const TypeID pokemonType) { return pokemonType == queriedType; });
	}

	/*! @brief Determines whether a battle slot's Pokemon has an ability with a specified name.
		@param[in] battleSlot The battle slot whose Pokemon is inspected.
		@param[in] provider The registries used to resolve ability identifiers to names.
		@param[in] expectedName The ability name to search for.
		@return `true` when a resolved ability name matches @p expectedName; otherwise, `false`.
		@note Returns `false` when the slot has no Pokemon, the ability registry is unavailable, or no ability matches.
		@since 0.9.10
		@version 0.12.17
	*/
	ATTR_NODISCARD static inline bool battleSlotHasAbilityByName(const BattleSlot &battleSlot, const RegistryProvider &provider,
																 const std::string_view &expectedName)
	{
		if (battleSlot.mPokemon == nullptr || provider.abilityRegistry == nullptr)
		{
			return false;
		}

		return std::ranges::any_of(battleSlot.mPokemon->getAbilityIDsArray(), [expectedName, &provider](const AbilityID pokemonAbility) {
			const std::optional<std::string_view> abilityName{provider.abilityRegistry->getAbilityName(pokemonAbility)};
			return abilityName.has_value() && abilityName.value() == expectedName;
		});
	}

	/*! @brief Determines whether a battle slot's Pokemon has a specified ability identifier.
		@param[in] battleSlot The battle slot whose Pokemon is inspected.
		@param[in] abilityID The ability identifier to search for.
		@return `true` when the slot contains a Pokemon with @p abilityID; otherwise, `false`.
		@note Returns `false` when the slot does not contain a Pokemon.
		@since 0.9.10
		@version 0.12.17
	*/
	ATTR_NODISCARD static inline bool battleSlotHasAbilityByID(const BattleSlot &battleSlot, const AbilityID &abilityID)
	{
		if (battleSlot.mPokemon == nullptr)
		{
			return false;
		}
		return std::ranges::contains(battleSlot.mPokemon->getAbilityIDsArray(), abilityID);
	}

	/*! @brief Determines whether a battle slot's Pokemon holds an item with a specified name.
		@param[in] battleSlot The battle slot whose Pokemon is inspected.
		@param[in] provider The registries used to resolve item identifiers to names.
		@param[in] expectedName The item name to search for.
		@return `true` when a resolved item name matches @p expectedName; otherwise, `false`.
		@note Returns `false` when the slot has no Pokemon, the item registry is unavailable, or no item matches.
		@since 0.9.10
		@version 0.12.17
	*/
	ATTR_NODISCARD static inline bool battleSlotHasItemByName(const BattleSlot &battleSlot, const RegistryProvider &provider,
															  const std::string_view &expectedName)
	{
		if (battleSlot.mPokemon == nullptr || provider.itemRegistry == nullptr)
		{
			return false;
		}

		return std::ranges::any_of(battleSlot.mPokemon->getItemsIDsArray(), [expectedName, &provider](const ItemID pokemonItem) {
			const std::optional<std::string_view> itemName{provider.itemRegistry->getItemName(pokemonItem)};
			return itemName.has_value() && itemName.value() == expectedName;
		});
	}

	/*! @brief Determines whether a battle slot's Pokemon holds a specified item identifier.
		@param[in] battleSlot The battle slot whose Pokemon is inspected.
		@param[in] itemID The item identifier to search for.
		@return `true` when the slot contains a Pokemon holding @p itemID; otherwise, `false`.
		@note Returns `false` when the slot does not contain a Pokemon.
		@since 0.9.10
		@version 0.12.17
	*/
	ATTR_NODISCARD static inline bool battleSlotHasItemByID(const BattleSlot &battleSlot, const ItemID itemID)
	{
		if (battleSlot.mPokemon == nullptr)
		{
			return false;
		}

		return std::ranges::contains(battleSlot.mPokemon->getItemsIDsArray(), itemID);
	}

	/*! @brief Determines whether a battle slot's Pokemon has a nature with a specified name.
		@param[in] battleSlot The battle slot whose Pokemon is inspected.
		@param[in] provider The registries used to resolve nature identifiers to names.
		@param[in] expectedName The nature name to search for.
		@return `true` when a resolved nature name matches @p expectedName; otherwise, `false`.
		@note Returns `false` when the slot has no Pokemon, the nature registry is unavailable, or no nature matches.
		@since 0.11.6
		@version 0.12.17
	*/
	ATTR_NODISCARD static inline bool battleSlotHasNatureByName(const BattleSlot &battleSlot, const RegistryProvider &provider,
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

	/*! @brief Determines whether a battle slot's Pokemon has a specified nature identifier.
		@param[in] battleSlot The battle slot whose Pokemon is inspected.
		@param[in] natureID The nature identifier to search for.
		@return `true` when the slot contains a Pokemon with @p natureID; otherwise, `false`.
		@note Returns `false` when the slot does not contain a Pokemon.
		@since 0.11.6
		@version 0.12.17
	*/
	ATTR_NODISCARD static inline bool battleSlotHasNatureByID(const BattleSlot &battleSlot, const NatureID natureID)
	{
		if (battleSlot.mPokemon == nullptr)
		{
			return false;
		}

		return std::ranges::contains(battleSlot.mPokemon->getNatureIDsArray(), natureID);
	}

	/*! @brief Determines whether a battle slot is not grounded.
		@param[in] battleSlot The battle slot whose grounding state is inspected.
		@return `true` when the slot is airborne because it is not grounded and its Pokemon has the Flying type; otherwise, `false`.
		@note An empty slot is treated as grounded because it cannot satisfy the Flying-type condition.
		@since 0.9.10
		@version 0.12.17
	*/
	ATTR_NODISCARD static inline bool isBattleSlotUngrounded(const BattleSlot &battleSlot)
	{
		if (battleSlot.mIsGrounded)
		{
			return false;
		}

		return battleSlotHasTypeByID(battleSlot, toTypeID(BuiltinTypeID::Flying));
	}

	/*! @brief Determines whether a battle slot is grounded.
		@param[in] battleSlot The battle slot whose grounding state is inspected.
		@return The logical negation of @ref isBattleSlotUngrounded.
		@note An empty slot is treated as grounded.
		@since 0.9.10
		@version 0.12.7
	*/
	ATTR_NODISCARD static inline bool isBattleSlotGrounded(const BattleSlot &battleSlot)
	{
		return !isBattleSlotUngrounded(battleSlot);
	}
} // namespace PocketCore::Effect

#endif
