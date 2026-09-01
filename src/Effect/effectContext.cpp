/*! @file effectContext.cpp
	@brief Contains the effect context implementation
	@date 09/01/2026
	@since 0.8.2
	@version 0.12.14
	@author Matthew Moore
*/

#include "Effect/effectContext.h"

#include <algorithm>
#include <cmath>
#include <cstddef>

#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Multiplier/multiplierID.h"
#include "Multiplier/multiplierMeta.h"
#include "Registry/multiplierRegistry.h"

namespace PocketCore::Effect
{
	using PocketCore::Configuration::FIXED_POINT_MULTIPLIER_DENOMINATOR;
	using PocketCore::Configuration::FIXED_POINT_MULTIPLIER_NUMERATOR;
	using PocketCore::Configuration::ROUND_DOWN_HALF_POINT;
	using PocketCore::Configuration::ROUND_DOWN_TOLERANCE;
	using PocketCore::Core::us;
	using PocketCore::Multiplier::MultiplierApplicationPolicy;
	using PocketCore::Multiplier::MultiplierID;
	using PocketCore::Multiplier::MultiplierMeta;
	using PocketCore::Registry::Multiplier::MultiplierRegistry;

	void EffectContext::setMultiplier(const MultiplierID multID, const double value)
	{
		const us multiplierValue{multID.getValue()};
		const bool isBuiltin{multiplierValue < BUILTIN_MULTIPLIER_COUNT};

		if (isBuiltin)
		{
			us &storedPosition{mBuiltinMultiplierPositions.at(multiplierValue)};

			if (storedPosition != 0U)
			{
				mActiveMultipliers.at(static_cast<std::size_t>(storedPosition - 1U)).second = value;
				return;
			}

			if (mActiveMultipliers.empty())
			{
				mActiveMultipliers.reserve(BUILTIN_MULTIPLIER_COUNT);
			}

			mActiveMultipliers.emplace_back(multID, value);
			storedPosition = static_cast<us>(mActiveMultipliers.size());
			return;
		}

		for (auto &[mid, val] : mActiveMultipliers)
		{
			if (mid == multID)
			{
				val = value;
				return;
			}
		}

		mActiveMultipliers.emplace_back(multID, value);
	}

	ATTR_NODISCARD us EffectContext::applyMultiplier(const us baseDamage, const MultiplierRegistry &multiplierRegistry) const
	{
		us damage{baseDamage};
		double otherMultiplierValue{FIXED_POINT_MULTIPLIER_NUMERATOR};

		const auto roundDownHalfSafe = [](const double value) -> double {
			const double floorPart{std::floor(value)};
			const double fractionalPart{value - floorPart};

			// If it is strictly past the half point + tolerance, round up
			if (fractionalPart > ROUND_DOWN_HALF_POINT + ROUND_DOWN_TOLERANCE)
			{
				return floorPart + 1.0;
			}

			// Otherwise, safely round down to the floor
			return floorPart;
		};

		for (const auto &[multID, multiplier] : mActiveMultipliers)
		{
			const double normalizedMultiplier{std::isfinite(multiplier) ? std::max(multiplier, 0.0) : 1.0};

			const MultiplierMeta *multiplierMeta{multiplierRegistry.getMultiplierMetadata(multID)};
			const MultiplierApplicationPolicy policy{
				multiplierMeta != nullptr ? multiplierMeta->mApplicationPolicy : MultiplierApplicationPolicy::RoundHalfDown,
			};

			switch (policy)
			{
				case MultiplierApplicationPolicy::RoundHalfDown:
					damage = static_cast<us>(roundDownHalfSafe(damage * normalizedMultiplier));
					break;
				case MultiplierApplicationPolicy::Floor:
					damage = static_cast<us>(std::floor(damage * normalizedMultiplier));
					break;
				case MultiplierApplicationPolicy::Other:
				default:
					otherMultiplierValue = std::round(otherMultiplierValue * normalizedMultiplier);
					break;
			}

			damage = std::max(damage, static_cast<us>(1));
		}

		otherMultiplierValue /= FIXED_POINT_MULTIPLIER_DENOMINATOR;

		return std::max(static_cast<us>(roundDownHalfSafe(damage * otherMultiplierValue)), static_cast<us>(1));
	}

	void EffectContext::resetMultipliers()
	{
		mActiveMultipliers.clear();
		mBuiltinMultiplierPositions.fill(0U);
	}
} // namespace PocketCore::Effect
