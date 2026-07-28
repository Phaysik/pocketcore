/*! @file abilityRegistryConfiguration.cpp
	@brief Defines validated user customization operations for the ability registry.
	@date 07/27/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#include "Configuration/abilityRegistryConfiguration.h"

#include <expected>
#include <span>
#include <string_view>
#include <vector>

#include "Ability/abilityID.h"
#include "Ability/abilityMeta.h"
#include "Ability/abilityTargetsAndTriggers.h"
#include "Configuration/constants.h"
#include "Core/attributeMacros.h"

namespace PocketCore::Configuration
{
	using PocketCore::Ability::AbilityID;
	using PocketCore::Ability::AbilityMeta;
	using PocketCore::Ability::AbilityTargetID;

	ATTR_NODISCARD std::expected<AbilityID, RegistryErrorInfo> AbilityRegistryConfiguration::addAbility(const AbilityMeta &abilityMeta)
	{
		return addMetadata(abilityMeta);
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> AbilityRegistryConfiguration::addAbilities(
		const std::span<const AbilityMeta> &abilityMetas)
	{
		return addMetadataBatch(abilityMetas, [](const AbilityMeta &definition) { return AbilityMeta{definition}; });
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> AbilityRegistryConfiguration::setAbilityTriggers(
		const std::string_view &abilityName, const std::span<const AbilityEffectTrigger> &triggers)
	{
		return mutateMetadata(abilityName, "setAbilityTriggers",
							  [&triggers](AbilityMeta &metadata) { metadata.mTriggers.assign(triggers.begin(), triggers.end()); });
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> AbilityRegistryConfiguration::setAbilityTriggers(
		const AbilityID abilityID, const std::span<const AbilityEffectTrigger> &triggers)
	{
		return mutateMetadata(abilityID, "setAbilityTriggers",
							  [&triggers](AbilityMeta &metadata) { metadata.mTriggers.assign(triggers.begin(), triggers.end()); });
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> AbilityRegistryConfiguration::setAbilityTarget(
		const std::string_view &abilityName, const AbilityTargetID target)
	{
		return mutateMetadata(abilityName, "setAbilityTarget", [&target](AbilityMeta &metadata) { metadata.mTargetID = target; });
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> AbilityRegistryConfiguration::setAbilityTarget(const AbilityID abilityID,
																										 const AbilityTargetID target)
	{
		return mutateMetadata(abilityID, "setAbilityTarget", [&target](AbilityMeta &metadata) { metadata.mTargetID = target; });
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> AbilityRegistryConfiguration::renameAbility(const std::string_view &oldName,
																									  const std::string_view &newName)
	{
		return renameMetadata(oldName, newName);
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> AbilityRegistryConfiguration::updateAbility(const std::string_view &abilityName,
																									  const AbilityMeta &abilityMeta)
	{
		return mutateMetadata(abilityName, "updateAbility", [&abilityMeta](AbilityMeta &metadata) { metadata = abilityMeta; });
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> AbilityRegistryConfiguration::updateAbility(const AbilityID abilityID,
																									  const AbilityMeta &abilityMeta)
	{
		return mutateMetadata(abilityID, "updateAbility", [&abilityMeta](AbilityMeta &metadata) { metadata = abilityMeta; });
	}

	ATTR_NODISCARD std::expected<AbilityID, RegistryErrorInfo> AbilityRegistryConfiguration::removeAbility(
		const std::string_view &abilityName)
	{
		return removeMetadata(abilityName);
	}

	ATTR_NODISCARD std::expected<AbilityID, RegistryErrorInfo> AbilityRegistryConfiguration::removeAbility(const AbilityID abilityID)
	{
		return removeMetadata(abilityID);
	}
} // namespace PocketCore::Configuration
