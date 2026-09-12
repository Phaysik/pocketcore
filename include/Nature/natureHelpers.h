/*! @file natureHelpers.h
	@brief Houses free functions that aide in handling natures.
	@date 09/11/2026
	@since 0.12.26
	@version 0.12.27
	@author Matthew Moore
*/

#ifndef INCLUDE_NATURE_NATURE_HELPERS_H
#define INCLUDE_NATURE_NATURE_HELPERS_H

#include <utility>

#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Registry/natureRegistry.h"
#include "Utility/random.h"

#include "builtInNatureID.h"
#include "natureID.h"

namespace PocketCore::Nature
{
	using PocketCore::Core::us;
	using PocketCore::Registry::Nature::NatureRegistry;
	using PocketCore::Utility::Random;

	/*! @brief Gets a random nature ID from the built-in natures.
		@return The random nature ID.
		@since 0.12.26
		@version 0.12.26
	*/
	ATTR_NODISCARD constexpr NatureID getRandomNatureIDFromBuiltIns()
	{
		return NatureID(
			Random::get<us>(std::to_underlying(BuiltinNatureID::None) + 1, std::to_underlying(BuiltinNatureID::FinalNature) - 1));
	}

	/*! @brief Gets a random nature ID from the given configuration.
		@param[in] config The configuration to get the random nature ID from.
		@return The random nature ID.
		@since 0.12.26
		@version 0.12.27
	*/
	ATTR_NODISCARD constexpr NatureID getRandomNatureIDFromConfiguration(const NatureRegistry &config)
	{
		return NatureID(Random::get<us>(std::to_underlying(BuiltinNatureID::None) + 1, config.getAmountRegistered()));
	}
} // namespace PocketCore::Nature

#endif
