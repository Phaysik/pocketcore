/*! @file abilityHelpers.h
	@brief Houses free functions that aide in handling abilities.
	@date 09/11/2026
	@since 0.12.26
	@version 0.12.26
	@author Matthew Moore
*/

#ifndef INCLUDE_ABILITY_ABILITY_HELPERS_H
#define INCLUDE_ABILITY_ABILITY_HELPERS_H

#include <utility>

#include "Configuration/abilityRegistryConfiguration.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Utility/random.h"

#include "abilityID.h"
#include "builtInAbilityID.h"

namespace PocketCore::Ability
{
	using PocketCore::Configuration::AbilityRegistryConfiguration;
	using PocketCore::Core::us;
	using PocketCore::Utility::Random;

	/*! @brief Gets a random ability ID from the built-in abilitys.
		@return The random ability ID.
		@since 0.12.26
		@version 0.12.26
	*/
	ATTR_NODISCARD constexpr AbilityID getRandomAbilityIDFromBuiltIns()
	{
		return AbilityID(
			Random::get<us>(std::to_underlying(BuiltinAbilityID::None) + 1, std::to_underlying(BuiltinAbilityID::FinalAbility) - 1));
	}

	/*! @brief Gets a random ability ID from the given configuration.
		@param[in] config The configuration to get the random ability ID from.
		@return The random ability ID.
		@since 0.12.26
		@version 0.12.26
	*/
	ATTR_NODISCARD constexpr AbilityID getRandomAbilityIDFromConfiguration(const AbilityRegistryConfiguration &config)
	{
		return AbilityID(Random::get<us>(std::to_underlying(BuiltinAbilityID::None) + 1, config.getAmountRegistered()));
	}
} // namespace PocketCore::Ability

#endif
