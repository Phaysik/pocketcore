/*! @file randomizationHandler.testHelper.h
	@brief Test helper for dealing with RandomizationHandler concepts.
	@date 10/19/2026
	@since 0.12.51
	@version 0.12.51
	@author Matthew Moore
*/

#ifndef TEST_INCLUDE_EFFECT_HANDLER_RANDOMIZATION_HANDLER_TEST_HELPER_H
#define TEST_INCLUDE_EFFECT_HANDLER_RANDOMIZATION_HANDLER_TEST_HELPER_H

#include <algorithm>
#include <utility>

#include "Effect/effectContext.h"
#include "Multiplier/builtInMultiplierID.h"
#include "Multiplier/multiplierID.h"
#include "Utility/Math/floatUtility.h"

namespace PocketCore::Testing
{
	using PocketCore::Effect::EffectContext;
	using PocketCore::Multiplier::BuiltinMultiplierID;
	using PocketCore::Multiplier::MultiplierID;
	using PocketCore::Multiplier::toMultiplierID;
	using PocketCore::Utility::Math::approximatelyEqualAbsRel;

	constexpr bool hasRandomizationMultiplier(const EffectContext &context, const double expectedValue = 1.0)
	{
		return std::ranges::any_of(context.getActiveMultipliers(), [expectedValue](const std::pair<MultiplierID, double> &multiplierPair) {
			return multiplierPair.first == toMultiplierID(BuiltinMultiplierID::Randomization)
				&& approximatelyEqualAbsRel(multiplierPair.second, expectedValue);
		});
	}

} // namespace PocketCore::Testing

#endif
