#include "Effect/effectContext.h"

#include "Configuration/constants.h"
#include "Multiplier/multiplierID.h"

namespace PocketCore::Effect
{
	using PocketCore::Configuration::BASE_MULTIPLIER_VALUE;
	using PocketCore::Multiplier::MultiplierID;

	void EffectContext::setMultiplier(const MultiplierID multID, const float value)
	{
		// Find if already present
		for (auto &[mid, val] : mActiveMultipliers)
		{
			if (mid == multID)
			{
				mCombinedMultiplier /= val; // Remove old contribution
				val = value;
				mCombinedMultiplier *= val; // Add new contribution
				return;
			}
		}

		mActiveMultipliers.emplace_back(multID, value);
		mCombinedMultiplier *= value;
	}

	void EffectContext::resetMultipliers()
	{
		mActiveMultipliers.clear();
		mCombinedMultiplier = BASE_MULTIPLIER_VALUE;
	}
} // namespace PocketCore::Effect
