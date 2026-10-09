/*! @file natureValidation.h
	@brief Defines validation functions that help with Nature.
	@date 10/09/2026
	@since 0.12.51
	@version 0.12.51
	@author Matthew Moore
*/

#ifndef INCLUDE_VALIDATION_NATURE_NATURE_VALIDATION_H
#define INCLUDE_VALIDATION_NATURE_NATURE_VALIDATION_H

#include <algorithm>
#include <expected>
#include <span>

#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Nature/natureID.h"
#include "Nature/natureMeta.h"
#include "Registry/natureRegistry.h"

#include "natureError.h"

namespace PocketCore::Validation::Nature
{
	using PocketCore::Configuration::MAX_NATURES_PER_POKEMON;
	using PocketCore::Core::us;
	using PocketCore::Nature::NatureID;
	using PocketCore::Nature::NatureMeta;
	using PocketCore::Registry::Nature::NatureRegistry;

	/*! @brief Validates the metadata for the given nature IDs.
		@param[in] natureIDs The array of nature IDs to validate.
		@param[in] natureRegistry The registry to validate nature metadata.
		@return A @ref NatureError if any nature IDs are invalid, or std::nullopt if all are valid.
		@since 0.12.26
		@version 0.12.51
	 */
	ATTR_NODISCARD constexpr std::expected<void, NatureError> validateNatureMetadata(
		const std::span<const NatureID, MAX_NATURES_PER_POKEMON> *natureIDs, const NatureRegistry &natureRegistry)
	{
		std::array<const NatureMeta *, MAX_NATURES_PER_POKEMON> natureMetas{};
		std::ranges::transform(*natureIDs, natureMetas.begin(),
							   [&natureRegistry](const NatureID natureID) { return natureRegistry.getNatureMetadata(natureID); });

		if (std::ranges::any_of(natureMetas, [](const NatureMeta *meta) { return meta == nullptr; }))
		{
			return std::unexpected{NatureError::MISSING_NATURE_METADATA};
		}

		return {};
	}

} // namespace PocketCore::Validation::Nature

#endif
