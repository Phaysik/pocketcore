/*! @file learnsetRegistryConfiguration.cpp
	@brief Defines validated user customization operations for the learnset registry.
	@date 10/08/2026
	@since 0.12.50
	@version 0.12.50
	@author Matthew Moore
*/

#include "Configuration/learnsetRegistryConfiguration.h"

#include <expected>
#include <span>
#include <string_view>

#include "Core/attributeMacros.h"
#include "Learnset/learnsetID.h"
#include "Learnset/learnsetMeta.h"
#include "Registry/registryError.h"

namespace PocketCore::Configuration
{
	using PocketCore::Learnset::LearnsetID;
	using PocketCore::Learnset::LearnsetMeta;
	using PocketCore::Registry::RegistryErrorInfo;

	ATTR_NODISCARD std::expected<LearnsetID, RegistryErrorInfo> LearnsetRegistryConfiguration::addLearnset(const LearnsetMeta &learnsetMeta)
	{
		return addMetadata(learnsetMeta);
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> LearnsetRegistryConfiguration::addLearnsets(
		const std::span<const LearnsetMeta> &learnsetMetas)
	{
		return addMetadataBatch(learnsetMetas, [](const LearnsetMeta &definition) { return LearnsetMeta{definition}; });
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> LearnsetRegistryConfiguration::renameLearnset(const std::string_view &oldName,
																										const std::string_view &newName)
	{
		return renameMetadata(oldName, newName);
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> LearnsetRegistryConfiguration::updateLearnset(
		const std::string_view &learnsetName, const LearnsetMeta &learnsetMeta)
	{
		return mutateMetadata(learnsetName, "updateLearnset", [&learnsetMeta](LearnsetMeta &metadata) { metadata = learnsetMeta; });
	}

	ATTR_NODISCARD std::expected<void, RegistryErrorInfo> LearnsetRegistryConfiguration::updateLearnset(const LearnsetID learnsetID,
																										const LearnsetMeta &learnsetMeta)
	{
		return mutateMetadata(learnsetID, "updateLearnset", [&learnsetMeta](LearnsetMeta &metadata) { metadata = learnsetMeta; });
	}

	ATTR_NODISCARD std::expected<LearnsetID, RegistryErrorInfo> LearnsetRegistryConfiguration::removeLearnset(
		const std::string_view &learnsetName)
	{
		return removeMetadata(learnsetName);
	}

	ATTR_NODISCARD std::expected<LearnsetID, RegistryErrorInfo> LearnsetRegistryConfiguration::removeLearnset(const LearnsetID learnsetID)
	{
		return removeMetadata(learnsetID);
	}
} // namespace PocketCore::Configuration
