/*! @file typeEffectivenessRegistryConfiguration.cpp
	@brief Defines validated user customization operations for the typeEffectiveness registry.
	@date 07/27/2026
	@version x.x.x
	@since x.x.x
	@author Matthew Moore
*/

#include "Configuration/typeEffectivenessRegistryConfiguration.h"

#include <expected>
#include <span>
#include <string_view>

#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Types/typeEffectivenessID.h"
#include "Types/typeEffectivenessMeta.h"

namespace PocketCore::Configuration
{
	using PocketCore::Types::TypeEffectivenessID;
	using PocketCore::Types::TypeEffectivenessMeta;

	ATTR_NODISCARD std::expected<TypeEffectivenessID, RegistryErrorInfo> TypeEffectivenessRegistryConfiguration::addTypeEffectiveness(
		const TypeEffectivenessMeta &typeEffectivenessMeta)
	{
		return addMetadata(typeEffectivenessMeta);
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> TypeEffectivenessRegistryConfiguration::addTypeEffectivenesss(
		const std::span<const TypeEffectivenessMeta> &typeEffectivenessMetas)
	{
		return addMetadataBatch(typeEffectivenessMetas,
								[](const TypeEffectivenessMeta &definition) { return TypeEffectivenessMeta{definition}; });
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> TypeEffectivenessRegistryConfiguration::renameTypeEffectiveness(
		const std::string_view &oldName, const std::string_view &newName)
	{
		return renameMetadata(oldName, newName);
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> TypeEffectivenessRegistryConfiguration::updateTypeEffectiveness(
		const std::string_view &typeEffectivenessName, const TypeEffectivenessMeta &typeEffectivenessMeta)
	{
		return mutateMetadata(typeEffectivenessName, "updateTypeEffectiveness",
							  [&typeEffectivenessMeta](TypeEffectivenessMeta &metadata) { metadata = typeEffectivenessMeta; });
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> TypeEffectivenessRegistryConfiguration::updateTypeEffectiveness(
		const TypeEffectivenessID typeEffectivenessID, const TypeEffectivenessMeta &typeEffectivenessMeta)
	{
		return mutateMetadata(typeEffectivenessID, "updateTypeEffectiveness",
							  [&typeEffectivenessMeta](TypeEffectivenessMeta &metadata) { metadata = typeEffectivenessMeta; });
	}

	ATTR_NODISCARD std::expected<TypeEffectivenessID, RegistryErrorInfo> TypeEffectivenessRegistryConfiguration::removeTypeEffectiveness(
		const std::string_view &typeEffectivenessName)
	{
		return removeMetadata(typeEffectivenessName);
	}

	ATTR_NODISCARD std::expected<TypeEffectivenessID, RegistryErrorInfo> TypeEffectivenessRegistryConfiguration::removeTypeEffectiveness(
		const TypeEffectivenessID typeEffectivenessID)
	{
		return removeMetadata(typeEffectivenessID);
	}
} // namespace PocketCore::Configuration
