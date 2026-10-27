/*! @file abilityValidation.h
	@brief Defines validation functions that help with Ability.
	@date 10/27/2026
	@since 0.12.51
	@version 0.12.53
	@author Matthew Moore
*/

#ifndef INCLUDE_VALIDATION_ABILITY_ABILITY_VALIDATION_H
#define INCLUDE_VALIDATION_ABILITY_ABILITY_VALIDATION_H

#include <algorithm>
#include <expected>
#include <span>

#include "Ability/abilityID.h"
#include "Ability/abilityMeta.h"
#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "EggGroup/builtInEggGroupID.h"
#include "EggGroup/eggGroupID.h"
#include "Pokemon/pokemon.h"
#include "Pokemon/pokemonMeta.h"
#include "Registry/abilityRegistry.h"

#include "abilityError.h"

namespace PocketCore::Validation::Ability
{
	using PocketCore::Ability::AbilityID;
	using PocketCore::Ability::AbilityMeta;
	using PocketCore::Configuration::MAX_ABILITIES_PER_POKEMON;
	using PocketCore::Core::us;
	using PocketCore::EggGroup::BuiltinEggGroupID;
	using PocketCore::EggGroup::EggGroupID;
	using PocketCore::EggGroup::NO_EGG_GROUP_ID;
	using PocketCore::EggGroup::toEggGroupID;
	using PocketCore::Pokemon::Pokemon;
	using PocketCore::Pokemon::PokemonGender;
	using PocketCore::Registry::Ability::AbilityRegistry;

	/*! @brief Validates the metadata for the given ability IDs.
		@param[in] abilityIDs The array of ability IDs to validate.
		@param[in] abilityRegistry The registry to validate ability metadata.
		@return A @ref AbilityError if any ability IDs are invalid, or std::nullopt if all are valid.
		@since 0.12.26
		@version 0.12.53
	 */
	ATTR_NODISCARD constexpr std::expected<void, AbilityError> validateAbilityMetadata(const std::span<const AbilityID> abilityIDs,
																					   const AbilityRegistry &abilityRegistry)
	{
		if (abilityIDs.size() > MAX_ABILITIES_PER_POKEMON)
		{
			return std::unexpected{AbilityError::TOO_MANY_ABILITIES};
		}

		std::array<const AbilityMeta *, MAX_ABILITIES_PER_POKEMON> abilityMetas{};
		std::ranges::transform(abilityIDs, abilityMetas.begin(),
							   [&abilityRegistry](const AbilityID abilityID) { return abilityRegistry.getAbilityMetadata(abilityID); });

		if (abilityIDs.size() > MAX_ABILITIES_PER_POKEMON
			|| std::ranges::any_of(abilityMetas, [](const AbilityMeta *meta) { return meta == nullptr; }))
		{
			return std::unexpected{AbilityError::MISSING_ABILITY_METADATA};
		}

		return {};
	}

	ATTR_NODISCARD constexpr std::expected<void, AbilityError> hasValidEggGroup(const Pokemon &parent1, const Pokemon &parent2)
	{
		constexpr auto hasMatchingEggGroup{
			[](const Pokemon &pokemon, const EggGroupID eggGroupID) -> bool {
				return eggGroupID != NO_EGG_GROUP_ID && std::ranges::contains(pokemon.getEggGroupIDsArray(), eggGroupID);
			},
		};

		constexpr auto allHasNoEggGroupIDs{
			[](const Pokemon &pokemon) {
				return std::ranges::all_of(pokemon.getEggGroupIDsArray(),
										   [](const EggGroupID eggGroupID) -> bool { return eggGroupID == NO_EGG_GROUP_ID; });
			},
		};

		if (hasMatchingEggGroup(parent1, toEggGroupID(BuiltinEggGroupID::NoEggs))
			|| hasMatchingEggGroup(parent2, toEggGroupID(BuiltinEggGroupID::NoEggs)))
		{
			return std::unexpected{AbilityError::PARENT_CANNOT_REPRODUCE};
		}

		const bool parent1isDitto{hasMatchingEggGroup(parent1, toEggGroupID(BuiltinEggGroupID::Ditto))};
		const bool parent2isDitto{hasMatchingEggGroup(parent2, toEggGroupID(BuiltinEggGroupID::Ditto))};

		if (parent1isDitto && parent2isDitto)
		{
			return std::unexpected{AbilityError::BOTH_PARENTS_ARE_DITTO};
		}

		const bool parent1NoEggGroups{allHasNoEggGroupIDs(parent1)};
		const bool parent2NoEggGroups{allHasNoEggGroupIDs(parent2)};

		if (parent1isDitto)
		{
			if (parent2NoEggGroups)
			{
				return std::unexpected{AbilityError::PARENT_HAS_NO_VALID_EGG_GROUP};
			}

			return {};
		}

		if (parent2isDitto)
		{
			if (parent1NoEggGroups)
			{
				return std::unexpected{AbilityError::PARENT_HAS_NO_VALID_EGG_GROUP};
			}

			return {};
		}

		if (!std::ranges::any_of(parent1.getEggGroupIDsArray(), [&hasMatchingEggGroup, &parent2](const EggGroupID eggGroupID) {
				return hasMatchingEggGroup(parent2, eggGroupID);
			}))
		{
			return std::unexpected{AbilityError::NO_COMMON_EGG_GROUP};
		}

		return {};
	}

	ATTR_NODISCARD ATTR_PURE constexpr std::expected<void, AbilityError> hasTwoMaleParents(const Pokemon &parent1, const Pokemon &parent2)
	{
		if (parent1.getGender() == PokemonGender::Male && parent2.getGender() == PokemonGender::Male)
		{
			return std::unexpected{AbilityError::SAME_SEX_PARENTS_CANT_REPRODUCE};
		}

		return {};
	}

	ATTR_NODISCARD ATTR_PURE constexpr std::expected<void, AbilityError> hasTwoFemaleParents(const Pokemon &parent1, const Pokemon &parent2)
	{
		if (parent1.getGender() == PokemonGender::Female && parent2.getGender() == PokemonGender::Female)
		{
			return std::unexpected{AbilityError::SAME_SEX_PARENTS_CANT_REPRODUCE};
		}

		return {};
	}

} // namespace PocketCore::Validation::Ability

#endif
