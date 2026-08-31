/*! @file effectRegistryConfiguration.cpp
	@brief Defines validated user customization operations for the effect registry.
	@date 08/28/2026
	@since 0.12.9
	@version 0.12.9
	@author Matthew Moore
*/

#include "Configuration/effectRegistryConfiguration.h"

#include <expected>
#include <span>
#include <string_view>

#include "Core/attributeMacros.h"
#include "Effect/effectID.h"
#include "Effect/effectMeta.h"

namespace PocketCore::Configuration
{
	using PocketCore::Effect::EffectID;
	using PocketCore::Effect::EffectMeta;

	ATTR_NODISCARD std::expected<EffectID, RegistryErrorInfo> EffectRegistryConfiguration::addEffect(const EffectMeta &effectMeta)
	{
		return addMetadata(effectMeta);
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> EffectRegistryConfiguration::addEffects(
		const std::span<const EffectMeta> &effectMetas)
	{
		return addMetadataBatch(effectMetas, [](const EffectMeta &definition) { return EffectMeta{definition}; });
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> EffectRegistryConfiguration::renameEffect(const std::string_view &oldName,
																									const std::string_view &newName)
	{
		return renameMetadata(oldName, newName);
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> EffectRegistryConfiguration::updateEffect(const std::string_view &effectName,
																									const EffectMeta &effectMeta)
	{
		return mutateMetadata(effectName, "updateEffect", [&effectMeta](EffectMeta &metadata) { metadata = effectMeta; });
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> EffectRegistryConfiguration::updateEffect(const EffectID effectID,
																									const EffectMeta &effectMeta)
	{
		return mutateMetadata(effectID, "updateEffect", [&effectMeta](EffectMeta &metadata) { metadata = effectMeta; });
	}

	ATTR_NODISCARD std::expected<EffectID, RegistryErrorInfo> EffectRegistryConfiguration::removeEffect(const std::string_view &effectName)
	{
		return removeMetadata(effectName);
	}

	ATTR_NODISCARD std::expected<EffectID, RegistryErrorInfo> EffectRegistryConfiguration::removeEffect(const EffectID effectID)
	{
		return removeMetadata(effectID);
	}
} // namespace PocketCore::Configuration
