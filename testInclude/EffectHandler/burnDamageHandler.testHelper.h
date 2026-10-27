/*! @file burnDamageHandler.testHelper.h
	@brief Test helper for dealing with BurnDamageHandler concepts.
	@date 10/27/2026
	@since 0.12.53
	@version 0.12.53
	@author Matthew Moore
*/

#ifndef TEST_INCLUDE_EFFECT_HANDLER_BURN_DAMAGE_HANDLER_TEST_HELPER_H
#define TEST_INCLUDE_EFFECT_HANDLER_BURN_DAMAGE_HANDLER_TEST_HELPER_H

#include <algorithm>
#include <utility>

#include "Configuration/constants.h"
#include "Effect/effectContext.h"
#include "Multiplier/builtInMultiplierID.h"
#include "Multiplier/multiplierID.h"
#include "Utility/Math/floatUtility.h"

namespace PocketCore::Testing
{
	using PocketCore::Configuration::BURN_MULTIPLIER_VALUE;
	using PocketCore::Effect::EffectContext;
	using PocketCore::Multiplier::BuiltinMultiplierID;
	using PocketCore::Multiplier::MultiplierID;
	using PocketCore::Multiplier::toMultiplierID;
	using PocketCore::Utility::Math::approximatelyEqualAbsRel;

	constexpr bool hasBurnDamageMultiplier(const EffectContext &context)
	{
		return std::ranges::any_of(context.getActiveMultipliers(), [](const std::pair<MultiplierID, double> &multiplierPair) {
			return multiplierPair.first == toMultiplierID(BuiltinMultiplierID::Burn)
				&& approximatelyEqualAbsRel(multiplierPair.second, BURN_MULTIPLIER_VALUE);
		});
	}

} // namespace PocketCore::Testing

#endif
