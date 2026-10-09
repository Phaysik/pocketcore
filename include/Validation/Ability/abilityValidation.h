/*! @file abilityValidation.h
	@brief Defines validation functions that help with Ability.
	@date 10/09/2026
	@since 0.12.51
	@version 0.12.51
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
#include "Registry/abilityRegistry.h"

#include "abilityError.h"

namespace PocketCore::Validation::Ability
{
	using PocketCore::Ability::AbilityID;
	using PocketCore::Ability::AbilityMeta;
	using PocketCore::Configuration::MAX_ABILITIES_PER_POKEMON;
	using PocketCore::Core::us;
	using PocketCore::Registry::Ability::AbilityRegistry;

	/*! @brief Validates the metadata for the given ability IDs.
		@param[in] abilityIDs The array of ability IDs to validate.
		@param[in] abilityRegistry The registry to validate ability metadata.
		@return A @ref AbilityError if any ability IDs are invalid, or std::nullopt if all are valid.
		@since 0.12.26
		@version 0.12.51
	 */
	ATTR_NODISCARD constexpr std::expected<void, AbilityError> validateAbilityMetadata(
		const std::span<const AbilityID, MAX_ABILITIES_PER_POKEMON> *abilityIDs, const AbilityRegistry &abilityRegistry)
	{
		std::array<const AbilityMeta *, MAX_ABILITIES_PER_POKEMON> abilityMetas{};
		std::ranges::transform(*abilityIDs, abilityMetas.begin(),
							   [&abilityRegistry](const AbilityID abilityID) { return abilityRegistry.getAbilityMetadata(abilityID); });

		if (std::ranges::any_of(abilityMetas, [](const AbilityMeta *meta) { return meta == nullptr; }))
		{
			return std::unexpected{AbilityError::MISSING_ABILITY_METADATA};
		}

		return {};
	}

} // namespace PocketCore::Validation::Ability

#endif
