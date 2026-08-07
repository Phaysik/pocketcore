#include "Effect/effectContext.h"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <span>
#include <utility>

#include "Configuration/constants.h"
#include "Core/attributeMacros.h"
#include "Core/typedefs.h"
#include "Multiplier/multiplierID.h"

namespace PocketCore::Effect
{
	using PocketCore::Configuration::FIXED_POINT_MULTIPLIER_DENOMINATOR;
	using PocketCore::Configuration::FIXED_POINT_MULTIPLIER_NUMERATOR;
	using PocketCore::Configuration::ROUND_DOWN_HALF_POINT;
	using PocketCore::Configuration::ROUND_DOWN_TOLERANCE;
	using PocketCore::Core::us;
	using PocketCore::Multiplier::MultiplierID;

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

	ATTR_NODISCARD us EffectContext::applyMultiplier(const us baseDamage) const
	{
		us damage{baseDamage};

		for (const auto &[multID, multiplier] : mActiveMultipliers)
		{
			const double normalizedMultiplier{std::max(multiplier, 1.0)};

			const auto roundDownHalfSafe = [](const double value) -> double {
				double integerPart{};
				const double fractionalPart{std::modf(value, &integerPart)};

				if (fractionalPart > ROUND_DOWN_HALF_POINT + ROUND_DOWN_TOLERANCE)
				{
					return integerPart + 1.0;
				}

				return integerPart;
			};

			const double fixedPointValue{
				(FIXED_POINT_MULTIPLIER_NUMERATOR * normalizedMultiplier) / FIXED_POINT_MULTIPLIER_DENOMINATOR,
			};

			damage = static_cast<us>(roundDownHalfSafe(damage * fixedPointValue));

			damage = std::max(damage, static_cast<us>(1));
		}

		return damage;
	}

	ATTR_NODISCARD ATTR_PURE std::span<const std::pair<MultiplierID, double>> EffectContext::getActiveMultipliers() const noexcept
	{
		return mActiveMultipliers;
	}

	void EffectContext::resetMultipliers()
	{
		mActiveMultipliers.clear();
		mBuiltinMultiplierPositions.fill(0U);
	}
} // namespace PocketCore::Effect
